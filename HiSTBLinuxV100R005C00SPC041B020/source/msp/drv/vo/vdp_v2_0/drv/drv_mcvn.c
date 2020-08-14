#include "drv_mcvn.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

static HI_U32 g_MacrovisionCoef[6][4][5]=
{
#if 0
    {
        {0x0,       0x0,      0x0,     0x0,       0x0},
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10607efe,0x800aaf40},
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10607efe,0x800aaf40},
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10607efe,0x800aaf40}
    },    /*PAL  PAL_N  PAL_Nc */
#else
    // 2011.9.17 ange for zg by caobin, according to new macrovision standard
	{
		{0x0,       0x0,      0x0,     0x0,       0x0},
		{0x8aa89a36,0x14f5c15,0xaa03fe,0x10407efe,0x800aaf40},
		{0x8aa89a36,0x14f5c15,0xaa03fe,0x10607efe,0x800aaf40},
		{0x8aa89a36,0x14f5c15,0xaa03fe,0x10507efe,0x800aaf40}
	},    /*PAL  PAL_N  PAL_Nc */

#endif

#if 0
    {
        {0x0,       0x0,      0x0,  0x0,       0x0},
        {0x46545d36,0x246db39,0x7f8,0x15600f0f,0x801ffc50},
        {0x46545d3e,0x246db39,0x7f8,0x15600f0f,0x801ffc50},
        {0x5615573e,0x246dbad,0x7f8,0x15500f0f,0x801ffc50}
    },    /*NTSC NTSC_J */
#else
    // 2011.9.17 ange for zg by caobin, according to new macrovision standard
	{
		{0x0,       0x0,      0x0,  0x0,       0x0},
		{0x46545d36,0x246db39,0x7f8,0x15400f0f,0x801ffc50},
		{0x46545d3e,0x246db39,0x7f8,0x15600f0f,0x801ffc50},
		{0x5615573e,0x246dbad,0x7f8,0x15500f0f,0x801ffc50}
	},    /*NTSC NTSC_J */

#endif
    
    {
        {0x0,       0x0,      0x0,  0x0,       0x0},
        {0x56155736,0x246dbad,0x7f8,0x15600f0f,0x801ffc50},
        {0x46545d36,0x246db39,0x7f8,0x15600f0f,0x801ffc50},
        {0x56155736,0x246dbad,0x7f8,0x15500f0f,0x801ffc50}
    },    /* NTSC_PAL_M*/

#if 0
    {
        {0x0,       0x0,      0x0,     0x0,   0x0       },
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10507efe,0x800aaf40},
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10507efe,0x800aaf40},
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10507efe,0x800aaf40}
    },    /*SECAM */
#else
    // 2011.9.17 ange for zg by caobin, according to new macrovision standard
    {
        {0x0,       0x0,      0x0,     0x0,   0x0       },
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10407efe,0x800aaf40},
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10607efe,0x800aaf40},
        {0x8aa89a36,0x14f5c15,0xaa03fe,0x10407efe,0x800aaf40}
    },    /*SECAM SIN/COS */
#endif
    
    {
        {0x0,       0x0,      0x0,  0x0,       0x0     },
        {0x56155736,0x246dbad,0x7f8,0x15601e1e,0x1ffc50},
        {0x56155736,0x246dbad,0x7f8,0x15601e1e,0x1ffc50},
        {0x56155736,0x246dbad,0x7f8,0x15601e1e,0x1ffc50}
    },  /*PAL_60*/

    {
        {0x0,       0x0,      0x0,  0x0,       0x0     },
        {0x56155736,0x246dbad,0x7f8,0x15500f0f,0x1ffc50},
        {0x56155736,0x246dbad,0x7f8,0x15500f0f,0x1ffc50},
        {0x56155736,0x246dbad,0x7f8,0x15500f0f,0x1ffc50}
    },  /*NTSC_443*/


};

DRV_MCVN_S g_stMavn[HI_DRV_DISPLAY_BUTT];

