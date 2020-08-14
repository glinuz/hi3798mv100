/*************************************************************************************
                  Silicon Laboratories Broadcast Si2144 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API commands structures and functions definitions
   FILE: Si2144_Commands.h
   Supported IC : Si2144
   Compiled for ROM 62 firmware 2_0_build_2
   Revision: REVISION_NAME
   Date: November 06 2014
  (C) Copyright 2014, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef    Si2144_COMMANDS_H
#define    Si2144_COMMANDS_H


/* STATUS structure definition */
  typedef struct { /* Si2144_COMMON_REPLY_struct */
    unsigned char   tunint;
    unsigned char   atvint;
    unsigned char   dtvint;
    unsigned char   err;
    unsigned char   cts;
 }  Si2144_COMMON_REPLY_struct;

/* STATUS fields definition */
  /* STATUS, TUNINT field definition (address 0, size 1, lsb 0, unsigned)*/
  #define  Si2144_STATUS_TUNINT_LSB         0
  #define  Si2144_STATUS_TUNINT_MASK        0x01
   #define Si2144_STATUS_TUNINT_NOT_TRIGGERED  0
   #define Si2144_STATUS_TUNINT_TRIGGERED      1
  /* STATUS, ATVINT field definition (address 0, size 1, lsb 1, unsigned)*/
  #define  Si2144_STATUS_ATVINT_LSB         1
  #define  Si2144_STATUS_ATVINT_MASK        0x01
   #define Si2144_STATUS_ATVINT_NOT_TRIGGERED  0
   #define Si2144_STATUS_ATVINT_TRIGGERED      1
  /* STATUS, DTVINT field definition (address 0, size 1, lsb 2, unsigned)*/
  #define  Si2144_STATUS_DTVINT_LSB         2
  #define  Si2144_STATUS_DTVINT_MASK        0x01
   #define Si2144_STATUS_DTVINT_NOT_TRIGGERED  0
   #define Si2144_STATUS_DTVINT_TRIGGERED      1
  /* STATUS, ERR field definition (address 0, size 1, lsb 6, unsigned)*/
  #define  Si2144_STATUS_ERR_LSB         6
  #define  Si2144_STATUS_ERR_MASK        0x01
   #define Si2144_STATUS_ERR_ERROR     1
   #define Si2144_STATUS_ERR_NO_ERROR  0
  /* STATUS, CTS field definition (address 0, size 1, lsb 7, unsigned)*/
  #define  Si2144_STATUS_CTS_LSB         7
  #define  Si2144_STATUS_CTS_MASK        0x01
   #define Si2144_STATUS_CTS_COMPLETED  1
   #define Si2144_STATUS_CTS_WAIT       0


/* Si2144_AGC_OVERRIDE command definition */
#define   Si2144_AGC_OVERRIDE_CMD 0x44

