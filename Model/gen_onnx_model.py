from ultralytics import YOLO
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
