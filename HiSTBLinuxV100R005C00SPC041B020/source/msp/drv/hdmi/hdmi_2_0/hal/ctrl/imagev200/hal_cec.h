#ifndef __HAL_CEC_H__
#define __HAL_CEC_H__
#ifdef HDMI_CEC_SUPPORT
#include "si_datatypes.h"
#include "si_drv_cpi_api.h"
#include "drv_hdmi_platform.h"

#define CEC_BASE_ADDRESS 0x3F00

#define MAKE_SRCDEST( src, dest )   ((( (src) << 4) & 0xF0) | ((dest) & 0x0F))
#define GET_CEC_SRC( srcDest )      (( srcDest >> 4) & 0x0F)
#define GET_CEC_DEST( srcDest )     (( srcDest >> 0) & 0x0F)
typedef enum // Operands for <Power Status> Opcode
{
	CEC_POWERSTATUS_ON              = 0x00,
	CEC_POWERSTATUS_STANDBY         = 0x01,
	CEC_POWERSTATUS_STANDBY_TO_ON   = 0x02,
	CEC_POWERSTATUS_ON_TO_STANDBY   = 0x03,
}HDMI_CEC_POWER_STATUS_S;


typedef enum _SiiCecLogicalAddresses_t
{
	CEC_LOGADDR_TV          	= 0x00,
	CEC_LOGADDR_RECDEV1     	= 0x01,
	CEC_LOGADDR_RECDEV2         = 0x02,
	CEC_LOGADDR_TUNER1          = 0x03,     // STB1 in Spev v1.3
	CEC_LOGADDR_PLAYBACK1       = 0x04,     // DVD1 in Spev v1.3
	CEC_LOGADDR_AUDSYS          = 0x05,
	CEC_LOGADDR_PURE_SWITCH     = 0x06,     // STB2 in Spec v1.3
	CEC_LOGADDR_VIDEO_PROCESSOR = 0x07,     // STB3 in Spec v1.3
	CEC_LOGADDR_PLAYBACK2   	= 0x08,     // DVD2 in Spec v1.3
	CEC_LOGADDR_RECDEV3    	 	= 0x09,
	CEC_LOGADDR_TUNER4      	= 0x0A,     // RES1 in Spec v1.3
	CEC_LOGADDR_PLAYBACK3   	= 0x0B,     // RES2 in Spec v1.3
	CEC_LOGADDR_RES3        	= 0x0C,
	CEC_LOGADDR_RES4        	= 0x0D,
	CEC_LOGADDR_FREEUSE     	= 0x0E,
	CEC_LOGADDR_UNREGORBC   	= 0x0F

} SiiCecLogicalAddresses_t;

typedef enum _SiiCecDeviceTypes_t   // CEC device types
{
	CEC_DT_TV,
	CEC_DT_RECORDING_DEVICE,
	CEC_DT_RSVD,
	CEC_DT_TUNER,
	CEC_DT_PLAYBACK,
	CEC_DT_AUDIO_SYSTEM,
	CEC_DT_PURE_CEC_SWITCH,
	CEC_DT_VIDEO_PROCESSOR,

	CEC_DT_COUNT
} SiiCecDeviceTypes_t;

