/*****************************************************************************
* Tuner sample code
*
* History:
* Date Athor Version Reason
* ============ ============= ========= =================
* 1.Apr.29.2010 Version1.0
*****************************************************************************/

#include <linux/delay.h>
#include "av2018.h"
#include "IBSP.h"
#include "IBase.h"
#include "II2C.h"
#include "drv_tuner_ioctl.h"
#include "drv_demod.h"

/*driver for AVL6211, beginning*/
#if defined(DEMOD_DEV_TYPE_AVL6211)
AVL_DVBSx_ErrorCode AV2018_I2C_write(AVL_uchar reg_start, AVL_uchar* buff, AVL_uchar len, struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r = 0;
    AVL_uint16 uiTimeOut = 0;
    AVL_uchar ucTemp[50];
    int i;

    /* AVL_DVBSx_IBSP_Delay(5); */ /* comment by wanghua */
    ucTemp[0] = reg_start;
    for (i = 1; i < len + 1; i++)
    {
        ucTemp[i] = *(buff + i - 1);
    }

    r = AVL_DVBSx_II2CRepeater_GetOPStatus( pTuner->m_pAVLChip );

    while (r != AVL_DVBSx_EC_OK)
    {
#ifdef AV2018_ENABLE_TIMEOUT
        if (uiTimeOut++ > AV2018_TIMEOUT)
        {
            return (AVL_DVBSx_EC_TimeOut);
        }
#endif

        AVL_DVBSx_IBSP_Delay(1);
        r = AVL_DVBSx_II2CRepeater_GetOPStatus( pTuner->m_pAVLChip );
    }

    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, len + 1, pTuner->m_pAVLChip );
    if (r != AVL_DVBSx_EC_OK)
    {
        return (r);
    }

    /* AVL_DVBSx_IBSP_Delay(5); */
    return (r);
}

AVL_DVBSx_ErrorCode ExtAV2018_RegInit(struct AVL_Tuner * pTuner)
{
    AVL_uchar reg[50];
    AVL_DVBSx_ErrorCode r;

    /*Tuner Initail registers R0~R41*/
    reg[0] = 0x38;
    reg[1] = 0x00;
    reg[2] = 0x00;
    reg[3] = 0x50;
    reg[4] = 0x1f;
    reg[5] = 0xa3;
    reg[6] = 0xfd;
    reg[7] = 0x58;

    /*reg[8]=0x3f;*/

    reg[8] = (0x7 << 3) | 0x04 | 0x03;

    reg[9]  = 0x82;
    reg[10] = 0x88;
    reg[11] = 0xb4;
    reg[12] = 0xd6;   /*RFLP=ON at Power on initial*/
    reg[13] = 0x40;

    reg[14] = 0x5b;
    reg[15] = 0x6a;

    reg[16] = 0x66;
    reg[17] = 0x40;
    reg[18] = 0x80;
    reg[19] = 0x2b;
    reg[20] = 0x6a;
    reg[21] = 0x50;
    reg[22] = 0x91;
    reg[23] = 0x27;
    reg[24] = 0x8f;
    reg[25] = 0xcc;
    reg[26] = 0x21;
    reg[27] = 0x10;
    reg[28] = 0x80;
    reg[29] = 0x02;
    reg[30] = 0xf5;
    reg[31] = 0x7f;
    reg[32] = 0x4a;
    reg[33] = 0x9b;
    reg[34] = 0xe0;
    reg[35] = 0xe0;
    reg[36] = 0x36;
    reg[37] = 0x00;   /*Disble FT-function at Power on initial*/
    reg[38] = 0xab;
    reg[39] = 0x97;
    reg[40] = 0xc5;
    reg[41] = 0xa8;

    /* Sequence 1, Send Reg0 ->Reg11 */
    r = AV2018_I2C_write(0, reg, 12, pTuner);
    AVL_DVBSx_IBSP_Delay(1);
    /* Sequence 2, Send Reg13 ->Reg24 */
    r = AV2018_I2C_write(13, reg + 13, 12, pTuner);
    /* Send Reg25 ->Reg35 */
    r = AV2018_I2C_write(25, reg + 25, 11, pTuner);
    /* Send Reg36 ->Reg41 */
    r = AV2018_I2C_write(36, reg + 36, 6, pTuner);
    AVL_DVBSx_IBSP_Delay(1);
    /* Sequence 3, send reg12 */
    r = AV2018_I2C_write(12, reg + 12, 1, pTuner);

    /* monsen 20081125, Wait 100 ms */
    AVL_DVBSx_IBSP_Delay(10);

    /* monsen 20081030, Reinitial again */
    {
        /* Sequence 1, Send Reg0 ->Reg11 */
        r = AV2018_I2C_write(0, reg, 12, pTuner);
        AVL_DVBSx_IBSP_Delay(1);
        /* Sequence 2, Send Reg13 ->Reg24 */
        r = AV2018_I2C_write(13, reg + 13, 12, pTuner);
        /* Send Reg25 ->Reg35 */
        r = AV2018_I2C_write(25, reg + 25, 11, pTuner);
        /* Send Reg36 ->Reg41 */
        r = AV2018_I2C_write(36, reg + 36, 6, pTuner);
        AVL_DVBSx_IBSP_Delay(1);
        /* Sequence 3, send reg12 */
        r = AV2018_I2C_write(12, reg + 12, 1, pTuner);
        AVL_DVBSx_IBSP_Delay(5);
        return (r);
    }
}


