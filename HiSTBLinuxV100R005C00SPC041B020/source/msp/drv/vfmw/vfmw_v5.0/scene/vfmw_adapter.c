/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:EG4uRhTwMmgcVFBsBnYHCDadN5jJKSuVyxmmaCmKFU6eJEbB2fyHF9weu4/jer/hxLHb+S1e
E0zVg4C3NiZh4exdkhFHEraoGcOeILRw1Dnv7buHSoKcV6T0S5fLECrc3HRugWFSumMXeS/h
zcVEyNXuPASsxlZgoc5SKBKrxBH2NFC4MMtdrGmqWThg2UrGoxkafI/3x0Yrr4uajISslJBP
7XulUcT5o7Q1PkRfrnf49MADwUdJ31s0nxEMW1S5jjGZA0bDDkiQyJ/JySm1Fw==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

#include "public.h"
#include "vfmw_debug.h"
#include "vfmw_adapter.h"

#ifdef HI_TEE_SUPPORT
#include "vfmw_tee_client.h"
#endif

#include "vfmw_osal_ext.h"

extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;

/*======================================================================*/
/*   Type definition                                                        */
/*======================================================================*/
typedef struct
{
    UINT8 IsSec;
}GLB_CHAN_INFO;

#define VFMW_ASSERT_RET(cond, else_print)                \
do {                                                     \
    if(!(cond))                                          \
    {                                                    \
        dprint(PRN_FATAL,"%s %d: %s\n", __func__, __LINE__, else_print ); \
        return VDEC_ERR;                                 \
    }                                                    \
}while(0)


/*======================================================================*/
/*   Internal data                                                      */
/*======================================================================*/
static GLB_CHAN_INFO g_ChanInfo[MAX_CHAN_NUM];


/*======================================================================*/
/*   Internal interface                                                 */
/*======================================================================*/
static UINT8 VFMW_GetTVPFlag(SINT32 ChanID, VDEC_CID_E eCmdID, VOID *pArgs)
{
    SINT64 *p64 = NULL;
    UINT8 SecFlag = 0;

    if (ChanID >= 0 && ChanID < MAX_CHAN_NUM)
    {
        SecFlag = g_ChanInfo[ChanID].IsSec;
    }
    else
    {
        if (VDEC_CID_CREATE_CHAN == eCmdID || VDEC_CID_CREATE_CHAN_WITH_OPTION == eCmdID
         || VDEC_CID_GET_CHAN_MEMSIZE == eCmdID || VDEC_CID_GET_CHAN_DETAIL_MEMSIZE_WITH_OPTION == eCmdID)
        {
            p64 = (SINT64*)pArgs;
            SecFlag = ((VDEC_CHAN_OPTION_S*)UINT64_PTR(p64[1]))->s32IsSecMode;
        }
    }

    return SecFlag;
}

static VOID VFMW_AfterControl(SINT32 ChanID, VDEC_CID_E eCmdID, VOID *pArgs, UINT8 IsSec)
{
    SINT32 *p32 = NULL;

    switch (eCmdID)
    {
        case VDEC_CID_CREATE_CHAN:
        case VDEC_CID_CREATE_CHAN_WITH_OPTION:
            p32 = (SINT32 *)pArgs;
            ChanID = p32[0];
            if (ChanID < 0 || ChanID >= MAX_CHAN_NUM)
            {
                dprint(PRN_ERROR, "%s eCmdID %d: ChanID %d invalid!\n", __func__, eCmdID, ChanID);
            }
            else
            {
                g_ChanInfo[ChanID].IsSec = (IsSec) ? 1 : 0;
            }
            break;

        default:
            break;
    }

    return;
}

/*======================================================================*/
/*   External interface                                                 */
/*======================================================================*/
VOID VFMW_OpenModule(VOID)
{
    VDEC_OpenModule();

#ifdef HI_TEE_SUPPORT
    TVP_VDEC_OpenModule();
#endif

    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(&g_ChanInfo, 0, MAX_CHAN_NUM * sizeof(GLB_CHAN_INFO));

    return;
}


VOID VFMW_ExitModule(VOID)
{
    VDEC_ExitModule();

#ifdef HI_TEE_SUPPORT
    TVP_VDEC_ExitModule();
#endif

    return;
}


