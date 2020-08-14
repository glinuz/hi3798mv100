/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_otp_intf.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include <linux/seq_file.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>

#include "hi_kernel_adapt.h"
#include "hi_drv_mem.h"
#include "drv_otp.h"
#include "drv_otp_ext.h"

#if defined(CHIP_TYPE_hi3716mv310)
#include "drv_dev_ext.h"
#else
#include "hi_drv_dev.h"
#endif

#include "drv_cipher_ext.h"
#include "hi_drv_module.h"
#include "drv_otp_common.h"
#include "drv_otp_ioctl.h"
#include "drv_otp_v200.h"

#include "hi_drv_proc.h"

CIPHER_EXPORT_FUNC_S *g_pCIPHERExportFunctionList = NULL;
extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);

static UMAP_DEVICE_S   g_stOtpUmapDev;
HI_DECLARE_MUTEX(g_OtpMutex);


HI_S32 OTP_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32   Ret = HI_SUCCESS;

    if (down_interruptible(&g_OtpMutex))
    {
        return -1;
    }

    switch (cmd)
    {
        case CMD_OTP_READ:
        {
            OTP_ENTRY_S *pOtpEntry = (OTP_ENTRY_S *)arg;

            if (HI_NULL == pOtpEntry)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            pOtpEntry->Value = DRV_OTP_Read(pOtpEntry->Addr);
            break;
        }
        case CMD_OTP_WRITE:
        {
            OTP_ENTRY_S *pOtpEntry = (OTP_ENTRY_S *)arg;

            if (HI_NULL == pOtpEntry)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = DRV_OTP_Write(pOtpEntry->Addr, pOtpEntry->Value);
            break;
        }
        case CMD_OTP_WRITE_BYTE:
        {
            OTP_ENTRY_S *pOtpEntry = (OTP_ENTRY_S *)arg;

            if (HI_NULL == pOtpEntry)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = DRV_OTP_Write_Byte(pOtpEntry->Addr, pOtpEntry->Value);
            break;
        }
        case CMD_OTP_SETCUSTOMERKEY:
        {
            OTP_CUSTOMER_KEY_TRANSTER_S *pstCustomerKeyTransfer = (OTP_CUSTOMER_KEY_TRANSTER_S *)arg;

            if (HI_NULL == pstCustomerKeyTransfer)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = DRV_OTP_Set_CustomerKey(&(pstCustomerKeyTransfer->stkey));
            break;
        }
        case CMD_OTP_GETCUSTOMERKEY:
        {
            OTP_CUSTOMER_KEY_TRANSTER_S *pstCustomerKeyTransfer = (OTP_CUSTOMER_KEY_TRANSTER_S *)arg;

            if (HI_NULL == pstCustomerKeyTransfer)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = DRV_OTP_Get_CustomerKey(&(pstCustomerKeyTransfer->stkey));
            break;
        }
        case CMD_OTP_GETDDPLUSFLAG:
        {
            HI_BOOL *pDDPLUSFlag = (HI_BOOL*)arg;

            if (HI_NULL == pDDPLUSFlag)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = DRV_OTP_Get_DDPLUS_Flag(pDDPLUSFlag);

            break;
        }
        case CMD_OTP_GETDTSFLAG:
        {
            HI_BOOL *pDTSFlag = (HI_BOOL*)arg;

            if (HI_NULL == pDTSFlag)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = DRV_OTP_Get_DTS_Flag(pDTSFlag);

            break;
        }
        case CMD_OTP_SETSTBPRIVDATA:
        {
            OTP_STB_PRIV_DATA_S *pStbPrivData = (OTP_STB_PRIV_DATA_S*)arg;

            if (HI_NULL == pStbPrivData)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = DRV_OTP_Set_StbPrivData(pStbPrivData);

            break;
        }
        case CMD_OTP_GETSTBPRIVDATA:
        {
            OTP_STB_PRIV_DATA_S *pStbPrivData = (OTP_STB_PRIV_DATA_S*)arg;

            if (HI_NULL == pStbPrivData)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = DRV_OTP_Get_StbPrivData(pStbPrivData);
            break;
        }
        case CMD_OTP_WRITE_BIT:
        {
            HI_U32 u32BitPos = 0;
            HI_U32 u32BitValue = 0;
            OTP_ENTRY_S *pOtpEntry = (OTP_ENTRY_S *)arg;

            if (HI_NULL == pOtpEntry)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            //The higher 16 bits are the position, the lower 16 bits are the value
            u32BitPos = (pOtpEntry->Value >> 16) & 0xffff;
            u32BitValue = pOtpEntry->Value & 0xffff;
            Ret = DRV_OTP_Write_Bit(pOtpEntry->Addr, u32BitPos, u32BitValue);
            break;
        }
        case CMD_OTP_RESET:
        {
            Ret = DRV_OTP_Reset();
            break;
        }
        case CMD_OTP_WRITEHDCPROOTKEY:
        {
            OTP_HDCP_ROOT_KEY_S stHdcpRootKey;

            if (HI_NULL == arg)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            memcpy(&stHdcpRootKey, arg, sizeof(OTP_HDCP_ROOT_KEY_S));
            Ret = OTP_V200_SetHdcpRootKey(stHdcpRootKey.u8Key);
            if ( HI_SUCCESS != Ret )
            {
                HI_ERR_OTP("OTP_V200_SetHdcpRootKey failed!\n");
                break;
            }

            Ret = DRV_OTP_Reset();
            Ret |= g_pCIPHERExportFunctionList->pfnCipherSoftReset();

            break;
        }
        case CMD_OTP_READHDCPROOTKEY:
        {
            OTP_HDCP_ROOT_KEY_S *pstHdcpRootKey = (OTP_HDCP_ROOT_KEY_S *)arg;

            if (HI_NULL == pstHdcpRootKey)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = OTP_V200_GetHdcpRootKey(pstHdcpRootKey->u8Key);

            break;
        }
        case CMD_OTP_LOCKHDCPROOTKEY:
        {
            Ret = OTP_V200_SetHdcpRootKeyLock();
            break;
        }
        case CMD_OTP_GETHDCPROOTKEYLOCKFLAG:
        {
            HI_BOOL *pbKeyLockFlag = (HI_BOOL *)arg;

            if (HI_NULL == pbKeyLockFlag)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = OTP_V200_GetHdcpRootKeyLock(pbKeyLockFlag);

            break;
        }
        case CMD_OTP_WRITESTBROOTKEY:
        {
            OTP_STB_ROOT_KEY_S stSTBRootKey;

            if (HI_NULL == arg)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            memcpy(&stSTBRootKey, arg, sizeof(OTP_STB_ROOT_KEY_S));
            Ret = OTP_V200_SetSTBRootKey(stSTBRootKey.u8Key);
            if ( HI_SUCCESS != Ret )
            {
                break;
            }

            Ret = DRV_OTP_Reset();
            Ret |= g_pCIPHERExportFunctionList->pfnCipherSoftReset();
            break;
        }
        case CMD_OTP_READSTBROOTKEY:
        {
            OTP_STB_ROOT_KEY_S *pstSTBRootKey = (OTP_STB_ROOT_KEY_S *)arg;

            if (HI_NULL == pstSTBRootKey)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = OTP_V200_GetSTBRootKey(pstSTBRootKey->u8Key);

            break;
        }
        case CMD_OTP_LOCKSTBROOTKEY:
        {
            Ret = OTP_V200_LockSTBRootKey();
            break;
        }
        case CMD_OTP_GETSTBROOTKEYLOCKFLAG:
        {
            HI_BOOL *pbSTBRootKeyLockFlag = (HI_BOOL *)arg;

            if (HI_NULL == pbSTBRootKeyLockFlag)
            {
                HI_ERR_OTP("invalid arg\n");
                up(&g_OtpMutex);
                return -1;
            }
            Ret = OTP_V200_GetSTBRootKeyLockFlag(pbSTBRootKeyLockFlag);

            break;
        }
        default:
        {
            Ret = -ENOTTY;
            break;
        }
    }

    up(&g_OtpMutex);
    return Ret;
}

