/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpAVvVw4ERKDSiimgdao4nwOhxbHUZ3TWiJ5iG78
Gfd3ktJnK4B0vtwHlXmoQRE7QnFwm4iPM6AVdpKv2f1Gdh6uzUgmCGjGq4AVmkMCmpI9pX6i
n33N3aRmILTUOmiNPYu7rv5Kauz0mQ/xjO4mxxxJIpAkWN+X7fxPmaDUOSOVLA==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

#include "vfmw_trust_intf.h"
#include "trusted_vfmw_osal.h"
#include "vfmw_ctrl.h"
#include "vfmw_tee_client.h"
#include "vfmw_osal_ext.h"

#define PRINT_TEE_VFMW_TIME  (0)

extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;

/* SECURE MARK */
MEMORY_NEEDED_SECVFMW_S *g_pVfmSharewMem  = NULL;
VDEC_CHAN_OPTION_S *g_pChanOption; //store chan option
RAW_ARRAY_NS *g_pStreamList = NULL; //manage stream buffer
CALLBACK_ARRAY_NS *g_pCallBackList = NULL;//manage callback buffer
VDEC_CHAN_STATE_S  *g_pChanState = NULL; //store chan state
IMAGE_QUEUE_NS     *g_pImageList = NULL; //store image info
UINT8              *g_pProcBuf     = NULL;

/************************************************************************
    TEE 通信封装接口
*************************************************************************/
SINT32 SEC_VFMW_Init(UINT32 pArgs)
{
    SINT32 ret = VDEC_ERR;

    pArgs = SOS_Mmap(pArgs, sizeof(VDEC_OPERATION_S), 1, 1);
    if (pArgs == NULL)
    {
        dprint(PRN_FATAL, "%s %d, map_section_entry failed!\n", __func__,__LINE__);
        return VDEC_ERR;
    }

    ret = VDEC_Init((VDEC_OPERATION_S *)pArgs);

    SOS_Unmap(pArgs, sizeof(VDEC_OPERATION_S));

    return ret;
}

SINT32 SEC_VFMW_Exit(VOID)
{
    SINT32 ret = VDEC_ERR;

    ret = VDEC_Exit();

    return ret;
}

SINT32 SEC_VFMW_Resume(VOID)
{
    return VDEC_Resume();
}

SINT32 SEC_VFMW_Suspend(VOID)
{
    return VDEC_Suspend();
}

SINT32 SEC_VFMW_Control(SINT32 ChanID, UINT32 eCmdID, UINT32 pArgs)
{
    SINT32 ret = VDEC_ERR;
    UINT8  MapFlag = 0;
    SINT32 MapSize = 4096;

    if (pArgs != 0)
    {
        pArgs = SOS_Mmap(pArgs, MapSize, 1, 1);//指针所指的内容长度未知，暂映射4K
        if (NULL == pArgs)
        {
            dprint(PRN_FATAL, "%s %d, map_section_entry failed ret=%d\n", __func__,__LINE__,ret);
            return VDEC_ERR;
        }
        MapFlag = 1;
    }

    ret = VDEC_Control(ChanID, eCmdID, (VOID *)pArgs);

    if (1 == MapFlag)
    {
        SOS_Unmap(pArgs, MapSize);
    }

    return ret;
}

SINT32 SEC_VFMW_Thread(VOID)
{
   	SINT32 ret =0;

#if (1 == PRINT_TEE_VFMW_TIME)
    static UINT32 BeginTime = 0;
    static UINT32 StartTime = 0;
    static UINT32 Last_StartTime = 0;

    static UINT32 EndTime = 0;
    static UINT32 TotalTime = 0;
    static UINT32 Count = 0;

    StartTime = SOS_GetTimeInUs();
    if (Count == 0)
    {
        BeginTime = StartTime;
    }
#endif

    ret = VCTRL_RunProcess();

#if (1 == PRINT_TEE_VFMW_TIME)
    EndTime = SOS_GetTimeInUs();
    TotalTime += EndTime - StartTime;
    Count++;

    if (StartTime - Last_StartTime >= 1000*15)
    {
        dprint(PRN_FATAL, "twice call secure thread exceed cost:   %d  us\n", StartTime - Last_StartTime);
    }

    Last_StartTime = EndTime;

    if (EndTime - StartTime >= 1000*5)
    {
        dprint(PRN_FATAL, "run secure thread exceed cost:  %d  start:   %d  end:   %d   us\n",
                           EndTime - StartTime, StartTime, EndTime);
    }

    if (EndTime - BeginTime >= 1000000)
    {
        dprint(PRN_FATAL, "run secure thread Total: %d  Count: %d  Avg:     %d    us\n", TotalTime, Count, TotalTime / Count);
        TotalTime = BeginTime = StartTime = EndTime = 0;
        Count = 0;
    }
#endif

    return ret;
}

SINT32 SEC_VFMW_Get_Image(SINT32 ChanID, UINT32 pImage)
{
    SINT32 ret = 0;
    pImage = SOS_Mmap(pImage, sizeof(IMAGE), 1, 1);
    if (NULL == pImage)
    {
        dprint(PRN_FATAL, "%s %d, map_section_entry failed ret=%d\n", __func__,__LINE__,ret);
    }
    else
    {
        ret = VCTRL_GetChanImage(ChanID, (IMAGE *)pImage);
    }
    SOS_Unmap(pImage, sizeof(IMAGE));

    return ret;
}

SINT32 SEC_VFMW_Release_Image(SINT32 ChanID, UINT32 pImage)
{
    SINT32 ret = 0;
    pImage = SOS_Mmap(pImage, sizeof(IMAGE), 1, 1);
    if (NULL == pImage)
    {
        dprint(PRN_FATAL, "%s %d, map_section_entry failed ret=%d\n", __func__,__LINE__,ret);
    }
    else
    {
        ret = VCTRL_ReleaseChanImage(ChanID, (IMAGE *)pImage);
    }
    SOS_Unmap(pImage, sizeof(IMAGE));
    return ret;
}

SINT32 SEC_VFMW_Read_Proc(UINT32 Page, SINT32 Count)
{
    SINT32 ret = 0;

    if (g_pProcBuf != NULL)
    {
        ret = VCTRL_ReadProc((SINT8 *)g_pProcBuf, Count);
    }

    return ret;
}

SINT32 SEC_VFMW_Write_Proc(UINT32 Option, SINT32 Value)
{
    return VCTRL_WriteProc(Option, Value);
}

SINT32 SEC_TEST_Memcpy(SINT32 Src, UINT32 Dst, UINT32 Len)
{
    SINT32 ret = -1;
    UINT32 viraddr;

    if (0 != sre_mmap(Src, Len, &viraddr, 1, 0))
    {
        uart_printf_func("ca2ta map_section_entry failed ret!\n");
    }
    else
    {
        memcpy((UINT8 *)Dst, (UINT8 *)viraddr, Len);
        sre_unmap(viraddr, Len);
        ret = 0;
    }

    return ret;
}

/*======================================================================*/
/*     Secure firmware initialization                                   */
/*======================================================================*/
DECLARE_TC_DRV(
        Trusted_Vfmw,
        0,
        0,
        0,
        0,
        VDEC_OpenModule,
        NULL
        );