typedef enum _SiiCecOpcodes_t       // CEC Messages
{
	CECOP_SII_SENDPING              = 0x100,    // Special opcode for SiiDrvCpiWrite()
	CECOP_FEATURE_ABORT             = 0x00,
	CECOP_IMAGE_VIEW_ON             = 0x04,
	CECOP_TUNER_STEP_INCREMENT      = 0x05,     // N/A
	CECOP_TUNER_STEP_DECREMENT      = 0x06,     // N/A
	CECOP_TUNER_DEVICE_STATUS       = 0x07,     // N/A
	CECOP_GIVE_TUNER_DEVICE_STATUS  = 0x08,     // N/A
	CECOP_RECORD_ON                 = 0x09,     // N/A
	CECOP_RECORD_STATUS             = 0x0A,     // N/A
	CECOP_RECORD_OFF                = 0x0B,     // N/A
	CECOP_TEXT_VIEW_ON              = 0x0D,
	CECOP_RECORD_TV_SCREEN          = 0x0F,     // N/A
	CECOP_GIVE_DECK_STATUS          = 0x1A,
	CECOP_DECK_STATUS               = 0x1B,
	CECOP_SET_MENU_LANGUAGE         = 0x32,
	CECOP_CLEAR_ANALOGUE_TIMER      = 0x33,     // Spec 1.3A
	CECOP_SET_ANALOGUE_TIMER        = 0x34,     // Spec 1.3A
	CECOP_TIMER_STATUS              = 0x35,     // Spec 1.3A
	CECOP_STANDBY                   = 0x36,
	CECOP_PLAY                      = 0x41,
	CECOP_DECK_CONTROL              = 0x42,
	CECOP_TIMER_CLEARED_STATUS      = 0x43,     // Spec 1.3A
	CECOP_USER_CONTROL_PRESSED      = 0x44,
	CECOP_USER_CONTROL_RELEASED     = 0x45,
	CECOP_GIVE_OSD_NAME             = 0x46,
	CECOP_SET_OSD_NAME              = 0x47,
	CECOP_SET_OSD_STRING            = 0x64,
	CECOP_SET_TIMER_PROGRAM_TITLE   = 0x67,     // Spec 1.3A
	CECOP_SYSTEM_AUDIO_MODE_REQUEST = 0x70,     // Spec 1.3A
	CECOP_GIVE_AUDIO_STATUS         = 0x71,     // Spec 1.3A
	CECOP_SET_SYSTEM_AUDIO_MODE     = 0x72,     // Spec 1.3A
	CECOP_REPORT_AUDIO_STATUS       = 0x7A,     // Spec 1.3A
	CECOP_GIVE_SYSTEM_AUDIO_MODE_STATUS = 0x7D, // Spec 1.3A
	CECOP_SYSTEM_AUDIO_MODE_STATUS  = 0x7E,     // Spec 1.3A
	CECOP_ROUTING_CHANGE            = 0x80,
	CECOP_ROUTING_INFORMATION       = 0x81,
	CECOP_ACTIVE_SOURCE             = 0x82,
	CECOP_GIVE_PHYSICAL_ADDRESS     = 0x83,
	CECOP_REPORT_PHYSICAL_ADDRESS   = 0x84,
	CECOP_REQUEST_ACTIVE_SOURCE     = 0x85,
	CECOP_SET_STREAM_PATH           = 0x86,
	CECOP_DEVICE_VENDOR_ID          = 0x87,
	CECOP_VENDOR_COMMAND            = 0x89,
	CECOP_VENDOR_REMOTE_BUTTON_DOWN = 0x8A,
	CECOP_VENDOR_REMOTE_BUTTON_UP   = 0x8B,
	CECOP_GIVE_DEVICE_VENDOR_ID     = 0x8C,
	CECOP_MENU_REQUEST              = 0x8D,
	CECOP_MENU_STATUS               = 0x8E,
	CECOP_GIVE_DEVICE_POWER_STATUS  = 0x8F,
	CECOP_REPORT_POWER_STATUS       = 0x90,
	CECOP_GET_MENU_LANGUAGE         = 0x91,
	CECOP_SELECT_ANALOGUE_SERVICE   = 0x92,     // Spec 1.3A    N/A
	CECOP_SELECT_DIGITAL_SERVICE    = 0x93,     //              N/A
	CECOP_SET_DIGITAL_TIMER         = 0x97,     // Spec 1.3A
	CECOP_CLEAR_DIGITAL_TIMER       = 0x99,     // Spec 1.3A
	CECOP_SET_AUDIO_RATE            = 0x9A,     // Spec 1.3A
	CECOP_INACTIVE_SOURCE           = 0x9D,     // Spec 1.3A
	CECOP_CEC_VERSION               = 0x9E,     // Spec 1.3A
	CECOP_GET_CEC_VERSION           = 0x9F,     // Spec 1.3A
	CECOP_VENDOR_COMMAND_WITH_ID    = 0xA0,     // Spec 1.3A
	CECOP_CLEAR_EXTERNAL_TIMER      = 0xA1,     // Spec 1.3A
	CECOP_SET_EXTERNAL_TIMER        = 0xA2,     // Spec 1.3A

	CECOP_REPORT_SHORT_AUDIO    	= 0xA3,     // Spec 1.4
	CECOP_REQUEST_SHORT_AUDIO    	= 0xA4,     // Spec 1.4

	CECOP_INITIATE_ARC              = 0xC0,
	CECOP_REPORT_ARC_INITIATED      = 0xC1,
	CECOP_REPORT_ARC_TERMINATED     = 0xC2,

	CECOP_REQUEST_ARC_INITIATION    = 0xC3,
	CECOP_REQUEST_ARC_TERMINATION   = 0xC4,
	CECOP_TERMINATE_ARC             = 0xC5,

	CDCOP_HEADER                    = 0xF8,
	CECOP_ABORT                     = 0xFF,

} SiiCecOpcodes_t;

