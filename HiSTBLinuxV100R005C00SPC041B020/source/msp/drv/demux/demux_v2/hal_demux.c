/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  hal_demux.c
* Description:  Define interfaces of demux hardware abstract layer.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20090927    y00106256      NULL      Create this file.
***********************************************************************************/

#include "hi_type.h"
#include "hi_module.h"
#include "hi_reg_common.h"

#include "demux_debug.h"
#include "drv_demux_reg.h"
#include "drv_demux_define.h"
#include "drv_demux_osal.h"
#include "hal_demux.h"

//#include "basedef.h"
#include "hi_drv_struct.h"
#include "hi_drv_sys.h"
#include <asm/barrier.h>    /*wmb() */ //we added this for DTS2013091204261
#include <linux/highmem.h>

#if 1
#define DMX_COM_EQUAL(exp, act)
#else
#define DMX_COM_EQUAL(exp, act)                                                         \
    do                                                                                  \
    {                                                                                   \
        if (exp != act)                                                                 \
        {                                                                               \
            HI_ERR_DEMUX("Write register error, exp=0x%x, act=0x%x\n", exp, act);       \
        }                                                                               \
    } while (0)
#endif

/*demux 中对于寄存器操作存在竞争，可能会引起莫名奇妙的错误，review 所有寄存器，统一修改
问题单号: DTS2013082001104 */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
spinlock_t DmxHalLock = SPIN_LOCK_UNLOCKED;
#else
spinlock_t DmxHalLock = __SPIN_LOCK_UNLOCKED(DmxHalLock);
#endif

#define DEMUX_MAP_DDR_PHYADDRESS(phys)      kmap((phys_to_page(phys)))
#define DEMUX_UMMAP_DDR_PHYADDRESS(phys)    kunmap((phys_to_page(phys)))


HI_U32 DmxHalGetDmxClk(Dmx_Set_S *DmxSet)
{
    U_DBG_DETECT0 DbgDetect;

    DbgDetect.u32 = DMX_READ_REG(DmxSet->IoBase, ADDR_DBG_DETECT0);

    return DbgDetect.bits.dbg_out_sample_freq;
}

/***********************************************************************************
* Function      : DmxHalTSOPortSetAttr
* Description   : Set TSO Port
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalTSOPortSetAttr(Dmx_Set_S *DmxSet, HI_U32 PortId,HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr)
{
    U_TSOUT_CFG tsout_cfg;
    HI_U32 u32TSOSourcePortID = 0;

    BUG_ON(PortId >= DmxSet->TSOPortCnt);

    if ( PortAttr->enTSSource >= HI_UNF_DMX_PORT_RAM_0 )
    {
        u32TSOSourcePortID = PortAttr->enTSSource - HI_UNF_DMX_PORT_RAM_0 + 0x10;
    }
    else if(PortAttr->enTSSource >= HI_UNF_DMX_PORT_TSI_0)
    {
        u32TSOSourcePortID = PortAttr->enTSSource - HI_UNF_DMX_PORT_TSI_0 + DMX_IFPORT_CNT + 0x1;
    }
    else
    {
        u32TSOSourcePortID = PortAttr->enTSSource + 0x1;
    }

    tsout_cfg.u32                   = DMX_READ_REG(DmxSet->IoBase, TSOUT_CFG(PortId));
    tsout_cfg.bits.dis              = PortAttr->bEnable? 0:1;
    tsout_cfg.bits.tsout0_press_en = 1;  /* enable tso backpress ram port if tso source port is ram port. */
    tsout_cfg.bits.tsout_source_sel = u32TSOSourcePortID;
    tsout_cfg.bits.clkgt_mode       = (PortAttr->enClkMode == HI_UNF_DMX_TSO_CLK_MODE_JITTER)?1:0;
    tsout_cfg.bits.vld_mode         = (PortAttr->enValidMode == HI_UNF_DMX_TSO_VALID_ACTIVE_HIGH)?1:0;
    tsout_cfg.bits.sync_pos         = (!PortAttr->bBitSync)?1:0;
    tsout_cfg.bits.spi_mode         = (!PortAttr->bSerial)?1:0;
    tsout_cfg.bits.serial_bit_sel   = (PortAttr->enBitSelector == HI_UNF_DMX_TSO_SERIAL_BIT_7)?1:0;
    if ( tsout_cfg.bits.serial_bit_sel == 0x1 )
    {
        tsout_cfg.bits.byte_endian  = (PortAttr->bLSB )?1:0;
    }
    else
    {
        tsout_cfg.bits.byte_endian  = (PortAttr->bLSB )?0:1;
    }

    DMX_WRITE_REG(DmxSet->IoBase, TSOUT_CFG(PortId), tsout_cfg.u32);
    DMX_COM_EQUAL(tsout_cfg.u32, DMX_READ_REG(DmxSet->IoBase, TSOUT_CFG(PortId)));
}

/***********************************************************************************
* Function      : DmxHalDvbPortSetAttr
* Description   : Set Tuner Port
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDvbPortSetAttr(
        Dmx_Set_S           *DmxSet,
        HI_U32                  RawPortId,
        HI_UNF_DMX_PORT_TYPE_E  PortType,
        HI_U32                  SyncOn,
        HI_U32                  SyncOff,
        HI_U32                  TunerInClk,
        HI_U32                  BitSelector
    )
{
    U_TS_INTERFACE  ts_interface;

    BUG_ON(RawPortId >= DmxSet->TunerPortCnt);

    ts_interface.u32 = DMX_READ_REG(DmxSet->IoBase, TS_INTERFACE(RawPortId));

    ts_interface.bits.port_sel      = 0;
    ts_interface.bits.sync_clear    = 1;
    DMX_WRITE_REG(DmxSet->IoBase, TS_INTERFACE(RawPortId), ts_interface.u32);

    DMX_COM_EQUAL(ts_interface.u32, DMX_READ_REG(DmxSet->IoBase, TS_INTERFACE(RawPortId)));

    msleep(2);

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

    ts_interface.bits.bit_sel = BitSelector;

    ts_interface.bits.syncon_th     = SyncOn;
    ts_interface.bits.syncoff_th    = SyncOff;
    ts_interface.bits.sync_clear    = 0;

    DMX_WRITE_REG(DmxSet->IoBase, TS_INTERFACE(RawPortId), ts_interface.u32);

    DMX_COM_EQUAL(ts_interface.u32, DMX_READ_REG(DmxSet->IoBase, TS_INTERFACE(RawPortId)));

    ts_interface.bits.port_sel = 1;
    DMX_WRITE_REG(DmxSet->IoBase, TS_INTERFACE(RawPortId), ts_interface.u32);

    DMX_COM_EQUAL(ts_interface.u32, DMX_READ_REG(DmxSet->IoBase, TS_INTERFACE(RawPortId)));
}

HI_VOID DmxHalDvbPortSetClkInPol(HI_U32 RawPortId, HI_BOOL Pol)
{
    U_PERI_CRG63 PeriCrg63;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    switch (RawPortId)
    {
#if    defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)
        case 0 :
            /*pvr_tsi1_pctrl acturally is point to tsi0,the name tsi1 because logic code use this name ,in logic code ,
            tsi begin from tsi1 */
            PeriCrg63.bits.pvr_tsi1_pctrl = Pol;
            break;

        case 1 :
            PeriCrg63.bits.pvr_tsi2_pctrl = Pol;
            break;

        default:
            WARN(1, "%s: Invalid TSI Port ID(%d).\n", __func__, RawPortId);
            return;

#elif  defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3719cv100)
        case 1 :
            /*pvr_tsi2_pctrl acturally is point to tsi0,the name tsi2 because logic code use this name ,in logic code ,
            tsi begin from tsi1 (inner QAM)*/
            PeriCrg63.bits.pvr_tsi2_pctrl = Pol;
            break;

        case 2 :
            PeriCrg63.bits.pvr_tsi3_pctrl = Pol;
            break;

        case 3 :
            PeriCrg63.bits.pvr_tsi4_pctrl = Pol;
            break;

        case 4 :
            PeriCrg63.bits.pvr_tsi5_pctrl = Pol;
            break;
#if defined(CHIP_TYPE_hi3716mv420)
        case 5:
            PeriCrg63.bits.pvr_tsi6_pctrl = Pol;
            break;
        case 6:
            PeriCrg63.bits.pvr_tsi7_pctrl = Pol;
            break;
#endif
        default:
            WARN(1, "%s: Invalid TSI Port ID(%d).\n", __func__, RawPortId);
            return;

/*for 3751v100 (S5), we have 2 inside QAM port， so , TSI port ID begin from 2 in driver code*/
#elif defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)
        case 2 :
            /*pvr_tsi3_pctrl acturally is point to tsi0,the name tsi3 because logic code use this name ,in logic code ,
            tsi begin from tsi3 ( 2 inner QAM)*/
            PeriCrg63.bits.pvr_tsi3_pctrl = Pol;
            break;

        case 3 :
            PeriCrg63.bits.pvr_tsi4_pctrl = Pol;
            break;

        case 4 :
            PeriCrg63.bits.pvr_tsi5_pctrl = Pol;
            break;

        case 5 :
            PeriCrg63.bits.pvr_tsi6_pctrl = Pol;
            break;

        case 6 :
            PeriCrg63.bits.pvr_tsi7_pctrl = Pol;
            break;

        case 7 :
            PeriCrg63.bits.pvr_tsi8_pctrl = Pol;
            break;

        default:
            WARN(1, "%s: Invalid TSI Port ID(%d).\n", __func__, RawPortId);
            return;
 #elif defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)
        case 0:
            PeriCrg63.bits.pvr_tsi1_pctrl = Pol;
            break;

        case 1:
            PeriCrg63.bits.pvr_tsi2_pctrl = Pol;
            break;

        case 2:
            PeriCrg63.bits.pvr_tsi3_pctrl = Pol;
            break;

        case 3:
            PeriCrg63.bits.pvr_tsi4_pctrl = Pol;
            break;

#if defined(CHIP_TYPE_hi3798cv200)
        case 4:
            PeriCrg63.bits.pvr_tsi5_pctrl = Pol;
            break;

        case 5:
            PeriCrg63.bits.pvr_tsi6_pctrl = Pol;
            break;
#endif

        default:
            WARN(1, "%s: Invalid TSI Port ID(%d).\n", __func__, RawPortId);
            return;
#elif defined(CHIP_TYPE_hi3796mv200)
        case 1:
            PeriCrg63.bits.pvr_tsi2_pctrl = Pol;
            break;

        case 2:
            PeriCrg63.bits.pvr_tsi3_pctrl = Pol;
            break;

        case 3:
            PeriCrg63.bits.pvr_tsi4_pctrl = Pol;
            break;

        case 4:
            PeriCrg63.bits.pvr_tsi5_pctrl = Pol;
            break;

        case 5:
            PeriCrg63.bits.pvr_tsi6_pctrl = Pol;
            break;

        case 6:
            PeriCrg63.bits.pvr_tsi7_pctrl = Pol;
            break;

        default:
            WARN(1, "%s: Invalid TSI Port ID(%d).\n", __func__, RawPortId);
            return;
#else
        default :
            return;
#endif
    }

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;
}

/***********************************************************************************
* Function      : DmxHalDvbPortGetShareClk
* Description   : Get Share Clk Port
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_S32 DmxHalDvbPortGetShareClk(HI_U32 PortID, HI_UNF_DMX_PORT_E *pSerialPortShareClk)
{
#if defined(CHIP_TYPE_hi3716mv410) \
 || defined(CHIP_TYPE_hi3716mv420)

    U_PERI_CRG63 PeriCrg63;
    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    switch (PortID)
    {
        case 0x3:
            *pSerialPortShareClk = ((0x1 == PeriCrg63.bits.pvr_tsi3_sel) ? HI_UNF_DMX_PORT_TSI_2: HI_UNF_DMX_PORT_BUTT);
            break;

        case 0x4:
            *pSerialPortShareClk = ((0x1 == PeriCrg63.bits.pvr_tsi4_sel) ? HI_UNF_DMX_PORT_TSI_2: HI_UNF_DMX_PORT_BUTT);
            break;

        case 0x5:
            *pSerialPortShareClk = ((0x1 == PeriCrg63.bits.pvr_tsi5_sel) ? HI_UNF_DMX_PORT_TSI_2: HI_UNF_DMX_PORT_BUTT);
            break;

        default:
            *pSerialPortShareClk = HI_UNF_DMX_PORT_BUTT; /* use original clock */
            HI_WARN_DEMUX("Use default clk, PortID(%d).\n", PortID);
            break;
    }

#elif defined(CHIP_TYPE_hi3798cv200)

    U_PERI_CRG64 PeriCrg64;

    if(PortID > 0x3)
    {
        *pSerialPortShareClk = HI_UNF_DMX_PORT_BUTT;/* use original clock */
        HI_WARN_DEMUX("Use default clk.\n");
        return HI_SUCCESS;
    }

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    switch (PeriCrg64.bits.clk_tsi03_com_sel)
    {
        case 0x4 : /* b100 */
            *pSerialPortShareClk = HI_UNF_DMX_PORT_TSI_0;
            break;

        case 0x5 : /* b101 */
            *pSerialPortShareClk = HI_UNF_DMX_PORT_TSI_1;
            break;

        case 0x6 : /* b110 */
            *pSerialPortShareClk = HI_UNF_DMX_PORT_TSI_2;
            break;

        case 0x7 : /* b111 */
            *pSerialPortShareClk = HI_UNF_DMX_PORT_TSI_3;
            break;

        default:
            *pSerialPortShareClk = HI_UNF_DMX_PORT_BUTT;
            HI_WARN_DEMUX("Use default clk.\n");
            break;
    }

#elif defined(CHIP_TYPE_hi3798mv200)   \
   || defined(CHIP_TYPE_hi3798mv200_a) \
   || defined(CHIP_TYPE_hi3796mv200)

    U_PERI_CRG64 PeriCrg64;

    if(PortID > 0x3 || 0x2 == PortID)
    {
        *pSerialPortShareClk = HI_UNF_DMX_PORT_BUTT; /* use original clock */
        HI_WARN_DEMUX("Use default clk.\n");
        return HI_SUCCESS;
    }

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    switch (PeriCrg64.bits.pvr_tsi_sel)
    {
        case 0x1:
            *pSerialPortShareClk = HI_UNF_DMX_PORT_TSI_2; /* clk_tsi0,1,3 share tsi2 clock */
            break;

        default:
            *pSerialPortShareClk = HI_UNF_DMX_PORT_BUTT; /* use original clock */
            HI_WARN_DEMUX("Use default clk.\n");
            break;
    }

#else
    HI_WARN_DEMUX("Not support share clock.\n");
    return HI_SUCCESS;
#endif
    return HI_SUCCESS;
}

/***********************************************************************************
* Function      : DmxHalDvbPortSetShareClk
* Description   : Set Share Clk Port
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDvbPortSetShareClk(HI_U32 PortID, HI_UNF_DMX_PORT_E SerialPortShareClk)
{
#if defined(CHIP_TYPE_hi3716mv410) \
 || defined(CHIP_TYPE_hi3716mv420)

    U_PERI_CRG63 PeriCrg63;
    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    switch (SerialPortShareClk)
    {
        case HI_UNF_DMX_PORT_TSI_2:
            if (0x3 == PortID)
            {
                PeriCrg63.bits.pvr_tsi3_sel = 0x1; /* clk_tsi3 share tsi2 clock */
            }

            if (0x4 == PortID)
            {
                PeriCrg63.bits.pvr_tsi4_sel = 0x1; /* clk_tsi4 share tsi2 clock */
            }

            if (0x5 == PortID)
            {
                PeriCrg63.bits.pvr_tsi5_sel = 0x1; /* clk_tsi5 share tsi2 clock */
            }
            break;

        default:
            if (0x3 == PortID)
            {
                PeriCrg63.bits.pvr_tsi3_sel = 0x0; /* clk_tsi3 original clock */
            }

            if (0x4 == PortID)
            {
                PeriCrg63.bits.pvr_tsi4_sel = 0x0; /* clk_tsi4 original clock */
            }

            if (0x5 == PortID)
            {
                PeriCrg63.bits.pvr_tsi5_sel = 0x0; /* clk_tsi5 original clock */
            }
            HI_WARN_DEMUX("Set default clk, ShardClkType(%d).\n", SerialPortShareClk);
            break;
    }
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;
    return;

#elif defined(CHIP_TYPE_hi3798cv200)

    U_PERI_CRG64 PeriCrg64;
    if(PortID > 0x3)
    {
        HI_WARN_DEMUX("Use default clk, PortID(%d).\n", PortID);
        return;
    }

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    switch (SerialPortShareClk)
    {
        case HI_UNF_DMX_PORT_TSI_0:
            PeriCrg64.bits.clk_tsi03_com_sel = 0x4; /* clk_tsi03 share tsi0 clock */
            break;

        case HI_UNF_DMX_PORT_TSI_1:
            PeriCrg64.bits.clk_tsi03_com_sel = 0x5; /* clk_tsi03 share tsi1 clock */
            break;

        case HI_UNF_DMX_PORT_TSI_2:
            PeriCrg64.bits.clk_tsi03_com_sel = 0x6; /* clk_tsi03 share tsi2 clock */
            break;

        case HI_UNF_DMX_PORT_TSI_3:
            PeriCrg64.bits.clk_tsi03_com_sel = 0x7; /* clk_tsi03 share tsi3 clock */
            break;

        default:
            PeriCrg64.bits.clk_tsi03_com_sel = 0x0; /* clk_tsi03 use original clock */
            HI_WARN_DEMUX("Set default clk, ShardClkType(%d).\n", SerialPortShareClk);
            break;
    }
    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
    return;

#elif defined(CHIP_TYPE_hi3798mv200)   \
   || defined(CHIP_TYPE_hi3798mv200_a) \
   || defined(CHIP_TYPE_hi3796mv200)

    U_PERI_CRG64 PeriCrg64;

    if(PortID > 0x3 || 0x2 == PortID)
    {
        HI_WARN_DEMUX("Use default clk, PortID(%d).\n", PortID);
        return;
    }

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    switch (SerialPortShareClk)
    {
        case HI_UNF_DMX_PORT_TSI_2:
            PeriCrg64.bits.pvr_tsi_sel = 0x1;/* clk_tsi0,1,3 share tsi2 clock */
            break;

        default:
            PeriCrg64.bits.pvr_tsi_sel = 0x0;/* use original clock */
            HI_WARN_DEMUX("Set default clk, ShardClkType(%d).\n", SerialPortShareClk);
            break;
    }
    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
    return;

#else
    HI_WARN_DEMUX("Not support share clock, ShardClkType(%d).\n", SerialPortShareClk);
    return;
#endif
}