/********************************************************************************
 * static AVL_DVBSx_ErrorCode Tuner_control(AVL_uint32  channel_freq, AVL_uint32  bb_sym, 
 *                      struct AVL_Tuner * pTuner,struct AV2018_Setting * AV2018_Configure)
 *
 * Arguments:
 * Parameter1: AVL_uint32  channel_freq : Channel freqency (Unit: MHz)
 * Parameter2: AVL_uint32  bb_sym : Baseband Symbol Rate (Unit: KHz)
 * Paramiter3: AVL_Tuner * pAVLChip : AVL_Tuner structure
 * Paramiter4: struct AV2018_Setting * AV2018_Configure : 
 *                     Special configuration for AV2018(IQ mode, Auto_Gain, Scan mode, RF_LP)
 * Return Value: INT32 : Result
 *****************************************************************************/
static AVL_DVBSx_ErrorCode Tuner_control(AVL_uint32 channel_freq, AVL_uint32 bb_sym, struct AVL_Tuner * pTuner,
                                  struct AV2018_Setting * AV2018_Configure)
{
    AVL_uchar reg[50];
    AVL_uint32 fracN;
    AVL_uint32 BW;
    AVL_uint32 BF;
    AVL_DVBSx_ErrorCode r;

    /* when sym is 0(KHz) or 45000(KHz), means auto-scan channel. */
    if ((bb_sym == 0) || (bb_sym == 45000))
    {
        AV2018_Configure->Auto_Scan = 1;
    }

    /* AVL_DVBSx_IBSP_Delay(50); */ /* comment by wanghua */
    fracN = (channel_freq + AV2018_tuner_crystal / 2) / AV2018_tuner_crystal;
    if (fracN > 0xff)
    {
        fracN = 0xff;
    }

    reg[0] = (fracN & 0xff);
    fracN  = (channel_freq << 17) / AV2018_tuner_crystal;
    fracN  = fracN & 0x1ffff;
    reg[1] = (fracN >> 9) & 0xff;
    reg[2] = (fracN >> 1) & 0xff;

    /* AV2020 no IQ mode */
    reg[3] = (((fracN << 7) & 0x80) | 0x50);

