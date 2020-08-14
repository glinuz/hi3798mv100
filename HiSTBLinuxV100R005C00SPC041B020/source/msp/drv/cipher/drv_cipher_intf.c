/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher_intf.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#include <linux/proc_fs.h>
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/seq_file.h>
#include <linux/version.h>
//#include <linux/himedia.h>

#include <asm/atomic.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/traps.h>

#include <linux/miscdevice.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_common.h"
#include "hi_kernel_adapt.h"
#include "hi_drv_dev.h"
#include "hi_drv_mem.h"
#include "hal_cipher.h"
#include "drv_cipher.h"
#include "drv_cipher_ioctl.h"
#include "drv_cipher_ext.h"
#include "drv_advca_ext.h"
#include "hi_drv_mmz.h"
#include "hi_drv_module.h"
#include "hi_drv_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static UMAP_DEVICE_S    g_CipherDevice;

HI_VOID *g_u32KladRegBase;
HI_VOID *g_u32CipherRegBase ;
HI_VOID *g_u32CipherMmuRegBase ;
HI_VOID *g_u32RsaRegBase;
HI_VOID *g_u32HashRegBase;
HI_VOID *g_u32RngRegBase;
HI_VOID *g_u32HdmiRegBase;

extern CIPHER_EXPORT_FUNC_S s_CipherExportFuncs;
#define CIPHER_NAME "HI_CIPHER"
extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipID);
extern HI_VOID DRV_CIPHER_UserCommCallBack(HI_U32 arg);


HI_S32 CIPHER_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *argp)
{
    HI_S32 ret = HI_SUCCESS;

    switch(cmd)
    {
	    case CMD_CIPHER_CREATEHANDLE:
		{
	        CIPHER_HANDLE_S *pstCIHandle = (CIPHER_HANDLE_S *)argp;
			ret = HI_DRV_CIPHER_CreateHandle(pstCIHandle, (HI_VOID*)file);
	        break;
	    }
	
	    case CMD_CIPHER_DESTROYHANDLE:
	    {
	        HI_HANDLE hCipherChn = *(HI_HANDLE *)argp;
			ret = HI_DRV_CIPHER_DestroyHandle(hCipherChn);
	        break;
	    }
	    case CMD_CIPHER_CONFIGHANDLE:
	    {
	        CIPHER_Config_CTRL stCIConfig = *(CIPHER_Config_CTRL *)argp;

	        HI_U32 softChnId = HI_HANDLE_GET_CHNID(stCIConfig.CIHandle);  
	        ret = HI_DRV_CIPHER_ConfigChn(softChnId, &stCIConfig.CIpstCtrl);
	        break;
	    }
	    case CMD_CIPHER_ENCRYPT:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;
			ret = HI_DRV_CIPHER_Encrypt(pstCIData);
			break;
	    }
	    case CMD_CIPHER_DECRYPT:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;
			ret = HI_DRV_CIPHER_Decrypt(pstCIData);
			break;
	    }
	    case CMD_CIPHER_ENCRYPTMULTI:
	    {
	        CIPHER_PKG_S *pstPkg = (CIPHER_PKG_S *)argp;
			ret = HI_DRV_CIPHER_EncryptMulti(pstPkg);
			break;
	    }
	    case CMD_CIPHER_DECRYPTMULTI:
	    {
	        CIPHER_PKG_S *pstPkg = (CIPHER_PKG_S *)argp;
			ret = HI_DRV_CIPHER_DecryptMulti(pstPkg);
	        break;
	    }
	    case CMD_CIPHER_GETRANDOMNUMBER:
	    {
	        CIPHER_RNG_S *pstRNG  = (CIPHER_RNG_S *)argp;
			ret = HI_DRV_CIPHER_GetRandomNumber(pstRNG);
	        break;
	    }
	    case CMD_CIPHER_GETHANDLECONFIG:
	    {
	        CIPHER_Config_CTRL *pstCIData = (CIPHER_Config_CTRL *)argp;
	        ret = HI_DRV_CIPHER_GetHandleConfig(pstCIData);
	        break;
	    }
	    case CMD_CIPHER_CALCHASHINIT:
	    {
	        CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S*)argp;

            pstCipherHashData->enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;
	        ret = HI_DRV_CIPHER_CalcHashInit(pstCipherHashData);
	        break;
	    }
	    case CMD_CIPHER_CALCHASHUPDATE:
	    {
	        CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S*)argp;
	        ret = HI_DRV_CIPHER_CalcHashUpdate(pstCipherHashData);
	        break;
	    }
	    case CMD_CIPHER_CALCHASHFINAL:
	    {
	        CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S*)argp;
	        ret = HI_DRV_CIPHER_CalcHashFinal(pstCipherHashData);
	        break;
	    }
		case CMD_CIPHER_ENCRYPTHDCPKEY:
		{
            CIPHER_HDCP_KEY_TRANSFER_S *pstParam = (CIPHER_HDCP_KEY_TRANSFER_S *)argp;            
            ret = HI_DRV_CIPHER_EncryptHDCPKey(pstParam);
			break;
		}
	    case CMD_CIPHER_LOADHDCPKEY:
	    {
	        HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S *pstParam = (HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S *)argp;
	        ret = HI_DRV_CIPHER_LoadHdcpKey(pstParam);
	        break;
	    }
        case CMD_CIPHER_CBCMAC_AUTH:
        {
        	CIPHER_CBCMAC_DATA_S *pstParam = (CIPHER_CBCMAC_DATA_S *)argp;
			ret = HI_DRV_CIPHER_CbcMacAuth(pstParam);
			break;
		}
