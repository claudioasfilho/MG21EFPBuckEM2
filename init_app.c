/***************************************************************************//**
 * @file
 * @brief init_app.c
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#include "hal-config.h"
#include "hal-config-board.h"
#else
#include "bspconfig.h"
#endif

#include "bsp.h"
#include "pti.h"

void initApp(void)
{
  // Enable PTI
  configEnablePti();



#warning "WARNING: This radio board uses the same GPIO pin to enable the VCOM port, the LCD display and the temperature sensor! Enabling any of these features might disrupt the serial pins of the EXP header!"
#if defined(HAL_I2CSENSOR_ENABLE) || defined(HAL_SPIDISPLAY_ENABLE) || defined(HAL_VCOM_ENABLE)
#if HAL_I2CSENSOR_ENABLE || HAL_SPIDISPLAY_ENABLE || HAL_VCOM_ENABLE
#define COMMON_ENABLE 1
#else
#define COMMON_ENABLE 0
#endif

#if defined(BSP_I2CSENSOR_ENABLE_PORT)
#define ENABLE_PORT BSP_I2CSENSOR_ENABLE_PORT
#define ENABLE_PIN BSP_I2CSENSOR_ENABLE_PIN
#elif defined(BSP_SPIDISPLAY_ENABLE_PORT)
#define ENABLE_PORT BSP_SPIDISPLAY_ENABLE_PORT
#define ENABLE_PIN BSP_SPIDISPLAY_ENABLE_PIN
#else
#define ENABLE_PORT BSP_VCOM_ENABLE_PORT
#define ENABLE_PIN BSP_VCOM_ENABLE_PIN
#endif

  //Enable if requested
  GPIO_PinModeSet(ENABLE_PORT, ENABLE_PIN, gpioModePushPull, COMMON_ENABLE);

#endif
}