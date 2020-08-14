#include "drv_reg_proc.h"

#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <mach/hardware.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "drv_hdmi.h"



//#include "mpi_priv_hdmi.h"
#include "hi_drv_hdmi.h"

#include "hi_unf_hdmi.h"

#include "si_audio.h"
#include "si_hdmitx.h"
#include "si_infofrm.h"
#include "si_txapidefs.h"
#include "si_defstx.h"
#include "si_hdcpdefs.h"
#include "si_eedefs.h"
#include "si_de.h"
#include "si_edid.h"
#include "si_txvidp.h"
#include "si_timer.h"
#include "si_eeprom.h"
#include "si_cec.h"
#include "si_phy.h"

#include "hi_unf_disp.h"
#include "si_mpi_hdmi.h"
#include "drv_disp_ext.h"
#include "hi_kernel_adapt.h"
#include "drv_cipher_ext.h"
#include "hi_drv_sys.h"

#include "drv_global.h"

#include "drv_reg_proc.h"



extern HI_U32 g_VirHDMI_PHY_BASE_ADDR;

#if defined (CEC_SUPPORT)
HI_S32 DRV_HDMI_CEC_ReadByte(HI_U8 ucAddr, HI_U8 *pData)
{
	volatile HI_U32  *pu32VirAddr = NULL;
	
       pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (ucAddr + 0x200)*4);
	if (0 == pData)	
	    return HI_FAILURE;    

      *pData = (*pu32VirAddr) & 0xff;
    
      //HI_INFO_HDMI("come to read CEC address:0x%x <--0x%x, data:0x%x\n", (HI_U32)pu32VirAddr, ucAddr, *pData);

      return HI_SUCCESS;
}

HI_U8 DRV_HDMI_CEC_WriteByte(HI_U8 ucAddr, HI_U8 u8Data)
{
	volatile HI_U32  *pu32VirAddr = NULL;
    
       pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (ucAddr + 0x200)*4);
	pu32VirAddr[0] = ((HI_U32)u8Data)&0xff;

    //HI_INFO_HDMI("come to write CEC address:0x%x <--0x%x, data:0x%x\n", (HI_U32)pu32VirAddr, ucAddr, u8Data);

    return HI_SUCCESS;
}
#endif


HI_S32 DRV_HDMI_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value)
{
    HI_U32 VirAddr;
    volatile HI_U32     *pu32VirAddr = HI_NULL;

    VirAddr = (HI_U32)IO_ADDRESS(u32RegAddr);
    COM_INFO("WriteRegister u32RegAddr:0x%x, VirAddr:0x%x\n", u32RegAddr, VirAddr);

    pu32VirAddr = (volatile HI_U32 *)(VirAddr);
    pu32VirAddr[0] = (HI_U32)u32Value;

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    HI_U32 VirAddr;
    volatile HI_U32     *pu32VirAddr = HI_NULL;

    VirAddr = (HI_U32)IO_ADDRESS(u32RegAddr);
    pu32VirAddr = (volatile HI_U32 *)(VirAddr);
    *pu32Value = *pu32VirAddr;

    return HI_SUCCESS;
}


HI_U8 DRV_ReadByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr)
{
    HI_U8 Data = 0;
	volatile HI_U32 	*pu32VirAddr = HI_NULL;

    if(SlaveAddr==TX_SLV0)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x000+RegAddr)*4);
        Data = (*pu32VirAddr) & 0xff;
    }
    else if(SlaveAddr==TX_SLV1)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x100+RegAddr)*4);
        Data = (*pu32VirAddr) & 0xff;
    }
#if defined (CEC_SUPPORT)
    else if(SlaveAddr==HDMI_DEV_CEC)
    {
        DRV_HDMI_CEC_ReadByte(RegAddr, &Data);
    }

    if (SlaveAddr == 0x72 && RegAddr == 0x70 && Data == 0)
    {
        ;
    }
    else
    {
    //    HI_INFO_HDMI("R%x_%#x<-%02x.\n", SlaveAddr, RegAddr, Data);
    }
#endif
    return Data;
}

//-------------------------------------------------------------------
void DRV_WriteByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U8 Data)
{
	volatile HI_U32 	*pu32VirAddr = HI_NULL;
    if(SlaveAddr==TX_SLV0)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x00+RegAddr)*4);
        pu32VirAddr[0] = ((HI_U32)Data)&0xff;
    }
    else if(SlaveAddr==TX_SLV1)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x100+RegAddr)*4);
        pu32VirAddr[0] = ((HI_U32)Data)&0xff;
    }
#if defined (CEC_SUPPORT)
    else if(SlaveAddr==HDMI_DEV_CEC)
    {
        DRV_HDMI_CEC_WriteByte(RegAddr, Data);
    }
#endif
    //udelay(1);
    //HI_INFO_HDMI("W%x_%#x->%02x.\n", SlaveAddr, RegAddr, Data);
}