/***********************************************************************************
* Function      : DmxHalDvbPortSetTsCountCtrl
* Description   : Set TS Count Ctrl
* Input         : PortId, option
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDvbPortSetTsCountCtrl(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_U32 option)
{
    U_TS_COUNT_CTRL ts_count_ctrl;

    BUG_ON(RawPortId >= DmxSet->TunerPortCnt);

    ts_count_ctrl.u32 = DMX_READ_REG(DmxSet->IoBase, TS_COUNT_CTRL(RawPortId));

    ts_count_ctrl.bits.ts_count_ctrl = option;
    DMX_WRITE_REG(DmxSet->IoBase, TS_COUNT_CTRL(RawPortId), ts_count_ctrl.u32);

    DMX_COM_EQUAL(ts_count_ctrl.u32, DMX_READ_REG(DmxSet->IoBase, TS_COUNT_CTRL(RawPortId)));
}

/***********************************************************************************
* Function      : DmxHalDvbPortGetTsPackCount
* Description   : Get TS Pack Counter
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalDvbPortGetTsPackCount(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    return DMX_READ_REG(DmxSet->IoBase, TS_COUNT(RawPortId));
}

/***********************************************************************************
* Function      : DmxHalDvbPortSetErrTsCountCtrl
* Description   : Set ETS Count Ctrl
* Input         : PortId, option
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDvbPortSetErrTsCountCtrl(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_U32 option)
{
    U_ETS_COUNT_CTRL ets_count_ctrl;

    BUG_ON(RawPortId >= DmxSet->TunerPortCnt);

    ets_count_ctrl.u32 = DMX_READ_REG(DmxSet->IoBase, ETS_COUNT_CTRL(RawPortId));

    ets_count_ctrl.bits.ets_count_ctrl = option;
    DMX_WRITE_REG(DmxSet->IoBase, ETS_COUNT_CTRL(RawPortId), ets_count_ctrl.u32);

    DMX_COM_EQUAL(ets_count_ctrl.u32, DMX_READ_REG(DmxSet->IoBase, ETS_COUNT_CTRL(RawPortId)));
}

/***********************************************************************************
* Function      : DmxHalDvbPortGetErrTsPackCount
* Description   : Get Error TS Pack Count
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalDvbPortGetErrTsPackCount(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    return DMX_READ_REG(DmxSet->IoBase, ETS_COUNT(RawPortId));
}

HI_VOID DmxHalIPPortSetAttr(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_UNF_DMX_PORT_TYPE_E PortType, HI_U32 SyncOn, HI_U32 SyncOff)
{
    U_IP_SYNC_TH_CFG ipsync_cfg;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    ipsync_cfg.u32 = DMX_READ_REG(DmxSet->IoBase, IP_SYNC_TH_CFG(RawPortId));

    switch (PortType)
    {
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188 :
            ipsync_cfg.bits.ip_sync_type = 0;
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204 :
            ipsync_cfg.bits.ip_sync_type = 1;
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204 :
        case HI_UNF_DMX_PORT_TYPE_USER_DEFINED :
            ipsync_cfg.bits.ip_sync_type = 2;
            break;

    #ifdef DMX_RAM_PORT_AUTO_SCAN_SUPPORT
        case HI_UNF_DMX_PORT_TYPE_AUTO :
            ipsync_cfg.bits.ip_sync_type = 3;
            break;
    #endif

        default :
            ipsync_cfg.bits.ip_sync_type = 2;
    }

    ipsync_cfg.bits.ip_sync_th = SyncOn;
    ipsync_cfg.bits.ip_loss_th = SyncOff;

    DMX_WRITE_REG(DmxSet->IoBase, IP_SYNC_TH_CFG(RawPortId), ipsync_cfg.u32);

    DMX_COM_EQUAL(ipsync_cfg.u32, DMX_READ_REG(DmxSet->IoBase, IP_SYNC_TH_CFG(RawPortId)));
}

HI_VOID DmxHalIPPortSetSyncLen(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 SyncLen1, HI_U32 SyncLen2)
{
    U_DMX_SYNC_LEN_SET SyncSet;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    SyncSet.value = DMX_READ_REG(DmxSet->IoBase, IP_SYNC_LEN(RawPortId));
    SyncSet.bits.ip_nosync_len1 = SyncLen1;
    SyncSet.bits.ip_nosync_len2 = SyncLen2;

    DMX_WRITE_REG(DmxSet->IoBase, IP_SYNC_LEN(RawPortId), SyncSet.value);

    DMX_COM_EQUAL(SyncSet.value, DMX_READ_REG(DmxSet->IoBase, IP_SYNC_LEN(RawPortId)));
}

#ifdef DMX_RAM_PORT_AUTO_SCAN_SUPPORT
HI_VOID DmxHalIPPortSetAutoScanRegion(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 len, HI_U32 step)
{
    U_DMX_SYNC_LEN_SET SyncSet;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    SyncSet.value = DMX_READ_REG(DmxSet->IoBase, IP_SYNC_LEN(RawPortId));
    SyncSet.bits.ip_nosync_region   = len;
    SyncSet.bits.ip_nosync_step     = step;

    DMX_WRITE_REG(DmxSet->IoBase, IP_SYNC_LEN(RawPortId), SyncSet.value);

    DMX_COM_EQUAL(SyncSet.value, DMX_READ_REG(DmxSet->IoBase, IP_SYNC_LEN(RawPortId)));
}
#endif

/***********************************************************************************
* Function      : DmxHalIPPortSetTsCountCtrl
* Description   : Set TS Pack Count
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalIPPortSetTsCountCtrl(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_BOOL enable)
{
    U_IP_DBG_CNT_EN ip_count_en;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    ip_count_en.u32 = DMX_READ_REG(DmxSet->IoBase, IP_DBG_CNT_EN(RawPortId));

    ip_count_en.bits.ip_dbg_cnt_en = enable;
    DMX_WRITE_REG(DmxSet->IoBase, IP_DBG_CNT_EN(RawPortId), ip_count_en.u32);

    DMX_COM_EQUAL(ip_count_en.u32, DMX_READ_REG(DmxSet->IoBase, IP_DBG_CNT_EN(RawPortId)));
}

/***********************************************************************************
* Function      : DmxHalIPPortGetTsPackCount
* Description   : Get TS Pack Counter Status
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalIPPortGetTsPackCount(Dmx_Set_S *DmxSet, HI_U32 PortId)
{
    HI_U32 RawRamId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    BUG_ON(DMX_INVALID_PORT_ID == RawRamId);

    return DMX_READ_REG(DmxSet->IoBase, IP_DBG_OUT1(RawRamId));
}

/***********************************************************************************
* Function      : DmxHalIPPortStartStream
* Description   : Start pushing stream
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID __DmxHalIPPortStartStream(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_BOOL Enable)
{
    U_IP_TS_OUT_STOP ipout_en;

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    ipout_en.u32 = DMX_READ_REG(DmxSet->IoBase, IP_TS_OUT_STOP(RawPortId));

    ipout_en.bits.ip_stop_en = Enable ? 0 : 1;
    DMX_WRITE_REG(DmxSet->IoBase, IP_TS_OUT_STOP(RawPortId), ipout_en.u32);

    DMX_COM_EQUAL(ipout_en.u32, DMX_READ_REG(DmxSet->IoBase, IP_TS_OUT_STOP(RawPortId)));
}

HI_VOID DmxHalIPPortStartStream(Dmx_Set_S *DmxSet, const HI_U32 PortId, const HI_BOOL Enable)
{
    HI_U32 RawRamId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    return __DmxHalIPPortStartStream(DmxSet, RawRamId, Enable);
}

HI_VOID __DmxHalIPPortRateSet(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 Rate)
{
    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    DMX_WRITE_REG(DmxSet->IoBase, IP_TS_RATE_CFG(RawPortId), Rate);
    DMX_COM_EQUAL(Rate, DMX_READ_REG(DmxSet->IoBase, IP_TS_RATE_CFG(RawPortId)));
}

HI_VOID DmxHalIPPortRateSet(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 Rate)
{
    HI_U32 RawRamId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    return __DmxHalIPPortRateSet(DmxSet, RawRamId, Rate);
}

/***********************************************************************************
* Function      : DmxHalIPPortDescSet
* Description   : Set IP descriptors
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID __DmxHalIPPortDescSet(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 StartAddr, HI_U32 Depth)
{
    U_IP_DESC_SIZE ipsize;

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    ipsize.u32 = DMX_READ_REG(DmxSet->IoBase, IP_DESC_SIZE(RawPortId));

    ipsize.bits.ip_desc_size = Depth;
    DMX_WRITE_REG(DmxSet->IoBase, IP_DESC_SIZE(RawPortId), ipsize.u32);

    DMX_COM_EQUAL(ipsize.u32, DMX_READ_REG(DmxSet->IoBase, IP_DESC_SIZE(RawPortId)));

    DMX_WRITE_REG(DmxSet->IoBase, IP_DESC_SADDR(RawPortId), StartAddr);

    DMX_COM_EQUAL(StartAddr, DMX_READ_REG(DmxSet->IoBase, IP_DESC_SADDR(RawPortId)));
}

HI_VOID DmxHalIPPortDescSet(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 StartAddr, HI_U32 Depth)
{
    HI_U32 RawRamId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    return __DmxHalIPPortDescSet(DmxSet, RawRamId, StartAddr, Depth);
}

/***********************************************************************************
* Function      : DmxHalIPPortDescAdd
* Description   : add descriptors
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalIPPortDescAdd(Dmx_Set_S *DmxSet, const HI_U32 PortId, const HI_U32 DescNum)
{
    U_IP_DESC_ADD DescAdd;
    HI_U32 RawRamId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    BUG_ON(DMX_INVALID_PORT_ID == RawRamId);

    wmb();/*sync the DDR*/
    DescAdd.u32 = DMX_READ_REG(DmxSet->IoBase, IP_DESC_ADD(RawRamId));

    DescAdd.bits.ip_desc_add = DescNum;
    DMX_WRITE_REG(DmxSet->IoBase, IP_DESC_ADD(RawRamId), DescAdd.u32);

    DMX_COM_EQUAL(DescAdd.u32, DMX_READ_REG(DmxSet->IoBase, IP_DESC_ADD(RawRamId)));
}

/***********************************************************************************
* Function      : DmxHalIPPortSetIntCnt
* Description   : Set ip out int level
* Input         : PortId, PackeNum
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalIPPortSetIntCnt(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 DescNum)
{
    U_IP_INT_CNT_CFG unIpIntCfg;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    unIpIntCfg.u32 = DMX_READ_REG(DmxSet->IoBase, IP_INT_CNT_CFG(RawPortId));

    unIpIntCfg.bits.ip_int_cfg = DescNum;
    DMX_WRITE_REG(DmxSet->IoBase, IP_INT_CNT_CFG(RawPortId), unIpIntCfg.u32);

    DMX_COM_EQUAL((unIpIntCfg.u32 & 0xf), (DMX_READ_REG(DmxSet->IoBase, IP_INT_CNT_CFG(RawPortId)) & 0xf));
}

/***********************************************************************************
* Function      : DmxHalIPPortGetOutIntStatus
* Description   : Get desc_out int status
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalIPPortGetOutIntStatus(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    U_IP_IINT_INT ip_int;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    ip_int.u32 = DMX_READ_REG(DmxSet->IoBase, IP_IINT_INT(RawPortId));

    return ip_int.bits.ip_iint_desc_out;
}

/***********************************************************************************
* Function      : DmxHalIPPortClearOutIntStatus
* Description   : clear desc_out int status
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalIPPortClearOutIntStatus(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    U_IP_IRAW_INT ip_raw_int;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    ip_raw_int.u32 = DMX_READ_REG(DmxSet->IoBase, IP_IRAW_INT(RawPortId));

    ip_raw_int.bits.ip_iraw_desc_out = 1;
    DMX_WRITE_REG(DmxSet->IoBase, IP_IRAW_INT(RawPortId), ip_raw_int.u32);
}

/***********************************************************************************
* Function      : DmxHalIPPortSetOutInt
* Description   : Set desc_out int enale
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID __DmxHalIPPortSetOutInt(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_BOOL Enable)
{
    U_IP_IENA_INT ip_int_ena;

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    ip_int_ena.u32 = DMX_READ_REG(DmxSet->IoBase, IP_IENA_INT(RawPortId));

    ip_int_ena.bits.ip_iena_desc_out = Enable;
    DMX_WRITE_REG(DmxSet->IoBase, IP_IENA_INT(RawPortId), ip_int_ena.u32);

    DMX_COM_EQUAL(ip_int_ena.u32, DMX_READ_REG(DmxSet->IoBase, IP_IENA_INT(RawPortId)));
}

HI_VOID DmxHalIPPortSetOutInt(Dmx_Set_S *DmxSet, const HI_U32 PortId, const HI_BOOL Enable)
{
    HI_U32 RawRamId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    return __DmxHalIPPortSetOutInt(DmxSet, RawRamId, Enable);
}
/***********************************************************************************
* Function      : DmxHalIPPortDescGetRead
* Description   : Get desc read offset
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalIPPortDescGetRead(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    U_IP_DESC_PTR ip_ptr;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    ip_ptr.u32 = DMX_READ_REG(DmxSet->IoBase, IP_DESC_PTR(RawPortId));

    return ip_ptr.bits.ip_desc_rptr;
}

/***********************************************************************************
* Function      : DmxHalIPPortEnableInt
* Description   : Set IP int enable
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalIPPortEnableInt(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    U_ENA_INT_TYPE  unEnaErr;
    U_IP_IENA_INT   unEnaIp;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);
    unEnaIp.u32 = DMX_READ_REG(DmxSet->IoBase, IP_IENA_INT(RawPortId));

    switch (RawPortId)
    {
        case 0:
            unEnaErr.bits.iena_ip0_all = 1;
            break;

        case 1:
            unEnaErr.bits.iena_ip1_all = 1;
            break;

        case 2:
            unEnaErr.bits.iena_ip2_all = 1;
            break;

        case 3:
            unEnaErr.bits.iena_ip3_all = 1;
            break;

        case 4:
            unEnaErr.bits.iena_ip4_all = 1;
            break;

        case 5:
            unEnaErr.bits.iena_ip5_all = 1;
            break;

        default:
            return;
    }

    unEnaIp.bits.ip_iena_all = 1;
    DMX_WRITE_REG(DmxSet->IoBase, ENA_INT_TYPE, unEnaErr.u32);
    DMX_WRITE_REG(DmxSet->IoBase, IP_IENA_INT(RawPortId), unEnaIp.u32);
}

/***********************************************************************************
* Function      : DmxHalIPPortDisableInt
* Description   : Set IP int disable
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalIPPortDisableInt(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    U_ENA_INT_TYPE unEnaErr;
    U_IP_IENA_INT unEnaIp;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);
    unEnaIp.u32 = DMX_READ_REG(DmxSet->IoBase, IP_IENA_INT(RawPortId));
    switch (RawPortId)
    {
    case 0:
        unEnaErr.bits.iena_ip0_all = 0;
        break;
    case 1:
        unEnaErr.bits.iena_ip1_all = 0;
        break;
    case 2:
        unEnaErr.bits.iena_ip2_all = 0;
        break;
    case 3:
        unEnaErr.bits.iena_ip3_all = 0;
        break;
    default:
        return;
    }

    unEnaIp.bits.ip_iena_all = 0;
    DMX_WRITE_REG(DmxSet->IoBase, ENA_INT_TYPE, unEnaErr.u32);
    DMX_WRITE_REG(DmxSet->IoBase, IP_IENA_INT(RawPortId), unEnaIp.u32);
}


HI_VOID DmxHalIPPortGetBPStatus(Dmx_Set_S *DmxSet, HI_U32 RawPortId, DMX_Proc_RamPort_BPStatus_S *BPStatus)
{
    HI_U32  offset;
    HI_U32 status32;
    HI_U32 status40;
    HI_U32 regaddr;
    HI_U32 OverflowBp;
    HI_U32 SwitchBufferBp;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    offset = 0;
    BPStatus->FQLow32BPEn = DMX_READ_REG(DmxSet->IoBase, IP_BP_FQ_CFG(RawPortId, offset));/*0xC220*/
    offset = 1;
    BPStatus->FQHigh8BPEn = DMX_READ_REG(DmxSet->IoBase, IP_BP_FQ_CFG(RawPortId, offset));/*0xC224*/

    IP_BP_FQ_STA(RawPortId, 0,regaddr);
    status32 = DMX_READ_REG(DmxSet->IoBase, regaddr);
    IP_BP_FQ_STA(RawPortId, 1,regaddr);
    status40 = DMX_READ_REG(DmxSet->IoBase, regaddr) & 0xff;

    BPStatus->FQBP =  (HI_BOOL)(status32 || status40);

    OverflowBp                  =  DMX_READ_REG(DmxSet->IoBase, IP_CHN_BP_STA) & 0x7;
    BPStatus->OverflowBp        =  (RawPortId == OverflowBp)? HI_TRUE:HI_FALSE;
    SwitchBufferBp              =   (DMX_READ_REG(DmxSet->IoBase, IP_CHN_BP_STA) >> 8 ) & 0x7;
    BPStatus->SwitchBufferBp    =  (RawPortId == SwitchBufferBp)? HI_TRUE:HI_FALSE;
    BPStatus->Rate              =   DMX_READ_REG(DmxSet->IoBase, IP_TS_RATE_CFG(RawPortId)) ;
    BPStatus->DebugEn           =   DMX_READ_REG(DmxSet->IoBase, IP_DBG_CNT_EN(RawPortId)) ;
    BPStatus->OutByte           =   DMX_READ_REG(DmxSet->IoBase, IP_DBG_OUT0(RawPortId));
    BPStatus->OutTSNum          =   DMX_READ_REG(DmxSet->IoBase, IP_DBG_OUT1(RawPortId));
    BPStatus->BPCount           =   DMX_READ_REG(DmxSet->IoBase, (IP_DBG_OUT2(RawPortId) >> 24)) & 0xff;

    return ;
}

HI_VOID DmxHalIPPortGetDescInfo(Dmx_Set_S *DmxSet, HI_U32 RawPortId, DMX_Proc_RamPort_DescInfo_S *DescInfo)
{
    HI_U32 DescWNodeAddr;
    HI_U32 DescRNodeAddr;
    HI_U32* DescWNodeVirAddr = NULL;
    HI_U32* DescRNodeVirAddr = NULL;
    HI_U32* DescSizeVirAddr = NULL;

    BUG_ON(RawPortId >= DmxSet->RamPortCnt);

    DescInfo->DescPhyAddr   = DMX_READ_REG(DmxSet->IoBase, IP_DESC_SADDR(RawPortId));
    DescInfo->DescDepth     = DMX_READ_REG(DmxSet->IoBase, IP_DESC_SIZE(RawPortId));
    DescInfo->DescWPtr      = (DMX_READ_REG(DmxSet->IoBase, IP_DESC_PTR(RawPortId)) >> 16 ) & 0xffff ;
    DescInfo->DescRPtr      = DMX_READ_REG(DmxSet->IoBase, IP_DESC_SIZE(RawPortId)) & 0xffff;
    DescInfo->ValidDescNum  = DMX_READ_REG(DmxSet->IoBase, IP_VLDDESC_CNT(RawPortId));
    DescInfo->AddDescNum    = DMX_READ_REG(DmxSet->IoBase, IP_DESC_ADD(RawPortId));

    DescWNodeAddr            = DescInfo->DescPhyAddr + DescInfo->DescWPtr * 16 ; /*每个描述子节点size 是 16BYTE*/
    DescRNodeAddr            = DescInfo->DescPhyAddr + DescInfo->DescRPtr * 16 ; /*每个描述子节点size 是 16BYTE*/

    if ( (DescWNodeAddr != 0) && (DescRNodeAddr != 0) )
    {
        DescWNodeVirAddr        =  ( HI_U32*)DEMUX_MAP_DDR_PHYADDRESS(DescWNodeAddr);
        DescRNodeVirAddr        =  ( HI_U32*)(DEMUX_MAP_DDR_PHYADDRESS(DescRNodeAddr));
        DescSizeVirAddr         =  ( HI_U32*)(DEMUX_MAP_DDR_PHYADDRESS(DescWNodeAddr) +  4);

        if ( DescWNodeVirAddr != NULL  )
        {
            DescInfo->DescWAddr  = *DescWNodeVirAddr;
        }

        if ( DescRNodeVirAddr != NULL )
        {
            DescInfo->DescRAddr  =  *DescRNodeVirAddr;
        }
        if ( DescSizeVirAddr != NULL )
        {
            DescInfo->DescSize = *DescSizeVirAddr & 0xffff;
        }

        DEMUX_UMMAP_DDR_PHYADDRESS(DescWNodeAddr);
        DEMUX_UMMAP_DDR_PHYADDRESS(DescRNodeAddr);
    }

    return ;
}

HI_VOID DMXHalGetChannelDataFlow(HI_U32 ChannelId, ChannelDataFlow_info_t *ChannelDF)
{
    #if 0
    HI_U32  value;
    HI_U32  offset  = ChannelId >> 5;
    HI_U32  bit     = ChannelId & 0x1f;

    ChannelDF->PIDTsPacket      =  DMX_READ_REG(CHANNEL_TS_COUNT(ChannelId));
    ChannelDF->BufferTsPacket   =  DMX_READ_REG(ADDR_INT_CNT(ChannelId));
    value                       = DMX_READ_REG(OQ_ENA_0(offset));
    ChannelDF->OQEn             = (value >> bit) & 0x1;
    value                       = DMX_READ_REG(FQ_ENA_0(offset));
    ChannelDF->FQEn             = (value >> bit) & 0x1;
    #endif
    return;
}

HI_VOID DmxHalGetChannelTSCount(HI_U32 ChanId, HI_U32 *ChanTsCount, HI_U32* OQTsCount)
{
    #if 0
    *ChanTsCount = DMX_READ_REG(CHANNEL_TS_COUNT(ChanId));
    *OQTsCount = DMX_READ_REG(ADDR_INT_CNT(ChanId));
    #endif
}