    /* Channel Filter Bandwidth Setting. */
    /* "sym" unit is Hz; */
    if (AV2018_Configure->Auto_Scan) /* autoscan requested by BB */
    {
        reg[5] = 0xA3; /* BW=27MHz */
        /* Sequence 4, Send Reg0 ->Reg4 */
        r = AV2018_I2C_write(0, reg, 4, pTuner);
        AVL_DVBSx_IBSP_Delay(5);
        /* Sequence 5, Send Reg5 */
        r = AV2018_I2C_write(5, reg + 5, 1, pTuner);
        AVL_DVBSx_IBSP_Delay(5);

        /* Fine-tune Function Control, Auto-scan mode , FT_block=1, FT_EN=0, FT_hold=1 */
        reg[37] = 0x05;
        r = AV2018_I2C_write(37, reg + 37, 1, pTuner);
        AVL_DVBSx_IBSP_Delay(4);
    }
    else
    {
        /* rolloff is 35% */
        BW = bb_sym * 135 / 200;

        /* monsen 20080726, BB request low IF when sym < 6.5MHz */
        /* add 6M when Rs<6.5M, */
        if (bb_sym < 6500)
        {
            BW = BW + 6000;
        }

        /* add 2M for LNB frequency shifting */
        BW = BW + 2000;

        /* add 8% margin since fc is not very accurate */
        BW = BW * 108 / 100;

        /* Bandwidth can be tuned from 4M to 40M */
        if (BW < 4000)
        {
            BW = 4000;
        }

        if (BW > 40000)
        {
            BW = 40000;
        }

        BF = (BW * 127 + 21100 / 2) / (21100);
        reg[5] = (AVL_uchar)BF;

        /* Sequence 4, Send Reg0 ->Reg4 */
        AVL_DVBSx_IBSP_Delay(5);
        r = AV2018_I2C_write(0, reg, 4, pTuner);
        AVL_DVBSx_IBSP_Delay(5);
        r = AV2018_I2C_write(0, reg, 4, pTuner);
        AVL_DVBSx_IBSP_Delay(5);
        /* Sequence 5, Send Reg5 */
        r = AV2018_I2C_write(5, reg + 5, 1, pTuner);
        AVL_DVBSx_IBSP_Delay(5);

        /* Fine-tune Function Control, not auto-scan case. enable block function., FT_block=1, FT_EN=1, FT_hold=0 */
        reg[37] = 0x04 | ((AV2018_Configure->AutoGain) << 1);
        r = AV2018_I2C_write(37, reg + 37, 1, pTuner);
        AVL_DVBSx_IBSP_Delay(5);

        /* AV2020 no IQ mode */
        reg[3] = (((fracN << 7) & 0x80) | 0x50);

        r = AV2018_I2C_write(3, reg + 3, 1, pTuner);
        AVL_DVBSx_IBSP_Delay(5);

        /* 
         * Disable RFLP at Lock Channel sequence after reg[37], RFLP=OFF at Lock Channel sequence, 
         * RFLP can be Turned OFF, only at Receving mode. 
         */
        reg[12] = 0x96 + ((AV2018_Configure->RF_Loop) << 6);
        r = AV2018_I2C_write(12, reg + 12, 1, pTuner);
        AVL_DVBSx_IBSP_Delay(5);
    }

    return r;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2018_Lock(  struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r;
    struct AV2018_Setting AV2018_Set;

    /* 
     * pTuner->m_pParameters is used store four special setting for AV2018:IQ mode, 
     * auto Gain setting, Work mode, RF loop through
     */
    if (pTuner->m_pParameters == 0)  /* if none */
    {
        AV2018_Set.Auto_Scan = 0;  /* Default Setting: Normal lock mode */
        AV2018_Set.AutoGain   = 1; /* Defualt Setting: Auto Gain control on */
        AV2018_Set.Single_End = 0; /* Defualt setting: IQ Differential mode */
        AV2018_Set.RF_Loop = 1;    /* Defualt setting: open RF loop through */
        /* use default setting if not set it in Struct pTuner->m_pParameters. */
        /* (struct AV2018_Setting)(pTuner->m_pParameters) = &AV2018_Set; */
        r = Tuner_control((AVL_uint32) ((pTuner->m_uiFrequency_100kHz) / 10), (AVL_uint32) ((pTuner->m_uiLPF_100kHz)
                                                                                            * 100), pTuner, &AV2018_Set);

    }
    else  /* set AV2018 tuner spectial setting accordding to pTuner->m_pParameter */
    {
        r = Tuner_control((AVL_uint32) ((pTuner->m_uiFrequency_100kHz) / 10), 
                          (AVL_uint32) ((pTuner->m_uiLPF_100kHz) * 100), pTuner,
                          (struct AV2018_Setting *)(pTuner->m_pParameters));

    }

    return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2018_GetLockStatus(struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r;
    AVL_uchar uilock;

    //Send register address */
    r = AVL_DVBSx_II2CRepeater_GetOPStatus(pTuner->m_pAVLChip);

    if (AVL_DVBSx_EC_OK != r)
    {
        HI_ERR_TUNER("%s,%s,%d,r = %x\n", __FILE__, __FUNCTION__, __LINE__, r);
        return (r);
    }

    AVL_DVBSx_IBSP_Delay(1);
    r = AVL_DVBSx_II2CRepeater_ReadData_Multi((AVL_uchar)(pTuner->m_uiSlaveAddress), &uilock, 0x0B, (AVL_uint16)(0x1),
                                              pTuner->m_pAVLChip );

    if (AVL_DVBSx_EC_OK == r)
    {
        if (0 == (uilock & AV2018_R0_PLL_LOCK))
        {
            HI_ERR_TUNER("%s,%s,%d,r = %x,uilock = %x\n", __FILE__, __FUNCTION__, __LINE__, r, uilock);
            r = AVL_DVBSx_EC_Running;
        }
    }

    return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2018_Initialize(struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r;

    r = AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_slave_addr_addr, pTuner->m_uiSlaveAddress);

    r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_use_internal_control_addr, 0);

    /* Clean up the LPF margin for blind scan. for external driver, this must be zero. */
    r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_LPF_margin_100kHz_addr, 0);
    /*
     * Set up the right LPF for blind scan to regulate the freq_step. 
     * This field should corresponding the flat response part of the LPF. 
     */
    r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_max_LPF_100kHz_addr, 360);
    r |= AVL_DVBSx_II2CRepeater_Initialize(pTuner->m_uiI2CBusClock_kHz, pTuner->m_pAVLChip);

    r |= ExtAV2018_RegInit(pTuner); /* init all tuner register */
    return (r);
}
#endif
/*driver for AVL6211, end*/

