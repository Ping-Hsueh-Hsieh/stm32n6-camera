from ultralytics import YOLO
import numpy as np
import onnx
from onnx import helper, numpy_helper

model = YOLO("yolov8n.pt")
model.export(
    format="onnx",
    quantize=8,  # 16, 8, 32
    imgsz=96,
    opset=12,
    simplify=True,
    dynamic=False,
    nms=False
)

# ---------------------------------------------------------------------------
# Fix the output quantization of the int8 QDQ export.
#
# The exported ONNX quantizes the whole [84, 189] output tensor with a single
# scale/offset sized for the box rows (range ~0..132 px, hence scale ~0.5163).
# The class-score rows (sigmoid, range [0, 1]) then collapse to a handful of
# levels (0 / 0.516 / 1.03), which destroys detection confidence.
#
# Fix: normalize the box branch to [0, 1] BEFORE the final Concat and shrink
# the output quantization scale to 1/256. Both boxes and scores now share the
# [0, 1] range and get full 8-bit resolution. The application restores pixel
# coordinates by multiplying the dequantized boxes by 255.
# ---------------------------------------------------------------------------
def normalize_boxes(path, out_path):
    m = onnx.load(path)
    g = m.graph

    concat = None
    for n in g.node:
        if n.op_type == "Concat" and n.output[0] == "output0_QuantizeLinear_Input":
            concat = n
            break
    assert concat is not None, "final Concat not found"

    box_in = concat.input[0]
    norm = numpy_helper.from_array(np.array(1.0 / 255.0, np.float32), name="box_norm_scale")
    box_out = box_in + "_box_normalized"
    g.node.append(helper.make_node("Mul", [box_in, "box_norm_scale"], [box_out], name="box_normalize"))
    concat.input[0] = box_out
    g.initializer.append(norm)

    for init in g.initializer:
        if init.name == "output0_scale":
            a = onnx.numpy_helper.to_array(init)
            a[...] = 1.0 / 256.0
            init.CopyFrom(numpy_helper.from_array(a, "output0_scale"))
        elif init.name == "output0_zero_point":
            a = onnx.numpy_helper.to_array(init)
            a[...] = -128
            init.CopyFrom(numpy_helper.from_array(a, "output0_zero_point"))

    onnx.save(m, out_path)

normalize_boxes("yolov8n_int8.onnx", "yolov8n_boxnorm.onnx")
print("wrote yolov8n_boxnorm.onnx")