/***********************************************************************************
* Function      : DmxHalSetChannelDataType
* Description   : Set Channel Data Type
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelDataType(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_CHAN_DATA_TYPE_E DataType)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));

    dmx_pidctrl.bits.data_type = DataType;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelAFMode
* Description   :  Set Channel AF Mode
* Input         : Portid (= 0,1,2)
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelAFMode(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Ch_AFMode_E eAfMode)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    dmx_pidctrl.bits.af_mode = eAfMode & 0x3;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      : DmxHalSetChannelCRCMode
* Description   : Set Channel CRC Mode
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelCRCMode(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_UNF_DMX_CHAN_CRC_MODE_E CrcMode)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));

    switch (CrcMode)
    {
        case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD :
            dmx_pidctrl.bits.crc_mode = 1;
            break;

        case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_SEND :
            dmx_pidctrl.bits.crc_mode = 2;
            break;

        case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD :
            dmx_pidctrl.bits.crc_mode = 3;
            break;

        case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_SEND :
            dmx_pidctrl.bits.crc_mode = 4;
            break;

        case HI_UNF_DMX_CHAN_CRC_MODE_FORBID :
        default :
            dmx_pidctrl.bits.crc_mode = 0;
    }

    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelCCDiscon
* Description   :  Set Channel CC discontiune Mode
* Input         : Portid (= 0,1,2)  DiscardFlag =0:not discard the discontinuous CC TS packet;
1 discard the discontinuous CC TS packet
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelCCDiscon(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 DiscardFlag)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    dmx_pidctrl.bits.cc_discon_ctrl = DiscardFlag & 0x1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelPusiCtrl
* Description   :  Set Channel pusi control
* Input         : Portid (= 0,1,2)  PusiCtrl =0 : PUSI valid, check PUSI, start output stream when PUSI incoming
1: PUSI invalid, not check PUSI, output stream directly
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelPusiCtrl(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 PusiCtrl)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    dmx_pidctrl.bits.pusi_disable = PusiCtrl & 0x1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelCCRepeatCtrl
* Description   :  Set Channel CC Repeat Ctrl
* Input         : Portid (= 0,1,2)  CCRepeatCtrl =0:discard this packet; 1: reserve this packet
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelCCRepeatCtrl(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 CCRepeatCtrl)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    dmx_pidctrl.bits.cc_equ_rve = CCRepeatCtrl & 0x1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelTsPostMode
* Description   :  Set Channel Ts Post Mode
* Input         : Portid (= 0,1,2)  TSPost =0:not select TS_POST mode; 1: select TS_POST mode
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelTsPostMode(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 TsPost)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    dmx_pidctrl.bits.ts_post_mode = TsPost & 0x1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelTsPostThresh
* Description   :  Set Channel Ts Post Thresh
* Input         : Portid (= 0,1,2) Threshold
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelTsPostThresh(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 Threshold)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    dmx_pidctrl.bits.ts_post_threshold = Threshold & 0x3f;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelAttr
* Description   :  Set Channel attr
* Input         : Portid (= 0,1,2) Threshold
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelAttr(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Ch_ATTR_E echattr)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    if ((echattr == DMX_CH_AUDIO) || (echattr == DMX_CH_VIDEO))
    {
        dmx_pidctrl.bits.pusi_disable = 1;
    }
    else
    {
        dmx_pidctrl.bits.pusi_disable = 0;
    }

    dmx_pidctrl.bits.ch_attri = echattr & 0x7;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);

    DMX_COM_EQUAL(dmx_pidctrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelFltMode
* Description   :  Set Channel Filter hit Mode
* Input         : u32Chid  bEnable
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelFltMode(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL bEnable)
{
    U_DMX_PID_REC_BUF unPidRecTrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    unPidRecTrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId));
    unPidRecTrl.bits.flt_hit_mode = bEnable;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId), unPidRecTrl.u32);

    DMX_COM_EQUAL(unPidRecTrl.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalGetChannelPlayDmxid
* Description   :   Get Channel PlayDmxid
* Input         : chid (= 0,1,2)
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalGetChannelPlayDmxid(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 *dmxid)
{
    U_DMX_PID_EN dmx_pid_en;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pid_en.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_EN(ChanId));
    *dmxid = (dmx_pid_en.bits.pid_play_dmx_id & 0x7) - 1;
}

/***********************************************************************************
* Function      :  DmxHalSetChannelPlayDmxid
* Description   :  Set Channel PlayDmxid
* Input         : Portid (= 0,1,2) Threshold
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelPlayDmxid(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 DmxId)
{
    U_DMX_PID_EN dmx_pid_en;
    HI_U32 RawDmxId = DMX_INVALID_DEMUX_ID == DmxId ? DmxId : DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pid_en.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_EN(ChanId));
    dmx_pid_en.bits.pid_play_dmx_id = (RawDmxId + 1) & 0x7;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_EN(ChanId), dmx_pid_en.u32);

    DMX_COM_EQUAL(dmx_pid_en.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_EN(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalSetChannelRecDmxid
* Description   :  Set Channel record Dmxid
* Input         : Portid (= 0,1,2) Threshold
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelRecDmxid(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 DmxId)
{
    U_DMX_PID_EN dmx_pid_en;
    HI_U32 RawDmxId = DMX_INVALID_DEMUX_ID == DmxId ? DmxId : DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pid_en.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_EN(ChanId));
    dmx_pid_en.bits.pid_rec_dmx_id = (RawDmxId + 1) & 0x7;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_EN(ChanId), dmx_pid_en.u32);

    DMX_COM_EQUAL(dmx_pid_en.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_EN(ChanId)));
}

/***********************************************************************************
* Function      : DmxHalSetChannelPid
* Description   : set Channel pid
* Input         : ChanId, pid
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelPid(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 pid)
{
    U_DMX_PID_VALUE dmx_pid;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pid.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_VALUE(ChanId));
    dmx_pid.bits.pid_value = pid & 0x1fff;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_VALUE(ChanId), dmx_pid.u32);

    DMX_COM_EQUAL(dmx_pid.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_VALUE(ChanId)));
}

#ifdef DMX_DUP_PID_CHANNEL_SUPPORT
HI_VOID DmxHalEnablePidCopy(Dmx_Set_S *DmxSet)
{
    U_DMX_MODE  dmx_mod;

    dmx_mod.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_MODE_0);
    dmx_mod.bits.sw_pidcopy_en = 1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_MODE_0, dmx_mod.u32);

    DMX_COM_EQUAL(dmx_mod.u32, DMX_READ_REG(DmxSet->IoBase, DMX_MODE_0));
}
#endif

/***********************************************************************************
* Function      : DmxHalSetChannelRecBufId
* Description   : Set Channel Rec BufId
* Input         : ChanId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelRecBufId(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 OQId)
{
    U_DMX_PID_REC_BUF pid_rec_buf;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);
    BUG_ON(DMX_INVALID_OQ_ID == OQId ? 0 : OQId >= DmxSet->DmxOqCnt);

    pid_rec_buf.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId));

    pid_rec_buf.bits.rec_buf = OQId;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId), pid_rec_buf.u32);

    DMX_COM_EQUAL(pid_rec_buf.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId)));
}

HI_VOID DmxHalSetShareRecChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL bEnable)
{
    U_DMX_PID_REC_BUF pid_rec_buf;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    pid_rec_buf.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId));

    pid_rec_buf.bits.com_pid_indicator = bEnable;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId), pid_rec_buf.u32);

    DMX_COM_EQUAL(pid_rec_buf.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_REC_BUF(ChanId)));
}

/***********************************************************************************
* Function      : DmxHalSetChannelPlayBufId
* Description   : Set Channel Play BufId
* Input         : ChanId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelPlayBufId(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 OQId)
{
    U_DMX_PID_PLAY_BUF pid_play_buf;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);
    BUG_ON(DMX_INVALID_OQ_ID == OQId ? 0 : OQId >= DmxSet->DmxOqCnt);

    pid_play_buf.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_PLAY_BUF(ChanId));

    pid_play_buf.bits.play_buf = OQId;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_PLAY_BUF(ChanId), pid_play_buf.u32);

    DMX_COM_EQUAL(pid_play_buf.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PID_PLAY_BUF(ChanId)));
}

HI_U32 DmxHalGetPortRawId(Dmx_Set_S *DmxSet, DMX_PORT_MODE_E PortMode, HI_U32 PortId)
{
    HI_U32 Id;

     switch(PortMode)
    {
        case DMX_PORT_MODE_TUNER:
        {
            Id = DmxSet->Ops->GetTunerRawId(DmxSet, PortId) ;

            BUG_ON(DMX_INVALID_PORT_ID == Id);

            Id += DMX_PORT_OFFSET;
            break;
        }

        case DMX_PORT_MODE_RAM:
        {
            Id = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

            BUG_ON(DMX_INVALID_PORT_ID == Id);

            Id += DMX_RAM_PORT_OFFSET + DMX_PORT_OFFSET;
            break;
        }

        case DMX_PORT_MODE_RMX:
        {
            Id = DmxSet->Ops->GetRmxRawId(DmxSet, PortId);

            BUG_ON(DMX_INVALID_REMUX_ID == Id);

            Id = PortId + RMX_PORT_OFFSET;

            break;
        }

        default:
        {
            WARN(DMX_INVALID_PORT_ID != PortId, "PortMod(%d) is invalid, but PortId(%d) is valid.\n", PortMode, PortId);

            Id = DMX_INVALID_PORT_ID;
        }
    }

    return Id;
}

/***********************************************************************************
* Function      : DmxHalSetDemuxPortId
* Description   : Set Dmx PortId
* Input         : DmxId, PortMode, PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID __DmxHalSetDemuxPortId(Dmx_Set_S *DmxSet, HI_U32 RawDmxId, DMX_PORT_MODE_E PortMode, HI_U32 PortId)
{
    HI_U32 Id = 0;
    HI_SIZE_T u32LockFlag;

    BUG_ON(RawDmxId >= DmxSet->DmxCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    Id = DmxHalGetPortRawId(DmxSet, PortMode, PortId);

    if (RawDmxId < 4)
    {
        U_SWITCH_CFG0   SwitchCfg0;

        SwitchCfg0.u32 = DMX_READ_REG(DmxSet->IoBase, SWITCH_CFG0);

        switch (RawDmxId)
        {
            case 0:
                SwitchCfg0.bits.switch_cfg1 = Id;
                break;

            case 1:
                SwitchCfg0.bits.switch_cfg2 = Id;
                break;

            case 2:
                SwitchCfg0.bits.switch_cfg3 = Id;
                break;

            case 3:
            default :
                SwitchCfg0.bits.switch_cfg4 = Id;
                break;
        }

        DMX_WRITE_REG(DmxSet->IoBase, SWITCH_CFG0, SwitchCfg0.u32);
    }
    else
    {
        U_SWITCH_CFG1   SwitchCfg1;

        SwitchCfg1.u32 = DMX_READ_REG(DmxSet->IoBase, SWITCH_CFG1);

        switch (RawDmxId)
        {
            case 4:
                SwitchCfg1.bits.switch_cfg5 = Id;
                break;

            case 5:
                SwitchCfg1.bits.switch_cfg6 = Id;
                break;

            case 6:
                SwitchCfg1.bits.switch_cfg7 = Id;
                break;

            default:
                break;
        }

        DMX_WRITE_REG(DmxSet->IoBase, SWITCH_CFG1, SwitchCfg1.u32);
    }

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID DmxHalSetDemuxPortId(Dmx_Set_S *DmxSet, HI_U32 DmxId, DMX_PORT_MODE_E PortMode, HI_U32 PortId)
{
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    __DmxHalSetDemuxPortId(DmxSet, RawDmxId, PortMode, PortId);
}

/***********************************************************************************
* Function      :  DmxHalSetDataFakeMod
* Description   :  Set Data Fake Mode
* Input         : bFakeEn
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetDataFakeMod(Dmx_Set_S *DmxSet, HI_BOOL bFakeEn)
{
    U_SWITCH_FAKE_EN switch_fake_en;

    switch_fake_en.u32 = DMX_READ_REG(DmxSet->IoBase, SWITCH_FAKE_EN);
    switch_fake_en.bits.switch_fake_en = bFakeEn;
    DMX_WRITE_REG(DmxSet->IoBase, SWITCH_FAKE_EN, switch_fake_en.u32);
}

/***********************************************************************************
* Function      : DmxHalSetRecType
* Description   : Set Rec Type
* Input         : DmxId, RecType
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetRecType(Dmx_Set_S *DmxSet, HI_U32 DmxId, DMX_REC_TYPE_E RecType)
{
    U_DMX_CTRL_FUNC dmx_ctrl_func;
    HI_SIZE_T u32LockFlag;
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    RawDmxId += DMX_SW_HW_OFFSET;

    dmx_ctrl_func.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_CTRL_FUNC);
    switch (RawDmxId)
    {
        case 1:
            dmx_ctrl_func.bits.dmx1_rec_ctrl = RecType;
            break;

        case 2:
            dmx_ctrl_func.bits.dmx2_rec_ctrl = RecType;
            break;

        case 3:
            dmx_ctrl_func.bits.dmx3_rec_ctrl = RecType;
            break;

        case 4:
            dmx_ctrl_func.bits.dmx4_rec_ctrl = RecType;
            break;

        case 5:
            dmx_ctrl_func.bits.dmx5_rec_ctrl = RecType;
            break;

        case 6:
            dmx_ctrl_func.bits.dmx6_rec_ctrl = RecType;
            break;

        case 7:
            dmx_ctrl_func.bits.dmx7_rec_ctrl = RecType;
            break;

        default:
            dmx_ctrl_func.bits.dmx1_rec_ctrl = RecType;
    }

    DMX_WRITE_REG(DmxSet->IoBase, DMX_CTRL_FUNC, dmx_ctrl_func.u32);

    DMX_COM_EQUAL(dmx_ctrl_func.u32, DMX_READ_REG(DmxSet->IoBase, DMX_CTRL_FUNC));

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      : DmxHalFlushChannel
* Description   : Flush Channel
* Input         : ChanId, FlushType
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalFlushChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_FLUSH_TYPE_E FlushType)
{
    U_DMX_GLB_FLUSH glb_flush;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    glb_flush.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_GLB_FLUSH);

    glb_flush.bits.flush_ch     = ChanId;
    glb_flush.bits.flush_type   = FlushType;
    glb_flush.bits.flush_cmd    = 1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_GLB_FLUSH, glb_flush.u32);
}

/***********************************************************************************
* Function      :  DmxHalIsFlushChannelDone
* Description   :  Is flush channel done
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_BOOL DmxHalIsFlushChannelDone(Dmx_Set_S *DmxSet)
{
    U_DMX_GLB_FLUSH glb_flush;

    glb_flush.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_GLB_FLUSH);

    if (glb_flush.bits.flush_done)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

#ifdef DMX_SCD_VERSION_2
HI_VOID DmxHalSetRecBuf(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 OqId)
{
    U_REC_BUF_SET buf_set;

    BUG_ON(RecId >= DmxSet->DmxRecCnt);
    BUG_ON(DMX_INVALID_OQ_ID == OqId ? 0 : OqId >= DmxSet->DmxOqCnt);

    buf_set.u32 = DMX_READ_REG(DmxSet->IoBase, REC_BUF_SET(RecId));

    if (OqId == DMX_INVALID_OQ_ID)
    {
        buf_set.bits.rec_buf_id = 0xff;
        buf_set.bits.rec_buf_vld = 0;
    }
    else
    {
        buf_set.bits.rec_buf_id = OqId;
        buf_set.bits.rec_buf_vld = 1;
    }

    DMX_WRITE_REG(DmxSet->IoBase, REC_BUF_SET(RecId), buf_set.u32);

    DMX_COM_EQUAL(buf_set.u32, DMX_READ_REG(DmxSet->IoBase, REC_BUF_SET(RecId)));
}

HI_VOID DmxHalSetDmxRecFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 RecId)
{
    HI_U32 dmx_rec_set;
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    BUG_ON(RecId >= DmxSet->DmxRecCnt);

    dmx_rec_set = DMX_READ_REG(DmxSet->IoBase, DMX_REC_SET(RawDmxId));

    dmx_rec_set |= (0x1 << RecId);

    DMX_WRITE_REG(DmxSet->IoBase, DMX_REC_SET(RawDmxId), dmx_rec_set);

    DMX_COM_EQUAL(dmx_rec_set.u32, DMX_READ_REG(DmxSet->IoBase, DMX_REC_SET(RawDmxId)));
}

HI_VOID DmxHalClearDmxRecFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 RecId)
{
    HI_U32 dmx_rec_set;
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    BUG_ON(RecId >= DmxSet->DmxRecCnt);

    dmx_rec_set = DMX_READ_REG(DmxSet->IoBase, DMX_REC_SET(RawDmxId));

    dmx_rec_set &= ~(0x1 << RecId);

    DMX_WRITE_REG(DmxSet->IoBase, DMX_REC_SET(RawDmxId), dmx_rec_set);

    DMX_COM_EQUAL(dmx_rec_set.u32, DMX_READ_REG(DmxSet->IoBase, DMX_REC_SET(RawDmxId)));
}
#endif

/***********************************************************************************
* Function      : DmxHalSetDmxRecBufId
* Description   : Set Ts Record Buf Id
* Input         : DmxId, OqId
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalSetDmxRecBufId(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 OqId)
{
    U_DMX_GLB_CTRL2 glb_ctrl2;
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    BUG_ON(DMX_INVALID_OQ_ID == OqId ? 0 : OqId >= DmxSet->DmxOqCnt);

    glb_ctrl2.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL2(RawDmxId));

    glb_ctrl2.bits.tsrec_pidbuf_en = 1;  /* always enable mutil rec if possible */
    glb_ctrl2.bits.dmx_tsrec_buf = OqId;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_GLB_CTRL2(RawDmxId), glb_ctrl2.u32);

    DMX_COM_EQUAL(glb_ctrl2.u32, DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL2(RawDmxId)));
}

/***********************************************************************************
* Function      : DmxHalSetSpsRefRecCh
* Description   : Set Sps Reference Channel
* Input         : DmxId, ChanId
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalSetSpsRefRecCh(Dmx_Set_S *DmxSet, HI_U32 RawDmxId, HI_U32 ChanId)
{
    U_DMX_GLB_CTRL3 glb_ctrl3;

    BUG_ON(RawDmxId >= DmxSet->DmxCnt);

    glb_ctrl3.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL3(RawDmxId));
    glb_ctrl3.bits.dmx_spsrec_pusi_ch = ChanId;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_GLB_CTRL3(RawDmxId), glb_ctrl3.u32);

    DMX_COM_EQUAL(glb_ctrl3.u32, DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL3(RawDmxId)));
}

/***********************************************************************************
* Function      :  DmxHalSetSpsPauseType
* Description   : Set Sps Reference Channel
* Input         : dmxid(= 1,2,3,4,5)type
* Output        :
* Return        :
* Others:       :type  0: after sps_ctrl valid, when find out the spx_num pes header in base channel, pause the play
1:after sps_ctrl valid, when find out ts tail in base channel, puase the timeshift
***********************************************************************************/
HI_VOID DmxHalSetSpsPauseType(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 type)
{
    U_DMX_GLB_CTRL3 glb_ctrl3;
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    glb_ctrl3.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL3(RawDmxId));
    glb_ctrl3.bits.dmx_sps_type = type & 0x1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_GLB_CTRL3(RawDmxId), glb_ctrl3.u32);

    DMX_COM_EQUAL(glb_ctrl3.u32, DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL3(RawDmxId)));
}

/***********************************************************************************
* Function      : DmxHalSetFilter
* Description   : Set filter attr
* Input         : FilterNum, Depth, Content, Mask
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalSetFilter(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U32 Depth, HI_U8 Content, HI_BOOL bReverse, HI_U8 Mask)
{
    U_DMX_FILTER dmx_filter_crtl;

    BUG_ON(FilterId >= DmxSet->DmxFilterCnt);

    dmx_filter_crtl.u32 = 0;

    dmx_filter_crtl.bits.wdata_mask     = Mask;
    dmx_filter_crtl.bits.wdata_content  = Content;
    dmx_filter_crtl.bits.wdata_mode     = bReverse;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_FILTERxy(FilterId, Depth), dmx_filter_crtl.u32);
}

/***********************************************************************************
* Function      : DmxHalClearOq
* Description   : Clear Oq
* Input         : OQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalClearOq(Dmx_Set_S *DmxSet, HI_U32 OqId, DMX_OQ_CLEAR_TYPE_E ClearType)
{
    U_CLEAR_CHANNEL clear_ch;

    BUG_ON(OqId >= DmxSet->DmxOqCnt);

    clear_ch.u32 = DMX_READ_REG(DmxSet->IoBase, CLR_CHN_CMD);

    clear_ch.bits.clear_chn   = OqId;
    clear_ch.bits.clear_type  = ClearType;
    clear_ch.bits.clear_start = 1;
    DMX_WRITE_REG(DmxSet->IoBase, CLR_CHN_CMD, clear_ch.u32);
}

/***********************************************************************************
* Function      : DmxHalIsClearOqDone
* Description   :
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_BOOL DmxHalIsClearOqDone(Dmx_Set_S *DmxSet)
{
    HI_BOOL         ret = HI_FALSE;
    U_IRAW_CLR_CHN  clear_ch;

    clear_ch.u32 = DMX_READ_REG(DmxSet->IoBase, RAW_CLR_CHN);
    if (clear_ch.bits.iraw_clr_chn)
    {
        DMX_WRITE_REG(DmxSet->IoBase, RAW_CLR_CHN, clear_ch.u32);

        ret = HI_TRUE;
    }

    return ret;
}

/***********************************************************************************
* Function      :  DmxHalEnableAllPVRInt
* Description   :    Get eop int status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableAllPVRInt(Dmx_Set_S *DmxSet)
{
    DMX_WRITE_REG(DmxSet->IoBase, ENA_PVR_INT, 1);
}

/***********************************************************************************
* Function      :  DmxHalDisableAllPVRInt
* Description   :    Get eop int status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableAllPVRInt(Dmx_Set_S *DmxSet)
{
    DMX_WRITE_REG(DmxSet->IoBase, ENA_PVR_INT, 0);

    DMX_COM_EQUAL(0, DMX_READ_REG(DmxSet->IoBase, ENA_PVR_INT));
}

/***********************************************************************************
* Function      : DmxHalFQEnableAllOverflowInt
* Description   : enable fq overflow int
* Input         :
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalFQEnableAllOverflowInt(Dmx_Set_S *DmxSet)
{
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);

    unEnaErr.bits.iena_chn_i = 0x3;
    DMX_WRITE_REG(DmxSet->IoBase, ENA_INT_TYPE, unEnaErr.u32);
}

/***********************************************************************************
* Function      : DmxHalFQGetAllOverflowIntStatus
* Description   : Get fq overflow int status
* Input         :
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_U32 DmxHalFQGetAllOverflowIntStatus(Dmx_Set_S *DmxSet)
{
    U_INT_STA_TYPE unIntSta;

    unIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, INT_STA_TYPE);

    return unIntSta.bits.iint_chn_i;
}

HI_U32 DmxHalFQGetOverflowIntStatus(Dmx_Set_S *DmxSet, HI_U32 offset)
{
    return DMX_READ_REG(DmxSet->IoBase, INT_FQ_CHN_0(offset));
}

HI_U32 DmxHalFQGetOverflowIntType(Dmx_Set_S *DmxSet, HI_U32 offset)
{
    return DMX_READ_REG(DmxSet->IoBase, TYPE_FQ_CHN_0(offset));
}

HI_VOID DmxHalFQClearOverflowInt(Dmx_Set_S *DmxSet, HI_U32 FqId)
{
    HI_U32  offset  = FqId >> 5;
    HI_U32  bit     = FqId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(FqId >= DmxSet->DmxFqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value = 1 << bit;
    DMX_WRITE_REG(DmxSet->IoBase, RAW_FQ_CHN_0(offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID DmxHalFQSetOverflowInt(Dmx_Set_S *DmxSet, HI_U32 FqId, HI_BOOL Enable)
{
    HI_U32  offset  = FqId >> 5;
    HI_U32  bit     = FqId & 0x1F;
    HI_U32  value;

    BUG_ON(FqId >= DmxSet->DmxFqCnt);

    value = DMX_READ_REG(DmxSet->IoBase, ENA_FQ_CHN_0(offset));

    if (Enable)
    {
        value |= 1 << bit;
    }
    else
    {
        value &= ~(1 << bit);
    }
    DMX_WRITE_REG(DmxSet->IoBase, ENA_FQ_CHN_0(offset), value);
}

HI_BOOL DmxHalFQIsEnableOverflowInt(Dmx_Set_S *DmxSet, HI_U32 FqId)
{
    HI_U32  offset  = FqId >> 5;
    HI_U32  bit     = FqId & 0x1F;
    HI_U32  value;

    BUG_ON(FqId >= DmxSet->DmxFqCnt);

    value = DMX_READ_REG(DmxSet->IoBase, ENA_FQ_CHN_0(offset));

    if (value & (1 << bit))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

/***********************************************************************************
* Function      : DmxHalOQGetAllOverflowIntStatus
* Description   : Get oq overflow int status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalOQGetAllOverflowIntStatus(Dmx_Set_S *DmxSet)
{
    U_INT_STA_TYPE unIntSta;

    unIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, INT_STA_TYPE);

    return unIntSta.bits.iint_chn_o;
}

/***********************************************************************************
* Function      : DmxHalOQGetAllEopIntStatus
* Description   :
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalOQGetAllEopIntStatus(Dmx_Set_S *DmxSet)
{
    U_INT_STA_TYPE unIntSta;

    unIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, INT_STA_TYPE);

    return unIntSta.bits.iint_eop_o;
}

/***********************************************************************************
* Function      :  DmxHalEnableAllChEopInt
* Description   :    Set channel overflow  int enable
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableAllChEopInt(Dmx_Set_S *DmxSet)
{
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);
    unEnaErr.bits.iena_eop_o = 0xf;
    DMX_WRITE_REG(DmxSet->IoBase, ENA_INT_TYPE, unEnaErr.u32);
}

/***********************************************************************************
* Function      :  DmxHalEnableAllChEnqueInt
* Description   :    Set channel overflow  int enable
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableAllChEnqueInt(Dmx_Set_S *DmxSet)
{
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);
    unEnaErr.bits.iena_desc_o = 0xf;
    DMX_WRITE_REG(DmxSet->IoBase, ENA_INT_TYPE, unEnaErr.u32);
}

/***********************************************************************************
* Function      :  DmxHalEnableD2BFOvflInt
* Description   :    Enable the DMX2BUF buffer overflow int of play channel
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableD2BFOvflInt(Dmx_Set_S *DmxSet)
{
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);
    unEnaErr.bits.iena_d2b_f_ovfl = 0x1;
    DMX_WRITE_REG(DmxSet->IoBase ,ENA_INT_TYPE, unEnaErr.u32);
}

/***********************************************************************************
* Function      :  DmxHalDisableD2BFOvflInt
* Description   :    Disable the DMX2BUF buffer overflow int of play channel
* Input          : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableD2BFOvflInt(Dmx_Set_S *DmxSet)
{
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);
    unEnaErr.bits.iena_d2b_f_ovfl = 0x0;

    DMX_WRITE_REG(DmxSet->IoBase ,ENA_INT_TYPE, unEnaErr.u32);
}

/***********************************************************************************
* Function      :  DmxHalGetD2BFOvflIntStatus
* Description  :  Get the DMX2BUF buffer overflow int status of play channel
* Input          : DmxSet
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32  DmxHalGetD2BFOvflIntStatus(Dmx_Set_S *DmxSet)
{
    U_INT_STA_TYPE unIntSta;

    unIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, INT_STA_TYPE);

    return unIntSta.bits.iint_d2b_f_ovfl;
}

/***********************************************************************************
* Function      :  DmxHalClsD2BFOvflIntStatus
* Description  :  Clear the DMX2BUF buffer overflow int status of play channel
* Input          : DmxSet
* Output        :
* Return        :
* Others:
***********************************************************************************/

