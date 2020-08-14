
#ifndef __VPSS_DEBUG_H__
#define __VPSS_DEBUG_H__
#include "vpss_common.h"
#include "hi_drv_vpss.h"
#include "vpss_osal.h"

#define DEF_DBG_CMD_YUV    "saveyuv"
#define DEF_DBG_CMD_STREAM "savestream"
#define DEF_DBG_CMD_DBG    "printinfo"
#define DEF_DBG_CMD_NONE   "none"
#define DEF_DBG_CMD_UHD_HIGH "high"
#define DEF_DBG_CMD_UHD_MIDDLE "middle"
#define DEF_DBG_CMD_UHD_LOW "low"
#define DEF_DBG_CMD_UHD_USR "adaptive"
#define DEF_DBG_CMD_SET_UV  "uvinvert"
#define DEF_DBG_CMD_CMP_ON     "cmpon"
#define DEF_DBG_CMD_CMP_OFF    "cmpoff"
#define DEF_DBG_CMD_SET_I2P "i2p"
#define DEF_DBG_CMD_SETBYPASS  "setbypass"
#define DEF_DBG_CMD_SETRATE    "setrate"

#define DEF_DBG_SRC    "src"
#define DEF_DBG_PORT_0 "port0"
#define DEF_DBG_PORT_1 "port1"
#define DEF_DBG_PORT_2 "port2"

#define DEF_DBG_SET_ON "on"
#define DEF_DBG_SET_OFF "off"

#define DEF_DBG_SRC_ID    0xffffffff
#define DEF_DBG_PORT0_ID  0x1
#define DEF_DBG_PORT1_ID  0x2
#define DEF_DBG_PORT2_ID  0x3

#define DEF_SDK_VERSIO_LOG "2015041700"
typedef enum hiVPSS_DEBUG_E{
    DBG_W_YUV = 0,
    DBG_W_STREAM ,
    DBG_INFO_FRM ,
    DBG_INFO_NONE ,
    DBG_INFO_ASP,
	DBG_SET_UHD,
	DBG_SET_UHD_LOW,
	DBG_SET_UHD_MID,
	DBG_SET_UHD_HIGH,
	DBG_SET_UHD_USR,
	DBG_SET_UV_INVERT,
	DBG_SET_CMP_ON,
	DBG_SET_CMP_OFF,
	DBG_SET_I2P,
	DBG_SET_FRMRATE,
    DBG_BUTT
}VPSS_DEBUG_E;

typedef struct hiVPSS_DBG_YUV_S{
    VPSS_HANDLE hPort;
    HI_CHAR chFile[DEF_FILE_NAMELENGTH];
}VPSS_DBG_YUV_S;

typedef union{
    VPSS_HANDLE hYuvPart;
    VPSS_HANDLE hDbgPart;
}DBG_CMD_PARA_U;

typedef struct hiVPSS_DBGCMD_S{
    VPSS_DEBUG_E enDbgType;

    VPSS_HANDLE hDbgPart;
	VPSS_HANDLE hDbgValue;
    //DBG_CMD_PARA_U unPara;
}VPSS_DBG_CMD_S;

typedef union{
    // Define the struct bits
    struct
    {
        unsigned int    imginfo              : 1   ; 
        unsigned int    dei                  : 1   ;  
        unsigned int    writeyuv             : 4   ;  
        unsigned int    writestream          : 1   ;      
		unsigned int    vpssbypass           : 1   ;  
        unsigned int    frameEn              : 1  ;        
        unsigned int    framerate            : 8  ;
        unsigned int    reserve_2            : 1   ; 
        unsigned int    reserve_3            : 14  ;
        
    } bits;
    // Define an unsigned member
    unsigned int    u32;

}DBG_INST_INFO_U;
typedef union{
    // Define the struct bits
    struct
    {
        unsigned int    frameinfo            : 1   ; 
        unsigned int    asp                  : 1   ;  
        unsigned int    writeyuv             : 4   ;  
        unsigned int    writestream          : 1   ;  
        unsigned int    uv_invert            : 1   ; 
        unsigned int    cmp                  : 2   ;  //defalt 0, ON 01, OFF 02, 03 reserved
		unsigned int    reserve_2            : 1   ;
        unsigned int    reserve_3            : 21  ;
        
    } bits;
    // Define an unsigned member
    unsigned int    u32;

}DBG_PORT_INFO_U;

typedef struct hiVPSS_DBG_PORT_S{
    DBG_PORT_INFO_U unInfo;
	HI_U32 u32LastH;
	HI_U32 u32LastM;
	HI_U32 u32LastS;
}VPSS_DBG_PORT_S;

typedef struct hiVPSS_DBG_INST_S{
    DBG_INST_INFO_U unInfo;
	HI_U32 u32LastH;
	HI_U32 u32LastM;
	HI_U32 u32LastS;
	HI_U32 u32UhdSetW;
	HI_U32 u32UhdSetH;
}VPSS_DBG_INST_S;

typedef struct hiVPSS_DBG_S{
    VPSS_DBG_INST_S stInstDbg;
    
    VPSS_DBG_PORT_S stPortDbg[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
}VPSS_DBG_S;

HI_S32 VPSS_DBG_DbgInit(VPSS_DBG_S *pstDbg);

HI_S32 VPSS_DBG_DbgDeInit(VPSS_DBG_S *pstDbg);

HI_S32 VPSS_DBG_SendDbgCmd(VPSS_DBG_S *pstDbg,VPSS_DBG_CMD_S *pstCmd);

HI_S32 VPSS_DBG_ReplyDbgCmd(VPSS_DBG_S *pstDbg,VPSS_DEBUG_E enCmd,HI_VOID* para1,HI_VOID* para2);

#endif