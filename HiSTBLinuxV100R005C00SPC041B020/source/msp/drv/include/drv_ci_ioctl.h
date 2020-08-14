#ifndef __DRV_CI_IOCTL_H__
#define __DRV_CI_IOCTL_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "hi_unf_ci.h"
#include "hi_drv_ci.h"
#include "hi_drv_struct.h"


typedef enum hiCI_PCCD_VOLT_E
{
    CI_PCCD_VOLT_5V,        /* 5V DC, default */
    CI_PCCD_VOLT_3V3,       /* 3V3 DC */
    CI_PCCD_VOLT_BUTT
}CI_PCCD_VOLT_E;

typedef enum hiCI_PCCD_SPEED_E
{
    CI_PCCD_SPEED_600NS,        /* 600ns, default */
    CI_PCCD_SPEED_250NS,        /* 250ns */
    CI_PCCD_SPEED_200NS,        /* 200ns */
    CI_PCCD_SPEED_150NS,        /* 150ns */
    CI_PCCD_SPEED_100NS,        /* 100ns */
    CI_PCCD_SPEED_BUTT
}CI_PCCD_SPEED_E;

typedef struct hiCI_PCCD_ATTR_S
{
    CI_PCCD_VOLT_E  enVolt;         /* PCMCIA card voltage, 5v or 3v3 DC */
    CI_PCCD_SPEED_E enSpeed;        /* PCMCIA card attribute read/write speed */
    HI_BOOL bIsCiplus;
    HI_U32  u32Ciprof;
} CI_PCCD_ATTR_S;

typedef struct hiCI_DEV_STATE_S
{
    HI_BOOL bCI0;
    HI_BOOL bCI1;
} CI_DEV_STATE_S;

typedef struct hiCI_CARD_PARAMETER_S
{
    HI_UNF_CI_PORT_E enCIPort;
    HI_UNF_CI_PCCD_E enCardId;
    HI_U32           u32IOBaseAddress;
    HI_U32           u32AttrBaseAddress;
} CI_CARD_PARAMETER_S;

typedef struct hiCI_PCCD_CTRLPOWER_S
{
    HI_UNF_CI_PORT_E           enCIPort;
    HI_UNF_CI_PCCD_E           enCardId;
    HI_UNF_CI_PCCD_CTRLPOWER_E enCtrlPower;
} CI_PCCD_CTRLPOWER_S;

typedef struct hiCI_PCCD_S
{
    HI_UNF_CI_PORT_E enCIPort;
    HI_UNF_CI_PCCD_E enCardId;
} CI_PCCD_S;

/* Added begin 2012-04-24 l00185424: support various CI device */
typedef struct hiCI_ATTR_S
{
    HI_UNF_CI_PORT_E enCIPort;
    HI_UNF_CI_ATTR_S stCIAttr;
} CI_ATTR_S;
/* Added end 2012-04-24 l00185424: support various CI device */

typedef struct hiCI_PCCD_READY_S
{
    HI_UNF_CI_PORT_E             enCIPort;
    HI_UNF_CI_PCCD_E             enCardId;
    HI_UNF_CI_PCCD_READY_E      *penCardReady;
} CI_PCCD_READY_S;

typedef struct hiCI_PCCD_DETECT_S
{
    HI_UNF_CI_PORT_E             enCIPort;
    HI_UNF_CI_PCCD_E             enCardId;
    HI_UNF_CI_PCCD_STATUS_E     *penCardStatus;
} CI_PCCD_DETECT_S;

typedef struct hiCI_PCCD_ACCESSMODE_S
{
    HI_UNF_CI_PORT_E            enCIPort;
    HI_UNF_CI_PCCD_E            enCardId;
    HI_UNF_CI_PCCD_ACCESSMODE_E enAccessMode;
} CI_PCCD_ACCESSMODE_S;

typedef struct hiCI_PCCD_STATUS_S
{
    HI_UNF_CI_PORT_E             enCIPort;
    HI_UNF_CI_PCCD_E             enCardId;
    HI_U8                       *pu8Value;
} CI_PCCD_STATUS_S;

