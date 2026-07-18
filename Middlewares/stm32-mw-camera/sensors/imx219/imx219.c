/**
  ******************************************************************************
  * @file    imx219.c
  * @author  MCD Application Team
  * @brief   This file provides the IMX219 camera driver
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

/* Includes ------------------------------------------------------------------*/
#include "imx219.h"
#include <string.h>
#include "util.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup IMX219
  * @brief     This file provides a set of functions needed to drive the
  *            IMX219 Camera module.
  * @{
  */

/** @defgroup IMX219_Private_TypesDefinitions Private Types definition
  * @{
  */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* Mode : resolution and related config&values */
struct imx219_mode
{
  /* Frame width */
  unsigned int width;
  /* Frame height */
  unsigned int height;

  /* V-timing */
  unsigned int fll_def;
};

/* Mode configs */
static const struct imx219_mode supported_modes[] = {
  {
    /* 8MPix 15fps mode */
    .width = 3280,
    .height = 2464,
    .fll_def = 3526,
  },
  {
    /* 1080P 30fps cropped */
    .width = 1920,
    .height = 1080,
    .fll_def = 1763,
  },
  {
    /* 2x2 binned 60fps mode */
    .width = 1640,
    .height = 1232,
    .fll_def = 1707,
  },
  {
    /* 640x480 60fps mode */
    .width = 640,
    .height = 480,
    .fll_def = 1707,
  },
};

/** @defgroup IMX219_Private_Constants Private Constants
  * @{
  */
struct regval
{
  uint16_t addr;
  uint8_t val;
};

const uint16_t exck_freq = IMX219_EXCK_FREQ(IMX219_XCLK_FREQ / 1000000);
#define EXCK_FREQ_LO (uint8_t)(exck_freq & 0x00FF)
#define EXCK_FREQ_HI (uint8_t)(exck_freq >> 8)

static const struct regval imx219_common_regs[] = {
  {IMX219_REG_MODE_SELECT, 0x00}, /* Mode Select */

  /* To Access Addresses 3000-5fff, send the following commands */
  {CCI_REG8(0x30eb), 0x05},
  {CCI_REG8(0x30eb), 0x0c},
  {CCI_REG8(0x300a), 0xff},
  {CCI_REG8(0x300b), 0xff},
  {CCI_REG8(0x30eb), 0x05},
  {CCI_REG8(0x30eb), 0x09},

  /* Undocumented registers */
  {CCI_REG8(0x455e), 0x00},
  {CCI_REG8(0x471e), 0x4b},
  {CCI_REG8(0x4767), 0x0f},
  {CCI_REG8(0x4750), 0x14},
  {CCI_REG8(0x4540), 0x00},
  {CCI_REG8(0x47b4), 0x14},
  {CCI_REG8(0x4713), 0x30},
  {CCI_REG8(0x478b), 0x10},
  {CCI_REG8(0x478f), 0x10},
  {CCI_REG8(0x4793), 0x10},
  {CCI_REG8(0x4797), 0x0e},
  {CCI_REG8(0x479b), 0x0e},

  /* Frame Bank Register Group "A" */
  {IMX219_REG_X_ODD_INC_A, 1},
  {IMX219_REG_Y_ODD_INC_A, 1},

  /* Output setup registers */
  {IMX219_REG_DPHY_CTRL, IMX219_DPHY_CTRL_TIMING_MANUAL},
  {IMX219_REG_EXCK_FREQ_HI, EXCK_FREQ_HI},
  {IMX219_REG_EXCK_FREQ_LO, EXCK_FREQ_LO},
};

static const struct regval imx219_2lane_regs[] = {
  /* PLL Clock Table */
  {IMX219_REG_VTPXCK_DIV, 10},
  {IMX219_REG_VTSYCK_DIV, 1},
  {IMX219_REG_PREPLLCK_VT_DIV, 3}, /* 0x03 = AUTO set */
  {IMX219_REG_PREPLLCK_OP_DIV, 3}, /* 0x03 = AUTO set */
  {IMX219_REG_PLL_VT_MPY_HI, 0},
  {IMX219_REG_PLL_VT_MPY_LO, 57},
  {IMX219_REG_OPSYCK_DIV, 1},
  {IMX219_REG_PLL_OP_MPY_HI, 0},
  {IMX219_REG_PLL_OP_MPY_LO, 57},

  /* 2-Lane CSI Mode */
  {IMX219_REG_CSI_LANE_MODE, IMX219_CSI_2_LANE_MODE},
};

static const struct regval imx219_4lane_regs[] = {
  /* PLL Clock Table */
  {IMX219_REG_VTPXCK_DIV, 5},
  {IMX219_REG_VTSYCK_DIV, 1},
  {IMX219_REG_PREPLLCK_VT_DIV, 3}, /* 0x03 = AUTO set */
  {IMX219_REG_PREPLLCK_OP_DIV, 3}, /* 0x03 = AUTO set */
  {IMX219_REG_PLL_VT_MPY_HI, 0},
  {IMX219_REG_PLL_VT_MPY_LO, 88},
  {IMX219_REG_OPSYCK_DIV, 1},
  {IMX219_REG_PLL_OP_MPY_HI, 0},
  {IMX219_REG_PLL_OP_MPY_LO, 91},

  /* 4-Lane CSI Mode */
  {IMX219_REG_CSI_LANE_MODE, IMX219_CSI_4_LANE_MODE},
};

// static const struct regval res_2592_1944_regs[] = {
//   {0x3000, 0x01},
//   {0x3002, 0x00},
//   {0x300c, 0x3b},
//   {0x300d, 0x2a},
//   {0x3018, 0x04},
//   {0x302c, 0x3c},
//   {0x302e, 0x20},
//   {0x3056, 0x98},
//   {0x3074, 0xc8},
//   {0x3076, 0x30},
//   {0x304c, 0x00},
//   {0x314c, 0xc6},
//   {0x315a, 0x02},
//   {0x3168, 0xa0},
//   {0x316a, 0x7e},
//   {0x31a1, 0x00},
//   {0x3288, 0x21},
//   {0x328a, 0x02},
//   {0x3414, 0x05},
//   {0x3416, 0x18},
//   {0x3648, 0x01},
//   {0x364a, 0x04},
//   {0x364c, 0x04},
//   {0x3678, 0x01},
//   {0x367c, 0x31},
//   {0x367e, 0x31},
//   {0x3706, 0x10},
//   {0x3708, 0x03},
//   {0x3714, 0x02},
//   {0x3715, 0x02},
//   {0x3716, 0x01},
//   {0x3717, 0x03},
//   {0x371c, 0x3d},
//   {0x371d, 0x3f},
//   {0x372c, 0x00},
//   {0x372d, 0x00},
//   {0x372e, 0x46},
//   {0x372f, 0x00},
//   {0x3730, 0x89},
//   {0x3731, 0x00},
//   {0x3732, 0x08},
//   {0x3733, 0x01},
//   {0x3734, 0xfe},
//   {0x3735, 0x05},
//   {0x3740, 0x02},
//   {0x375d, 0x00},
//   {0x375e, 0x00},
//   {0x375f, 0x11},
//   {0x3760, 0x01},
//   {0x3768, 0x1b},
//   {0x3769, 0x1b},
//   {0x376a, 0x1b},
//   {0x376b, 0x1b},
//   {0x376c, 0x1a},
//   {0x376d, 0x17},
//   {0x376e, 0x0f},
//   {0x3776, 0x00},
//   {0x3777, 0x00},
//   {0x3778, 0x46},
//   {0x3779, 0x00},
//   {0x377a, 0x89},
//   {0x377b, 0x00},
//   {0x377c, 0x08},
//   {0x377d, 0x01},
//   {0x377e, 0x23},
//   {0x377f, 0x02},
//   {0x3780, 0xd9},
//   {0x3781, 0x03},
//   {0x3782, 0xf5},
//   {0x3783, 0x06},
//   {0x3784, 0xa5},
//   {0x3788, 0x0f},
//   {0x378a, 0xd9},
//   {0x378b, 0x03},
//   {0x378c, 0xeb},
//   {0x378d, 0x05},
//   {0x378e, 0x87},
//   {0x378f, 0x06},
//   {0x3790, 0xf5},
//   {0x3792, 0x43},
//   {0x3794, 0x7a},
//   {0x3796, 0xa1},
//   {0x37b0, 0x36},
//   {0x3a00, 0x01},
// };
//
// static const struct regval mode_2l_10b_regs[] = {
//   {0x3050, 0x00},
//   {0x319D, 0x00},
//   {0x341c, 0xff},
//   {0x341d, 0x01},
//   {0x3a01, 0x01},
// };
//
// static const struct regval inck_74Mhz_regs[] = {
//   {0x300c, 0xB6},
//   {0x300d, 0x7F},
//   {0x314c, 0x80},
//   {0x314d, 0x00},
//   {0x315a, 0x03},
//   {0x3168, 0x68},
//   {0x316a, 0x7F},
// };
//
// static const struct regval inck_27Mhz_regs[] = {
//   {0x300c, 0x42},
//   {0x300d, 0x2E},
//   {0x314c, 0xB0},
//   {0x314d, 0x00},
//   {0x315a, 0x02},
//   {0x3168, 0x8F},
//   {0x316a, 0x7E},
// };
//
// static const struct regval inck_24Mhz_regs[] = {
//   {0x300c, 0x3B},
//   {0x300d, 0x2A},
//   {0x314c, 0xC6},
//   {0x314d, 0x00},
//   {0x315a, 0x02},
//   {0x3168, 0xA0},
//   {0x316a, 0x7E},
// };
//
// static const struct regval inck_18Mhz_regs[] = {
//   {0x300c, 0x2D},
//   {0x300d, 0x1F},
//   {0x314c, 0x84},
//   {0x314d, 0x00},
//   {0x315a, 0x01},
//   {0x3168, 0x6B},
//   {0x316a, 0x7D},
// };
//
// static const struct regval inck_6Mhz_regs[] = {
//   {0x300c, 0x0F},
//   {0x300d, 0x0B},
//   {0x314c, 0xC6},
//   {0x314d, 0x00},
//   {0x315a, 0x00},
//   {0x3168, 0xA0},
//   {0x316a, 0x7C},
// };
//
// static const struct regval framerate_10fps_regs[] = {
//   {0x3030, 0xC0},
//   {0x3031, 0x34},
// };
//
// static const struct regval framerate_15fps_regs[] = {
//   {0x3030, 0x2A},
//   {0x3031, 0x23},
// };
//
// static const struct regval framerate_20fps_regs[] = {
//   {0x3030, 0x60},
//   {0x3031, 0x1A},
// };
//
// static const struct regval framerate_25fps_regs[] = {
//   {0x3030, 0x1A},
//   {0x3031, 0x15},
// };
//
// static const struct regval framerate_30fps_regs[] = {
//   {0x3030, 0x94},
//   {0x3031, 0x11},
// };
//
// static const struct regval mirrorflip_mode_regs[][10] = {
//   {
//     {AREA3_ST_ADR_1_LSB, 0xc8}, //AREA3_ST_ADR_1 LSB
//     {AREA3_ST_ADR_1_MSB, 0x00}, //AREA3_ST_ADR_1 MSB
//     {IMX219_REG_HREVERSE, 0x00}, //HREVERSE 0
//     {IMX219_REG_VREVERSE, 0x00}, //VREVERSE 0
//     {0x3081, 0x02}, //RESERVED
//     {0x3083, 0x02}, //RESERVED
//     {0x30b6, 0x00}, //RESERVED
//     {0x30b7, 0x00}, //RESERVED
//     {0x3116, 0x08}, //RESERVED
//     {0x3117, 0x00}, //RESERVED
//   },
//   {
//     {AREA3_ST_ADR_1_LSB, 0xf8}, //AREA3_ST_ADR_1 LSB
//     {AREA3_ST_ADR_1_MSB, 0x0f}, //AREA3_ST_ADR_1 MSB
//     {IMX219_REG_HREVERSE, 0x00}, //HREVERSE 0
//     {IMX219_REG_VREVERSE, 0x01}, //VREVERSE 1
//     {0x3081, 0xfe}, //RESERVED
//     {0x3083, 0xfe}, //RESERVED
//     {0x30b6, 0xfa}, //RESERVED
//     {0x30b7, 0x01}, //RESERVED
//     {0x3116, 0x02}, //RESERVED
//     {0x3117, 0x00}, //RESERVED
//   },
//   {
//     {AREA3_ST_ADR_1_LSB, 0xc8}, //AREA3_ST_ADR_1 LSB
//     {AREA3_ST_ADR_1_MSB, 0x00}, //AREA3_ST_ADR_1 MSB
//     {IMX219_REG_HREVERSE, 0x01}, //HREVERSE 1
//     {IMX219_REG_VREVERSE, 0x00}, //VREVERSE 0
//     {0x3081, 0x02}, //RESERVED
//     {0x3083, 0x02}, //RESERVED
//     {0x30b6, 0x00}, //RESERVED
//     {0x30b7, 0x00}, //RESERVED
//     {0x3116, 0x08}, //RESERVED
//     {0x3117, 0x00}, //RESERVED
//   },
//   {
//     {AREA3_ST_ADR_1_LSB, 0xf8}, //AREA3_ST_ADR_1 LSB
//     {AREA3_ST_ADR_1_MSB, 0x0f}, //AREA3_ST_ADR_1 MSB
//     {IMX219_REG_HREVERSE, 0x01}, //HREVERSE 1
//     {IMX219_REG_VREVERSE, 0x01}, //VREVERSE 1
//     {0x3081, 0xfe}, //RESERVED
//     {0x3083, 0xfe}, //RESERVED
//     {0x30b6, 0xfa}, //RESERVED
//     {0x30b7, 0x01}, //RESERVED
//     {0x3116, 0x02}, //RESERVED
//     {0x3117, 0x00}, //RESERVED
//   },
// };
//
// static const struct regval test_pattern_enable_regs[] = {
//   {0x3148, 0x10},
//   {0x3280, 0x00},
//   {0x329c, 0x01},
//   {0x32a0, 0x11},
//   {0x3302, 0x00},
//   {0x3303, 0x00},
//   {0x336c, 0x00},
// };
//
// static const struct regval test_pattern_disable_regs[] = {
//   {0x3148, 0x00},
//   {0x3280, 0x01},
//   {0x329c, 0x00},
//   {0x32a0, 0x10},
//   {0x3302, 0x32},
//   {0x3303, 0x00},
//   {0x336c, 0x01},
// };

#define IMX219_1H_PERIOD_USEC (1000000.0F / 4500 / 30)

/**
  * @}
  */

/** @defgroup IMX219_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t IMX219_WriteTable(IMX219_Object_t* pObj, const struct regval* regs, uint32_t size);
static int32_t IMX219_ReadRegWrap(void* handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
static int32_t IMX219_WriteRegWrap(void* handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
static int32_t IMX219_Delay(IMX219_Object_t* pObj, uint32_t Delay);

/**
  * @}
  */

/** @defgroup IMX219_Private_Functions Private Functions
  * @{
  */
static int32_t IMX219_WriteTable(IMX219_Object_t* pObj, const struct regval* regs, uint32_t size)
{
  uint32_t index;
  int32_t ret = IMX219_OK;

  /* Set registers */
  for (index = 0; index < size; index++)
  {
    if (ret != IMX219_ERROR)
    {
      if (imx219_write_reg(&pObj->Ctx, regs[index].addr, (uint8_t*)&(regs[index].val), 1) != IMX219_OK)
      {
        ret = IMX219_ERROR;
      }
    }
  }
  return ret;
}

/**
  * @brief This function provides accurate delay (in milliseconds)
  * @param pObj   pointer to component object
  * @param Delay  specifies the delay time length, in milliseconds
  * @retval IMX219_OK
  */
static int32_t IMX219_Delay(IMX219_Object_t* pObj, uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = pObj->IO.GetTick();
  while ((pObj->IO.GetTick() - tickstart) < Delay)
  {}
  return IMX219_OK;
}

/**
  * @brief  Wrap component ReadReg to Bus Read function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t IMX219_ReadRegWrap(void* handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  IMX219_Object_t* pObj = (IMX219_Object_t*)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t IMX219_WriteRegWrap(void* handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  IMX219_Object_t* pObj = (IMX219_Object_t*)handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @}
  */

/** @defgroup IMX219_Exported_Functions IMX219 Exported Functions
  * @{
  */
/**
  * @brief  Register component IO bus
  * @param  Component object pointer
  * @retval Component status
  */
int32_t IMX219_RegisterBusIO(IMX219_Object_t* pObj, IMX219_IO_t* pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = IMX219_ERROR;
  } else
  {
    pObj->IO.Init = pIO->Init;
    pObj->IO.DeInit = pIO->DeInit;
    pObj->IO.Address = pIO->Address;
    pObj->IO.WriteReg = pIO->WriteReg;
    pObj->IO.ReadReg = pIO->ReadReg;
    pObj->IO.GetTick = pIO->GetTick;

    pObj->Ctx.ReadReg = IMX219_ReadRegWrap;
    pObj->Ctx.WriteReg = IMX219_WriteRegWrap;
    pObj->Ctx.handle = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    } else
    {
      ret = IMX219_ERROR;
    }
  }

  return ret;
}

