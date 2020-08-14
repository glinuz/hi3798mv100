/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_debug.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/


#include "drv_hdmi_platform.h"
#include "hdmi_hal_intf.h"
#include "hdmi_platform.h"


#define DEBUG_NULL_CHK_NO_RET(p) do{\
	if(HI_NULL == p)\
	{HDMI_WARN("%s is null pointer!return fail.\n",#p);\
	return ;}\
}while(0)

#define HDMI_HAL_INFO           HI_DRV_PROC_EchoHelper


const static HI_CHAR *s_acStrProccessTalbe[]=
{
    "UNDO", //PROCESS_UNDO,
    "OK",   //PROCESS_OK,
    "FAIL", //PROCESS_FAIL,
};

#define STR_PROCESS(n)  ({(n>=HDMI_ARRAY_SIZE(s_acStrProccessTalbe)) ? "ERROR" : s_acStrProccessTalbe[n];})

const static  HI_CHAR *s_acStrBoolTalbe[]=
{
    "NO",   
    "YES", 
};

#define STR_BOOL(n)  ({(n>=HDMI_ARRAY_SIZE(s_acStrBoolTalbe)) ? "ERROR" : s_acStrBoolTalbe[n];})

const HI_CHAR *s_acStrDdcFunTalbe[]=
{
    "EDID",     //DDC_FUNC_TYPE_EDID, //NO ACK
    "HDCP",     //DDC_FUNC_TYPE_HDCP, //NO ACK
    "SCDC",     //DDC_FUNC_TYPE_SCDC, //ACK
};
#define STR_DDC_FUN(n)  ({(n>=HDMI_ARRAY_SIZE(s_acStrDdcFunTalbe)) ? "ERROR" : s_acStrDdcFunTalbe[n];})


const static  HI_CHAR *s_acStrDdcIssueModeTalbe[]=
{

    "R_Sing_N_Ack",         //DDC_MODE_READ_SINGLE_NO_ACK,    //0b0000
    "R_Sing_Y_Ack",         //DDC_MODE_READ_SINGLE_ACK,       //0b0001
    "R_Muti_N_Ack",         //DDC_MODE_READ_MUTIL_NO_ACK,     //0b0010
    "R_Muti_Y_Ack",         //DDC_MODE_READ_MUTIL_ACK,        //0b0011
    "R_Segm_N_Ack",         //DDC_MODE_READ_SEGMENT_NO_ACK,   //0b0100
    "R_Segm_Y_Ack",         //DDC_MODE_READ_SEGMENT_ACK,      //0b0101
    "W_Muti_N_ack",         //DDC_MODE_WRITE_MUTIL_NO_ACK,    //0b0110
    "W_Muti_Y_ack",         //DDC_MODE_WRITE_MUTIL_ACK,       //0b0111
                //DDC_MODE_BUTT,
    
};
#define STR_DDC_MODE(n)  ({(n>=HDMI_ARRAY_SIZE(s_acStrDdcIssueModeTalbe)) ? "ERROR" : s_acStrDdcIssueModeTalbe[n];})

const static  HI_CHAR *s_acStrDdcMasterModeTalbe[]=
{
    "PWD",  //DDC_MASTER_MODE_PWD  = 0x0,
    "AON",  //DDC_MASTER_MODE_AON  = 0x1,
};
#define STR_DDC_MASTER(n)  ({(n>=HDMI_ARRAY_SIZE(s_acStrDdcMasterModeTalbe)) ? "ERROR" : s_acStrDdcMasterModeTalbe[n];})


const static  HI_CHAR *s_acStrScdcErr[]=
{

	"SINK_UNREAD",      //SCDC_ERROR_SINK_UNREAD,
    "SCRAMBLE",         //SCDC_ERROR_SCRAMBLE,
	"OE_DISABLE",       //SCDC_ERROR_OE_DISABLE,
	"OE_ENABLE",        //SCDC_ERROR_OE_ENABLE,
	//SCDC_ERROR_RR_TEST,
};
#define STR_SCDC_ERR(n)  ({(n>=HDMI_ARRAY_SIZE(s_acStrScdcErr)) ? "ERROR" : s_acStrScdcErr[n];})


const static  HI_CHAR *s_acStrCecMachTalbe[]=
{

    "IDLE",             //CEC_MACH_IDLE,
    "PING_START",       //CEC_MACH_PING_START,
    "PING_WAIT",        //CEC_MACH_PING_WAIT_DONE,
    "RX_WAIT",          //CEC_MACH_RX_WAIT_DONE,
    "TX_START",         //CEC_MACH_TX_START,
    "WAIT_DONE",        //CEC_MACH_TX_WAIT_DONE,
};
#define STR_CEC_MACH(n)  ({(n>=HDMI_ARRAY_SIZE(s_acStrCecMachTalbe)) ? "ERROR" : s_acStrCecMachTalbe[n];})

const static  HI_CHAR *s_acStrCecAddrTalbe[]=
{
    "TV",                   //CEC_LOGICALADD_TV               = 0X00, 
    "RECORDDEV_1",          //CEC_LOGICALADD_RECORDDEV_1      = 0X01, 
    "RECORDDEV_2",          //CEC_LOGICALADD_RECORDDEV_2      = 0X02, 
    "TUNER_1",              //CEC_LOGICALADD_TUNER_1          = 0X03, 
    "PLAYDEV_1",            //CEC_LOGICALADD_PLAYDEV_1        = 0X04,
    "AUDIO_SYSTEM",         //CEC_LOGICALADD_AUDIOSYSTEM      = 0X05,
    "TUNER_2",              //CEC_LOGICALADD_TUNER_2          = 0X06,
    "TUNER_3",              //CEC_LOGICALADD_TUNER_3          = 0X07, 
    "PLAYDEV_2",            //CEC_LOGICALADD_PLAYDEV_2        = 0X08, 
    "RECORDDEV_3",          //CEC_LOGICALADD_RECORDDEV_3      = 0X09, 
    "TUNER_4",              //CEC_LOGICALADD_TUNER_4          = 0X0A, 
    "PLAYDEV_3",            //CEC_LOGICALADD_PLAYDEV_3        = 0X0B, 
    "RESERVED_1",           //CEC_LOGICALADD_RESERVED_1       = 0X0C, 
    "RESERVED_2",           //CEC_LOGICALADD_RESERVED_2       = 0X0D, 
    "SPECIAL_USE",          //CEC_LOGICALADD_SPECIALUSE       = 0X0E, 
    "BROAD_CAST",           //CEC_LOGICALADD_BROADCAST        = 0X0F, 
};

