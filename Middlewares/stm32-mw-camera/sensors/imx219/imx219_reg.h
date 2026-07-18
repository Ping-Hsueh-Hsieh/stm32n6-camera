/**
  ******************************************************************************
  * @file    imx219_reg.h
  * @author  MCD Application Team
  * @brief   Header of imx219_reg.c
  *
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
#ifndef IMX219_REG_H
#define IMX219_REG_H

#include <cmsis_compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup BSP
  * @{
  */

/*
 * Macros to define register address with the register width encoded
 * into the higher bits.
 */
#define CCI_REG_ADDR_MASK GENMASK(15, 0)
#define CCI_REG_WIDTH_SHIFT 16
#define CCI_REG_WIDTH_MASK GENMASK(19, 16)
/*
 * Private CCI register flags, for the use of drivers.
 */
#define CCI_REG_PRIVATE_SHIFT 28U
#define CCI_REG_PRIVATE_MASK GENMASK(31U, CCI_REG_PRIVATE_SHIFT)

#define CCI_REG_WIDTH_BYTES(x) FIELD_GET(CCI_REG_WIDTH_MASK, x)
#define CCI_REG_WIDTH(x) (CCI_REG_WIDTH_BYTES(x) << 3)
#define CCI_REG_ADDR(x) FIELD_GET(CCI_REG_ADDR_MASK, x)
#define CCI_REG_LE BIT(20)

// NOTE: not sure why V4L2 needs to shift register up
// #define CCI_REG8(x) ((1 << CCI_REG_WIDTH_SHIFT) | (x))
// #define CCI_REG16(x) ((2 << CCI_REG_WIDTH_SHIFT) | (x))
// #define CCI_REG24(x) ((3 << CCI_REG_WIDTH_SHIFT) | (x))
// #define CCI_REG32(x) ((4 << CCI_REG_WIDTH_SHIFT) | (x))
// #define CCI_REG64(x) ((8 << CCI_REG_WIDTH_SHIFT) | (x))
// #define CCI_REG16_LE(x) (CCI_REG_LE | (2U << CCI_REG_WIDTH_SHIFT) | (x))
// #define CCI_REG24_LE(x) (CCI_REG_LE | (3U << CCI_REG_WIDTH_SHIFT) | (x))
// #define CCI_REG32_LE(x) (CCI_REG_LE | (4U << CCI_REG_WIDTH_SHIFT) | (x))
// #define CCI_REG64_LE(x) (CCI_REG_LE | (8U << CCI_REG_WIDTH_SHIFT) | (x))

#define CCI_REG8(x) (x)
#define CCI_REG16(x)(x)
#define CCI_REG24(x)(x)
#define CCI_REG32(x)(x)
#define CCI_REG64(x)(x)

/** @addtogroup Components
  * @{
  */

/** @addtogroup IMX219
  * @{
  */

/** @defgroup IMX219_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup IMX219_Exported_Constants IMX219 Exported Constants
  * @{
  */
#define IMX219_REG_MODE_SELECT 0x0100
#define IMX219_MODE_STANDBY 0x00
#define IMX219_MODE_STREAMING 0x01

#define IMX219_REG_ID 0x0000
#define IMX219_CHIP_ID 0x0219

#define IMX219_REG_CSI_LANE_MODE CCI_REG8(0x0114)
#define IMX219_CSI_2_LANE_MODE 0x01
#define IMX219_CSI_4_LANE_MODE 0x03

#define IMX219_REG_DPHY_CTRL CCI_REG8(0x0128)
#define IMX219_DPHY_CTRL_TIMING_AUTO 0
#define IMX219_DPHY_CTRL_TIMING_MANUAL 1

#define IMX219_REG_EXCK_FREQ_HI CCI_REG8(0x012a)
#define IMX219_REG_EXCK_FREQ_LO CCI_REG8(0x012b)
#define IMX219_EXCK_FREQ(n) ((n) * 256) /* n expressed in MHz */

/* Analog gain control */
#define IMX219_REG_ANALOG_GAIN CCI_REG8(0x0157)
#define IMX219_ANA_GAIN_MIN 0
#define IMX219_ANA_GAIN_MAX 232
#define IMX219_ANA_GAIN_STEP 1
#define IMX219_ANA_GAIN_DEFAULT 0x0