typedef enum _SiiCecDevState_t
{
	CEC_DEVSTATE_ON                 = 1,
	CEC_DEVSTATE_STANDBY            = 2,
	CEC_DEVSTATE_OFF                = 3,
	CEC_DEVSTATE_IMAGE_DISPLAY      = 4,
	CEC_DEVSTATE_MENU_DISPLAY       = 5,
	CEC_DEVSTATE_TEXT_DISPLAY       = 6,
	CEC_DEVSTATE_MENU_ACTIVE        = 7,
	CEC_DEVSTATE_MENU_INACTIVE      = 8,
	CEC_DEVSTATE_RECORDING          = 9,
	CEC_DEVSTATE_NOT_RECORDING      = 10,
	CEC_DEVSTATE_DECK_ACTIVE        = 11,
	CEC_DEVSTATE_DECK_INACTIVE      = 12
} SiiCecDevState_t;

typedef enum _SiiCecAbortReason_t   // Operands for <Feature Abort> Opcode
{
	CECAR_UNRECOG_OPCODE            = 0x00,
	CECAR_NOT_CORRECT_MODE,
	CECAR_CANT_PROVIDE_SOURCE,
	CECAR_INVALID_OPERAND,
	CECAR_REFUSED,
	CECAR_UNABLE_TO_DETERMINE
} SiiCecAbortReason_t;


typedef enum _SiiCecDeckControl_t   // Operands for <Deck Control> Opcode
{
	CEC_DECKCONTROL_SKIP_FWD        = 0x01,
	CEC_DECKCONTROL_SKIP_BACK       = 0x02,
	CEC_DECKCONTROL_STOP            = 0x03,
	CEC_DECKCONTROL_EJECT           = 0x04
} SiiCecDeckControl_t;

typedef enum _SiiCecDeckStatus_t
{
	/* Operands for <Give Deck Status> Opcode   */

	CEC_DECKSTATUS_ON               = 0x01,
	CEC_DECKSTATUS_OFF              = 0x02,
	CEC_DECKSTATUS_ONCE             = 0x03,

	/* Operands for <Deck Status> Opcode        */

	CEC_DECKSTATUS_PLAY             = 0x11,
	CEC_DECKSTATUS_RECORD           = 0x12,
	CEC_DECKSTATUS_PLAY_REV         = 0x13,
	CEC_DECKSTATUS_STILL            = 0x14,
	CEC_DECKSTATUS_SLOW             = 0x15,
	CEC_DECKSTATUS_SLOW_REV         = 0x16,
	CEC_DECKSTATUS_SEARCH_FWD       = 0x17,
	CEC_DECKSTATUS_SEARCH_REV       = 0x18,
	CEC_DECKSTATUS_NO_MEDIA         = 0x19,
	CEC_DECKSTATUS_STOP             = 0x1A,
	CEC_DECKSTATUS_WIND             = 0x1B,
	CEC_DECKSTATUS_REWIND           = 0x1C,
	CEC_DECKSTATUS_INDEX_SEARCH_FWD = 0x1D,
	CEC_DECKSTATUS_INDEX_SEARCH_REV = 0x1E,
	CEC_DECKSTATUS_OTHER_STATUS     = 0x1F
} SiiCecDeckStatus_t;

