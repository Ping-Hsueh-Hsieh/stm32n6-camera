# Goal

Make IMX219 with this example.

# TODO

- [x] IMX219(DMCIPP2): flip the image
- [x] UVC(DMCIPP1): adjust the right x, y, w, h
- [ ] Use STM32 ISP IQTune.
- [ ] Fix exposure problem ISP_MW_SW_AEC_ALGO_SUPPORT
- [ ] Change nn with Edge_AI

# IMX219 configuration

Thanks to [decode from raspberry pi](https://forum.arduino.cc/t/raspberry-pi-camera-v2-1/538817/7), the image is quite stable.

# STM AWB/AEC tool

- STM32 ISP IQTune
    - [overview](https://www.st.com/en/development-tools/stm32-isp-iqtune.html#)
    - [how to tune](https://wiki.st.com/stm32mcu/wiki/ISP:How_to_tune_ISP_using_the_STM32_ISP_IQTune)
    - [stm32-mw-isp](https://github.com/STMicroelectronics/stm32-mw-isp)
    - [x-cube-isp](https://github.com/STMicroelectronics/x-cube-isp)

- [Someone tried to do it.](https://community.st.com/stm32-mpus-embedded-software-and-solutions-46/unable-to-use-dcmipp-isp-ctrl-tool-at-runtime-with-imx219-sensor-on-stm32mp257-ev1-156678), but no luck.

# Original Repo from ST

[STM32N6-GettingStarted-ImageClassification](https://github.com/STMicroelectronics/STM32N6-GettingStarted-ImageClassification?tab=License-1-ov-file)
