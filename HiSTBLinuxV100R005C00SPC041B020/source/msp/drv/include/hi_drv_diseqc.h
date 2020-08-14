#ifndef __HI_DRV_DISEQC_H__
#define __HI_DRV_DISEQC_H__

#define LO_FREQUENCY_L (0x0)
#define LO_FREQUENCY_H (0x1)
#define POLARISATION_V (0x0)
#define POLARISATION_H (0x2)
/*#define SAT_POSITION_A (0x0)
#define SAT_POSITION_B (0x4)
#define SWITCH_OPTION_A (0x0)
#define SWITCH_OPTION_B (0x8)*/
#define PORT_GROUP_CLR_BITS (0xF0)

//#define DISEQC_CMD_LEN (6)

#define FRAMING_BYTE (0)
#define ADDRESS_BYTE (1)
#define COMMAND_BYTE (2)
#define DATA_BYTE_0 (3)
#define DATA_BYTE_1 (4)
#define DATA_BYTE_2 (5)

#define DISEQC_DELAY_BETWEEN_CMD_MS (25)

#define FORMAT_DISEQC_CMD(a, F, A, C) \
    { \
        a[FRAMING_BYTE] = F; \
        a[ADDRESS_BYTE] = A; \
        a[COMMAND_BYTE] = C; \
    }

#define FORMAT_DISEQC_CMD_VALUE(a, F, A, C, aD, L) \
    { \
        int i; \
        a[FRAMING_BYTE] = F; \
        a[ADDRESS_BYTE] = A; \
        a[COMMAND_BYTE] = C; \
        for (i = 0; i < L; i++){  a[DATA_BYTE_0 + i] = *((HI_U8*)aD + i); } \
    }

/* DiSEqC Framing code */
//typedef enum
//{
#define MASTER_NOREPLY_FIRST       0xE0
//#define MASTER_NOREPLY_REPEAT      0xE1
#define MASTER_REPLY_FIRST         0xE2
/*#define MASTER_REPLY_REPEAT        0xE3
#define SLAVE_OK                   0xE4
#define SLAVE_NOT_SUPPORT          0xE5
#define SLAVE_PARITY_ERROR         0xE6
#define SLAVE_BAD_COMMAND          0xE7*/
//} DISEQC_FRAMING_E;

#define DEVICE_ANY                      0x00
#define DEVICE_ANY_LNB_SW_SMATV         0x10
/*#define DEVICE_LNB                      0x11
#define DEVICE_LNB_LOOPTHROUGH          0x12
#define DEVICE_SW                       0x14
#define DEVICE_SW_LOOPTHROUGH           0x15
#define DEVICE_SMATV                    0x18
#define DEVICE_ANY_POLARISER            0x20
#define DEVICE_LINEAR_POLARISER         0x21
#define DEVICE_ANY_POSITIONER           0x30*/
#define DEVICE_AZIMUTH_POSITIONER       0x31
/*#define DEVICE_ELEVATION_POSITIONER     0x32
#define DEVICE_ANY_INSTALLER_AID        0x40
#define DEVICE_SIGNAL_STRENGTH          0x41
#define DEVICE_ANY_INTELLIGENT          0x70
#define DEVICE_SUBSCRIBER_CONTROL       0x71
#define DEVICE_BUTT                     0x72*/ 

        /* M*R/1.0 */
#define CMD_RESET             0x00       /**<Reset DiSEqC microcontroller*/
        /* R/2.0 */
//#define CMD_CLR_RESET         0x01   /**<Clear the "Reset" flag*/
        /* R/1.0 */
#define CMD_STANDBY           0x02    /**<Switch peripheral power supply off*/
#define CMD_POWER_ON          0x03    /**<Switch peripheral power supply on*/
        /* S/2.0 */
//#define CMD_SET_CONTEND       0x04 /**<Set Contention flag*/
//#define CMD_CONTEND           0x05     /**<Return address only if Contention flag is set*/
        /* R/2.0 */
//#define CMD_CLR_CONTEND       0x06 /**<Clear Contention flag*/
        /* S/2.0 */
//#define CMD_ADDRESS           0x07     /**<Return address unless Contention flag is set*/
//#define CMD_MOVE_C            0x08      /**<Change address only if Contention flag is set*/
        /* R/2.0 */
#if 0
#define CMD_MOVE              0x90    /**<Change address unless Contention flag is set*/
#define CMD_STATUS            0x10    /**<Read Status register flags*/
#define CMD_CONFIG            0x11    /**<Read Configuration flags (peripheral hardware)*/
#define CMD_SWITCH_0          0x14    /**<Read Switching state flags (Committed port)*/
#define CMD_SWITCH_1          0x15    /**<Read Switching state flags (Uncommitted port)*/
#define CMD_SWITCH_2          0x16
#define CMD_SWITCH_3          0x17
 
        /* R/1.0 */