HI_VOID DmxHalClsD2BFOvflIntStatus(Dmx_Set_S *DmxSet)
{
    U_IRAW_D2B_OVFL unIntSta;

    unIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, RAW_D2B_OVFL);
    unIntSta.bits.iraw_d2b_f_ovfl = 0x1;
    DMX_WRITE_REG(DmxSet->IoBase ,RAW_D2B_OVFL, unIntSta.u32);
}

/***********************************************************************************
* Function      :  DmxHalEnableD2BROvflInt
* Description   :    Enable the DMX2BUF buffer overflow int of record channel
* Input         : DmxSet
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableD2BROvflInt(Dmx_Set_S *DmxSet)
{
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);
    unEnaErr.bits.iena_d2b_r_ovfl = 0x1;
    DMX_WRITE_REG(DmxSet->IoBase ,ENA_INT_TYPE, unEnaErr.u32);
}

/***********************************************************************************
* Function      :  DmxHalDisableD2BROvflInt
* Description   :    Disable the DMX2BUF buffer overflow int of play channel
* Input          : DmxSet
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableD2BROvflInt(Dmx_Set_S *DmxSet)
{
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_TYPE);
    unEnaErr.bits.iena_d2b_r_ovfl = 0x0;

    DMX_WRITE_REG(DmxSet->IoBase ,ENA_INT_TYPE, unEnaErr.u32);
}

/***********************************************************************************
* Function      :  DmxHalGetD2BROvflIntStatus
* Description  :  Get the DMX2BUF buffer overflow int status of record channel
* Input          : DmxSet
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32  DmxHalGetD2BROvflIntStatus(Dmx_Set_S *DmxSet)
{
    U_INT_STA_TYPE unIntSta;

    unIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, INT_STA_TYPE);

    return unIntSta.bits.iint_d2b_r_ovfl;
}

/***********************************************************************************
* Function      :  DmxHalClsD2BFOvflIntStatus
* Description  :  Clear the DMX2BUF buffer overflow int status of record channel
* Input          : DmxSet
* Output        :
* Return        :
* Others:
***********************************************************************************/

HI_VOID DmxHalClsD2BROvflIntStatus(Dmx_Set_S *DmxSet)
{
    U_IRAW_D2B_OVFL unIntSta;

    unIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, RAW_D2B_OVFL);
    unIntSta.bits.iraw_d2b_r_ovfl = 0x1;
    DMX_WRITE_REG(DmxSet->IoBase ,RAW_D2B_OVFL, unIntSta.u32);
}

/***********************************************************************************
* Function      :  DmxHalEnableFQOutqueInt
* Description   :    Set channel overflow  int enable
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableFQOutqueInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_desc_i = 0x3;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalDisableFQOutqueInt
* Description   :    Set IP  int disable
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableFQOutqueInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_desc_i = 0;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalSetFlushMaxWaitTime
* Description   :    Set play dmx2buf bp mode
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetFlushMaxWaitTime(Dmx_Set_S *DmxSet, HI_U32 u32MaxTime)
{
    U_CLR_WAIT_TIME unWaitTime;

    unWaitTime.u32 = DMX_READ_REG(DmxSet->IoBase, CLR_WAIT_TIME);
    unWaitTime.bits.clr_wait_time = u32MaxTime & 0xffff;
    DMX_WRITE_REG(DmxSet->IoBase, CLR_WAIT_TIME, unWaitTime.u32);
}

#ifndef DMX_SCD_VERSION_2
/***********************************************************************************
* Function      :  DmxHalSetScdFilter
* Description   :    set SCD filter content
* Input         : u32FltId u8Content
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_S32 DmxHalSetScdFilter(Dmx_Set_S *DmxSet, HI_U32 u32FltId, HI_U8 u8Content)
{
    U_SCD_FLT filter;
    HI_U32 offset = 0;

    BUG_ON(u32FltId >= DMX_SCD_BYTE_FILTER_CNT);

    offset = u32FltId >> 2;

    offset <<= 2;

    u32FltId -= offset;

    filter.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_FLT0_3 + offset);

    switch (u32FltId)
    {
    case 0:
        filter.bits.flt0 = u8Content;
        break;
    case 1:
        filter.bits.flt1 = u8Content;
        break;
    case 2:
        filter.bits.flt2 = u8Content;
        break;
    case 3:
        filter.bits.flt3 = u8Content;
        break;
    default:
        return HI_FAILURE;
    }

    DMX_WRITE_REG(DmxSet->IoBase, SCD_FLT0_3 + offset, filter.u32);

    DMX_COM_EQUAL(filter.u32, DMX_READ_REG(DmxSet->IoBase, DmxSet->IoBase, SCD_FLT0_3 + offset));

    return HI_SUCCESS;
}

/***********************************************************************************
* Function      :  DmxHalSetScdRangeFilter
* Description   :    set SCD filter range
* Input         : u32FltId u8High u8Low
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_S32 DmxHalSetScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 u32FltId, HI_U8 u8High, HI_U8 u8Low)
{
    U_SCD_FLT filter;
    HI_U32 offset = 0;

    BUG_ON(u32FltId >= DMX_SCD_RANGE_FILTER_CNT);

    /*calculate the offset of each scd filter*/
    switch (u32FltId)
    {
    case 1:
    case 2:
        offset = 4;
        break;
    case 3:
    case 4:
        offset = 8;
        break;
    case 5:
    case 6:
        offset = 12;
        break;
    case 0:
    default:
        offset = 0;
        break;
    }

    filter.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_FLT8_11 + offset);

    switch ((u32FltId + 1) % 2)
    {
    case 0:
        filter.bits.flt0 = u8Low;
        filter.bits.flt1 = u8High;
        break;
    case 1:
        filter.bits.flt2 = u8Low;
        filter.bits.flt3 = u8High;
        break;
    }

    DMX_WRITE_REG(DmxSet->IoBase, SCD_FLT8_11 + offset, filter.u32);

    DMX_COM_EQUAL(filter.u32, DMX_READ_REG(DmxSet->IoBase, SCD_FLT8_11 + offset));

    return HI_SUCCESS;
}