#define STR_CEC_ADDR(n)  ({(n>=HDMI_ARRAY_SIZE(s_acStrCecAddrTalbe)) ? "ERROR" : s_acStrCecAddrTalbe[n];})


typedef struct 
{
    HI_U8       *pu8Name;
    HI_U8       u8Code;
}DEBUG_CEC_CMD_S;

const static  DEBUG_CEC_CMD_S s_astUiOpCode[]=
{

    { "SELECT",               CEC_UICMD_SELECT                      },//= 0x00,
    { "UP",                   CEC_UICMD_UP                          },//= 0x01,
    { "DOWN",                 CEC_UICMD_DOWN                        },//= 0x02,
    { "LEFT",                 CEC_UICMD_LEFT                        },//= 0x03,
    { "RIGHT",                CEC_UICMD_RIGHT                       },//= 0x04,
    { "RIGHT_U",              CEC_UICMD_RIGHT_UP                    },//= 0x05,
    { "RIGHT_DOWN",           CEC_UICMD_RIGHT_DOWN                  },//= 0x06,
    { "LEFT_UP",              CEC_UICMD_LEFT_UP                     },//= 0x07,
    { "LEFT_DOWN",            CEC_UICMD_LEFT_DOWN                   },//= 0x08,
    { "ROOT_MENU",            CEC_UICMD_ROOT_MENU                   },//= 0x09,
    { "SETUP_MENU",           CEC_UICMD_SETUP_MENU                  },//= 0x0A,
    { "CONTENTS_MENU",        CEC_UICMD_CONTENTS_MENU               },//= 0x0B,
    { "FAVORITE_MENU",        CEC_UICMD_FAVORITE_MENU               },//= 0x0C,
    { "EXIT",                 CEC_UICMD_EXIT                        },//= 0x0D,

    { "NUM_0",                CEC_UICMD_NUM_0                       },//= 0x20,
    { "NUM_1",                CEC_UICMD_NUM_1                       },//= 0x21,
    { "NUM_2",                CEC_UICMD_NUM_2                       },//= 0x22,
    { "NUM_3",                CEC_UICMD_NUM_3                       },//= 0x23,
    { "NUM_4",                CEC_UICMD_NUM_4                       },//= 0x24,
    { "NUM_5",                CEC_UICMD_NUM_5                       },//= 0x25,
    { "NUM_6",                CEC_UICMD_NUM_6                       },//= 0x26,
    { "NUM_7",                CEC_UICMD_NUM_7                       },//= 0x27,
    { "NUM_8",                CEC_UICMD_NUM_8                       },//= 0x28,
    { "NUM_9",                CEC_UICMD_NUM_9                       },//= 0x29,
    { "DOT",                  CEC_UICMD_DOT                         },//= 0x2A,
    { "ENTER",                CEC_UICMD_ENTER                       },//= 0x2B,
    { "CLEAR",                CEC_UICMD_CLEAR                       },//= 0x2C,
    { "NEXT_FAVORITE",        CEC_UICMD_NEXT_FAVORITE               },//= 0x2F,

    { "CHANNEL_UP",           CEC_UICMD_CHANNEL_UP                  },//= 0x30,
    { "CHANNEL_DOWN",         CEC_UICMD_CHANNEL_DOWN                },//= 0x31,
    { "PREVIOUS_CHANNEL",     CEC_UICMD_PREVIOUS_CHANNEL            },//= 0x32,
    { "SOUND_SELECT",         CEC_UICMD_SOUND_SELECT                },//= 0x33,
    { "INPUT_SELECT",         CEC_UICMD_INPUT_SELECT                },//= 0x34,
    { "DISPLAY_INFORMATION",  CEC_UICMD_DISPLAY_INFORMATION         },//= 0x35,
    { "HELP",                 CEC_UICMD_HELP                        },//= 0x36,
    { "PAGE_UP",              CEC_UICMD_PAGE_UP                     },//= 0x37,
    { "PAGE_DOWN",            CEC_UICMD_PAGE_DOWN                   },//= 0x38,

    { "POWER",                CEC_UICMD_POWER                       },//= 0x40,
    { "VOLUME_UP",            CEC_UICMD_VOLUME_UP                   },//= 0x41,
    { "VOLUME_DOWN",          CEC_UICMD_VOLUME_DOWN                 },//= 0x42,
    { "MUTE",                 CEC_UICMD_MUTE                        },//= 0x43,
    { "PLAY",                 CEC_UICMD_PLAY                        },//= 0x44,
    { "STOP",                 CEC_UICMD_STOP                        },//= 0x45,
    { "PAUSE",                CEC_UICMD_PAUSE                       },//= 0x46,
    { "RECORD",               CEC_UICMD_RECORD                      },//= 0x47,
    { "REWIND",               CEC_UICMD_REWIND                      },//= 0x48,
    { "FAST_FORWARD",         CEC_UICMD_FAST_FORWARD                },//= 0x49,
    { "EJECT",                CEC_UICMD_EJECT                       },//= 0x4A,
    { "FORWARD",              CEC_UICMD_FORWARD                     },//= 0x4B,
    { "BACKWARD",             CEC_UICMD_BACKWARD                    },//= 0x4C,
    { "STOP_RECORD",          CEC_UICMD_STOP_RECORD                 },//= 0x4D,
    { "PAUSE_RECORD",         CEC_UICMD_PAUSE_RECORD                },//= 0x4E,

    { "ANGLE",                CEC_UICMD_ANGLE                       },//= 0x50,
    { "SUBPICTURE",           CEC_UICMD_SUBPICTURE                  },//= 0x51,
    { "VIDEO_ON_DEMAND",      CEC_UICMD_VIDEO_ON_DEMAND             },//= 0x52,
    { "PROGRAM_GUIDE",        CEC_UICMD_ELECTRONIC_PROGRAM_GUIDE    },//= 0x53,
    { "TIMER_PROGRAM",        CEC_UICMD_TIMER_PROGRAMMING           },//= 0x54,
    { "INITIAL_CFG",          CEC_UICMD_INITIAL_CONFIGURATION       },//= 0x55,

    { "PLAY_FUN",             CEC_UICMD_PLAY_FUNCTION               },//= 0x60,
    { "PAUSE_PLAY_FUN",       CEC_UICMD_PAUSE_PLAY_FUNCTION         },//= 0x61,
    { "RECORD_FUN",           CEC_UICMD_RECORD_FUNCTION             },//= 0x62,
    { "PAUSE_RECORD_FUN",     CEC_UICMD_PAUSE_RECORD_FUNCTION       },//= 0x63,
    { "STOP_FUN",             CEC_UICMD_STOP_FUNCTION               },//= 0x64,
    { "MUTE_FUN",             CEC_UICMD_MUTE_FUNCTION               },//= 0x65,
    { "RESTORE_VOLUME_FUN",   CEC_UICMD_RESTORE_VOLUME_FUNCTION     },//= 0x66,
    { "TUNE_FUN",             CEC_UICMD_TUNE_FUNCTION               },//= 0x67,
    { "SELECT_MEDIA_FUN",     CEC_UICMD_SELECT_MEDIA_FUNCTION       },//= 0x68,
    { "SELECT_AV_INPUT_FUN",  CEC_UICMD_SELECT_AV_INPUT_FUNCTION    },//= 0x69,
    { "SELECT_AUDIO_IN",      CEC_UICMD_SELECT_AUDIO_INPUT_FUNCTION },//= 0x6A,
    { "POWER_TOGGLE_FUN",     CEC_UICMD_POWER_TOGGLE_FUNCTION       },//= 0x6B,
    { "POWER_OFF_FUN",        CEC_UICMD_POWER_OFF_FUNCTION          },//= 0x6C,
    { "POWER_ON_FUN",         CEC_UICMD_POWER_ON_FUNCTION           },//= 0x6D,

    { "F1_BLUE",              CEC_UICMD_F1_BLUE                     },//= 0x71,
    { "F2_RED",               CEC_UICMD_F2_RED                      },//= 0x72,
    { "F3_GREEN",             CEC_UICMD_F3_GREEN                    },//= 0x73,
    { "F4_YELLOW",            CEC_UICMD_F4_YELLOW                   },//= 0x74,
    { "F5",                   CEC_UICMD_F5                          },//= 0x75,
    { "DATA",                 CEC_UICMD_DATA                        },//= 0x76

};