typedef enum _SiiCecPlayModes_t     // Operand for <Play> Opcode
{
	CEC_PLAY_SCAN_FWD_MIN           = 0x05,
	CEC_PLAY_SCAN_FWD_MED           = 0x06,
	CEC_PLAY_SCAN_FWD_MAX           = 0x07,
	CEC_PLAY_SCAN_REV_MIN           = 0x09,
	CEC_PLAY_SCAN_REV_MED           = 0x0A,
	CEC_PLAY_SCAN_REV_MAX           = 0x0B,
	CEC_PLAY_SLOW_FWD_MIN           = 0x15,
	CEC_PLAY_SLOW_FWD_MED           = 0x16,
	CEC_PLAY_SLOW_FWD_MAX           = 0x17,
	CEC_PLAY_SLOW_REV_MIN           = 0x19,
	CEC_PLAY_SLOW_REV_MED           = 0x1A,
	CEC_PLAY_SLOW_REV_MAX           = 0x1B,
	CEC_PLAY_REVERSE                = 0x20,
	CEC_PLAY_FORWARD                = 0x24,
	CEC_PLAY_STILL                  = 0x25
} SiiCecPlayModes_t;

typedef enum _SiiCecMenuRequests_t
{
	CEC_MENUREQ_ACTIVATE            = 0x00,
	CEC_MENUREQ_DEACTIVATE          = 0x01,
	CEC_MENUREQ_QUERY               = 0x02
} SiiCecMenuRequests_t;

typedef enum _SiiCecMenuStates_t
{
	CEC_MENUSTATE_ACTIVATED         = 0x00,
	CEC_MENUSTATE_DEACTIVATED       = 0x01
} SiiCecMenuStates_t;