/***********************************************************************************
* Function      : DmxHalSetScdNewRangeFilter
* Description   : set SCD new filter range
* Input         : FilterId, High, Low, Mask, Negate
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetScdNewRangeFilter(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U8 High, HI_U8 Low, HI_U8 Mask, HI_BOOL Negate)
{
    U_SCD_FLT_V200  filter;
    HI_U32          new_flt_neg;

    filter.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_NEW_FLTSET(FilterId));
    filter.bits.byte_h  = High;
    filter.bits.byte_l  = Low;
    filter.bits.mask    = Mask;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_NEW_FLTSET(FilterId), filter.u32);

    DMX_COM_EQUAL(filter.u32, DMX_READ_REG(DmxSet->IoBase, SCD_NEW_FLTSET(FilterId)));

    new_flt_neg = DMX_READ_REG(DmxSet->IoBase, SCD_NEW_FLT_NEG);
    if (Negate)
    {
        new_flt_neg |= (1 << FilterId);
    }
    else
    {
        new_flt_neg &= ~(1 << FilterId);
    }

    DMX_WRITE_REG(DmxSet->IoBase, SCD_NEW_FLT_NEG, new_flt_neg);

    DMX_COM_EQUAL(new_flt_neg, DMX_READ_REG(DmxSet->IoBase, SCD_NEW_FLT_NEG));
}

/***********************************************************************************
* Function      : DmxHalEnScdFilter
* Description   : Choose Scd Filter
* Input         : ScdId, FilterId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalEnScdFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 FilterId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);
    BUG_ON(FilterId >= DMX_SCD_BYTE_FILTER_CNT);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    //Note: for 3716MV200, SCD's byte_flt_en(10bit) is opened always for all SCD.
    scd_set.bits.flt_en0 |= (1 << FilterId);

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalClrScdFilter
* Description   : Clear Scd Filter
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalClrScdFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.flt_en0 = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalEnScdRangeFilter
* Description   : Choose Scd Range Filter
* Input         : ScdId, FilterId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalEnScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 FilterId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);
    BUG_ON(FilterId >= DMX_SCD_RANGE_FILTER_CNT);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    if (FilterId < 2)
    {
        scd_set.bits.flt_en1 |= (1 << FilterId);
    }
    else
    {
        scd_set.bits.flt_en2 |= (1 << (FilterId - 2));
    }

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalClrScdRangeFilter
* Description   : Clear Scd Range Filter
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalClrScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.flt_en1 = 0;
    scd_set.bits.flt_en2 = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalEnScdNewRangeFilter
* Description   : Choose Scd New Range Filter
* Input         : ScdId, FilterId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalEnScdNewRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 FilterId)
{
    HI_U32 flt_en;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    flt_en = DMX_READ_REG(DmxSet->IoBase, SCD_NEW_FLTEN(ScdId));

    flt_en |= (1 << FilterId);
    DMX_WRITE_REG(DmxSet->IoBase, SCD_NEW_FLTEN(ScdId), flt_en);

    DMX_COM_EQUAL(flt_en, DMX_READ_REG(DmxSet->IoBase, SCD_NEW_FLTEN(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalClrScdNewRangeFilter
* Description   : Clear Scd New Range Filter
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalClrScdNewRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    HI_U32 flt_en;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    flt_en = DMX_READ_REG(DmxSet->IoBase, SCD_NEW_FLTEN(ScdId));

    flt_en &= 0xFFFF0000;
    DMX_WRITE_REG(DmxSet->IoBase, SCD_NEW_FLTEN(ScdId), flt_en);

    DMX_COM_EQUAL(flt_en, DMX_READ_REG(DmxSet->IoBase, SCD_NEW_FLTEN(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalEnablePesSCD
* Description   : enbale pes
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalEnablePesSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.pes_en = 1;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalDisablePesSCD
* Description   : disbale pes
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalDisablePesSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.pes_en = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalEnableEsSCD
* Description   : enbale es
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalEnableEsSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.esscd_en = 1;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalDisableEsSCD
* Description   : disbale es
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalDisableEsSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.esscd_en = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalEnableMp4SCD
* Description   : enbale m4_short_en
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalEnableMp4SCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.m4_short_en = 1;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalDisableMp4SCD
* Description   : disbale m4_short_en
* Input         : ScdId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalDisableMp4SCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.m4_short_en = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}


/***********************************************************************************
* Function      : DmxHalSetSCDAttachChannel
* Description   : set SCD channel
* Input         : ScdId ChanId
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalSetSCDAttachChannel(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 ChanId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);
    BUG_ON(DMX_INVALID_CHAN_ID == ChanId ? 0 : ChanId >= DmxSet->DmxChanCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId));

    scd_set.bits.scd_ch = ChanId;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SETa(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SETa(ScdId)));
}

/***********************************************************************************
* Function      : DmxHalSetSCDBufferId
* Description   : set SCD buffer id
* Input         : ScdId, OqId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetSCDBufferId(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 OqId)
{
    U_SCD_BUF scd_buf;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);
    BUG_ON(OqId >= DmxSet->DmxOqCnt);

    scd_buf.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_BUF0(ScdId));

    scd_buf.bits.buf_num = OqId & 0x7f;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_BUF0(ScdId), scd_buf.u32);

    DMX_COM_EQUAL(scd_buf.u32, DMX_READ_REG(DmxSet->IoBase, SCD_BUF0(ScdId)));
}

HI_VOID DmxHalFlushScdBuf(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_CLRBUF unScdClrBuf;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    unScdClrBuf.bits.buf_id = ScdId;
    DMX_WRITE_REG(DmxSet->IoBase, SCD_CLRBUF, unScdClrBuf.u32);
}

#else

HI_S32 DmxHalSetScdFilter(Dmx_Set_S *DmxSet, HI_U32 u32FltId, HI_U8 u8Content)
{
    U_SCD_FLT filter;
    HI_U32 grp_offset , offset;

    BUG_ON(u32FltId >= DMX_SCD_BYTE_FILTER_CNT);

    grp_offset = u32FltId >> 2;
    grp_offset <<= 2;

    offset = u32FltId - grp_offset;

    filter.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_BYTE_FLT(grp_offset));

    switch (offset)
    {
    case 0:
        filter.bits.flt0 = u8Content;
        break;
    case 1:
        filter.bits.flt1 = u8Content;
        break;
    case 2:
        filter.bits.flt2 = u8Content;
        break;
    case 3:
        filter.bits.flt3 = u8Content;
        break;
    default:
        BUG();
    }

    DMX_WRITE_REG(DmxSet->IoBase, SCD_BYTE_FLT(grp_offset), filter.u32);

    DMX_COM_EQUAL(filter.u32, DMX_READ_REG(DmxSet->IoBase, SCD_BYTE_FLT(grp_offset)));

    return HI_SUCCESS;
}

HI_VOID DmxHalEnScdFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 FilterId)
{
    U_SCD_BYTE_FLT_EN filter_en;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);
    BUG_ON(FilterId >= DMX_SCD_BYTE_FILTER_CNT);

    filter_en.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_BYTE_FLT_EN(ScdId));

    filter_en.u32 |= (1 << FilterId);

    DMX_WRITE_REG(DmxSet->IoBase, SCD_BYTE_FLT_EN(ScdId), filter_en.u32);

    DMX_COM_EQUAL(filter_en.u32, DMX_READ_REG(DmxSet->IoBase, SCD_BYTE_FLT_EN(ScdId)));
}

HI_VOID DmxHalClrScdFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_BYTE_FLT_EN filter_en;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    filter_en.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_BYTE_FLT_EN(ScdId));

    filter_en.u32 = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_BYTE_FLT_EN(ScdId), filter_en.u32);

    DMX_COM_EQUAL(filter_en.u32, DMX_READ_REG(DmxSet->IoBase, SCD_BYTE_FLT_EN(ScdId)));
}

HI_S32 DmxHalSetScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 u32FltId, HI_U8 u8High, HI_U8 u8Low)
{
    U_SCD_RANGE_FLT filter;

    BUG_ON(u32FltId >= DMX_SCD_RANGE_FILTER_CNT);

    filter.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_RANGE_FLT(u32FltId));

    filter.bits.lo = u8Low;
    filter.bits.hi = u8High;
    filter.bits.mask = 0xff;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_RANGE_FLT(u32FltId), filter.u32);

    DMX_COM_EQUAL(filter.u32, DMX_READ_REG(DmxSet->IoBase, SCD_RANGE_FLT(u32FltId)));

    return HI_SUCCESS;
}

HI_VOID DmxHalEnScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 FilterId)
{
    U_SCD_RANGE_FLT_EN filter_en;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);
    BUG_ON(FilterId >= DMX_SCD_RANGE_FILTER_CNT);

    filter_en.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_RANGE_FLT_EN(ScdId));

    filter_en.u32 |= (1 << FilterId);

    DMX_WRITE_REG(DmxSet->IoBase, SCD_RANGE_FLT_EN(ScdId), filter_en.u32);

    DMX_COM_EQUAL(filter_en.u32, DMX_READ_REG(DmxSet->IoBase, SCD_RANGE_FLT_EN(ScdId)));
}

HI_VOID DmxHalClrScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_RANGE_FLT_EN filter_en;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    filter_en.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_RANGE_FLT_EN(ScdId));

    filter_en.u32 = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_RANGE_FLT_EN(ScdId), filter_en.u32);

    DMX_COM_EQUAL(filter_en.u32, DMX_READ_REG(DmxSet->IoBase, SCD_RANGE_FLT_EN(ScdId)));
}

HI_VOID DmxHalEnablePesSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId));

    scd_set.bits.scd_pes_en = 1;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SET(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId)));
}

HI_VOID DmxHalDisablePesSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId));

    scd_set.bits.scd_pes_en = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SET(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId)));
}

HI_VOID DmxHalEnableEsSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId));

    scd_set.bits.scd_es_en = 1;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SET(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId)));
}

HI_VOID DmxHalDisableEsSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId));

    scd_set.bits.scd_es_en = 0;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SET(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId)));
}

HI_VOID DmxHalSetSCDAttachChannel(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 ChanId)
{
    U_SCD_SET scd_set;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);
    BUG_ON(DMX_INVALID_CHAN_ID == ChanId ? 0 : ChanId >= DmxSet->DmxChanCnt);

    scd_set.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId));

    if (DMX_INVALID_CHAN_ID != ChanId)
    {
        scd_set.bits.rec_pid_ch = ChanId;
        scd_set.bits.pid_ch_vld = 1;
    }
    else
    {
        scd_set.bits.rec_pid_ch = ChanId;
        scd_set.bits.pid_ch_vld = 0;
    }

    DMX_WRITE_REG(DmxSet->IoBase, SCD_SET(ScdId), scd_set.u32);

    DMX_COM_EQUAL(scd_set.u32, DMX_READ_REG(DmxSet->IoBase, SCD_SET(ScdId)));
}

HI_VOID DmxHalSetSCDBufferId(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 OqId)
{
    U_REC_BUF_SET scd_buf;

    BUG_ON(RecId >= DmxSet->DmxRecCnt);
    BUG_ON(DMX_INVALID_OQ_ID == OqId ? 0 : OqId >= DmxSet->DmxOqCnt);

    scd_buf.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_BUF_SET(RecId));

    if (DMX_INVALID_OQ_ID != OqId)
    {
        scd_buf.bits.rec_buf_id = OqId;
        scd_buf.bits.rec_buf_vld = 1;
    }
    else
    {
        scd_buf.bits.rec_buf_id = OqId;
        scd_buf.bits.rec_buf_vld = 0;
    }

    DMX_WRITE_REG(DmxSet->IoBase, SCD_BUF_SET(RecId), scd_buf.u32);

    DMX_COM_EQUAL(scd_buf.u32, DMX_READ_REG(DmxSet->IoBase, SCD_BUF_SET(RecId)));
}

HI_VOID DmxHalFlushScdBuf(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    U_SCD_CLRBUF unScdClrBuf;

    BUG_ON(RecId >= DmxSet->DmxRecCnt);

    unScdClrBuf.bits.buf_id = RecId;
    DMX_WRITE_REG(DmxSet->IoBase, SCD_CLRBUF, unScdClrBuf.u32);
}

#endif

HI_VOID DmxHalFlushScdChn(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    U_SCD_CLRCH unScdClrCh;

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    unScdClrCh.bits.ch_id = ScdId;

    DMX_WRITE_REG(DmxSet->IoBase, SCD_CLRCH, unScdClrCh.u32);
}

/***********************************************************************************
* Function      :  DmxHalSetFlushIPPort
* Description   :  Set Flush IP Port
* Input         : u32PortId
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalSetFlushIPPort(Dmx_Set_S *DmxSet, HI_U32 PortId)
{
    U_IP_CLRCHN_REQ ip_flush;
    HI_U32 RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    ip_flush.u32 = DMX_READ_REG(DmxSet->IoBase, IP_CLRCHN_REQ(RawPortId));
    ip_flush.bits.ip_clrchn_req = 1;
    DMX_WRITE_REG(DmxSet->IoBase, IP_CLRCHN_REQ(RawPortId), ip_flush.u32);
}

/***********************************************************************************
* Function      :  DmxHalGetChannelTSScrambleFlag
* Description   :   Get Channel CRC Mode
* Input         : chid (= 0,1,2)
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalGetChannelTSScrambleFlag(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL  *pEnable)
{
    U_CH_HIS ch_sramble;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    ch_sramble.u32 = DMX_READ_REG(DmxSet->IoBase, CH_HIS(ChanId));
    *pEnable = (ch_sramble.bits.ts_scr_flag & 0x1);
}

/***********************************************************************************
* Function      :  DmxHalGetChannelPesScrambleFlag
* Description   :   Get Channel CRC Mode
* Input         : chid (= 0,1,2)
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalGetChannelPesScrambleFlag(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL  *pEnable)
{
    U_CH_HIS ch_sramble;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    ch_sramble.u32 = DMX_READ_REG(DmxSet->IoBase, CH_HIS(ChanId));
    *pEnable = (ch_sramble.bits.pes_scr_flag & 0x1);
}

HI_VOID DmxHalGetChannelDescStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL *DoScram, HI_U32 *KeyId)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    *DoScram = dmx_pidctrl.bits.do_scram ? HI_TRUE : HI_FALSE;
    *KeyId = dmx_pidctrl.bits.cw_index;
}

/***********************************************************************************
* Function      : DmxHalClrAutoIPBP
* Description   : IP Auto Clear BP
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalIPPortAutoClearBP(Dmx_Set_S * DmxSet)
{
    U_IP_BP_CLR_CFG reg;

    reg.value = DMX_READ_REG(DmxSet->IoBase, IP_BP_CLR_CFG);

    reg.bits.ipaful_clr_ena = 0;
    DMX_WRITE_REG(DmxSet->IoBase, IP_BP_CLR_CFG, reg.value);

    DMX_COM_EQUAL(reg.value, DMX_READ_REG(DmxSet->IoBase, IP_BP_CLR_CFG));
}

/***********************************************************************************
* Function      : DmxHalGetIPBPStatus
* Description   : Get IP BP Status
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_BOOL DmxHalGetIPBPStatus(Dmx_Set_S *DmxSet, HI_U32 PortId)
{
    HI_SIZE_T u32LockFlag;
    HI_U32 status32;
    HI_U32 status40;
    HI_U32 regaddr;
    HI_U32 RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    IP_BP_FQ_STA(RawPortId, 0,regaddr);
    status32 = DMX_READ_REG(DmxSet->IoBase, regaddr);
    IP_BP_FQ_STA(RawPortId, 1,regaddr);
    status40 = DMX_READ_REG(DmxSet->IoBase, regaddr) & 0xff;

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);

    if (status32 || status40)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

/***********************************************************************************
* Function      : DmxHalClrIPBPStatus
* Description   : clear IP BP Status
* Input         : PortId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalClrIPBPStatus(Dmx_Set_S *DmxSet, HI_U32 PortId)
{
    HI_U32 regaddr;
    HI_SIZE_T u32LockFlag;
    HI_U32 RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    IP_BP_FQ_STA(RawPortId, 0,regaddr);
    DMX_WRITE_REG(DmxSet->IoBase, regaddr, 0);

    IP_BP_FQ_STA(RawPortId, 1,regaddr);
    DMX_WRITE_REG(DmxSet->IoBase, regaddr, 0);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      : DmxHalClrIPFqBPStatus
* Description   : clear IP BP Status for one fq
* Input         : PortId, FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalClrIPFqBPStatus(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 FQId)
{
    HI_U32  offset  = FQId >> 5;
    HI_U32  bit     = FQId & 0x1F;
    HI_U32  value;
    HI_U32 regaddr;
    HI_U32 RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);
    HI_SIZE_T u32LockFlag;

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);
    BUG_ON(FQId >= DmxSet->DmxFqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    IP_BP_FQ_STA(RawPortId, offset,regaddr);
    value = DMX_READ_REG(DmxSet->IoBase, regaddr);

    value &= ~(1 << bit);
    DMX_WRITE_REG(DmxSet->IoBase, regaddr, value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      : DmxHalSetPcrDmxId
* Description   : Set PCR DmxId
* Input         : PcrId, DmxId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetPcrDmxId(Dmx_Set_S *DmxSet, const HI_U32 PcrId, const HI_U32 DmxId)
{
    U_DMX_PCR_SET PcrSet;
    HI_U32 RawDmxId ;

    BUG_ON(PcrId >= DmxSet->DmxPcrCnt);

    RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    PcrSet.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PCR_SET(PcrId));

    PcrSet.bits.pcr_dmx_id = RawDmxId + 1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PCR_SET(PcrId), PcrSet.u32);

    DMX_COM_EQUAL(PcrSet.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PCR_SET(PcrId)));
}

/***********************************************************************************
* Function      : DmxHalSetPcrPid
* Description   : Set PCR Pid
* Input         : PcrId, PcrPid
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetPcrPid(Dmx_Set_S *DmxSet, const HI_U32 PcrId, const HI_U32 PcrPid)
{
    U_DMX_PCR_SET PcrSet;

    BUG_ON(PcrId >= DmxSet->DmxPcrCnt);

    PcrSet.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PCR_SET(PcrId));

    PcrSet.bits.pcr_pid = PcrPid & 0x1fff;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PCR_SET(PcrId), PcrSet.u32);

    DMX_COM_EQUAL(PcrSet.u32, DMX_READ_REG(DmxSet->IoBase, DMX_PCR_SET(PcrId)));
}

/***********************************************************************************
* Function      : DmxHalGetPcrValue
* Description   : Get PCR value
* Input         : PcrId, PcrVal
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalGetPcrValue(Dmx_Set_S *DmxSet, const HI_U32 PcrId, HI_U64 *PcrVal)
{
    HI_U64 value0 = 0, value1 = 0;

    BUG_ON(PcrId >= DmxSet->DmxPcrCnt);

    //PCR value(extend bit and high bit)register 0,0x3F00+0x4*j
    //pcr_extra_8_0 16:8    RO  0x000   PCR extend bit
    //reserved       7:1    RO  0x00    reserved
    //pcr_base_32      0    RO  0x0     PCR_base the highest bit
    //PCR value(low 32 bits) register 1 0x3F04+0x4*j
    //pcr_base_31_0 31:0    RO  0x00000000  PCR_base low 32 bits

    value0 = DMX_READ_REG(DmxSet->IoBase, DMX_CH_PCR_VALUE0(PcrId));
    value1 = DMX_READ_REG(DmxSet->IoBase, DMX_CH_PCR_VALUE1(PcrId));

    //*PcrVal = ((((value0&0xff00)>>7) +(value0&0x1))<<32) + value1;
    *PcrVal = ((value0 & 0x1) << 32) + value1;
}

/***********************************************************************************
* Function      : DmxHalGetScrValue
* Description   : Get SCR value
* Input         : PcrId, ScrVal
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalGetScrValue(Dmx_Set_S *DmxSet, const HI_U32 PcrId, HI_U64 *ScrVal)
{
    HI_U64 value0 = 0, value1 = 0;

    BUG_ON(PcrId >= DmxSet->DmxPcrCnt);

    //when PCR incoming SCRvalue(extend bit and high bit) register 0    0x3F08+0x4*j
    //scr_extra_8_0 16:8    RO  0x000   SCR extend bit
    //reserved  7:1 RO  0x00    reserved
    //scr_base_32   0   RO  0x0 SCR_base the highest bit

    //when PCR incoming SCR value(low 32 bits) register 1   0x3F0C+0x4*j
    //scr_base_31_0 31:0    RO  0x00000000  SCR_base low 32 bits

    value0 = DMX_READ_REG(DmxSet->IoBase, DMX_CH_SCR_VALUE0(PcrId));
    value1 = DMX_READ_REG(DmxSet->IoBase, DMX_CH_SCR_VALUE1(PcrId));

    //*ScrVal = ((((value0&0xff00)>>7) +(value0&0x1))<<32) + value1;
    *ScrVal = ((value0 & 0x1) << 32) + value1;
}

/***********************************************************************************
* Function      :  DmxHalGetTotalTeiIntStatus
* Description   :    Get Total Sync Status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetTotalTeiIntStatus(Dmx_Set_S *DmxSet)
{
    U_PVR_INT_SCAN unPvrIntSta;

    unPvrIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, PVR_INT_SCAN);
    return unPvrIntSta.bits.total_int_err;
}

/***********************************************************************************
* Function      :  DmxHalGetTotalPcrIntStatus
* Description   :    Get Total pcr int  Status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetTotalPcrIntStatus(Dmx_Set_S *DmxSet)
{
    U_PVR_INT_SCAN unPvrIntSta;

    unPvrIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, PVR_INT_SCAN);
    return unPvrIntSta.bits.total_int_pcr;
}

/***********************************************************************************
* Function      :  DmxHalGetTotalDiscIntStatus
* Description   :    Get Total Disc int Status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetTotalDiscIntStatus(Dmx_Set_S *DmxSet)
{
    U_PVR_INT_SCAN unPvrIntSta;

    unPvrIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, PVR_INT_SCAN);
    return unPvrIntSta.bits.total_int_disc;
}

/***********************************************************************************
* Function      :  DmxHalGetTotalCrcIntStatus
* Description   :    Get Total crc int Status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetTotalCrcIntStatus(Dmx_Set_S *DmxSet)
{
    U_PVR_INT_SCAN unPvrIntSta;

    unPvrIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, PVR_INT_SCAN);
    return unPvrIntSta.bits.total_int_fltcrc;
}

/***********************************************************************************
* Function      :  DmxHalGetTotalPenLenIntStatus
* Description   :    Get Total pes len int Status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetTotalPenLenIntStatus(Dmx_Set_S *DmxSet)
{
    U_PVR_INT_SCAN unPvrIntSta;

    unPvrIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, PVR_INT_SCAN);
    return unPvrIntSta.bits.total_int_peslen;
}

/***********************************************************************************
* Function      : DmxHalGetPcrIntStatus
* Description   : Get Pcr Int Status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetPcrIntStatus(Dmx_Set_S *DmxSet)
{
    U_STA_PCR_ARRI PcrIntSta;

    PcrIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, STA_PCR_ARRI);

    return PcrIntSta.bits.sta_pcr;
}

/***********************************************************************************
* Function      : DmxHalClrPcrIntStatus
* Description   : Clean Pcr Int
* Input         : PcrId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalClrPcrIntStatus(Dmx_Set_S *DmxSet, const HI_U32 PcrId)
{
    DMX_WRITE_REG(DmxSet->IoBase, RAW_PCR_ARRI, (1 << PcrId));
}

/***********************************************************************************
* Function      : DmxHalSetPcrIntEnable
* Description   : Set Pcr Int Enable
* Input         : PcrId, Enable
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetPcrIntEnable(Dmx_Set_S *DmxSet, const HI_U32 PcrId, const HI_BOOL Enable)
{
    U_ENA_PCR_ARRI  EnaPcrAttr;

    BUG_ON(PcrId >= DmxSet->DmxPcrCnt);

    EnaPcrAttr.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_PCR_ARRI);

    if (Enable)
    {
        EnaPcrAttr.bits.ena_pcr |= (1 << PcrId);
    }
    else
    {
        EnaPcrAttr.bits.ena_pcr &= ~(1 << PcrId);
    }

    DMX_WRITE_REG(DmxSet->IoBase, ENA_PCR_ARRI, EnaPcrAttr.u32);
}

/***********************************************************************************
* Function      :  DmxHalGetTeiIntInfo
* Description   :    Get Tei Int Info
* Input         :
* Output        :   pu32DmxId , pu32ChanId
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalGetTeiIntInfo(Dmx_Set_S *DmxSet, HI_U32* pu32DmxId, HI_U32* pu32ChanId)
{
    if (pu32DmxId && pu32ChanId)
    {
        U_STA_TEI unTeiIntSta;

        unTeiIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, STA_TEI);

        *pu32DmxId  = unTeiIntSta.bits.tei_dmx;
        *pu32ChanId = unTeiIntSta.bits.tei_ch;
    }
}

/***********************************************************************************
* Function      :  DmxHalClrTeiIntStatus
* Description   :    Clr Tei Int Status
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalClrTeiIntStatus(Dmx_Set_S *DmxSet)
{
    DMX_WRITE_REG(DmxSet->IoBase, RAW_TEI, 1);
}

/***********************************************************************************
* Function      : DmxHalGetDiscIntStatus
* Description   : Get disc int status
* Input         : RegionNum
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetDiscIntStatus(Dmx_Set_S *DmxSet, HI_U32 RegionNum)
{
    return DMX_READ_REG(DmxSet->IoBase, STA_DISC0(RegionNum));
}

/***********************************************************************************
* Function      : DmxHalClearDiscIntStatus
* Description   : Clr Disc int status
* Input         : ChanId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalClearDiscIntStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_U32  offset  = ChanId >> 5;
    HI_U32  bit     = ChanId & 0x1f;
    HI_U32  value   = (1 << bit);

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    DMX_WRITE_REG(DmxSet->IoBase, RAW_DISC0(offset), value);
}

/***********************************************************************************
* Function      : DmxHalGetCrcIntStatus
* Description   : Get Crc int status
* Input         : RegionNum
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetCrcIntStatus(Dmx_Set_S *DmxSet, HI_U32 RegionNum)
{
    return DMX_READ_REG(DmxSet->IoBase, STA_FLTCRC0(RegionNum));
}

/***********************************************************************************
* Function      : DmxHalClearCrcIntStatus
* Description   : Clr Crc int status
* Input         : ChanId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalClearCrcIntStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_U32  offset  = ChanId >> 5;
    HI_U32  bit     = ChanId & 0x1f;
    HI_U32  value   = (1 << bit);

    DMX_WRITE_REG(DmxSet->IoBase, RAW_FLTCRC0(offset), value);
}

/***********************************************************************************
* Function      : DmxHalGetPesLenIntStatus
* Description   : Get Pes Len int status
* Input         : RegionNum
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetPesLenIntStatus(Dmx_Set_S *DmxSet, HI_U32 RegionNum)
{
    return DMX_READ_REG(DmxSet->IoBase, STA_PES_LEN0(RegionNum));
}

/***********************************************************************************
* Function      : DmxHalClearPesLenIntStatus
* Description   : Clr Pes Len int status
* Input         : ChanId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalClearPesLenIntStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_U32  offset  = ChanId >> 5;
    HI_U32  bit     = ChanId & 0x1f;
    HI_U32  value   = (1 << bit);

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    DMX_WRITE_REG(DmxSet->IoBase, RAW_PES_LEN0(offset), value);
}

HI_VOID DmxHalClearRecTsCnt(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    BUG_ON(RecId >= DmxSet->DmxRecCnt);

    if (RecId < 6)
    {
        DMX_WRITE_REG(DmxSet->IoBase, TS_CNT0_5_L(RecId), 0);
        DMX_WRITE_REG(DmxSet->IoBase, TS_CNT0_5_H(RecId), 0);
    }
    else
    {
        DMX_WRITE_REG(DmxSet->IoBase, TS_CNT6_31_L(RecId - 6), 0);
        DMX_WRITE_REG(DmxSet->IoBase, TS_CNT6_31_H(RecId - 6), 0);
    }
}

HI_VOID DmxHalGetRecTsCnt(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U64 *TsCnt)
{
    HI_U64 TsCntLow;
    HI_U64 TsCntHigh;

    BUG_ON(RecId >= DmxSet->DmxRecCnt);

    if (RecId < 6)
    {
        TsCntLow = DMX_READ_REG(DmxSet->IoBase, TS_CNT0_5_L(RecId));
        TsCntHigh = DMX_READ_REG(DmxSet->IoBase, TS_CNT0_5_H(RecId)) & 0xff;
    }
    else
    {
        TsCntLow = DMX_READ_REG(DmxSet->IoBase, TS_CNT6_31_L(RecId - 6));
        TsCntHigh = DMX_READ_REG(DmxSet->IoBase, TS_CNT6_31_H(RecId - 6)) & 0xff;
    }

    *TsCnt = TsCntLow | TsCntHigh << 32 ;
}

HI_VOID DmxHalGetCurrentSCR(Dmx_Set_S *DmxSet, HI_U32 *ScrClk)
{

    *ScrClk = DMX_READ_REG(DmxSet->IoBase, DMX_SCR_VALUE1);
}

#ifdef DMX_SUPPORT_DMX_CLK_DYNAMIC_TUNE
HI_VOID DmxHalDynamicTuneDmxClk(HI_U32 Reduce)
{
    U_PERI_CRG64 PeriCrg64;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    PeriCrg64.bits.sw_dmxclk_loaden = 0;

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;

    mb();

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    PeriCrg64.bits.sw_dmx_clk_div = Reduce & 0x1f;
    PeriCrg64.bits.sw_dmxclk_loaden = 1;

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}
#endif

#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
HI_VOID DmxHalConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;
    U_PERI_CRG64 PeriCrg64;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    /* reset demux */
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    mb();

    PeriCrg63.bits.pvr_bus_cken     = 1;
    PeriCrg63.bits.pvr_dmx_cken     = 1;
    PeriCrg63.bits.pvr_27m_cken     = 1;
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

#if defined(CHIP_TYPE_hi3716mv410)
    PeriCrg64.bits.pvr_dmx_clk_sel = 0x2; /* 200Mhz */
#endif
    PeriCrg64.bits.pvr_ts0_clk_sel  = 1;
#if defined(CHIP_TYPE_hi3716mv420)
    PeriCrg64.bits.pvr_ts1_clk_sel  = 1;
#endif

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}

HI_VOID DmxHalDeConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;
    U_PERI_CRG64 PeriCrg64;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    /* reset demux */
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    mb();

    PeriCrg63.bits.pvr_bus_cken     = 0;
    PeriCrg63.bits.pvr_dmx_cken     = 0;
    PeriCrg63.bits.pvr_27m_cken     = 0;
    PeriCrg63.bits.pvr_tsi1_cken    = 0;
    PeriCrg63.bits.pvr_tsi2_cken    = 0;
    PeriCrg63.bits.pvr_tsi3_cken    = 0;
    PeriCrg63.bits.pvr_tsi4_cken    = 0;
    PeriCrg63.bits.pvr_tsi5_cken    = 0;
#if defined(CHIP_TYPE_hi3716mv420)
    PeriCrg63.bits.pvr_tsi6_cken    = 0;
    PeriCrg63.bits.pvr_tsi7_cken    = 0;
#endif
    PeriCrg63.bits.pvr_ts0_cken     = 0;
    PeriCrg63.bits.pvr_tsout0_cken  = 0;
#if defined(CHIP_TYPE_hi3716mv420)
    PeriCrg63.bits.pvr_ts1_cken     = 0;
    PeriCrg63.bits.pvr_tsout1_cken  = 0;
#endif
    PeriCrg63.bits.pvr_srst_req     = 1;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

#if defined(CHIP_TYPE_hi3716mv410)
    PeriCrg64.bits.pvr_dmx_clk_sel = 0x2; /* 200Mhz */
#endif
    PeriCrg64.bits.pvr_ts0_clk_sel  = 0;
#if defined(CHIP_TYPE_hi3716mv420)
    PeriCrg64.bits.pvr_ts1_clk_sel  = 0;
#endif

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)
HI_VOID DmxHalConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    // reset demux
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    mb();

    PeriCrg63.bits.pvr_bus_cken     = 1;
    PeriCrg63.bits.pvr_dmx_cken     = 1;
    PeriCrg63.bits.pvr_27m_cken     = 1;
    PeriCrg63.bits.pvr_tsi1_cken    = 1;
    PeriCrg63.bits.pvr_tsi2_cken    = 1;

    PeriCrg63.bits.pvr_srst_req     = 0;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;
}

HI_VOID DmxHalDeConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    // reset demux
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    mb();

    PeriCrg63.bits.pvr_bus_cken     = 0;
    PeriCrg63.bits.pvr_dmx_cken     = 0;
    PeriCrg63.bits.pvr_27m_cken     = 0;
    PeriCrg63.bits.pvr_tsi1_cken    = 0;
    PeriCrg63.bits.pvr_tsi2_cken    = 0;

    PeriCrg63.bits.pvr_srst_req     = 1;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;
}

#elif defined(CHIP_TYPE_hi3798cv200)  \
   || defined(CHIP_TYPE_hi3798mv200)  \
   || defined(CHIP_TYPE_hi3798mv200_a)
HI_VOID DmxHalConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;
    U_PERI_CRG64 PeriCrg64;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    // reset demux
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    mb();

    PeriCrg63.bits.pvr_bus_cken     = 1;
    PeriCrg63.bits.pvr_dmx_cken     = 1;
    PeriCrg63.bits.pvr_27m_cken     = 1;
    PeriCrg63.bits.pvr_tsi1_cken    = 1;
    PeriCrg63.bits.pvr_tsi2_cken    = 1;
    PeriCrg63.bits.pvr_tsi3_cken    = 1;
    PeriCrg63.bits.pvr_tsi4_cken    = 1;
#if defined(CHIP_TYPE_hi3798cv200)
    PeriCrg63.bits.pvr_tsi5_cken    = 1;
    PeriCrg63.bits.pvr_tsi6_cken    = 1;
#endif
    PeriCrg63.bits.pvr_ts0_cken     = 1;
    PeriCrg63.bits.pvr_tsout0_cken  = 1;
#if defined(CHIP_TYPE_hi3798cv200)
    PeriCrg63.bits.pvr_ts1_cken     = 1;
    PeriCrg63.bits.pvr_tsout1_cken  = 1;
#endif
    PeriCrg63.bits.pvr_srst_req     = 0;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    PeriCrg64.bits.pvr_ts0_clk_sel  = 1;
#if defined(CHIP_TYPE_hi3798cv200)
    PeriCrg64.bits.pvr_ts1_clk_sel  = 1;
#endif

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}

HI_VOID DmxHalDeConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;
    U_PERI_CRG64 PeriCrg64;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    // reset demux
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    mb();

    PeriCrg63.bits.pvr_bus_cken     = 0;
    PeriCrg63.bits.pvr_dmx_cken     = 0;
    PeriCrg63.bits.pvr_27m_cken     = 0;
    PeriCrg63.bits.pvr_tsi1_cken    = 0;
    PeriCrg63.bits.pvr_tsi2_cken    = 0;
    PeriCrg63.bits.pvr_tsi3_cken    = 0;
    PeriCrg63.bits.pvr_tsi4_cken    = 0;
#if defined(CHIP_TYPE_hi3798cv200)
    PeriCrg63.bits.pvr_tsi5_cken    = 0;
    PeriCrg63.bits.pvr_tsi6_cken    = 0;
#endif
    PeriCrg63.bits.pvr_ts0_cken     = 0;
    PeriCrg63.bits.pvr_tsout0_cken  = 0;
#if defined(CHIP_TYPE_hi3798cv200)
    PeriCrg63.bits.pvr_ts1_cken     = 0;
    PeriCrg63.bits.pvr_tsout1_cken  = 0;
