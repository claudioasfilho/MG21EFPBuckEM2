/***************************************************************************//**
 * @file
 * @brief EFP (Energy Friendly PMIC) driver configuration definitions.
 * @version 5.7.0
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

//#define SL_EFPDRV_EM_CTRL_GPIO_BITBANG  // GPIO driven "direct mode" EM transitions
#define SL_EFPDRV_EM_CTRL_I2C         // I2C transfers control EM transitions
//#define SL_EFPDRV_EM_CTRL_EMU         // Builtin EMU controlled "direct mode" EM transitions

#define SL_EFPDRV_INIT_BRD4179A {                   \
    0,          /* No initial config */    \
    NULL,       /* No config data */       \
    true,       /* Init GPIO as EFP IRQ */ \
    gpioPortD,  /* EFP IRQ port */         \
    3,          /* EFP IRQ pin */          \
    I2C0,       /* Use I2C instance 0 */   \
    gpioPortC,  /* SCL port */             \
    0,         /* SCL pin */              \
    gpioPortC,  /* SDA port */             \
    2         /* SDA pin */              \
}

#define SL_EFPDRV_INIT_BRD4181A {                   \
    0,          /* No initial config */    \
    NULL,       /* No config data */       \
    true,       /* Init GPIO as EFP IRQ */ \
    gpioPortD,  /* EFP IRQ port */         \
    4,          /* EFP IRQ pin */          \
    I2C0,       /* Use I2C instance 0 */   \
    gpioPortC,  /* SCL port */             \
    4,         /* SCL pin */              \
    gpioPortC,  /* SDA port */             \
    5         /* SDA pin */              \
 }

// Define optional EFP register writes here if you need to modify or extend the
// default setting. Set init.config_size to the number of register writes and
// make init.config_data point to the array of extra settings.
//
// #define EXTRA_EFP_CONFIG_SETTINGS {
//     { EFP_VMC_BIAS_SW, 0xXX },
//     { EFP_BBK_CTRL, 0xYY },
//     ...
//     { EFP_CC_CAL, 0xZZ },
// }