static  HI_U8 *DebugCecOpCodeStrGet(HI_U8 u8OpCode)
{
    HI_U32  i = 0;
    const DEBUG_CEC_CMD_S *pstCecCmd = &s_astUiOpCode[0]; 

    for (i=0;i<HDMI_ARRAY_SIZE(s_astUiOpCode) && pstCecCmd ;i++,pstCecCmd++)
    {
        if (pstCecCmd->u8Code == u8OpCode)
        {
            return pstCecCmd->pu8Name;
        }
    }
    
    return HI_NULL; 
}


const static  DEBUG_CEC_CMD_S s_astCecCmd[]= 
{
    { "Active Source",                  0x82 },
    { "Image View On",                  0x04 }, 
    { "Text View On",                   0x0D }, 
    { "Active Source",                  0x82 },
    { "Inactive Source",                0x9D },
    { "Request Active Source",          0x85 },

    { "Routing Change",                 0x80 }, 
    { "Routing Information",            0x81 }, 
    { "Set Stream Path",                0x86 }, 
    { "Standby",                        0x36 }, 

    { "Record Off",                     0x0B }, 
    { "Record On",                      0x09 }, 
    { "Record Status",                  0x0A }, 
    { "Record TV Screen",               0x0F }, 

    { "Clear Analogue Timer",           0x33 }, 
    { "Clear Digital Timer",            0x99 }, 
    { "Clear External Timer",           0xA1 }, 
    { "Set Analogue Timer",             0x34 }, 
    { "Set Digital Timer",              0x97 }, 

    { "Set External Timer",             0xA2 }, 
    { "Set Timer Program Title",        0x67 }, 
    { "Timer Cleared Status",           0x43 }, 
    { "Timer Status",                   0x35 }, 

    { "CEC Version",                    0x9E }, 
    { "Get CEC Version",                0x9F }, 
    { "Give Physical Address",          0x83 }, 
    { "Get Menu Language",              0x91 }, 

    //{ "Polling Message",        - 
    { "Report Physical Address",        0x84 }, 
    { "Set Menu Language",              0x32 }, 

    { "Deck Control",                   0x42 }, 
    { "Deck Status",                    0x1B }, 
    { "Give Deck Status",               0x1A }, 

    { "Play",                           0x41 }, 

    { "Give Tuner Device Status",       0x08 }, 
    { "Select Analogue Service",        0x92 }, 
    { "Select Digital Service",         0x93 }, 
    { "Tuner Device Status",            0x07 }, 
    { "Tuner Step Decrement",           0x06 }, 
    { "Tuner Step Increment",           0x05 }, 

    { "CEC Version",                    0x9E }, 
    { "Device Vendor ID",               0x87 }, 
    { "Get CEC Version",                0x9F }, 

    { "Give Device Vendor ID",          0x8C }, 
    { "Vendor Command",                 0x89 }, 
    { "Vendor Command With ID",         0xA0 }, 
    { "Vendor Remote Button Down",      0x8A }, 
    { "Vendor Remote Button Up",        0x8B }, 

    { "Set OSD String",                 0x64 }, 
    { "Give OSD Name",                  0x46 }, 
    { "Set OSD Name",                   0x47 }, 

    { "Menu Request",                   0x8D }, 
    { "Menu Status",                    0x8E }, 
    { "User Control Pressed",           0x44 }, 

    { "User Control Released",          0x45 }, 
    { "Give Device PowerStatus",        0x8F }, 
    { "Report Power Status",            0x90 }, 

    { "Feature Abort",                  0x00 }, 
    { "Abort Message",                  0xFF }, 

    { "Give Audio Status",              0x71 }, 
    { "Give System Audio Mode Status",  0x7D }, 
    { "Report Audio Status",            0x7A }, 
    { "Report Short Audio Descriptor",  0xA3 }, 
    { "Request Short Audio Descriptor", 0xA4 }, 
    { "Set System Audio Mode",          0x72 }, 

    { "System Audio Mode Request",      0x70 }, 
    { "System Audio Mode Status",       0x7E }, 
    { "User Control Pressed",           0x44 }, 

    { "User Control Released",          0x45 }, 
    { "Set Audio Rate",                 0x9A },

    { "Initiate ARC",                   0xC0 }, 
    { "Report ARC Initiated",           0xC1 }, 
    { "Report ARC Terminated",          0xC2 }, 
    { "Request ARC Initiation",         0xC3 }, 
    { "Request ARC Termination",        0xC4 }, 

    { "CDC Message",                    0xF8 }, 

};