#ifdef CONFIG_RSA_HARDWARE_SUPPORT
        case CMD_CIPHER_CALCRSA:
        {
	        CIPHER_RSA_DATA_S *pCipherRsaData = (CIPHER_RSA_DATA_S*)argp;
	        ret = HI_DRV_CIPHER_CalcRsa(pCipherRsaData);
	        break;
	    } 
#endif
#ifdef CONFIG_COMPAT
#ifdef CONFIG_RSA_HARDWARE_SUPPORT
        case CMD_CIPHER_COMPAT_CALCRSA:
        {
	        CIPHER_COMPAT_RSA_DATA_S *pCipherCompatRsaData = (CIPHER_COMPAT_RSA_DATA_S*)argp;
            CIPHER_RSA_DATA_S stCipherRsaData;

            stCipherRsaData.pu8Input = U32_TO_POINT(pCipherCompatRsaData->u32InputVia);
            stCipherRsaData.pu8Output = U32_TO_POINT(pCipherCompatRsaData->u328OutputVia);
            stCipherRsaData.pu8K = U32_TO_POINT(pCipherCompatRsaData->u328KVia);
            stCipherRsaData.pu8N = U32_TO_POINT(pCipherCompatRsaData->u328NVia);
            stCipherRsaData.u16KLen = pCipherCompatRsaData->u16KLen;
            stCipherRsaData.u16NLen = pCipherCompatRsaData->u16NLen;
            stCipherRsaData.u32DataLen = pCipherCompatRsaData->u32DataLen;
            
	        ret = HI_DRV_CIPHER_CalcRsa(&stCipherRsaData);
	        break;
	    } 
#endif
	    case CMD_CIPHER_COMPAT_ENCRYPTMULTI:
	    {
	        CIPHER_COMPAT_PKG_S *pstPkg = (CIPHER_COMPAT_PKG_S *)argp;
            CIPHER_PKG_S stPkg;

            stPkg.CIHandle = pstPkg->CIHandle;
            stPkg.u32PkgNum = pstPkg->u32PkgNum;
            stPkg.pu8Pkg = U32_TO_POINT(pstPkg->u32PkgVia);
            
			ret = HI_DRV_CIPHER_EncryptMulti(&stPkg);
			break;
	    }
	    case CMD_CIPHER_COMPAT_DECRYPTMULTI:
	    {
	        CIPHER_COMPAT_PKG_S *pstPkg = (CIPHER_COMPAT_PKG_S *)argp;
            CIPHER_PKG_S stPkg;

            stPkg.CIHandle = pstPkg->CIHandle;
            stPkg.u32PkgNum = pstPkg->u32PkgNum;
            stPkg.pu8Pkg = U32_TO_POINT(pstPkg->u32PkgVia);
            
			ret = HI_DRV_CIPHER_DecryptMulti(&stPkg);
	        break;
	    }
#endif
	    default:
	    {
            HI_ERR_CIPHER("Unsupport cmd, MOD_ID=0x%02X, NR=0x%02x, SIZE=0x%02x!\n", _IOC_TYPE (cmd), _IOC_NR (cmd), _IOC_SIZE(cmd));
	        ret = HI_FAILURE;
	        break;
	    }
    }

    return ret;
}

static long DRV_CIPHER_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long ret;

    ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, CIPHER_Ioctl);

    return ret;
}

/** <* ref from linux/fs.h @by g00182102 */
static struct file_operations DRV_CIPHER_Fops=
{
    .owner            = THIS_MODULE,
    .open             = DRV_CIPHER_Open,
    .unlocked_ioctl   = DRV_CIPHER_Ioctl,
#ifdef CONFIG_COMPAT    
    .compat_ioctl     = DRV_CIPHER_Ioctl,
#endif  
    .release          = DRV_CIPHER_Release,
};

static HI_S32  cipher_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_DRV_CIPHER_Suspend();
    HI_PRINT("CIPHER suspend OK\n");
    return 0;
}