typedef enum _SiiCecUiCommand_t         // Remote Control Pass-through and UI command codes
{
	CEC_RC_SELECT                       = 0x00,
	CEC_RC_UP                           = 0x01,
	CEC_RC_DOWN                         = 0x02,
	CEC_RC_LEFT                         = 0x03,
	CEC_RC_RIGHT                        = 0x04,
	CEC_RC_RIGHT_UP                     = 0x05,
	CEC_RC_RIGHT_DOWN                   = 0x06,
	CEC_RC_LEFT_UP                      = 0x07,
	CEC_RC_LEFT_DOWN                    = 0x08,
	CEC_RC_ROOT_MENU                    = 0x09,
	CEC_RC_SETUP_MENU                   = 0x0A,
	CEC_RC_CONTENTS_MENU                = 0x0B,
	CEC_RC_FAVORITE_MENU                = 0x0C,
	CEC_RC_EXIT                         = 0x0D,
	// 0x0E - 0x1F Reserved 
	CEC_RC_0                            = 0x20,
	CEC_RC_1                            = 0x21,
	CEC_RC_2                            = 0x22,
	CEC_RC_3                            = 0x23,
	CEC_RC_4                            = 0x24,
	CEC_RC_5                            = 0x25,
	CEC_RC_6                            = 0x26,
	CEC_RC_7                            = 0x27,
	CEC_RC_8                            = 0x28,
	CEC_RC_9                            = 0x29,
	CEC_RC_DOT                          = 0x2A,
	CEC_RC_ENTER                        = 0x2B,
	CEC_RC_CLEAR                        = 0x2C,
	// 0x2D - 0x2F Reserved 
	CEC_RC_CHANNEL_UP                   = 0x30,
	CEC_RC_CHANNEL_DOWN                 = 0x31,
	CEC_RC_PREVIOUS_CHANNEL             = 0x32,
	CEC_RC_SOUND_SELECT                 = 0x33,
	CEC_RC_INPUT_SELECT                 = 0x34,
	CEC_RC_DISPLAY_INFORMATION          = 0x35,
	CEC_RC_HELP                         = 0x36,
	CEC_RC_PAGE_UP                      = 0x37,
	CEC_RC_PAGE_DOWN                    = 0x38,
	// 0x39 - 0x3F Reserved 
	CEC_RC_POWER                        = 0x40,
	CEC_RC_VOLUME_UP                    = 0x41,
	CEC_RC_VOLUME_DOWN                  = 0x42,
	CEC_RC_MUTE                         = 0x43,
	CEC_RC_PLAY                         = 0x44,
	CEC_RC_STOP                         = 0x45,
	CEC_RC_PAUSE                        = 0x46,
	CEC_RC_RECORD                       = 0x47,
	CEC_RC_REWIND                       = 0x48,
	CEC_RC_FAST_FORWARD                 = 0x49,
	CEC_RC_EJECT                        = 0x4A,
	CEC_RC_FORWARD                      = 0x4B,
	CEC_RC_BACKWARD                     = 0x4C,
	CEC_RC_STOP_RECORD                  = 0x4D,
	CEC_RC_PAUSE_RECORD                 = 0x4E,
	// 0x4F Reserved
	CEC_RC_ANGLE                        = 0x50,
	CEC_RC_SUB_PICTURE                  = 0x51,
	CEC_RC_VIDEO_ON_DEMAND              = 0x52,
	CEC_RC_ELECTRONIC_PROGRAM_GUIDE     = 0x53,
	CEC_RC_TIMER_PGRMING                = 0x54,
	CEC_RC_INITIAL_CONFIGURATION        = 0x55,
	CEC_RC_SELECT_BROADCAST_TYPE        = 0x56,
	CEC_RC_SELECT_SOUND_PRESENTATION    = 0x57,
	// 0x58 - 0x5F Reserved
	CEC_RC_PLAY_FUNCTION                = 0x60,
	CEC_RC_PAUSE_PLAY_FUNCTION          = 0x61,
	CEC_RC_RECORD_FUNCTION              = 0x62,
	CEC_RC_PAUSE_RECORD_FUNCTION        = 0x63,
	CEC_RC_STOP_FUNCTION                = 0x64,
	CEC_RC_MUTE_FUNCTION                = 0x65,
	CEC_RC_RESTORE_VOLUME_FUNCTION      = 0x66,
	CEC_RC_TUNE_FUNCTION                = 0x67,
	CEC_RC_SELECT_DISK_FUNCTION         = 0x68,
	CEC_RC_SELECT_AV_INPUT_FUNCTION     = 0x69,
	CEC_RC_SELECT_AUDIO_INPUT_FUNCTION  = 0x6A,
	CEC_RC_POWER_TOGGLE_FUNCTION        = 0x6B,
	CEC_RC_POWER_OFF_FUNCTION           = 0x6C,
	CEC_RC_POWER_ON_FUNCTION            = 0x6D,
	// 0x6E - 0x70 Reserved
	CEC_RC_F1_BLUE                      = 0x71,
	CEC_RC_F2_RED                       = 0x72,
	CEC_RC_F3_GREEN                     = 0x73,
	CEC_RC_F4_YELLOW                    = 0x74,
	CEC_RC_F5                           = 0x75,
	CEC_RC_DATA                         = 0x76
	// 0x77 - 0x7F Reserved
} SiiCecUiCommand_t;

