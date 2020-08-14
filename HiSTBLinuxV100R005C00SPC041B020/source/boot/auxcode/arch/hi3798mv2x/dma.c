/******************************************************************
* Copyright (C), 2005-2015, HISILICON Tech. Co., Ltd.               *
*                                                                                          *
* File name: dma.c                                                                  *
*                                                                                           *
* Description:                                                                         *
*       DMA operation to read data from flash to SRAM/DDR          *
*                                                                                           *
* Author:                                                                               *
*       Min Xin(00190812)                                                         *
*                                                                                           *
* History:                                                                               *
*    v1.1  Hi3716MV410B02 version by m00190812                      *
*                                                                                           *
*******************************************************************/
#include "platform.h"
#include "dma.h"
#include "aux_common.h"

// count一定以byte为单位
static int check_dma(void* dest, const void* src, unsigned int count, dma_cfg_t* cfg)
{
    unsigned int channel_id = DMA_CHANNEL_ID;
    unsigned int tmp_dest = 0;
    unsigned int tmp_src = 0;
    DMAC_CX_CONTROL_U un_dmac_cx_control;

    tmp_src = readl(DMAC_CX_SRC_ADDR(channel_id));
    tmp_dest = readl(DMAC_CX_DEST_ADDR(channel_id));
    un_dmac_cx_control.u32 = readl(DMAC_CX_CONTROL(channel_id));

    if (0 == cfg->src_inc)
    {// 1 burst for eMMC
        if (tmp_src != (unsigned int)src)
        {
            sys_reset();
            return AUTH_FAILURE;
        }

        if (tmp_dest != ((unsigned int)dest + ((count / 4) - 1) * 4))
        {
            sys_reset();
            return AUTH_FAILURE;
        }
    }
    else if (0 == cfg->dest_inc)
    {// 1 burst for RSA
        if (tmp_src != ((unsigned int)src + ((count / 4) - 1) * 4))
        {
            sys_reset();
            return AUTH_FAILURE;
        }

        if (tmp_dest != (unsigned int)dest)
        {
            sys_reset();
            return AUTH_FAILURE;
        }
    }
    else
    {// 4 burst for FMC
        if (tmp_src != ((unsigned int)src + ((count / 4) - 1) * 4))
        {
            sys_reset();
            return AUTH_FAILURE;
        }

        if (tmp_dest != ((unsigned int)dest + ((count / 4) - 1) * 4))
        {
            sys_reset();
            return AUTH_FAILURE;
        }
    }

    return AUTH_SUCCESS;
}

static void dma_init(void)
{
    PERI_CRG_DMA_U un_crg_dma;
    DMAC_CONFIG_U un_dmac_config;
    
    //dma reset
    un_crg_dma.u32 = readl(CRG_DMA);
    
    un_crg_dma.bits.dmac_cken = 1;
    un_crg_dma.bits.dmac_srst_req = 1;
    writel(un_crg_dma.u32, CRG_DMA);
    udelay(20);
    
    un_crg_dma.bits.dmac_srst_req = 0;
    writel(un_crg_dma.u32, CRG_DMA);

    //enable DMAC
    un_dmac_config.u32 = readl(DMAC_CONFIG);
    un_dmac_config.bits.dmac_enable = 1;            //enable DMAC
    un_dmac_config.bits.m1_endian = 0;              //master 1: little endian
    un_dmac_config.bits.m2_endian = 0;              //master 2: little endian
    writel(un_dmac_config.u32, DMAC_CONFIG);
    
    /*enable the sync logic for the 16 peripheral*/
    writel(0x0, DMAC_SYNC);
    
    /*clear transfer complete int*/
    writel(0xf, DMAC_INT_TC_CLR);
    
    /*clear error  int */
    writel(0xf, DMAC_INT_ERR_CLR);

    /*clear channel configuration*/
    writel(0x0, DMAC_CX_CONFIG(DMA_CHANNEL_ID));
    
    udelay(30);
    
    return;
}

static void dma_deinit(void)
{
    PERI_CRG_DMA_U un_crg_dma;
    
    //dma reset
    un_crg_dma.u32 = readl(CRG_DMA);
    
    un_crg_dma.bits.dmac_cken = 1;
    un_crg_dma.bits.dmac_srst_req = 1;
    writel(un_crg_dma.u32, CRG_DMA);
    udelay(20);
    
    un_crg_dma.bits.dmac_srst_req = 0;
    writel(un_crg_dma.u32, CRG_DMA);

    return;
}