#define CMD_SET_LO            0x20       /**<Select the Low Local Oscillator frequency*/
#define CMD_SET_VR            0x21       /**<Select Vertical Polarisation (or Right circular)*/
#define CMD_SET_POS_A         0x22    /**<Select Satellite position A (or position C)*/
#define CMD_SET_S0_A          0x23     /**<Select Switch Option A (e.g. positions A/B)*/
#define CMD_SET_HI            0x24       /**<Select the High Local Oscillator frequency*/
#define CMD_SET_HL            0x25       /**<Select Horizontal Polarisation (or Left circular)*/
#define CMD_SET_POS_B         0x26    /**<Select Satellite position B (or position D)*/
#define CMD_SET_S0_B          0x27     /**<Select Switch Option B (e.g. positions C/D)*/
        /* R/1.1 */                
#define CMD_SET_S1_A          0x28     /**<Select switch S1 input A (deselect input B)*/
#define CMD_SET_S2_A          0x29     /**<Select switch S2 input A (deselect input B)*/
#define CMD_SET_S3_A          0x2A     /**<Select switch S3 input A (deselect input B)*/
#define CMD_SET_S4_A          0x2B     /**<Select switch S4 input A (deselect input B)*/
#define CMD_SET_S1_B          0x2C     /**<Select switch S1 input B (deselect input A)*/
#define CMD_SET_S2_B          0x2D     /**<Select switch S2 input B (deselect input A)*/
#define CMD_SET_S3_B          0x2E     /**<Select switch S3 input B (deselect input A)*/
#define CMD_SET_S4_B          0x2F     /**<Select switch S4 input B (deselect input A)*/
#define CMD_SLEEP             0x30        /**<Ignore all bus commands except "Awake"*/
#define CMD_AWAKE             0x31        /**<Respond to future bus commands normally*/
#endif
        /* M/1.0 */
#define CMD_WRITE_N0          0x38    /**<Write to Port group 0 (Committed switches)*/
        /* M/1.1 */           
#define CMD_WRITE_N1          0x39    /**<Write to Port group 1 (Uncommitted switches)*/
//#define CMD_WRITE_N2          0x3A
//#define CMD_WRITE_N3          0x3B
 
        /* S/2.0 */
#if 0
#define CMD_READ_A0           0x40      /**<Read Analogue value A0*/
#define CMD_READ_A1           0x41      /**<Read Analogue value A1*/
        /* R/1.2 */
#define CMD_WRITE_A0          0x48    /**<Write Analogue value A0 (e.g. Skew)*/
        /* S/1.2 */          
#define CMD_WRITE_A1          0x49    /**<Write Analogue value A1*/
        /* S/2.0 */
#define CMD_LO_STRING         0x50   /**<Read current frequency [Reply = BCD string]*/
        /* R/2.0 */
#define CMD_LO_NOW            0x51      /**<Read current frequency table entry number*/
        /* S/2.0 */
#define CMD_LO_LO             0x52       /**<Read Lo frequency table entry number*/
#define CMD_LO_HI             0x53       /**<Read Hi frequency table entry number*/
#endif
        /* M/1.1 */
//#define CMD_WRITE_RREQ        0x58  /**<Write channel frequency (BCD string)*/
//#define CMD_CH_NO             0x59       /**<Write (receiver's) selected channel number*/
        /* M/1.2 */
#define CMD_HALT              0x60        /**<Stop Positioner movement*/
#define CMD_LIMITS_OFF        0x63  /**<Disable Limits*/
        /* R/2.2 */
//#define CMD_POS_STAT          0x64    /**<Read Positioner Status Register*/
        /* M/1.2 */
#define CMD_LIMIT_E           0x66     /**<Set East Limit (& Enable recommended)*/
#define CMD_LIMIT_W           0x67     /**<Set West Limit (& Enable recommended)*/
#define CMD_DRIVE_EAST        0x68  /**<Drive Motor East (with optional timeout/steps)*/
#define CMD_DRIVE_WEST        0x69  /**<Drive Motor West (with optional timeout/steps)*/
#define CMD_STORE_NN          0x6A    /**<Store Satellite Position & Enable Limits*/
#define CMD_GOTO_NN           0x6B     /**<Drive Motor to Satellite Position nn*/
#define CMD_GOTO_XX           0x6E     /**<Drive Motor to Angular Position*/
        /* R/1.2 */
#define CMD_SET_POSNS         0x6F   /**<(Re-)Calculate Satellite Positions*/
//#define CMD_BUTT

#endif

