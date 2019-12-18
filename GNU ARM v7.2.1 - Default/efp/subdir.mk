################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../efp/sl_efpdrv.c 

OBJS += \
./efp/sl_efpdrv.o 

C_DEPS += \
./efp/sl_efpdrv.d 


# Each subdirectory must supply rules for building sources it contributes
efp/sl_efpdrv.o: ../efp/sl_efpdrv.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DHAL_CONFIG=1' '-D__StackLimit=0x20000000' '-DNVM3_DEFAULT_NVM_SIZE=24576' '-D__STACK_SIZE=0x800' '-D__HEAP_SIZE=0xD00' '-DEFR32MG21A020F1024IM32=1' -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/efp" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/Device/SiliconLabs/EFR32MG21/Include" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/radio/rail_lib/protocol/ieee802154" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emlib/inc" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emlib/src" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/hardware/kit/common/bsp" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/CMSIS/Include" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/hardware/kit/common/halconfig" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/radio/rail_lib/common" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/hardware/kit/common/drivers" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/protocol/bluetooth/ble_stack/inc/soc" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emdrv/uartdrv/inc" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/hardware/kit/EFR32MG21_BRD4180A/config" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emdrv/nvm3/inc" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emdrv/sleep/src" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/app/bluetooth/common/util" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/Device/SiliconLabs/EFR32MG21/Source" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/protocol/bluetooth/ble_stack/inc/common" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emdrv/nvm3/src" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/bootloader/api" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emdrv/gpiointerrupt/inc" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/halconfig/inc/hal-config" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/Device/SiliconLabs/EFR32MG21/Source/GCC" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/radio/rail_lib/protocol/ble" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emdrv/sleep/inc" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/emdrv/common/inc" -I"/Users/clfilho/SimplicityStudio/NM_workspace/soc-empty-efp01-efr32xg21_Tiago_0dBm_iBeacon/platform/bootloader" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv5-sp-d16 -mfloat-abi=hard -MMD -MP -MF"efp/sl_efpdrv.d" -MT"efp/sl_efpdrv.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