static HI_U8 *DebugCecCmdStrGet(HI_U8 u8CmdCode)
{
    HI_U32  i = 0;
    const DEBUG_CEC_CMD_S *pstCecCmd = &s_astCecCmd[0]; 

    for (i=0;i<HDMI_ARRAY_SIZE(s_astCecCmd) && pstCecCmd ;i++,pstCecCmd++)
    {
        if (pstCecCmd->u8Code == u8CmdCode)
        {
            return pstCecCmd->pu8Name;
        }
    }
    
    return HI_NULL; 
}



HI_VOID HAL_HDMI_DebugMachPrint(HI_VOID *pvData)

{
    HI_U32                  i = 0,j = 0;
    HI_U32                  u32TmpPtr = 0;
    HI_U64                  u64TmpStamp = 0;
    HDMI_MACH_STAT_S        stStat ;
    HDMI_MACH_ELEM_STAT_S   stStatus ;
    HDMI_MACH_CTRL_S        stMachCtrl;
    HDMI_DEVICE_ID_E    enHdmiId = HDMI_DEVICE_ID0;

    DEBUG_NULL_CHK_NO_RET(pvData);

    enHdmiId = *(HDMI_DEVICE_ID_E *)pvData;
    HAL_HDMI_MachStatusGet(&stStat);

    HDMI_HAL_INFO("----------------- HDMI machine status --------------\n");
    HDMI_HAL_INFO("%-18s:%-14d|",  "bInits",     stStat.bInit);
    HDMI_HAL_INFO("%-18s:%u\n",    "u32Total",   stStat.u32Total);

    for (i = 0; stStat.bInit && (i < stStat.u32Total) ; i++)
    {
        HDMI_HAL_INFO("\n-------------------MACH[%u]-------------------------\n",i);
        HAL_HDMI_MachElemStatusGet(i,&stStatus);
        HAL_HDMI_MachCfgGet(i,&stMachCtrl);
        
        HDMI_HAL_INFO("%-18s:%-14s|",  "au8Name",         stStatus.stMachRun.au8Name);
        HDMI_HAL_INFO("%-18s:%llu\n",  "u64EnableTime",   stStatus.stMachRun.u64EnableTime);

        HDMI_HAL_INFO("%-18s:%-14d|",  "bEnable",         stStatus.stMachRun.bEnable);
        HDMI_HAL_INFO("%-18s:%llu\n",  "u64DisableTime",  stStatus.stMachRun.u64DisableTime);
        
        HDMI_HAL_INFO("%-18s:%-14d|",  "bValidId",        stStatus.stMachRun.bValidId);
        HDMI_HAL_INFO("%-18s:%llu\n",  "u64LastTime",     stStatus.stMachRun.u64LastTime);

        HDMI_HAL_INFO("%-18s:%-14u|",  "u32RunCnt",       stStatus.stMachRun.u32RunCnt);
        HDMI_HAL_INFO("%-18s:%llu\n",  "u64Interval",     stStatus.stMachCtrl.u64Interval);

        u32TmpPtr=stStatus.stMachRun.u32StampPtr;
        for (j=0;j<HDMI_MACH_MAX_STAMPE_NUM-1;j++)
        {
            u64TmpStamp = stStatus.stMachRun.au64Timestamp[u32TmpPtr];   
            u32TmpPtr = (u32TmpPtr+1) % HDMI_MACH_MAX_STAMPE_NUM;
            HDMI_HAL_INFO("%-12s[%u]:%llu (%llu ms)\n","au64Timestamp",j,stStatus.stMachRun.au64Timestamp[u32TmpPtr],stStatus.stMachRun.au64Timestamp[u32TmpPtr]-u64TmpStamp);
        }
        
    }

    return ;
}