#endif
    PeriCrg63.bits.pvr_srst_req     = 1;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    PeriCrg64.bits.pvr_ts0_clk_sel  = 0;
#if defined(CHIP_TYPE_hi3798cv200)
    PeriCrg64.bits.pvr_ts1_clk_sel  = 0;
#endif

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}

#elif defined(CHIP_TYPE_hi3796mv200)
HI_VOID DmxHalConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;
    U_PERI_CRG64 PeriCrg64;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    // reset demux
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    mb();

    PeriCrg63.bits.pvr_bus_cken     = 1;
    PeriCrg63.bits.pvr_dmx_cken     = 1;
    PeriCrg63.bits.pvr_27m_cken     = 1;
    PeriCrg63.bits.pvr_tsi1_cken    = 1;
    PeriCrg63.bits.pvr_tsi2_cken    = 1;
    PeriCrg63.bits.pvr_tsi3_cken    = 1;
    PeriCrg63.bits.pvr_tsi4_cken    = 1;
    PeriCrg63.bits.pvr_tsi5_cken    = 1;
    PeriCrg63.bits.pvr_tsi6_cken    = 1;
    PeriCrg63.bits.pvr_tsi7_cken    = 1;
    PeriCrg63.bits.pvr_tsi8_cken    = 1;
    PeriCrg63.bits.pvr_ts0_cken     = 1;
    PeriCrg63.bits.pvr_tsout0_cken  = 1;
    PeriCrg63.bits.pvr_tsout1_cken  = 1;
    PeriCrg63.bits.pvr_srst_req     = 0;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    PeriCrg64.bits.pvr_ts0_clk_sel  = 1;
    PeriCrg64.bits.pvr_ts1_clk_sel  = 1;

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}

HI_VOID DmxHalDeConfigHardware(HI_VOID)
{
    U_PERI_CRG63 PeriCrg63;
    U_PERI_CRG64 PeriCrg64;

    PeriCrg63.u32 = g_pstRegCrg->PERI_CRG63.u32;

    // reset demux
    PeriCrg63.bits.pvr_srst_req = 1;
    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    mb();

    PeriCrg63.bits.pvr_bus_cken     = 0;
    PeriCrg63.bits.pvr_dmx_cken     = 0;
    PeriCrg63.bits.pvr_27m_cken     = 0;
    PeriCrg63.bits.pvr_tsi1_cken    = 0;
    PeriCrg63.bits.pvr_tsi2_cken    = 0;
    PeriCrg63.bits.pvr_tsi3_cken    = 0;
    PeriCrg63.bits.pvr_tsi4_cken    = 0;
    PeriCrg63.bits.pvr_tsi5_cken    = 0;
    PeriCrg63.bits.pvr_tsi6_cken    = 0;
    PeriCrg63.bits.pvr_tsi7_cken    = 0;
    PeriCrg63.bits.pvr_tsi8_cken    = 0;
    PeriCrg63.bits.pvr_ts0_cken     = 0;
    PeriCrg63.bits.pvr_tsout0_cken  = 0;
    PeriCrg63.bits.pvr_tsout1_cken  = 0;
    PeriCrg63.bits.pvr_srst_req     = 1;

    g_pstRegCrg->PERI_CRG63.u32 = PeriCrg63.u32;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;

    PeriCrg64.bits.pvr_ts0_clk_sel  = 0;
    PeriCrg64.bits.pvr_ts1_clk_sel  = 0;

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}

#endif

/***********************************************************************************
* Function      : DmxHalCfgTSOClk
* Description   : Config TSO clock pahse
* Input         :
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
#if (DMX_TSOPORT_CNT == 2)
HI_VOID DmxHalCfgTSOClk(HI_U32 PortId,HI_BOOL ClkReverse,HI_U32 enClk,HI_U32 ClkDiv)
{
    U_PERI_CRG64 PeriCrg64;
    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;
    if ( PortId == 0 )
    {
        PeriCrg64.bits.pvr_tsout0_pctrl = ClkReverse?1:0;
        PeriCrg64.bits.pvr_ts0_clk_sel  = enClk;
        PeriCrg64.bits.pvr_ts0_clk_div  = ClkDiv/2 - 1;
    }
    else if(PortId == 1)
    {
        PeriCrg64.bits.pvr_tsout1_pctrl = ClkReverse?1:0;
        PeriCrg64.bits.pvr_ts1_clk_sel  = enClk;
        PeriCrg64.bits.pvr_ts1_clk_div  = ClkDiv/2 - 1;
    }
    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}

/***********************************************************************************
* Function      : DmxHalGetTSOClkCfg
* Description   : Config TSO clock pahse
* Input         :
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalGetTSOClkCfg(HI_U32 PortId,HI_BOOL *ClkReverse,HI_U32 *enClk,HI_U32 *ClkDiv)
{
    U_PERI_CRG64 PeriCrg64;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;
    if ( PortId == 0 )
    {
        *ClkReverse = PeriCrg64.bits.pvr_tsout0_pctrl;
        *enClk      = PeriCrg64.bits.pvr_ts0_clk_sel;
        *ClkDiv     = (PeriCrg64.bits.pvr_ts0_clk_div + 1) * 2 ;
    }
    else if(PortId == 1)
    {
        *ClkReverse = PeriCrg64.bits.pvr_tsout0_pctrl;
        *enClk      = PeriCrg64.bits.pvr_ts1_clk_sel;
        *ClkDiv     = (PeriCrg64.bits.pvr_ts1_clk_div + 1) * 2 ;
    }
}
#elif (DMX_TSOPORT_CNT == 1)
HI_VOID DmxHalCfgTSOClk(HI_U32 PortId,HI_BOOL ClkReverse,HI_U32 enClk,HI_U32 ClkDiv)
{
    U_PERI_CRG64 PeriCrg64;
    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;
    if ( PortId == 0 )
    {
        PeriCrg64.bits.pvr_tsout0_pctrl = ClkReverse?1:0;
        PeriCrg64.bits.pvr_ts0_clk_sel  = enClk;
        PeriCrg64.bits.pvr_ts0_clk_div  = ClkDiv/2 - 1;
    }
    else
    {
        BUG();
    }

    g_pstRegCrg->PERI_CRG64.u32 = PeriCrg64.u32;
}
HI_VOID DmxHalGetTSOClkCfg(HI_U32 PortId,HI_BOOL *ClkReverse,HI_U32 *enClk,HI_U32 *ClkDiv)
{
    U_PERI_CRG64 PeriCrg64;

    PeriCrg64.u32 = g_pstRegCrg->PERI_CRG64.u32;
    if ( PortId == 0 )
    {
        *ClkReverse = PeriCrg64.bits.pvr_tsout0_pctrl;
        *enClk      = PeriCrg64.bits.pvr_ts0_clk_sel;
        *ClkDiv     = (PeriCrg64.bits.pvr_ts0_clk_div + 1) * 2 ;
    }
    else
    {
        BUG();
    }
}
#else
HI_VOID DmxHalCfgTSOClk(HI_U32 PortId,HI_BOOL ClkReverse,HI_U32 enClk,HI_U32 ClkDiv)
{
    BUG();
}
HI_VOID DmxHalGetTSOClkCfg(HI_U32 PortId,HI_BOOL *ClkReverse,HI_U32 *enClk,HI_U32 *ClkDiv)
{
    BUG();
}
#endif

/***********************************************************************************
* Function      : DmxHalAttachFilter
* Description   : Attach Filter to channel
* Input         : FilterId, ChanId
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalAttachFilter(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U32 ChanId)
{
    HI_U32  filter_en;
    HI_U32  value;
    HI_U32  offset;
    HI_U32  i;

    BUG_ON(FilterId >= DmxSet->DmxFilterCnt);
    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    value = DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_EN(ChanId));
    for (i = 0; i < 32; i++)
    {
        if (0 == (value & (1 << i)))
        {
            break;
        }
    }

    value = DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_ID(ChanId, i));

    offset = (i & 0x3) * 8;

    value &= ~(0xFF << offset);
    value |= FilterId << offset;

    DMX_WRITE_REG(DmxSet->IoBase, DMX_FILTER_ID(ChanId, i), value);

    filter_en = (1 << i);

    filter_en |= DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_EN(ChanId));

    DMX_WRITE_REG(DmxSet->IoBase, DMX_FILTER_EN(ChanId), filter_en);

    DMX_COM_EQUAL(filter_en, DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_EN(ChanId)));
}

/***********************************************************************************
* Function      : DmxHalDetachFilter
* Description   : Detach Filter from channel
* Input         : FilterId, ChanId
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalDetachFilter(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U32 ChanId)
{
    HI_U32  filter_en = ~(1 << (FilterId & 0x1F));

    HI_U32  value;
    HI_U32  i;

    BUG_ON(FilterId >= DmxSet->DmxFilterCnt);
    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    for (i = 0; i < 8; i++)
    {
        HI_U32 j;

        value = DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_ID(ChanId, i * 4));

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

    filter_en &= DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_EN(ChanId));

    DMX_WRITE_REG(DmxSet->IoBase, DMX_FILTER_EN(ChanId), filter_en);

    DMX_COM_EQUAL(filter_en, DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_EN(ChanId)));
}

/***********************************************************************************
* Function      :  DmxHalEnableOQOutDInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableOQOutDInt(HI_U32 OQId)
{
    #if 0
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1F;
    HI_U32  value;

    value  = DMX_READ_REG(ENA_OQ_DESC_0(offset));

    value |= 1 << bit;
    DMX_WRITE_REG(ENA_OQ_DESC_0(offset), value);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalDisableOQOutDInt
* Description   :    Set fix int enale
* Input         : u32OQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableOQOutDInt(HI_U32 OQId)
{
    #if 0
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1F;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value  = DMX_READ_REG(ENA_OQ_DESC_0(offset));

    value &= ~(1 << bit);
    DMX_WRITE_REG(ENA_OQ_DESC_0(offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalGetOQEopIntStatus
* Description   :    Get fix int status
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_BOOL DmxHalGetOQEopIntStatus(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;

    value = DMX_READ_REG(DmxSet->IoBase, INT_OQ_EOP_0(offset));

    if (value & (1 << bit))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

/*
 * Clear EOP raw interrupt state which always set when OQ received data.
 */
static inline HI_VOID __DmxHalClearOQEopIntStatus(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value = 1 << bit;

    DMX_WRITE_REG(DmxSet->IoBase, RAW_OQ_EOP_0(offset), value);
}

HI_VOID DmxHalClearOQEopIntStatus(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    __DmxHalClearOQEopIntStatus(DmxSet, OQId);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/*
 * enable EOP interrupt.
 */
static inline HI_VOID __DmxHalEnableOQEopInt(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;

    value  = DMX_READ_REG(DmxSet->IoBase, ENA_OQ_EOP_0(offset));
    value |= (1 << bit);
    DMX_WRITE_REG(DmxSet->IoBase, ENA_OQ_EOP_0(offset), value);
}

HI_VOID DmxHalEnableOQEopInt(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    if (0 == atomic_read(&DmxSet->DmxOqInfo[OQId].EopIntActiveCount))
    {
        /* enable OQ Eop interrupt */
        __DmxHalEnableOQEopInt(DmxSet, OQId);
    }

    atomic_inc(&DmxSet->DmxOqInfo[OQId].EopIntActiveCount);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);

    //HI_DBG_DEMUX("OQ channel(id:%d) EOP interrupt active usage count is [%d].\n", OQId, atomic_read(&DmxSet->DmxOqInfo[OQId].EopIntActiveCount));
}

/*
 * disable EOP interrupt.
 */
static inline HI_VOID __DmxHalDisableOQEopInt(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;

    value  = DMX_READ_REG(DmxSet->IoBase, ENA_OQ_EOP_0(offset));
    value &= ~(1 << bit);
    DMX_WRITE_REG(DmxSet->IoBase, ENA_OQ_EOP_0(offset), value);
}

HI_VOID DmxHalDisableOQEopInt(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    if (unlikely(0 == atomic_read(&DmxSet->DmxOqInfo[OQId].EopIntActiveCount)))
    {
        goto out; /* Eop interrupt has not enabled */
    }
    else
    {
        atomic_dec(&DmxSet->DmxOqInfo[OQId].EopIntActiveCount);

        if (0 == atomic_read(&DmxSet->DmxOqInfo[OQId].EopIntActiveCount))
        {
            /* disable OQ Eop int */
            __DmxHalDisableOQEopInt(DmxSet, OQId);
        }
    }

out:
    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);

    //HI_DBG_DEMUX("OQ channel(id:%d) EOP interrupt active usage count is [%d].\n", OQId, atomic_read(&DmxSet->DmxOqInfo[OQId].EopIntActiveCount));
}

HI_BOOL DmxHalOQGetOverflowIntStatus(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value = DMX_READ_REG(DmxSet->IoBase, INT_OQ_CHN_0(offset));

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);

    if (value & (1 << bit))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_VOID DmxHalOQClearOverflowInt(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value = 1 << bit;
    DMX_WRITE_REG(DmxSet->IoBase, RAW_OQ_CHN_0(offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      :  DmxHalOQEnableOverflowInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalOQEnableOverflowInt(HI_U32 OQId)
{
    #if 0
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value  = DMX_READ_REG(ENA_OQ_CHN_0(offset));

    value |= 1 << bit;
    DMX_WRITE_REG(ENA_OQ_CHN_0(offset), value);
    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalOQDisableOverflowInt
* Description   :    Set fix int enale
* Input         : u32OQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalOQDisableOverflowInt(HI_U32 OQId)
{
    #if 0
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value  = DMX_READ_REG(ENA_OQ_CHN_0(offset));

    value &= ~(1 << bit);
    DMX_WRITE_REG(ENA_OQ_CHN_0(offset), value);
    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
    #endif
}

HI_U32 DmxHalOQGetAllOutputIntStatus(Dmx_Set_S *DmxSet)
{
    U_INT_STA_TYPE unIntSta;

    unIntSta.u32 = DMX_READ_REG(DmxSet->IoBase, INT_STA_TYPE);

    return unIntSta.bits.iint_desc_o;
}

HI_U32 DmxHalOQGetOutputIntStatus(Dmx_Set_S *DmxSet, HI_U32 OqRegionId)
{
    return DMX_READ_REG(DmxSet->IoBase, INT_OQ_DESC_0(OqRegionId));
}

HI_VOID DmxHalOQEnableOutputInt(Dmx_Set_S *DmxSet, HI_U32 OQId, HI_BOOL Enable)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    // clear raw int
    value = 1 << bit;
    DMX_WRITE_REG(DmxSet->IoBase, RAW_OQ_DESC_0(offset), value);

    value = DMX_READ_REG(DmxSet->IoBase, ENA_OQ_DESC_0(offset));

    if (Enable)
    {
        value |= 1 << bit;
    }
    else
    {
        value &= ~(1 << bit);
    }
    DMX_WRITE_REG(DmxSet->IoBase, ENA_OQ_DESC_0(offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      :  DmxHalEnableFQOvflErrInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableFQOvflErrInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_fq_ovfl_err = 1;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalDisableFQOvflErrInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableFQOvflErrInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_fq_ovfl_err = 0;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalEnableFQOvflErrInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableOQOvflErrInt(HI_VOID)
{
     #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_oq_ovfl_err = 1;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalDisableOQOvflErrInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableOQOvflErrInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_oq_ovfl_err = 0;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalEnableFQCfgErrInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableFQCfgErrInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_fq_cfg_err = 1;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalDisableFQCfgErrInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableFQCfgErrInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_fq_cfg_err = 0;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalEnableFQDescErrInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableFQDescErrInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_fq_desc_err = 1;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalDisableFQDescErrInt
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableFQDescErrInt(HI_VOID)
{
    #if 0
    U_ENA_INT_TYPE unEnaErr;

    unEnaErr.u32 = DMX_READ_REG(ENA_INT_TYPE);
    unEnaErr.bits.iena_fq_desc_err = 0;
    DMX_WRITE_REG(ENA_INT_TYPE, unEnaErr.u32);
    #endif
}

/***********************************************************************************
* Function      :  DmxHalEnableAllDavInt
* Description   :    Enable All  Int
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableAllDavInt(Dmx_Set_S *DmxSet)
{
    U_ENA_INT_ALL ena_all_int;

    ena_all_int.u32 = DMX_READ_REG(DmxSet->IoBase, ENA_INT_ALL);
    ena_all_int.bits.iena_all = 1;
    DMX_WRITE_REG(DmxSet->IoBase, ENA_INT_ALL, ena_all_int.u32);
}

/***********************************************************************************
* Function      :  DmxHalEnableOQRecive
* Description   :    Set fix int enale
* Input         : u32OQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalEnableOQRecive(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value  = DMX_READ_REG(DmxSet->IoBase, OQ_ENA_0(offset));
    value |= 1 << bit;
    DMX_WRITE_REG(DmxSet->IoBase, OQ_ENA_0(offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);

}

/***********************************************************************************
* Function      :  DmxHalDisableOQRecive
* Description   :    Set fix int enale
* Input         : u32OQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDisableOQRecive(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value  = DMX_READ_REG(DmxSet->IoBase, OQ_ENA_0(offset));
    value &= ~(1 << bit);
    DMX_WRITE_REG(DmxSet->IoBase, OQ_ENA_0(offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);

}

HI_BOOL DmxHalGetOQEnableStatus(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_BOOL ret;
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value  = DMX_READ_REG(DmxSet->IoBase, OQ_ENA_0(offset));
    value &= 1 << bit;
    if (value)
    {
        ret =  HI_TRUE;
    }
    else
    {
        ret =  HI_FALSE;
    }

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
    return ret;
}

/***********************************************************************************
* Function      :  DmxHalFQEnableRecive
* Description   :    Set fix int enale
* Input         : u32FQId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalFQEnableRecive(Dmx_Set_S *DmxSet, HI_U32 FQId, HI_BOOL Enable)
{
    HI_U32  offset  = FQId >> 5;
    HI_U32  bit     = FQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(FQId >= DmxSet->DmxFqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value = DMX_READ_REG(DmxSet->IoBase, FQ_ENA_0(offset));

    if (Enable)
    {
        value |= 1 << bit;
    }
    else
    {
        value &= ~(1 << bit);
    }
    DMX_WRITE_REG(DmxSet->IoBase, FQ_ENA_0(offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_S32 DmxHalGetInitStatus(Dmx_Set_S * DmxSet)
{
    HI_S32 ret      = HI_FAILURE;
    HI_U32 FqStatus, OqStatus;
    unsigned long start = jiffies, end = start + HZ; /* 1s */

    do
    {
        FqStatus = DMX_READ_REG(DmxSet->IoBase, FQ_INIT_DONE) & DMX_MASK_BIT_0;
        OqStatus = DMX_READ_REG(DmxSet->IoBase, OQ_INIT_DONE) & DMX_MASK_BIT_0;

        udelay(10);
    }while((FqStatus || OqStatus) && time_in_range(jiffies, start, end));

    if (!FqStatus && !OqStatus)
    {
        ret = HI_SUCCESS;
    }

    return ret;
}

/***********************************************************************************
* Function      :  DmxHalSetFQWORDx
* Description   :    Set fix int enale
* Input         : u32FQId,u32Data
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetFQWORDx(Dmx_Set_S *DmxSet, HI_U32 FQId, HI_U32 Offset, HI_U32 Value)
{
    BUG_ON(FQId >= DmxSet->DmxFqCnt);

    wmb();/*sync the DDR*/
    switch (Offset)
    {
        case DMX_FQ_CTRL_OFFSET:
        {
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_FQ_WORD0(FQId), Value);
            break;
        }

        case DMX_FQ_RDVD_OFFSET:
        {
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_FQ_WORD1(FQId), Value);
            break;
        }

        case DMX_FQ_SZWR_OFFSET:
        {
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_FQ_WORD2(FQId), Value);
            break;
        }

        case DMX_FQ_START_OFFSET:
        {
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_FQ_WORD3(FQId), Value);
            break;
        }

        default:
        {
            break;
        }
    }
}

/***********************************************************************************
* Function      :  DmxHalGetFQWORDx
* Description   :    Set fix int enale
* Input         : u32FQId,u32Data
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalGetFQWORDx(Dmx_Set_S *DmxSet, HI_U32 FQId, HI_U32 Offset, HI_U32 *pu32Data)
{
    BUG_ON(FQId >= DmxSet->DmxFqCnt);

    switch (Offset)
    {
        case DMX_FQ_CTRL_OFFSET:
            *pu32Data = DMX_READ_REG(DmxSet->IoBase, ADDR_FQ_WORD0(FQId));
            break;

        case DMX_FQ_RDVD_OFFSET:
            *pu32Data = DMX_READ_REG(DmxSet->IoBase, ADDR_FQ_WORD1(FQId));
            break;

        case DMX_FQ_SZWR_OFFSET:
            *pu32Data = DMX_READ_REG(DmxSet->IoBase, ADDR_FQ_WORD2(FQId));
            break;

        case DMX_FQ_START_OFFSET:
            *pu32Data = DMX_READ_REG(DmxSet->IoBase, ADDR_FQ_WORD3(FQId));
            break;

        default:
        {
            break;
        }
    }
}

/***********************************************************************************
* Function      :  DmxHalSetFQWritePtr
* Description   :    Set fix int enale
* Input         : u32FQId,u32Data
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetFQWritePtr(Dmx_Set_S *DmxSet, HI_U32 FQId, HI_U32 WritePtr)
{
    BUG_ON(FQId >= DmxSet->DmxFqCnt);

    wmb();/*sync the DDR*/
    DMX_WRITE_REG(DmxSet->IoBase, ADDR_FQ_WORD2(FQId), WritePtr);
}

HI_VOID DmxHalGetFQReadWritePtr(Dmx_Set_S *DmxSet, HI_U32 FQId, HI_U32 *Read, HI_U32 *Write)
{
    HI_SIZE_T LockFlag;
    BUG_ON(FQId >= DmxSet->DmxFqCnt);

    spin_lock_irqsave(&DmxHalLock, LockFlag);

    if (Read)
    {
        *Read = DMX_READ_REG(DmxSet->IoBase, ADDR_FQ_WORD1(FQId)) & 0xffff;
    }

    if (Write)
    {
        *Write = DMX_READ_REG(DmxSet->IoBase, ADDR_FQ_WORD2(FQId)) & 0xffff;
    }

    spin_unlock_irqrestore(&DmxHalLock, LockFlag);
}

