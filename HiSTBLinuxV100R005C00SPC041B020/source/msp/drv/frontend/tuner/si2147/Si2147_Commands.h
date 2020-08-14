/*************************************************************************************
                  Silicon Laboratories Broadcast Si2147 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API commands structures and functions definitions
   FILE: Si2147_Commands.h
   Supported IC : Si2147
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Date: November 06 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef    Si2147_COMMANDS_H
#define    Si2147_COMMANDS_H


/* STATUS structure definition */
  typedef struct { /* Si2147_COMMON_REPLY_struct */
    unsigned char   tunint;
    unsigned char   atvint;
    unsigned char   dtvint;
    unsigned char   err;
    unsigned char   cts;
 }  Si2147_COMMON_REPLY_struct;

/* STATUS fields definition */
  /* STATUS, TUNINT field definition (address 0, size 1, lsb 0, unsigned)*/
  #define  Si2147_STATUS_TUNINT_LSB         0
  #define  Si2147_STATUS_TUNINT_MASK        0x01
   #define Si2147_STATUS_TUNINT_NOT_TRIGGERED  0
   #define Si2147_STATUS_TUNINT_TRIGGERED      1
  /* STATUS, ATVINT field definition (address 0, size 1, lsb 1, unsigned)*/
  #define  Si2147_STATUS_ATVINT_LSB         1
  #define  Si2147_STATUS_ATVINT_MASK        0x01
   #define Si2147_STATUS_ATVINT_NOT_TRIGGERED  0
   #define Si2147_STATUS_ATVINT_TRIGGERED      1
  /* STATUS, DTVINT field definition (address 0, size 1, lsb 2, unsigned)*/
  #define  Si2147_STATUS_DTVINT_LSB         2
  #define  Si2147_STATUS_DTVINT_MASK        0x01
   #define Si2147_STATUS_DTVINT_NOT_TRIGGERED  0
   #define Si2147_STATUS_DTVINT_TRIGGERED      1
  /* STATUS, ERR field definition (address 0, size 1, lsb 6, unsigned)*/
  #define  Si2147_STATUS_ERR_LSB         6
  #define  Si2147_STATUS_ERR_MASK        0x01
   #define Si2147_STATUS_ERR_ERROR     1
   #define Si2147_STATUS_ERR_NO_ERROR  0
  /* STATUS, CTS field definition (address 0, size 1, lsb 7, unsigned)*/
  #define  Si2147_STATUS_CTS_LSB         7
  #define  Si2147_STATUS_CTS_MASK        0x01
   #define Si2147_STATUS_CTS_COMPLETED  1
   #define Si2147_STATUS_CTS_WAIT       0


/* Si2147_AGC_OVERRIDE command definition */
#define   Si2147_AGC_OVERRIDE_CMD 0x44