HI_VOID HAL_HDMI_DebugHdcp14Print(HI_VOID *pvData)
{
    HI_U32              i = 0;
    HDCP14_STAUTS_S     *pstStauts = HI_NULL;
    HDCP14_RUN_STAT_S   stRunStat;
    HDMI_DEVICE_ID_E    enHdmiId = HDMI_DEVICE_ID0;

    DEBUG_NULL_CHK_NO_RET(pvData);
    enHdmiId = *(HDMI_DEVICE_ID_E *)pvData;

    pstStauts = HDMI_KMALLOC(sizeof(HDCP14_STAUTS_S));
    DEBUG_NULL_CHK_NO_RET(pstStauts);
    
    HAL_HDMI_Hdcp14StatusGet(enHdmiId, pstStauts);
    HAL_HDMI_Hdcp14RunStatusGet(enHdmiId, &stRunStat);

    HDMI_HAL_INFO("----------------- HDMI hdcp14 run status --------------\n");

    HDMI_HAL_INFO("%-18s:%-20d|",    "bEnalbe",         stRunStat.bEnable);
    HDMI_HAL_INFO("%-18s:%-20u\n",   "enMachStatus",    stRunStat.enMachStatus);

    HDMI_HAL_INFO("%-18s:%-20d|",     "u32AuthenCnt",   stRunStat.u32AuthenCnt);
    HDMI_HAL_INFO("%-18s:%-20llu\n", "MachStartStamp",  stRunStat.u64MachStartStamp);
    
    HDMI_HAL_INFO("%-18s:%-20d|",    "u32AuthenCnt",    stRunStat.u32AuthenCnt);
    HDMI_HAL_INFO("%-18s:%-20llu\n", "u64ReAuthWaitMs", stRunStat.u64ReAuthWaitMs);

    HDMI_HAL_INFO("%-18s:%-20u|",    "u32RiErrCnt",     stRunStat.u32RiErrCnt);
    HDMI_HAL_INFO("%-18s:%-20llu\n", "PIIIRiInterval",  stRunStat.u64PIIIRiInterval);
    
    HDMI_HAL_INFO("----------------- hdcp14 Real Hw status ----------------\n");
    
    HDMI_HAL_INFO("%-18s:%-20s|",   "bHdcp14Mode",      STR_BOOL(pstStauts->stRealHw.bHdcp14Mode));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "bAvmute",          STR_BOOL(pstStauts->stRealHw.bAvmute));
    
    HDMI_HAL_INFO("%-18s:%-20s|",   "bEncryption",      STR_BOOL(pstStauts->stRealHw.bEncryption));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "bRepeaterEnalbe",  STR_BOOL(pstStauts->stRealHw.bRepeaterEnalbe));

    HDMI_HAL_INFO("%-18s:%-20s\n",  "bShaEnable",       STR_BOOL(pstStauts->stRealHw.bShaEnable));
    
    HDMI_HAL_INFO("----------------- hdcp14 PART I status ----------------\n");
    HDMI_HAL_INFO("%-18s:%-20s|",   "enLoadKey",        STR_PROCESS(pstStauts->stPartI.enLoadKey));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "enCrcCheck",       STR_PROCESS(pstStauts->stPartI.enCrcCheck));

    HDMI_HAL_INFO("%-18s:%-20s|",   "enBksvRead",       STR_PROCESS(pstStauts->stPartI.enBksvRead));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "enBCapsRead",      STR_PROCESS(pstStauts->stPartI.enBCapsRead));
    
    HDMI_HAL_INFO("%-18s:%-20s|",   "AnGenerateSent",   STR_PROCESS(pstStauts->stPartI.enAnGenerateSent));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "enAksvSent",       STR_PROCESS(pstStauts->stPartI.enAksvSent));

    HDMI_HAL_INFO("%-18s:%-20s|",   "enAR0BR0Cmp",      STR_PROCESS(pstStauts->stPartI.enAR0BR0Cmp));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "enEncrypEnable",   STR_PROCESS(pstStauts->stPartI.enEncrypEnable));

    HDMI_HAL_INFO("%-18s:%-20s|",   "enAR0BR0Cmp",      STR_PROCESS(pstStauts->stPartI.enAR0BR0Cmp));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "Repeater",         STR_BOOL(pstStauts->stPartI.unBCaps.u8.bzRepeater));


    HDMI_HAL_INFO("%-18s:",   "Aksv [H]");
    for (i=0;i<sizeof(pstStauts->stPartI.au8Aksv);i++)
    {
        HDMI_HAL_INFO("%02x ", pstStauts->stPartI.au8Aksv[i]);
    }
    HDMI_HAL_INFO("     |");
    
    HDMI_HAL_INFO("%-18s:",   "AR0 [H]");
    for (i=0;i<sizeof(pstStauts->stPartI.au8AR0);i++)
    {
        HDMI_HAL_INFO("%02x ", pstStauts->stPartI.au8AR0[i]);
    }
    HDMI_HAL_INFO("\n");

    HDMI_HAL_INFO("%-18s:",   "Bksv [H]");
    for (i=0;i<sizeof(pstStauts->stPartI.au8Bksv);i++)
    {
        HDMI_HAL_INFO("%02x ", pstStauts->stPartI.au8Bksv[i]);
    }
    HDMI_HAL_INFO("     |");
    
    HDMI_HAL_INFO("%-18s:",   "BR0 [H]");
    for (i=0;i<sizeof(pstStauts->stPartI.au8BR0);i++)
    {
        HDMI_HAL_INFO("%02x ", pstStauts->stPartI.au8BR0[i]);
    }
    HDMI_HAL_INFO("\n");

    HDMI_HAL_INFO("%-18s:",   "An   [H]");
    for (i=0;i<sizeof(pstStauts->stPartI.au8An);i++)
    {
        HDMI_HAL_INFO("%02x ", pstStauts->stPartI.au8An[i]);
    }
    HDMI_HAL_INFO("\n");

    HDMI_HAL_INFO("----------------- hdcp14 PART II status ---------------\n");
    
    HDMI_HAL_INFO("%-18s:%-20s|",   "enBlistReady",      STR_PROCESS(pstStauts->stPartII.enBlistReady));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "enBksvListChk",     STR_PROCESS(pstStauts->stPartII.enBksvListChk));

    HDMI_HAL_INFO("%-18s:%-20s|",   "enHashDone",        STR_PROCESS(pstStauts->stPartII.enHashDone));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "enViReadCmp",       STR_PROCESS(pstStauts->stPartII.enViReadCmp));

    HDMI_HAL_INFO("%-18s:%-20s|",   "BC_KsvFifoReady",   STR_BOOL(pstStauts->stPartII.unBcaps.u8.bzKsvFifoReady));
    HDMI_HAL_INFO("%-18s:%-20u\n",  "BS_DeviceCnt",      pstStauts->stPartII.unBstatus.u16.bzDeviceCnt);

    HDMI_HAL_INFO("%-18s:%-20s|",   "BS_MaxDevExceed",   STR_BOOL(pstStauts->stPartII.unBstatus.u16.bzMaxDevsExceeded));
    HDMI_HAL_INFO("%-18s:%-20u\n",  "BS_Depth",          pstStauts->stPartII.unBstatus.u16.bzDepth);
    
    HDMI_HAL_INFO("%-18s:%-20s|",   "BS_MaxCasExceed",   STR_BOOL(pstStauts->stPartII.unBstatus.u16.bzMaxCasCadeExceeded));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "BS_HdmiMode",       STR_BOOL(pstStauts->stPartII.unBstatus.u16.bzHdmiMode));

    HDMI_HAL_INFO("%-18s:",   "au8AVi  [H]");
    for (i=0;i<sizeof(pstStauts->stPartII.au8AVi);i++)
    {
        HDMI_HAL_INFO("%02x ", pstStauts->stPartII.au8AVi[i]);
    }
    HDMI_HAL_INFO("\n");

    HDMI_HAL_INFO("%-18s:",   "au8BVi  [H]");
    for (i=0;i<sizeof(pstStauts->stPartII.au8BVi);i++)
    {
        HDMI_HAL_INFO("%02x ", pstStauts->stPartII.au8BVi[i]);
    }
    HDMI_HAL_INFO("\n");

    HDMI_HAL_INFO("%-18s:",   "au8BksvList[H]");
    if (pstStauts->stPartII.pu8BksvListData)
    {
        for (i=0;i<pstStauts->stPartII.u32BksvListSize;i++)
        {
            HDMI_HAL_INFO("%02x ", pstStauts->stPartII.pu8BksvListData[i]);
        }    
    }
    HDMI_HAL_INFO("\n");

    HDMI_HAL_INFO("----------------- hdcp14 PART III status --------------\n");

    for (i=0;i<HDCP14_MAX_III_RECORD_NUM;i++)
    {
        HDMI_HAL_INFO("%-18s:%-20u|",       "EncrypFrameCnt", pstStauts->stPartIII[i].u32EncrypFrameCnt);
        HDMI_HAL_INFO("%-18s:%02x %02x\n",  "u8ARi[H]",       pstStauts->stPartIII[i].u8ARi[0],pstStauts->stPartIII[i].u8ARi[1]);

        HDMI_HAL_INFO("%-18s:%-20llu|",     "u64TimeStamp",   pstStauts->stPartIII[i].u64TimeStamp);
        HDMI_HAL_INFO("%-18s:%02x %02x\n",  "u8BRi[H]",      pstStauts->stPartIII[i].u8BRi[0],pstStauts->stPartIII[i].u8BRi[1]);         

        HDMI_HAL_INFO("\n");
    }
    
    
    if (pstStauts)
    {
        HDMI_KFREE(pstStauts);
        pstStauts = HI_NULL;
    }
}

