
#include "hi_type.h"

#include "hi_common.h"
#include "hi_reg_common.h"

#include "demux_reg.h"
#include "demux_hal.h"

#define DMX_PORT_OFFSET             1

HI_VOID DmxHalCSA3Reset(HI_BOOL Enable)
{
    U_PVR_CSA3_RST reg;

    reg.value = DMX_READ_REG(PVR_CSA3_RST);

    reg.bits.pvr_csa3_soft_rst = Enable;
    DMX_WRITE_REG(PVR_CSA3_RST, reg.value);
}

#if defined(CHIP_TYPE_hi3716h) || defined(CHIP_TYPE_hi3716c)

HI_VOID DmxHalConfigHardware(HI_VOID)
{
    U_PERI_CRG25 reg;

    reg.value = SYS_READ_REG(PERI_CRG25);

    reg.bits.pvr_srst_req = 1;
    SYS_WRITE_REG(PERI_CRG25, reg.value);

    reg.bits.tsi_clkout_cken    = 1;
    reg.bits.tsi2_cken          = 1;
    reg.bits.tsi1_cken          = 1;
    reg.bits.tsi0_cken          = 1;
    reg.bits.dmx_clk_sel        = 1;
    reg.bits.dmx_cken           = 1;
    reg.bits.pvr_hcken          = 1;
    reg.bits.pvr_srst_req       = 0;

    SYS_WRITE_REG(PERI_CRG25, reg.value);
}

#elif defined(CHIP_TYPE_hi3716m)

HI_VOID DmxHalConfigHardware(HI_VOID)
{
    HI_CHIP_TYPE_E      ChipType = HI_CHIP_TYPE_HI3716M;
    HI_CHIP_VERSION_E   Version = HI_CHIP_VERSION_V300;

    HI_DRV_SYS_GetChipVersion(&ChipType, &Version);
    if ((HI_CHIP_TYPE_HI3716M == ChipType) && (HI_CHIP_VERSION_V300 == Version))
    {
        U_PERI_CRG25_V300 reg;

        reg.value = SYS_READ_REG(PERI_CRG25);

        reg.bits.pvr_srst_req = 1;
        SYS_WRITE_REG(PERI_CRG25, reg.value);

        reg.bits.dmx_clk_sel    = 0;    // use 198MHz
        reg.bits.tsi2_cken      = 1;
        reg.bits.tsi1_cken      = 1;
        reg.bits.tsi0_cken      = 1;
        reg.bits.dmx_cken       = 1;
        reg.bits.pvr_hcken      = 1;
        reg.bits.pvr_srst_req   = 0;

        DmxHalCSA3Reset(HI_TRUE);

        SYS_WRITE_REG(PERI_CRG25, reg.value);
    }
    else
    {
        U_PERI_CRG25 reg;

        reg.value = SYS_READ_REG(PERI_CRG25);

        reg.bits.pvr_srst_req = 1;
        SYS_WRITE_REG(PERI_CRG25, reg.value);

        reg.bits.tsi_clkout_cken    = 1;
        reg.bits.tsi2_cken          = 1;
        reg.bits.tsi1_cken          = 1;
        reg.bits.tsi0_cken          = 1;
        reg.bits.dmx_clk_sel        = 0;
        reg.bits.dmx_cken           = 1;
        reg.bits.pvr_hcken          = 1;
        reg.bits.pvr_srst_req       = 0;

        SYS_WRITE_REG(PERI_CRG25, reg.value);
    }
}

#elif defined(CHIP_TYPE_hi3712) || defined(CHIP_TYPE_hi3715)

HI_VOID DmxHalConfigHardware(HI_VOID)
{
    U_PERI_CRG25 reg;

    reg.value = SYS_READ_REG(PERI_CRG25);

    reg.bits.pvr_srst_req = 1;
    SYS_WRITE_REG(PERI_CRG25, reg.value);

    reg.bits.dmx_clk_sel    = 0;
    reg.bits.tsi1_cken      = 1;
    reg.bits.tsi0_cken      = 1;
    reg.bits.dmx_cken       = 1;
    reg.bits.pvr_hcken      = 1;
    reg.bits.pvr_srst_req   = 0;

    SYS_WRITE_REG(PERI_CRG25, reg.value);
}

#elif  defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3719mv100_a) \
    || defined(CHIP_TYPE_hi3716cv200es)

HI_VOID DmxHalConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;
    U_PERI_CRG64 PeriCrg64;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    // reset demux
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    udelay(1);

    PeriCrg63.bits.pvr_bus_cken     = 1;
    PeriCrg63.bits.pvr_dmx_cken     = 1;
    PeriCrg63.bits.pvr_27m_cken     = 1;
    PeriCrg63.bits.pvr_tsi1_cken    = 1;
    PeriCrg63.bits.pvr_tsi2_cken    = 1;
