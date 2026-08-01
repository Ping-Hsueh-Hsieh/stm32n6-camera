 /**
 ******************************************************************************
 * @file    app_config.h
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef APP_CONFIG
#define APP_CONFIG

#include "stai_network.h"
#include "arm_math.h"

#define USE_DCACHE

/*Defines: CMW_MIRRORFLIP_NONE; CMW_MIRRORFLIP_FLIP; CMW_MIRRORFLIP_MIRROR; CMW_MIRRORFLIP_FLIP_MIRROR;*/
#define CAMERA_FLIP CMW_MIRRORFLIP_MIRROR

/* Model Related Info */
#define POSTPROCESS_TYPE POSTPROCESS_SSEG_DEEPLAB_V3_UI

#define ASPECT_RATIO_CROP       (1) /* Crop both pipes to nn input aspect ratio; Original aspect ratio kept */
#define ASPECT_RATIO_FIT        (2) /* Resize both pipe to NN input aspect ratio; Original aspect ratio not kept */
#define ASPECT_RATIO_FULLSCREEN (3) /* Resize camera image to NN input size and display a maximized image. See Doc/Build-Options.md#aspect-ratio-mode */
#define ASPECT_RATIO_MODE ASPECT_RATIO_FULLSCREEN

#define COLOR_BGR (0)
#define COLOR_RGB (1)
#define COLOR_MODE COLOR_RGB

#define NB_CLASSES 2

/* I/O configuration */
#define AI_SSEG_DEEPLABV3_PP_WIDTH            (STAI_NETWORK_IN_1_WIDTH)
#define AI_SSEG_DEEPLABV3_PP_HEIGHT           (STAI_NETWORK_IN_1_HEIGHT)
#define AI_SSEG_DEEPLABV3_PP_NB_CLASSES       (NB_CLASSES)

/* Display */
#define WELCOME_MSG_1         "deeplab_v3_mobilenetv2_05_16_256_asppv2_qdq_int8.onnx"
#define WELCOME_MSG_2         ((char *[2]) {"Model Running in STM32 MCU", "internal memory"})

#endif