HI_VOID HAL_HDMI_DebugDdcPrint(HI_VOID *pvData)


{
    HI_S32              i = 0,j = 0;
    HI_U8               *pu8Data = HI_NULL;
    DDC_RUN_S           stDdcRun;
    DDC_RECORD_ELEM_S   stElem;
    HDMI_DEVICE_ID_E    enHdmiId = HDMI_DEVICE_ID0;

    DEBUG_NULL_CHK_NO_RET(pvData);

    enHdmiId = *(HDMI_DEVICE_ID_E *)pvData;
    
    HDMI_HAL_INFO("----------------------------- HDMI ddc status -----------------------------\n");

    HAL_HDMI_DdcStatusGet(enHdmiId,&stDdcRun);
    HDMI_HAL_INFO("%-18s:%-20s|",       "bInit",     STR_BOOL(stDdcRun.bInit));
    HDMI_HAL_INFO("%-18s:%-20u\n",      "TotalNum",  stDdcRun.u32TotalNum);
    
    for (i=1;i<=DDC_MAX_RECORD_NUM && i<stDdcRun.u32TotalNum;i++)
    {
        HAL_HDMI_DdcElemGet(enHdmiId,i,&stElem);

        HDMI_HAL_INFO("\n-------------------------ddc issue NUM[%d] ---------------------------\n",i);
        HDMI_HAL_INFO("[ECode]|AcceM|SclL |SdaL |IssuM|IsOut|NoAck|InPrg|HpdNo|ClrFail\n");
        HDMI_HAL_INFO("0x%-05x|%-5s|%-5s|%-5s|%-5s|%-5s|%-5s|%-5s|%-5s|%-5s\n",
                                    stElem.unErr.u32,
                                    STR_BOOL(stElem.unErr.u32.bzAccessWaitTimeOut),
                                    STR_BOOL(stElem.unErr.u32.bzBusLowScl),
                                    STR_BOOL(stElem.unErr.u32.bzBusLowSda),
                                    STR_BOOL(stElem.unErr.u32.bzErrIsseuMode),
                                    STR_BOOL(stElem.unErr.u32.bzIssueTimeOut),
                                    STR_BOOL(stElem.unErr.u32.bzBusNoAck),
                                    STR_BOOL(stElem.unErr.u32.bzInProgTimeOut),
                                    STR_BOOL(stElem.unErr.u32.bzHpdNoExist),
                                    STR_BOOL(stElem.unErr.u32.bzClrErrFail));

        HDMI_HAL_INFO("[FuncM]|MastT|Offse|segmt|speed|Csize|Rsize|T_Len|IssMode\n");
        HDMI_HAL_INFO("%-7s|%-5s|%-5u|%-5u|%-5u|%-5d|%-5d|%-5llu|%s\n",
                                    STR_DDC_FUN(stElem.stCfg.enFuncType),
                                    STR_DDC_MASTER(stElem.stCfg.enMasterMode),
                                    stElem.stCfg.u8Offset,
                                    stElem.stCfg.u8Segment,
                                    stElem.stCfg.u32Speed,
                                    stElem.stCfg.s32DataSize,
                                    stElem.s32DataSize,
                                    stElem.u64TimeLen,
                                    STR_DDC_MODE(stElem.stCfg.enIssueMode));

        HDMI_HAL_INFO("%-18s:",      "IssueData");

        for (pu8Data=stElem.pu8Data,j=0; pu8Data && (stElem.s32DataSize>=0) && (j<stElem.s32DataSize);pu8Data++,j++)
        {
            if (j && (j%16==0))
            {
                HDMI_HAL_INFO("\n%-18s","");                
            }
            if (j && (j%4==0))
            {
                HDMI_HAL_INFO(" ");                
            }
            HDMI_HAL_INFO("%02x ", *pu8Data);
        }
        HDMI_HAL_INFO("\n");

    }
    
    return ;
}