/***********************************************************************************
* Function      :  DmxHalSetOQWORDx
* Description   :    Set fix int enale
* Input         : u32FQId,u32Data
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetOQWORDx(Dmx_Set_S *DmxSet, HI_U32 OQId, HI_U32 Offset, HI_U32 Value)
{
    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    wmb();/*sync the DDR*/
    switch (Offset)
    {
        case DMX_OQ_RSV_OFFSET:
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD0(OQId), Value);
            break;

        case DMX_OQ_CTRL_OFFSET:
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD1(OQId), Value);
            break;

        case DMX_OQ_EOPWR_OFFSET:
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD2(OQId), Value);
            break;

        case DMX_OQ_SZUS_OFFSET:
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD3(OQId), Value);
            break;

        case DMX_OQ_SADDR_OFFSET:
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD4(OQId), Value);
            break;

        case DMX_OQ_RDWR_OFFSET:
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD5(OQId), Value);
            break;

        case DMX_OQ_CFG_OFFSET:
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD6(OQId), Value);
            break;

        case DMX_OQ_START_OFFSET:
            DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD7(OQId), Value);
            break;

        default:
            break;
    }
}

/***********************************************************************************
* Function      :  DmxHalGetOQWORDx
* Description   :    Set fix int enale
* Input         : OQId,u32Data
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalGetOQWORDx(Dmx_Set_S *DmxSet, HI_U32 OQId, HI_U32 Offset, HI_U32 *Value)
{
    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    switch (Offset)
    {
        case DMX_OQ_RSV_OFFSET:
            *Value = DMX_READ_REG(DmxSet->IoBase, ADDR_OQ_WORD0(OQId));
            break;

        case DMX_OQ_CTRL_OFFSET:
            *Value = DMX_READ_REG(DmxSet->IoBase, ADDR_OQ_WORD1(OQId));
            break;

        case DMX_OQ_EOPWR_OFFSET:
            *Value = DMX_READ_REG(DmxSet->IoBase, ADDR_OQ_WORD2(OQId));
            break;

        case DMX_OQ_SZUS_OFFSET:
            *Value = DMX_READ_REG(DmxSet->IoBase, ADDR_OQ_WORD3(OQId));
            break;

        case DMX_OQ_SADDR_OFFSET:
            *Value = DMX_READ_REG(DmxSet->IoBase, ADDR_OQ_WORD4(OQId));
            break;

        case DMX_OQ_RDWR_OFFSET:
            *Value = DMX_READ_REG(DmxSet->IoBase, ADDR_OQ_WORD5(OQId));
            break;

        case DMX_OQ_CFG_OFFSET:
            *Value = DMX_READ_REG(DmxSet->IoBase, ADDR_OQ_WORD6(OQId));
            break;

        case DMX_OQ_START_OFFSET:
            *Value = DMX_READ_REG(DmxSet->IoBase, ADDR_OQ_WORD7(OQId));
            break;

        default:
            break;
    }
}

//set oq description word mask bit, set bit to 1 for the needless bit
HI_VOID DxmHalSetOQRegMask(Dmx_Set_S *DmxSet, HI_U32 u32MaskValue)
{
    DMX_WRITE_REG(DmxSet->IoBase, OQ_WR_MASK, u32MaskValue & 0xf);
}

HI_U32 DxmHalGetOQRegMask(Dmx_Set_S *DmxSet)
{
    return DMX_READ_REG(DmxSet->IoBase, OQ_WR_MASK);
}

#ifdef DMX_MMU_SUPPORT
#ifdef DMX_MMU_VERSION_1
HI_VOID DmxHalEnableGblMmu(Dmx_Mmu_S *DmxMmu)
{
    HI_U32 CbTtbr = 0, ErrRdAddr = 0, ErrWrAddr = 0;
    SMMU_GLB_CTL value;
    HI_SIZE_T u32LockFlag;

    HI_DRV_SMMU_GetPageTableAddr(&CbTtbr, &ErrRdAddr, &ErrWrAddr);

    BUG_ON(0 == DmxMmu->CbTtbr ? 0 : DmxMmu->CbTtbr != CbTtbr);

    DmxMmu->CbTtbr = CbTtbr;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

#if defined(DMX_SECURE_CHANNEL_SUPPORT) && !defined(DMX_TEE_SUPPORT)
    DMX_WRITE_REG(DmxMmu->IoBase, SMMU_SCB_TTBR_OFFSET, CbTtbr);
#endif
    DMX_WRITE_REG(DmxMmu->IoBase, SMMU_CB_TTBR_OFFSET, CbTtbr);
    DMX_WRITE_REG(DmxMmu->IoBase, SMMU_ERR_RADDR_OFFSET, ErrRdAddr);
    DMX_WRITE_REG(DmxMmu->IoBase, SMMU_ERR_WRADDR_OFFSET, ErrWrAddr);

    value.u32 = DMX_READ_REG(DmxMmu->IoBase, SMMU_GLB_CTL_OFFSET);

    value.bits.glb_bypass = 0;
    value.bits.int_en = 1;

    DMX_WRITE_REG(DmxMmu->IoBase, SMMU_GLB_CTL_OFFSET, value.u32);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID DmxHalEnableMmu(Dmx_Mmu_S *DmxMmu)
{
}

HI_VOID DmxHalEnMmuInt(Dmx_Mmu_S *DmxMmu)
{
    SMMU_GLB_CTL value;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value.u32 = DMX_READ_REG(DmxMmu->IoBase, SMMU_GLB_CTL_OFFSET);

    value.bits.int_en = 1;

    DMX_WRITE_REG(DmxMmu->IoBase, SMMU_GLB_CTL_OFFSET, value.u32);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID DmxHalDisMmuInt(Dmx_Mmu_S *DmxMmu)
{
    SMMU_GLB_CTL value;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value.u32 = DMX_READ_REG(DmxMmu->IoBase, SMMU_GLB_CTL_OFFSET);

    value.bits.int_en = 0;

    DMX_WRITE_REG(DmxMmu->IoBase, SMMU_GLB_CTL_OFFSET, value.u32);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_U32 DmxHalGetMmuIntStatus(Dmx_Mmu_S *DmxMmu)
{
    return DMX_READ_REG(DmxMmu->IoBase, SMMU_INT_STA_OFFSET);
}

HI_VOID DmxHalClrMmuIntStatus(Dmx_Mmu_S *DmxMmu)
{
    DMX_WRITE_REG(DmxMmu->IoBase, SMMU_INT_CLR_OFFSET, 0xFFFFFFFF);
}

HI_VOID DmxHalGetMmuErrAddr(Dmx_Mmu_S *DmxMmu, HI_U32 *ErrAddr1, HI_U32 *ErrAddr2)
{
    *ErrAddr1 = DMX_READ_REG(DmxMmu->IoBase, SMMU_READ_ERR_ADDR);
    *ErrAddr2 = DMX_READ_REG(DmxMmu->IoBase, SMMU_WRITE_ERR_ADDR);
}
#elif defined(DMX_MMU_VERSION_2)
HI_VOID DmxHalEnableGblMmu(Dmx_Mmu_S *DmxMmu)
{
}

HI_VOID DmxHalEnableMmu(Dmx_Mmu_S *DmxMmu)
{
    HI_U32 CbTtbr = 0, ErrRdAddr = 0, ErrWrAddr = 0;
    MMU_ENA_INT MmuEnaInt;
    U_ENA_INT_TYPE EnaInt;
    HI_SIZE_T u32LockFlag;

    HI_DRV_SMMU_GetPageTableAddr(&CbTtbr, &ErrRdAddr, &ErrWrAddr);

    BUG_ON(0 == DmxMmu->CbTtbr ? 0 : DmxMmu->CbTtbr != CbTtbr);

    DmxMmu->CbTtbr = CbTtbr;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    DMX_WRITE_REG(DmxMmu->IoBase, MMU_TAB_NONSEC_OFFSET, CbTtbr);
#if defined(DMX_SECURE_CHANNEL_SUPPORT) && !defined(DMX_TEE_SUPPORT)
    DMX_WRITE_REG(DmxMmu->IoBase, MMU_TAB_SEC_OFFSET, CbTtbr);
#endif
    DMX_WRITE_REG(DmxMmu->IoBase, MMU_BYPASS_OFFSET, 0);

    MmuEnaInt.u32 = DMX_READ_REG(DmxMmu->IoBase, MMU_ENA_INT_OFFSET);
    MmuEnaInt.bits.ena_mmu_tab_err = 1;
    MmuEnaInt.bits.ena_bus_rep_err = 1;
    DMX_WRITE_REG(DmxMmu->IoBase, MMU_ENA_INT_OFFSET, MmuEnaInt.u32);

    EnaInt.u32 = DMX_READ_REG(DmxMmu->IoBase, ENA_INT_TYPE);
    EnaInt.bits.iena_mmu = 1;
    DMX_WRITE_REG(DmxMmu->IoBase, ENA_INT_TYPE, EnaInt.u32);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID DmxHalEnMmuInt(Dmx_Mmu_S *DmxMmu)
{
#if 0
    MMU_ENA_INT value;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value.u32 = DMX_READ_REG(DmxMmu->IoBase, MMU_ENA_INT_OFFSET);

    value.bits.ena_mmu_tab_err = 1;
    value.bits.ena_bus_rep_err = 1;

    DMX_WRITE_REG(DmxMmu->IoBase, MMU_ENA_INT_OFFSET, value.u32);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
#endif
}

HI_VOID DmxHalDisMmuInt(Dmx_Mmu_S *DmxMmu)
{
#if  0
    MMU_ENA_INT value;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value.u32 = DMX_READ_REG(DmxMmu->IoBase, MMU_ENA_INT_OFFSET);

    value.bits.ena_mmu_tab_err = 0;
    value.bits.ena_bus_rep_err = 0;

    DMX_WRITE_REG(DmxMmu->IoBase, MMU_ENA_INT_OFFSET, value.u32);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
#endif
}

HI_U32 DmxHalGetMmuIntStatus(Dmx_Mmu_S *DmxMmu)
{
    return DMX_READ_REG(DmxMmu->IoBase, MMU_STA_INT_OFFSET);
}

HI_VOID DmxHalClrMmuIntStatus(Dmx_Mmu_S *DmxMmu)
{
    MMU_RAW_INT value;

    value.u32 = DMX_READ_REG(DmxMmu->IoBase, MMU_RAW_INT_OFFSET);

    value.bits.raw_mmu_tab_err = 1;
    value.bits.raw_bus_rep_err = 1;

    DMX_WRITE_REG(DmxMmu->IoBase, MMU_RAW_INT_OFFSET, value.u32);
}

HI_VOID DmxHalGetMmuErrAddr(Dmx_Mmu_S *DmxMmu, HI_U32 *ErrAddr1, HI_U32 *ErrAddr2)
{
    MMU_ERR_PA PhyAddr;

    *ErrAddr1  = DMX_READ_REG(DmxMmu->IoBase,MMU_ERR_VA_OFFSET);

    PhyAddr.u32  = DMX_READ_REG(DmxMmu->IoBase,MMU_ERR_PA_OFFSET);
    *ErrAddr2 = PhyAddr.bits.dbg_mmu_err_mmu_pa;
}

HI_VOID DmxHalGetMmuErrInfo(Dmx_Mmu_S *DmxMmu, HI_U32 *ErrInfo)
{
   *ErrInfo = DMX_READ_REG(DmxMmu->IoBase, MMU_ERR_INFO_OFFSET);
}
#endif

HI_VOID DmxHalEnableOQMmu(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    /* enable oq smmu mode. */
    value = DMX_READ_REG(DmxSet->IoBase, ENA_OQ_SMMU_0(offset));
    value &= ~(1 << bit);
    DMX_WRITE_REG(DmxSet->IoBase, ENA_OQ_SMMU_0(offset), value);

    /* clr dummy operation. */
    value = (1 << bit);
    DMX_WRITE_REG(DmxSet->IoBase, CLR_OQ_SMMU_0(offset), value);

    /* FIXME: sw should wait clr dummy operation finished, but now hw dont support. */

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID DmxHalDisableOQMmu(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    HI_U32  offset  = OQId >> 5;
    HI_U32  bit     = OQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value = DMX_READ_REG(DmxSet->IoBase, ENA_OQ_SMMU_0(offset));
    value |= 1 << bit;
    DMX_WRITE_REG(DmxSet->IoBase, ENA_OQ_SMMU_0(offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID __DmxHalEnableRamMmu(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    HI_U32 value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    /* enable ram port smmu mode. */
    value = DMX_READ_REG(DmxSet->IoBase, ENA_IP_SMMU_0);
    value &= ~(1 << RawPortId);
    DMX_WRITE_REG(DmxSet->IoBase, ENA_IP_SMMU_0, value);

    /* clr dummy operation. */
    value = (1 << RawPortId);
    DMX_WRITE_REG(DmxSet->IoBase, CLR_IP_SMMU_0, value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID DmxHalEnableRamMmu(Dmx_Set_S *DmxSet, HI_U32 PortId)
{
    HI_U32 RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    return __DmxHalEnableRamMmu(DmxSet, RawPortId);
}

HI_VOID __DmxHalDisableRamMmu(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    HI_U32 value;
    HI_SIZE_T u32LockFlag;

    BUG_ON(DMX_INVALID_PORT_ID == RawPortId);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value = DMX_READ_REG(DmxSet->IoBase, ENA_IP_SMMU_0);
    value |= 1 << RawPortId;
    DMX_WRITE_REG(DmxSet->IoBase, ENA_IP_SMMU_0, value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

HI_VOID DmxHalDisableRamMmu(Dmx_Set_S *DmxSet, HI_U32 PortId)
{
    HI_U32 RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    return __DmxHalDisableRamMmu(DmxSet, RawPortId);
}
#endif

/***********************************************************************************
* Function      :  DmxHalSetOQReadPtr
* Description   :    Set fix int enale
* Input         : u32OQId,u32ReadPtr
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetOQReadPtr(Dmx_Set_S *DmxSet, HI_U32 OQId, HI_U32 ReadPtr)
{
    HI_U32 u32MaskValue;

    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    //wmb();/*sync the DDR*/

    ReadPtr = (ReadPtr & DMX_OQ_DEPTH) << 16;

    if (DmxHalGetOQEnableStatus(DmxSet, OQId) == HI_FALSE)
    {
        u32MaskValue = DxmHalGetOQRegMask(DmxSet);
        DxmHalSetOQRegMask(DmxSet, 0x3); //shield the needless bits, prevent from effecting other bits
        DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD5(OQId), ReadPtr);
        DxmHalSetOQRegMask(DmxSet, u32MaskValue);
    }
    else
    {
        DMX_WRITE_REG(DmxSet->IoBase, ADDR_OQ_WORD5(OQId), ReadPtr);
    }
}

/***********************************************************************************
* Function      :  DmxHalAttachIPBPFQ
* Description   :  Set Ip Back Push OQ and enable
* Input         : u32IPNum,BufferId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalAttachIPBPFQ(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 FQId)
{
    HI_U32  offset  = FQId >> 5;
    HI_U32  bit     = FQId & 0x1f;
    HI_U32  value;
    HI_SIZE_T u32LockFlag;
    HI_U32 RawRamId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    BUG_ON(DMX_INVALID_PORT_ID == RawRamId);
    BUG_ON(FQId >= DmxSet->DmxFqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value = DMX_READ_REG(DmxSet->IoBase, IP_BP_FQ_CFG(RawRamId, offset));

    value |= 1 << bit;
    DMX_WRITE_REG(DmxSet->IoBase, IP_BP_FQ_CFG(RawRamId, offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      :  DmxHalDetachIPBPFQ
* Description   :  Set Ip Back Push OQ and enable
* Input         : u32IPNum,BufferId
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalDetachIPBPFQ(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 FQId)
{
    HI_U32  offset  = FQId >> 5;
    HI_U32  bit     = FQId & 0x1f;
    HI_U32  value;
    HI_U32 RawRamId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);
    HI_SIZE_T u32LockFlag;

    BUG_ON(DMX_INVALID_PORT_ID == RawRamId);
    BUG_ON(FQId >= DmxSet->DmxFqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    value = DMX_READ_REG(DmxSet->IoBase, IP_BP_FQ_CFG(RawRamId, offset));

    value &= ~(1 << bit);
    DMX_WRITE_REG(DmxSet->IoBase, IP_BP_FQ_CFG(RawRamId, offset), value);

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      :  DmxHalSetRecTsCounter
* Description   :  Set Record Ts Counter and enable
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
#define __DmxHalSetRecTsCounter(u32GroupId, u32TsCounter, u32OqId)  do {\
    U_REC_TSCNT_CFG_##u32GroupId value;\
    value.u32 = DMX_READ_REG(DmxSet->IoBase, REC_TSCNT_CFG_##u32GroupId); \
    value.bits.tscnt##u32TsCounter##_oqid = u32OqId & 0x7f; \
    value.bits.tscnt##u32TsCounter##_ena = 1;\
    DMX_WRITE_REG(DmxSet->IoBase, REC_TSCNT_CFG_##u32GroupId, value.u32); \
}while(0);

HI_VOID DmxHalSetRecTsCounter(Dmx_Set_S *DmxSet, HI_U32 u32RecId, HI_U32 u32OqId)
{
    HI_SIZE_T u32LockFlag;

    BUG_ON(u32RecId >= DmxSet->DmxRecCnt);
    BUG_ON(u32OqId >= DmxSet->DmxOqCnt);

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    switch (u32RecId)
    {
    case 0:
        __DmxHalSetRecTsCounter(0, 0, u32OqId);
        break;
    case 1:
        __DmxHalSetRecTsCounter(0, 1, u32OqId);
        break;
    case 2:
        __DmxHalSetRecTsCounter(0, 2, u32OqId);
        break;
    case 3:
        __DmxHalSetRecTsCounter(0, 3, u32OqId);
        break;
    case 4:
        __DmxHalSetRecTsCounter(1, 4, u32OqId);
        break;
    case 5:
        __DmxHalSetRecTsCounter(1, 5, u32OqId);
        break;
    case 6:
        __DmxHalSetRecTsCounter(1, 6, u32OqId);
        break;
     case 7:
        __DmxHalSetRecTsCounter(1, 7, u32OqId);
        break;
    case 8:
        __DmxHalSetRecTsCounter(2, 8, u32OqId);
        break;
    case 9:
        __DmxHalSetRecTsCounter(2, 9, u32OqId);
        break;
    case 10:
        __DmxHalSetRecTsCounter(2, 10, u32OqId);
        break;
    case 11:
        __DmxHalSetRecTsCounter(2, 11, u32OqId);
        break;
    case 12:
        __DmxHalSetRecTsCounter(3, 12, u32OqId);
        break;
    case 13:
        __DmxHalSetRecTsCounter(3, 13, u32OqId);
        break;
    case 14:
        __DmxHalSetRecTsCounter(3, 14, u32OqId);
        break;
    case 15:
        __DmxHalSetRecTsCounter(3, 15, u32OqId);
        break;
    case 16:
        __DmxHalSetRecTsCounter(4, 16, u32OqId);
        break;
    case 17:
        __DmxHalSetRecTsCounter(4, 17, u32OqId);
        break;
    case 18:
        __DmxHalSetRecTsCounter(4, 18, u32OqId);
        break;
    case 19:
        __DmxHalSetRecTsCounter(4, 19, u32OqId);
        break;
    case 20:
        __DmxHalSetRecTsCounter(5, 20, u32OqId);
        break;
    case 21:
        __DmxHalSetRecTsCounter(5, 21, u32OqId);
        break;
    case 22:
        __DmxHalSetRecTsCounter(5, 22, u32OqId);
        break;
    case 23:
        __DmxHalSetRecTsCounter(5, 23, u32OqId);
        break;
    case 24:
        __DmxHalSetRecTsCounter(6, 24, u32OqId);
        break;
    case 25:
        __DmxHalSetRecTsCounter(6, 25, u32OqId);
        break;
    case 26:
        __DmxHalSetRecTsCounter(6, 26, u32OqId);
        break;
    case 27:
        __DmxHalSetRecTsCounter(6, 27, u32OqId);
        break;
    case 28:
        __DmxHalSetRecTsCounter(7, 28, u32OqId);
        break;
    case 29:
        __DmxHalSetRecTsCounter(7, 29, u32OqId);
        break;
    case 30:
        __DmxHalSetRecTsCounter(7, 30, u32OqId);
        break;
    case 31:
        __DmxHalSetRecTsCounter(7, 31, u32OqId);
        break;
    default:
        BUG();
        break;
    }

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      :  DmxHalSetRecTsCntReplace
* Description   :  Set Record Ts Counter replace
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetRecTsCntReplace(Dmx_Set_S *DmxSet, HI_U32 u32RecId)
{
    U_SCD_TSCNT_ENA unTsCntRpl;

    BUG_ON(u32RecId >= DmxSet->DmxRecCnt);

    unTsCntRpl.u32 = DMX_READ_REG(DmxSet->IoBase, SCD_TSCNT_ENA);
    switch (u32RecId)
    {
    case 0:
        unTsCntRpl.bits.tscnt0_rep_ena = 1;
        break;
    case 1:
        unTsCntRpl.bits.tscnt1_rep_ena = 1;
        break;
    case 2:
        unTsCntRpl.bits.tscnt2_rep_ena = 1;
        break;
    case 3:
        unTsCntRpl.bits.tscnt3_rep_ena = 1;
        break;
    case 4:
        unTsCntRpl.bits.tscnt4_rep_ena = 1;
        break;
    case 5:
        unTsCntRpl.bits.tscnt5_rep_ena = 1;
        break;
    case 6:
        unTsCntRpl.bits.tscnt6_rep_ena = 1;
        break;
     case 7:
        unTsCntRpl.bits.tscnt7_rep_ena = 1;
        break;
    case 8:
        unTsCntRpl.bits.tscnt8_rep_ena = 1;
        break;
    case 9:
        unTsCntRpl.bits.tscnt9_rep_ena = 1;
        break;
    case 10:
        unTsCntRpl.bits.tscnt10_rep_ena = 1;
        break;
    case 11:
        unTsCntRpl.bits.tscnt11_rep_ena = 1;
        break;
    case 12:
        unTsCntRpl.bits.tscnt12_rep_ena = 1;
        break;
    case 13:
        unTsCntRpl.bits.tscnt13_rep_ena = 1;
        break;
    case 14:
        unTsCntRpl.bits.tscnt14_rep_ena = 1;
        break;
    case 15:
        unTsCntRpl.bits.tscnt15_rep_ena = 1;
        break;
    case 16:
        unTsCntRpl.bits.tscnt16_rep_ena = 1;
        break;
    case 17:
        unTsCntRpl.bits.tscnt17_rep_ena = 1;
        break;
    case 18:
        unTsCntRpl.bits.tscnt18_rep_ena = 1;
        break;
    case 19:
        unTsCntRpl.bits.tscnt19_rep_ena = 1;
        break;
    case 20:
        unTsCntRpl.bits.tscnt20_rep_ena = 1;
        break;
    case 21:
        unTsCntRpl.bits.tscnt21_rep_ena = 1;
        break;
    case 22:
        unTsCntRpl.bits.tscnt22_rep_ena = 1;
        break;
    case 23:
        unTsCntRpl.bits.tscnt23_rep_ena = 1;
        break;
    case 24:
        unTsCntRpl.bits.tscnt24_rep_ena = 1;
        break;
    case 25:
        unTsCntRpl.bits.tscnt25_rep_ena = 1;
        break;
    case 26:
        unTsCntRpl.bits.tscnt26_rep_ena = 1;
        break;
    case 27:
        unTsCntRpl.bits.tscnt27_rep_ena = 1;
        break;
    case 28:
        unTsCntRpl.bits.tscnt28_rep_ena = 1;
        break;
    case 29:
        unTsCntRpl.bits.tscnt29_rep_ena = 1;
        break;
    case 30:
        unTsCntRpl.bits.tscnt30_rep_ena = 1;
        break;
    case 31:
       unTsCntRpl.bits.tscnt31_rep_ena = 1;
    default:
        return;
    }

    DMX_WRITE_REG(DmxSet->IoBase, SCD_TSCNT_ENA, unTsCntRpl.u32);
}

/*
 * Get shared rec pid channel drop flag.
 */
HI_U32 DmxHalGetDavDropTsFlag(Dmx_Set_S *DmxSet)
{
    return DMX_READ_REG(DmxSet->IoBase, DAV_DROP_TS_FLAG);
}

/*
 * clear dav drop ts flag.
 */
HI_VOID DmxHalClrDavDropTsFlag(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    BUG_ON(RecId >= DmxSet->DmxRecCnt);

    DMX_WRITE_REG(DmxSet->IoBase, DAV_DROP_TS_FLAG_CLR, RecId & 0xff);
}


/*
 * get dmx switch drop flag.
 */
 HI_U32 DmxHalGetSwitchDropTsFlag(Dmx_Set_S *DmxSet)
{
    return DMX_READ_REG(DmxSet->IoBase, DMX_SWITCH_STATE20);
}

/*
 * clear dmx switch drop ts flag.
 */
HI_VOID DmxHalClrSwitchDropTsFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId)
{
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);
    HI_U32 DropFlag;

    BUG_ON(RawDmxId >= DmxSet->DmxCnt);

    DropFlag = DMX_READ_REG(DmxSet->IoBase, DMX_SWITCH_STATE20);
    DropFlag &= ~(1 << RawDmxId);
    DMX_WRITE_REG(DmxSet->IoBase, DMX_SWITCH_STATE20, DropFlag);
}

/***********************************************************************************
* Function      :  DmxHalGetOqCounter
* Description   :  Get Oq Counter
* Data Accessed :  (Optional)
* Data Updated  :  (Optional)
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetOqCounter(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    return DMX_READ_REG(DmxSet->IoBase, ADDR_INT_CNT(OQId));
}

/***********************************************************************************
* Function      :  DmxHalResetOqCounter
* Description   :  Get Oq Counter
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalResetOqCounter(Dmx_Set_S *DmxSet, HI_U32 OQId)
{
    BUG_ON(OQId >= DmxSet->DmxOqCnt);

    DMX_WRITE_REG(DmxSet->IoBase, ADDR_INT_CNT(OQId), 0);
}

/***********************************************************************************
* Function      :  DmxHalGetChannelCounter
* Description   :  Get Oq Counter
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_U32 DmxHalGetChannelCounter(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    return DMX_READ_REG(DmxSet->IoBase, CHANNEL_TS_COUNT(ChanId));
}

/***********************************************************************************
* Function      :  DmxHalResetOqCounter
* Description   :  Get Oq Counter
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalResetChannelCounter(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    DMX_WRITE_REG(DmxSet->IoBase, CHANNEL_TS_COUNT(ChanId), 0);
}

/***********************************************************************************
* Function      :  DmxHalFilterSetSecStuffCtrl
* Description   :
* Input         :  Enable: 1 - receive the stuff not 0xff
*                          0 - do not receive the stuff not 0xff
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalFilterSetSecStuffCtrl(Dmx_Set_S * DmxSet, HI_BOOL Enable)
{
    U_DMX_FILTER_CTRL flt_ctrl;

    flt_ctrl.value = DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_CTRL);
    if (Enable)
    {
        flt_ctrl.bits.sec_stuff_nopusi_e = 1;
    }
    else
    {
        flt_ctrl.bits.sec_stuff_nopusi_e = 0;
    }

    DMX_WRITE_REG(DmxSet->IoBase, DMX_FILTER_CTRL, flt_ctrl.value);
    DMX_COM_EQUAL(flt_ctrl.value, DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_CTRL));
}

/***********************************************************************************
* Function      :  DmxHalSetTei
* Description   :
* Input         :
*
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetTei(Dmx_Set_S *DmxSet, HI_U32 u32DemuxID,HI_BOOL bCheckTei)
{
    U_DMX_GLB_CTRL1 glb_cfg1;
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, u32DemuxID);

    glb_cfg1.value = DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId));/*(0x3B00 + ((DmxId) << 4))*/
    if (bCheckTei)
    {
        glb_cfg1.bits.dmx_tei_ctrl = 1;
    }
    else
    {
        glb_cfg1.bits.dmx_tei_ctrl = 0;
    }
    DMX_WRITE_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId), glb_cfg1.value);
    DMX_COM_EQUAL(glb_cfg1.value, DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId)));
}

