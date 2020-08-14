/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_otp_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#ifndef __DRV_OTP_IOCTL_H__
#define __DRV_OTP_IOCTL_H__

/* add include here */
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_otp.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct hiOTP_ENTRY_S
{
    HI_U32 Addr;
    HI_U32 Value;
}OTP_ENTRY_S;

typedef struct hiOTP_CUSTOMER_KEY_S
{
    HI_U32 u32CustomerKey[4];
}OTP_CUSTOMER_KEY_S;

typedef struct hiOTP_CUSTOMER_KEY_TRANSFER_S
{
    HI_U32 u32KeyLen;
    OTP_CUSTOMER_KEY_S stkey;    
}OTP_CUSTOMER_KEY_TRANSTER_S;

typedef struct hiOTP_HDCP_ROOT_KEY_S
{
    HI_U8 u8Key[16];
}OTP_HDCP_ROOT_KEY_S;

typedef struct hiOTP_STB_ROOT_KEY_S
{
    HI_U8 u8Key[16];
}OTP_STB_ROOT_KEY_S;

typedef struct hiOTP_STB_PRIV_DATA_S
{
    HI_U32 u32Offset;
    HI_U8 u8Data;
}OTP_STB_PRIV_DATA_S;


#define CMD_OTP_READ              		_IOWR(IOC_TYPE_OTP, 0x1, OTP_ENTRY_S)
#define CMD_OTP_WRITE             		_IOW(IOC_TYPE_OTP, 	0x2, OTP_ENTRY_S)
#define CMD_OTP_WRITE_BYTE        		_IOW(IOC_TYPE_OTP, 	0x3, OTP_ENTRY_S)
#define CMD_OTP_SETPROTECT        		_IO(IOC_TYPE_OTP, 	0x4)
#define CMD_OTP_GETPROTECT        		_IOR(IOC_TYPE_OTP, 	0x5, HI_U32)
#define CMD_OTP_FUNCDISABLE       		_IOW(IOC_TYPE_OTP, 	0x6, HI_U32)
#define CMD_OTP_SETCUSTOMERKEY    		_IOW(IOC_TYPE_OTP, 	0x7, OTP_CUSTOMER_KEY_TRANSTER_S)
#define CMD_OTP_GETCUSTOMERKEY    		_IOWR(IOC_TYPE_OTP, 0x8, OTP_CUSTOMER_KEY_TRANSTER_S)
#define CMD_OTP_GETDDPLUSFLAG     		_IOR(IOC_TYPE_OTP, 	0x9, HI_BOOL)
#define CMD_OTP_GETDTSFLAG        		_IOR(IOC_TYPE_OTP, 	0xA, HI_BOOL)
#define CMD_OTP_SETSTBPRIVDATA    		_IOW(IOC_TYPE_OTP, 	0xB, OTP_STB_PRIV_DATA_S)
#define CMD_OTP_GETSTBPRIVDATA    		_IOWR(IOC_TYPE_OTP, 0xC, OTP_STB_PRIV_DATA_S)

#define CMD_OTP_WRITEHDCPROOTKEY 		_IOW(IOC_TYPE_OTP, 	0xD, OTP_HDCP_ROOT_KEY_S)
#define CMD_OTP_READHDCPROOTKEY 		_IOWR(IOC_TYPE_OTP, 0xE, OTP_HDCP_ROOT_KEY_S)
#define CMD_OTP_LOCKHDCPROOTKEY 		_IO(IOC_TYPE_OTP, 	0xF)
#define CMD_OTP_GETHDCPROOTKEYLOCKFLAG 	_IOR(IOC_TYPE_OTP, 	0x10, HI_BOOL)
#define CMD_OTP_GETDTSENFLAG 			_IOR(IOC_TYPE_OTP, 	0x11, HI_BOOL)
#define CMD_OTP_GETDOLBYENFLAG 		    _IOR(IOC_TYPE_OTP, 	0x12, HI_BOOL)

#define CMD_OTP_WRITESTBROOTKEY         _IOW(IOC_TYPE_OTP,  0x13, OTP_STB_ROOT_KEY_S)
#define CMD_OTP_READSTBROOTKEY          _IOWR(IOC_TYPE_OTP, 0x14, OTP_STB_ROOT_KEY_S)
#define CMD_OTP_LOCKSTBROOTKEY          _IO(IOC_TYPE_OTP,   0x15)
#define CMD_OTP_GETSTBROOTKEYLOCKFLAG   _IOR(IOC_TYPE_OTP,  0x16, HI_BOOL)

#define CMD_OTP_WRITE_BIT         		_IOW(IOC_TYPE_OTP, 0x17, OTP_ENTRY_S)
#define CMD_OTP_RESET             		_IO(IOC_TYPE_OTP, 0x18)


#ifdef __cplusplus
}
#endif
#endif /* __DRV_OTP_IOCTL_H__ */