typedef struct hiCI_PCCD_READ_S
{
    HI_UNF_CI_PORT_E     enCIPort;
    HI_UNF_CI_PCCD_E     enCardId;
    HI_U8               *pu8Buffer;
    HI_U32              *pu32ReadLen;
} CI_PCCD_READ_S;

typedef struct hiCI_PCCD_WRITE_S
{
    HI_UNF_CI_PORT_E     enCIPort;
    HI_UNF_CI_PCCD_E     enCardId;
    HI_U8               *pu8Buffer;
    HI_U32              *pu32WriteOKLen;
    HI_U32               u32WriteLen;    
} CI_PCCD_WRITE_S;

typedef struct hiCI_PCCD_BUFFERSIZE_S
{
    HI_UNF_CI_PORT_E     enCIPort;
    HI_UNF_CI_PCCD_E     enCardId;
    HI_U16              *pu16BufferSize;
} CI_PCCD_BUFFERSIZE_S;

typedef struct hiCI_PCCD_TSBYPASS_S
{
    HI_UNF_CI_PORT_E        enCIPort;
    HI_UNF_CI_PCCD_E        enCardId;
    HI_UNF_CI_PCCD_TSCTRL_E enCMD;
    HI_VOID             *   pParam;
} CI_PCCD_TSCTRL_S;

typedef struct hiCI_PCCD_CIS_S
{
    HI_UNF_CI_PORT_E enCIPort;
    HI_UNF_CI_PCCD_E enCardId;
    HI_U8           *pu8Buffer;
    HI_U32          *pu32CisLen;
} CI_PCCD_CIS_S;

typedef struct hiCI_PCCD_COR_S
{
    HI_UNF_CI_PORT_E enCIPort;
    HI_UNF_CI_PCCD_E enCardId;
    HI_U16            u16Addr;
    HI_U8           u8Data;
} CI_PCCD_COR_S;

#ifdef CONFIG_COMPAT 
typedef struct hiCI_COMPAT_PCCD_STATUS_S
{
    HI_UNF_CI_PORT_E             enCIPort;
    HI_UNF_CI_PCCD_E             enCardId;
    HI_U32                       ValueAddr;
} CI_COMPAT_PCCD_STATUS_S;

typedef struct hiCI_COMPAT_PCCD_READ_S
{
    HI_UNF_CI_PORT_E     enCIPort;
    HI_UNF_CI_PCCD_E     enCardId;
    HI_U32               BufferAddr;
    HI_U32               ReadLenAddr;
} CI_COMPAT_PCCD_READ_S;

typedef struct hiCI_COMPAT_PCCD_WRITE_S
{
    HI_UNF_CI_PORT_E     enCIPort;
    HI_UNF_CI_PCCD_E     enCardId;
    HI_U8                BufferAddr;
    HI_U32               WriteOKLenAddr;
    HI_U32               u32WriteLen;    
} CI_COMPAT_PCCD_WRITE_S;

typedef struct hiCI_COMPAT_PCCD_BUFFERSIZE_S
{
    HI_UNF_CI_PORT_E     enCIPort;
    HI_UNF_CI_PCCD_E     enCardId;
    HI_U32               BufferSizeAddr;
} CI_COMPAT_PCCD_BUFFERSIZE_S;

typedef struct hiCI_COMPAT_PCCD_TSBYPASS_S
{
    HI_UNF_CI_PORT_E        enCIPort;
    HI_UNF_CI_PCCD_E        enCardId;
    HI_UNF_CI_PCCD_TSCTRL_E enCMD;
    HI_U32                  ParamAddr;
} CI_COMPAT_PCCD_TSCTRL_S;

typedef struct hiCI_COMPAT_PCCD_CIS_S
{
    HI_UNF_CI_PORT_E enCIPort;
    HI_UNF_CI_PCCD_E enCardId;
    HI_U32           BufferAddr;
    HI_U32           CisLenAddr;
} CI_COMPAT_PCCD_CIS_S;

#endif

