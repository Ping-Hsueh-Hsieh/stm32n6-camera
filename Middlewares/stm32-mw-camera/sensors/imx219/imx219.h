/**
  ******************************************************************************
  * @file    imx219.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the imx219.c
  *          driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IMX219_H
#define IMX219_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include "imx219_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup imx219
  * @{
  */

/** @defgroup IMX219_Exported_Types
  * @{
  */

typedef int32_t (*IMX219_Init_Func)(void);
typedef int32_t (*IMX219_DeInit_Func)(void);
typedef int32_t (*IMX219_GetTick_Func)(void);
typedef int32_t (*IMX219_WriteReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*IMX219_ReadReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  IMX219_Init_Func Init;
  IMX219_DeInit_Func DeInit;
  uint16_t Address;
  IMX219_WriteReg_Func WriteReg;
  IMX219_ReadReg_Func ReadReg;
  IMX219_GetTick_Func GetTick;
} IMX219_IO_t;

typedef struct
{
  IMX219_IO_t IO;
  imx219_ctx_t Ctx;
  uint8_t IsInitialized;
} IMX219_Object_t;

typedef struct
{
  uint32_t Config_Resolution;
  uint32_t Config_LightMode;
  uint32_t Config_SpecialEffect;
  uint32_t Config_Brightness;
  uint32_t Config_Saturation;
  uint32_t Config_Contrast;
  uint32_t Config_HueDegree;
  uint32_t Config_Gain;
  uint32_t Config_Exposure;
  uint32_t Config_MirrorFlip;
  uint32_t Config_Zoom;
  uint32_t Config_NightMode;
  uint32_t Config_ExposureMode;
  uint32_t Config_SensorInfo;
  uint32_t Config_TestPattern;
} IMX219_Capabilities_t;

/**
  * @}
  */

/** @defgroup IMX219_Exported_Constants
  * @{
  */
#define IMX219_OK (0)
#define IMX219_ERROR (-1)
/**
 * @brief  IMX219 Features Parameters
 */
/* Camera resolutions */
#define IMX219_R3280_2464 0U /* 3280x2464 Resolution       */
#define IMX219_R1920_1080 1U /* 1920x1080 Resolution       */
#define IMX219_R1640_1232 2U /* 1640x1232 Resolution       */
#define IMX219_R640_480 3U   /* 640x480 Resolution       */

#define IMX219_RESOLUTION IMX219_R1920_1080

#if (IMX219_RESOLUTION == IMX219_R3280_2464)
#define IMX219_WIDTH              3280
#define IMX219_HEIGHT             2464
#define IMX219_BINNING_FAC        1
#elif (IMX219_RESOLUTION == IMX219_R1920_1080)
#define IMX219_WIDTH              1920
#define IMX219_HEIGHT             1080
#define IMX219_BINNING_FAC        1
#elif (IMX219_RESOLUTION == IMX219_R1640_1232)
#define IMX219_WIDTH              1640
#define IMX219_HEIGHT             1232
#define IMX219_BINNING_FAC        2
#elif (IMX219_RESOLUTION == IMX219_R640_480)
#define IMX219_WIDTH              640
#define IMX219_HEIGHT             480
#define IMX219_BINNING_FAC        1
#endif

/* Camera Pixel Format */
#define IMX219_RAW_RGGB10 10U /* Pixel Format RAW_RGGB10    */

/* Mirror/Flip */
#define IMX219_MIRROR_FLIP_NONE 0x00U /* Set camera normal mode     */
#define IMX219_FLIP 0x01U             /* Set camera flip config     */
#define IMX219_MIRROR 0x02U           /* Set camera mirror config   */
#define IMX219_MIRROR_FLIP 0x03U      /* Set camera mirror and flip */

/**
  * @}
  */

/** @defgroup IMX219_Exported_Functions IMX219 Exported Functions
  * @{
  */
int32_t IMX219_RegisterBusIO(IMX219_Object_t* pObj, IMX219_IO_t* pIO);
int32_t IMX219_Init(IMX219_Object_t* pObj, uint32_t Resolution, uint32_t PixelFormat);
int32_t IMX219_Start(IMX219_Object_t* pObj);
int32_t IMX219_DeInit(IMX219_Object_t* pObj);
int32_t IMX219_ReadID(IMX219_Object_t* pObj, uint32_t* Id);
int32_t IMX219_GetCapabilities(IMX219_Object_t* pObj, IMX219_Capabilities_t* Capabilities);
int32_t IMX219_SetGain(IMX219_Object_t* pObj, int32_t gain);
int32_t IMX219_SetExposure(IMX219_Object_t* pObj, int32_t exposure);
int32_t IMX219_SetFrequency(IMX219_Object_t* pObj, int32_t frequency);
int32_t IMX219_SetFramerate(IMX219_Object_t* pObj, int32_t framerate);
int32_t IMX219_MirrorFlipConfig(IMX219_Object_t* pObj, uint32_t Config);
int32_t IMX219_SetTestPattern(IMX219_Object_t* pObj, int32_t mode);

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