#ifdef    Si2144_AGC_OVERRIDE_CMD
  #define Si2144_AGC_OVERRIDE_CMD_CODE 0x010044

    typedef struct { /* Si2144_AGC_OVERRIDE_CMD_struct */
     unsigned char   force_max_gain;
     unsigned char   force_top_gain;
   } Si2144_AGC_OVERRIDE_CMD_struct;


    typedef struct { /* Si2144_AGC_OVERRIDE_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_AGC_OVERRIDE_CMD_REPLY_struct;

   /* AGC_OVERRIDE command, FORCE_MAX_GAIN field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2144_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_LSB         0
   #define  Si2144_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_MASK        0x01
   #define  Si2144_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_MIN         0
   #define  Si2144_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_MAX         1
    #define Si2144_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_DISABLE  0
    #define Si2144_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_ENABLE   1
   /* AGC_OVERRIDE command, FORCE_TOP_GAIN field definition (address 1,size 1, lsb 1, unsigned) */
   #define  Si2144_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_LSB         1
   #define  Si2144_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_MASK        0x01
   #define  Si2144_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_MIN         0
   #define  Si2144_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_MAX         1
    #define Si2144_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_DISABLE  0
    #define Si2144_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_ENABLE   1
#endif /* Si2144_AGC_OVERRIDE_CMD */

/* Si2144_CONFIG_CLOCKS command definition */
#define   Si2144_CONFIG_CLOCKS_CMD 0xc0

#ifdef    Si2144_CONFIG_CLOCKS_CMD
  #define Si2144_CONFIG_CLOCKS_CMD_CODE 0x0100c0

    typedef struct { /* Si2144_CONFIG_CLOCKS_CMD_struct */
     unsigned char   subcode;
     unsigned char   clock_mode;
     unsigned char   en_xout;
   } Si2144_CONFIG_CLOCKS_CMD_struct;


    typedef struct { /* Si2144_CONFIG_CLOCKS_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_CONFIG_CLOCKS_CMD_REPLY_struct;

   /* CONFIG_CLOCKS command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_CONFIG_CLOCKS_CMD_SUBCODE_LSB         0
   #define  Si2144_CONFIG_CLOCKS_CMD_SUBCODE_MASK        0xff
   #define  Si2144_CONFIG_CLOCKS_CMD_SUBCODE_MIN         0
   #define  Si2144_CONFIG_CLOCKS_CMD_SUBCODE_MAX         0
    #define Si2144_CONFIG_CLOCKS_CMD_SUBCODE_CODE  0
   /* CONFIG_CLOCKS command, CLOCK_MODE field definition (address 2,size 2, lsb 0, unsigned) */
   #define  Si2144_CONFIG_CLOCKS_CMD_CLOCK_MODE_LSB         0
   #define  Si2144_CONFIG_CLOCKS_CMD_CLOCK_MODE_MASK        0x03
   #define  Si2144_CONFIG_CLOCKS_CMD_CLOCK_MODE_MIN         1
   #define  Si2144_CONFIG_CLOCKS_CMD_CLOCK_MODE_MAX         3
    #define Si2144_CONFIG_CLOCKS_CMD_CLOCK_MODE_EXTCLK  3
    #define Si2144_CONFIG_CLOCKS_CMD_CLOCK_MODE_XTAL    1
   /* CONFIG_CLOCKS command, EN_XOUT field definition (address 2,size 3, lsb 2, unsigned) */
   #define  Si2144_CONFIG_CLOCKS_CMD_EN_XOUT_LSB         2
   #define  Si2144_CONFIG_CLOCKS_CMD_EN_XOUT_MASK        0x07
   #define  Si2144_CONFIG_CLOCKS_CMD_EN_XOUT_MIN         0
   #define  Si2144_CONFIG_CLOCKS_CMD_EN_XOUT_MAX         3
    #define Si2144_CONFIG_CLOCKS_CMD_EN_XOUT_DIS_XOUT  0
    #define Si2144_CONFIG_CLOCKS_CMD_EN_XOUT_EN_XOUT   3
#endif /* Si2144_CONFIG_CLOCKS_CMD */

/* Si2144_CONFIG_I2C command definition */
#define   Si2144_CONFIG_I2C_CMD 0xc0

#ifdef    Si2144_CONFIG_I2C_CMD
  #define Si2144_CONFIG_I2C_CMD_CODE 0x0200c0

    typedef struct { /* Si2144_CONFIG_I2C_CMD_struct */
     unsigned char   subcode;
     unsigned char   i2c_broadcast;
   } Si2144_CONFIG_I2C_CMD_struct;


    typedef struct { /* Si2144_CONFIG_I2C_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_CONFIG_I2C_CMD_REPLY_struct;

   /* CONFIG_I2C command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_CONFIG_I2C_CMD_SUBCODE_LSB         0
   #define  Si2144_CONFIG_I2C_CMD_SUBCODE_MASK        0xff
   #define  Si2144_CONFIG_I2C_CMD_SUBCODE_MIN         36
   #define  Si2144_CONFIG_I2C_CMD_SUBCODE_MAX         36
    #define Si2144_CONFIG_I2C_CMD_SUBCODE_CODE  36
   /* CONFIG_I2C command, I2C_BROADCAST field definition (address 2,size 2, lsb 0, unsigned) */
   #define  Si2144_CONFIG_I2C_CMD_I2C_BROADCAST_LSB         0
   #define  Si2144_CONFIG_I2C_CMD_I2C_BROADCAST_MASK        0x03
   #define  Si2144_CONFIG_I2C_CMD_I2C_BROADCAST_MIN         0
   #define  Si2144_CONFIG_I2C_CMD_I2C_BROADCAST_MAX         1
    #define Si2144_CONFIG_I2C_CMD_I2C_BROADCAST_DISABLED  0
    #define Si2144_CONFIG_I2C_CMD_I2C_BROADCAST_ENABLED   1
#endif /* Si2144_CONFIG_I2C_CMD */

/* Si2144_CONFIG_PINS command definition */
#define   Si2144_CONFIG_PINS_CMD 0x12

#ifdef    Si2144_CONFIG_PINS_CMD
  #define Si2144_CONFIG_PINS_CMD_CODE 0x010012

    typedef struct { /* Si2144_CONFIG_PINS_CMD_struct */
     unsigned char   gpio1_mode;
     unsigned char   gpio1_read;
     unsigned char   gpio2_mode;
     unsigned char   gpio2_read;
     unsigned char   agc1_mode;
     unsigned char   agc1_read;
     unsigned char   agc2_mode;
     unsigned char   agc2_read;
     unsigned char   xout_mode;
   } Si2144_CONFIG_PINS_CMD_struct;


    typedef struct { /* Si2144_CONFIG_PINS_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
      unsigned char   gpio1_mode;
      unsigned char   gpio1_state;
      unsigned char   gpio2_mode;
      unsigned char   gpio2_state;
      unsigned char   agc1_mode;
      unsigned char   agc1_state;
      unsigned char   agc2_mode;
      unsigned char   agc2_state;
      unsigned char   xout_mode;
      unsigned char   xout_state;
   }  Si2144_CONFIG_PINS_CMD_REPLY_struct;

   /* CONFIG_PINS command, GPIO1_MODE field definition (address 1,size 7, lsb 0, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_GPIO1_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_CMD_GPIO1_MODE_MASK        0x7f
   #define  Si2144_CONFIG_PINS_CMD_GPIO1_MODE_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_GPIO1_MODE_MAX         9
    #define Si2144_CONFIG_PINS_CMD_GPIO1_MODE_DISABLE          1
    #define Si2144_CONFIG_PINS_CMD_GPIO1_MODE_DRIVE_0          2
    #define Si2144_CONFIG_PINS_CMD_GPIO1_MODE_DRIVE_1          3
    #define Si2144_CONFIG_PINS_CMD_GPIO1_MODE_DTV_FREEZE_HIGH  7
    #define Si2144_CONFIG_PINS_CMD_GPIO1_MODE_DTV_FREEZE_LOW   8
    #define Si2144_CONFIG_PINS_CMD_GPIO1_MODE_NO_CHANGE        0
   /* CONFIG_PINS command, GPIO1_READ field definition (address 1,size 1, lsb 7, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_GPIO1_READ_LSB         7
   #define  Si2144_CONFIG_PINS_CMD_GPIO1_READ_MASK        0x01
   #define  Si2144_CONFIG_PINS_CMD_GPIO1_READ_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_GPIO1_READ_MAX         1
    #define Si2144_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ  0
    #define Si2144_CONFIG_PINS_CMD_GPIO1_READ_READ         1
   /* CONFIG_PINS command, GPIO2_MODE field definition (address 2,size 7, lsb 0, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_GPIO2_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_CMD_GPIO2_MODE_MASK        0x7f
   #define  Si2144_CONFIG_PINS_CMD_GPIO2_MODE_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_GPIO2_MODE_MAX         9
    #define Si2144_CONFIG_PINS_CMD_GPIO2_MODE_DISABLE          1
    #define Si2144_CONFIG_PINS_CMD_GPIO2_MODE_DRIVE_0          2
    #define Si2144_CONFIG_PINS_CMD_GPIO2_MODE_DRIVE_1          3
    #define Si2144_CONFIG_PINS_CMD_GPIO2_MODE_DTV_FREEZE_HIGH  7
    #define Si2144_CONFIG_PINS_CMD_GPIO2_MODE_DTV_FREEZE_LOW   8
    #define Si2144_CONFIG_PINS_CMD_GPIO2_MODE_NO_CHANGE        0
   /* CONFIG_PINS command, GPIO2_READ field definition (address 2,size 1, lsb 7, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_GPIO2_READ_LSB         7
   #define  Si2144_CONFIG_PINS_CMD_GPIO2_READ_MASK        0x01
   #define  Si2144_CONFIG_PINS_CMD_GPIO2_READ_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_GPIO2_READ_MAX         1
    #define Si2144_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ  0
    #define Si2144_CONFIG_PINS_CMD_GPIO2_READ_READ         1
   /* CONFIG_PINS command, AGC1_MODE field definition (address 3,size 7, lsb 0, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_AGC1_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_CMD_AGC1_MODE_MASK        0x7f
   #define  Si2144_CONFIG_PINS_CMD_AGC1_MODE_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_AGC1_MODE_MAX         9
    #define Si2144_CONFIG_PINS_CMD_AGC1_MODE_DISABLE          1
    #define Si2144_CONFIG_PINS_CMD_AGC1_MODE_DRIVE_0          2
    #define Si2144_CONFIG_PINS_CMD_AGC1_MODE_DRIVE_1          3
    #define Si2144_CONFIG_PINS_CMD_AGC1_MODE_DTV_AGC          5
    #define Si2144_CONFIG_PINS_CMD_AGC1_MODE_DTV_FREEZE_HIGH  7
    #define Si2144_CONFIG_PINS_CMD_AGC1_MODE_DTV_FREEZE_LOW   8
    #define Si2144_CONFIG_PINS_CMD_AGC1_MODE_NO_CHANGE        0
   /* CONFIG_PINS command, AGC1_READ field definition (address 3,size 1, lsb 7, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_AGC1_READ_LSB         7
   #define  Si2144_CONFIG_PINS_CMD_AGC1_READ_MASK        0x01
   #define  Si2144_CONFIG_PINS_CMD_AGC1_READ_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_AGC1_READ_MAX         1
    #define Si2144_CONFIG_PINS_CMD_AGC1_READ_DO_NOT_READ  0
    #define Si2144_CONFIG_PINS_CMD_AGC1_READ_READ         1
   /* CONFIG_PINS command, AGC2_MODE field definition (address 5,size 7, lsb 0, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_AGC2_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_CMD_AGC2_MODE_MASK        0x7f
   #define  Si2144_CONFIG_PINS_CMD_AGC2_MODE_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_AGC2_MODE_MAX         8
    #define Si2144_CONFIG_PINS_CMD_AGC2_MODE_DISABLE          1
    #define Si2144_CONFIG_PINS_CMD_AGC2_MODE_DTV_AGC          5
    #define Si2144_CONFIG_PINS_CMD_AGC2_MODE_DTV_FREEZE_HIGH  7
    #define Si2144_CONFIG_PINS_CMD_AGC2_MODE_DTV_FREEZE_LOW   8
    #define Si2144_CONFIG_PINS_CMD_AGC2_MODE_NO_CHANGE        0
   /* CONFIG_PINS command, AGC2_READ field definition (address 5,size 1, lsb 7, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_AGC2_READ_LSB         7
   #define  Si2144_CONFIG_PINS_CMD_AGC2_READ_MASK        0x01
   #define  Si2144_CONFIG_PINS_CMD_AGC2_READ_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_AGC2_READ_MAX         1
    #define Si2144_CONFIG_PINS_CMD_AGC2_READ_DO_NOT_READ  0
    #define Si2144_CONFIG_PINS_CMD_AGC2_READ_READ         1
   /* CONFIG_PINS command, XOUT_MODE field definition (address 6,size 7, lsb 0, unsigned) */
   #define  Si2144_CONFIG_PINS_CMD_XOUT_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_CMD_XOUT_MODE_MASK        0x7f
   #define  Si2144_CONFIG_PINS_CMD_XOUT_MODE_MIN         0
   #define  Si2144_CONFIG_PINS_CMD_XOUT_MODE_MAX         9
    #define Si2144_CONFIG_PINS_CMD_XOUT_MODE_DISABLE        1
    #define Si2144_CONFIG_PINS_CMD_XOUT_MODE_DRIVE_0        2
    #define Si2144_CONFIG_PINS_CMD_XOUT_MODE_DRIVE_1        3
    #define Si2144_CONFIG_PINS_CMD_XOUT_MODE_NO_CHANGE      0
   /* CONFIG_PINS command, GPIO1_MODE field definition (address 1, size 7, lsb 0, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_GPIO1_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_RESPONSE_GPIO1_MODE_MASK        0x7f
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO1_MODE_DISABLE          1
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO1_MODE_DRIVE_0          2
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO1_MODE_DRIVE_1          3
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO1_MODE_DTV_FREEZE_HIGH  7
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO1_MODE_DTV_FREEZE_LOW   8
   /* CONFIG_PINS command, GPIO1_STATE field definition (address 1, size 1, lsb 7, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_GPIO1_STATE_LSB         7
   #define  Si2144_CONFIG_PINS_RESPONSE_GPIO1_STATE_MASK        0x01
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO1_STATE_READ_0  0
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO1_STATE_READ_1  1
   /* CONFIG_PINS command, GPIO2_MODE field definition (address 2, size 7, lsb 0, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_GPIO2_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_RESPONSE_GPIO2_MODE_MASK        0x7f
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO2_MODE_DISABLE          1
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO2_MODE_DRIVE_0          2
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO2_MODE_DRIVE_1          3
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO2_MODE_DTV_FREEZE_HIGH  7
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO2_MODE_DTV_FREEZE_LOW   8
   /* CONFIG_PINS command, GPIO2_STATE field definition (address 2, size 1, lsb 7, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_GPIO2_STATE_LSB         7
   #define  Si2144_CONFIG_PINS_RESPONSE_GPIO2_STATE_MASK        0x01
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO2_STATE_READ_0  0
    #define Si2144_CONFIG_PINS_RESPONSE_GPIO2_STATE_READ_1  1
   /* CONFIG_PINS command, AGC1_MODE field definition (address 3, size 7, lsb 0, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_AGC1_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_RESPONSE_AGC1_MODE_MASK        0x7f
    #define Si2144_CONFIG_PINS_RESPONSE_AGC1_MODE_DISABLE          1
    #define Si2144_CONFIG_PINS_RESPONSE_AGC1_MODE_DRIVE_0          2
    #define Si2144_CONFIG_PINS_RESPONSE_AGC1_MODE_DRIVE_1          3
    #define Si2144_CONFIG_PINS_RESPONSE_AGC1_MODE_DTV_AGC          5
    #define Si2144_CONFIG_PINS_RESPONSE_AGC1_MODE_DTV_FREEZE_HIGH  7
    #define Si2144_CONFIG_PINS_RESPONSE_AGC1_MODE_DTV_FREEZE_LOW   8
   /* CONFIG_PINS command, AGC1_STATE field definition (address 3, size 1, lsb 7, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_AGC1_STATE_LSB         7
   #define  Si2144_CONFIG_PINS_RESPONSE_AGC1_STATE_MASK        0x01
    #define Si2144_CONFIG_PINS_RESPONSE_AGC1_STATE_READ_0  0
    #define Si2144_CONFIG_PINS_RESPONSE_AGC1_STATE_READ_1  1
   /* CONFIG_PINS command, AGC2_MODE field definition (address 5, size 7, lsb 0, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_AGC2_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_RESPONSE_AGC2_MODE_MASK        0x7f
    #define Si2144_CONFIG_PINS_RESPONSE_AGC2_MODE_DISABLE          1
    #define Si2144_CONFIG_PINS_RESPONSE_AGC2_MODE_DTV_AGC          5
    #define Si2144_CONFIG_PINS_RESPONSE_AGC2_MODE_DTV_FREEZE_HIGH  7
    #define Si2144_CONFIG_PINS_RESPONSE_AGC2_MODE_DTV_FREEZE_LOW   8
   /* CONFIG_PINS command, AGC2_STATE field definition (address 5, size 1, lsb 7, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_AGC2_STATE_LSB         7
   #define  Si2144_CONFIG_PINS_RESPONSE_AGC2_STATE_MASK        0x01
    #define Si2144_CONFIG_PINS_RESPONSE_AGC2_STATE_READ_0  0
    #define Si2144_CONFIG_PINS_RESPONSE_AGC2_STATE_READ_1  1
   /* CONFIG_PINS command, XOUT_MODE field definition (address 6, size 7, lsb 0, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_XOUT_MODE_LSB         0
   #define  Si2144_CONFIG_PINS_RESPONSE_XOUT_MODE_MASK        0x7f
    #define Si2144_CONFIG_PINS_RESPONSE_XOUT_MODE_DISABLE        1
    #define Si2144_CONFIG_PINS_RESPONSE_XOUT_MODE_DRIVE_0        2
    #define Si2144_CONFIG_PINS_RESPONSE_XOUT_MODE_DRIVE_1        3
   /* CONFIG_PINS command, XOUT_STATE field definition (address 6, size 1, lsb 7, unsigned)*/
   #define  Si2144_CONFIG_PINS_RESPONSE_XOUT_STATE_LSB         7
   #define  Si2144_CONFIG_PINS_RESPONSE_XOUT_STATE_MASK        0x01
    #define Si2144_CONFIG_PINS_RESPONSE_XOUT_STATE_READ_0  0
    #define Si2144_CONFIG_PINS_RESPONSE_XOUT_STATE_READ_1  1

#endif /* Si2144_CONFIG_PINS_CMD */

/* Si2144_EXIT_BOOTLOADER command definition */
#define   Si2144_EXIT_BOOTLOADER_CMD 0x01

#ifdef    Si2144_EXIT_BOOTLOADER_CMD
  #define Si2144_EXIT_BOOTLOADER_CMD_CODE 0x010001

    typedef struct { /* Si2144_EXIT_BOOTLOADER_CMD_struct */
     unsigned char   func;
     unsigned char   ctsien;
   } Si2144_EXIT_BOOTLOADER_CMD_struct;


    typedef struct { /* Si2144_EXIT_BOOTLOADER_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_EXIT_BOOTLOADER_CMD_REPLY_struct;

   /* EXIT_BOOTLOADER command, FUNC field definition (address 1,size 7, lsb 0, unsigned) */
   #define  Si2144_EXIT_BOOTLOADER_CMD_FUNC_LSB         0
   #define  Si2144_EXIT_BOOTLOADER_CMD_FUNC_MASK        0x7f
   #define  Si2144_EXIT_BOOTLOADER_CMD_FUNC_MIN         1
   #define  Si2144_EXIT_BOOTLOADER_CMD_FUNC_MAX         1
    #define Si2144_EXIT_BOOTLOADER_CMD_FUNC_TUNER  1
   /* EXIT_BOOTLOADER command, CTSIEN field definition (address 1,size 1, lsb 7, unsigned) */
   #define  Si2144_EXIT_BOOTLOADER_CMD_CTSIEN_LSB         7
   #define  Si2144_EXIT_BOOTLOADER_CMD_CTSIEN_MASK        0x01
   #define  Si2144_EXIT_BOOTLOADER_CMD_CTSIEN_MIN         0
   #define  Si2144_EXIT_BOOTLOADER_CMD_CTSIEN_MAX         1
    #define Si2144_EXIT_BOOTLOADER_CMD_CTSIEN_OFF  0
    #define Si2144_EXIT_BOOTLOADER_CMD_CTSIEN_ON   1
#endif /* Si2144_EXIT_BOOTLOADER_CMD */

/* Si2144_FINE_TUNE command definition */
#define   Si2144_FINE_TUNE_CMD 0x45

#ifdef    Si2144_FINE_TUNE_CMD
  #define Si2144_FINE_TUNE_CMD_CODE 0x010045

    typedef struct { /* Si2144_FINE_TUNE_CMD_struct */
     unsigned char   persistence;
              int    offset_500hz;
   } Si2144_FINE_TUNE_CMD_struct;


    typedef struct { /* Si2144_FINE_TUNE_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_FINE_TUNE_CMD_REPLY_struct;

   /* FINE_TUNE command, PERSISTENCE field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2144_FINE_TUNE_CMD_PERSISTENCE_LSB         0
   #define  Si2144_FINE_TUNE_CMD_PERSISTENCE_MASK        0x01
   #define  Si2144_FINE_TUNE_CMD_PERSISTENCE_MIN         0
   #define  Si2144_FINE_TUNE_CMD_PERSISTENCE_MAX         1
    #define Si2144_FINE_TUNE_CMD_PERSISTENCE_NON_PERSISTENT  0
    #define Si2144_FINE_TUNE_CMD_PERSISTENCE_PERSISTENT      1
   /* FINE_TUNE command, OFFSET_500HZ field definition (address 2,size 16, lsb 0, signed) */
   #define  Si2144_FINE_TUNE_CMD_OFFSET_500HZ_LSB         0
   #define  Si2144_FINE_TUNE_CMD_OFFSET_500HZ_MASK        0xffff
   #define  Si2144_FINE_TUNE_CMD_OFFSET_500HZ_SHIFT       16
   #define  Si2144_FINE_TUNE_CMD_OFFSET_500HZ_MIN         -30000
   #define  Si2144_FINE_TUNE_CMD_OFFSET_500HZ_MAX         30000
    #define Si2144_FINE_TUNE_CMD_OFFSET_500HZ_OFFSET_500HZ_MIN  -30000
    #define Si2144_FINE_TUNE_CMD_OFFSET_500HZ_OFFSET_500HZ_MAX  30000
#endif /* Si2144_FINE_TUNE_CMD */

/* Si2144_GET_PROPERTY command definition */
#define   Si2144_GET_PROPERTY_CMD 0x15

#ifdef    Si2144_GET_PROPERTY_CMD
  #define Si2144_GET_PROPERTY_CMD_CODE 0x010015

    typedef struct { /* Si2144_GET_PROPERTY_CMD_struct */
     unsigned char   reserved;
     unsigned int    prop;
   } Si2144_GET_PROPERTY_CMD_struct;


    typedef struct { /* Si2144_GET_PROPERTY_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
      unsigned char   reserved;
      unsigned int    data;
   }  Si2144_GET_PROPERTY_CMD_REPLY_struct;

   /* GET_PROPERTY command, RESERVED field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_GET_PROPERTY_CMD_RESERVED_LSB         0
   #define  Si2144_GET_PROPERTY_CMD_RESERVED_MASK        0xff
   #define  Si2144_GET_PROPERTY_CMD_RESERVED_MIN         0
   #define  Si2144_GET_PROPERTY_CMD_RESERVED_MAX         0
    #define Si2144_GET_PROPERTY_CMD_RESERVED_RESERVED_MIN  0
    #define Si2144_GET_PROPERTY_CMD_RESERVED_RESERVED_MAX  0
   /* GET_PROPERTY command, PROP field definition (address 2,size 16, lsb 0, unsigned) */
   #define  Si2144_GET_PROPERTY_CMD_PROP_LSB         0
   #define  Si2144_GET_PROPERTY_CMD_PROP_MASK        0xffff
   #define  Si2144_GET_PROPERTY_CMD_PROP_MIN         0
   #define  Si2144_GET_PROPERTY_CMD_PROP_MAX         65535
    #define Si2144_GET_PROPERTY_CMD_PROP_PROP_MIN  0
    #define Si2144_GET_PROPERTY_CMD_PROP_PROP_MAX  65535
   /* GET_PROPERTY command, RESERVED field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2144_GET_PROPERTY_RESPONSE_RESERVED_LSB         0
   #define  Si2144_GET_PROPERTY_RESPONSE_RESERVED_MASK        0xff
   /* GET_PROPERTY command, DATA field definition (address 2, size 16, lsb 0, unsigned)*/
   #define  Si2144_GET_PROPERTY_RESPONSE_DATA_LSB         0
   #define  Si2144_GET_PROPERTY_RESPONSE_DATA_MASK        0xffff

#endif /* Si2144_GET_PROPERTY_CMD */

/* Si2144_GET_REV command definition */
#define   Si2144_GET_REV_CMD 0x11

#ifdef    Si2144_GET_REV_CMD
  #define Si2144_GET_REV_CMD_CODE 0x010011

    typedef struct { /* Si2144_GET_REV_CMD_struct */
         unsigned char   nothing;   } Si2144_GET_REV_CMD_struct;


    typedef struct { /* Si2144_GET_REV_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
      unsigned char   pn;
      unsigned char   pn2;
      unsigned char   fwmajor;
      unsigned char   fwminor;
      unsigned int    patch;
      unsigned char   cmpmajor;
      unsigned char   cmpminor;
      unsigned char   cmpbuild;
      unsigned char   chiprev;
   }  Si2144_GET_REV_CMD_REPLY_struct;

   /* GET_REV command, PN field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_PN_LSB         0
   #define  Si2144_GET_REV_RESPONSE_PN_MASK        0xff
   /* GET_REV command, PN2 field definition (address 12, size 8, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_PN2_LSB         0
   #define  Si2144_GET_REV_RESPONSE_PN2_MASK        0xff
   /* GET_REV command, FWMAJOR field definition (address 2, size 8, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_FWMAJOR_LSB         0
   #define  Si2144_GET_REV_RESPONSE_FWMAJOR_MASK        0xff
   /* GET_REV command, FWMINOR field definition (address 3, size 8, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_FWMINOR_LSB         0
   #define  Si2144_GET_REV_RESPONSE_FWMINOR_MASK        0xff
   /* GET_REV command, PATCH field definition (address 4, size 16, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_PATCH_LSB         0
   #define  Si2144_GET_REV_RESPONSE_PATCH_MASK        0xffff
   /* GET_REV command, CMPMAJOR field definition (address 6, size 8, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_CMPMAJOR_LSB         0
   #define  Si2144_GET_REV_RESPONSE_CMPMAJOR_MASK        0xff
   /* GET_REV command, CMPMINOR field definition (address 7, size 8, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_CMPMINOR_LSB         0
   #define  Si2144_GET_REV_RESPONSE_CMPMINOR_MASK        0xff
   /* GET_REV command, CMPBUILD field definition (address 8, size 8, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_CMPBUILD_LSB         0
   #define  Si2144_GET_REV_RESPONSE_CMPBUILD_MASK        0xff
    #define Si2144_GET_REV_RESPONSE_CMPBUILD_CMPBUILD_MIN  0
    #define Si2144_GET_REV_RESPONSE_CMPBUILD_CMPBUILD_MAX  255
   /* GET_REV command, CHIPREV field definition (address 9, size 4, lsb 0, unsigned)*/
   #define  Si2144_GET_REV_RESPONSE_CHIPREV_LSB         0
   #define  Si2144_GET_REV_RESPONSE_CHIPREV_MASK        0x0f
    #define Si2144_GET_REV_RESPONSE_CHIPREV_A  1
    #define Si2144_GET_REV_RESPONSE_CHIPREV_B  2

#endif /* Si2144_GET_REV_CMD */

/* Si2144_PART_INFO command definition */
#define   Si2144_PART_INFO_CMD 0x02

#ifdef    Si2144_PART_INFO_CMD
  #define Si2144_PART_INFO_CMD_CODE 0x010002

    typedef struct { /* Si2144_PART_INFO_CMD_struct */
         unsigned char   nothing;   } Si2144_PART_INFO_CMD_struct;


    typedef struct { /* Si2144_PART_INFO_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
      unsigned char   chiprev;
      unsigned char   romid;
      unsigned char   part2;
      unsigned char   part;
      unsigned char   pmajor;
      unsigned char   pminor;
      unsigned char   pbuild;
      unsigned int    reserved;
      unsigned long   serial;
   }  Si2144_PART_INFO_CMD_REPLY_struct;

   /* PART_INFO command, CHIPREV field definition (address 1, size 4, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_CHIPREV_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_CHIPREV_MASK        0x0f
    #define Si2144_PART_INFO_RESPONSE_CHIPREV_A  1
    #define Si2144_PART_INFO_RESPONSE_CHIPREV_B  2
   /* PART_INFO command, ROMID field definition (address 12, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_ROMID_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_ROMID_MASK        0xff
   /* PART_INFO command, PART2 field definition (address 13, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_PART2_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_PART2_MASK        0xff
   /* PART_INFO command, PART field definition (address 2, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_PART_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_PART_MASK        0xff
   /* PART_INFO command, PMAJOR field definition (address 3, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_PMAJOR_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_PMAJOR_MASK        0xff
   /* PART_INFO command, PMINOR field definition (address 4, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_PMINOR_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_PMINOR_MASK        0xff
   /* PART_INFO command, PBUILD field definition (address 5, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_PBUILD_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_PBUILD_MASK        0xff
   /* PART_INFO command, RESERVED field definition (address 6, size 16, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_RESERVED_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_RESERVED_MASK        0xffff
   /* PART_INFO command, SERIAL field definition (address 8, size 32, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO_RESPONSE_SERIAL_LSB         0
   #define  Si2144_PART_INFO_RESPONSE_SERIAL_MASK        0xffffffff

#endif /* Si2144_PART_INFO_CMD */

/* Si2144_PART_INFO2 command definition */
#define   Si2144_PART_INFO2_CMD 0x0e

#ifdef    Si2144_PART_INFO2_CMD
  #define Si2144_PART_INFO2_CMD_CODE 0x01000e

    typedef struct { /* Si2144_PART_INFO2_CMD_struct */
         unsigned char   nothing;   } Si2144_PART_INFO2_CMD_struct;


    typedef struct { /* Si2144_PART_INFO2_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
      unsigned char   romid;
      unsigned char   part2;
      unsigned char   reserved;
      unsigned long   serial;
   }  Si2144_PART_INFO2_CMD_REPLY_struct;

   /* PART_INFO2 command, ROMID field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO2_RESPONSE_ROMID_LSB         0
   #define  Si2144_PART_INFO2_RESPONSE_ROMID_MASK        0xff
   /* PART_INFO2 command, PART2 field definition (address 2, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO2_RESPONSE_PART2_LSB         0
   #define  Si2144_PART_INFO2_RESPONSE_PART2_MASK        0xff
   /* PART_INFO2 command, RESERVED field definition (address 3, size 8, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO2_RESPONSE_RESERVED_LSB         0
   #define  Si2144_PART_INFO2_RESPONSE_RESERVED_MASK        0xff
   /* PART_INFO2 command, SERIAL field definition (address 4, size 32, lsb 0, unsigned)*/
   #define  Si2144_PART_INFO2_RESPONSE_SERIAL_LSB         0
   #define  Si2144_PART_INFO2_RESPONSE_SERIAL_MASK        0xffffffff

#endif /* Si2144_PART_INFO2_CMD */

/* Si2144_POLL_CTS command definition */
#define   Si2144_POLL_CTS_CMD 0xfe

#ifdef    Si2144_POLL_CTS_CMD
  #define Si2144_POLL_CTS_CMD_CODE 0x0100fe

    typedef struct { /* Si2144_POLL_CTS_CMD_struct */
         unsigned char   nothing;   } Si2144_POLL_CTS_CMD_struct;


    typedef struct { /* Si2144_POLL_CTS_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_POLL_CTS_CMD_REPLY_struct;

#endif /* Si2144_POLL_CTS_CMD */

/* Si2144_POWER_DOWN_HW command definition */
#define   Si2144_POWER_DOWN_HW_CMD 0xc0

#ifdef    Si2144_POWER_DOWN_HW_CMD
  #define Si2144_POWER_DOWN_HW_CMD_CODE 0x0300c0

    typedef struct { /* Si2144_POWER_DOWN_HW_CMD_struct */
     unsigned char   subcode;
     unsigned char   pdb_xo_osc;
     unsigned char   reserved1;
     unsigned char   en_xout;
     unsigned char   reserved2;
     unsigned char   reserved3;
     unsigned char   reserved4;
     unsigned char   reserved5;
     unsigned char   reserved6;
     unsigned char   reserved7;
     unsigned char   reserved8;
     unsigned char   reserved9;
   } Si2144_POWER_DOWN_HW_CMD_struct;


    typedef struct { /* Si2144_POWER_DOWN_HW_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_POWER_DOWN_HW_CMD_REPLY_struct;

   /* POWER_DOWN_HW command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_SUBCODE_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_SUBCODE_MASK        0xff
   #define  Si2144_POWER_DOWN_HW_CMD_SUBCODE_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_SUBCODE_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_SUBCODE_CODE  0
   /* POWER_DOWN_HW command, PDB_XO_OSC field definition (address 2,size 1, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_PDB_XO_OSC_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_PDB_XO_OSC_MASK        0x01
   #define  Si2144_POWER_DOWN_HW_CMD_PDB_XO_OSC_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_PDB_XO_OSC_MAX         1
    #define Si2144_POWER_DOWN_HW_CMD_PDB_XO_OSC_XO_OSC_POWER_DOWN  0
    #define Si2144_POWER_DOWN_HW_CMD_PDB_XO_OSC_XO_OSC_POWER_UP    1
   /* POWER_DOWN_HW command, RESERVED1 field definition (address 2,size 1, lsb 1, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED1_LSB         1
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED1_MASK        0x01
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED1_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED1_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED1_RESERVED  0
   /* POWER_DOWN_HW command, EN_XOUT field definition (address 2,size 3, lsb 2, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_EN_XOUT_LSB         2
   #define  Si2144_POWER_DOWN_HW_CMD_EN_XOUT_MASK        0x07
   #define  Si2144_POWER_DOWN_HW_CMD_EN_XOUT_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_EN_XOUT_MAX         3
    #define Si2144_POWER_DOWN_HW_CMD_EN_XOUT_DIS_XOUT  0
    #define Si2144_POWER_DOWN_HW_CMD_EN_XOUT_EN_XOUT   3
   /* POWER_DOWN_HW command, RESERVED2 field definition (address 2,size 3, lsb 5, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED2_LSB         5
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED2_MASK        0x07
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED2_MIN         1
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED2_MAX         1
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED2_RESERVED  1
   /* POWER_DOWN_HW command, RESERVED3 field definition (address 3,size 4, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED3_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED3_MASK        0x0f
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED3_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED3_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED3_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED4 field definition (address 4,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED4_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED4_MASK        0xff
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED4_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED4_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED4_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED5 field definition (address 5,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED5_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED5_MASK        0xff
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED5_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED5_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED5_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED6 field definition (address 6,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED6_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED6_MASK        0xff
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED6_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED6_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED6_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED7 field definition (address 7,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED7_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED7_MASK        0xff
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED7_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED7_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED7_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED8 field definition (address 8,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED8_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED8_MASK        0xff
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED8_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED8_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED8_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED9 field definition (address 9,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED9_LSB         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED9_MASK        0xff
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED9_MIN         0
   #define  Si2144_POWER_DOWN_HW_CMD_RESERVED9_MAX         0
    #define Si2144_POWER_DOWN_HW_CMD_RESERVED9_RESERVED  0
#endif /* Si2144_POWER_DOWN_HW_CMD */

/* Si2144_POWER_UP command definition */
#define   Si2144_POWER_UP_CMD 0xc0

#ifdef    Si2144_POWER_UP_CMD
  #define Si2144_POWER_UP_CMD_CODE 0x0400c0

    typedef struct { /* Si2144_POWER_UP_CMD_struct */
     unsigned char   subcode;
     unsigned char   clock_mode;
     unsigned char   en_xout;
     unsigned char   config_1p8v;
     unsigned char   reserved3;
     unsigned char   reserved4;
     unsigned char   reserved5;
     unsigned char   reserved6;
     unsigned char   reserved7;
     unsigned char   reserved8;
   } Si2144_POWER_UP_CMD_struct;


    typedef struct { /* Si2144_POWER_UP_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_POWER_UP_CMD_REPLY_struct;

   /* POWER_UP command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_SUBCODE_LSB         0
   #define  Si2144_POWER_UP_CMD_SUBCODE_MASK        0xff
   #define  Si2144_POWER_UP_CMD_SUBCODE_MIN         0
   #define  Si2144_POWER_UP_CMD_SUBCODE_MAX         0
    #define Si2144_POWER_UP_CMD_SUBCODE_CODE  0
   /* POWER_UP command, CLOCK_MODE field definition (address 2,size 2, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_CLOCK_MODE_LSB         0
   #define  Si2144_POWER_UP_CMD_CLOCK_MODE_MASK        0x03
   #define  Si2144_POWER_UP_CMD_CLOCK_MODE_MIN         1
   #define  Si2144_POWER_UP_CMD_CLOCK_MODE_MAX         3
    #define Si2144_POWER_UP_CMD_CLOCK_MODE_EXTCLK  3
    #define Si2144_POWER_UP_CMD_CLOCK_MODE_XTAL    1
   /* POWER_UP command, EN_XOUT field definition (address 2,size 3, lsb 2, unsigned) */
   #define  Si2144_POWER_UP_CMD_EN_XOUT_LSB         2
   #define  Si2144_POWER_UP_CMD_EN_XOUT_MASK        0x07
   #define  Si2144_POWER_UP_CMD_EN_XOUT_MIN         0
   #define  Si2144_POWER_UP_CMD_EN_XOUT_MAX         3
    #define Si2144_POWER_UP_CMD_EN_XOUT_DIS_XOUT  0
    #define Si2144_POWER_UP_CMD_EN_XOUT_EN_XOUT   3
   /* POWER_UP command, CONFIG_1P8V field definition (address 3,size 4, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_CONFIG_1P8V_LSB         0
   #define  Si2144_POWER_UP_CMD_CONFIG_1P8V_MASK        0x0f
   #define  Si2144_POWER_UP_CMD_CONFIG_1P8V_MIN         6
   #define  Si2144_POWER_UP_CMD_CONFIG_1P8V_MAX         14
    #define Si2144_POWER_UP_CMD_CONFIG_1P8V_EXTERNAL_PNP_LDO  7
    #define Si2144_POWER_UP_CMD_CONFIG_1P8V_EXTERNAL_SUPPLY   6
    #define Si2144_POWER_UP_CMD_CONFIG_1P8V_INTERNAL_REG      14
   /* POWER_UP command, RESERVED3 field definition (address 4,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_RESERVED3_LSB         0
   #define  Si2144_POWER_UP_CMD_RESERVED3_MASK        0xff
   #define  Si2144_POWER_UP_CMD_RESERVED3_MIN         0
   #define  Si2144_POWER_UP_CMD_RESERVED3_MAX         0
    #define Si2144_POWER_UP_CMD_RESERVED3_RESERVED  0
   /* POWER_UP command, RESERVED4 field definition (address 5,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_RESERVED4_LSB         0
   #define  Si2144_POWER_UP_CMD_RESERVED4_MASK        0xff
   #define  Si2144_POWER_UP_CMD_RESERVED4_MIN         1
   #define  Si2144_POWER_UP_CMD_RESERVED4_MAX         1
    #define Si2144_POWER_UP_CMD_RESERVED4_RESERVED  1
   /* POWER_UP command, RESERVED5 field definition (address 6,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_RESERVED5_LSB         0
   #define  Si2144_POWER_UP_CMD_RESERVED5_MASK        0xff
   #define  Si2144_POWER_UP_CMD_RESERVED5_MIN         1
   #define  Si2144_POWER_UP_CMD_RESERVED5_MAX         1
    #define Si2144_POWER_UP_CMD_RESERVED5_RESERVED  1
   /* POWER_UP command, RESERVED6 field definition (address 7,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_RESERVED6_LSB         0
   #define  Si2144_POWER_UP_CMD_RESERVED6_MASK        0xff
   #define  Si2144_POWER_UP_CMD_RESERVED6_MIN         1
   #define  Si2144_POWER_UP_CMD_RESERVED6_MAX         1
    #define Si2144_POWER_UP_CMD_RESERVED6_RESERVED  1
   /* POWER_UP command, RESERVED7 field definition (address 8,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_RESERVED7_LSB         0
   #define  Si2144_POWER_UP_CMD_RESERVED7_MASK        0xff
   #define  Si2144_POWER_UP_CMD_RESERVED7_MIN         1
   #define  Si2144_POWER_UP_CMD_RESERVED7_MAX         1
    #define Si2144_POWER_UP_CMD_RESERVED7_RESERVED  1
   /* POWER_UP command, RESERVED8 field definition (address 9,size 8, lsb 0, unsigned) */
   #define  Si2144_POWER_UP_CMD_RESERVED8_LSB         0
   #define  Si2144_POWER_UP_CMD_RESERVED8_MASK        0xff
   #define  Si2144_POWER_UP_CMD_RESERVED8_MIN         3
   #define  Si2144_POWER_UP_CMD_RESERVED8_MAX         3
    #define Si2144_POWER_UP_CMD_RESERVED8_RESERVED  3
#endif /* Si2144_POWER_UP_CMD */

/* Si2144_REREAD_REPLY command definition */
#define   Si2144_REREAD_REPLY_CMD 0x17

#ifdef    Si2144_REREAD_REPLY_CMD
  #define Si2144_REREAD_REPLY_CMD_CODE 0x010017

    typedef struct { /* Si2144_REREAD_REPLY_CMD_struct */
     unsigned char   offset;
   } Si2144_REREAD_REPLY_CMD_struct;


    typedef struct { /* Si2144_REREAD_REPLY_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
      unsigned char   reply1;
      unsigned char   reply10;
      unsigned char   reply11;
      unsigned char   reply12;
      unsigned char   reply13;
      unsigned char   reply14;
      unsigned char   reply15;
      unsigned char   reply2;
      unsigned char   reply3;
      unsigned char   reply4;
      unsigned char   reply5;
      unsigned char   reply6;
      unsigned char   reply7;
      unsigned char   reply8;
      unsigned char   reply9;
   }  Si2144_REREAD_REPLY_CMD_REPLY_struct;

   /* REREAD_REPLY command, OFFSET field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_REREAD_REPLY_CMD_OFFSET_LSB         0
   #define  Si2144_REREAD_REPLY_CMD_OFFSET_MASK        0xff
   #define  Si2144_REREAD_REPLY_CMD_OFFSET_MIN         0
   #define  Si2144_REREAD_REPLY_CMD_OFFSET_MAX         14
    #define Si2144_REREAD_REPLY_CMD_OFFSET_OFFSET_MIN  0
    #define Si2144_REREAD_REPLY_CMD_OFFSET_OFFSET_MAX  14
   /* REREAD_REPLY command, REPLY1 field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY1_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY1_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY1_REPLY1_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY1_REPLY1_MAX  255
   /* REREAD_REPLY command, REPLY10 field definition (address 10, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY10_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY10_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY10_REPLY10_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY10_REPLY10_MAX  255
   /* REREAD_REPLY command, REPLY11 field definition (address 11, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY11_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY11_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY11_REPLY11_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY11_REPLY11_MAX  255
   /* REREAD_REPLY command, REPLY12 field definition (address 12, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY12_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY12_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY12_REPLY12_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY12_REPLY12_MAX  255
   /* REREAD_REPLY command, REPLY13 field definition (address 13, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY13_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY13_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY13_REPLY13_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY13_REPLY13_MAX  255
   /* REREAD_REPLY command, REPLY14 field definition (address 14, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY14_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY14_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY14_REPLY14_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY14_REPLY14_MAX  255
   /* REREAD_REPLY command, REPLY15 field definition (address 15, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY15_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY15_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY15_REPLY15_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY15_REPLY15_MAX  255
   /* REREAD_REPLY command, REPLY2 field definition (address 2, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY2_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY2_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY2_REPLY2_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY2_REPLY2_MAX  255
   /* REREAD_REPLY command, REPLY3 field definition (address 3, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY3_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY3_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY3_REPLY3_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY3_REPLY3_MAX  255
   /* REREAD_REPLY command, REPLY4 field definition (address 4, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY4_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY4_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY4_REPLY4_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY4_REPLY4_MAX  255
   /* REREAD_REPLY command, REPLY5 field definition (address 5, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY5_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY5_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY5_REPLY5_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY5_REPLY5_MAX  255
   /* REREAD_REPLY command, REPLY6 field definition (address 6, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY6_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY6_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY6_REPLY6_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY6_REPLY6_MAX  255
   /* REREAD_REPLY command, REPLY7 field definition (address 7, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY7_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY7_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY7_REPLY7_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY7_REPLY7_MAX  255
   /* REREAD_REPLY command, REPLY8 field definition (address 8, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY8_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY8_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY8_REPLY8_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY8_REPLY8_MAX  255
   /* REREAD_REPLY command, REPLY9 field definition (address 9, size 8, lsb 0, unsigned)*/
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY9_LSB         0
   #define  Si2144_REREAD_REPLY_RESPONSE_REPLY9_MASK        0xff
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY9_REPLY9_MIN  0
    #define Si2144_REREAD_REPLY_RESPONSE_REPLY9_REPLY9_MAX  255

#endif /* Si2144_REREAD_REPLY_CMD */

/* Si2144_RESET_HW command definition */
#define   Si2144_RESET_HW_CMD 0xc0

#ifdef    Si2144_RESET_HW_CMD
  #define Si2144_RESET_HW_CMD_CODE 0x0700c0

    typedef struct { /* Si2144_RESET_HW_CMD_struct */
     unsigned char   subcode;
     unsigned char   reserved1;
     unsigned char   reserved2;
   } Si2144_RESET_HW_CMD_struct;


    typedef struct { /* Si2144_RESET_HW_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_RESET_HW_CMD_REPLY_struct;

   /* RESET_HW command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_RESET_HW_CMD_SUBCODE_LSB         0
   #define  Si2144_RESET_HW_CMD_SUBCODE_MASK        0xff
   #define  Si2144_RESET_HW_CMD_SUBCODE_MIN         5
   #define  Si2144_RESET_HW_CMD_SUBCODE_MAX         5
    #define Si2144_RESET_HW_CMD_SUBCODE_CODE  5
   /* RESET_HW command, RESERVED1 field definition (address 2,size 8, lsb 0, unsigned) */
   #define  Si2144_RESET_HW_CMD_RESERVED1_LSB         0
   #define  Si2144_RESET_HW_CMD_RESERVED1_MASK        0xff
   #define  Si2144_RESET_HW_CMD_RESERVED1_MIN         0
   #define  Si2144_RESET_HW_CMD_RESERVED1_MAX         0
    #define Si2144_RESET_HW_CMD_RESERVED1_RESERVED  0
   /* RESET_HW command, RESERVED2 field definition (address 3,size 8, lsb 0, unsigned) */
   #define  Si2144_RESET_HW_CMD_RESERVED2_LSB         0
   #define  Si2144_RESET_HW_CMD_RESERVED2_MASK        0xff
   #define  Si2144_RESET_HW_CMD_RESERVED2_MIN         0
   #define  Si2144_RESET_HW_CMD_RESERVED2_MAX         0
    #define Si2144_RESET_HW_CMD_RESERVED2_RESERVED  0
#endif /* Si2144_RESET_HW_CMD */

/* Si2144_SET_PROPERTY command definition */
#define   Si2144_SET_PROPERTY_CMD 0x14

#ifdef    Si2144_SET_PROPERTY_CMD
  #define Si2144_SET_PROPERTY_CMD_CODE 0x010014

    typedef struct { /* Si2144_SET_PROPERTY_CMD_struct */
     unsigned char   reserved;
     unsigned int    prop;
     unsigned int    data;
   } Si2144_SET_PROPERTY_CMD_struct;


    typedef struct { /* Si2144_SET_PROPERTY_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
      unsigned char   reserved;
      unsigned int    data;
   }  Si2144_SET_PROPERTY_CMD_REPLY_struct;

   /* SET_PROPERTY command, RESERVED field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_SET_PROPERTY_CMD_RESERVED_LSB         0
   #define  Si2144_SET_PROPERTY_CMD_RESERVED_MASK        0xff
   #define  Si2144_SET_PROPERTY_CMD_RESERVED_MIN         0
   #define  Si2144_SET_PROPERTY_CMD_RESERVED_MAX         255.0
   /* SET_PROPERTY command, PROP field definition (address 2,size 16, lsb 0, unsigned) */
   #define  Si2144_SET_PROPERTY_CMD_PROP_LSB         0
   #define  Si2144_SET_PROPERTY_CMD_PROP_MASK        0xffff
   #define  Si2144_SET_PROPERTY_CMD_PROP_MIN         0
   #define  Si2144_SET_PROPERTY_CMD_PROP_MAX         65535
    #define Si2144_SET_PROPERTY_CMD_PROP_PROP_MIN  0
    #define Si2144_SET_PROPERTY_CMD_PROP_PROP_MAX  65535
   /* SET_PROPERTY command, DATA field definition (address 4,size 16, lsb 0, unsigned) */
   #define  Si2144_SET_PROPERTY_CMD_DATA_LSB         0
   #define  Si2144_SET_PROPERTY_CMD_DATA_MASK        0xffff
   #define  Si2144_SET_PROPERTY_CMD_DATA_MIN         0
   #define  Si2144_SET_PROPERTY_CMD_DATA_MAX         65535
    #define Si2144_SET_PROPERTY_CMD_DATA_DATA_MIN  0
    #define Si2144_SET_PROPERTY_CMD_DATA_DATA_MAX  65535
   /* SET_PROPERTY command, RESERVED field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2144_SET_PROPERTY_RESPONSE_RESERVED_LSB         0
   #define  Si2144_SET_PROPERTY_RESPONSE_RESERVED_MASK        0xff
    #define Si2144_SET_PROPERTY_RESPONSE_RESERVED_RESERVED_MIN  0
    #define Si2144_SET_PROPERTY_RESPONSE_RESERVED_RESERVED_MAX  0
   /* SET_PROPERTY command, DATA field definition (address 2, size 16, lsb 0, unsigned)*/
   #define  Si2144_SET_PROPERTY_RESPONSE_DATA_LSB         0
   #define  Si2144_SET_PROPERTY_RESPONSE_DATA_MASK        0xffff

#endif /* Si2144_SET_PROPERTY_CMD */

/* Si2144_STANDBY command definition */
#define   Si2144_STANDBY_CMD 0x16

#ifdef    Si2144_STANDBY_CMD
  #define Si2144_STANDBY_CMD_CODE 0x010016

    typedef struct { /* Si2144_STANDBY_CMD_struct */
     unsigned char   type;
   } Si2144_STANDBY_CMD_struct;


    typedef struct { /* Si2144_STANDBY_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_STANDBY_CMD_REPLY_struct;

   /* STANDBY command, TYPE field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2144_STANDBY_CMD_TYPE_LSB         0
   #define  Si2144_STANDBY_CMD_TYPE_MASK        0x01
   #define  Si2144_STANDBY_CMD_TYPE_MIN         0
   #define  Si2144_STANDBY_CMD_TYPE_MAX         1
    #define Si2144_STANDBY_CMD_TYPE_LNA_OFF  1
    #define Si2144_STANDBY_CMD_TYPE_LNA_ON   0
#endif /* Si2144_STANDBY_CMD */

/* Si2144_TUNER_STATUS command definition */
#define   Si2144_TUNER_STATUS_CMD 0x42

#ifdef    Si2144_TUNER_STATUS_CMD
  #define Si2144_TUNER_STATUS_CMD_CODE 0x010042

    typedef struct { /* Si2144_TUNER_STATUS_CMD_struct */
         unsigned char   nothing;   } Si2144_TUNER_STATUS_CMD_struct;


    typedef struct { /* Si2144_TUNER_STATUS_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
      unsigned char   rsvd1;
               int    vco_code;
      unsigned char   rsvd2;
               char   rssi;
      unsigned long   freq;
      unsigned char   mode;
      unsigned char   wb_att_status;
   }  Si2144_TUNER_STATUS_CMD_REPLY_struct;

   /* TUNER_STATUS command, RSVD1 field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2144_TUNER_STATUS_RESPONSE_RSVD1_LSB         0
   #define  Si2144_TUNER_STATUS_RESPONSE_RSVD1_MASK        0xff
   /* TUNER_STATUS command, VCO_CODE field definition (address 10, size 16, lsb 0, signed)*/
   #define  Si2144_TUNER_STATUS_RESPONSE_VCO_CODE_LSB         0
   #define  Si2144_TUNER_STATUS_RESPONSE_VCO_CODE_MASK        0xffff
   #define  Si2144_TUNER_STATUS_RESPONSE_VCO_CODE_SHIFT       16
   /* TUNER_STATUS command, RSVD2 field definition (address 2, size 8, lsb 0, unsigned)*/
   #define  Si2144_TUNER_STATUS_RESPONSE_RSVD2_LSB         0
   #define  Si2144_TUNER_STATUS_RESPONSE_RSVD2_MASK        0xff
   /* TUNER_STATUS command, RSSI field definition (address 3, size 8, lsb 0, signed)*/
   #define  Si2144_TUNER_STATUS_RESPONSE_RSSI_LSB         0
   #define  Si2144_TUNER_STATUS_RESPONSE_RSSI_MASK        0xff
   #define  Si2144_TUNER_STATUS_RESPONSE_RSSI_SHIFT       24
   /* TUNER_STATUS command, FREQ field definition (address 4, size 32, lsb 0, unsigned)*/
   #define  Si2144_TUNER_STATUS_RESPONSE_FREQ_LSB         0
   #define  Si2144_TUNER_STATUS_RESPONSE_FREQ_MASK        0xffffffff
   /* TUNER_STATUS command, MODE field definition (address 8, size 1, lsb 0, unsigned)*/
   #define  Si2144_TUNER_STATUS_RESPONSE_MODE_LSB         0
   #define  Si2144_TUNER_STATUS_RESPONSE_MODE_MASK        0x01
    #define Si2144_TUNER_STATUS_RESPONSE_MODE_DTV  0
   /* TUNER_STATUS command, WB_ATT_STATUS field definition (address 8, size 1, lsb 1, unsigned)*/
   #define  Si2144_TUNER_STATUS_RESPONSE_WB_ATT_STATUS_LSB         1
   #define  Si2144_TUNER_STATUS_RESPONSE_WB_ATT_STATUS_MASK        0x01
    #define Si2144_TUNER_STATUS_RESPONSE_WB_ATT_STATUS_OFF  0
    #define Si2144_TUNER_STATUS_RESPONSE_WB_ATT_STATUS_ON   1

#endif /* Si2144_TUNER_STATUS_CMD */

/* Si2144_TUNER_TUNE_FREQ command definition */
#define   Si2144_TUNER_TUNE_FREQ_CMD 0x41

#ifdef    Si2144_TUNER_TUNE_FREQ_CMD
  #define Si2144_TUNER_TUNE_FREQ_CMD_CODE 0x010041

    typedef struct { /* Si2144_TUNER_TUNE_FREQ_CMD_struct */
     unsigned char   mode;
     unsigned long   freq;
   } Si2144_TUNER_TUNE_FREQ_CMD_struct;


    typedef struct { /* Si2144_TUNER_TUNE_FREQ_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_TUNER_TUNE_FREQ_CMD_REPLY_struct;

   /* TUNER_TUNE_FREQ command, MODE field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2144_TUNER_TUNE_FREQ_CMD_MODE_LSB         0
   #define  Si2144_TUNER_TUNE_FREQ_CMD_MODE_MASK        0x01
   #define  Si2144_TUNER_TUNE_FREQ_CMD_MODE_MIN         0
   #define  Si2144_TUNER_TUNE_FREQ_CMD_MODE_MAX         1
    #define Si2144_TUNER_TUNE_FREQ_CMD_MODE_DTV  0
   /* TUNER_TUNE_FREQ command, FREQ field definition (address 4,size 32, lsb 0, unsigned) */
   #define  Si2144_TUNER_TUNE_FREQ_CMD_FREQ_LSB         0
   #define  Si2144_TUNER_TUNE_FREQ_CMD_FREQ_MASK        0xffffffff
   #define  Si2144_TUNER_TUNE_FREQ_CMD_FREQ_MIN         40000000
   #define  Si2144_TUNER_TUNE_FREQ_CMD_FREQ_MAX         1002000000
    #define Si2144_TUNER_TUNE_FREQ_CMD_FREQ_FREQ_MIN  40000000
    #define Si2144_TUNER_TUNE_FREQ_CMD_FREQ_FREQ_MAX  1002000000
#endif /* Si2144_TUNER_TUNE_FREQ_CMD */

/* Si2144_WAKE_UP command definition */
#define   Si2144_WAKE_UP_CMD 0xc0

#ifdef    Si2144_WAKE_UP_CMD
  #define Si2144_WAKE_UP_CMD_CODE 0x0800c0

    typedef struct { /* Si2144_WAKE_UP_CMD_struct */
     unsigned char   subcode;
     unsigned char   reset;
     unsigned char   clock_freq;
     unsigned char   reserved9;
     unsigned char   func;
     unsigned char   wake_up;
   } Si2144_WAKE_UP_CMD_struct;


    typedef struct { /* Si2144_WAKE_UP_CMD_REPLY_struct */
       Si2144_COMMON_REPLY_struct * STATUS;
   }  Si2144_WAKE_UP_CMD_REPLY_struct;

   /* WAKE_UP command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2144_WAKE_UP_CMD_SUBCODE_LSB         0
   #define  Si2144_WAKE_UP_CMD_SUBCODE_MASK        0xff
   #define  Si2144_WAKE_UP_CMD_SUBCODE_MIN         8
   #define  Si2144_WAKE_UP_CMD_SUBCODE_MAX         8
    #define Si2144_WAKE_UP_CMD_SUBCODE_CODE  8
   /* WAKE_UP command, RESET field definition (address 2,size 8, lsb 0, unsigned) */
   #define  Si2144_WAKE_UP_CMD_RESET_LSB         0
   #define  Si2144_WAKE_UP_CMD_RESET_MASK        0xff
   #define  Si2144_WAKE_UP_CMD_RESET_MIN         1
   #define  Si2144_WAKE_UP_CMD_RESET_MAX         1
    #define Si2144_WAKE_UP_CMD_RESET_RESET  1
   /* WAKE_UP command, CLOCK_FREQ field definition (address 3,size 2, lsb 0, unsigned) */
   #define  Si2144_WAKE_UP_CMD_CLOCK_FREQ_LSB         0
   #define  Si2144_WAKE_UP_CMD_CLOCK_FREQ_MASK        0x03
   #define  Si2144_WAKE_UP_CMD_CLOCK_FREQ_MIN         2
   #define  Si2144_WAKE_UP_CMD_CLOCK_FREQ_MAX         2
    #define Si2144_WAKE_UP_CMD_CLOCK_FREQ_CLK_24MHZ  2
   /* WAKE_UP command, RESERVED9 field definition (address 4,size 8, lsb 0, unsigned) */
   #define  Si2144_WAKE_UP_CMD_RESERVED9_LSB         0
   #define  Si2144_WAKE_UP_CMD_RESERVED9_MASK        0xff
   #define  Si2144_WAKE_UP_CMD_RESERVED9_MIN         0
   #define  Si2144_WAKE_UP_CMD_RESERVED9_MAX         0
    #define Si2144_WAKE_UP_CMD_RESERVED9_RESERVED  0
   /* WAKE_UP command, FUNC field definition (address 5,size 7, lsb 0, unsigned) */
   #define  Si2144_WAKE_UP_CMD_FUNC_LSB         0
   #define  Si2144_WAKE_UP_CMD_FUNC_MASK        0x7f
   #define  Si2144_WAKE_UP_CMD_FUNC_MIN         8
   #define  Si2144_WAKE_UP_CMD_FUNC_MAX         8
    #define Si2144_WAKE_UP_CMD_FUNC_BOOTLOADER  8
   /* WAKE_UP command, WAKE_UP field definition (address 6,size 1, lsb 0, unsigned) */
   #define  Si2144_WAKE_UP_CMD_WAKE_UP_LSB         0
   #define  Si2144_WAKE_UP_CMD_WAKE_UP_MASK        0x01
   #define  Si2144_WAKE_UP_CMD_WAKE_UP_MIN         1
   #define  Si2144_WAKE_UP_CMD_WAKE_UP_MAX         1
    #define Si2144_WAKE_UP_CMD_WAKE_UP_WAKE_UP  1
#endif /* Si2144_WAKE_UP_CMD */



  /* --------------------------------------------*/
  /* COMMANDS STRUCT                             */
  /* This is used to store all command fields    */
  /* --------------------------------------------*/
  typedef struct { /* Si2144_CmdObj struct */
    #ifdef    Si2144_AGC_OVERRIDE_CMD
              Si2144_AGC_OVERRIDE_CMD_struct     agc_override;
    #endif /* Si2144_AGC_OVERRIDE_CMD */
    #ifdef    Si2144_CONFIG_CLOCKS_CMD
              Si2144_CONFIG_CLOCKS_CMD_struct    config_clocks;
    #endif /* Si2144_CONFIG_CLOCKS_CMD */
    #ifdef    Si2144_CONFIG_I2C_CMD
              Si2144_CONFIG_I2C_CMD_struct       config_i2c;
    #endif /* Si2144_CONFIG_I2C_CMD */
    #ifdef    Si2144_CONFIG_PINS_CMD
              Si2144_CONFIG_PINS_CMD_struct      config_pins;
    #endif /* Si2144_CONFIG_PINS_CMD */
    #ifdef    Si2144_EXIT_BOOTLOADER_CMD
              Si2144_EXIT_BOOTLOADER_CMD_struct  exit_bootloader;
    #endif /* Si2144_EXIT_BOOTLOADER_CMD */
    #ifdef    Si2144_FINE_TUNE_CMD
              Si2144_FINE_TUNE_CMD_struct        fine_tune;
    #endif /* Si2144_FINE_TUNE_CMD */
    #ifdef    Si2144_GET_PROPERTY_CMD
              Si2144_GET_PROPERTY_CMD_struct     get_property;
    #endif /* Si2144_GET_PROPERTY_CMD */
    #ifdef    Si2144_GET_REV_CMD
              Si2144_GET_REV_CMD_struct          get_rev;
    #endif /* Si2144_GET_REV_CMD */
    #ifdef    Si2144_PART_INFO_CMD
              Si2144_PART_INFO_CMD_struct        part_info;
    #endif /* Si2144_PART_INFO_CMD */
    #ifdef    Si2144_PART_INFO2_CMD
              Si2144_PART_INFO2_CMD_struct       part_info2;
    #endif /* Si2144_PART_INFO2_CMD */
    #ifdef    Si2144_POLL_CTS_CMD
              Si2144_POLL_CTS_CMD_struct         poll_cts;
    #endif /* Si2144_POLL_CTS_CMD */
    #ifdef    Si2144_POWER_DOWN_HW_CMD
              Si2144_POWER_DOWN_HW_CMD_struct    power_down_hw;
    #endif /* Si2144_POWER_DOWN_HW_CMD */
    #ifdef    Si2144_POWER_UP_CMD
              Si2144_POWER_UP_CMD_struct         power_up;
    #endif /* Si2144_POWER_UP_CMD */
    #ifdef    Si2144_REREAD_REPLY_CMD
              Si2144_REREAD_REPLY_CMD_struct     reread_reply;
    #endif /* Si2144_REREAD_REPLY_CMD */
    #ifdef    Si2144_RESET_HW_CMD
              Si2144_RESET_HW_CMD_struct         reset_hw;
    #endif /* Si2144_RESET_HW_CMD */
    #ifdef    Si2144_SET_PROPERTY_CMD
              Si2144_SET_PROPERTY_CMD_struct     set_property;
    #endif /* Si2144_SET_PROPERTY_CMD */
    #ifdef    Si2144_STANDBY_CMD
              Si2144_STANDBY_CMD_struct          standby;
    #endif /* Si2144_STANDBY_CMD */
    #ifdef    Si2144_TUNER_STATUS_CMD
              Si2144_TUNER_STATUS_CMD_struct     tuner_status;
    #endif /* Si2144_TUNER_STATUS_CMD */
    #ifdef    Si2144_TUNER_TUNE_FREQ_CMD
              Si2144_TUNER_TUNE_FREQ_CMD_struct  tuner_tune_freq;
    #endif /* Si2144_TUNER_TUNE_FREQ_CMD */
    #ifdef    Si2144_WAKE_UP_CMD
              Si2144_WAKE_UP_CMD_struct          wake_up;
    #endif /* Si2144_WAKE_UP_CMD */
  } Si2144_CmdObj;


  /* --------------------------------------------*/
  /* COMMANDS REPLY STRUCT                       */
  /* This stores all command reply fields        */
  /* --------------------------------------------*/
  typedef struct { /* Si2144_CmdReplyObj struct */
    #ifdef    Si2144_AGC_OVERRIDE_CMD
              Si2144_AGC_OVERRIDE_CMD_REPLY_struct     agc_override;
    #endif /* Si2144_AGC_OVERRIDE_CMD */
    #ifdef    Si2144_CONFIG_CLOCKS_CMD
              Si2144_CONFIG_CLOCKS_CMD_REPLY_struct    config_clocks;
    #endif /* Si2144_CONFIG_CLOCKS_CMD */
    #ifdef    Si2144_CONFIG_I2C_CMD
              Si2144_CONFIG_I2C_CMD_REPLY_struct       config_i2c;
    #endif /* Si2144_CONFIG_I2C_CMD */
    #ifdef    Si2144_CONFIG_PINS_CMD
              Si2144_CONFIG_PINS_CMD_REPLY_struct      config_pins;
    #endif /* Si2144_CONFIG_PINS_CMD */
    #ifdef    Si2144_EXIT_BOOTLOADER_CMD
              Si2144_EXIT_BOOTLOADER_CMD_REPLY_struct  exit_bootloader;
    #endif /* Si2144_EXIT_BOOTLOADER_CMD */
    #ifdef    Si2144_FINE_TUNE_CMD
              Si2144_FINE_TUNE_CMD_REPLY_struct        fine_tune;
    #endif /* Si2144_FINE_TUNE_CMD */
    #ifdef    Si2144_GET_PROPERTY_CMD
              Si2144_GET_PROPERTY_CMD_REPLY_struct     get_property;
    #endif /* Si2144_GET_PROPERTY_CMD */
    #ifdef    Si2144_GET_REV_CMD
              Si2144_GET_REV_CMD_REPLY_struct          get_rev;
    #endif /* Si2144_GET_REV_CMD */
    #ifdef    Si2144_PART_INFO_CMD
              Si2144_PART_INFO_CMD_REPLY_struct        part_info;
    #endif /* Si2144_PART_INFO_CMD */
    #ifdef    Si2144_PART_INFO2_CMD
              Si2144_PART_INFO2_CMD_REPLY_struct       part_info2;
    #endif /* Si2144_PART_INFO2_CMD */
    #ifdef    Si2144_POLL_CTS_CMD
              Si2144_POLL_CTS_CMD_REPLY_struct         poll_cts;
    #endif /* Si2144_POLL_CTS_CMD */
    #ifdef    Si2144_POWER_DOWN_HW_CMD
              Si2144_POWER_DOWN_HW_CMD_REPLY_struct    power_down_hw;
    #endif /* Si2144_POWER_DOWN_HW_CMD */
    #ifdef    Si2144_POWER_UP_CMD
              Si2144_POWER_UP_CMD_REPLY_struct         power_up;
    #endif /* Si2144_POWER_UP_CMD */
    #ifdef    Si2144_REREAD_REPLY_CMD
              Si2144_REREAD_REPLY_CMD_REPLY_struct     reread_reply;
    #endif /* Si2144_REREAD_REPLY_CMD */
    #ifdef    Si2144_RESET_HW_CMD
              Si2144_RESET_HW_CMD_REPLY_struct         reset_hw;
    #endif /* Si2144_RESET_HW_CMD */
    #ifdef    Si2144_SET_PROPERTY_CMD
              Si2144_SET_PROPERTY_CMD_REPLY_struct     set_property;
    #endif /* Si2144_SET_PROPERTY_CMD */
    #ifdef    Si2144_STANDBY_CMD
              Si2144_STANDBY_CMD_REPLY_struct          standby;
    #endif /* Si2144_STANDBY_CMD */
    #ifdef    Si2144_TUNER_STATUS_CMD
              Si2144_TUNER_STATUS_CMD_REPLY_struct     tuner_status;
    #endif /* Si2144_TUNER_STATUS_CMD */
    #ifdef    Si2144_TUNER_TUNE_FREQ_CMD
              Si2144_TUNER_TUNE_FREQ_CMD_REPLY_struct  tuner_tune_freq;
    #endif /* Si2144_TUNER_TUNE_FREQ_CMD */
    #ifdef    Si2144_WAKE_UP_CMD
              Si2144_WAKE_UP_CMD_REPLY_struct          wake_up;
    #endif /* Si2144_WAKE_UP_CMD */
  } Si2144_CmdReplyObj;

#ifdef    Si2144_COMMAND_PROTOTYPES
//#define   Si2144_GET_COMMAND_STRINGS
#endif /* Si2144_COMMAND_PROTOTYPES */

#endif /* Si2144_COMMANDS_H */