// count一定以byte为单位
static void dma_operation_config(void* dest, const void* src, unsigned int count, dma_cfg_t* cfg)
{
    unsigned int channel_id = DMA_CHANNEL_ID;
    DMAC_CX_CONTROL_U un_dmac_cx_control;
    DMAC_CX_CONFIG_U un_dmac_cx_config;
    unsigned int tmp_dest = 0;
    unsigned int tmp_src = 0;

    writel(0x0, DMAC_CX_CONTROL(channel_id));
    
    /*clear transfer complete int*/
    writel(0xf, DMAC_INT_TC_CLR);
    
    /*clear error  int */
    writel(0xf, DMAC_INT_ERR_CLR);

    //set SRC address
    writel((unsigned int)src, DMAC_CX_SRC_ADDR(channel_id));

    //set DEST address
    writel((unsigned int)dest, DMAC_CX_DEST_ADDR(channel_id));

    //not use LLI
    writel(0, DMAC_CX_LLI(channel_id));

    un_dmac_cx_control.u32 = readl(DMAC_CX_CONTROL(channel_id));
    un_dmac_cx_control.bits.interrupt = 1;          //interrupt enable
    un_dmac_cx_control.bits.dest_master = 1;        //master 2
    un_dmac_cx_control.bits.src_master = 1;         //master 2
    un_dmac_cx_control.bits.dwidth = 2;             //word transfer
    un_dmac_cx_control.bits.swidth = 2;             //word transfer
    un_dmac_cx_control.bits.src_inc = cfg->src_inc;
    un_dmac_cx_control.bits.dest_inc = cfg->dest_inc;
    un_dmac_cx_control.bits.dbsize = cfg->burst_size;
    un_dmac_cx_control.bits.sbsize = cfg->burst_size;
    un_dmac_cx_control.bits.transfersize = count / 4;   //transfer size 以dwidth和swidth为单位，当前设置为了4 byte
    
    writel(un_dmac_cx_control.u32, DMAC_CX_CONTROL(channel_id));

    tmp_src = readl(DMAC_CX_SRC_ADDR(channel_id));
    tmp_dest = readl(DMAC_CX_DEST_ADDR(channel_id));
    if (tmp_src != (unsigned int)src)
    {
        sys_reset();
    }

    if (tmp_dest != (unsigned int)dest)
    {
        sys_reset();
    }

    un_dmac_cx_config.u32 = 0;
    un_dmac_cx_config.bits.channel_enable = 1;
    un_dmac_cx_config.bits.int_err = 1;
    un_dmac_cx_config.bits.int_tc = 1;
    writel(un_dmac_cx_config.u32, DMAC_CX_CONFIG(channel_id));

    return ;
}

// count以byte为单位
static int unit_read_with_dma(void* dest, const void* src, unsigned int count, dma_cfg_t* cfg)
{
    int ret = AUTH_FAILURE;
    DMAC_CONFIG_U un_dmac_config;
    unsigned int timeout = TIMEOUT_10S * timer_get_divider();        //10s
    unsigned int int_tc = 0;
    unsigned int int_err = 0;

    dma_operation_config(dest, src, count, cfg);

    //enable DMAC
    un_dmac_config.u32 = readl(DMAC_CONFIG);
    un_dmac_config.bits.dmac_enable= 1;             //enable DMAC
    writel(un_dmac_config.u32, DMAC_CONFIG);

    //check result
    timer_start();
    do {
        int_tc = readl(DMAC_RAW_INT_TC_STATUS);
        int_err = readl(DMAC_RAW_INT_ERR_STATUS);
        if (timer_get_val() > timeout)
            break;
    } while(((int_tc & 0x01) != 0x01) && ((int_err & 0x01) != 0x01));

    /*clear transfer complete int*/
    writel(0xf, DMAC_INT_TC_CLR);
    
    /*clear error  int */
    writel(0xf, DMAC_INT_ERR_CLR);
    
    if (((int_tc & 0x01) == 0x01) && ((int_err & 0x01) != 0x01))
    {
        ret = check_dma(dest, (void *)src, count, cfg);
        if (AUTH_SUCCESS != ret)
        {
            sys_reset();
        }

        return AUTH_SUCCESS;
    }
    
    return AUTH_FAILURE;
}

// count一定以byte为单位
int read_with_dma(void* dest, const void* src, unsigned int count, dma_cfg_t* cfg)
{
    int ret = AUTH_FAILURE;
    unsigned int unit_size = 0;
    unsigned int *tmp_dest = (unsigned int*) dest;
    unsigned int *tmp_src = (unsigned int*) src;

    //最大只能做到一次读取0x800的数据，读不到0xfff。transfer size的单位以swidth和dwidth来确定，当前固定为4 byte
    unit_size = (1 << (TRANSFER_SIZE_LEN - 1)) * 4;
    dma_init();
    while (count > 0)    
    {
        if (unit_size > count)
        {
    	    unit_size = count;
        }
        
        ret = unit_read_with_dma(tmp_dest, (void *)tmp_src, unit_size, cfg);
        if (AUTH_SUCCESS == ret)
        {
            ret = check_dma(tmp_dest, (void *)tmp_src, unit_size, cfg);
            if (AUTH_SUCCESS != ret)
            {
                sys_reset();
            }
        
            count = count - unit_size;
            if (1 == cfg->dest_inc)
            {
                tmp_dest = (unsigned int *)((unsigned int)tmp_dest + unit_size);
            }

            if (1 == cfg->src_inc)
            {
                tmp_src = (unsigned int *)((unsigned int)tmp_src + unit_size);
            }
        }
        else
        {
            dma_deinit();
            return ret;
        }
    }

    dma_deinit();
    
    return AUTH_SUCCESS;
}

