cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(CMAKE_C_FLAGS   "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Prevent macOS host arch flags from leaking in
set(CMAKE_OSX_ARCHITECTURES "")
set(CMAKE_OSX_SYSROOT "")

# STM32H7 core flags
set(MCU_FLAGS "-mcpu=cortex-m55 -mthumb")

set(FPU_FLAGS "-mfpu=fpv5-d16 -mfloat-abi=hard")

set(COMMON_FLAGS "${MCU_FLAGS} ${FPU_FLAGS} -ffunction-sections -fdata-sections")

set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS_INIT "${MCU_FLAGS} ${FPU_FLAGS}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

project(can-test LANGUAGES C VERSION 0.1)

####################################################################################################
### MAIN
####################################################################################################

set(BOARD "NUCLEO-N657X0-Q")
file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Application/${BOARD}/*.c")
file(GLOB_RECURSE MODEL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Model/${BOARD}/*.c")
file(GLOB_RECURSE FW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/*.c")
file(GLOB_RECURSE MIDDLEWARE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/*.c")

add_executable(${PROJECT_NAME}
    ${APP_SRCS}
    ${MODEL_SRCS}
    ${FW_SRCS}
    ${MIDDLEWARE_SRCS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    STM32N657xx
    STM32N6570_NUCLEO_REV
    SCR_LIB_USE_UVCL
    SCR_LIB_USE_NONE
    UVC_LIB_USE_USBX
    UX_INCLUDE_USER_DEFINE_FILE
    USBL_PACKET_PER_MICRO_FRAME=3
    UX_STANDALONE
    USE_FULL_ASSERT
    USE_FULL_LL_DRIVER
    VECT_TAB_SRAM
    LL_ATON_DUMP_DEBUG_API
    LL_ATON_PLATFORM=LL_ATON_PLAT_STM32N6
    LL_ATON_OSAL=LL_ATON_OSAL_BARE_METAL
    LL_ATON_RT_MODE=LL_ATON_RT_ASYNC
    LL_ATON_SW_FALLBACK
    LL_ATON_DBG_BUFFER_INFO_EXCLUDED=1
    __ARM_FEATURE_CMSE=3
    __ARM_ARCH_PROFILE='M'
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application/NUCLEO-N657X0-Q/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/Src
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/ili9341
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/uvcl/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/uvcl/Inc/usbd
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/uvcl/Inc/usbx
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/uvcl/Src
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/uvcl/Src/usbd
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/screenl/uvcl/Src/usbx
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Misc/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Npu/Devices/SR6P3E
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Npu/Devices/SR6P3E/ATON-idxs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Npu/Devices/STM32H7xx
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Npu/Devices/STM32H7xx/ATON-idxs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Npu/Devices/STM32N6xx
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Npu/Devices/STM32N6xx/ATON-idxs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Npu/ll_aton
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/Reloc/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stedgeai-lib/SystemPerformance/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/ISP_Library/isp/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/ISP_Library/isp/USBX/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/ISP_Library/isp_param_conf
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/sensors
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/sensors/imx335
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/sensors/imx219
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/sensors/ov5640
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/sensors/vd1943
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/sensors/vd55g1
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/stm32-mw-camera/sensors/vd6g
    ${CMAKE_CURRENT_SOURCE_DIR}/Model/NUCLEO-N657X0-Q
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/BSP/Components/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/BSP/Components/aps256xx
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/BSP/Components/mx25um51245g
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/BSP/Components/mx66uw1g45g
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/BSP/Components/rk050hr18
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/BSP/STM32N6xx_Nucleo
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core/Include/a-profile
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core/Include/m-profile
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core/Include/r-profile
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core/Template/Device_A/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core/Template/Device_A/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core/Template/Device_M/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core/Template/Device_M/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core_A/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Core_A/Include/a-profile
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/ComputeLibrary/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/ARM/arm_graphic_equalizer_example
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/ARM/arm_linear_interp_example
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/ARM/arm_matrix_example
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/ARM/arm_signal_converge_example
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/RTE/Device/SSE-300-MPS3
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/RTE/Device/SSE-310-MPS3
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/projects/RTE/Device/ARMCM0P
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/projects/RTE/Device/ARMCM7_DP
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/projects/RTE/Device/SSE_300_MPS3
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/projects/RTE/Device/SSE_310_MPS3
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/projects/RTE/_Release_VHT-Corstone-300
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/projects/RTE/_Release_VHT-Corstone-310
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/projects/RTE/_Release_VHT_M0P
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Examples/cmsis_build/projects/RTE/_Release_VHT_M7
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Include/dsp
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/PrivateInclude
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/DSP/Source/DistanceFunctions
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Device/ST/STM32N6xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Device/ST/STM32N6xx/Include/Templates
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Include/m-profile
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/Include/r-profile
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/CMSIS/RTOS2/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/STM32N6xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Drivers/STM32N6xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/common/core/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/common/usbx_device_classes/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/common/usbx_host_classes/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/common/usbx_host_controllers/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/common/usbx_network/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/common/usbx_pictbridge/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/common/usbx_stm32_device_controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/common/usbx_stm32_host_controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_a7/gnu/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_a7/iar/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_m0/gnu/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_m0/iar/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_m3/gnu/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_m3/iar/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_m4/gnu/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_m4/iar/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_m7/gnu/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/cortex_m7/iar/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/generic/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/ports/linux/gnu/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/test/cmake/libs
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/test/cmake/usbx
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Middlewares/ST/usbx/test/regression
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Utilities/Fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW_N6/Utilities/lcd
)

# target_link_directories(${PROJECT_NAME}
#     PRIVATE
# )

# target_link_libraries(${PROJECT_NAME}
#     PRIVATE
# )

# set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