HI_S32 DRV_MCVN_Init(HI_DRV_DISPLAY_E enDisp,HI_DRV_DISP_FMT_E enFmt)
{
    if( HI_DRV_DISPLAY_0 == enDisp)
    {
        DRV_MCVN_SetDateMcvn(enFmt, HI_DRV_DISP_MACROVISION_TYPE0);

        g_stMavn[enDisp].enMcvn = HI_DRV_DISP_MACROVISION_TYPE0;
    }
    else if ( HI_DRV_DISPLAY_1 == enDisp)
    {
        VDP_MVCN_SetMcvnEnable(HI_FALSE);
        
        g_stMavn[enDisp].enMcvn = HI_DRV_DISP_MACROVISION_TYPE0;
    }
    else
    {

    }
    
    return HI_SUCCESS;
}

HI_S32 DRV_MCVN_DeInit(HI_DRV_DISPLAY_E enDisp)
{
    return HI_SUCCESS;
}

HI_S32 DRV_MCVN_SetDateMcvn(HI_DRV_DISP_FMT_E enFmt, HI_DRV_DISP_MACROVISION_E enMcvn)
{
    HI_U32 *pu32McvnDate;

    switch (enFmt)
    {
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_SIN:
            if(enMcvn >= 4)
                return HI_SUCCESS;
            pu32McvnDate = g_MacrovisionCoef[3][enMcvn];
            break;

        case HI_DRV_DISP_FMT_PAL_M:

            if(enMcvn >= 4)
                return HI_SUCCESS;
            pu32McvnDate = g_MacrovisionCoef[2][enMcvn];
            break;

        case HI_DRV_DISP_FMT_NTSC:
        case HI_DRV_DISP_FMT_NTSC_J:

            if(enMcvn >= 4)
                return HI_SUCCESS;
            pu32McvnDate = g_MacrovisionCoef[1][enMcvn];
            break;

        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:

            if(enMcvn >= 4)
                return HI_SUCCESS;
            pu32McvnDate = g_MacrovisionCoef[0][enMcvn];
            break;

        default:
            pu32McvnDate = g_MacrovisionCoef[0][0];
            break;
    }

    VDP_MVCN_DATE_SetMcvn(pu32McvnDate);

    return HI_SUCCESS;
}

HI_S32 DRV_MCVN_SetMacrovision(HI_DRV_DISPLAY_E enDisp, 
                                HI_DRV_DISP_MACROVISION_E enMode,
                                HI_DRV_DISP_FMT_E enFmt)
{
    if(g_stMavn[enDisp].enMcvn == enMode)
    {
        return HI_SUCCESS;
    }

    if (HI_DRV_DISPLAY_1 == enDisp)
    {
        if(enMode == HI_DRV_DISP_MACROVISION_TYPE0)
        {
            VDP_MVCN_SetMcvnEnable(HI_FALSE);
        }
        else if(  (enMode >= HI_DRV_DISP_MACROVISION_TYPE1)
                &&(enMode <= HI_DRV_DISP_MACROVISION_TYPE3))
        {
            VDP_MVCN_SetMcvnEnable(HI_TRUE);
        }
        else
        {
            HI_ERR_DISP("HD DISP does not support the macrovision type!\n");
            return HI_ERR_DISP_INVALID_PARA;
        }

        g_stMavn[enDisp].enMcvn = enMode;
    }
    else if (HI_DRV_DISPLAY_0 == enDisp)
    {
        if(  (enMode >= HI_DRV_DISP_MACROVISION_TYPE0)
                &&(enMode <= HI_DRV_DISP_MACROVISION_TYPE3))
        {
            DRV_MCVN_SetDateMcvn(enFmt, enMode);
        }
        
        g_stMavn[enDisp].enMcvn = enMode;
    }

    return HI_SUCCESS;
}                                

HI_S32 DRV_MCVN_GetMacrovision(HI_DRV_DISPLAY_E enDisp, 
                                HI_DRV_DISP_MACROVISION_E *penMode)
{
    *penMode = g_stMavn[enDisp].enMcvn;
    
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

