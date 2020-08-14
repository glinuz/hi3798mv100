/**< hdmi interface needs to be init in linux */
#if defined __LINUX__
#include <string.h>
#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#ifndef __HI3798CV200__
#include "hi_adp_audio.h"
#endif
#include "hi_adp_mpi.h"
#include "hi_adp_hdmi.h"

#include "osal_type.h"
#include "sme_macro.h"

#include "test_hdmi_init.h"

V_INT32 TestHDMIInit(V_CHAR* pszType)
{
    V_INT32 Ret = 0;
    HI_UNF_ENC_FMT_E eFmt = HI_UNF_ENC_FMT_1080P_60;

    HI_SYS_Init();

    if(NULL != pszType)
    {
        if(!strcmp(pszType, "1080P_60"))
        {
            eFmt = HI_UNF_ENC_FMT_1080P_60;
        }
        else if(!strcmp(pszType, "1080P_50"))
        {
            eFmt = HI_UNF_ENC_FMT_1080P_50;
        }
        else if(!strcmp(pszType, "1080P_25"))
        {
            eFmt = HI_UNF_ENC_FMT_1080P_25;
        }
        else if(!strcmp(pszType, "1080P_24"))
        {
            eFmt = HI_UNF_ENC_FMT_1080P_24;
        }
        else if(!strcmp(pszType, "720P_60"))
        {
            eFmt = HI_UNF_ENC_FMT_720P_60;
        }
        else if(!strcmp(pszType, "720P_50"))
        {
            eFmt = HI_UNF_ENC_FMT_720P_50;
        }
        else if(!strcmp(pszType, "2160P_30"))
        {
            eFmt = HI_UNF_ENC_FMT_3840X2160_30;
        }
        else if(!strcmp(pszType, "2160P_24"))
        {
            eFmt = HI_UNF_ENC_FMT_3840X2160_24;
        }
        else if(!strcmp(pszType, "576P_50"))
        {
            eFmt = HI_UNF_ENC_FMT_576P_50;
        }
        else if(!strcmp(pszType, "480P_60"))
        {
            eFmt = HI_UNF_ENC_FMT_480P_60;
        }
    }

    Ret = HIADP_Disp_Init(eFmt);
    if (Ret != ICS_SUCCESS)
    {
        ICS_LOGE("call DispInit failed.\n");
        return -1;
    }

    return 0;
}

V_INT32 TestHDMIDeinit()
{
    HIADP_Disp_DeInit();
    HI_SYS_DeInit();

    return 0;
}

#endif