#if defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)
    PeriCrg63.bits.pvr_tsi3_cken    = 1;
    PeriCrg63.bits.pvr_tsi4_cken    = 1;
    PeriCrg63.bits.pvr_tsi5_cken    = 1;
#endif

#if defined(CHIP_TYPE_hi3716cv200es)
    PeriCrg63.bits.pvr_tsi6_cken    = 1;
    PeriCrg63.bits.pvr_tsi7_cken    = 1;
#endif
    PeriCrg63.bits.pvr_ts0_cken     = 1;
    PeriCrg63.bits.pvr_ts1_cken     = 1;
    PeriCrg63.bits.pvr_tsout0_cken  = 1;
    PeriCrg63.bits.pvr_tsout1_cken  = 1;
    PeriCrg63.bits.pvr_srst_req     = 0;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    PeriCrg64.bits.pvr_ts0_clk_sel  = 1;
    PeriCrg64.bits.pvr_ts1_clk_sel  = 1;

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}

#elif  defined(CHIP_TYPE_hi3716mv410)  || defined(CHIP_TYPE_hi3716mv420)
HI_VOID DmxHalConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;
    U_PERI_CRG64 PeriCrg64;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;
    
    // reset demux
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    udelay(1);

    PeriCrg63.bits.pvr_bus_cken     = 1;
    PeriCrg63.bits.pvr_dmx_cken     = 1;
    PeriCrg63.bits.pvr_27m_cken     = 0;
    PeriCrg63.bits.pvr_tsi1_cken    = 1;
    PeriCrg63.bits.pvr_tsi2_cken    = 1;
    PeriCrg63.bits.pvr_tsi3_cken    = 1;
    PeriCrg63.bits.pvr_tsi4_cken    = 1;
    PeriCrg63.bits.pvr_tsi5_cken    = 1;
#if defined(CHIP_TYPE_hi3716mv420)    
    PeriCrg63.bits.pvr_tsi6_cken    = 1;
    PeriCrg63.bits.pvr_tsi7_cken    = 1;
#endif    
    PeriCrg63.bits.pvr_ts0_cken     = 1;
    PeriCrg63.bits.pvr_tsout0_cken  = 1;
#if defined(CHIP_TYPE_hi3716mv420) 
    PeriCrg63.bits.pvr_ts1_cken     = 1;
    PeriCrg63.bits.pvr_tsout1_cken  = 1;
#endif
    PeriCrg63.bits.pvr_srst_req     = 0;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    PeriCrg64.bits.pvr_ts0_clk_sel  = 1;
#if defined(CHIP_TYPE_hi3716mv420)  
    PeriCrg64.bits.pvr_ts1_clk_sel  = 1;
#endif

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}
#endif

HI_S32 DmxHalGetInitStatus(HI_VOID)
{
    HI_S32 ret      = HI_FAILURE;
    HI_U32 FqStatus = DMX_READ_REG(FQ_INIT_DONE) & 1;
    HI_U32 OqStatus = DMX_READ_REG(OQ_INIT_DONE) & 1;

    if (!FqStatus && !OqStatus)
    {
        ret = HI_SUCCESS;
    }

    return ret;
}

