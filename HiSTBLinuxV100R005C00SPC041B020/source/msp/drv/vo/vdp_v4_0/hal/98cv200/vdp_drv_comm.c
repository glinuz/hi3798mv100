//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_drv_comm.cpp
// Author      : s173141
// Data        : 2012/10/17
// Version     : v1.0
// Abstract    : header of vdp define
//                                                                         
// Modification history                                                    
//----------------------------------------------------------------------  
// Version       Data(yyyy/mm/dd)      name                                  
// Description                                                              
//                                                                          
//
//
//
//
//**********************************************************************

#include "vdp_drv_comm.h"
#include "vdp_fpga_define.h"
VDP_COEF_ADDR_S gstVdpCoefBufAddr;
#if VDP_CBB_FPGA
#include "hi_drv_mmz.h"
#if FPGA_AUTORUN
#include "vdp_util.h"
#endif
#endif
//----------------------------------------------------------------------
// memory/address managerment
//----------------------------------------------------------------------
HI_S32 VDP_DRV_AllocateMem(HI_U32 u32Size,VDP_MMZ_BUFFER_S *stVdpMmzBuffer)
{
#if VDP_CBB_FPGA
	HI_S32 nRet;
	nRet = HI_DRV_MMZ_AllocAndMap("VDP_DDR_CFG", HI_NULL, u32Size, 0, (MMZ_BUFFER_S *)stVdpMmzBuffer);
	if (nRet != 0)
	{
		 VDP_PRINT("VDP_DDR_CFG  failed\n");
		 return HI_FAILURE;
	}
#else
    stVdpMmzBuffer->pu8StartVirAddr= 0x8000000;
    stVdpMmzBuffer->u32StartPhyAddr = 0x8000000;
#endif
	
    return HI_SUCCESS;
}


HI_S32 VDP_DRV_DeleteMem(VDP_MMZ_BUFFER_S *stVdpMmzBuffer)
{
#if VDP_CBB_FPGA
    HI_DRV_MMZ_UnmapAndRelease((MMZ_BUFFER_S *)stVdpMmzBuffer);
#endif
    return HI_SUCCESS;
}

HI_S32 VDP_DRV_IoRemap(HI_U32 u32Phy, HI_U32 u32Size,HI_U32 **pu32VirtualAddr)
{
	
#if VDP_CBB_FPGA
	*pu32VirtualAddr = ioremap_nocache(u32Phy, u32Size);
    if (HI_NULL == *pu32VirtualAddr)
    {
		return HI_FAILURE;
	}		
#endif

	return HI_SUCCESS;	
}

HI_S32 VDP_DRV_IoUnmap(HI_U32 **pu32VirtualAddr)
{
#if VDP_CBB_FPGA
	(HI_VOID)iounmap(*pu32VirtualAddr);
	*pu32VirtualAddr = HI_NULL;
#endif

	return HI_SUCCESS;	
}