#ifdef DMX_FILTER_DEPTH_SUPPORT
/***********************************************************************************
* Function      : DmxHalFilterEnableDepth
* Description   : enable filter depth
* Input         :
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalFilterEnableDepth(Dmx_Set_S * DmxSet)
{
    U_DMX_FILTER_CTRL flt_ctrl;

    flt_ctrl.value = DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_CTRL);

    flt_ctrl.bits.minlen_discard_by_flt = 1;

    DMX_WRITE_REG(DmxSet->IoBase, DMX_FILTER_CTRL, flt_ctrl.value);

    DMX_COM_EQUAL(flt_ctrl.value, DMX_READ_REG(DmxSet->IoBase, DMX_FILTER_CTRL));
}

/***********************************************************************************
* Function      : DmxHalFilterSetDepth
* Description   : Set filter depth
* Input         : FilterId, Depth
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalFilterSetDepth(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U32 Depth)
{
    U_DMX_FILTER_NUM flt_num;

    BUG_ON(FilterId >= DmxSet->DmxFilterCnt);

    flt_num.value = DMX_READ_REG(DmxSet->IoBase, DMX_FLT_NUM(FilterId));

    flt_num.bits.flt_depth = Depth;

    DMX_WRITE_REG(DmxSet->IoBase, DMX_FLT_NUM(FilterId), flt_num.value);

    DMX_COM_EQUAL(flt_num.value, DMX_READ_REG(DmxSet->IoBase, DMX_FLT_NUM(FilterId)));
}
#endif

#ifdef DMX_REC_EXCLUDE_PID_SUPPORT
/***********************************************************************************
* Function      : DmxHalEnableAllRecExcludePid
* Description   : Enable all rec exclude pid function
* Input         :
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalEnableAllRecExcludePid(Dmx_Set_S *DmxSet, HI_U32 DmxID)
{
    U_DMX_GLB_CTRL1 glb_cfg1;
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxID);

    glb_cfg1.value = DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId));
    glb_cfg1.bits.dmx_allrec_neg_en = 1;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId), glb_cfg1.value);
    DMX_COM_EQUAL(glb_cfg1.value, DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId)));
}

/***********************************************************************************
* Function      : DmxHalDisableAllRecExcludePid
* Description   : Disable all rec exclude pid function
* Input         :
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalDisableAllRecExcludePid(Dmx_Set_S *DmxSet, HI_U32 DmxID)
{
    U_DMX_GLB_CTRL1 glb_cfg1;
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxID);

    glb_cfg1.value = DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId));
    glb_cfg1.bits.dmx_allrec_neg_en = 0;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId), glb_cfg1.value);

    DMX_COM_EQUAL(glb_cfg1.value, DMX_READ_REG(DmxSet->IoBase, DMX_GLB_CTRL1(RawDmxId)));
}

/***********************************************************************************
* Function      : DmxHalGetAllRecExcludePid
* Description   : Get all rec exclude pid
* Input         :
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalGetAllRecExcludePid(Dmx_Set_S *DmxSet, HI_U32 RecCfgID, HI_U32* DmxID, HI_U32* PID)
{
    U_STA_ALLREC_CFG allrec_cfg;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    allrec_cfg.value = DMX_READ_REG(DmxSet->IoBase, ALLREC_CFG0_29(RecCfgID));

    *DmxID = allrec_cfg.bits.recdel_pid_dmxid;
    *PID   = allrec_cfg.bits.recdel_pid_value;

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

/***********************************************************************************
* Function      : DmxHalSetAllRecExcludePid
* Description   : Set filter depth
* Input         : FilterId, Depth
* Output        :
* Return        :
* Others:       :
***********************************************************************************/
HI_VOID DmxHalSetAllRecExcludePid(Dmx_Set_S *DmxSet, HI_U32 RecCfgID, HI_U32 RawDmxID, HI_U32 PID)
{
    U_STA_ALLREC_CFG allrec_cfg;
    HI_SIZE_T u32LockFlag;

    spin_lock_irqsave(&DmxHalLock, u32LockFlag);

    allrec_cfg.value = DMX_READ_REG(DmxSet->IoBase, ALLREC_CFG0_29(RecCfgID));

    allrec_cfg.bits.recdel_pid_dmxid = RawDmxID;
    allrec_cfg.bits.recdel_pid_value = PID;

    DMX_WRITE_REG(DmxSet->IoBase, ALLREC_CFG0_29(RecCfgID), allrec_cfg.value);

    DMX_COM_EQUAL(allrec_cfg.value, DMX_READ_REG(DmxSet->IoBase, ALLREC_CFG0_29(RecCfgID)));

    spin_unlock_irqrestore(&DmxHalLock, u32LockFlag);
}

#endif

/***********************************************************************************
* Function      : DmxHalConfigRecTsTimeStamp
* Description   : Config Record Ts time stamp
* Input         : DmxID,enRecTimeStamp
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalConfigRecTsTimeStamp(Dmx_Set_S *DmxSet, HI_U32 DmxId, DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp)
{
    HI_U32 RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);
    U_TIMESTAMP_CTRL timestamp_ctrl;

    BUG_ON(RawDmxId >= DmxSet->DmxCnt);

    timestamp_ctrl.value = DMX_READ_REG(DmxSet->IoBase, TIMESTAMP_CTRL);
    timestamp_ctrl.value |= ((enRecTimeStamp << RawDmxId) && (0x3fff));
    DMX_WRITE_REG(DmxSet->IoBase, TIMESTAMP_CTRL, timestamp_ctrl.value);
}

#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
/*
 * enable or disable TS port tag deal mode
 */
HI_S32 __DmxHalSetTagDealCtl(HI_BOOL bEnable, HI_UNF_DMX_PORT_E enPortId, HI_U32 u32SyncMode, HI_U32 u32TagLen)
{
    U_MUX_SOURCE mux_source;
    U_MUX_CONFIG mux_config;
    U_DBG_DVBT dbg_config;
    HI_U32 PhyPortId, RawPortId;

    /* enPortId maybe is HI_UNF_DMX_PORT_BUTT or others invalid value */
    if (enPortId >= HI_UNF_DMX_PORT_TSI_0 && enPortId <= HI_UNF_DMX_PORT_TSI_7)
    {
        PhyPortId = enPortId - HI_UNF_DMX_PORT_TSI_0;
        RawPortId = PhyPortId + DMX_PORT_OFFSET;
    }
    else
    {
        PhyPortId = DMX_INVALID_PORT_ID;
        RawPortId = 0;   /* 0: no mux stream */
    }

    if (HI_TRUE == bEnable)
    {
        mux_source.u32 = DMX_READ_REG(TS_TAG_MUX_SOURCE);
        mux_source.bits.mux_dvb_inf_sel = RawPortId;
        DMX_WRITE_REG(TS_TAG_MUX_SOURCE, mux_source.u32);

        if (PhyPortId < DMX_TUNERPORT_CNT)
        {
            dbg_config.u32 = DMX_READ_REG(TS_DBG_DVBT_INTERFACE(PhyPortId));
            dbg_config.bits.dbg_ser_len_bypass = 1;
            DMX_WRITE_REG(TS_DBG_DVBT_INTERFACE(PhyPortId), dbg_config.u32);
        }
    }
    else
    {
        mux_source.u32 = DMX_READ_REG(TS_TAG_MUX_SOURCE);
        mux_source.bits.mux_dvb_inf_sel = RawPortId;
        DMX_WRITE_REG(TS_TAG_MUX_SOURCE, mux_source.u32);

        if (PhyPortId < DMX_TUNERPORT_CNT)
        {
            dbg_config.u32 = DMX_READ_REG(TS_DBG_DVBT_INTERFACE(PhyPortId));
            dbg_config.bits.dbg_ser_len_bypass = 0;
            DMX_WRITE_REG(TS_DBG_DVBT_INTERFACE(PhyPortId), dbg_config.u32);
        }
    }

    mux_config.u32 = DMX_READ_REG(TS_TAG_MUX_CONFIG);
    mux_config.bits.mux_work_en = bEnable;
    mux_config.bits.sync_mode_sel = u32SyncMode;
    mux_config.bits.tag_length = u32TagLen;
    DMX_WRITE_REG(TS_TAG_MUX_CONFIG, mux_config.u32);

    return HI_SUCCESS;
}

HI_S32 DmxHalSetTagDealCtl(HI_BOOL bEnable, HI_UNF_DMX_PORT_E enPortId, HI_U32 u32SyncMode, HI_U32 u32TagLen)
{
    /* tag source port must be dvb port */
    if (! (enPortId >= HI_UNF_DMX_PORT_TSI_0 && enPortId <= HI_UNF_DMX_PORT_TSI_7) )
    {
        HI_ERR_DEMUX("invalid TS source port(%d).\n", enPortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    return __DmxHalSetTagDealCtl(bEnable, enPortId, u32SyncMode, u32TagLen);
}

/*
 * set tag values
 */
HI_VOID DmxHalSetTagDealAttr(HI_U32 u32TagPortId, HI_U32 u32Low, HI_U32 u32Mid, HI_U32 u32High)
{
    DMX_WRITE_REG(TS_TAG_LOW_REG(u32TagPortId), u32Low);
    DMX_WRITE_REG(TS_TAG_LOW_REG(u32TagPortId), u32Mid);
    DMX_WRITE_REG(TS_TAG_LOW_REG(u32TagPortId), u32High);
}

#endif

#ifdef DMX_SUPPORT_RAM_CLK_AUTO_CTL
/*
 * ram clk gate control is an built_in mechanism of chip logical for reducing power consumption.
 * here we just enabled it.
 */
HI_VOID DmxHalEnableRamClkAutoCtl(Dmx_Set_S * DmxSet)
{
    HI_U32 value;

#define ENABLE_REG_OFFSET_1 (0x0a38)
#define ENABLE_BIT_OFFSET_1 (0)
#define ENABLE_REG_OFFSET_2 (0xc158)
#define ENABLE_BIT_OFFSET_2 (15)

    value = DMX_READ_REG(DmxSet->IoBase, ENABLE_REG_OFFSET_1);
    value |= 1 << ENABLE_BIT_OFFSET_1;
    DMX_WRITE_REG(DmxSet->IoBase, ENABLE_REG_OFFSET_1, value);

    BUG_ON(0 == (DMX_READ_REG(DmxSet->IoBase, ENABLE_REG_OFFSET_1) & (1 << ENABLE_BIT_OFFSET_1)) );

    value = DMX_READ_REG(DmxSet->IoBase, ENABLE_REG_OFFSET_2);
    value |= 1 << ENABLE_BIT_OFFSET_2;
    DMX_WRITE_REG(DmxSet->IoBase, ENABLE_REG_OFFSET_2, value);

    BUG_ON(0 == (DMX_READ_REG(DmxSet->IoBase, ENABLE_REG_OFFSET_2) & (1 << ENABLE_BIT_OFFSET_2)) );

}
#endif

#ifdef RMX_SUPPORT
HI_VOID DmxHalSetRemuxPort(Dmx_Set_S * DmxSet, HI_U32 ChanId, HI_U32 RmxPortId, DMX_PORT_MODE_E PortMode, HI_U32 PortId)
{
    HI_U32 Id;
    RMX_PORT_CTL Ctl;

    Id = DmxHalGetPortRawId(DmxSet, PortMode, PortId);

    BUG_ON(DMX_INVALID_PORT_ID == Id);

    Ctl.u32 = DMX_READ_REG(DmxSet->IoBase, RMX_PORT_CTL_OFFSET(ChanId));

    switch(RmxPortId)
    {
        case 0:
            Ctl.bits.sw_rmx_src0_sel = Id;
            break;

        case 1:
            Ctl.bits.sw_rmx_src1_sel = Id;
            break;

        case 2:
            Ctl.bits.sw_rmx_src2_sel = Id;
            break;

        case 3:
            Ctl.bits.sw_rmx_src3_sel = Id;
            break;

        default:
            BUG();
    }

    DMX_WRITE_REG(DmxSet->IoBase, RMX_PORT_CTL_OFFSET(ChanId), Ctl.u32);
}

HI_VOID DmxHalUnsetRemuxPort(Dmx_Set_S * DmxSet, HI_U32 ChanId)
{
    DMX_WRITE_REG(DmxSet->IoBase, RMX_PORT_CTL_OFFSET(ChanId), 0);
}

HI_VOID DmxHalSetRemuxPortAllPass(Dmx_Set_S * DmxSet, HI_U32 RmxPortId)
{
    HI_U32 value;

    value = DMX_READ_REG(DmxSet->IoBase, RMX_PORT_ALL_PASS_OFFSET);
    value |= (1 << RmxPortId);
    DMX_WRITE_REG(DmxSet->IoBase, RMX_PORT_ALL_PASS_OFFSET, value);
}

HI_VOID DmxHalUnSetRemuxPortAllPass(Dmx_Set_S * DmxSet, HI_U32 RmxPortId)
{
    HI_U32 value;

    value = DMX_READ_REG(DmxSet->IoBase, RMX_PORT_ALL_PASS_OFFSET);
    value &= ~(1 << RmxPortId);
    DMX_WRITE_REG(DmxSet->IoBase, RMX_PORT_ALL_PASS_OFFSET, value);
}

HI_VOID DmxHalEnDetectRemuxPort(Dmx_Set_S * DmxSet, HI_U32 RmxPortId)
{
    RMX_PORT_DETECT Detect;

    Detect.bits.sw_rmx_full_cnten = 1;

    DMX_WRITE_REG(DmxSet->IoBase, RMX_PORT_DETECT_OFFSET(RmxPortId), Detect.u32);
}

HI_VOID DmxHalDisDetectRemuxPort(Dmx_Set_S * DmxSet, HI_U32 RmxPortId)
{
    RMX_PORT_DETECT Detect;

    Detect.bits.sw_rmx_full_cnten = 0;

    DMX_WRITE_REG(DmxSet->IoBase, RMX_PORT_DETECT_OFFSET(RmxPortId), Detect.u32);
}

HI_U32 DmxHalGetRemuxPortOverflowCount(Dmx_Set_S * DmxSet, HI_U32 RmxPortId)
{
    RMX_PORT_DETECT Detect;

    Detect.u32 = DMX_READ_REG(DmxSet->IoBase, RMX_PORT_DETECT_OFFSET(RmxPortId));

    return Detect.bits.dbg_full_err_cnt;
}

HI_VOID DmxHalSetRemuxPidTab(Dmx_Set_S * DmxSet, HI_U32 PumpId, HI_U32 RmxPortId, HI_U32 Pid)
{
    RMX_PID_CTL Ctl;

    Ctl.u32 = 0;

    Ctl.bits.original_pid = Pid;
    Ctl.bits.remap_source_id = RmxPortId;
    Ctl.bits.pidtab_en = 1;

    DMX_WRITE_REG(DmxSet->IoBase, RMX_PID_TAB_OFFSET(PumpId), Ctl.u32);
}

HI_VOID DmxHalSetRemuxRemapPidTab(Dmx_Set_S * DmxSet, HI_U32 PumpId, HI_U32 RmxPortId, HI_U32 Pid, HI_U32 NewPid)
{
    RMX_PID_CTL Ctl;

    Ctl.u32 = 0;

    Ctl.bits.original_pid = Pid;
    Ctl.bits.remap_source_id = RmxPortId;
    Ctl.bits.pid_remap_pid = NewPid;
    Ctl.bits.pidtab_en = 1;
    Ctl.bits.pid_remap_mod = 1;

    DMX_WRITE_REG(DmxSet->IoBase, RMX_PID_TAB_OFFSET(PumpId), Ctl.u32);
}

HI_VOID DmxHalUnSetRemuxPidTab(Dmx_Set_S * DmxSet, HI_U32 PumpId)
{
    DMX_WRITE_REG(DmxSet->IoBase, RMX_PID_TAB_OFFSET(PumpId), 0);
}

#endif