/*driver for hi3136, beginning*/
#if defined(DEMOD_DEV_TYPE_HI3136)
static HI_S32 av2018_write(HI_U32 u32TunerPort, HI_U8 enI2cChannel,
                           HI_U8 u8DevAddr, HI_U8 u8RegAddr, HI_U8 *pu8Buf, HI_U32 u32Num)
{
    HI_U32 i = 0;
    HI_U8 au8Data[255] = {0};
    TUNER_I2C_DATA_S stI2cData = {0};

    HI_INFO_TUNER("> %s: %d\n", __FUNCTION__, __LINE__);

    if (NULL == pu8Buf)
    {
        HI_ERR_TUNER( "pointer(pu8Buf) is null\n");
        return HI_FAILURE;
    }

    au8Data[0] = u8RegAddr;
    for (i = 0; i < u32Num; i++)
    {
        au8Data[i + 1] = pu8Buf[i];
    }

    stI2cData.pu8ReceiveBuf = 0;
    stI2cData.u32ReceiveLength = 0;
    stI2cData.pu8SendBuf = au8Data;
    stI2cData.u32SendLength = u32Num + 1;

    return tuner_i2c_send_data(enI2cChannel, u8DevAddr, &stI2cData);
}

HI_S32 av2018_init_tuner(HI_U32 u32TunerPort)
{
    HI_U8 reg[50];
    HI_U8 reg_start;
    HI_U8 tuner_address;
    HI_U8 enI2cChannel = 0;
    HI_S32 s32Ret = HI_SUCCESS;


    enI2cChannel = g_stTunerOps[u32TunerPort].enI2cChannel;
    //Tuner Initail registers R0~R41
    reg[0]=0x38;
    reg[1]=0x00;
    reg[2]=0x00;
    reg[3]=0x50;
    reg[4]=0x1f;
    reg[5]=0xa3;
    reg[6]=0xfd;
    reg[7]=0x58;
    //reg[8]=0x3f;

    reg[8] = 0x7;

    reg[9]=0x82;
    reg[10]=0x88;
    reg[11]=0xb4;
    reg[12]=0xd6; //RFLP=ON at Power on initial
    reg[13]=0x40;
    reg[14]=0x94;
    reg[15]=0x4a;
    //#else
    /*reg[14]=0x5b;
    reg[15]=0x6a;*/
    //#endif
    reg[16]=0x66;
    reg[17]=0x40;
    reg[18]=0x80;
    reg[19]=0x2b;
    reg[20]=0x6a;
    reg[21]=0x50;
    reg[22]=0x91;
    reg[23]=0x27;
    reg[24]=0x8f;
    reg[25]=0xcc;
    reg[26]=0x21;
    reg[27]=0x10;
    reg[28]=0x80;
    reg[29]=0xE2;
    reg[30]=0xf5;
    reg[31]=0x7f;
    reg[32]=0x4a;
    reg[33]=0x9b;
    reg[34]=0xe0;
    reg[35]=0xe0;
    reg[36]=0x36;

    reg[37]=0x00; //Disble FT-function at Power on initial
    reg[38]=0xab;
    reg[39]=0x97;
    reg[40]=0xc5;
    reg[41]=0xa8;

    tuner_address = g_stTunerOps[u32TunerPort].u32TunerAddress;

    // Sequence 1, Send Reg0 ->Reg11
    reg_start = 0;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg, 12);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "av2018_write error\n");
        return s32Ret;
    }
    tuner_mdelay(1);

    // Sequence 2, Send Reg13 ->Reg24
    reg_start = 13;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 13, 12);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "av2018_write error\n");
        return s32Ret;
    }

    // Send Reg25 ->Reg35
    reg_start = 25;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 25, 11);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "av2018_write error\n");
        return s32Ret;
    }
    // Send Reg36 ->Reg41
    reg_start = 36;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 36, 6);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "av2018_write error\n");
        return s32Ret;
    }
    tuner_mdelay(1);
    // Sequence 3, send reg12
    reg_start = 12;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 12, 1);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "av2018_write error\n");
        return s32Ret;
    }

    tuner_mdelay(10);
    //Reinitial again
    {
        // Sequence 1, Send Reg0 ->Reg11
        reg_start = 0;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg, 12);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER( "av2018_write error\n");
            return s32Ret;
        }
        tuner_mdelay(1);

        // Sequence 2, Send Reg13 ->Reg24
        reg_start = 13;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 13, 12);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER( "av2018_write error\n");
            return s32Ret;
        }
        // Send Reg25 ->Reg35
        reg_start = 25;
        hi3136_config_i2c_out(u32TunerPort, 1);		
        s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 25, 11);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER( "av2018_write error\n");
            return s32Ret;
        }
        // Send Reg36 ->Reg41
        reg_start = 36;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 36, 6);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER( "av2018_write error\n");
            return s32Ret;
        }
        tuner_mdelay(1);

        // Sequence 3, send reg12
        reg_start = 12;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 12, 1);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER( "av2018_write error\n");
            return s32Ret;
        }
        tuner_mdelay(5);
    }

    return HI_SUCCESS;
}