typedef enum 
{
	RESULT_CEC_SUCCESS,             // Success result code
	RESULT_CEC_FAIL,                // General Failure result code

	RESULT_CEC_INVALID_PARAMETER,

	RESULT_CEC_INVALID_LOGICAL_ADDRESS,
	RESULT_CEC_INVALID_PHYSICAL_ADDRESS,
	RESULT_CEC_INVALID_PORT_INDEX,
	RESULT_CEC_NOT_ADJACENT,
	RESULT_CEC_NO_PA_FOUND,
	RESULT_CEC_TASK_QUEUE_FULL,
	RESULT_CEC_NO_LA_FOUND,

} SiiDrvCecError_t;


typedef enum
{
	SiiCEC_POWERSTATE_CHANGE    = 0x0001,
	SiiCEC_SOURCE_LOST          = 0x0002
} SiiCecStatus_t;



typedef struct 
{
	HI_S32   deviceType;    // 0 - Device is a TV.
                        	// 1 - Device is a Recording device
                        	// 2 - Device is a reserved device
                        	// 3 - Device is a Tuner
                        	// 4 - Device is a Playback device
                        	// 5 - Device is an Audio System
	HI_U16    devPA;        // CEC Physical address of the device.
	HI_U16    devLA;
	HI_BOOL   selected;
	HI_U8     osdName[10];
	HI_U32    vendorId;
	HDMI_CEC_POWER_STATUS_S   sourcePowerStatus;
    
}HDMI_CEC_DEVICE_S;

#define CEC_MSG_POOL_SIZE 512
#define CEC_MAX_ARG_SIZE 16
typedef struct 
{
	HI_U8         srcDestAddr;            // Source in upper nibble, dest in lower nibble
	HI_U16        opcode;
	HI_U8         args[ CEC_MAX_ARG_SIZE ];
	HI_U8         argCount;
}HDMI_CEC_MSG_S;

typedef struct
{
    HDMI_MUTEX          msgMutex;
    HDMI_WAIT_QUEUE     msgWaitQueue;
    HI_U16              msgWritePtr;
    HI_U16              msgReadPtr;
    HI_U16              msgValidNum;
    HDMI_CEC_MSG_S      cecMsgPool[CEC_MSG_POOL_SIZE];
}HDMI_CEC_MSG_QUEUE_S; 

typedef struct 
{
    HI_BOOL                  bEnable;
    HI_BOOL                  isActiveSource;
    HI_U8                    logicalAddr;
    HI_U8                    network[16];
    HI_U16                   physicalAddr;
	HI_U8                    osdName[14];
	HI_U32                   osdNameLen;
    HDMI_CEC_POWER_STATUS_S  sourcePowerStatus;
    HDMI_CEC_POWER_STATUS_S  powerState;
    HDMI_CEC_DEVICE_S        logicalDeviceInfo [16];
	HI_BOOL		             bWakeUpFlag;	
    HDMI_CEC_MSG_QUEUE_S     msgQueue;
    HI_BOOL                  bNetValid;
}HDMI_HAL_CEC_S;

HI_S32 SiiCecMsgSend(HDMI_HAL_CEC_S* pstCec, SiiCpiData_t* cecFrame);
HI_S32 SiiCecMsgRead(HDMI_HAL_CEC_S* pstCec, HDMI_CEC_MSG_S* msg);
HDMI_HAL_CEC_S* SiiCecDeviceCreate(SiiInst_t inst, HI_U16 cpiBaseAddr );
void SiiCecDeviceDelete(HDMI_HAL_CEC_S* pstCec);
HI_S32 SiiCecAutoPing(HDMI_HAL_CEC_S* pstCec);
HI_VOID SiiCecEnable(HDMI_HAL_CEC_S* pstCec, HI_BOOL bEnable);
HI_VOID SiiCecAutoPingEnable(HDMI_HAL_CEC_S* pstCec);

#endif
#endif