static int32_t imx219_configure_lanes(IMX219_Object_t* pObj)
{
  /* Write the appropriate PLL settings for the number of MIPI lanes */
  return IMX219_WriteTable(pObj, imx219_2lane_regs, ARRAY_SIZE(imx219_2lane_regs));
};

static uint8_t imx219_get_format_bpp(const uint32_t resolution)
{
  // switch (format->code) {
  // case MEDIA_BUS_FMT_SRGGB8_1X8:
  // case MEDIA_BUS_FMT_SGRBG8_1X8:
  // case MEDIA_BUS_FMT_SGBRG8_1X8:
  // case MEDIA_BUS_FMT_SBGGR8_1X8:
  // 	return 8;
  //
  // case MEDIA_BUS_FMT_SRGGB10_1X10:
  // case MEDIA_BUS_FMT_SGRBG10_1X10:
  // case MEDIA_BUS_FMT_SGBRG10_1X10:
  // case MEDIA_BUS_FMT_SBGGR10_1X10:
  // default:
  // 	return 10;
  // }
  return 10;
}

static void imx219_get_binning(uint8_t* bin_h, uint8_t* bin_v)
{
  if (IMX219_BINNING_FAC > 1) {
    *bin_h = IMX219_BINNING_X2;
    *bin_v = IMX219_BINNING_X2;
  } else {
    *bin_h = IMX219_BINNING_NONE;
    *bin_v = IMX219_BINNING_NONE;
  }
}