HI_S32 av2018_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF/*kHz*/)  //freq :unit MHZ, symbol:unit  Khz
{	
    HI_U8 reg[50];
    HI_U8 reg_start;
    HI_U8 tuner_address;
    HI_U32  fracN;
    HI_U32  BW;
    HI_U32  BF;
    //HI_U8  auto_scan=0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 /*u32RFfreq_MHz = 0,*/ tuner_crystal_kHz, int_div, round_div;

    //Lock Channel sequence 
    //***** After RFAGC is starting tracking
    //***** Make sure the RFAGC do not have sharp jump after sending reg[37]   
    // RF Channel Fregency Setting.  

    tuner_address = g_stTunerOps[u32TunerPort].u32TunerAddress;    
    tuner_crystal_kHz = AV2018_tuner_crystal * 1000;

    int_div = u32PuRF / tuner_crystal_kHz;  
    round_div = (u32PuRF + tuner_crystal_kHz/2) / tuner_crystal_kHz;  

    if (round_div > 0xff)
        round_div = 0xff; 
    reg[0] = round_div & 0xff;

    fracN = ((u32PuRF-int_div*tuner_crystal_kHz) << 17) / tuner_crystal_kHz;
    fracN = fracN & 0x1ffff;
    reg[1] = (fracN >> 9) & 0xff;
    reg[2] = (fracN >> 1) & 0xff;    	
    // reg[3]_D7 is frac<0>, D6~D0 is 0x50	

    reg[3] = ((fracN << 7) & 0x80) | 0x50;	// default is 0x50


    BW = u32LPF_KHz[u32TunerPort];  

    //BW is determined to demodulation.  here only limit the value	
    if (BW < 4000)
        BW = 4000;
    else if (BW> 40000)
        BW = 40000;

    BF = (BW * 127 + 21100 / 2) / (21100); // 	BW * 1.27 / 211KHz
    reg[5] = (HI_U8)BF;

    {
        // Sequence 4	
        // Send Reg0 ->Reg3
        //mdelay(2);
        reg_start = 0;
        hi3136_config_i2c_out(u32TunerPort, 1);	
        av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg, 4);
        tuner_mdelay(5);
        hi3136_config_i2c_out(u32TunerPort, 1);	
        av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg, 4);
        tuner_mdelay(5);
        
        // Sequence 5
        // Send Reg5  	
        reg_start = 5;
        hi3136_config_i2c_out(u32TunerPort, 1);		
        av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg + 5, 1);
        //mdelay(2);
        
        // Fine-tune Function Control
        //Auto-scan mode. FT_block=1, FT_EN=0, FT_hold=1
        reg[37] = 0x06;

        reg_start = 37;
        hi3136_config_i2c_out(u32TunerPort, 1);
        av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg+37, 1);
        //mdelay(2);

        reg_start = 3;
        hi3136_config_i2c_out(u32TunerPort, 1);
        av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg+3, 1);
        //mdelay(2);

        //reg[12] = 0x96; //Disable RFLP at Lock Channel sequence after reg[37]		
        reg[12] = 0xd6; //Enable RFLP at Lock Channel sequence after reg[37]  		
        reg_start = 12;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = av2018_write(u32TunerPort, enI2cChannel, tuner_address, reg_start, reg+12, 1);
        tuner_mdelay(2);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "av2018_set_tuner error\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

#endif
/*driver for hi3136, end*/

