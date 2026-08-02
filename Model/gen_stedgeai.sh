#!/usr/bin/env zsh

stedgeai analyze -m /path/to/model.tflite --target stm32
stedgeai generate -m /path/to/model.tflite --target stm32 -o ./st_output
stedgeai validate -m /path/to/model.tflite --target stm32 --mode host
stedgeai validate -m /path/to/model.tflite --target stm32 --mode target -d serial:/dev/tty.usbmodemXXXX


stedgeai analyze \
    --type onnx \
    --model ./yolov8n_int8.onnx \
    --target stm32n6 \
    --st-neural-art \
    --optimization balanced \
    --compression lossless \
    --c-api st-ai \
    --verbosity 3

stedgeai generate \
    --type onnx \
    --model ./yolov8n.onnx \
    --target stm32n6 \
    --st-neural-art \
    --optimization balanced \
    --compression lossless \
    --memory-pool ./stm32n6-app2_NUCLEO-N657X0-Q.mpool \
    --c-api st-ai \
    --workspace st_yolo_ws \
    --output st_yolo_output

usage: stedgeai --model FILE --target TARGET --device DEVICE
                [--type keras|onnx|tflite] [--name STR]
                [--compression none|lossless|low|medium|high]
                [--allocate-inputs] [--allocate-outputs]
                [--no-inputs-allocation] [--no-outputs-allocation]
                [--input-memory-alignment INT] [--output-memory-alignment INT]
                [--workspace DIR] [--output DIR] [--split-weights]
                [--optimization OBJ] [--memory-pool FILE]
                [--no-onnx-optimizer] [--use-onnx-simplifier]
                [--fix-parametric-shapes FIX_PARAMETRIC_SHAPES]
                [--input-data-type float32|int8|uint8]
                [--output-data-type float32|int8|uint8]
                [--inputs-ch-position chfirst|chlast]
                [--outputs-ch-position chfirst|chlast]
                [--prefetch-compressed-weights] [--custom FILE]
                [--c-api st-ai|legacy] [--cut-input-tensors CUT_INPUT_TENSORS]
                [--cut-output-tensors CUT_OUTPUT_TENSORS]
                [--cut-input-layers CUT_INPUT_LAYERS]
                [--cut-output-layers CUT_OUTPUT_LAYERS]
                [--allocate-activations] [--allocate-states] [--hsp [HSP]]
                [--st-neural-art [ST_NEURAL_ART]] [--binary] [--dll]
                [--address ADDR] [--copy-weights-at ADDR]
                [--relocatable [STR]] [--enable-epoch-controller] [--lib DIR]
                [--batch-size INT]
                [--mode host|target|host-io-only|target-io-only] [--desc DESC]
                [--val-json FILE] [--valinput FILE [FILE ...]]
                [--valoutput FILE [FILE ...]] [--range MIN MAX [MIN MAX ...]]
                [--full] [--io-only] [--save-csv] [--classifier] [--no-check]
                [--no-exec-model] [--seed SEED] [--ispu-conf FILE]
                [--with-report] [--no-report] [--no-workspace] [-h]
                [--version] [--tools-version] [--verbosity [0|1|2|3]]
                [--quiet]
                analyze|generate|validate|supported-ops

ST Edge AI Core v4.0.1-20581 7ed50de05
   ISPU 2.0.1-RC2
   MLC 1.2.4-RC2
   STM32CubeAI 12.0.1-RC2

command:
  analyze|generate|validate|supported-ops
                        must be the first argument (default: analyze)
                        analyze
                        	check if the model is supported and get information about the model
                        	architecture and memory footprint to know if the generated code can be
                        	deployed on the target device
                        validate
                        	validate the converted model by itself or versus the original model
                        generate
                        	generate the converted model for the target device
                        supported-ops
                        	list the supported operators

