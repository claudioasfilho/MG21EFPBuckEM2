/***************************************************************************//**
 * @file app.c
 * @brief Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
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

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

#include "app.h"
#include "sl_efpdrv.h"
#include "em_cmu.h"
#include "infrastructure.h"
#include "em_emu.h"

/* Print boot message */
static void bootMessage(struct gecko_msg_system_boot_evt_t *bootevt);

/* Flag for indicating DFU Reset must be performed */
static uint8_t boot_to_dfu = 0;

/* EFP */
//bool USESWO = false;  // Seems to cause ~15uA of current
bool USELED = true;
//bool POWERDOWNRAM = false;
bool EFP_POWER_1P2V = true;
bool EFP_POWER_1P8V = true;
bool DISABLE_I2C0_ON_EM2 = true;
bool EFP_GO_TO_EM2 = true;

uint32_t vbatt_mV = 3000;
uint32_t vob_em01_mV = 1150;
uint32_t vob_em23_mV = 1150;
uint32_t ind_voa_nH = 2200;
uint32_t ind_vob_nH = 2200;

volatile uint32_t delay;
volatile uint32_t msTicks;        // Counts 1ms timeTicks.

static void Delay(uint32_t dlyTicks);
void em_EM2(bool powerdownRam);
static void disableClocks(void);
void Led_Flash(int);
void Reset_EFP01(void);



/*this is the original EFP code. Some of it is probably unnecessary since the BLE stack/sample app does it already
 *
 *
 * maybe it would be better to pass EFP_POWER_1P2V/EFP_POWER_1P8V as flags to efp_setup() and make the buck voltages and inductor values constants since they
 * won't change on a given board.
 *
 * */

void efp_setup(){

	int ret;
	//initLog();
	// Add delay to keep from bricking part.
	  for(int delay = 0; delay < 4000000; delay++);

	  uint8_t i2cCtrl, devRevId;

	  // Initialize LED driver.
	  //BSP_LedsInit();
	 // BSP_LedClear(0);
	 // BSP_LedClear(1);

	 // printLog("efp_setup()\r\n");
	  if (EFP_POWER_1P2V | EFP_POWER_1P8V) {
	    // Initialize EFP.
	    sl_efpdrv_init_data init = SL_EFPDRV_INIT_BRD4179A;
	    ret = sl_efpdrv_init(&init);

	    //Reset_EFP01();  This causes EFP to go into a 70uA state???

	    sl_efpdrv_read_register(EFP01_I2C_CTRL, &i2cCtrl);      // Should be 0x09
	    sl_efpdrv_read_register(EFP01_DEVREV_ID, &devRevId);    // Should be 0x38

	    // Clear Force EM0 on I2C Start
	    //    sl_efpdrv_write_register_field(EFP01_EM_CRSREG_CTRL,
	    //                                        0,
	    //                                        _EFP01_EM_CRSREG_CTRL_EM_0ONI2CST_MASK,
	    //                                        _EFP01_EM_CRSREG_CTRL_EM_0ONI2CST_SHIFT);

	    // Note: All settings required to configure VOB could be defined in
	    // sl_efpdrv_config.h as part of the "init" structure.
	    ret = sl_efpdrv_set_voa_em01_peak_current(20, vbatt_mV, 1800, ind_voa_nH);
	   // printLog("sl_efpdrv_set_voa_em01_peak_current() returns %d\r\n", ret);
	    sl_efpdrv_set_voa_em23_peak_current(20, vbatt_mV, 1800, ind_voa_nH);

	    if (EFP_POWER_1P2V) {
	      // Set VOB Output Voltage and Peak Currents.
	      sl_efpdrv_set_vob_em01_voltage(vob_em01_mV);
	      sl_efpdrv_set_vob_em01_peak_current(10, vbatt_mV, vob_em01_mV, ind_vob_nH);
	      sl_efpdrv_set_vob_em23_voltage(vob_em23_mV);
	      sl_efpdrv_set_vob_em23_peak_current(0, vbatt_mV, vob_em23_mV, ind_vob_nH);

	      // Setup HDREG & LDREG to be overdriven
	      // Note the below writes don't work on EFR32MG21, because the secure element
	      // prevents access to any of the EMU hidden registers.  For production,
	      // we need a modification to the secure element code to allow the hdreg/ldreg
	      // to be disabled.
	      // Set EMU_TESTCTRL.REGDIS, offset 0x8C
	      // *(volatile uint32_t*)0x4000408C |= (0x1UL << 14); // Set REGDIS

	      // Enable VOB.
	      ret = sl_efpdrv_set_vob_mode(efp_vob_mode_buck);
          // Make sure VOB is ready before turning off internal LDO regulator.
	      uint8_t tmp;
	      int status;
	     // GPIO_PinOutSet(BSP_LED0_PORT,BSP_LED0_PIN);


	      do {
	        status = sl_efpdrv_read_register(EFP01_STATUS_LIVE, &tmp);

	      } while (((tmp & _EFP01_STATUS_LIVE_VOB_INREG_LIVE_MASK) == 0)
	               || (status != SL_EFPDRV_OK));

	    //  GPIO_PinOutClear(BSP_LED0_PORT,BSP_LED0_PIN);
	      // Turn off internal Panther LDO regulator.
	      // This funciton isn't currently doing anything for Panther
	      sl_efpdrv_shutdown_emu_ldo();
	    }

	    // Prepare GPIO pins for GPIO bitbang EM control of EFP
	   // sl_efpdrv_enable_direct_mode();

	    // If SL_EFPDRV_EM_CTRL_GPIO_BITBANG is defined in sl_efpdrv_config.h, EFP
	    // EM transfers will use "direct mode" GPIO bitbanging.
	    // If SL_EFPDRV_EM_CTRL_I2C is defined in sl_efpdrv_config.h, EFP EM transfers
	    // will be done using I2C commands.
	    //int ret;
        //ret = sl_efpdrv_enter_em0();
        //if(ret){
        //	BSP_LedSet(0);
       // }

      //  printLog("sl_efpdrv_enter_em0 returns %d in setup\r\n", ret);

	  }
	  if(0 !=  sl_efpdrv_enter_em2()){
		//  BSP_LedSet(1);
	  }
}

