/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_ao.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/17
  Description   :
  History       :
  1.Date        : 2013/04/17
    Author      : zgjie
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <mach/hardware.h>

/* Unf headers */
#include "hi_error_mpi.h"
#include "hi_drv_module.h"

/* Drv headers */
#include "hi_drv_ao.h"
#include "hi_drv_ai.h"
#include "drv_ao_ioctl.h"
#include "drv_ao_ext.h"
#include "drv_adsp_ext.h"
#include "drv_ao_private.h"

#include "hi_audsp_aoe.h"
#include "hal_aoe.h"
#include "hal_cast.h"
#include "hal_aiao.h"

#include "drv_ao_op.h"
#include "drv_ao_track.h"
#include "audio_util.h"
#include "drv_aiao_ioctl_veri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* private state */
typedef struct
{
    HI_U32  u32Port[AIAO_INT_BUTT];
} AIAO_STATE_S;

#define CHECK_AIAO_PORT_OPEN(Port) \
    do                                                         \
    {                                                          \
        if (AIAO_PORT_BUTT == g_pAiaoState->u32Port[Port])   \
        {                                                       \
            HI_ERR_AIAO(" Invalid Port id 0x%x\n", Port);        \
            return HI_FAILURE;                       \
        }                                                       \
    } while (0)

static AIAO_STATE_S g_AIAODevState;
static AIAO_STATE_S* g_pAiaoState = &g_AIAODevState;

HI_VOID AIAO_VERI_Open(HI_VOID)
{
    HI_S32 Id;

    /* init aiao state */
    for (Id = 0; Id < AIAO_INT_BUTT; Id++)
    {
        g_pAiaoState->u32Port[Id] = AIAO_PORT_BUTT;
    }
}

HI_VOID AIAO_VERI_Release(HI_VOID)
{
    HI_S32 Id;

    for (Id = 0; Id < AIAO_INT_BUTT; Id++)
    {
        if (AIAO_INT_BUTT != g_pAiaoState->u32Port[Id])
        {
            HAL_AIAO_P_Close_Veri(ID2PORT(g_pAiaoState->u32Port[Id]));
            g_pAiaoState->u32Port[Id] = AIAO_PORT_BUTT;
        }
    }
}