static HI_S32 DRV_OTP_Open(struct inode *inode, struct file *filp)
{
    HI_S32 ret = HI_SUCCESS;

    if(down_interruptible(&g_OtpMutex))
    {
        return -1;
    }

    ret = HI_DRV_MODULE_GetFunction(HI_ID_CIPHER, (HI_VOID**)&g_pCIPHERExportFunctionList);
    if( (HI_SUCCESS != ret) || (NULL == g_pCIPHERExportFunctionList) )
    {
        HI_FATAL_OTP("Get cipher functions failed!\n");
        up(&g_OtpMutex);
        return HI_FAILURE;
    }

    up(&g_OtpMutex);
    return 0;
}

static HI_S32 DRV_OTP_Close(struct inode *inode, struct file *filp)
{
    if(down_interruptible(&g_OtpMutex))
    {
        return -1;
    }

    up(&g_OtpMutex);
    return 0;
}


static long DRV_OTP_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, OTP_Ioctl);

    return Ret;
}

static struct file_operations OTP_FOPS =
{
    .owner          = THIS_MODULE,
    .open           = DRV_OTP_Open,
    .unlocked_ioctl = DRV_OTP_Ioctl,
#ifdef CONFIG_COMPAT    
    .compat_ioctl   = DRV_OTP_Ioctl,
#endif    
    .release        = DRV_OTP_Close,
};

int OTP_PM_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    return HI_DRV_OTP_Suspend();
}

int OTP_PM_Resume(PM_BASEDEV_S *pdev)
{
    return HI_DRV_OTP_Resume();
}

static PM_BASEOPS_S  otp_drvops = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = OTP_PM_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = OTP_PM_Resume,
};


HI_S32 OTP_DRV_ModInit(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    snprintf(g_stOtpUmapDev.devfs_name, sizeof(UMAP_DEVNAME_OTP), UMAP_DEVNAME_OTP);
    g_stOtpUmapDev.minor = UMAP_MIN_MINOR_OTP;
    g_stOtpUmapDev.owner  = THIS_MODULE;
    g_stOtpUmapDev.drvops = &otp_drvops;
    g_stOtpUmapDev.fops = &OTP_FOPS;

    if (HI_DRV_DEV_Register(&g_stOtpUmapDev) < 0)
    {
        HI_FATAL_OTP("register otp failed.\n");
        return HI_FAILURE;
    }

    ret = DRV_OTP_Init();
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    (HI_VOID)DRV_OTP_DieID_Check();
#endif

#ifdef MODULE
    HI_PRINT("Load hi_otp.ko success.  \t(%s)\n", VERSION_STRING);
#endif

    return 0;
}

HI_VOID OTP_DRV_ModExit(HI_VOID)
{
    DRV_OTP_DeInit();

    HI_DRV_DEV_UnRegister(&g_stOtpUmapDev);
    return;
}

#ifdef MODULE
module_init(OTP_DRV_ModInit);
module_exit(OTP_DRV_ModExit);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");