#define CMD_CI_OPEN                 _IOW(HI_ID_CI, 0x1, HI_UNF_CI_PORT_E)
#define CMD_CI_CLOSE                _IOW(HI_ID_CI, 0x2, HI_UNF_CI_PORT_E)

#define CMD_CI_PCCD_OPEN            _IOW(HI_ID_CI, 0x3, CI_PCCD_S)
#define CMD_CI_PCCD_CLOSE           _IOW(HI_ID_CI, 0x4, CI_PCCD_S)

#define CMD_CI_PCCD_CTRLPOWER       _IOW(HI_ID_CI, 0x5, CI_PCCD_CTRLPOWER_S)
#define CMD_CI_PCCD_RESET           _IOW(HI_ID_CI, 0x6, CI_PCCD_S)
#define CMD_CI_PCCD_READY           _IOWR(HI_ID_CI, 0x7, CI_PCCD_READY_S)
#define CMD_CI_PCCD_DETECT          _IOWR(HI_ID_CI, 0x8, CI_PCCD_DETECT_S)
#define CMD_CI_PCCD_SET_ACCESSMODE  _IOW(HI_ID_CI, 0x9, CI_PCCD_ACCESSMODE_S)
#define CMD_CI_PCCD_GET_STATUS      _IOWR(HI_ID_CI, 0xa, CI_PCCD_STATUS_S)
#define CMD_CI_PCCD_READ            _IOWR(HI_ID_CI, 0xb, CI_PCCD_READ_S)
#define CMD_CI_PCCD_WRITE           _IOWR(HI_ID_CI, 0xc, CI_PCCD_WRITE_S)
#define CMD_CI_PCCD_CHECKCIS        _IOW(HI_ID_CI, 0xd, CI_PCCD_S)
#define CMD_CI_PCCD_WRITECOR        _IOW(HI_ID_CI, 0xe, CI_PCCD_S)
#define CMD_CI_PCCD_IORESET         _IOW(HI_ID_CI, 0xf, CI_PCCD_S)
#define CMD_CI_PCCD_NEG_BUFFERSIZE  _IOWR(HI_ID_CI, 0x10, CI_PCCD_BUFFERSIZE_S)
#define CMD_CI_PCCD_TSCTRL          _IOWR(HI_ID_CI, 0x11, CI_PCCD_TSCTRL_S)

/* Added begin 2012-04-24 l00185424: support various CI device */
#define CMD_CI_SETATTR              _IOW(HI_ID_CI, 0x12, CI_ATTR_S)
#define CMD_CI_GETATTR              _IOWR(HI_ID_CI, 0x13, CI_ATTR_S)
/* Added end 2012-04-24 l00185424: support various CI device */

#define CMD_CI_PCCD_GETCIS          _IOWR(HI_ID_CI, 0x14, CI_PCCD_CIS_S)
#define CMD_CI_PCCD_WRITECOREX      _IOW(HI_ID_CI, 0x15, CI_PCCD_COR_S)

#ifdef CONFIG_COMPAT 
#define CMD_CI_COMPAT_PCCD_GET_STATUS      _IOWR(HI_ID_CI, 0x30, CI_COMPAT_PCCD_STATUS_S)
#define CMD_CI_COMPAT_PCCD_READ            _IOWR(HI_ID_CI, 0x31, CI_COMPAT_PCCD_READ_S)
#define CMD_CI_COMPAT_PCCD_WRITE           _IOWR(HI_ID_CI, 0x32, CI_COMPAT_PCCD_WRITE_S)
#define CMD_CI_COMPAT_PCCD_NEG_BUFFERSIZE  _IOWR(HI_ID_CI, 0x33, CI_COMPAT_PCCD_BUFFERSIZE_S)
#define CMD_CI_COMPAT_PCCD_TSCTRL          _IOWR(HI_ID_CI, 0x34, CI_COMPAT_PCCD_TSCTRL_S)
#define CMD_CI_COMPAT_PCCD_GETCIS          _IOWR(HI_ID_CI, 0x35, CI_COMPAT_PCCD_CIS_S)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __DRV_CI_IOCTL_H__*/