HI_S32 AIAO_VERI_ProcRead(struct seq_file* p, HI_VOID* v)
{
    HI_U32 Id;
    HI_S32 s32Ret;
    AIAO_PORT_STAUTS_S stStatus;
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
    AIAO_PORT_STAUTS_S stPortProcStatus;
    AIAO_PORT_ID_E enTmpPortID;
#endif

    for (Id = 0; Id < AIAO_INT_BUTT; Id++)
    {
        AIAO_PORT_ID_E enPortID = ID2PORT(g_pAiaoState->u32Port[Id]);
        if (AIAO_PORT_BUTT != enPortID)
        {
            HI_U32 u32Bytes = HAL_AIAO_P_QueryBufData(enPortID);
            HI_U32 u32Percent;

            s32Ret = HAL_AIAO_P_GetStatus( enPortID, &stStatus);
            if (HI_SUCCESS != s32Ret)
            {
                return s32Ret;
            }
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
            if (enPortID == AIAO_PORT_SPDIF_TX0)
            {
                enTmpPortID = AIAO_PORT_TX2;//SndOpGetPort(HI_UNF_SND_OUTPUTPORT_HDMI0, SND_AOP_TYPE_I2S);

                s32Ret = HAL_AIAO_P_GetStatus(enTmpPortID, &stPortProcStatus);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_AIAO("HAL_AIAO_P_GetStatus (port:%d) failed\n", (HI_U32)enPortID);
                    return s32Ret;
                }
                memcpy(&stStatus.stProcStatus, &stPortProcStatus.stProcStatus, sizeof(AIAO_PROC_STAUTS_S));
            }
#endif
            u32Percent = (u32Bytes * 100) / stStatus.stBuf.u32BUFF_SIZE;

            if ((AIAO_DIR_TX == PORT2DIR(enPortID)))
            {
                PROC_PRINT(p,
                           "%s_%s(%.2d),status(%s), Fs(%.6d), Ch(%.2d), Bit(%2d),DmaCnt(%.6u),DmaTry(%.6u),TotByte(%.6u),BufUsed(%.2u%%), PeriodSize(0x%.6x), PeriodNum(%.2d)\n",
                           (HI_CHAR*)(AIAO_MODE_TXI2S == PORT2MODE(enPortID) ? "i2s" : "spd"),
                           "tx", PORT2CHID(enPortID),
                           (HI_CHAR*)((AIAO_PORT_STATUS_START == stStatus.enStatus) ? "start" : ((AIAO_PORT_STATUS_STOP == stStatus.enStatus) ? "stop" : "stopping")),
                           stStatus.stUserConfig.stIfAttr.enRate,
                           stStatus.stUserConfig.stIfAttr.enChNum,
                           stStatus.stUserConfig.stIfAttr.enBitDepth,
                           stStatus.stProcStatus.uDMACnt,
                           stStatus.stProcStatus.uTryWriteCnt,
                           stStatus.stProcStatus.uTotalByteWrite,
                           u32Percent,
                           stStatus.stUserConfig.stBufConfig.u32PeriodBufSize,
                           stStatus.stUserConfig.stBufConfig.u32PeriodNumber);

                PROC_PRINT(p,
                           "    BufEmptyCnt(%.6u), BufEmptyWarningCnt(%.6u),BusFiFoEmptyCnt(%.6u),IfFiFoEmptyCnt(%.6u),BusTimeOutCnt(%.6u)\n\n",
                           stStatus.stProcStatus.uBufEmptyCnt, stStatus.stProcStatus.uBufEmptyWarningCnt, stStatus.stProcStatus.uBusFiFoEmptyCnt,
                           stStatus.stProcStatus.uInfFiFoEmptyCnt, stStatus.stProcStatus.uBusTimeOutCnt);
            }
            else
            {
                PROC_PRINT(p,
                           "i2s_%s(%.2d),status(%s), Fs(%.6d), Ch(%.2d), Bit(%.2d),DmaCnt(%.6u),DmaTry(%.6u),TotByte(%.6u),BufUsed(%.2u%%), PeriodSize(0x%.6x), PeriodNum(%.2d)\n",
                           "rx", PORT2CHID(enPortID),
                           (HI_CHAR*)((AIAO_PORT_STATUS_START == stStatus.enStatus) ? "start" : ((AIAO_PORT_STATUS_STOP == stStatus.enStatus) ? "stop" : "stopping")),
                           stStatus.stUserConfig.stIfAttr.enRate,
                           stStatus.stUserConfig.stIfAttr.enChNum,
                           stStatus.stUserConfig.stIfAttr.enBitDepth,
                           stStatus.stProcStatus.uDMACnt,
                           stStatus.stProcStatus.uTryWriteCnt,
                           stStatus.stProcStatus.uTotalByteRead,
                           u32Percent,
                           stStatus.stUserConfig.stBufConfig.u32PeriodBufSize,
                           stStatus.stUserConfig.stBufConfig.u32PeriodNumber);

                PROC_PRINT(p,
                           "    BufFullCnt(%.7u), BufFullWarningCnt(%.7u),BusFiFoFullCnt(:%.7u),IfFiFoFullCnt(%.7u),BusTimeOutCnt(%.7u)\n\n",
                           stStatus.stProcStatus.uBufFullCnt, stStatus.stProcStatus.uBufFullWarningCnt, stStatus.stProcStatus.uBusFiFoFullCnt,
                           stStatus.stProcStatus.uInfFiFoFullCnt, stStatus.stProcStatus.uBusTimeOutCnt);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 AIAO_VERI_ProcessCmd( struct inode* inode, struct file* file, HI_U32 cmd, HI_VOID* arg )
{
    HI_S32 Ret = HI_SUCCESS;
    switch (cmd)
    {
        case CMD_AIAO_GET_CAPABILITY:
        {
            AIAO_Port_GetCapability_S* Param = (AIAO_Port_GetCapability_S*)arg;
            HAL_AIAO_GetHwCapability(&Param->u32Capability);
            break;
        }

        case CMD_AIAO_SET_SYSCRG:
        {
            //HAL_AIAO_SetSysCrg(Param->u32CrgSkip);
            break;
        }

        case CMD_AIAO_RW_REGISTER:
        {
            AIAO_Dbg_Reg_S* pstDbgReg = (AIAO_Dbg_Reg_S*)arg;
            HAL_AIAO_DBG_RWReg(pstDbgReg);
            break;
        }

        case CMD_AIAO_PORT_OPEN:
        {
            AIAO_Port_Open_S* Param = (AIAO_Port_Open_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            if ( AIAO_PORT_BUTT != g_pAiaoState->u32Port[Id])
            {
                HI_ERR_AIAO("enPortID(0x%x), had open .\n", Param->enPortID);

                Ret = HI_FAILURE;
                break;
            }

            Param->pstConfig->pIsrFunc = HAL_AIAO_P_ProcStatistics;
            Ret = HAL_AIAO_P_Open_Veri(Param->enPortID, Param->pstConfig);
            if (HI_SUCCESS == Ret)
            {
                g_pAiaoState->u32Port[Id] = Id;
                //HI_ERR_AIAO("enPortID(0x%x), open success.\n", Param->enPortID);
            }
            else
            {
                HI_ERR_AIAO("enPortID(0x%x), open fail(0x%x).\n", Param->enPortID, Ret);
            }

            break;
        }

        case CMD_AIAO_PORT_CLOSE:
        {
            AIAO_Port_Close_S* Param = (AIAO_Port_Close_S*)arg;

            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            HAL_AIAO_P_Close_Veri(Param->enPortID);
            g_pAiaoState->u32Port[Id] = AIAO_PORT_BUTT;
            //HI_ERR_AIAO("enPortID(0x%x), close.\n", Param->enPortID);

            break;
        }

        case CMD_AIAO_PORT_START:
        {
            AIAO_Port_Start_S* Param = (AIAO_Port_Start_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_P_Start(Param->enPortID);
            break;
        }

        case CMD_AIAO_PORT_SEL_SPDIFOUTSOURCE:
        {
            AIAO_Port_SelectSpdifSource_S* Param = (AIAO_Port_SelectSpdifSource_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_P_SelectSpdifSource(Param->enPortID, Param->eSrcChnId);
            break;
            break;
        }

        case CMD_AIAO_PORT_SET_SPDIFOUTPORT:
        {
            AIAO_Port_SetSpdifOutPort_S* Param = (AIAO_Port_SetSpdifOutPort_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_P_SetSpdifOutPort(Param->enPortID, Param->bEn);
            break;
        }

        case CMD_AIAO_PORT_SET_I2SDATASEL:
        {
            AIAO_Port_I2SDataSel_S* Param = (AIAO_Port_I2SDataSel_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_P_SetI2SSdSelect(Param->enPortID, &Param->stSdSel);
            break;
        }

        case CMD_AIAO_PORT_STOP:
        {
            AIAO_Port_Stop_S* Param = (AIAO_Port_Stop_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_P_Stop(Param->enPortID, Param->enStopMode);
            break;
        }

        case CMD_AIAO_PORT_MUTE:
        {
            AIAO_Port_Mute_S* Param = (AIAO_Port_Mute_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_P_Mute(Param->enPortID, Param->bMute);
            break;
        }

        case CMD_AIAO_PORT_TRACKMODE:
        {
            AIAO_Port_TrackMode_S* Param = (AIAO_Port_TrackMode_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_P_SetTrackMode(Param->enPortID, Param->enTrackMode);
            break;
        }

        case CMD_AIAO_PORT_VOLUME:
        {
            AIAO_Port_Volume_S* Param = (AIAO_Port_Volume_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);

            Ret = HAL_AIAO_P_SetVolume(Param->enPortID, Param->u32VolumedB);
            break;
        }

        case CMD_AIAO_PORT_GET_USERCONFIG:
        {
            AIAO_Port_GetUserConfig_S* Param = (AIAO_Port_GetUserConfig_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);

            //HI_ERR_AIAO("enPortID(0x%x) .\n", Param->enPortID);
            Ret = HAL_AIAO_P_GetUserCongfig(Param->enPortID, &Param->stUserConfig);
            break;
        }

        case CMD_AIAO_PORT_GET_STATUS:
        {
            AIAO_Port_GetSTATUS_S* Param = (AIAO_Port_GetSTATUS_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_P_GetStatus(Param->enPortID, &Param->stStatus);
            if (HI_SUCCESS != Ret)
            {
                return Ret;
            }
            break;
        }

        case CMD_AIAO_PORT_READ_DATA:
        {
            AIAO_Port_ReadData_S* Param = (AIAO_Port_ReadData_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);

            Param->u32ReadBytes = HAL_AIAO_P_ReadData(Param->enPortID, Param->pu32Dest, Param->u32DestSize);
            break;
        }

        case CMD_AIAO_PORT_WRITE_DATA:
        case CMD_AIAO_PORT_PREPARE_DATA:
        {
            AIAO_Port_WriteData_S* Param = (AIAO_Port_WriteData_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);

            Param->u32WriteBytes = HAL_AIAO_P_WriteData(Param->enPortID, Param->pu32Src, Param->u32SrcLen);
            break;
        }

        case CMD_AIAO_PORT_UPDATE_RPTR:
        {
            AIAO_Port_ReadData_S* Param = (AIAO_Port_ReadData_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);
            Param->u32ReadBytes = HAL_AIAO_P_UpdateRptr(Param->enPortID, Param->pu32Dest, Param->u32DestSize);
            break;
        }

        case CMD_AIAO_PORT_UPDATE_WPTR:
        {
            AIAO_Port_WriteData_S* Param = (AIAO_Port_WriteData_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);

            Param->u32WriteBytes = HAL_AIAO_P_UpdateWptr(Param->enPortID, Param->pu32Src, Param->u32SrcLen);
            break;
        }

        case CMD_AIAO_PORT_QUERY_BUF:
        {
            AIAO_Port_BufStatus_S* Param = (AIAO_Port_BufStatus_S*)arg;
            HI_U32 Id = PORT2ID(Param->enPortID);
            CHECK_AIAO_PORT_OPEN(Id);

            Param->u32DataSize = HAL_AIAO_P_QueryBufData(Param->enPortID);
            Param->u32FreeSize = HAL_AIAO_P_QueryBufFree(Param->enPortID);
            break;
        }

        case CMD_AIAO_HDMI_SETAUDIO_ATTR:
        {
            AIAO_HDMI_AUDIO_ATRR_S* Param = (AIAO_HDMI_AUDIO_ATRR_S*)arg;
            HDMI_AUDIO_ATTR_S stHDMIAtr;
            HDMI_EXPORT_FUNC_S*      pstHdmiFunc;
            /* Get hdmi functions */
            Ret = HI_DRV_MODULE_GetFunction(HI_ID_HDMI, (HI_VOID**)&pstHdmiFunc);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AIAO("Get hdmi function err:%#x!\n", Ret);
                return Ret;
            }
            if (pstHdmiFunc && pstHdmiFunc->pfnHdmiGetAoAttr)
            {
                Ret = (pstHdmiFunc->pfnHdmiGetAoAttr)(HI_UNF_HDMI_ID_0, &stHDMIAtr);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AIAO("hdmi function pfnHdmiGetAoAttr err:%#x!\n", Ret);
                    return Ret;
                }
                stHDMIAtr.enSoundIntf  = Param->enSoundIntf;
                stHDMIAtr.enSampleRate = Param->enSampleRate;
                stHDMIAtr.u32Channels  = Param->u32Channels;
                stHDMIAtr.enBitDepth   = Param->enBitDepth;
                /*get the capability of the max pcm channels of the output device*/
                Ret = (pstHdmiFunc->pfnHdmiAudioChange)(HI_UNF_HDMI_ID_0, &stHDMIAtr);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AIAO("hdmi function pfnHdmiAudioChange err:%#x!\n", Ret);
                    return Ret;
                }
            }
            else
            {
                HI_ERR_AIAO(" hdmi function dont support pfnHdmiGetAoAttr err:%#x!\n", Ret);
                return HI_FAILURE;
            }
            break;
        }

#ifdef HI_AIAO_TIMER_SUPPORT
        case CMD_AIAO_TIMER_CREATE:
        {
            AIAO_Timer_Create_S* Param = (AIAO_Timer_Create_S*)arg;
            HI_U32 Id = TIMER2ID(Param->enTimerID);
            if ( AIAO_PORT_BUTT != g_pAiaoState->u32Port[Id])
            {
                HI_ERR_AIAO("enPortID(0x%x), had open .\n", Param->enTimerID);
                Ret = HI_FAILURE;
                break;
            }
            Param->pTimerIsrFunc = HAL_AIAO_T_TIMERProcess;
            Ret = HAL_AIAO_T_Create(Param->enTimerID, Param);
            if (HI_SUCCESS == Ret)
            {
                g_pAiaoState->u32Port[Id] = Id;
            }
            else
            {
                HI_ERR_AIAO("TimerID(0x%x), create timer fail(0x%x).\n", Param->enTimerID, Ret);
            }
            break;
        }

        case CMD_AIAO_TIMER_DESTROY:
        {
            AIAO_Timer_Destroy_S* Param = (AIAO_Timer_Destroy_S*)arg;
            HI_U32 Id = TIMER2ID(Param->enTimerID);
            CHECK_AIAO_PORT_OPEN(Id);
            HAL_AIAO_T_Destroy(Param->enTimerID);
            g_pAiaoState->u32Port[Id] = AIAO_PORT_BUTT;
            break;
        }

        case CMD_AIAO_TIMER_SETATTR:
        {
            AIAO_TIMER_Attr_S* Param = (AIAO_TIMER_Attr_S*)arg;
            HI_U32 Id = TIMER2ID(Param->enTimerID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_T_SetTimerAttr(Param->enTimerID, Param);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AIAO("enPortID(0x%x), CMD_AIAO_TIMER_SETATTR fail(0x%x).\n", Param->enTimerID, Ret);
            }
            break;
        }

        case CMD_AIAO_TIMER_SETENABLE:
        {
            AIAO_TIMER_Enable_S* Param = (AIAO_TIMER_Enable_S*)arg;
            HI_U32 Id = TIMER2ID(Param->enTimerID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_T_SetTimerEnable(Param->enTimerID, Param->bEnable);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AIAO("enPortID(0x%x), CMD_AIAO_TIMER_SETENABLE fail(0x%x).\n", Param->enTimerID, Ret);
            }
            break;
        }

        case CMD_AIAO_TIMER_GETSTATUS:
        {
            AIAO_TIMER_Status_S* Param = (AIAO_TIMER_Status_S*)arg;
            HI_U32 Id = TIMER2ID(Param->enTimerID);
            CHECK_AIAO_PORT_OPEN(Id);
            Ret = HAL_AIAO_T_GetStatus(Param->enTimerID, Param);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AIAO("enPortID(0x%x), CMD_AIAO_TIMER_GETSTATUS fail(0x%x).\n", Param->enTimerID, Ret);
            }
            break;
        }
#endif
        default:
            Ret = HI_FAILURE;
            {
                HI_WARN_AIAO("unknown cmd: 0x%x\n", cmd);
            }
            break;
    }

    return Ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