static int32_t imx219_set_framefmt(IMX219_Object_t* pObj, uint32_t resolution)
{
  uint16_t width = IMX219_WIDTH;
  uint16_t height = IMX219_HEIGHT;
  uint8_t bin_h, bin_v;
  uint8_t bpp = imx219_get_format_bpp(resolution);
  uint16_t reg_x_sta_a = IMX219_ACTIVE_AREA_LEFT;
  uint16_t reg_x_end_a = IMX219_ACTIVE_AREA_LEFT + width - 1;
  uint16_t reg_y_sta_a = IMX219_ACTIVE_AREA_LEFT;
  uint16_t reg_y_end_a = IMX219_ACTIVE_AREA_LEFT + width - 1;

  imx219_write_reg(&pObj->Ctx, IMX219_REG_X_ADD_STA_A, (uint8_t*)&reg_x_sta_a, 2);
  imx219_write_reg(&pObj->Ctx, IMX219_REG_X_ADD_END_A, (uint8_t*)&reg_x_end_a, 2);

  imx219_write_reg(&pObj->Ctx, IMX219_REG_Y_ADD_STA_A, (uint8_t*)&reg_y_sta_a, 2);
  imx219_write_reg(&pObj->Ctx, IMX219_REG_Y_ADD_END_A, (uint8_t*)&reg_y_end_a, 2);

  imx219_get_binning(&bin_h, &bin_v);

  imx219_write_reg(&pObj->Ctx, IMX219_REG_BINNING_MODE_H, &bin_h, 1);
  imx219_write_reg(&pObj->Ctx, IMX219_REG_BINNING_MODE_V, &bin_v, 1);

  imx219_write_reg(&pObj->Ctx, IMX219_REG_X_OUTPUT_SIZE, (uint8_t*)&width, 2);
  imx219_write_reg(&pObj->Ctx, IMX219_REG_Y_OUTPUT_SIZE, (uint8_t*)&height, 2);

  imx219_write_reg(&pObj->Ctx, IMX219_REG_TP_WINDOW_WIDTH, (uint8_t*)&width, 2);
  imx219_write_reg(&pObj->Ctx, IMX219_REG_TP_WINDOW_HEIGHT, (uint8_t*)&height, 2);

  uint16_t bpp_u16 = (bpp << 8) | bpp;
  imx219_write_reg(&pObj->Ctx, IMX219_REG_CSI_DATA_FORMAT_A, (uint8_t*)&bpp_u16, 2);
  imx219_write_reg(&pObj->Ctx, IMX219_REG_OPPXCK_DIV, &bpp, 1);
  return IMX219_OK;
}