/***********************************************************************************
* Function      : DmxHalDvbPortSetAttr
* Description   : Set Tuner Port
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDvbPortSetAttr(HI_U32 PortId,  HI_UNF_DMX_PORT_TYPE_E  PortType,
        HI_U32                  SyncOn,
        HI_U32                  SyncOff,
        HI_U32                  TunerInClk,
        HI_U32                  BitSelector)
{
    U_TS_INTERFACE  ts_interface;

    ts_interface.value = DMX_READ_REG(TS_INTERFACE(PortId));

    ts_interface.bits.port_sel = 0;
    DMX_WRITE_REG(TS_INTERFACE(PortId), ts_interface.value);

    switch (PortType)
    {
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST :
        {
            ts_interface.bits.serial_sel    = 0;
            ts_interface.bits.sync_mode     = 0;

            break;
        }

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188 :
        {
            ts_interface.bits.serial_sel        = 0;
            ts_interface.bits.sync_mode         = 2;
            ts_interface.bits.nosync_fixed_204  = 0;

            break;
        }

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204 :
        {
            ts_interface.bits.serial_sel        = 0;
            ts_interface.bits.sync_mode         = 2;
            ts_interface.bits.nosync_fixed_204  = 1;

            break;
        }

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204 :
        {
            ts_interface.bits.serial_sel    = 0;
            ts_interface.bits.sync_mode     = 3;

            break;
        }

        case HI_UNF_DMX_PORT_TYPE_SERIAL :
        {
            ts_interface.bits.serial_sel    = 1;
            ts_interface.bits.sync_mode     = 0;
            ts_interface.bits.ser_2bit_mode = 0;
            ts_interface.bits.ser_2bit_rev  = 0;
            ts_interface.bits.ser_nosync    = 0;

            break;
        }

        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT :
        {
            ts_interface.bits.serial_sel    = 1;
            ts_interface.bits.sync_mode     = 0;
            ts_interface.bits.ser_2bit_mode = 1;
            ts_interface.bits.ser_2bit_rev  = 0;
            ts_interface.bits.ser_nosync    = 0;

            break;
        }

        case HI_UNF_DMX_PORT_TYPE_SERIAL_NOSYNC :
        {
            ts_interface.bits.serial_sel    = 1;
            ts_interface.bits.sync_mode     = 0;
            ts_interface.bits.ser_2bit_mode = 0;
            ts_interface.bits.ser_2bit_rev  = 0;
            ts_interface.bits.ser_nosync    = 1;

            break;
        }

        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT_NOSYNC :
        {
            ts_interface.bits.serial_sel    = 1;
            ts_interface.bits.sync_mode     = 0;
            ts_interface.bits.ser_2bit_mode = 1;
            ts_interface.bits.ser_2bit_rev  = 0;
            ts_interface.bits.ser_nosync    = 1;

            break;
        }

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID :
        default :
        {
            ts_interface.bits.serial_sel    = 0;
            ts_interface.bits.sync_mode     = 1;
        }
    }

    ts_interface.bits.bit_sel           = BitSelector;
    ts_interface.bits.syncon_th         = SyncOn;
    ts_interface.bits.syncoff_th        = SyncOff;

    DMX_WRITE_REG(TS_INTERFACE(PortId), ts_interface.value);

    ts_interface.bits.port_sel = 1;
    DMX_WRITE_REG(TS_INTERFACE(PortId), ts_interface.value);
}

HI_VOID DmxHalDvbPortSetTsCountCtrl(HI_U32 PortId, HI_U32 option)
{
    U_TS_COUNT_CTRL ts_count_ctrl;

    ts_count_ctrl.value = DMX_READ_REG(TS_COUNT_CTRL(PortId));

    ts_count_ctrl.bits.ts_count_ctrl = option;
    DMX_WRITE_REG(TS_COUNT_CTRL(PortId), ts_count_ctrl.value);
}

HI_VOID DmxHalDvbPortSetErrTsCountCtrl(HI_U32 PortId, HI_U32 option)
{
    U_ETS_COUNT_CTRL ets_count_ctrl;

    ets_count_ctrl.value = DMX_READ_REG(ETS_COUNT_CTRL(PortId));

    ets_count_ctrl.bits.ets_count_ctrl = option;
    DMX_WRITE_REG(ETS_COUNT_CTRL(PortId), ets_count_ctrl.value);
}

/***********************************************************************************
* Function      : DmxHalDemuxSetPortId
* Description   : Set Dmx PortId
* Input         : DmxId, PortMode, PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDemuxSetPortId(HI_U32 DmxId, DMX_PORT_MODE_E PortMode, HI_U32 PortId)
{
    HI_U32          Id = 0;
    U_SWITCH_CFG    SwitchCfg;

    if (DMX_PORT_MODE_TUNER == PortMode)
    {
        Id = PortId + DMX_PORT_OFFSET;
    }

    SwitchCfg.value = DMX_READ_REG(SWITCH_CFG);

    switch (DmxId)
    {
        case 0:
            SwitchCfg.bits.switch_cfg1 = Id;
            break;

        case 1:
            SwitchCfg.bits.switch_cfg2 = Id;
            break;

        case 2:
            SwitchCfg.bits.switch_cfg3 = Id;
            break;

        case 3:
            SwitchCfg.bits.switch_cfg4 = Id;
            break;

        case 4:
            SwitchCfg.bits.switch_cfg5 = Id;
            break;

        default:
            break;
    }

    DMX_WRITE_REG(SWITCH_CFG, SwitchCfg.value);
}

/***********************************************************************************
* Function      : DmxHalChannelSetPlayDmxId
* Description   : Set Channel Play DmxId
* Input         : ChanId, DmxId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalChannelSetPlayDmxId(HI_U32 ChanId, HI_U32 DmxId)
{
    U_DMX_PID_EN DmxPidEn;

    DmxPidEn.value = DMX_READ_REG(DMX_PID_EN(ChanId));

    DmxPidEn.bits.pid_play_dmx_id = (DMX_INVALID_DMX_ID == DmxId) ? 0 : (1 + DmxId);
    DMX_WRITE_REG(DMX_PID_EN(ChanId), DmxPidEn.value);
}

HI_VOID DmxHalChannelSetPlayBufId(HI_U32 ChanId, HI_U32 OQId)
{
    U_DMX_PID_PLAY_BUF pid_play_buf;

    pid_play_buf.value = DMX_READ_REG(DMX_PID_PLAY_BUF(ChanId));

    pid_play_buf.bits.play_buf = OQId;
    DMX_WRITE_REG(DMX_PID_PLAY_BUF(ChanId), pid_play_buf.value);
}

HI_VOID DmxHalChannelSetDataType(HI_U32 ChanId, HI_UNF_DMX_CHAN_TYPE_E ChanType)
{
    U_DMX_PID_CTRL  dmx_pid_ctrl;
    HI_U32          type;

    switch (ChanType)
    {
        case HI_UNF_DMX_CHAN_TYPE_SEC :
        case HI_UNF_DMX_CHAN_TYPE_ECM_EMM :
            type = 0;
            break;

        case HI_UNF_DMX_CHAN_TYPE_PES :
        case HI_UNF_DMX_CHAN_TYPE_AUD :
        case HI_UNF_DMX_CHAN_TYPE_VID :
        case HI_UNF_DMX_CHAN_TYPE_POST :
        default :
            type = 1;
    }

    dmx_pid_ctrl.value = DMX_READ_REG(DMX_PID_CTRL(ChanId));

    dmx_pid_ctrl.bits.data_type = type;
    DMX_WRITE_REG(DMX_PID_CTRL(ChanId), dmx_pid_ctrl.value);
}

HI_VOID DmxHalChannelSetAttr(HI_U32 ChanId, HI_UNF_DMX_CHAN_TYPE_E ChanType)
{
    U_DMX_PID_CTRL  dmx_pid_ctrl;
    HI_U32          type;

    switch (ChanType)
    {
        case HI_UNF_DMX_CHAN_TYPE_PES :
            type = 3;
            break;

        case HI_UNF_DMX_CHAN_TYPE_AUD :
            type = 6;
            break;

        case HI_UNF_DMX_CHAN_TYPE_VID :
            type = 7;
            break;

        case HI_UNF_DMX_CHAN_TYPE_SEC :
        case HI_UNF_DMX_CHAN_TYPE_ECM_EMM :
        case HI_UNF_DMX_CHAN_TYPE_POST :
        default :
            type = 0;
    }

    dmx_pid_ctrl.value = DMX_READ_REG(DMX_PID_CTRL(ChanId));

    dmx_pid_ctrl.bits.ch_attri = type;
    DMX_WRITE_REG(DMX_PID_CTRL(ChanId), dmx_pid_ctrl.value);
}

HI_VOID DmxHalChannelSetPusiCtrl(HI_U32 ChanId, HI_BOOL PusiCtrl)
{
    U_DMX_PID_CTRL  dmx_pid_ctrl;

    dmx_pid_ctrl.value = DMX_READ_REG(DMX_PID_CTRL(ChanId));

    dmx_pid_ctrl.bits.pusi_disable = PusiCtrl;
    DMX_WRITE_REG(DMX_PID_CTRL(ChanId), dmx_pid_ctrl.value);
}

HI_VOID DmxHalChannelSetCRCMode(HI_U32 ChanId, HI_UNF_DMX_CHAN_CRC_MODE_E CrcMode)
{
    U_DMX_PID_CTRL  dmx_pid_ctrl;
    HI_U32          mode;

    switch (CrcMode)
    {
        case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD :
            mode = 1;
            break;

        case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_SEND :
            mode = 2;
            break;

        case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD :
            mode = 3;
            break;

        case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_SEND :
            mode = 4;
            break;

        case HI_UNF_DMX_CHAN_CRC_MODE_FORBID :
        default :
            mode = 0;
    }

    dmx_pid_ctrl.value = DMX_READ_REG(DMX_PID_CTRL(ChanId));

    dmx_pid_ctrl.bits.crc_mode = mode;
    DMX_WRITE_REG(DMX_PID_CTRL(ChanId), dmx_pid_ctrl.value);
}

HI_VOID DmxHalChannelSetAFMode(HI_U32 ChanId)
{
    U_DMX_PID_CTRL dmx_pid_ctrl;

    dmx_pid_ctrl.value = DMX_READ_REG(DMX_PID_CTRL(ChanId));

    dmx_pid_ctrl.bits.af_mode = 1;
    DMX_WRITE_REG(DMX_PID_CTRL(ChanId), dmx_pid_ctrl.value);
}

HI_VOID DmxHalChannelSetCCDiscon(HI_U32 ChanId, HI_BOOL DiscardFlag)
{
    U_DMX_PID_CTRL dmx_pid_ctrl;

    dmx_pid_ctrl.value = DMX_READ_REG(DMX_PID_CTRL(ChanId));

    dmx_pid_ctrl.bits.cc_discon_ctrl = DiscardFlag;
    DMX_WRITE_REG(DMX_PID_CTRL(ChanId), dmx_pid_ctrl.value);
}

HI_VOID DmxHalChannelSetCCRepeatCtrl(HI_U32 ChanId, HI_U32 CCRepeatCtrl)
{
    U_DMX_PID_CTRL dmx_pid_ctrl;

    dmx_pid_ctrl.value = DMX_READ_REG(DMX_PID_CTRL(ChanId));

    dmx_pid_ctrl.bits.cc_equ_rve = CCRepeatCtrl;
    DMX_WRITE_REG(DMX_PID_CTRL(ChanId), dmx_pid_ctrl.value);
}

/***********************************************************************************
* Function      : DmxHalChannelSetPid
* Description   : set Channel pid
* Input         : ChanId, pid
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalChannelSetPid(HI_U32 ChanId, HI_U32 pid)
{
    U_DMX_PID_VALUE DmxPidValue;

    DmxPidValue.value = DMX_READ_REG(DMX_PID_VALUE(ChanId));

    DmxPidValue.bits.pid_value = pid;
    DMX_WRITE_REG(DMX_PID_VALUE(ChanId), DmxPidValue.value);
}

HI_VOID DmxHalChannelSetFilterMode(HI_U32 ChanId, HI_BOOL Enable)
{
    U_DMX_PID_REC_BUF PidRecBuf;

    PidRecBuf.value = DMX_READ_REG(DMX_PID_REC_BUF(ChanId));

    PidRecBuf.bits.flt_hit_mode = Enable;
    DMX_WRITE_REG(DMX_PID_REC_BUF(ChanId), PidRecBuf.value);
}

HI_VOID DmxHalChannelResetCounter(HI_U32 ChanId)
{
    DMX_WRITE_REG(CHANNEL_TS_COUNT(ChanId), 0);
}

HI_VOID DmxHalChannelStartFlush(HI_U32 ChanId)
{
    U_DMX_GLB_FLUSH GlbFlush;

    GlbFlush.value = DMX_READ_REG(DMX_GLB_FLUSH);

    GlbFlush.bits.flush_ch      = ChanId;
    GlbFlush.bits.flush_type    = 0x1;
    GlbFlush.bits.flush_cmd     = 0x1;
    DMX_WRITE_REG(DMX_GLB_FLUSH, GlbFlush.value);
}

HI_BOOL DmxHalChannelIsFlushDone(HI_VOID)
{
    U_DMX_GLB_FLUSH GlbFlush;

    GlbFlush.value = DMX_READ_REG(DMX_GLB_FLUSH);

    return GlbFlush.bits.flush_done ? HI_TRUE : HI_FALSE;
}

//-------------------------- FQ --------------------------
HI_VOID DmxHalFQClrUsedCount(HI_U32 FQId)
{
    U_ADDR_FQ_WORD0 FqWord0;

    FqWord0.value = DMX_READ_REG(ADDR_FQ_WORD0(FQId));

    FqWord0.bits.UsedCount = 0;
    DMX_WRITE_REG(ADDR_FQ_WORD0(FQId), FqWord0.value);
}

HI_VOID DmxHalFQSetAlmostOverflow(HI_U32 FQId, HI_U32 AlmostOverflow)
{
    U_ADDR_FQ_WORD0 FqWord0;

    FqWord0.value = DMX_READ_REG(ADDR_FQ_WORD0(FQId));

    FqWord0.bits.AlmostOverflow = AlmostOverflow;
    DMX_WRITE_REG(ADDR_FQ_WORD0(FQId), FqWord0.value);
}

HI_VOID DmxHalFQSetRead(HI_U32 FQId, HI_U32 Read)
{
    U_ADDR_FQ_WORD1 FqWord1;

    FqWord1.value = DMX_READ_REG(ADDR_FQ_WORD1(FQId));

    FqWord1.bits.Read = Read;
    DMX_WRITE_REG(ADDR_FQ_WORD1(FQId), FqWord1.value);
}

HI_VOID DmxHalFQClrFreeCount(HI_U32 FQId)
{
    U_ADDR_FQ_WORD1 FqWord1;

    FqWord1.value = DMX_READ_REG(ADDR_FQ_WORD1(FQId));

    FqWord1.bits.FreeCount = 0;
    DMX_WRITE_REG(ADDR_FQ_WORD1(FQId), FqWord1.value);
}

HI_U32 DmxHalFQGetWrite(HI_U32 FQId)
{
    U_ADDR_FQ_WORD2 FqWord2;

    FqWord2.value = DMX_READ_REG(ADDR_FQ_WORD2(FQId));

    return FqWord2.bits.Write;
}

HI_VOID DmxHalFQSetWrite(HI_U32 FQId, HI_U32 Write)
{
    U_ADDR_FQ_WORD2 FqWord2;

    FqWord2.value = DMX_READ_REG(ADDR_FQ_WORD2(FQId));

    FqWord2.bits.Write = Write;
    DMX_WRITE_REG(ADDR_FQ_WORD2(FQId), FqWord2.value);
}

HI_VOID DmxHalFQSetDepth(HI_U32 FQId, HI_U32 Depth)
{
    U_ADDR_FQ_WORD2 FqWord2;

    FqWord2.value = DMX_READ_REG(ADDR_FQ_WORD2(FQId));

    FqWord2.bits.Depth = Depth;
    DMX_WRITE_REG(ADDR_FQ_WORD2(FQId), FqWord2.value);
}

HI_VOID DmxHalFQSetStartAddr(HI_U32 FQId, HI_U32 StartAddr)
{
    DMX_WRITE_REG(ADDR_FQ_WORD3(FQId), StartAddr);
}

HI_VOID DmxHalFQEnableRecive(HI_U32 FQId)
{
    HI_U32  offset  = FQId >> 5;
    HI_U32  bit     = FQId & 0x1f;
    HI_U32  value;

    value = DMX_READ_REG(FQ_ENA_0(offset));

    value |= 1 << bit;
    DMX_WRITE_REG(FQ_ENA_0(offset), value);
}

//-------------------------- OQ --------------------------
HI_VOID DmxHalOQSetEopResByte(HI_U32 OQId, HI_U32 Byte)
{
    U_ADDR_OQ_WORD0 oq_word0;

    oq_word0.value = DMX_READ_REG(ADDR_OQ_WORD0(OQId));

    oq_word0.bits.EopResByte = Byte;
    DMX_WRITE_REG(ADDR_OQ_WORD0(OQId), oq_word0.value);
}

HI_VOID DmxHalOQSetWResByte(HI_U32 OQId, HI_U32 Byte)
{
    U_ADDR_OQ_WORD1 oq_word1;

    oq_word1.value = DMX_READ_REG(ADDR_OQ_WORD1(OQId));

    oq_word1.bits.WResByte = Byte;
    DMX_WRITE_REG(ADDR_OQ_WORD1(OQId), oq_word1.value);
}

HI_U32 DmxHalOQGetPvrCtrl(HI_U32 OQId)
{
    U_ADDR_OQ_WORD1 oq_word1;

    oq_word1.value = DMX_READ_REG(ADDR_OQ_WORD1(OQId));

    return oq_word1.bits.PVRCtrl;
}

HI_VOID DmxHalOQSetPvrCtrl(HI_U32 OQId, HI_U32 Val)
{
    U_ADDR_OQ_WORD1 oq_word1;

    oq_word1.value = DMX_READ_REG(ADDR_OQ_WORD1(OQId));

    oq_word1.bits.PVRCtrl = Val;
    DMX_WRITE_REG(ADDR_OQ_WORD1(OQId), oq_word1.value);
}

HI_U32 DmxHalOQGetEopAddr(HI_U32 OQId)
{
    U_ADDR_OQ_WORD2 oq_word2;

    oq_word2.value = DMX_READ_REG(ADDR_OQ_WORD2(OQId));

    return oq_word2.bits.BBEopAddr;
}

HI_VOID DmxHalOQSetEopAddr(HI_U32 OQId, HI_U32 addr)
{
    U_ADDR_OQ_WORD2 oq_word2;

    oq_word2.value = DMX_READ_REG(ADDR_OQ_WORD2(OQId));

    oq_word2.bits.BBEopAddr = addr;
    DMX_WRITE_REG(ADDR_OQ_WORD2(OQId), oq_word2.value);
}

HI_VOID DmxHalOQSetBBWaddr(HI_U32 OQId, HI_U32 addr)
{
    U_ADDR_OQ_WORD2 oq_word2;

    oq_word2.value = DMX_READ_REG(ADDR_OQ_WORD2(OQId));

    oq_word2.bits.BBWaddr = addr;
    DMX_WRITE_REG(ADDR_OQ_WORD2(OQId), oq_word2.value);
}

HI_U32 DmxHalOQGetBBSize(HI_U32 OQId)
{
    U_ADDR_OQ_WORD3 oq_word3;

    oq_word3.value = DMX_READ_REG(ADDR_OQ_WORD3(OQId));

    return oq_word3.bits.BBSize;
}

HI_VOID DmxHalOQSetBBSize(HI_U32 OQId, HI_U32 Size)
{
    U_ADDR_OQ_WORD3 oq_word3;

    oq_word3.value = DMX_READ_REG(ADDR_OQ_WORD3(OQId));

    oq_word3.bits.BBSize = Size;
    DMX_WRITE_REG(ADDR_OQ_WORD3(OQId), oq_word3.value);
}

HI_VOID DmxHalOQSetBQVal(HI_U32 OQId, HI_U32 Val)
{
    U_ADDR_OQ_WORD0 oq_word0;
    U_ADDR_OQ_WORD3 oq_word3;

    oq_word0.value = DMX_READ_REG(ADDR_OQ_WORD0(OQId));
    oq_word3.value = DMX_READ_REG(ADDR_OQ_WORD3(OQId));

    oq_word0.bits.BQVal = Val & 0xFF;
    oq_word3.bits.BQVal = Val >> 8;

    DMX_WRITE_REG(ADDR_OQ_WORD0(OQId), oq_word0.value);
    DMX_WRITE_REG(ADDR_OQ_WORD3(OQId), oq_word3.value);
}

HI_VOID DmxHalOQSetBQUse(HI_U32 OQId, HI_U32 Use)
{
    U_ADDR_OQ_WORD3 oq_word3;

    oq_word3.value = DMX_READ_REG(ADDR_OQ_WORD3(OQId));

    oq_word3.bits.BQUse = Use;
    DMX_WRITE_REG(ADDR_OQ_WORD3(OQId), oq_word3.value);
}

HI_U32 DmxHalOQGetBBSAddr(HI_U32 OQId)
{
    return DMX_READ_REG(ADDR_OQ_WORD4(OQId));
}

HI_VOID DmxHalOQSetBBSAddr(HI_U32 OQId, HI_U32 addr)
{
    DMX_WRITE_REG(ADDR_OQ_WORD4(OQId), addr);
}

HI_U32 DmxHalOQGetRead(HI_U32 OQId)
{
    U_ADDR_OQ_WORD5 oq_word5;

    oq_word5.value = DMX_READ_REG(ADDR_OQ_WORD5(OQId));

    return oq_word5.bits.BQRPtr;
}

HI_VOID DmxHalOQSetRead(HI_U32 OQId, HI_U32 Read)
{
    U_ADDR_OQ_WORD5 oq_word5;

    oq_word5.value = DMX_READ_REG(ADDR_OQ_WORD5(OQId));

    oq_word5.bits.BQRPtr = Read;
    DMX_WRITE_REG(ADDR_OQ_WORD5(OQId), oq_word5.value);
}

HI_VOID DmxHalOQSetIntCount(HI_U32 OQId, HI_U32 Count)
{
    U_ADDR_OQ_WORD5 oq_word5;

    oq_word5.value = DMX_READ_REG(ADDR_OQ_WORD5(OQId));

    oq_word5.bits.BQIntCnt = Count;
    DMX_WRITE_REG(ADDR_OQ_WORD5(OQId), oq_word5.value);
}

HI_U32 DmxHalOQGetWrite(HI_U32 OQId)
{
    U_ADDR_OQ_WORD5 oq_word5;

    oq_word5.value = DMX_READ_REG(ADDR_OQ_WORD5(OQId));

    return oq_word5.bits.BQWPtr;
}

HI_VOID DmxHalOQSetWrite(HI_U32 OQId, HI_U32 Write)
{
    U_ADDR_OQ_WORD5 oq_word5;

    oq_word5.value = DMX_READ_REG(ADDR_OQ_WORD5(OQId));

    oq_word5.bits.BQWPtr = Write;
    DMX_WRITE_REG(ADDR_OQ_WORD5(OQId), oq_word5.value);
}

HI_VOID DmxHalOQSetIntCfg(HI_U32 OQId, HI_U32 Count)
{
    U_ADDR_OQ_WORD6 oq_word6;

    oq_word6.value = DMX_READ_REG(ADDR_OQ_WORD6(OQId));

    oq_word6.bits.BQIntCfg = Count;
    DMX_WRITE_REG(ADDR_OQ_WORD6(OQId), oq_word6.value);
}

HI_VOID DmxHalOQSetDepth(HI_U32 OQId, HI_U32 Depth)
{
    U_ADDR_OQ_WORD6 oq_word6;

    oq_word6.value = DMX_READ_REG(ADDR_OQ_WORD6(OQId));

    oq_word6.bits.BQSize = Depth;
    DMX_WRITE_REG(ADDR_OQ_WORD6(OQId), oq_word6.value);
}

HI_VOID DmxHalOQSetAlmostOverflow(HI_U32 OQId, HI_U32 AlmostOverflow)
{
    U_ADDR_OQ_WORD6 oq_word6;

    oq_word6.value = DMX_READ_REG(ADDR_OQ_WORD6(OQId));

    oq_word6.bits.BQAlovfl_TH = AlmostOverflow;
    DMX_WRITE_REG(ADDR_OQ_WORD6(OQId), oq_word6.value);
}

HI_VOID DmxHalOQSetFQId(HI_U32 OQId, HI_U32 FQId)
{
    U_ADDR_OQ_WORD6 oq_word6;

    oq_word6.value = DMX_READ_REG(ADDR_OQ_WORD6(OQId));

    oq_word6.bits.FqCfg = FQId;
    DMX_WRITE_REG(ADDR_OQ_WORD6(OQId), oq_word6.value);
}

HI_VOID DmxHalOQSetStartAddr(HI_U32 OQId, HI_U32 StartAddr)
{
    DMX_WRITE_REG(ADDR_OQ_WORD7(OQId), StartAddr);
}

HI_VOID DmxHalOQResetCounter(HI_U32 OQId)
{
    DMX_WRITE_REG(ADDR_INT_CNT(OQId), 0);
}

HI_VOID DmxHalOQEnableRecive(HI_U32 OQId, HI_BOOL Enable)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;

    value = DMX_READ_REG(OQ_ENA_0(offset));

    if (Enable)
    {
        value |= 1 << bit;
    }
    else
    {
        value &= ~(1 << bit);
    }
    DMX_WRITE_REG(OQ_ENA_0(offset), value);
}

HI_VOID DmxHalOQStartFlush(HI_U32 OQId)
{
    U_CLR_CHN_CMD ClrChnCmd;

    ClrChnCmd.value = DMX_READ_REG(CLR_CHN_CMD);

    ClrChnCmd.bits.clear_chn    = OQId;
    ClrChnCmd.bits.clear_type   = 0;
    ClrChnCmd.bits.clear_start  = 1;
    DMX_WRITE_REG(CLR_CHN_CMD, ClrChnCmd.value);
}

HI_BOOL DmxHalOQIsFlushDone(HI_VOID)
{
    U_IRAW_CLR_CHN clear_ch;

    clear_ch.u32 = DMX_READ_REG(RAW_CLR_CHN);
    if (clear_ch.bits.iraw_clr_chn)
    {
        DMX_WRITE_REG(RAW_CLR_CHN, clear_ch.u32);
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

//-------------------------- Filter --------------------------
HI_VOID DmxHalFilterSet(HI_U32 FilterId, HI_U32 Depth, HI_U8 Content, HI_U32 Reverse, HI_U8 Mask)
{
    U_DMX_FILTER dmx_flt;

    dmx_flt.value = 0;

    dmx_flt.bits.wdata_mask     = Mask;
    dmx_flt.bits.wdata_content  = Content;
    dmx_flt.bits.wdata_mode     = Reverse;

    DMX_WRITE_REG(DMX_FILTERxy(FilterId, Depth), dmx_flt.value);
}

HI_VOID DmxHalFilterAttach(HI_U32 FilterId, HI_U32 ChanId)
{
    HI_U32  filter_en = 1 << (FilterId & 0x1F);
#ifndef DMX_REGION_SUPPORT
    HI_U32  value;
    HI_U32  offset;
    HI_U32  i;

    value = DMX_READ_REG(DMX_FILTER_EN(ChanId));
    for (i = 0; i < 32; i++)
    {
        if (0 == (value & (1 << i)))
        {
            break;
        }
    }

    value = DMX_READ_REG(DMX_FILTER_ID(ChanId, i));

    offset = (i & 0x3) * 8;

    value &= ~(0xFF << offset);
    value |= FilterId << offset;

    DMX_WRITE_REG(DMX_FILTER_ID(ChanId, i), value);

    filter_en = (1 << i);
#endif
    filter_en |= DMX_READ_REG(DMX_FILTER_EN(ChanId));

    DMX_WRITE_REG(DMX_FILTER_EN(ChanId), filter_en);
}

HI_VOID DmxHalFilterDetach(HI_U32 FilterId, HI_U32 ChanId)
{
    HI_U32  filter_en = ~(1 << (FilterId & 0x1F));
#ifndef DMX_REGION_SUPPORT
    HI_U32  value;
    HI_U32  i;

    for (i = 0; i < 8; i++)
    {
        HI_U32 j;

        value = DMX_READ_REG(DMX_FILTER_ID(ChanId, i * 4));

        for (j = 0; j < 4; j++)
        {
            HI_U32 Id = (value >> (j * 8)) & 0xff;

            if (Id == FilterId)
            {
                break;
            }
        }

        if (j < 4)
        {
            filter_en = ~(1 << (i * 4 + j));

            break;
        }
    }
#endif
    filter_en &= DMX_READ_REG(DMX_FILTER_EN(ChanId));

    DMX_WRITE_REG(DMX_FILTER_EN(ChanId), filter_en);
}