HI_VOID HAL_HDMI_DebugCecPrint(HI_VOID *pvData)
{

    HI_U8               j = 0;
    HI_U32              i = 0;
    CEC_ELEM_IDX_S      stIdx;
    CEC_STATUS_S        stStatus;
    CEC_MSG_ELEM_S      stElem;
    HDMI_DEVICE_ID_E    enHdmiId;
    DEBUG_DATA_CEC_S    *pstCecData = (DEBUG_DATA_CEC_S *)pvData;

    DEBUG_NULL_CHK_NO_RET(pstCecData);

    enHdmiId = pstCecData->u32HdmiId;
    HAL_HDMI_CecStatGet(enHdmiId, &stStatus);

    for (i=0;(i<stStatus.u32TxMsgCnt) && (i<CEC_MAX_MSG_NUM);i++)
    {
        
        stIdx.u32Idx = i;
        stIdx.enType = pstCecData->bCecTx ? CEC_MSG_TYPE_SENT : CEC_MSG_TYPE_RECV ;
        
        HAL_HDMI_CecRecordMsgGet(enHdmiId, &stIdx, &stElem);
    
        HDMI_HAL_INFO("\n------------------------- cec tx msg NUM[%03u] ---------------------------\n",i);
        HDMI_HAL_INFO("%-18s:%-20s|",  "bCecEnable",   STR_BOOL(stElem.bCecEnable));
        HDMI_HAL_INFO("%-18s:%-20s\n",  "enProcess",   STR_PROCESS(stElem.enProcess));

        HDMI_HAL_INFO("%-18s:%-20lls|","enDestAddr",   STR_CEC_ADDR(stElem.stMsg.enDestAddr));
        HDMI_HAL_INFO("%-18s:%-20llu\n","u64TimeStamp", stElem.u64TimeStamp);

        
        HDMI_HAL_INFO("%-18s:%-20lls|","enSrcAddr",    STR_CEC_ADDR(stElem.stMsg.enSrcAddr));        
        HDMI_HAL_INFO("%-18s:%-20s\n",  "u8Opcode",    DebugCecCmdStrGet(stElem.stMsg.u8Opcode));

        HDMI_HAL_INFO("%-18s:%-20lls\n","enUIOpcode",  DebugCecOpCodeStrGet(stElem.stMsg.stOperand.enUIOpcode));
        HDMI_HAL_INFO("%-18s:","Opcode_Data");
        for(j=0;j<stElem.stMsg.stOperand.stRawData.u8Length && j<CEC_MAX_RAW_SIZE;j++)
        {
            HDMI_HAL_INFO("%02x ",    stElem.stMsg.stOperand.stRawData.u8Data[j]);
        }
        HDMI_HAL_INFO("\n");
    }

    HDMI_HAL_INFO("\n------------------------- HDMI cec stauts ----------------------------\n");

    HDMI_HAL_INFO("%-18s:%-20s|",   "bEnalbe",              STR_BOOL(stStatus.stDrvInfo.bCecEnable));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "bInit",                STR_BOOL(stStatus.bInit));

    HDMI_HAL_INFO("%-18s:%-20s|",   "enMachStatus",         STR_CEC_MACH(stStatus.enMachStatus));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "enPingProcess",        STR_PROCESS(stStatus.enPingProcess));

    HDMI_HAL_INFO("%-18s:0x%02x %-15s|",   "enLogicAddr",   stStatus.stDrvInfo.u8LogicalAddr,STR_CEC_ADDR(stStatus.stDrvInfo.u8LogicalAddr));
    HDMI_HAL_INFO("%-18s:%x.%x.%x.%x\n",  "enPhyAddr ",     stStatus.stDrvInfo.au8PhysicalAddr[0],stStatus.stDrvInfo.au8PhysicalAddr[1],
                                                            stStatus.stDrvInfo.au8PhysicalAddr[2],stStatus.stDrvInfo.au8PhysicalAddr[3]);

    HDMI_HAL_INFO("%-18s:%-20u|",   "u32MachId",            stStatus.u32MachId);
    //HDMI_HAL_INFO("%-18s:%-20u\n",  "u32MachInterval",      stStatus.stAttr.u32MachInterval);

    HDMI_HAL_INFO("\n");
    HDMI_HAL_INFO("%-18s:%-20u|",   "u32TxMsgCnt",          stStatus.u32TxMsgCnt);
    HDMI_HAL_INFO("%-18s:%-20u\n",  "u32RxMsgCnt",          stStatus.u32RxMsgCnt);

    HDMI_HAL_INFO("%-18s:%-20u|",   "u32TxRecordWtPtr",     stStatus.u32TxRecordWtPtr);
    HDMI_HAL_INFO("%-18s:%-20u\n",  "u32RxRecordWtPtr",     stStatus.u32RxRecordWtPtr);

    HDMI_HAL_INFO("%-18s:%-20u|",   "u32TxPoolWtPtr",       stStatus.u32TxPoolWtPtr);
    HDMI_HAL_INFO("%-18s:%-20u\n",  "u32RxPoolWtPtr",       stStatus.u32RxPoolWtPtr);

    HDMI_HAL_INFO("%-18s:%-20u|",   "u32TxPoolRdPtr",       stStatus.u32TxPoolRdPtr);
    HDMI_HAL_INFO("%-18s:%-20u\n",  "u32RxPoolRdPtr",       stStatus.u32RxPoolRdPtr);
    
    HDMI_HAL_INFO("%-18s:%-20u|",   "u32TxPoolRdAbleCnt",   stStatus.u32TxPoolRdAbleCnt);   
    HDMI_HAL_INFO("%-18s:%-20u\n",  "u32RxPoolRdAbleCnt",   stStatus.u32RxPoolRdAbleCnt); 
    return ;
}


