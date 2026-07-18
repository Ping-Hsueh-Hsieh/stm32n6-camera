/**
  ******************************************************************************
  * @file    imx219_reg.c
  * @author  MCD Application Team
  * @brief   This file provides unitary register function to control the IMX219
  *          Camera driver.
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
#include "imx219_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup IMX219
  * @brief     This file provides a set of functions needed to drive the
  *            IMX219 Camera sensor.
  * @{
  */

/**
  * @brief  Read IMX219 component registers
  * @param  ctx component context
  * @param  reg Register to read from
  * @param  pdata Pointer to data buffer
  * @param  length Number of data to read
  * @retval Component status
  */
int32_t imx219_read_reg(imx219_ctx_t* ctx, uint16_t reg, uint8_t* pdata, uint16_t length)
{
  return ctx->ReadReg(ctx->handle, reg, pdata, length);
}

/**
  * @brief  Write IMX219 component registers
  * @param  ctx component context
  * @param  reg Register to write to
  * @param  pdata Pointer to data buffer
  * @param  length Number of data to write
  * @retval Component status
  */
int32_t imx219_write_reg(imx219_ctx_t* ctx, uint16_t reg, uint8_t* data, uint16_t length)
{
  uint32_t res = 0;
  for (uint16_t i = 0; i < length; i++) {
    res |= ctx->WriteReg(ctx->handle, reg+i, data+(length - i - 1), 1);
  }
  return res;
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