static HI_S32 cipher_pm_resume(PM_BASEDEV_S *pdev)
{
    HI_S32 ret = HI_SUCCESS;

    ret = HI_DRV_CIPHER_Resume();
    if(ret != HI_SUCCESS)
    {
        HI_FATAL_CIPHER("CIPHER resume failed!\n");
        return HI_FAILURE;
    }

    HI_PRINT("CIPHER resume OK\n");
    return HI_SUCCESS;
}

static PM_BASEOPS_S cipher_drvops = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = cipher_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = cipher_pm_resume,
};

HI_S32 CIPHER_DRV_ModInit(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;

    snprintf(g_CipherDevice.devfs_name, sizeof(UMAP_DEVNAME_CIPHER), UMAP_DEVNAME_CIPHER);
    g_CipherDevice.fops = &DRV_CIPHER_Fops;
    g_CipherDevice.minor = UMAP_MIN_MINOR_CIPHER;
    g_CipherDevice.owner  = THIS_MODULE;
    g_CipherDevice.drvops = &cipher_drvops;

    if (HI_DRV_DEV_Register(&g_CipherDevice) < 0)
    {
        HI_FATAL_CIPHER("register CIPHER failed.\n");
        return HI_FAILURE;
    }

    ret = HI_DRV_MODULE_Register(HI_ID_CIPHER, CIPHER_NAME, (HI_VOID*)&s_CipherExportFuncs);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_CIPHER("HI_DRV_MODULE_Register failed\n");
        return ret;
    }

    g_u32CipherRegBase = ioremap_nocache(CIPHER_CIPHER_REG_BASE_ADDR_PHY, 0x2000);
    if (g_u32CipherRegBase == HI_NULL)
    {
        HI_FATAL_CIPHER("ioremap_nocache Cipher Reg failed\n");
        return ret;
    }
#ifdef HI_SMMU_SUPPORT
    g_u32CipherMmuRegBase = ioremap_nocache(CIPHER_MMU_REG_BASE_ADDR_PHY, 0x1000);
    if (g_u32CipherMmuRegBase == HI_NULL)
    {
        HI_FATAL_CIPHER("ioremap_nocache Cipher mmu Reg failed\n");
        return ret;
    }
#endif
	g_u32KladRegBase = ioremap_nocache(CIPHER_KLAD_REG_BASE_ADDR_PHY, 0x1000);
    if (g_u32KladRegBase == HI_NULL)
    {
        HI_FATAL_CIPHER("ioremap_nocache klad Reg failed\n");
        return ret;
    }
    g_u32HashRegBase = ioremap_nocache(CIPHER_HASH_REG_BASE_ADDR_PHY, 0x1000);
    if (g_u32HashRegBase == HI_NULL)
    {
        HI_FATAL_CIPHER("ioremap_nocache hash Reg failed\n");
        return ret;
    }
    g_u32RngRegBase = ioremap_nocache(CIPHER_RNG_REG_BASE_ADDR_PHY, 0x1000);
    if (g_u32RngRegBase == HI_NULL)
    {
        HI_FATAL_CIPHER("ioremap_nocache rng Reg failed\n");
        return ret;
    }
    g_u32RsaRegBase = ioremap_nocache(CIPHER_RSA_REG_BASE_ADDR_PHY, 0x1000);
    if (g_u32RsaRegBase == HI_NULL)
    {
        HI_FATAL_CIPHER("ioremap_nocache rsa Reg failed\n");
        return ret;
    }
    g_u32HdmiRegBase = ioremap_nocache(CIPHER_HDMI_REG_BASE_ADDR_PHY, 0x1000);
    if (g_u32HdmiRegBase == HI_NULL)
    {
        HI_FATAL_CIPHER("ioremap_nocache hdmi Reg failed\n");
        return ret;
    }
    
    ret = DRV_CIPHER_Init();
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
	

#ifdef MODULE
    HI_PRINT("Load hi_cipher.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID CIPHER_DRV_ModExit(HI_VOID)
{


    HI_DRV_DEV_UnRegister(&g_CipherDevice);

    (HI_VOID)DRV_CIPHER_DeInit();

    iounmap(g_u32CipherRegBase);
	iounmap(g_u32KladRegBase);
#ifdef HI_SMMU_SUPPORT
    iounmap(g_u32CipherMmuRegBase);
#endif
    iounmap(g_u32HashRegBase);
    iounmap(g_u32RsaRegBase);
    iounmap(g_u32RngRegBase);
    iounmap(g_u32HdmiRegBase);

    HI_DRV_MODULE_UnRegister(HI_ID_CIPHER);

    return ;
}

#ifdef MODULE
module_init(CIPHER_DRV_ModInit);
module_exit(CIPHER_DRV_ModExit);
#endif

MODULE_AUTHOR("Hi3720 MPP GRP");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