HI_VOID HAL_HDMI_DebugScdcPrint(HI_VOID *pvData)
{
    HI_U8           i = 0;
    HI_U32          u32TmpPtr = 0;
    HI_U32          u32TmpTotalErr = 0;
    HI_U64          u64TmpTime;
    HI_BOOL         bSinkSupportScdc = HI_FALSE;
    SCDC_STATUS_S   stStatus;
    SCDC_ATTR_S     stScdcAttr;
    HDMI_DEVICE_ID_E    enHdmiId = HDMI_DEVICE_ID0;

    DEBUG_NULL_CHK_NO_RET(pvData);

    enHdmiId = *(HDMI_DEVICE_ID_E *)pvData;

    HDMI_MEMSET(&stStatus,0,sizeof(SCDC_STATUS_S));
    HDMI_MEMSET(&stScdcAttr,0,sizeof(SCDC_ATTR_S));

    HAL_HDMI_ScdcSupportGet(enHdmiId,&bSinkSupportScdc);
    HAL_HDMI_ScdcAllStatusGet(enHdmiId, &stStatus);
    HAL_HDMI_ScdcAttrGet(enHdmiId,&stScdcAttr);

    u32TmpTotalErr = stStatus.stErrCnt.u32OeDisableFailCnt + stStatus.stErrCnt.u32OeEnableFailCnt + stStatus.stErrCnt.u32SrambleFailCnt;
    u32TmpPtr = (u32TmpTotalErr <= SCDC_MAX_ERROR_RECORD_NUM) ? 0 : stStatus.u32ErrRecordPtr;
    u64TmpTime = stStatus.astErrRecord[u32TmpPtr].u64TimeStamp;
    for ( i=0;
          u32TmpTotalErr && (i<SCDC_MAX_ERROR_RECORD_NUM) && (i<u32TmpTotalErr);
          i++,u32TmpPtr=(u32TmpPtr+1)%SCDC_MAX_ERROR_RECORD_NUM )
    {            
        HDMI_HAL_INFO("\n------------------------- scdc scramble err[%u]-------------------------\n",i); 
        HDMI_HAL_INFO("TimeLenMs |ErrorType |SrcScramb |SinkSrcamb|Ratio40x  |IntervalMs|timeoutMs\n");

        HDMI_HAL_INFO("%-10llu|%-10s|%-10s|%-10s|%-10s|%-10u|%-10u\n", 
                        (stStatus.astErrRecord[u32TmpPtr].u64TimeStamp - u64TmpTime),
                        STR_SCDC_ERR(stStatus.astErrRecord[u32TmpPtr].enErrType),
                        STR_BOOL((stStatus.astErrRecord[u32TmpPtr].stErrAttr.bSrcScramble)),
                        STR_BOOL(stStatus.astErrRecord[u32TmpPtr].stErrAttr.bSinkScramble),
                        STR_BOOL(stStatus.astErrRecord[u32TmpPtr].stErrAttr.bTmdsClkRatio40x),
                        stStatus.astErrRecord[u32TmpPtr].stErrAttr.u32ScrambleInterval,
                        stStatus.astErrRecord[u32TmpPtr].stErrAttr.u32ScrambleTimeout);
        u64TmpTime = stStatus.astErrRecord[u32TmpPtr].u64TimeStamp;
    }

    for ( i=0,u32TmpPtr=stStatus.u32RegUpdatePtr;
          i<SCDC_MAX_UPDATE_RECORD_NUM;
          i++,u32TmpPtr=(u32TmpPtr+1)%SCDC_MAX_UPDATE_RECORD_NUM )
    {
        HDMI_HAL_INFO("\n------------------------- scdc update NUM[%u]----------------------------\n",i);
        HDMI_HAL_INFO("scrab|StaUp|CedUp|RrTes|ClkLK|Ch0LK|Ch1LK|Ch2LK|ChSum|Ch0Err|Ch1Err|Ch2Err\n");
        HDMI_HAL_INFO("%-5s|%-5s|%-5s|%-5s|%-5s|%-5s|%-5s|%-5s|%-5u|%-6u|%-6u|%-6u\n",
                        STR_BOOL(stStatus.astRegUpdate[u32TmpPtr].u8ScrambleStatus.u8.bzScreamblingStatus),
                        STR_BOOL(stStatus.astRegUpdate[u32TmpPtr].u8Update0.u8.bzStatusUpdate),
                        STR_BOOL(stStatus.astRegUpdate[u32TmpPtr].u8Update0.u8.bzCedUpdate),
                        STR_BOOL(stStatus.astRegUpdate[u32TmpPtr].u8Update0.u8.bzRrTest),
                        STR_BOOL(stStatus.astRegUpdate[u32TmpPtr].u8StatusFlag0.u8.bzColckDetected),
                        STR_BOOL(stStatus.astRegUpdate[u32TmpPtr].u8StatusFlag0.u8.bzCh0Locked),
                        STR_BOOL(stStatus.astRegUpdate[u32TmpPtr].u8StatusFlag0.u8.bzCh1Locked),
                        STR_BOOL(stStatus.astRegUpdate[u32TmpPtr].u8StatusFlag0.u8.bzCh2Locked),
                        stStatus.astRegUpdate[u32TmpPtr].u8ErrChkSum,
                        stStatus.astRegUpdate[u32TmpPtr].u16Ch0Err.u16Word,
                        stStatus.astRegUpdate[u32TmpPtr].u16Ch1Err.u16Word,
                        stStatus.astRegUpdate[u32TmpPtr].u16Ch2Err.u16Word);

    }

    HDMI_HAL_INFO("\n------------------------- HDMI scdc attr   -----------------------------\n");

    HDMI_HAL_INFO("%-18s:%-20s|",   "bInit",                STR_BOOL(stStatus.bInit)); 
    HDMI_HAL_INFO("%-18s:%-20s\n",  "bMachEnable",          STR_BOOL(stStatus.bMachEnable)); 
    
    HDMI_HAL_INFO("%-18s:%-20s|",   "bSinkSupportScdc",     STR_BOOL(bSinkSupportScdc));   
    HDMI_HAL_INFO("%-18s:%-20u\n",  "u32MachId",            stStatus.u32MachId);

    HDMI_HAL_INFO("%-18s:%-20u|",   "enHdmiId",             stStatus.enHdmiId); 
    HDMI_HAL_INFO("%-18s:%-20llu\n","u64MachInterval",      stStatus.u64MachInterval);
    
    HDMI_HAL_INFO("------------------------- scdc scramble stauts -------------------------\n");  

    HDMI_HAL_INFO("%-18s:%-20u|",   "ScrambleInterval",     stScdcAttr.u32ScrambleInterval);
    HDMI_HAL_INFO("%-18s:%-20u\n",  "ScrambleTimeout",      stScdcAttr.u32ScrambleTimeout); 

    HDMI_HAL_INFO("%-18s:%-20s|",   "bSrcScramble",         STR_BOOL(stScdcAttr.bSrcScramble));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "bSinkScramble",        STR_BOOL(stScdcAttr.bSinkScramble));

    HDMI_HAL_INFO("%-18s:%-20s|",   "bSinkReadQuest",       STR_BOOL(stScdcAttr.bSinkReadQuest));
    HDMI_HAL_INFO("%-18s:%-20s\n",  "bTmdsClkRatio40x",     STR_BOOL(stScdcAttr.bTmdsClkRatio40x));    

    HDMI_HAL_INFO("%-18s:%-20u|",   "u32SinkUnReadCnt",     stStatus.stErrCnt.u32SinkUnReadCnt);
    HDMI_HAL_INFO("%-18s:%-20u\n",  "OeDisableFailCnt",     stStatus.stErrCnt.u32OeDisableFailCnt);
    
    HDMI_HAL_INFO("%-18s:%-20u|",   "u32SrambleFailCnt",    stStatus.stErrCnt.u32SrambleFailCnt);
    HDMI_HAL_INFO("%-18s:%-20u\n",  "u32OeEnableFailCnt",   stStatus.stErrCnt.u32OeEnableFailCnt);
    
    HDMI_HAL_INFO("%-18s:%-20u|",   "u8SrcVersion",         stStatus.stRegStatic.u8SrcVersion);
    HDMI_HAL_INFO("%-18s:%-20u\n",  "u8SinkVersion",        stStatus.stRegStatic.u8SinkVersion);

    HDMI_HAL_INFO("\n");

    HDMI_HAL_INFO("%-18s:%-20s|",   "bzTestReadRequest",    STR_BOOL(stStatus.stRegStatic.u8RrTestCfg.u8.bzTestReadRequest));
    HDMI_HAL_INFO("%-18s:%-20u\n",  "ReadRequestDelay",     stStatus.stRegStatic.u8RrTestCfg.u8.bzTestReadRequestDelay);

    HDMI_HAL_INFO("%-18s:%-20s|",   "bzRrEnable",           STR_BOOL(stStatus.stRegStatic.u8Config0.u8.bzRrEnable));
    HDMI_HAL_INFO("%-18s:",  "au8IeeeOut(H)");
    for (i=0;i<sizeof(stStatus.stRegStatic.au8IeeeOut);i++)
    {
        HDMI_HAL_INFO("%02x ",   stStatus.stRegStatic.au8IeeeOut[i]);
    }
    HDMI_HAL_INFO("\n");

    HDMI_HAL_INFO("%-18s:",  "au8DeviceId(H)");
    for (i=0;i<sizeof(stStatus.stRegStatic.au8DeviceId);i++)
    {
        HDMI_HAL_INFO("%02x ",   stStatus.stRegStatic.au8DeviceId[i]);
    }
    HDMI_HAL_INFO("\n");

    return ;
}