/* Digital gain control */
#define IMX219_REG_DIGITAL_GAIN CCI_REG16(0x0158)
#define IMX219_DGTL_GAIN_MIN 0x0100
#define IMX219_DGTL_GAIN_MAX 0x0fff
#define IMX219_DGTL_GAIN_DEFAULT 0x0100
#define IMX219_DGTL_GAIN_STEP 1

/* Exposure control */
#define IMX219_REG_EXPOSURE CCI_REG16(0x015a)
#define IMX219_EXPOSURE_MIN 4
#define IMX219_EXPOSURE_STEP 1
#define IMX219_EXPOSURE_DEFAULT 0x640
#define IMX219_EXPOSURE_MAX 65535
#define IMX219_EXPOSURE_OFFSET 4

/* V_TIMING internal */
#define IMX219_REG_FRM_LENGTH_A CCI_REG16(0x0160)
#define IMX219_FLL_MAX 0xfffe
#define IMX219_VBLANK_MIN 32
#define IMX219_REG_LINE_LENGTH_A CCI_REG16(0x0162)
#define IMX219_LLP_MIN 0x0d78
#define IMX219_BINNED_LLP_MIN 0x0de8
#define IMX219_LLP_MAX 0x7ff0

#define IMX219_REG_X_ADD_STA_A CCI_REG16(0x0164)
#define IMX219_REG_X_ADD_END_A CCI_REG16(0x0166)
#define IMX219_REG_Y_ADD_STA_A CCI_REG16(0x0168)
#define IMX219_REG_Y_ADD_END_A CCI_REG16(0x016a)
#define IMX219_REG_X_OUTPUT_SIZE CCI_REG16(0x016c)
#define IMX219_REG_Y_OUTPUT_SIZE CCI_REG16(0x016e)
#define IMX219_REG_X_ODD_INC_A CCI_REG8(0x0170)
#define IMX219_REG_Y_ODD_INC_A CCI_REG8(0x0171)
#define IMX219_REG_ORIENTATION CCI_REG8(0x0172)

/* Binning  Mode */
#define IMX219_REG_BINNING_MODE_H CCI_REG8(0x0174)
#define IMX219_REG_BINNING_MODE_V CCI_REG8(0x0175)
#define IMX219_BINNING_NONE 0x00
#define IMX219_BINNING_X2 0x01
#define IMX219_BINNING_X4 0x02
#define IMX219_BINNING_X2_ANALOG 0x03

#define IMX219_REG_CSI_DATA_FORMAT_A CCI_REG16(0x018c)

/* PLL Settings */
#define IMX219_REG_VTPXCK_DIV CCI_REG8(0x0301)
#define IMX219_REG_VTSYCK_DIV CCI_REG8(0x0303)
#define IMX219_REG_PREPLLCK_VT_DIV CCI_REG8(0x0304)
#define IMX219_REG_PREPLLCK_OP_DIV CCI_REG8(0x0305)
#define IMX219_REG_PLL_VT_MPY_HI CCI_REG16(0x0306)
#define IMX219_REG_PLL_VT_MPY_LO CCI_REG16(0x0307)
#define IMX219_REG_OPPXCK_DIV CCI_REG8(0x0309)
#define IMX219_REG_OPSYCK_DIV CCI_REG8(0x030b)
#define IMX219_REG_PLL_OP_MPY_HI CCI_REG8(0x030c)
#define IMX219_REG_PLL_OP_MPY_LO CCI_REG8(0x030d)

/* Test Pattern Control */
#define IMX219_REG_TEST_PATTERN CCI_REG16(0x0600)
#define IMX219_TEST_PATTERN_DISABLE 0
#define IMX219_TEST_PATTERN_SOLID_COLOR 1
#define IMX219_TEST_PATTERN_COLOR_BARS 2
#define IMX219_TEST_PATTERN_GREY_COLOR 3
#define IMX219_TEST_PATTERN_PN9 4