SINT32 VFMW_Init(VDEC_OPERATION_S *pArgs)
{
    SINT32 ret = VDEC_ERR;

    if (NULL == pArgs)
    {
        dprint(PRN_ERROR, "%s pArgs = NULL\n", __func__);

        return HI_FAILURE;
    }

    if (pArgs->is_secure)
    {
    #ifdef HI_TEE_SUPPORT
        ret = TVP_VDEC_TrustDecoderInit(pArgs);
    #endif
    }
    else
    {
        ret = VDEC_Init(pArgs);
    }

    return (ret == VDEC_OK)? HI_SUCCESS: HI_FAILURE;
}


SINT32 VFMW_Control(SINT32 ChanID, VDEC_CID_E eCmdID, VOID *pArgs)
{
    SINT32 ret = VDEC_ERR;
    UINT8 IsSec = 0;

    IsSec = VFMW_GetTVPFlag(ChanID, eCmdID, pArgs);

    if (IsSec)
    {
    #ifdef HI_TEE_SUPPORT
        ret = TVP_VDEC_Control(ChanID, eCmdID, pArgs);
    #endif
    }
    else
    {
        ret = VDEC_Control(ChanID, eCmdID, pArgs);
    }

    if (ret == VDEC_OK)
    {
        VFMW_AfterControl(ChanID, eCmdID, pArgs, IsSec);
    }

    return (ret == VDEC_OK)? HI_SUCCESS: HI_FAILURE;
}

SINT32 VFMW_Exit(UINT8 IsSec)
{
    SINT32 ret = VDEC_ERR;

    if (IsSec)
    {
    #ifdef HI_TEE_SUPPORT
        ret = TVP_VDEC_TrustDecoderExit();
    #endif
    }
    else
    {
        ret = VDEC_Exit();
    }

    return (ret == VDEC_OK)? HI_SUCCESS: HI_FAILURE;
}


SINT32 VFMW_Suspend(VOID)
{
    SINT32 ret;

    ret = VDEC_Suspend();
    if (ret != VDEC_OK)
    {
        HI_FATAL_VFMW("%s Call VDEC_Suspend Failed!\n", __func__);
    }

#ifdef HI_TEE_SUPPORT
    ret = TVP_VDEC_Suspend();
    if (ret != VDEC_OK)
    {
        HI_FATAL_VFMW("%s Call TVP_VDEC_Suspend Failed!\n", __func__);
    }
#endif

#ifdef VDH_DISTRIBUTOR_ENABLE
    if (g_HalDisable != 1)
    {
        VDH_Suspend();
    }
#endif

    return VDEC_OK;
}


SINT32 VFMW_Resume(VOID)
{
    SINT32 ret;

#ifdef VDH_DISTRIBUTOR_ENABLE
    if (g_HalDisable != 1)
    {
        VDH_Resume();
    }
#endif

    ret = VDEC_Resume();
    if (ret != VDEC_OK)
    {
        HI_FATAL_VFMW("%s Call VDEC_Resume Failed!\n", __func__);
    }

#ifdef HI_TEE_SUPPORT
    ret = TVP_VDEC_Resume();
    if (ret != VDEC_OK)
    {
        HI_FATAL_VFMW("%s Call TVP_VDEC_Resume Failed!\n", __func__);
    }
#endif

    return VDEC_OK;
}

SINT32 VFMW_GetImage(SINT32 ChanID, IMAGE *pImage)
{
    SINT32 ret = VDEC_ERR;

    VFMW_ASSERT_RET(ChanID >= 0 && ChanID < MAX_CHAN_NUM, "ChanID out of range");

#ifdef HI_TEE_SUPPORT
    if (g_ChanInfo[ChanID].IsSec)
    {
        ret = TVP_VDEC_GetImage(ChanID, pImage);
    }
    else
#endif
    {
        ret = VDEC_GetImage(ChanID, pImage);
    }

    return ret;
}

SINT32 VFMW_ReleaseImage(SINT32 ChanID, IMAGE *pImage)
{
    SINT32 ret = VDEC_ERR;

    VFMW_ASSERT_RET(ChanID >= 0 && ChanID < MAX_CHAN_NUM, "ChanID out of range");

#ifdef HI_TEE_SUPPORT
    if (g_ChanInfo[ChanID].IsSec)
    {
        ret = TVP_VDEC_ReleaseImage(ChanID, pImage);
    }
    else
#endif
    {
        ret = VDEC_ReleaseImage(ChanID, pImage);
    }

    return ret;
}