#ifdef    Si2147_AGC_OVERRIDE_CMD
  #define Si2147_AGC_OVERRIDE_CMD_CODE 0x010044

    typedef struct { /* Si2147_AGC_OVERRIDE_CMD_struct */
     unsigned char   force_max_gain;
     unsigned char   force_top_gain;
   } Si2147_AGC_OVERRIDE_CMD_struct;


    typedef struct { /* Si2147_AGC_OVERRIDE_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_AGC_OVERRIDE_CMD_REPLY_struct;

   /* AGC_OVERRIDE command, FORCE_MAX_GAIN field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_LSB         0
   #define  Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_MASK        0x01
   #define  Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_MIN         0
   #define  Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_MAX         1
    #define Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_DISABLE  0
    #define Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_ENABLE   1
   /* AGC_OVERRIDE command, FORCE_TOP_GAIN field definition (address 1,size 1, lsb 1, unsigned) */
   #define  Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_LSB         1
   #define  Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_MASK        0x01
   #define  Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_MIN         0
   #define  Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_MAX         1
    #define Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_DISABLE  0
    #define Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_ENABLE   1
#endif /* Si2147_AGC_OVERRIDE_CMD */

/* Si2147_ATV_CW_TEST command definition */
#define   Si2147_ATV_CW_TEST_CMD 0x53

#ifdef    Si2147_ATV_CW_TEST_CMD
  #define Si2147_ATV_CW_TEST_CMD_CODE 0x010053

    typedef struct { /* Si2147_ATV_CW_TEST_CMD_struct */
     unsigned char   pc_lock;
   } Si2147_ATV_CW_TEST_CMD_struct;


    typedef struct { /* Si2147_ATV_CW_TEST_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_ATV_CW_TEST_CMD_REPLY_struct;

   /* ATV_CW_TEST command, PC_LOCK field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2147_ATV_CW_TEST_CMD_PC_LOCK_LSB         0
   #define  Si2147_ATV_CW_TEST_CMD_PC_LOCK_MASK        0x01
   #define  Si2147_ATV_CW_TEST_CMD_PC_LOCK_MIN         0
   #define  Si2147_ATV_CW_TEST_CMD_PC_LOCK_MAX         1
    #define Si2147_ATV_CW_TEST_CMD_PC_LOCK_LOCK    1
    #define Si2147_ATV_CW_TEST_CMD_PC_LOCK_UNLOCK  0
#endif /* Si2147_ATV_CW_TEST_CMD */

/* Si2147_ATV_RESTART command definition */
#define   Si2147_ATV_RESTART_CMD 0x51

#ifdef    Si2147_ATV_RESTART_CMD
  #define Si2147_ATV_RESTART_CMD_CODE 0x010051

    typedef struct { /* Si2147_ATV_RESTART_CMD_struct */
         unsigned char   nothing;   } Si2147_ATV_RESTART_CMD_struct;


    typedef struct { /* Si2147_ATV_RESTART_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_ATV_RESTART_CMD_REPLY_struct;

#endif /* Si2147_ATV_RESTART_CMD */

/* Si2147_ATV_SCAN_TUNE command definition */
#define   Si2147_ATV_SCAN_TUNE_CMD 0x54

#ifdef    Si2147_ATV_SCAN_TUNE_CMD
  #define Si2147_ATV_SCAN_TUNE_CMD_CODE 0x010054

    typedef struct { /* Si2147_ATV_SCAN_TUNE_CMD_struct */
     unsigned long   freq;
   } Si2147_ATV_SCAN_TUNE_CMD_struct;


    typedef struct { /* Si2147_ATV_SCAN_TUNE_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_ATV_SCAN_TUNE_CMD_REPLY_struct;

   /* ATV_SCAN_TUNE command, FREQ field definition (address 4,size 32, lsb 0, unsigned) */
   #define  Si2147_ATV_SCAN_TUNE_CMD_FREQ_LSB         0
   #define  Si2147_ATV_SCAN_TUNE_CMD_FREQ_MASK        0xffffffff
   #define  Si2147_ATV_SCAN_TUNE_CMD_FREQ_MIN         40000000
   #define  Si2147_ATV_SCAN_TUNE_CMD_FREQ_MAX         1002000000
    #define Si2147_ATV_SCAN_TUNE_CMD_FREQ_FREQ_MIN  40000000
    #define Si2147_ATV_SCAN_TUNE_CMD_FREQ_FREQ_MAX  1002000000
#endif /* Si2147_ATV_SCAN_TUNE_CMD */

/* Si2147_ATV_STATUS command definition */
#define   Si2147_ATV_STATUS_CMD 0x52

#ifdef    Si2147_ATV_STATUS_CMD
  #define Si2147_ATV_STATUS_CMD_CODE 0x010052

    typedef struct { /* Si2147_ATV_STATUS_CMD_struct */
     unsigned char   intack;
   } Si2147_ATV_STATUS_CMD_struct;


    typedef struct { /* Si2147_ATV_STATUS_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
      unsigned char   chlint;
      unsigned char   pclint;
      unsigned char   chl;
      unsigned char   pcl;
               int    afc_freq;
      unsigned char   video_sys;
      unsigned char   color;
   }  Si2147_ATV_STATUS_CMD_REPLY_struct;

   /* ATV_STATUS command, INTACK field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2147_ATV_STATUS_CMD_INTACK_LSB         0
   #define  Si2147_ATV_STATUS_CMD_INTACK_MASK        0x01
   #define  Si2147_ATV_STATUS_CMD_INTACK_MIN         0
   #define  Si2147_ATV_STATUS_CMD_INTACK_MAX         1
    #define Si2147_ATV_STATUS_CMD_INTACK_CLEAR  1
    #define Si2147_ATV_STATUS_CMD_INTACK_OK     0
   /* ATV_STATUS command, CHLINT field definition (address 1, size 1, lsb 0, unsigned)*/
   #define  Si2147_ATV_STATUS_RESPONSE_CHLINT_LSB         0
   #define  Si2147_ATV_STATUS_RESPONSE_CHLINT_MASK        0x01
    #define Si2147_ATV_STATUS_RESPONSE_CHLINT_CHANGED    1
    #define Si2147_ATV_STATUS_RESPONSE_CHLINT_NO_CHANGE  0
   /* ATV_STATUS command, PCLINT field definition (address 1, size 1, lsb 1, unsigned)*/
   #define  Si2147_ATV_STATUS_RESPONSE_PCLINT_LSB         1
   #define  Si2147_ATV_STATUS_RESPONSE_PCLINT_MASK        0x01
    #define Si2147_ATV_STATUS_RESPONSE_PCLINT_CHANGED    1
    #define Si2147_ATV_STATUS_RESPONSE_PCLINT_NO_CHANGE  0
   /* ATV_STATUS command, CHL field definition (address 2, size 1, lsb 0, unsigned)*/
   #define  Si2147_ATV_STATUS_RESPONSE_CHL_LSB         0
   #define  Si2147_ATV_STATUS_RESPONSE_CHL_MASK        0x01
    #define Si2147_ATV_STATUS_RESPONSE_CHL_CHANNEL     1
    #define Si2147_ATV_STATUS_RESPONSE_CHL_NO_CHANNEL  0
   /* ATV_STATUS command, PCL field definition (address 2, size 1, lsb 1, unsigned)*/
   #define  Si2147_ATV_STATUS_RESPONSE_PCL_LSB         1
   #define  Si2147_ATV_STATUS_RESPONSE_PCL_MASK        0x01
    #define Si2147_ATV_STATUS_RESPONSE_PCL_LOCKED   1
    #define Si2147_ATV_STATUS_RESPONSE_PCL_NO_LOCK  0
   /* ATV_STATUS command, AFC_FREQ field definition (address 4, size 16, lsb 0, signed)*/
   #define  Si2147_ATV_STATUS_RESPONSE_AFC_FREQ_LSB         0
   #define  Si2147_ATV_STATUS_RESPONSE_AFC_FREQ_MASK        0xffff
   #define  Si2147_ATV_STATUS_RESPONSE_AFC_FREQ_SHIFT       16
   /* ATV_STATUS command, VIDEO_SYS field definition (address 8, size 3, lsb 0, unsigned)*/
   #define  Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_LSB         0
   #define  Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_MASK        0x07
    #define Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_B   0
    #define Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_DK  5
    #define Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_GH  1
    #define Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_I   4
    #define Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_L   6
    #define Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_LP  7
    #define Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_M   2
    #define Si2147_ATV_STATUS_RESPONSE_VIDEO_SYS_N   3
   /* ATV_STATUS command, COLOR field definition (address 8, size 1, lsb 4, unsigned)*/
   #define  Si2147_ATV_STATUS_RESPONSE_COLOR_LSB         4
   #define  Si2147_ATV_STATUS_RESPONSE_COLOR_MASK        0x01
    #define Si2147_ATV_STATUS_RESPONSE_COLOR_PAL_NTSC  0
    #define Si2147_ATV_STATUS_RESPONSE_COLOR_SECAM     1

#endif /* Si2147_ATV_STATUS_CMD */

/* Si2147_CONFIG_CLOCKS command definition */
#define   Si2147_CONFIG_CLOCKS_CMD 0xc0

#ifdef    Si2147_CONFIG_CLOCKS_CMD
  #define Si2147_CONFIG_CLOCKS_CMD_CODE 0x0100c0

    typedef struct { /* Si2147_CONFIG_CLOCKS_CMD_struct */
     unsigned char   subcode;
     unsigned char   clock_mode;
     unsigned char   en_xout;
   } Si2147_CONFIG_CLOCKS_CMD_struct;


    typedef struct { /* Si2147_CONFIG_CLOCKS_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_CONFIG_CLOCKS_CMD_REPLY_struct;

   /* CONFIG_CLOCKS command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2147_CONFIG_CLOCKS_CMD_SUBCODE_LSB         0
   #define  Si2147_CONFIG_CLOCKS_CMD_SUBCODE_MASK        0xff
   #define  Si2147_CONFIG_CLOCKS_CMD_SUBCODE_MIN         0
   #define  Si2147_CONFIG_CLOCKS_CMD_SUBCODE_MAX         0
    #define Si2147_CONFIG_CLOCKS_CMD_SUBCODE_CODE  0
   /* CONFIG_CLOCKS command, CLOCK_MODE field definition (address 2,size 2, lsb 0, unsigned) */
   #define  Si2147_CONFIG_CLOCKS_CMD_CLOCK_MODE_LSB         0
   #define  Si2147_CONFIG_CLOCKS_CMD_CLOCK_MODE_MASK        0x03
   #define  Si2147_CONFIG_CLOCKS_CMD_CLOCK_MODE_MIN         0
   #define  Si2147_CONFIG_CLOCKS_CMD_CLOCK_MODE_MAX         2
    #define Si2147_CONFIG_CLOCKS_CMD_CLOCK_MODE_EXTCLK  2
    #define Si2147_CONFIG_CLOCKS_CMD_CLOCK_MODE_XTAL    0
   /* CONFIG_CLOCKS command, EN_XOUT field definition (address 2,size 3, lsb 2, unsigned) */
   #define  Si2147_CONFIG_CLOCKS_CMD_EN_XOUT_LSB         2
   #define  Si2147_CONFIG_CLOCKS_CMD_EN_XOUT_MASK        0x07
   #define  Si2147_CONFIG_CLOCKS_CMD_EN_XOUT_MIN         0
   #define  Si2147_CONFIG_CLOCKS_CMD_EN_XOUT_MAX         3
    #define Si2147_CONFIG_CLOCKS_CMD_EN_XOUT_DIS_XOUT  0
    #define Si2147_CONFIG_CLOCKS_CMD_EN_XOUT_EN_XOUT   3
#endif /* Si2147_CONFIG_CLOCKS_CMD */

/* Si2147_CONFIG_I2C command definition */
#define   Si2147_CONFIG_I2C_CMD 0xc0

#ifdef    Si2147_CONFIG_I2C_CMD
  #define Si2147_CONFIG_I2C_CMD_CODE 0x0200c0

    typedef struct { /* Si2147_CONFIG_I2C_CMD_struct */
     unsigned char   subcode;
     unsigned char   i2c_broadcast;
   } Si2147_CONFIG_I2C_CMD_struct;


    typedef struct { /* Si2147_CONFIG_I2C_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_CONFIG_I2C_CMD_REPLY_struct;

   /* CONFIG_I2C command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2147_CONFIG_I2C_CMD_SUBCODE_LSB         0
   #define  Si2147_CONFIG_I2C_CMD_SUBCODE_MASK        0xff
   #define  Si2147_CONFIG_I2C_CMD_SUBCODE_MIN         36
   #define  Si2147_CONFIG_I2C_CMD_SUBCODE_MAX         36
    #define Si2147_CONFIG_I2C_CMD_SUBCODE_CODE  36
   /* CONFIG_I2C command, I2C_BROADCAST field definition (address 2,size 2, lsb 0, unsigned) */
   #define  Si2147_CONFIG_I2C_CMD_I2C_BROADCAST_LSB         0
   #define  Si2147_CONFIG_I2C_CMD_I2C_BROADCAST_MASK        0x03
   #define  Si2147_CONFIG_I2C_CMD_I2C_BROADCAST_MIN         0
   #define  Si2147_CONFIG_I2C_CMD_I2C_BROADCAST_MAX         1
    #define Si2147_CONFIG_I2C_CMD_I2C_BROADCAST_DISABLED  0
    #define Si2147_CONFIG_I2C_CMD_I2C_BROADCAST_ENABLED   1
#endif /* Si2147_CONFIG_I2C_CMD */

/* Si2147_CONFIG_PINS command definition */
#define   Si2147_CONFIG_PINS_CMD 0x12

#ifdef    Si2147_CONFIG_PINS_CMD
  #define Si2147_CONFIG_PINS_CMD_CODE 0x010012

    typedef struct { /* Si2147_CONFIG_PINS_CMD_struct */
     unsigned char   gpio1_mode;
     unsigned char   gpio1_read;
     unsigned char   gpio2_mode;
     unsigned char   gpio2_read;
     unsigned char   reserved1;
     unsigned char   reserved2;
     unsigned char   reserved3;
   } Si2147_CONFIG_PINS_CMD_struct;


    typedef struct { /* Si2147_CONFIG_PINS_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
      unsigned char   gpio1_mode;
      unsigned char   gpio1_state;
      unsigned char   gpio2_mode;
      unsigned char   gpio2_state;
      unsigned char   reserved1;
      unsigned char   reserved2;
      unsigned char   reserved3;
   }  Si2147_CONFIG_PINS_CMD_REPLY_struct;

   /* CONFIG_PINS command, GPIO1_MODE field definition (address 1,size 7, lsb 0, unsigned) */
   #define  Si2147_CONFIG_PINS_CMD_GPIO1_MODE_LSB         0
   #define  Si2147_CONFIG_PINS_CMD_GPIO1_MODE_MASK        0x7f
   #define  Si2147_CONFIG_PINS_CMD_GPIO1_MODE_MIN         0
   #define  Si2147_CONFIG_PINS_CMD_GPIO1_MODE_MAX         3
    #define Si2147_CONFIG_PINS_CMD_GPIO1_MODE_DISABLE    1
    #define Si2147_CONFIG_PINS_CMD_GPIO1_MODE_DRIVE_0    2
    #define Si2147_CONFIG_PINS_CMD_GPIO1_MODE_DRIVE_1    3
    #define Si2147_CONFIG_PINS_CMD_GPIO1_MODE_NO_CHANGE  0
   /* CONFIG_PINS command, GPIO1_READ field definition (address 1,size 1, lsb 7, unsigned) */
   #define  Si2147_CONFIG_PINS_CMD_GPIO1_READ_LSB         7
   #define  Si2147_CONFIG_PINS_CMD_GPIO1_READ_MASK        0x01
   #define  Si2147_CONFIG_PINS_CMD_GPIO1_READ_MIN         0
   #define  Si2147_CONFIG_PINS_CMD_GPIO1_READ_MAX         1
    #define Si2147_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ  0
    #define Si2147_CONFIG_PINS_CMD_GPIO1_READ_READ         1
   /* CONFIG_PINS command, GPIO2_MODE field definition (address 2,size 7, lsb 0, unsigned) */
   #define  Si2147_CONFIG_PINS_CMD_GPIO2_MODE_LSB         0
   #define  Si2147_CONFIG_PINS_CMD_GPIO2_MODE_MASK        0x7f
   #define  Si2147_CONFIG_PINS_CMD_GPIO2_MODE_MIN         0
   #define  Si2147_CONFIG_PINS_CMD_GPIO2_MODE_MAX         3
    #define Si2147_CONFIG_PINS_CMD_GPIO2_MODE_DISABLE    1
    #define Si2147_CONFIG_PINS_CMD_GPIO2_MODE_DRIVE_0    2
    #define Si2147_CONFIG_PINS_CMD_GPIO2_MODE_DRIVE_1    3
    #define Si2147_CONFIG_PINS_CMD_GPIO2_MODE_NO_CHANGE  0
   /* CONFIG_PINS command, GPIO2_READ field definition (address 2,size 1, lsb 7, unsigned) */
   #define  Si2147_CONFIG_PINS_CMD_GPIO2_READ_LSB         7
   #define  Si2147_CONFIG_PINS_CMD_GPIO2_READ_MASK        0x01
   #define  Si2147_CONFIG_PINS_CMD_GPIO2_READ_MIN         0
   #define  Si2147_CONFIG_PINS_CMD_GPIO2_READ_MAX         1
    #define Si2147_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ  0
    #define Si2147_CONFIG_PINS_CMD_GPIO2_READ_READ         1
   /* CONFIG_PINS command, RESERVED1 field definition (address 3,size 8, lsb 0, unsigned) */
   #define  Si2147_CONFIG_PINS_CMD_RESERVED1_LSB         0
   #define  Si2147_CONFIG_PINS_CMD_RESERVED1_MASK        0xff
   #define  Si2147_CONFIG_PINS_CMD_RESERVED1_MIN         1
   #define  Si2147_CONFIG_PINS_CMD_RESERVED1_MAX         1
    #define Si2147_CONFIG_PINS_CMD_RESERVED1_RESERVED  1
   /* CONFIG_PINS command, RESERVED2 field definition (address 4,size 8, lsb 0, unsigned) */
   #define  Si2147_CONFIG_PINS_CMD_RESERVED2_LSB         0
   #define  Si2147_CONFIG_PINS_CMD_RESERVED2_MASK        0xff
   #define  Si2147_CONFIG_PINS_CMD_RESERVED2_MIN         1
   #define  Si2147_CONFIG_PINS_CMD_RESERVED2_MAX         1
    #define Si2147_CONFIG_PINS_CMD_RESERVED2_RESERVED  1
   /* CONFIG_PINS command, RESERVED3 field definition (address 5,size 8, lsb 0, unsigned) */
   #define  Si2147_CONFIG_PINS_CMD_RESERVED3_LSB         0
   #define  Si2147_CONFIG_PINS_CMD_RESERVED3_MASK        0xff
   #define  Si2147_CONFIG_PINS_CMD_RESERVED3_MIN         1
   #define  Si2147_CONFIG_PINS_CMD_RESERVED3_MAX         1
    #define Si2147_CONFIG_PINS_CMD_RESERVED3_RESERVED  1
   /* CONFIG_PINS command, GPIO1_MODE field definition (address 1, size 7, lsb 0, unsigned)*/
   #define  Si2147_CONFIG_PINS_RESPONSE_GPIO1_MODE_LSB         0
   #define  Si2147_CONFIG_PINS_RESPONSE_GPIO1_MODE_MASK        0x7f
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO1_MODE_DISABLE  1
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO1_MODE_DRIVE_0  2
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO1_MODE_DRIVE_1  3
   /* CONFIG_PINS command, GPIO1_STATE field definition (address 1, size 1, lsb 7, unsigned)*/
   #define  Si2147_CONFIG_PINS_RESPONSE_GPIO1_STATE_LSB         7
   #define  Si2147_CONFIG_PINS_RESPONSE_GPIO1_STATE_MASK        0x01
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO1_STATE_READ_0  0
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO1_STATE_READ_1  1
   /* CONFIG_PINS command, GPIO2_MODE field definition (address 2, size 7, lsb 0, unsigned)*/
   #define  Si2147_CONFIG_PINS_RESPONSE_GPIO2_MODE_LSB         0
   #define  Si2147_CONFIG_PINS_RESPONSE_GPIO2_MODE_MASK        0x7f
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO2_MODE_DISABLE  1
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO2_MODE_DRIVE_0  2
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO2_MODE_DRIVE_1  3
   /* CONFIG_PINS command, GPIO2_STATE field definition (address 2, size 1, lsb 7, unsigned)*/
   #define  Si2147_CONFIG_PINS_RESPONSE_GPIO2_STATE_LSB         7
   #define  Si2147_CONFIG_PINS_RESPONSE_GPIO2_STATE_MASK        0x01
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO2_STATE_READ_0  0
    #define Si2147_CONFIG_PINS_RESPONSE_GPIO2_STATE_READ_1  1
   /* CONFIG_PINS command, RESERVED1 field definition (address 3, size 8, lsb 0, unsigned)*/
   #define  Si2147_CONFIG_PINS_RESPONSE_RESERVED1_LSB         0
   #define  Si2147_CONFIG_PINS_RESPONSE_RESERVED1_MASK        0xff
    #define Si2147_CONFIG_PINS_RESPONSE_RESERVED1_RESERVED1_MIN  1
    #define Si2147_CONFIG_PINS_RESPONSE_RESERVED1_RESERVED1_MAX  1
   /* CONFIG_PINS command, RESERVED2 field definition (address 4, size 8, lsb 0, unsigned)*/
   #define  Si2147_CONFIG_PINS_RESPONSE_RESERVED2_LSB         0
   #define  Si2147_CONFIG_PINS_RESPONSE_RESERVED2_MASK        0xff
    #define Si2147_CONFIG_PINS_RESPONSE_RESERVED2_RESERVED2_MIN  1
    #define Si2147_CONFIG_PINS_RESPONSE_RESERVED2_RESERVED2_MAX  1
   /* CONFIG_PINS command, RESERVED3 field definition (address 5, size 8, lsb 0, unsigned)*/
   #define  Si2147_CONFIG_PINS_RESPONSE_RESERVED3_LSB         0
   #define  Si2147_CONFIG_PINS_RESPONSE_RESERVED3_MASK        0xff
    #define Si2147_CONFIG_PINS_RESPONSE_RESERVED3_RESERVED3_MIN  1
    #define Si2147_CONFIG_PINS_RESPONSE_RESERVED3_RESERVED3_MAX  1

#endif /* Si2147_CONFIG_PINS_CMD */

/* Si2147_DTV_RESTART command definition */
#define   Si2147_DTV_RESTART_CMD 0x61

#ifdef    Si2147_DTV_RESTART_CMD
  #define Si2147_DTV_RESTART_CMD_CODE 0x010061

    typedef struct { /* Si2147_DTV_RESTART_CMD_struct */
         unsigned char   nothing;   } Si2147_DTV_RESTART_CMD_struct;


    typedef struct { /* Si2147_DTV_RESTART_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_DTV_RESTART_CMD_REPLY_struct;

#endif /* Si2147_DTV_RESTART_CMD */

/* Si2147_DTV_STATUS command definition */
#define   Si2147_DTV_STATUS_CMD 0x62

#ifdef    Si2147_DTV_STATUS_CMD
  #define Si2147_DTV_STATUS_CMD_CODE 0x010062

    typedef struct { /* Si2147_DTV_STATUS_CMD_struct */
     unsigned char   intack;
   } Si2147_DTV_STATUS_CMD_struct;


    typedef struct { /* Si2147_DTV_STATUS_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
      unsigned char   chlint;
      unsigned char   chl;
      unsigned char   bw;
      unsigned char   modulation;
   }  Si2147_DTV_STATUS_CMD_REPLY_struct;

   /* DTV_STATUS command, INTACK field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2147_DTV_STATUS_CMD_INTACK_LSB         0
   #define  Si2147_DTV_STATUS_CMD_INTACK_MASK        0x01
   #define  Si2147_DTV_STATUS_CMD_INTACK_MIN         0
   #define  Si2147_DTV_STATUS_CMD_INTACK_MAX         1
    #define Si2147_DTV_STATUS_CMD_INTACK_CLEAR  1
    #define Si2147_DTV_STATUS_CMD_INTACK_OK     0
   /* DTV_STATUS command, CHLINT field definition (address 1, size 1, lsb 0, unsigned)*/
   #define  Si2147_DTV_STATUS_RESPONSE_CHLINT_LSB         0
   #define  Si2147_DTV_STATUS_RESPONSE_CHLINT_MASK        0x01
    #define Si2147_DTV_STATUS_RESPONSE_CHLINT_CHANGED    1
    #define Si2147_DTV_STATUS_RESPONSE_CHLINT_NO_CHANGE  0
   /* DTV_STATUS command, CHL field definition (address 2, size 1, lsb 0, unsigned)*/
   #define  Si2147_DTV_STATUS_RESPONSE_CHL_LSB         0
   #define  Si2147_DTV_STATUS_RESPONSE_CHL_MASK        0x01
    #define Si2147_DTV_STATUS_RESPONSE_CHL_CHANNEL     1
    #define Si2147_DTV_STATUS_RESPONSE_CHL_NO_CHANNEL  0
   /* DTV_STATUS command, BW field definition (address 3, size 4, lsb 0, unsigned)*/
   #define  Si2147_DTV_STATUS_RESPONSE_BW_LSB         0
   #define  Si2147_DTV_STATUS_RESPONSE_BW_MASK        0x0f
    #define Si2147_DTV_STATUS_RESPONSE_BW_BW_1P7MHZ  9
    #define Si2147_DTV_STATUS_RESPONSE_BW_BW_6MHZ    6
    #define Si2147_DTV_STATUS_RESPONSE_BW_BW_6P1MHZ  10
    #define Si2147_DTV_STATUS_RESPONSE_BW_BW_7MHZ    7
    #define Si2147_DTV_STATUS_RESPONSE_BW_BW_8MHZ    8
   /* DTV_STATUS command, MODULATION field definition (address 3, size 4, lsb 4, unsigned)*/
   #define  Si2147_DTV_STATUS_RESPONSE_MODULATION_LSB         4
   #define  Si2147_DTV_STATUS_RESPONSE_MODULATION_MASK        0x0f
    #define Si2147_DTV_STATUS_RESPONSE_MODULATION_ATSC       0
    #define Si2147_DTV_STATUS_RESPONSE_MODULATION_DTMB       6
    #define Si2147_DTV_STATUS_RESPONSE_MODULATION_DVBC       3
    #define Si2147_DTV_STATUS_RESPONSE_MODULATION_DVBT       2
    #define Si2147_DTV_STATUS_RESPONSE_MODULATION_ISDBC      5
    #define Si2147_DTV_STATUS_RESPONSE_MODULATION_ISDBT      4
    #define Si2147_DTV_STATUS_RESPONSE_MODULATION_QAM_US     1

#endif /* Si2147_DTV_STATUS_CMD */

/* Si2147_EXIT_BOOTLOADER command definition */
#define   Si2147_EXIT_BOOTLOADER_CMD 0x01

#ifdef    Si2147_EXIT_BOOTLOADER_CMD
  #define Si2147_EXIT_BOOTLOADER_CMD_CODE 0x010001

    typedef struct { /* Si2147_EXIT_BOOTLOADER_CMD_struct */
     unsigned char   func;
     unsigned char   ctsien;
   } Si2147_EXIT_BOOTLOADER_CMD_struct;


    typedef struct { /* Si2147_EXIT_BOOTLOADER_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_EXIT_BOOTLOADER_CMD_REPLY_struct;

   /* EXIT_BOOTLOADER command, FUNC field definition (address 1,size 3, lsb 0, unsigned) */
   #define  Si2147_EXIT_BOOTLOADER_CMD_FUNC_LSB         0
   #define  Si2147_EXIT_BOOTLOADER_CMD_FUNC_MASK        0x07
   #define  Si2147_EXIT_BOOTLOADER_CMD_FUNC_MIN         0
   #define  Si2147_EXIT_BOOTLOADER_CMD_FUNC_MAX         1
    #define Si2147_EXIT_BOOTLOADER_CMD_FUNC_BOOTLOADER  0
    #define Si2147_EXIT_BOOTLOADER_CMD_FUNC_TUNER       1
   /* EXIT_BOOTLOADER command, CTSIEN field definition (address 1,size 1, lsb 7, unsigned) */
   #define  Si2147_EXIT_BOOTLOADER_CMD_CTSIEN_LSB         7
   #define  Si2147_EXIT_BOOTLOADER_CMD_CTSIEN_MASK        0x01
   #define  Si2147_EXIT_BOOTLOADER_CMD_CTSIEN_MIN         0
   #define  Si2147_EXIT_BOOTLOADER_CMD_CTSIEN_MAX         1
    #define Si2147_EXIT_BOOTLOADER_CMD_CTSIEN_OFF  0
    #define Si2147_EXIT_BOOTLOADER_CMD_CTSIEN_ON   1
#endif /* Si2147_EXIT_BOOTLOADER_CMD */

/* Si2147_FINE_TUNE command definition */
#define   Si2147_FINE_TUNE_CMD 0x45

#ifdef    Si2147_FINE_TUNE_CMD
  #define Si2147_FINE_TUNE_CMD_CODE 0x010045

    typedef struct { /* Si2147_FINE_TUNE_CMD_struct */
     unsigned char   persistence;
     unsigned char   apply_to_lif;
              int    offset_500hz;
   } Si2147_FINE_TUNE_CMD_struct;


    typedef struct { /* Si2147_FINE_TUNE_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_FINE_TUNE_CMD_REPLY_struct;

   /* FINE_TUNE command, PERSISTENCE field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2147_FINE_TUNE_CMD_PERSISTENCE_LSB         0
   #define  Si2147_FINE_TUNE_CMD_PERSISTENCE_MASK        0x01
   #define  Si2147_FINE_TUNE_CMD_PERSISTENCE_MIN         0
   #define  Si2147_FINE_TUNE_CMD_PERSISTENCE_MAX         1
    #define Si2147_FINE_TUNE_CMD_PERSISTENCE_NON_PERSISTENT  0
    #define Si2147_FINE_TUNE_CMD_PERSISTENCE_PERSISTENT      1
   /* FINE_TUNE command, APPLY_TO_LIF field definition (address 1,size 1, lsb 1, unsigned) */
   #define  Si2147_FINE_TUNE_CMD_APPLY_TO_LIF_LSB         1
   #define  Si2147_FINE_TUNE_CMD_APPLY_TO_LIF_MASK        0x01
   #define  Si2147_FINE_TUNE_CMD_APPLY_TO_LIF_MIN         0
   #define  Si2147_FINE_TUNE_CMD_APPLY_TO_LIF_MAX         1
    #define Si2147_FINE_TUNE_CMD_APPLY_TO_LIF_APPLY_TO_LIF         1
    #define Si2147_FINE_TUNE_CMD_APPLY_TO_LIF_DO_NOT_APPLY_TO_LIF  0
   /* FINE_TUNE command, OFFSET_500HZ field definition (address 2,size 16, lsb 0, signed) */
   #define  Si2147_FINE_TUNE_CMD_OFFSET_500HZ_LSB         0
   #define  Si2147_FINE_TUNE_CMD_OFFSET_500HZ_MASK        0xffff
   #define  Si2147_FINE_TUNE_CMD_OFFSET_500HZ_SHIFT       16
   #define  Si2147_FINE_TUNE_CMD_OFFSET_500HZ_MIN         -4000
   #define  Si2147_FINE_TUNE_CMD_OFFSET_500HZ_MAX         4000
    #define Si2147_FINE_TUNE_CMD_OFFSET_500HZ_OFFSET_500HZ_MIN  -4000
    #define Si2147_FINE_TUNE_CMD_OFFSET_500HZ_OFFSET_500HZ_MAX  4000
#endif /* Si2147_FINE_TUNE_CMD */

/* Si2147_GET_PROPERTY command definition */
#define   Si2147_GET_PROPERTY_CMD 0x15

#ifdef    Si2147_GET_PROPERTY_CMD
  #define Si2147_GET_PROPERTY_CMD_CODE 0x010015

    typedef struct { /* Si2147_GET_PROPERTY_CMD_struct */
     unsigned char   reserved;
     unsigned int    prop;
   } Si2147_GET_PROPERTY_CMD_struct;


    typedef struct { /* Si2147_GET_PROPERTY_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
      unsigned char   reserved;
      unsigned int    data;
   }  Si2147_GET_PROPERTY_CMD_REPLY_struct;

   /* GET_PROPERTY command, RESERVED field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2147_GET_PROPERTY_CMD_RESERVED_LSB         0
   #define  Si2147_GET_PROPERTY_CMD_RESERVED_MASK        0xff
   #define  Si2147_GET_PROPERTY_CMD_RESERVED_MIN         0
   #define  Si2147_GET_PROPERTY_CMD_RESERVED_MAX         0
    #define Si2147_GET_PROPERTY_CMD_RESERVED_RESERVED_MIN  0
    #define Si2147_GET_PROPERTY_CMD_RESERVED_RESERVED_MAX  0
   /* GET_PROPERTY command, PROP field definition (address 2,size 16, lsb 0, unsigned) */
   #define  Si2147_GET_PROPERTY_CMD_PROP_LSB         0
   #define  Si2147_GET_PROPERTY_CMD_PROP_MASK        0xffff
   #define  Si2147_GET_PROPERTY_CMD_PROP_MIN         0
   #define  Si2147_GET_PROPERTY_CMD_PROP_MAX         65535
    #define Si2147_GET_PROPERTY_CMD_PROP_PROP_MIN  0
    #define Si2147_GET_PROPERTY_CMD_PROP_PROP_MAX  65535
   /* GET_PROPERTY command, RESERVED field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2147_GET_PROPERTY_RESPONSE_RESERVED_LSB         0
   #define  Si2147_GET_PROPERTY_RESPONSE_RESERVED_MASK        0xff
   /* GET_PROPERTY command, DATA field definition (address 2, size 16, lsb 0, unsigned)*/
   #define  Si2147_GET_PROPERTY_RESPONSE_DATA_LSB         0
   #define  Si2147_GET_PROPERTY_RESPONSE_DATA_MASK        0xffff

#endif /* Si2147_GET_PROPERTY_CMD */

/* Si2147_GET_REV command definition */
#define   Si2147_GET_REV_CMD 0x11

#ifdef    Si2147_GET_REV_CMD
  #define Si2147_GET_REV_CMD_CODE 0x010011

    typedef struct { /* Si2147_GET_REV_CMD_struct */
         unsigned char   nothing;   } Si2147_GET_REV_CMD_struct;


    typedef struct { /* Si2147_GET_REV_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
      unsigned char   pn;
      unsigned char   fwmajor;
      unsigned char   fwminor;
      unsigned int    patch;
      unsigned char   cmpmajor;
      unsigned char   cmpminor;
      unsigned char   cmpbuild;
      unsigned char   chiprev;
   }  Si2147_GET_REV_CMD_REPLY_struct;

   /* GET_REV command, PN field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2147_GET_REV_RESPONSE_PN_LSB         0
   #define  Si2147_GET_REV_RESPONSE_PN_MASK        0xff
   /* GET_REV command, FWMAJOR field definition (address 2, size 8, lsb 0, unsigned)*/
   #define  Si2147_GET_REV_RESPONSE_FWMAJOR_LSB         0
   #define  Si2147_GET_REV_RESPONSE_FWMAJOR_MASK        0xff
   /* GET_REV command, FWMINOR field definition (address 3, size 8, lsb 0, unsigned)*/
   #define  Si2147_GET_REV_RESPONSE_FWMINOR_LSB         0
   #define  Si2147_GET_REV_RESPONSE_FWMINOR_MASK        0xff
   /* GET_REV command, PATCH field definition (address 4, size 16, lsb 0, unsigned)*/
   #define  Si2147_GET_REV_RESPONSE_PATCH_LSB         0
   #define  Si2147_GET_REV_RESPONSE_PATCH_MASK        0xffff
   /* GET_REV command, CMPMAJOR field definition (address 6, size 8, lsb 0, unsigned)*/
   #define  Si2147_GET_REV_RESPONSE_CMPMAJOR_LSB         0
   #define  Si2147_GET_REV_RESPONSE_CMPMAJOR_MASK        0xff
   /* GET_REV command, CMPMINOR field definition (address 7, size 8, lsb 0, unsigned)*/
   #define  Si2147_GET_REV_RESPONSE_CMPMINOR_LSB         0
   #define  Si2147_GET_REV_RESPONSE_CMPMINOR_MASK        0xff
   /* GET_REV command, CMPBUILD field definition (address 8, size 8, lsb 0, unsigned)*/
   #define  Si2147_GET_REV_RESPONSE_CMPBUILD_LSB         0
   #define  Si2147_GET_REV_RESPONSE_CMPBUILD_MASK        0xff
    #define Si2147_GET_REV_RESPONSE_CMPBUILD_CMPBUILD_MIN  0
    #define Si2147_GET_REV_RESPONSE_CMPBUILD_CMPBUILD_MAX  255
   /* GET_REV command, CHIPREV field definition (address 9, size 4, lsb 0, unsigned)*/
   #define  Si2147_GET_REV_RESPONSE_CHIPREV_LSB         0
   #define  Si2147_GET_REV_RESPONSE_CHIPREV_MASK        0x0f
    #define Si2147_GET_REV_RESPONSE_CHIPREV_A  1
    #define Si2147_GET_REV_RESPONSE_CHIPREV_B  2

#endif /* Si2147_GET_REV_CMD */

/* Si2147_PART_INFO command definition */
#define   Si2147_PART_INFO_CMD 0x02

#ifdef    Si2147_PART_INFO_CMD
  #define Si2147_PART_INFO_CMD_CODE 0x010002

    typedef struct { /* Si2147_PART_INFO_CMD_struct */
         unsigned char   nothing;   } Si2147_PART_INFO_CMD_struct;


    typedef struct { /* Si2147_PART_INFO_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
      unsigned char   chiprev;
      unsigned char   romid;
      unsigned char   part;
      unsigned char   pmajor;
      unsigned char   pminor;
      unsigned char   pbuild;
      unsigned int    reserved;
      unsigned long   serial;
   }  Si2147_PART_INFO_CMD_REPLY_struct;

   /* PART_INFO command, CHIPREV field definition (address 1, size 4, lsb 0, unsigned)*/
   #define  Si2147_PART_INFO_RESPONSE_CHIPREV_LSB         0
   #define  Si2147_PART_INFO_RESPONSE_CHIPREV_MASK        0x0f
    #define Si2147_PART_INFO_RESPONSE_CHIPREV_A  1
    #define Si2147_PART_INFO_RESPONSE_CHIPREV_B  2
   /* PART_INFO command, ROMID field definition (address 12, size 8, lsb 0, unsigned)*/
   #define  Si2147_PART_INFO_RESPONSE_ROMID_LSB         0
   #define  Si2147_PART_INFO_RESPONSE_ROMID_MASK        0xff
   /* PART_INFO command, PART field definition (address 2, size 8, lsb 0, unsigned)*/
   #define  Si2147_PART_INFO_RESPONSE_PART_LSB         0
   #define  Si2147_PART_INFO_RESPONSE_PART_MASK        0xff
   /* PART_INFO command, PMAJOR field definition (address 3, size 8, lsb 0, unsigned)*/
   #define  Si2147_PART_INFO_RESPONSE_PMAJOR_LSB         0
   #define  Si2147_PART_INFO_RESPONSE_PMAJOR_MASK        0xff
   /* PART_INFO command, PMINOR field definition (address 4, size 8, lsb 0, unsigned)*/
   #define  Si2147_PART_INFO_RESPONSE_PMINOR_LSB         0
   #define  Si2147_PART_INFO_RESPONSE_PMINOR_MASK        0xff
   /* PART_INFO command, PBUILD field definition (address 5, size 8, lsb 0, unsigned)*/
   #define  Si2147_PART_INFO_RESPONSE_PBUILD_LSB         0
   #define  Si2147_PART_INFO_RESPONSE_PBUILD_MASK        0xff
   /* PART_INFO command, RESERVED field definition (address 6, size 16, lsb 0, unsigned)*/
   #define  Si2147_PART_INFO_RESPONSE_RESERVED_LSB         0
   #define  Si2147_PART_INFO_RESPONSE_RESERVED_MASK        0xffff
   /* PART_INFO command, SERIAL field definition (address 8, size 32, lsb 0, unsigned)*/
   #define  Si2147_PART_INFO_RESPONSE_SERIAL_LSB         0
   #define  Si2147_PART_INFO_RESPONSE_SERIAL_MASK        0xffffffff

#endif /* Si2147_PART_INFO_CMD */

/* Si2147_POWER_DOWN command definition */
#define   Si2147_POWER_DOWN_CMD 0x13

#ifdef    Si2147_POWER_DOWN_CMD
  #define Si2147_POWER_DOWN_CMD_CODE 0x010013

    typedef struct { /* Si2147_POWER_DOWN_CMD_struct */
         unsigned char   nothing;   } Si2147_POWER_DOWN_CMD_struct;


    typedef struct { /* Si2147_POWER_DOWN_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_POWER_DOWN_CMD_REPLY_struct;

#endif /* Si2147_POWER_DOWN_CMD */

/* Si2147_POWER_DOWN_HW command definition */
#define   Si2147_POWER_DOWN_HW_CMD 0xc0

#ifdef    Si2147_POWER_DOWN_HW_CMD
  #define Si2147_POWER_DOWN_HW_CMD_CODE 0x0300c0

    typedef struct { /* Si2147_POWER_DOWN_HW_CMD_struct */
     unsigned char   subcode;
     unsigned char   pd_xo_osc;
     unsigned char   reserved1;
     unsigned char   en_xout;
     unsigned char   reserved2;
     unsigned char   pd_ldo;
     unsigned char   reserved3;
     unsigned char   reserved4;
     unsigned char   reserved5;
     unsigned char   reserved6;
     unsigned char   reserved7;
     unsigned char   reserved8;
   } Si2147_POWER_DOWN_HW_CMD_struct;


    typedef struct { /* Si2147_POWER_DOWN_HW_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_POWER_DOWN_HW_CMD_REPLY_struct;

   /* POWER_DOWN_HW command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_SUBCODE_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_SUBCODE_MASK        0xff
   #define  Si2147_POWER_DOWN_HW_CMD_SUBCODE_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_SUBCODE_MAX         0
    #define Si2147_POWER_DOWN_HW_CMD_SUBCODE_CODE  0
   /* POWER_DOWN_HW command, PD_XO_OSC field definition (address 2,size 1, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_PD_XO_OSC_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_PD_XO_OSC_MASK        0x01
   #define  Si2147_POWER_DOWN_HW_CMD_PD_XO_OSC_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_PD_XO_OSC_MAX         1
    #define Si2147_POWER_DOWN_HW_CMD_PD_XO_OSC_XO_OSC_POWER_DOWN  1
    #define Si2147_POWER_DOWN_HW_CMD_PD_XO_OSC_XO_OSC_POWER_UP    0
   /* POWER_DOWN_HW command, RESERVED1 field definition (address 2,size 1, lsb 1, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED1_LSB         1
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED1_MASK        0x01
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED1_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED1_MAX         0
    #define Si2147_POWER_DOWN_HW_CMD_RESERVED1_RESERVED  0
   /* POWER_DOWN_HW command, EN_XOUT field definition (address 2,size 3, lsb 2, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_EN_XOUT_LSB         2
   #define  Si2147_POWER_DOWN_HW_CMD_EN_XOUT_MASK        0x07
   #define  Si2147_POWER_DOWN_HW_CMD_EN_XOUT_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_EN_XOUT_MAX         3
    #define Si2147_POWER_DOWN_HW_CMD_EN_XOUT_DIS_XOUT  0
    #define Si2147_POWER_DOWN_HW_CMD_EN_XOUT_EN_XOUT   3
   /* POWER_DOWN_HW command, RESERVED2 field definition (address 2,size 3, lsb 5, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED2_LSB         5
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED2_MASK        0x07
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED2_MIN         1
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED2_MAX         1
    #define Si2147_POWER_DOWN_HW_CMD_RESERVED2_RESERVED  1
   /* POWER_DOWN_HW command, PD_LDO field definition (address 3,size 1, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_PD_LDO_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_PD_LDO_MASK        0x01
   #define  Si2147_POWER_DOWN_HW_CMD_PD_LDO_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_PD_LDO_MAX         1
    #define Si2147_POWER_DOWN_HW_CMD_PD_LDO_LDO_POWER_DOWN  1
    #define Si2147_POWER_DOWN_HW_CMD_PD_LDO_LDO_POWER_UP    0
   /* POWER_DOWN_HW command, RESERVED3 field definition (address 4,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED3_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED3_MASK        0xff
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED3_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED3_MAX         0
    #define Si2147_POWER_DOWN_HW_CMD_RESERVED3_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED4 field definition (address 5,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED4_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED4_MASK        0xff
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED4_MIN         1
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED4_MAX         1
    #define Si2147_POWER_DOWN_HW_CMD_RESERVED4_RESERVED  1
   /* POWER_DOWN_HW command, RESERVED5 field definition (address 6,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED5_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED5_MASK        0xff
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED5_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED5_MAX         0
    #define Si2147_POWER_DOWN_HW_CMD_RESERVED5_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED6 field definition (address 7,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED6_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED6_MASK        0xff
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED6_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED6_MAX         0
    #define Si2147_POWER_DOWN_HW_CMD_RESERVED6_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED7 field definition (address 8,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED7_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED7_MASK        0xff
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED7_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED7_MAX         0
    #define Si2147_POWER_DOWN_HW_CMD_RESERVED7_RESERVED  0
   /* POWER_DOWN_HW command, RESERVED8 field definition (address 9,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED8_LSB         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED8_MASK        0xff
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED8_MIN         0
   #define  Si2147_POWER_DOWN_HW_CMD_RESERVED8_MAX         0
    #define Si2147_POWER_DOWN_HW_CMD_RESERVED8_RESERVED  0
#endif /* Si2147_POWER_DOWN_HW_CMD */

/* Si2147_POWER_UP command definition */
#define   Si2147_POWER_UP_CMD 0xc0

#ifdef    Si2147_POWER_UP_CMD
  #define Si2147_POWER_UP_CMD_CODE 0x0400c0

    typedef struct { /* Si2147_POWER_UP_CMD_struct */
     unsigned char   subcode;
     unsigned char   clock_mode;
     unsigned char   en_xout;
     unsigned char   pd_ldo;
     unsigned char   reserved2;
     unsigned char   reserved3;
     unsigned char   reserved4;
     unsigned char   reserved5;
     unsigned char   reserved6;
     unsigned char   reserved7;
     unsigned char   reset;
     unsigned char   clock_freq;
     unsigned char   reserved8;
     unsigned char   func;
     unsigned char   reserved9;
     unsigned char   ctsien;
     unsigned char   wake_up;
   } Si2147_POWER_UP_CMD_struct;


    typedef struct { /* Si2147_POWER_UP_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_POWER_UP_CMD_REPLY_struct;

   /* POWER_UP command, SUBCODE field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_SUBCODE_LSB         0
   #define  Si2147_POWER_UP_CMD_SUBCODE_MASK        0xff
   #define  Si2147_POWER_UP_CMD_SUBCODE_MIN         0
   #define  Si2147_POWER_UP_CMD_SUBCODE_MAX         0
    #define Si2147_POWER_UP_CMD_SUBCODE_CODE  0
   /* POWER_UP command, CLOCK_MODE field definition (address 2,size 2, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_CLOCK_MODE_LSB         0
   #define  Si2147_POWER_UP_CMD_CLOCK_MODE_MASK        0x03
   #define  Si2147_POWER_UP_CMD_CLOCK_MODE_MIN         0
   #define  Si2147_POWER_UP_CMD_CLOCK_MODE_MAX         2
    #define Si2147_POWER_UP_CMD_CLOCK_MODE_EXTCLK  2
    #define Si2147_POWER_UP_CMD_CLOCK_MODE_XTAL    0
   /* POWER_UP command, EN_XOUT field definition (address 2,size 3, lsb 2, unsigned) */
   #define  Si2147_POWER_UP_CMD_EN_XOUT_LSB         2
   #define  Si2147_POWER_UP_CMD_EN_XOUT_MASK        0x07
   #define  Si2147_POWER_UP_CMD_EN_XOUT_MIN         0
   #define  Si2147_POWER_UP_CMD_EN_XOUT_MAX         3
    #define Si2147_POWER_UP_CMD_EN_XOUT_DIS_XOUT  0
    #define Si2147_POWER_UP_CMD_EN_XOUT_EN_XOUT   3
   /* POWER_UP command, PD_LDO field definition (address 3,size 1, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_PD_LDO_LSB         0
   #define  Si2147_POWER_UP_CMD_PD_LDO_MASK        0x01
   #define  Si2147_POWER_UP_CMD_PD_LDO_MIN         0
   #define  Si2147_POWER_UP_CMD_PD_LDO_MAX         1
    #define Si2147_POWER_UP_CMD_PD_LDO_LDO_POWER_DOWN  1
    #define Si2147_POWER_UP_CMD_PD_LDO_LDO_POWER_UP    0
   /* POWER_UP command, RESERVED2 field definition (address 4,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESERVED2_LSB         0
   #define  Si2147_POWER_UP_CMD_RESERVED2_MASK        0xff
   #define  Si2147_POWER_UP_CMD_RESERVED2_MIN         0
   #define  Si2147_POWER_UP_CMD_RESERVED2_MAX         0
    #define Si2147_POWER_UP_CMD_RESERVED2_RESERVED  0
   /* POWER_UP command, RESERVED3 field definition (address 5,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESERVED3_LSB         0
   #define  Si2147_POWER_UP_CMD_RESERVED3_MASK        0xff
   #define  Si2147_POWER_UP_CMD_RESERVED3_MIN         1
   #define  Si2147_POWER_UP_CMD_RESERVED3_MAX         1
    #define Si2147_POWER_UP_CMD_RESERVED3_RESERVED  1
   /* POWER_UP command, RESERVED4 field definition (address 6,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESERVED4_LSB         0
   #define  Si2147_POWER_UP_CMD_RESERVED4_MASK        0xff
   #define  Si2147_POWER_UP_CMD_RESERVED4_MIN         1
   #define  Si2147_POWER_UP_CMD_RESERVED4_MAX         1
    #define Si2147_POWER_UP_CMD_RESERVED4_RESERVED  1
   /* POWER_UP command, RESERVED5 field definition (address 7,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESERVED5_LSB         0
   #define  Si2147_POWER_UP_CMD_RESERVED5_MASK        0xff
   #define  Si2147_POWER_UP_CMD_RESERVED5_MIN         1
   #define  Si2147_POWER_UP_CMD_RESERVED5_MAX         1
    #define Si2147_POWER_UP_CMD_RESERVED5_RESERVED  1
   /* POWER_UP command, RESERVED6 field definition (address 8,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESERVED6_LSB         0
   #define  Si2147_POWER_UP_CMD_RESERVED6_MASK        0xff
   #define  Si2147_POWER_UP_CMD_RESERVED6_MIN         1
   #define  Si2147_POWER_UP_CMD_RESERVED6_MAX         1
    #define Si2147_POWER_UP_CMD_RESERVED6_RESERVED  1
   /* POWER_UP command, RESERVED7 field definition (address 9,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESERVED7_LSB         0
   #define  Si2147_POWER_UP_CMD_RESERVED7_MASK        0xff
   #define  Si2147_POWER_UP_CMD_RESERVED7_MIN         1
   #define  Si2147_POWER_UP_CMD_RESERVED7_MAX         1
    #define Si2147_POWER_UP_CMD_RESERVED7_RESERVED  1
   /* POWER_UP command, RESET field definition (address 10,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESET_LSB         0
   #define  Si2147_POWER_UP_CMD_RESET_MASK        0xff
   #define  Si2147_POWER_UP_CMD_RESET_MIN         1
   #define  Si2147_POWER_UP_CMD_RESET_MAX         1
    #define Si2147_POWER_UP_CMD_RESET_RESET  1
   /* POWER_UP command, CLOCK_FREQ field definition (address 11,size 2, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_CLOCK_FREQ_LSB         0
   #define  Si2147_POWER_UP_CMD_CLOCK_FREQ_MASK        0x03
   #define  Si2147_POWER_UP_CMD_CLOCK_FREQ_MIN         0
   #define  Si2147_POWER_UP_CMD_CLOCK_FREQ_MAX         3
    #define Si2147_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ  2
   /* POWER_UP command, RESERVED8 field definition (address 12,size 8, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESERVED8_LSB         0
   #define  Si2147_POWER_UP_CMD_RESERVED8_MASK        0xff
   #define  Si2147_POWER_UP_CMD_RESERVED8_MIN         0
   #define  Si2147_POWER_UP_CMD_RESERVED8_MAX         0
    #define Si2147_POWER_UP_CMD_RESERVED8_RESERVED  0
   /* POWER_UP command, FUNC field definition (address 13,size 3, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_FUNC_LSB         0
   #define  Si2147_POWER_UP_CMD_FUNC_MASK        0x07
   #define  Si2147_POWER_UP_CMD_FUNC_MIN         0
   #define  Si2147_POWER_UP_CMD_FUNC_MAX         1
    #define Si2147_POWER_UP_CMD_FUNC_BOOTLOADER  0
    #define Si2147_POWER_UP_CMD_FUNC_NORMAL      1
   /* POWER_UP command, RESERVED9 field definition (address 13,size 4, lsb 3, unsigned) */
   #define  Si2147_POWER_UP_CMD_RESERVED9_LSB         3
   #define  Si2147_POWER_UP_CMD_RESERVED9_MASK        0x0f
   #define  Si2147_POWER_UP_CMD_RESERVED9_MIN         0
   #define  Si2147_POWER_UP_CMD_RESERVED9_MAX         0
    #define Si2147_POWER_UP_CMD_RESERVED9_RESERVED  0
   /* POWER_UP command, CTSIEN field definition (address 13,size 1, lsb 7, unsigned) */
   #define  Si2147_POWER_UP_CMD_CTSIEN_LSB         7
   #define  Si2147_POWER_UP_CMD_CTSIEN_MASK        0x01
   #define  Si2147_POWER_UP_CMD_CTSIEN_MIN         0
   #define  Si2147_POWER_UP_CMD_CTSIEN_MAX         1
    #define Si2147_POWER_UP_CMD_CTSIEN_DISABLE  0
    #define Si2147_POWER_UP_CMD_CTSIEN_ENABLE   1
   /* POWER_UP command, WAKE_UP field definition (address 14,size 1, lsb 0, unsigned) */
   #define  Si2147_POWER_UP_CMD_WAKE_UP_LSB         0
   #define  Si2147_POWER_UP_CMD_WAKE_UP_MASK        0x01
   #define  Si2147_POWER_UP_CMD_WAKE_UP_MIN         1
   #define  Si2147_POWER_UP_CMD_WAKE_UP_MAX         1
    #define Si2147_POWER_UP_CMD_WAKE_UP_WAKE_UP  1
#endif /* Si2147_POWER_UP_CMD */

/* Si2147_SET_PROPERTY command definition */
#define   Si2147_SET_PROPERTY_CMD 0x14

#ifdef    Si2147_SET_PROPERTY_CMD
  #define Si2147_SET_PROPERTY_CMD_CODE 0x010014

    typedef struct { /* Si2147_SET_PROPERTY_CMD_struct */
     unsigned char   reserved;
     unsigned int    prop;
     unsigned int    data;
   } Si2147_SET_PROPERTY_CMD_struct;


    typedef struct { /* Si2147_SET_PROPERTY_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
      unsigned char   reserved;
      unsigned int    data;
   }  Si2147_SET_PROPERTY_CMD_REPLY_struct;

   /* SET_PROPERTY command, RESERVED field definition (address 1,size 8, lsb 0, unsigned) */
   #define  Si2147_SET_PROPERTY_CMD_RESERVED_LSB         0
   #define  Si2147_SET_PROPERTY_CMD_RESERVED_MASK        0xff
   #define  Si2147_SET_PROPERTY_CMD_RESERVED_MIN         0
   #define  Si2147_SET_PROPERTY_CMD_RESERVED_MAX         255.0
   /* SET_PROPERTY command, PROP field definition (address 2,size 16, lsb 0, unsigned) */
   #define  Si2147_SET_PROPERTY_CMD_PROP_LSB         0
   #define  Si2147_SET_PROPERTY_CMD_PROP_MASK        0xffff
   #define  Si2147_SET_PROPERTY_CMD_PROP_MIN         0
   #define  Si2147_SET_PROPERTY_CMD_PROP_MAX         65535
    #define Si2147_SET_PROPERTY_CMD_PROP_PROP_MIN  0
    #define Si2147_SET_PROPERTY_CMD_PROP_PROP_MAX  65535
   /* SET_PROPERTY command, DATA field definition (address 4,size 16, lsb 0, unsigned) */
   #define  Si2147_SET_PROPERTY_CMD_DATA_LSB         0
   #define  Si2147_SET_PROPERTY_CMD_DATA_MASK        0xffff
   #define  Si2147_SET_PROPERTY_CMD_DATA_MIN         0
   #define  Si2147_SET_PROPERTY_CMD_DATA_MAX         65535
    #define Si2147_SET_PROPERTY_CMD_DATA_DATA_MIN  0
    #define Si2147_SET_PROPERTY_CMD_DATA_DATA_MAX  65535
   /* SET_PROPERTY command, RESERVED field definition (address 1, size 8, lsb 0, unsigned)*/
   #define  Si2147_SET_PROPERTY_RESPONSE_RESERVED_LSB         0
   #define  Si2147_SET_PROPERTY_RESPONSE_RESERVED_MASK        0xff
    #define Si2147_SET_PROPERTY_RESPONSE_RESERVED_RESERVED_MIN  0
    #define Si2147_SET_PROPERTY_RESPONSE_RESERVED_RESERVED_MAX  0
   /* SET_PROPERTY command, DATA field definition (address 2, size 16, lsb 0, unsigned)*/
   #define  Si2147_SET_PROPERTY_RESPONSE_DATA_LSB         0
   #define  Si2147_SET_PROPERTY_RESPONSE_DATA_MASK        0xffff

#endif /* Si2147_SET_PROPERTY_CMD */

/* Si2147_STANDBY command definition */
#define   Si2147_STANDBY_CMD 0x16

#ifdef    Si2147_STANDBY_CMD
  #define Si2147_STANDBY_CMD_CODE 0x010016

    typedef struct { /* Si2147_STANDBY_CMD_struct */
     unsigned char   type;
   } Si2147_STANDBY_CMD_struct;


    typedef struct { /* Si2147_STANDBY_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_STANDBY_CMD_REPLY_struct;

   /* STANDBY command, TYPE field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2147_STANDBY_CMD_TYPE_LSB         0
   #define  Si2147_STANDBY_CMD_TYPE_MASK        0x01
   #define  Si2147_STANDBY_CMD_TYPE_MIN         0
   #define  Si2147_STANDBY_CMD_TYPE_MAX         1
    #define Si2147_STANDBY_CMD_TYPE_LNA_OFF  1
    #define Si2147_STANDBY_CMD_TYPE_LNA_ON   0
#endif /* Si2147_STANDBY_CMD */

/* Si2147_TUNER_STATUS command definition */
#define   Si2147_TUNER_STATUS_CMD 0x42

#ifdef    Si2147_TUNER_STATUS_CMD
  #define Si2147_TUNER_STATUS_CMD_CODE 0x010042

    typedef struct { /* Si2147_TUNER_STATUS_CMD_struct */
     unsigned char   intack;
   } Si2147_TUNER_STATUS_CMD_struct;


    typedef struct { /* Si2147_TUNER_STATUS_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
      unsigned char   tcint;
      unsigned char   rssilint;
      unsigned char   rssihint;
               int    vco_code;
      unsigned char   tc;
      unsigned char   rssil;
      unsigned char   rssih;
               char   rssi;
      unsigned long   freq;
      unsigned char   mode;
   }  Si2147_TUNER_STATUS_CMD_REPLY_struct;

   /* TUNER_STATUS command, INTACK field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2147_TUNER_STATUS_CMD_INTACK_LSB         0
   #define  Si2147_TUNER_STATUS_CMD_INTACK_MASK        0x01
   #define  Si2147_TUNER_STATUS_CMD_INTACK_MIN         0
   #define  Si2147_TUNER_STATUS_CMD_INTACK_MAX         1
    #define Si2147_TUNER_STATUS_CMD_INTACK_CLEAR  1
    #define Si2147_TUNER_STATUS_CMD_INTACK_OK     0
   /* TUNER_STATUS command, TCINT field definition (address 1, size 1, lsb 0, unsigned)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_TCINT_LSB         0
   #define  Si2147_TUNER_STATUS_RESPONSE_TCINT_MASK        0x01
    #define Si2147_TUNER_STATUS_RESPONSE_TCINT_CHANGED    1
    #define Si2147_TUNER_STATUS_RESPONSE_TCINT_NO_CHANGE  0
   /* TUNER_STATUS command, RSSILINT field definition (address 1, size 1, lsb 1, unsigned)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSILINT_LSB         1
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSILINT_MASK        0x01
    #define Si2147_TUNER_STATUS_RESPONSE_RSSILINT_CHANGED    1
    #define Si2147_TUNER_STATUS_RESPONSE_RSSILINT_NO_CHANGE  0
   /* TUNER_STATUS command, RSSIHINT field definition (address 1, size 1, lsb 2, unsigned)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSIHINT_LSB         2
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSIHINT_MASK        0x01
    #define Si2147_TUNER_STATUS_RESPONSE_RSSIHINT_CHANGED    1
    #define Si2147_TUNER_STATUS_RESPONSE_RSSIHINT_NO_CHANGE  0
   /* TUNER_STATUS command, VCO_CODE field definition (address 10, size 16, lsb 0, signed)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_VCO_CODE_LSB         0
   #define  Si2147_TUNER_STATUS_RESPONSE_VCO_CODE_MASK        0xffff
   #define  Si2147_TUNER_STATUS_RESPONSE_VCO_CODE_SHIFT       16
   /* TUNER_STATUS command, TC field definition (address 2, size 1, lsb 0, unsigned)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_TC_LSB         0
   #define  Si2147_TUNER_STATUS_RESPONSE_TC_MASK        0x01
    #define Si2147_TUNER_STATUS_RESPONSE_TC_BUSY  0
    #define Si2147_TUNER_STATUS_RESPONSE_TC_DONE  1
   /* TUNER_STATUS command, RSSIL field definition (address 2, size 1, lsb 1, unsigned)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSIL_LSB         1
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSIL_MASK        0x01
    #define Si2147_TUNER_STATUS_RESPONSE_RSSIL_LOW  1
    #define Si2147_TUNER_STATUS_RESPONSE_RSSIL_OK   0
   /* TUNER_STATUS command, RSSIH field definition (address 2, size 1, lsb 2, unsigned)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSIH_LSB         2
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSIH_MASK        0x01
    #define Si2147_TUNER_STATUS_RESPONSE_RSSIH_HIGH  1
    #define Si2147_TUNER_STATUS_RESPONSE_RSSIH_OK    0
   /* TUNER_STATUS command, RSSI field definition (address 3, size 8, lsb 0, signed)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSI_LSB         0
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSI_MASK        0xff
   #define  Si2147_TUNER_STATUS_RESPONSE_RSSI_SHIFT       24
   /* TUNER_STATUS command, FREQ field definition (address 4, size 32, lsb 0, unsigned)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_FREQ_LSB         0
   #define  Si2147_TUNER_STATUS_RESPONSE_FREQ_MASK        0xffffffff
   /* TUNER_STATUS command, MODE field definition (address 8, size 1, lsb 0, unsigned)*/
   #define  Si2147_TUNER_STATUS_RESPONSE_MODE_LSB         0
   #define  Si2147_TUNER_STATUS_RESPONSE_MODE_MASK        0x01
    #define Si2147_TUNER_STATUS_RESPONSE_MODE_ATV  1
    #define Si2147_TUNER_STATUS_RESPONSE_MODE_DTV  0

#endif /* Si2147_TUNER_STATUS_CMD */

/* Si2147_TUNER_TUNE_FREQ command definition */
#define   Si2147_TUNER_TUNE_FREQ_CMD 0x41

#ifdef    Si2147_TUNER_TUNE_FREQ_CMD
  #define Si2147_TUNER_TUNE_FREQ_CMD_CODE 0x010041

    typedef struct { /* Si2147_TUNER_TUNE_FREQ_CMD_struct */
     unsigned char   mode;
     unsigned long   freq;
   } Si2147_TUNER_TUNE_FREQ_CMD_struct;


    typedef struct { /* Si2147_TUNER_TUNE_FREQ_CMD_REPLY_struct */
       Si2147_COMMON_REPLY_struct * STATUS;
   }  Si2147_TUNER_TUNE_FREQ_CMD_REPLY_struct;

   /* TUNER_TUNE_FREQ command, MODE field definition (address 1,size 1, lsb 0, unsigned) */
   #define  Si2147_TUNER_TUNE_FREQ_CMD_MODE_LSB         0
   #define  Si2147_TUNER_TUNE_FREQ_CMD_MODE_MASK        0x01
   #define  Si2147_TUNER_TUNE_FREQ_CMD_MODE_MIN         0
   #define  Si2147_TUNER_TUNE_FREQ_CMD_MODE_MAX         1
    #define Si2147_TUNER_TUNE_FREQ_CMD_MODE_ATV  1
    #define Si2147_TUNER_TUNE_FREQ_CMD_MODE_DTV  0
   /* TUNER_TUNE_FREQ command, FREQ field definition (address 4,size 32, lsb 0, unsigned) */
   #define  Si2147_TUNER_TUNE_FREQ_CMD_FREQ_LSB         0
   #define  Si2147_TUNER_TUNE_FREQ_CMD_FREQ_MASK        0xffffffff
   #define  Si2147_TUNER_TUNE_FREQ_CMD_FREQ_MIN         40000000
   #define  Si2147_TUNER_TUNE_FREQ_CMD_FREQ_MAX         1002000000
    #define Si2147_TUNER_TUNE_FREQ_CMD_FREQ_FREQ_MIN  40000000
    #define Si2147_TUNER_TUNE_FREQ_CMD_FREQ_FREQ_MAX  1002000000
#endif /* Si2147_TUNER_TUNE_FREQ_CMD */



  /* --------------------------------------------*/
  /* COMMANDS STRUCT                             */
  /* This is used to store all command fields    */
  /* --------------------------------------------*/
  typedef struct { /* Si2147_CmdObj struct */
    #ifdef    Si2147_AGC_OVERRIDE_CMD
              Si2147_AGC_OVERRIDE_CMD_struct     agc_override;
    #endif /* Si2147_AGC_OVERRIDE_CMD */
    #ifdef    Si2147_ATV_CW_TEST_CMD
              Si2147_ATV_CW_TEST_CMD_struct      atv_cw_test;
    #endif /* Si2147_ATV_CW_TEST_CMD */
    #ifdef    Si2147_ATV_RESTART_CMD
              Si2147_ATV_RESTART_CMD_struct      atv_restart;
    #endif /* Si2147_ATV_RESTART_CMD */
    #ifdef    Si2147_ATV_SCAN_TUNE_CMD
              Si2147_ATV_SCAN_TUNE_CMD_struct    atv_scan_tune;
    #endif /* Si2147_ATV_SCAN_TUNE_CMD */
    #ifdef    Si2147_ATV_STATUS_CMD
              Si2147_ATV_STATUS_CMD_struct       atv_status;
    #endif /* Si2147_ATV_STATUS_CMD */
    #ifdef    Si2147_CONFIG_CLOCKS_CMD
              Si2147_CONFIG_CLOCKS_CMD_struct    config_clocks;
    #endif /* Si2147_CONFIG_CLOCKS_CMD */
    #ifdef    Si2147_CONFIG_I2C_CMD
              Si2147_CONFIG_I2C_CMD_struct       config_i2c;
    #endif /* Si2147_CONFIG_I2C_CMD */
    #ifdef    Si2147_CONFIG_PINS_CMD
              Si2147_CONFIG_PINS_CMD_struct      config_pins;
    #endif /* Si2147_CONFIG_PINS_CMD */
    #ifdef    Si2147_DTV_RESTART_CMD
              Si2147_DTV_RESTART_CMD_struct      dtv_restart;
    #endif /* Si2147_DTV_RESTART_CMD */
    #ifdef    Si2147_DTV_STATUS_CMD
              Si2147_DTV_STATUS_CMD_struct       dtv_status;
    #endif /* Si2147_DTV_STATUS_CMD */
    #ifdef    Si2147_EXIT_BOOTLOADER_CMD
              Si2147_EXIT_BOOTLOADER_CMD_struct  exit_bootloader;
    #endif /* Si2147_EXIT_BOOTLOADER_CMD */
    #ifdef    Si2147_FINE_TUNE_CMD
              Si2147_FINE_TUNE_CMD_struct        fine_tune;
    #endif /* Si2147_FINE_TUNE_CMD */
    #ifdef    Si2147_GET_PROPERTY_CMD
              Si2147_GET_PROPERTY_CMD_struct     get_property;
    #endif /* Si2147_GET_PROPERTY_CMD */
    #ifdef    Si2147_GET_REV_CMD
              Si2147_GET_REV_CMD_struct          get_rev;
    #endif /* Si2147_GET_REV_CMD */
    #ifdef    Si2147_PART_INFO_CMD
              Si2147_PART_INFO_CMD_struct        part_info;
    #endif /* Si2147_PART_INFO_CMD */
    #ifdef    Si2147_POWER_DOWN_CMD
              Si2147_POWER_DOWN_CMD_struct       power_down;
    #endif /* Si2147_POWER_DOWN_CMD */
    #ifdef    Si2147_POWER_DOWN_HW_CMD
              Si2147_POWER_DOWN_HW_CMD_struct    power_down_hw;
    #endif /* Si2147_POWER_DOWN_HW_CMD */
    #ifdef    Si2147_POWER_UP_CMD
              Si2147_POWER_UP_CMD_struct         power_up;
    #endif /* Si2147_POWER_UP_CMD */
    #ifdef    Si2147_SET_PROPERTY_CMD
              Si2147_SET_PROPERTY_CMD_struct     set_property;
    #endif /* Si2147_SET_PROPERTY_CMD */
    #ifdef    Si2147_STANDBY_CMD
              Si2147_STANDBY_CMD_struct          standby;
    #endif /* Si2147_STANDBY_CMD */
    #ifdef    Si2147_TUNER_STATUS_CMD
              Si2147_TUNER_STATUS_CMD_struct     tuner_status;
    #endif /* Si2147_TUNER_STATUS_CMD */
    #ifdef    Si2147_TUNER_TUNE_FREQ_CMD
              Si2147_TUNER_TUNE_FREQ_CMD_struct  tuner_tune_freq;
    #endif /* Si2147_TUNER_TUNE_FREQ_CMD */
  } Si2147_CmdObj;


  /* --------------------------------------------*/
  /* COMMANDS REPLY STRUCT                       */
  /* This stores all command reply fields        */
  /* --------------------------------------------*/
  typedef struct { /* Si2147_CmdReplyObj struct */
    #ifdef    Si2147_AGC_OVERRIDE_CMD
              Si2147_AGC_OVERRIDE_CMD_REPLY_struct     agc_override;
    #endif /* Si2147_AGC_OVERRIDE_CMD */
    #ifdef    Si2147_ATV_CW_TEST_CMD
              Si2147_ATV_CW_TEST_CMD_REPLY_struct      atv_cw_test;
    #endif /* Si2147_ATV_CW_TEST_CMD */
    #ifdef    Si2147_ATV_RESTART_CMD
              Si2147_ATV_RESTART_CMD_REPLY_struct      atv_restart;
    #endif /* Si2147_ATV_RESTART_CMD */
    #ifdef    Si2147_ATV_SCAN_TUNE_CMD
              Si2147_ATV_SCAN_TUNE_CMD_REPLY_struct    atv_scan_tune;
    #endif /* Si2147_ATV_SCAN_TUNE_CMD */
    #ifdef    Si2147_ATV_STATUS_CMD
              Si2147_ATV_STATUS_CMD_REPLY_struct       atv_status;
    #endif /* Si2147_ATV_STATUS_CMD */
    #ifdef    Si2147_CONFIG_CLOCKS_CMD
              Si2147_CONFIG_CLOCKS_CMD_REPLY_struct    config_clocks;
    #endif /* Si2147_CONFIG_CLOCKS_CMD */
    #ifdef    Si2147_CONFIG_I2C_CMD
              Si2147_CONFIG_I2C_CMD_REPLY_struct       config_i2c;
    #endif /* Si2147_CONFIG_I2C_CMD */
    #ifdef    Si2147_CONFIG_PINS_CMD
              Si2147_CONFIG_PINS_CMD_REPLY_struct      config_pins;
    #endif /* Si2147_CONFIG_PINS_CMD */
    #ifdef    Si2147_DTV_RESTART_CMD
              Si2147_DTV_RESTART_CMD_REPLY_struct      dtv_restart;
    #endif /* Si2147_DTV_RESTART_CMD */
    #ifdef    Si2147_DTV_STATUS_CMD
              Si2147_DTV_STATUS_CMD_REPLY_struct       dtv_status;
    #endif /* Si2147_DTV_STATUS_CMD */
    #ifdef    Si2147_EXIT_BOOTLOADER_CMD
              Si2147_EXIT_BOOTLOADER_CMD_REPLY_struct  exit_bootloader;
    #endif /* Si2147_EXIT_BOOTLOADER_CMD */
    #ifdef    Si2147_FINE_TUNE_CMD
              Si2147_FINE_TUNE_CMD_REPLY_struct        fine_tune;
    #endif /* Si2147_FINE_TUNE_CMD */
    #ifdef    Si2147_GET_PROPERTY_CMD
              Si2147_GET_PROPERTY_CMD_REPLY_struct     get_property;
    #endif /* Si2147_GET_PROPERTY_CMD */
    #ifdef    Si2147_GET_REV_CMD
              Si2147_GET_REV_CMD_REPLY_struct          get_rev;
    #endif /* Si2147_GET_REV_CMD */
    #ifdef    Si2147_PART_INFO_CMD
              Si2147_PART_INFO_CMD_REPLY_struct        part_info;
    #endif /* Si2147_PART_INFO_CMD */
    #ifdef    Si2147_POWER_DOWN_CMD
              Si2147_POWER_DOWN_CMD_REPLY_struct       power_down;
    #endif /* Si2147_POWER_DOWN_CMD */
    #ifdef    Si2147_POWER_DOWN_HW_CMD
              Si2147_POWER_DOWN_HW_CMD_REPLY_struct    power_down_hw;
    #endif /* Si2147_POWER_DOWN_HW_CMD */
    #ifdef    Si2147_POWER_UP_CMD
              Si2147_POWER_UP_CMD_REPLY_struct         power_up;
    #endif /* Si2147_POWER_UP_CMD */
    #ifdef    Si2147_SET_PROPERTY_CMD
              Si2147_SET_PROPERTY_CMD_REPLY_struct     set_property;
    #endif /* Si2147_SET_PROPERTY_CMD */
    #ifdef    Si2147_STANDBY_CMD
              Si2147_STANDBY_CMD_REPLY_struct          standby;
    #endif /* Si2147_STANDBY_CMD */
    #ifdef    Si2147_TUNER_STATUS_CMD
              Si2147_TUNER_STATUS_CMD_REPLY_struct     tuner_status;
    #endif /* Si2147_TUNER_STATUS_CMD */
    #ifdef    Si2147_TUNER_TUNE_FREQ_CMD
              Si2147_TUNER_TUNE_FREQ_CMD_REPLY_struct  tuner_tune_freq;
    #endif /* Si2147_TUNER_TUNE_FREQ_CMD */
  } Si2147_CmdReplyObj;

#ifdef    Si2147_COMMAND_PROTOTYPES
#define   Si2147_GET_COMMAND_STRINGS
#endif /* Si2147_COMMAND_PROTOTYPES */

#endif /* Si2147_COMMANDS_H */