//--------------------------------------------------------------------
HI_U16 DRV_ReadWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr)
{
    HI_U16 Data = 0xffff;
	volatile HI_U32 	*pu32VirAddr = HI_NULL;

    if(SlaveAddr==TX_SLV0)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x000+RegAddr)*4);
        Data = (HI_U16)((pu32VirAddr[0]&0xff) | ((pu32VirAddr[1]&0xff) << 8));
    }
    else if(SlaveAddr==TX_SLV1)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x100+RegAddr)*4);
        Data = (HI_U16)((pu32VirAddr[0]&0xff) | ((pu32VirAddr[1]&0xff) << 8));
    }

    return Data;
}

//-------------------------------------------------------------------
HI_U8 DRV_ReadByte_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr)
{
    HI_U8 Data = 0xff;
	volatile HI_U32 	*pu32VirAddr = HI_NULL;

    if(SlaveAddr==TX_SLV0)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x000+RegAddr)*4);
        Data = (HI_U8 )(pu32VirAddr[0]&0xff);
    }
    else if(SlaveAddr==TX_SLV1)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x100+RegAddr)*4);
        Data = (HI_U8 )(pu32VirAddr[0]&0xff);
    }

    return Data;
}


//-------------------------------------------------------------------
HI_U16 DRV_ReadWord_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr)
{
    HI_U16 Data = 0xffff;
	volatile HI_U32 	*pu32VirAddr = HI_NULL;

    if(SlaveAddr==TX_SLV0)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x000+RegAddr)*4);
        Data = (HI_U16)((pu32VirAddr[0]&0xff) | ((pu32VirAddr[1]&0xff) << 8));
    }
    else if(SlaveAddr==TX_SLV1)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x100+RegAddr)*4);
        Data = (HI_U16)((pu32VirAddr[0]&0xff) | ((pu32VirAddr[1]&0xff) << 8));
    }

    return Data;
}

//-------------------------------------------------------------------
void DRV_WriteWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U16 Data)
{
	volatile HI_U32 	*pu32VirAddr = HI_NULL;

    if(SlaveAddr==TX_SLV0)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x00+RegAddr)*4);
        pu32VirAddr[0] = Data&0xff;
        pu32VirAddr[1] = (Data&0xff00) >> 8;
    }
    else if(SlaveAddr==TX_SLV1)
    {
        pu32VirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x100+RegAddr)*4);
        pu32VirAddr[0] = Data&0xff;
        pu32VirAddr[1] = (Data&0xff00) >> 8;
    }
}

//------------------------------------------------------------------------------------
HI_S32 DRV_BlockRead_8BAS(I2CShortCommandType * I2CCommand, HI_U8 * Data)
{
    volatile HI_U32 *pulVirAddr = HI_NULL;
    HI_U32 ulIndex = 0;

    if (Data == 0 || I2CCommand == 0)
    {
        return HI_FAILURE;
    }

    if ((I2CCommand->NBytes > 16) || 
        ((I2CCommand->NBytes + I2CCommand->RegAddrL) > 255)
       )
    {
        COM_ERR("CMD INVALID Reg or Size.\n");
        return HI_FAILURE;
    }

    if(I2CCommand->SlaveAddr == TX_SLV0)
    {
        pulVirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x00+I2CCommand->RegAddrL)*4);
    }
    else if(I2CCommand->SlaveAddr == TX_SLV1)
    {
        pulVirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x100+I2CCommand->RegAddrL)*4);
    }
    else
    {
        COM_ERR("SLVADDR ERR:%u.\n", I2CCommand->SlaveAddr);
        return HI_FAILURE;
    }

    //HI_INFO_HDMI("BRead%x_%x * %d:", I2CCommand->SlaveAddr, I2CCommand->RegAddrL, I2CCommand->NBytes);

    for (ulIndex = 0; ulIndex < I2CCommand->NBytes; ulIndex ++)
    {
        Data[ulIndex] = (HI_U8)(pulVirAddr[ulIndex]&0xff);
    }
    return HI_SUCCESS;
}

//------------------------------------------------------------------------------------
HI_U8 DRV_BlockWrite_8BAS( I2CShortCommandType * I2CCommand, HI_U8 * Data )
{
    volatile HI_U32 *pulVirAddr = HI_NULL;
    HI_U32 ulIndex = 0;

    if (Data == 0 || I2CCommand == 0)
    {
        return HI_FAILURE;
    }

    if ((I2CCommand->NBytes > 16) || 
        ((I2CCommand->NBytes + I2CCommand->RegAddrL) > 255)
       )
    {
        COM_ERR("CMD INVALID Reg or Size.\n");
        return HI_FAILURE;
    }

    if(I2CCommand->SlaveAddr == TX_SLV0)
    {
        pulVirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x00+I2CCommand->RegAddrL)*4);
    }
    else if(I2CCommand->SlaveAddr == TX_SLV1)
    {
        pulVirAddr = (volatile HI_U32 *)(g_VirHDMI_PHY_BASE_ADDR + (0x100+I2CCommand->RegAddrL)*4);
    }
    else
    {
        COM_ERR("SLVADDR error:%u\n", I2CCommand->SlaveAddr);
        return HI_FAILURE;
    }

    for (ulIndex = 0; ulIndex < I2CCommand->NBytes; ulIndex ++)
    {
        pulVirAddr[ulIndex] = Data[ulIndex];
    }

    return HI_SUCCESS;
}