static int32_t imx219_enable_streaming(IMX219_Object_t* pObj)
{
  uint8_t tmp = IMX219_MODE_STREAMING;
  return imx219_write_reg(&pObj->Ctx, IMX219_REG_MODE_SELECT, &tmp, 1);
}

/**
  * @brief  Initializes the IMX219 CAMERA component.
  * @param  pObj  pointer to component object
  * @param  Resolution  Camera resolution
  * @param  PixelFormat pixel format to be configured
  * @retval Component status
  */
int32_t IMX219_Init(IMX219_Object_t* pObj, uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret = IMX219_OK;

  if (pObj->IsInitialized == 0U)
  {
    if (IMX219_WriteTable(pObj, imx219_common_regs, ARRAY_SIZE(imx219_common_regs)) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    } else
    if (imx219_configure_lanes(pObj) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    } else if (imx219_set_framefmt(pObj, Resolution) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initializes the IMX219 CAMERA component.
  * @param  pObj  pointer to component object
  * @param  Resolution  Camera resolution
  * @param  PixelFormat pixel format to be configured
  * @retval Component status
  */
int32_t IMX219_Start(IMX219_Object_t* pObj)
{
  uint8_t tmp;
  int32_t ret = IMX219_OK;
  /* Start streaming */
  tmp = IMX219_MODE_STREAMING;
  ret = imx219_write_reg(&pObj->Ctx, IMX219_REG_MODE_SELECT, &tmp, 1);
  if (ret != IMX219_OK)
  {
    return IMX219_ERROR;
  }
  IMX219_Delay(pObj, 20);
  return ret;
}

/**
  * @brief  De-initializes the camera sensor.
  * @param  pObj  pointer to component object
  * @retval Component status
  */
int32_t IMX219_DeInit(IMX219_Object_t* pObj)
{
  if (pObj->IsInitialized == 1U)
  {
    /* De-initialize camera sensor interface */
    pObj->IsInitialized = 0U;
  }

  return IMX219_OK;
}

/**
  * @brief  Read the IMX219 Camera identity.
  * @param  pObj  pointer to component object
  * @param  Id    pointer to component ID
  * @retval Component status
  */
int32_t IMX219_ReadID(IMX219_Object_t* pObj, uint32_t* Id)
{
  int32_t ret;
  uint8_t tmp[2];

  /* Initialize I2C */
  pObj->IO.Init();

  if (imx219_read_reg(&pObj->Ctx, IMX219_REG_ID, &tmp[0], 2) != IMX219_OK)
  {
    ret = IMX219_ERROR;
  } else
  {
    *Id = tmp[0] << 8 | tmp[1];
    ret = IMX219_OK;
  }

  /* Component status */
  return ret;
}

/**
  * @brief  Read the IMX219 Camera Capabilities.
  * @param  pObj          pointer to component object
  * @param  Capabilities  pointer to component Capabilities
  * @retval Component status
  */
int32_t IMX219_GetCapabilities(IMX219_Object_t* pObj, IMX219_Capabilities_t* Capabilities)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = IMX219_ERROR;
  } else
  {
    Capabilities->Config_Brightness = 0;
    Capabilities->Config_Contrast = 0;
    Capabilities->Config_HueDegree = 0;
    Capabilities->Config_Gain = 1;
    Capabilities->Config_Exposure = 1;
    Capabilities->Config_ExposureMode = 0;
    Capabilities->Config_LightMode = 0;
    Capabilities->Config_MirrorFlip = 1;
    Capabilities->Config_NightMode = 0;
    Capabilities->Config_Resolution = 0;
    Capabilities->Config_Saturation = 0;
    Capabilities->Config_SpecialEffect = 0;
    Capabilities->Config_Zoom = 0;
    Capabilities->Config_SensorInfo = 1;
    Capabilities->Config_TestPattern = 1;

    ret = IMX219_OK;
  }

  return ret;
}

/**
  * @brief  Set the gain
  * @param  pObj  pointer to component object
  * @param  Gain Gain in mdB
  * @retval Component status
  */
int32_t IMX219_SetGain(IMX219_Object_t* pObj, int32_t gain)
{
  int32_t ret = IMX219_OK;

  if ((gain > IMX219_GAIN_MAX) || (gain < IMX219_GAIN_MIN))
  {
    ret = IMX219_ERROR;
  } else
  {
    // gain = 256 / (256 - X)
    uint32_t gain_param = 256000UL - 256000UL / gain;
    gain_param /= 1000;
    uint8_t gain_param_u8 = (uint8_t)gain_param;
    if (imx219_write_reg(&pObj->Ctx, IMX219_REG_ANALOG_GAIN, &gain_param_u8, 1) != IMX219_OK)
    {
      ret = IMX219_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Set the exposure
  * @param  pObj  pointer to component object
  * @param  Exposure Exposure in micro seconds
  * @retval Component status
  */
int32_t IMX219_SetExposure(IMX219_Object_t* pObj, int32_t exposure)
{
  int32_t ret = IMX219_OK;

  uint8_t rate_factor = IMX219_BINNING_FAC;
  uint16_t val = exposure / rate_factor;
  if (val >= IMX219_EXPOSURE_MIN && val <= IMX219_EXPOSURE_MAX)
  {
    return imx219_write_reg(&pObj->Ctx, IMX219_REG_EXPOSURE, (uint8_t*)&val, 2);
  } else
  {
    DEV_ASSERT(0, "value out of bound");
  }

  return ret;
}

/**
  * @brief  Set the Frequency
  * @param  pObj  pointer to component object
  * @param  frequency in Mhz
  * @retval Component status
  */
int32_t IMX219_SetFrequency(IMX219_Object_t* pObj, int32_t frequency)
{
  (void)pObj;
  (void)frequency;
  uint32_t ret = IMX219_OK;

  // NOTE: default to IMX219_XCLK_FREQ, written during init with common table

  return ret;
}

/**
  * @brief  Set the Framerate
  * @param  pObj  pointer to component object
  * @param  framerate 10, 15, 20, 25 or 30fps
  * @retval Component status
  */
int32_t IMX219_SetFramerate(IMX219_Object_t* pObj, int32_t framerate)
{
  (void)pObj;
  (void)framerate;
  uint32_t ret = IMX219_OK;

  return ret;
}

/**
  * @brief  Control imx219 camera mirror/vflip.
  * @param  pObj  pointer to component object
  * @param  Config To configure mirror, flip, both or none
  * @retval Component status
  */
int32_t IMX219_MirrorFlipConfig(IMX219_Object_t* pObj, uint32_t Config)
{
  int32_t ret = IMX219_OK;
  uint8_t value;

#define HFLIP_DIS 0x00U
#define HFLIP_ENA 0x01U
#define VFLIP_DIS 0x00U
#define VFLIP_ENA 0x02U

  switch (Config)
  {
  case IMX219_FLIP: value = HFLIP_DIS | VFLIP_ENA; break;
  case IMX219_MIRROR: value = HFLIP_ENA | VFLIP_DIS; break;
  case IMX219_MIRROR_FLIP: value = HFLIP_ENA | VFLIP_ENA; break;
  case IMX219_MIRROR_FLIP_NONE:
  default: value = HFLIP_DIS | VFLIP_DIS; break;
  }

  if (imx219_write_reg(&pObj->Ctx, IMX219_REG_ORIENTATION, &value, 1) != IMX219_OK)
  {
    return IMX219_ERROR;
  }
  return ret;
}

static const int imx219_test_pattern_val[] = {
  IMX219_TEST_PATTERN_DISABLE, IMX219_TEST_PATTERN_COLOR_BARS, IMX219_TEST_PATTERN_SOLID_COLOR, IMX219_TEST_PATTERN_GREY_COLOR, IMX219_TEST_PATTERN_PN9,
};

/**
  * @brief  Set the Test Pattern Generator
  * @param  pObj  pointer to component object
  * @param  mode Pattern mode:
  *              -1 : Disable
  *               0 : All 000h
  *               1 : All FFFh
  *               2 : All 555h
  *               3 : All AAAh
  *               4 : Toggle 555/AAAh
  *               5 : Toggle AAA/555h
  *               6 : Toggle 000/555h
  *               7 : Toggle 555/000h
  *               8 : Toggle 000/FFFh
  *               9 : Toggle FFF/000h
  *               10: Horizontal color bars
  *               11: Vertical color bars
  * @retval Component status
  */
int32_t IMX219_SetTestPattern(IMX219_Object_t* pObj, int32_t mode)
{
  int32_t ret = IMX219_OK;
  uint8_t val;
  int32_t test_cnt = ARRAY_SIZE(imx219_test_pattern_val);

  if (mode >= 0 && mode < test_cnt)
  {
    /* Enable Test Pattern #mode */
    val = imx219_test_pattern_val[mode];
    if (imx219_write_reg(&pObj->Ctx, IMX219_REG_TEST_PATTERN, &val, 1) != IMX219_OK)
    {
      return IMX219_ERROR;
    }
  } else
  {
    val = IMX219_TEST_PATTERN_DISABLE;
    /* Disable Test Pattern */
    if (imx219_write_reg(&pObj->Ctx, IMX219_REG_TEST_PATTERN, &val, 1) != IMX219_OK)
    {
      return IMX219_ERROR;
    }
  }

  return ret;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
