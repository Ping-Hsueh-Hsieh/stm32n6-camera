set -eu # Exit on any error, Exit on unset variable

$STEDGEAI_CORE_BIN generate \
    --type onnx \
    --model ./yolov8n.onnx \
    --target stm32n6 \
    --st-neural-art default@user_neuralart_NUCLEO-N657X0-Q.json \
    --optimization balanced \
    --compression lossless \
    --c-api st-ai \
    --workspace st_yolo_ws \
    --lib $STEDGEAI_CORE_LIB \
    --output st_yolo_output

mv st_yolo_output/network.c NUCLEO-N657X0-Q/network.c
mv st_yolo_output/network.h NUCLEO-N657X0-Q/network.h
mv st_yolo_output/network_ecblobs.h NUCLEO-N657X0-Q/network_ecblobs.h
mv st_yolo_output/stai_network.c NUCLEO-N657X0-Q/stai_network.c
mv st_yolo_output/stai_network.h NUCLEO-N657X0-Q/stai_network.h
mv st_yolo_output/network_atonbuf.xSPI2.raw NUCLEO-N657X0-Q/network_atonbuf.xSPI2.bin

arm-none-eabi-objcopy \
    -I binary NUCLEO-N657X0-Q/network_atonbuf.xSPI2.bin \
    --change-addresses 0x70380000 \
    -O ihex NUCLEO-N657X0-Q/network_atonbuf.hex

rm -rf ./st_yolo_output
rm -rf ./st_yolo_ws
