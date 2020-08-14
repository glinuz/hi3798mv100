#ifndef __DRV_IR_IOCTL_H__
#define __DRV_IR_IOCTL_H__

#include "hi_type.h"

/********************************* Ioctl definitions ************/
/* 1:check keyup */
#define CMD_IR_ENABLE_KEYUP _IOW(HI_ID_IR, 0x1, uint)

/* 1:check repkey, 0:hardware behave */
#define CMD_IR_ENABLE_REPKEY _IOW(HI_ID_IR, 0x2, uint)
#define CMD_IR_SET_REPKEY_TIMEOUT _IOW(HI_ID_IR, 0x3, uint)

/* 1:enable ir, 0:disable ir */
#define CMD_IR_SET_ENABLE _IOW(HI_ID_IR, 0x4, uint)
#define CMD_IR_RESET _IO(HI_ID_IR, 0x5)
#define CMD_IR_SET_BLOCKTIME _IOW(HI_ID_IR, 0x6, uint)
#define CMD_IR_SET_FORMAT _IOW(HI_ID_IR, 0x7, uint)
#define CMD_IR_SET_BUF _IOW(HI_ID_IR, 0x8, uint)

/* raw symbol fetch(1) or key fetch(0) */
#define CMD_IR_SET_FETCH_METHOD _IOW(HI_ID_IR, 0x9, uint)

/* enable or disalbe a protocol */
#define CMD_IR_SET_PROT_ENABLE _IOW(HI_ID_IR, 0xa, uint)
#define CMD_IR_SET_PROT_DISABLE _IOW(HI_ID_IR, 0xb, uint)
#define CMD_IR_GET_PROT_ENABLED _IOWR(HI_ID_IR, 0xc, uint)

#endif /* __DRV_IR_IOCTL_H__ */