HI_S32 VDP_DRV_CoefBufAddrDistribute(VDP_COEF_ADDR_S *stVdpCoefBufAddr)
{
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_IN_GAMMA   ]  = stVdpCoefBufAddr->stBufBaseAddr.pu8StartVirAddr + 0;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_OUT_GAMMA  ]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_IN_GAMMA   ] + COEF_SIZE_IN_GAMMA   ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_IN_DEGAMMA ]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_OUT_GAMMA  ] + COEF_SIZE_OUT_GAMMA  ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_OUT_DEGAMMA]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_IN_DEGAMMA ] + COEF_SIZE_IN_DEGAMMA ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_CVM        ]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_OUT_DEGAMMA] + COEF_SIZE_OUT_DEGAMMA;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_GDM_GAMMA  ]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_CVM        ] + COEF_SIZE_CVM        ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_GDM_DEGAMMA]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_GDM_GAMMA  ] + COEF_SIZE_GDM_GAMMA  ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZMEH]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_GDM_DEGAMMA] + COEF_SIZE_GDM_DEGAMMA        ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZMEV]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZMEH] + COEF_SIZE_ZMEH  ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP] = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZMEV] + COEF_SIZE_ZMEV        ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_VMX_V0]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP] + COEF_SIZE_ZME_TMP        ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_VMX_V1]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_VMX_V0] + COEF_SIZE_VMX_V0;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_VMX_VP0] = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_VMX_V1] + COEF_SIZE_VMX_V1        ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_WBC_ZMEH]  = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_VMX_VP0] + COEF_SIZE_VMX_VP0;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_WBC_ZMEV] = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_WBC_ZMEH] + COEF_SIZE_WBC_ZMEH        ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_HDR_METADATA] = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_WBC_ZMEV] + COEF_SIZE_WBC_ZMEV        ;

    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZMEH_V1] = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_HDR_METADATA] + COEF_SIZE_HDR_METADATA ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZMEV_V1] = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZMEH_V1] + COEF_SIZE_ZMEH_V1 ;
	

    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_MUTE_V1_L] = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_ZMEV_V1] + COEF_SIZE_ZMEV_V1 ;
    stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_MUTE_V1_C] = stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_MUTE_V1_L] + COEF_SIZE_V1_MUTE_L ;
	
	
	memset((HI_VOID *)stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_MUTE_V1_L],0x10,COEF_SIZE_V1_MUTE_L);
	memset((HI_VOID *)stVdpCoefBufAddr->pu8CoefVirAddr[VDP_COEF_BUF_MUTE_V1_C],0x80,COEF_SIZE_V1_MUTE_C);

    
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_IN_GAMMA   ]  = stVdpCoefBufAddr->stBufBaseAddr.u32StartPhyAddr + 0;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_OUT_GAMMA  ]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_IN_GAMMA   ] + COEF_SIZE_IN_GAMMA   ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_IN_DEGAMMA ]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_OUT_GAMMA  ] + COEF_SIZE_OUT_GAMMA  ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_OUT_DEGAMMA]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_IN_DEGAMMA ] + COEF_SIZE_IN_DEGAMMA ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_CVM        ]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_OUT_DEGAMMA] + COEF_SIZE_OUT_DEGAMMA;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_GDM_GAMMA  ]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_CVM        ] + COEF_SIZE_CVM        ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_GDM_DEGAMMA]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_GDM_GAMMA  ] + COEF_SIZE_GDM_GAMMA  ;

    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZMEH]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_GDM_DEGAMMA] + COEF_SIZE_GDM_DEGAMMA        ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZMEV]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZMEH] + COEF_SIZE_ZMEH  ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZME_TMP]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZMEV] + COEF_SIZE_ZMEV        ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_VMX_V0]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZME_TMP] + COEF_SIZE_ZME_TMP        ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_VMX_V1]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_VMX_V0] + COEF_SIZE_VMX_V0;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_VMX_VP0] = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_VMX_V1] + COEF_SIZE_VMX_V1        ;
	
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_WBC_ZMEH]  = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_VMX_VP0] + COEF_SIZE_VMX_VP0;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_WBC_ZMEV] = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_WBC_ZMEH] + COEF_SIZE_WBC_ZMEH        ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_HDR_METADATA] = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_WBC_ZMEV] + COEF_SIZE_WBC_ZMEV        ;

    
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZMEH_V1] = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_HDR_METADATA] + COEF_SIZE_HDR_METADATA ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZMEV_V1] = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZMEH_V1] + COEF_SIZE_ZMEH_V1 ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_MUTE_V1_L] = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_ZMEV_V1] + COEF_SIZE_ZMEV_V1 ;
    stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_MUTE_V1_C] = stVdpCoefBufAddr->u32CoefPhyAddr[VDP_COEF_BUF_MUTE_V1_L] + COEF_SIZE_V1_MUTE_L ;

    
    return HI_SUCCESS;
}


HI_S32 VDP_DRV_LayerBufAddrDistribute(VDP_LAYER_BUF_ADDR_S  *stVdpLayerBufAddr)
{
#if EDA_TEST
    stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V0]  = stVdpLayerBufAddr->stBufBaseAddr.u32StartVirAddr + 0;
    stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V1]  = stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V0] + VDP_BUF_SIZE_V0;
    stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V2]  = stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V1] + VDP_BUF_SIZE_V1;
    stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V3]  = stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V2] + VDP_BUF_SIZE_V2;
    stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V4]  = stVdpLayerBufAddr->u32BufAddr[VDP_BUF_V3] + VDP_BUF_SIZE_V3;
#endif

    return HI_SUCCESS;
}

//----------------------------------------------------------------------
// function
//----------------------------------------------------------------------
HI_U32 uGetRand(HI_U32 max, HI_U32 min)
{
	#if  0
    HI_U32 u32_max_data = 0;
    HI_U32 u32_min_data = 0;
    
    if(max > min)
    {
        u32_max_data = max;
        u32_min_data = min;
    }
    else
    {
        u32_max_data = min;
        u32_min_data = max;
    }
    
    return (u32_min_data + rand()%((HI_U32)u32_max_data + 1 - u32_min_data)); 
	#endif
	return HI_SUCCESS;
}