/* Test pattern colour components */
#define IMX219_REG_TESTP_RED CCI_REG16(0x0602)
#define IMX219_REG_TESTP_GREENR CCI_REG16(0x0604)
#define IMX219_REG_TESTP_BLUE CCI_REG16(0x0606)
#define IMX219_REG_TESTP_GREENB CCI_REG16(0x0608)
#define IMX219_TESTP_COLOUR_MIN 0
#define IMX219_TESTP_COLOUR_MAX 0x03ff
#define IMX219_TESTP_COLOUR_STEP 1

#define IMX219_REG_TP_WINDOW_WIDTH CCI_REG16(0x0624)
#define IMX219_REG_TP_WINDOW_HEIGHT CCI_REG16(0x0626)

/* External clock frequency is 24.0M */
// #define IMX219_XCLK_FREQ 24000000
#define IMX219_XCLK_FREQ 12000000

/* Pixel rate is fixed for all the modes */
#define IMX219_PIXEL_RATE 182400000
#define IMX219_PIXEL_RATE_4LANE 281600000

#define IMX219_DEFAULT_LINK_FREQ 456000000
#define IMX219_DEFAULT_LINK_FREQ_4LANE_UNSUPPORTED 363000000
#define IMX219_DEFAULT_LINK_FREQ_4LANE 364000000

/* IMX219 native and active pixel array size. */
#define IMX219_NATIVE_WIDTH 3296U
#define IMX219_NATIVE_HEIGHT 2480U
#define IMX219_ACTIVE_AREA_LEFT 8U
#define IMX219_ACTIVE_AREA_TOP 8U
#define IMX219_ACTIVE_AREA_WIDTH 3280U
#define IMX219_ACTIVE_AREA_HEIGHT 2464U

// #define IMX219_REG_HOLD           0x3001
// #define IMX219_REG_VMAX           0x3030
// #define IMX219_REG_SHUTTER        0x3058
// #define IMX219_REG_GAIN           0x30e8
// #define IMX219_REG_TPG            0x329e
//
// #define IMX219_SHUTTER_MIN        9
//
// #define IMX219_EXPOSURE_DEFAULT   23814
//
#define IMX219_NAME               "IMX219"
#define IMX219_BAYER_PATTERN      0 /* From ISP definition RGGB / TODO comnon enumeration in camera */
#define IMX219_COLOR_DEPTH        10 /* in bits */
#define IMX219_GAIN_MIN           (1000)
#define IMX219_GAIN_MAX           (10625)
#define IMX219_AGAIN_MAX          (10625)
// #define IMX219_GAIN_DEFAULT       (20 * 1000)
// #define IMX219_GAIN_UNIT_MDB      300
// #define IMX219_EXPOSURE_MIN       8           /* in us, for sensor @30fps (round up of 7.40 usec) */
// #define IMX219_EXPOSURE_MAX       33266       /* in us, for sensor @30fps */
//
//
//
// #define IMX219_REG_HREVERSE       0x304EU
// #define IMX219_REG_VREVERSE       0x304FU
// #define AREA3_ST_ADR_1_LSB        0x3074U
// #define AREA3_ST_ADR_1_MSB        0x3075U
//
// /* For 2592x1944 */
// #define IMX219_WIDTH              2592
// #define IMX219_HEIGHT             1944
// #define IMX219_PCLK               396000000

/**
  * @}
  */

/************** Generic Function  *******************/

typedef int32_t (*IMX219_Write_Func)(void*, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*IMX219_Read_Func)(void*, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  IMX219_Write_Func WriteReg;
  IMX219_Read_Func ReadReg;
  void* handle;
} imx219_ctx_t;

/*******************************************************************************
* Register      : Generic - All
* Address       : Generic - All
* Bit Group Name: None
* Permission    : W
*******************************************************************************/
int32_t imx219_write_reg(imx219_ctx_t* ctx, uint16_t reg, uint8_t* pdata, uint16_t length);
int32_t imx219_read_reg(imx219_ctx_t* ctx, uint16_t reg, uint8_t* pdata, uint16_t length);

int32_t imx219_register_set(imx219_ctx_t* ctx, uint16_t reg, uint8_t value);

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* IMX219_REG_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