common options:
  --model FILE, -m FILE
                        paths of the original model files
  --target TARGET       target selector - available targets are
                        ispu --device DEVICE
                        mlc
                        stm32n6 [--st-neural-art]
                        stm32wl
                        stm32u5
                        stm32l5
                        stm32h5
                        stm32c5
                        stm32mp157c
                        stm32f4
                        stm32l4
                        stm32l4p
                        stm32g4
                        stm32f3
                        stm32wb
                        stm32v8
                        stm32h7
                        stm32f7
                        stm32u3 [--hsp]
                        stm32f0
                        stm32l0
                        stm32g0
                        stm32c0
  --device DEVICE       device selector - available devices are
                        ism330is
                        lsm6dso16is
                        imu_22 (valid for either ISM330IS or LSM6DSO16IS)
                        iis3dwb10is
  --type keras|onnx|tflite, -t keras|onnx|tflite
                        indicate/force the model type to use
  --name STR, -n STR    name of the generated C-model (default: network)
  --compression none|lossless|low|medium|high, -c none|lossless|low|medium|high
                        compression level (default: lossless)
  --allocate-inputs     (deprecated) reserve a space in the "activations" buffer to
                        store the input buffers.
                        (default: input buffers are stored in a reserved space within the "activations" buffer)
  --allocate-outputs    (deprecated) reserve a space in the "activations" buffer to
                        store the output buffers.
                        (default: output buffers are stored in a reserved space within the "activations" buffer)
  --no-inputs-allocation
                        input buffers must be provided by the application.
                        (default: input buffers are stored in a reserved space within the "activations" buffer)
  --no-outputs-allocation
                        output buffers must be provided by the application.
                        (default: output buffers are stored in a reserved space within the "activations" buffer)
  --input-memory-alignment INT
                        input data alignment (to be provided only when input buffers are stored in a reserved
                        space within the "activations" buffer)
                        (default: default value depends on the target)
  --output-memory-alignment INT
                        output data alignment (to be provided only when output buffers are stored in a reserved
                        space within the "activations" buffer)
                        (default: default value depends on the target)
  --workspace DIR, -w DIR
                        workspace folder to use (default: st_ai_ws)
  --output DIR, -o DIR  folder where the generated files are saved (default: st_ai_output)
  --split-weights       generate a C-data file with a C-array per tensor
                        (default: single C-array for all tensors)
  --optimization OBJ, -O OBJ
                        define the global optimization objectives
                            'time': optimize the inference time
                            'ram': minimize the size of the require ram
                            'balanced': default, trade-off between the inference time and ram size
  --memory-pool FILE, --memory_pool FILE
                        path of the memory pool descriptor file to describe
                        the memory regions which can be used to support the
                        multi-heaps/activations UCs.
                        (default: infinite SRAM is considered)
  --no-onnx-optimizer   disable the ONNX optimizer pass (default: False)
  --use-onnx-simplifier
                        Enable the ONNX simplifier (default: False)
  --fix-parametric-shapes FIX_PARAMETRIC_SHAPES
                        Set parametric dimensions in the shapes of the input tensors
  --input-data-type float32|int8|uint8
                        indicate the expected inputs data type of the generated model
                        Multiple inputs: in_data_type_1,in_data_type_2,...
                        If one data type is given, it will be applied for all inputs
  --output-data-type float32|int8|uint8
                        indicate the expected outputs data type of the generated model
                        Multiple outputs: out_data_type_1,out_data_type_2,...
                        If one data type is given, it will be applied for all outputs
  --inputs-ch-position chfirst|chlast
                        indicate the expected channel position
                        for the input tensors.
  --outputs-ch-position chfirst|chlast
                        indicate the expected channel position
                        for the output tensors.
  --prefetch-compressed-weights
                        (experimental) enable the prefetch of the compressed x4,x8 weights
                        (extra space will be reserved in the activations buffer)
  --custom FILE         path of the configuration file for the custom layers (JSON format)
  --c-api st-ai|legacy  select the embedded c-api of the generated model
                        (use --target to get the default and availability for each target)
  --cut-input-tensors CUT_INPUT_TENSORS
                        specify the input tensors by which to cut the input model
                        (default: None)
  --cut-output-tensors CUT_OUTPUT_TENSORS
                        specify the output tensors by which to cut the input model
                        (default: None)
  --cut-input-layers CUT_INPUT_LAYERS
                        specify the input layers by which to cut the input model
                        (default: None)
  --cut-output-layers CUT_OUTPUT_LAYERS
                        specify the output layers by which to cut the input model
                        (default: None)
  --allocate-activations
                        (experimental) indicate that the memory buffers requested to store the activations
                        are allocated in the runtime space. Only supported by the c-api 'st-ai'.
                        (default: must be provided by the application)
  --allocate-states     (experimental) indicate that the memory buffers to store the states
                        are allocated in the runtime space. Only supported by the c-api 'st-ai'
                        (default: must be provided by the application)
  --hsp [HSP]           reserved size of the BRAM (in word, default: 4096) to
                        enable the support of the Hardware Signal Processing (HSP) unit
  --st-neural-art [ST_NEURAL_ART]
                        launch the Neural Art compiler with options given as arguments