extern uint32_t wakeup_process_time_overhead;

/**
 * @brief Function for creating a custom advertisement package
 *
 * The function builds the advertisement package according to Apple iBeacon specifications,
 * configures this as the device advertisement data and starts broadcasting.
 */
void bcnSetupAdvBeaconing(void)
{
  /* This function sets up a custom advertisement package according to iBeacon specifications.
   * The advertisement package is 30 bytes long. See the iBeacon specification for further details.
   */

  static struct {
    uint8_t flagsLen;     /* Length of the Flags field. */
    uint8_t flagsType;    /* Type of the Flags field. */
    uint8_t flags;        /* Flags field. */
    uint8_t mandataLen;   /* Length of the Manufacturer Data field. */
    uint8_t mandataType;  /* Type of the Manufacturer Data field. */
    uint8_t compId[2];    /* Company ID field. */
    uint8_t beacType[2];  /* Beacon Type field. */
    uint8_t uuid[16];     /* 128-bit Universally Unique Identifier (UUID). The UUID is an identifier for the company using the beacon*/
    uint8_t majNum[2];    /* Beacon major number. Used to group related beacons. */
    uint8_t minNum[2];    /* Beacon minor number. Used to specify individual beacons within a group.*/
    uint8_t txPower;      /* The Beacon's measured RSSI at 1 meter distance in dBm. See the iBeacon specification for measurement guidelines. */
  }
  bcnBeaconAdvData
    = {
    /* Flag bits - See Bluetooth 4.0 Core Specification , Volume 3, Appendix C, 18.1 for more details on flags. */
    2,  /* length  */
    0x01, /* type */
    0x04 | 0x02, /* Flags: LE General Discoverable Mode, BR/EDR is disabled. */

    /* Manufacturer specific data */
    26,  /* length of field*/
    0xFF, /* type of field */

    /* The first two data octets shall contain a company identifier code from
     * the Assigned Numbers - Company Identifiers document */
    /* 0x004C = Apple */
    { UINT16_TO_BYTES(0x004C) },

    /* Beacon type */
    /* 0x0215 is iBeacon */
    { UINT16_TO_BYTE1(0x0215), UINT16_TO_BYTE0(0x0215) },

    /* 128 bit / 16 byte UUID */
    { 0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, \
      0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0 },

    /* Beacon major number */
    /* Set to 34987 and converted to correct format */
    { UINT16_TO_BYTE1(34987), UINT16_TO_BYTE0(34987) },

    /* Beacon minor number */
    /* Set as 1025 and converted to correct format */
    { UINT16_TO_BYTE1(1025), UINT16_TO_BYTE0(1025) },

    /* The Beacon's measured RSSI at 1 meter distance in dBm */
    /* 0xD7 is -41dBm */
    0xD7
    };

  //
  uint8_t len = sizeof(bcnBeaconAdvData);
  uint8_t *pData = (uint8_t*)(&bcnBeaconAdvData);

  /* Set 0 dBm Transmit Power */
  gecko_cmd_system_set_tx_power(0);

  /* Set custom advertising data */
  gecko_cmd_le_gap_bt5_set_adv_data(0, 0, len, pData);

  /* Set advertising parameters. 100ms advertisement interval.
   * The first two parameters are minimum and maximum advertising interval,
   * both in units of (milliseconds * 1.6). */
  gecko_cmd_le_gap_set_advertise_timing(0, 6400, 6400, 0, 0);

  /* Start advertising in user mode and enable connections */
  gecko_cmd_le_gap_start_advertising(0, le_gap_user_data, le_gap_non_connectable);
}

