# diagnose_onnx_missing_inputs.py
import onnx, onnxsim
from collections import defaultdict

model = onnx.load("./yolov8n.onnx")   # change path if needed

def validate_1():
    _m_simp, check = onnxsim.simplify(model)
    print("simplified:", check)

def validate_2():
    graph = model.graph

    # names available before nodes run: graph inputs and initializers
    available = set(inp.name for inp in graph.input)
    available.update(init.name for init in graph.initializer)

    # Map outputs -> node index
    output_producer = {}
    for i, n in enumerate(graph.node):
        for out in n.output:
            output_producer[out] = i

    missing = defaultdict(list)  # missing_name -> list of nodes that reference it

    for i, n in enumerate(graph.node):
        for inp in n.input:
            if inp == "": 
                continue
            if inp not in available and inp not in output_producer:
                missing[inp].append((i, n.name if n.name else n.op_type))

    if missing:
        print("Missing inputs referenced by nodes:")
        for name, refs in missing.items():
            print(f" - {name}:")
            for idx, node_name in refs:
                print(f"     node[{idx}] op={node_name}")
    else:
        print("No missing inputs detected.")

    # Also locate any node that outputs the suspicious name if present
    suspects = [n for n in graph.node if any(o.startswith("graph_input_cast") for o in n.output)]
    if suspects:
        print("\nNodes that output names starting with graph_input_cast:")
        for n in suspects:
            print(f" - op: {n.op_type}, name: {n.name}, outputs: {list(n.output)}")

if __name__ == '__main__':
    validate_1()
    validate_2()