specific generate options:
  --binary              generate model weights as a binary file
  --dll                 generate the x86 dynamic library
  --address ADDR        address of the weights array (can be external memory)
  --copy-weights-at ADDR, --copy-weight-at ADDR
                        include code to copy the weights to the specified address
  --relocatable [STR], -r [STR], --reloc [STR]
                        generate the model as a runtime loadable model (relocatable).
                        Extra arguments can be added (comma separated) to tune the
                        relocatable model generation:
                            split: separated binary file is generated for weights/params
                            gen-c-files: generate C files to be compiled separately
                            no-secure: non-secure mode generation
                            (see documentation for more details)
  --enable-epoch-controller, --eec
                        Use the epoch controller feature of Neural-Art NPU if it is used.
  --lib DIR, -l DIR     root dir of the AI runtime libraries

specific validate options:
  --batch-size INT, -b INT
                        number of samples for the validation
  --mode host|target|host-io-only|target-io-only
                        validation mode to use
  --desc DESC, -d DESC  
                        COM port and baud rate to use for communication with the board. Syntax:
                            serial[:COMPORT][:baudrate]
  --val-json FILE       json file to use for validation
  --valinput FILE [FILE ...], -vi FILE [FILE ...]
                        files containing data to use as input for validation
  --valoutput FILE [FILE ...], -vo FILE [FILE ...]
                        files containing data to use as reference output for validation
  --range MIN MAX [MIN MAX ...]
                        range of values to generate the random input data (default: [0 1])
  --full                (deprecated) enable the full validation process extracting the error per model layer
  --io-only             disable profiling per layer
  --save-csv            force the storage of all io data in csv files
  --classifier          specify that the model is a classifier (otherwise auto-detection is performed)
  --no-check            disable internal checks (mode/model type dependent)
  --no-exec-model       disable execution of the original model
  --seed SEED           random seed used to initialize the pseudo-random number generator [0, 2^32 - 1]
  --ispu-conf FILE      ISPU configuration file (JSON format) of the model to be validated on target

specific supported-ops options:
  --with-report         generate report (markdown format) with detailed info of the supported operators

additional options:
  --no-report           do not generate the report file
  --no-workspace        do not create the workspace folder
  -h, --help            show this help message and exit (use --target to get target specific help)
  --version             print the version of the tool
  --tools-version       print the versions of the third party packages used by the tool
  --verbosity [0|1|2|3], -v [0|1|2|3], --verbose [0|1|2|3]
                        set verbosity level
  --quiet               disable the progress-bar

examples:
  stedgeai analyze  --target <target> -m mymodel.h5
  stedgeai validate --target <target> -m mymodel.h5 --mode host
  stedgeai validate --target <target> -m mymodel.h5 -vi test_data.csv
  stedgeai generate --target <target> -m mymodel.h5 -o output_dir
  stedgeai supported-ops [--type tflite|keras|onnx]