HI_S32 sGetRand(HI_S32 max, HI_S32 min)
{
	#if  0
    HI_U32 u32_max_data = 0;
    HI_U32 u32_min_data = 0;
    
    HI_S32 s32_return_data = 0;

    if(((max == 0)&&(min == 0)) || (min == max))
    {
        //cout << "Error, Worng sRand seed!" << endl;
    }
    else if(((max >= 0)&&(min > 0)) || ((max > 0)&&(min >= 0)))
    {
        if(max > min)
        {
            u32_max_data = max;
            u32_min_data = min;
        }
        else
        {
            u32_max_data = min;
            u32_min_data = max;
        }
        
        s32_return_data = (u32_min_data + rand()%(u32_max_data + 1 - u32_min_data)); 
    }
    else if(((max <= 0)&&(min < 0)) || ((max < 0)&&(min <= 0)))
    {
        if(VDP_ABS(max) > VDP_ABS(min))
        {
            u32_max_data = VDP_ABS(max);
            u32_min_data = VDP_ABS(min);
        }
        else
        {
            u32_max_data = VDP_ABS(min);
            u32_min_data = VDP_ABS(max);
        }
        
        s32_return_data = -(u32_min_data + rand()%(u32_max_data + 1 - u32_min_data)); 
    }
    else 
    {
        if((max > 0)&&(min < 0))
        {
            u32_max_data = VDP_ABS(max);
            u32_min_data = VDP_ABS(min);
        }
        else
        {
            u32_max_data = VDP_ABS(min);
            u32_min_data = VDP_ABS(max);
        }
        
        if(rand()%2 == 0)
        {
            s32_return_data = -(rand()%(u32_min_data + 1));
        }
        else
        {
            s32_return_data =    rand()%(u32_max_data + 1);
        }
    }
    
    return s32_return_data; 
	#endif
	
	return HI_SUCCESS;
}

HI_U32 uGetRandEx(HI_U32 max, HI_U32 min)
{
	#if  0
    HI_U32 u32_max_data = 0;
    HI_U32 u32_min_data = 0;
    
    if(max > min)
    {
        u32_max_data = max;
        u32_min_data = min;
    }
    else
    {
        u32_max_data = min;
        u32_min_data = max;
    }
    
    return (u32_min_data + rand()%((HI_U32)u32_max_data + 1 - u32_min_data)); 
	#endif
	
	return HI_SUCCESS;
}

HI_S32 sGetRandEx(HI_S32 max, HI_S32 min)
{
	#if  0
    HI_U32 u32_max_data = 0;
    HI_U32 u32_min_data = 0;
    
    HI_S32 s32_return_data = 0;

    if(((max == 0)&&(min == 0)) || (min == max))
    {
      //  cout << "Error, Worng sRand seed!" << endl;
    }
    if(((max >= 0)&&(min > 0)) || ((max > 0)&&(min >= 0)))
    {
        if(max > min)
        {
            u32_max_data = max;
            u32_min_data = min;
        }
        else
        {
            u32_max_data = min;
            u32_min_data = max;
        }
        
        s32_return_data = (u32_min_data + rand()%(u32_max_data + 1 - u32_min_data)); 
    }
    else if(((max <= 0)&&(min < 0)) || ((max < 0)&&(min <= 0)))
    {
        if(VDP_ABS(max) > VDP_ABS(min))
        {
            u32_max_data = VDP_ABS(max);
            u32_min_data = VDP_ABS(min);
        }
        else
        {
            u32_max_data = VDP_ABS(min);
            u32_min_data = VDP_ABS(max);
        }
        
        s32_return_data = -(u32_min_data + rand()%(u32_max_data + 1 - u32_min_data)); 
    }
    else 
    {
        if((max > 0)&&(min < 0))
        {
            u32_max_data = VDP_ABS(max);
            u32_min_data = VDP_ABS(min);
        }
        else
        {
            u32_max_data = VDP_ABS(min);
            u32_min_data = VDP_ABS(max);
        }
        
        if(rand()%2 == 0)
        {
            s32_return_data = -(rand()%(u32_min_data + 1));
        }
        else
        {
            s32_return_data =    rand()%(u32_max_data + 1);
        }
    }
    
    return s32_return_data; 
	#endif
	
	return HI_SUCCESS;
}