/* Main application */
void appMain(gecko_configuration_t *pconfig)
{
	uint8 test_data[2] = {0xfe,0xca};
#if DISABLE_SLEEP > 0
  pconfig->sleep.flags = 0;
#endif

  /* Initialize debug prints. Note: debug prints are off by default. See DEBUG_LEVEL in app.h */
  initLog();
 // wakeup_process_time_overhead = 10000;

  /* Initialize stack */
  gecko_init(pconfig);

  efp_setup();

  EMU_EnterEM2(0);
  while(1);

  while (1) {
    /* Event pointer for handling events */
    struct gecko_cmd_packet* evt;

    /* if there are no events pending then the next call to gecko_wait_event() may cause
     * device go to deep sleep. Make sure that debug prints are flushed before going to sleep */
    if (!gecko_event_pending()) {
      flushLog();
    }

    /* Check for stack event. This is a blocking event listener. If you want non-blocking please see UG136. */
    evt = gecko_wait_event();

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {
      /* This boot event is generated when the system boots up after reset.
       * Do not call any stack commands before receiving the boot event.
       * Here the system is set to start advertising immediately after boot procedure. */
      case gecko_evt_system_boot_id:

        bootMessage(&(evt->data.evt_system_boot));
        printLog("boot event - starting advertising\r\n");

        bcnSetupAdvBeaconing();

        break;

      case gecko_evt_le_connection_opened_id:

        printLog("connection opened\r\n");

        break;

      case gecko_evt_le_connection_closed_id:

        printLog("connection closed, reason: 0x%2.2x\r\n", evt->data.evt_le_connection_closed.reason);

        /* Check if need to boot to OTA DFU mode */
        if (boot_to_dfu) {
          /* Enter to OTA DFU mode */
          gecko_cmd_system_reset(2);
        } else {
          /* Restart advertising after client has disconnected */
          gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
        }
        break;

      /* Events related to OTA upgrading
         ----------------------------------------------------------------------------- */

      /* Check if the user-type OTA Control Characteristic was written.
       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:

        if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1;
          /* Send response to Write Request */
          gecko_cmd_gatt_server_send_user_write_response(
            evt->data.evt_gatt_server_user_write_request.connection,
            gattdb_ota_control,
            bg_err_success);

          /* Close connection to enter to DFU OTA mode */
          gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
        }
        break;

      /* Add additional event handlers as your application requires */

      case gecko_evt_gatt_server_characteristic_status_id:
              /* Check that the characteristic in question is temperature - its ID is defined
               * in gatt.xml as "temperature_measurement". Also check that status_flags = 1, meaning that
               * the characteristic client configuration was changed (notifications or indications
               * enabled or disabled). */
              if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_test_data)
                  && (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01)) {
                if (evt->data.evt_gatt_server_characteristic_status.client_config_flags > 0) {
                  /* Indications have been turned ON - start the repeating timer. The 1st parameter '32768'
                   * tells the timer to run for 1 second (32.768 kHz oscillator), the 2nd parameter is
                   * the timer handle and the 3rd parameter '0' tells the timer to repeat continuously until
                   * stopped manually.*/
                  gecko_cmd_hardware_set_soft_timer(32768, 0, 0);
                } else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x00) {
                  /* Indications have been turned OFF - stop the timer. */
                  gecko_cmd_hardware_set_soft_timer(0, 0, 0);
                }
              }
              break;
      case gecko_evt_hardware_soft_timer_id:
    	  if(0==evt->data.evt_hardware_soft_timer.handle){
    		  gecko_cmd_gatt_server_send_characteristic_notification(0xFF,gattdb_test_data,sizeof(test_data),test_data);

    	  }
    	  break;
      default:
        break;
    }
  }
}

/* Print stack version and local Bluetooth address as boot message */
static void bootMessage(struct gecko_msg_system_boot_evt_t *bootevt)
{
#if DEBUG_LEVEL
  bd_addr local_addr;
  int i;

  printLog("stack version: %u.%u.%u\r\n", bootevt->major, bootevt->minor, bootevt->patch);
  local_addr = gecko_cmd_system_get_bt_address()->address;

  printLog("local BT device address: ");
  for (i = 0; i < 5; i++) {
    printLog("%2.2x:", local_addr.addr[5 - i]);
  }
  printLog("%2.2x\r\n", local_addr.addr[0]);
#endif
}
