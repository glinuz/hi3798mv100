
//#include <linux/delay.h>
#include "sharp_qm1d1c004x.h"
#include "II2CRepeater.h"
#include "IBSP.h"
#include "II2C.h"
#include "IBase.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"
#include "hi_drv_i2c.h"

#define SHARPQM1D1C004x_PLL_LOCK 0x40
#define OSCILLATION_FREQUENCY_MHZ 16
#define QM1D1C0042 0x48
#define QM1D1C0045 0x58
#define QM1D1C0045_2 0x68

static AVL_uint16 Chip_ID = 0;

AVL_uchar ExtSharpQM1D1C004x_VCO_Setting(AVL_uint16 m_uiFrequency_MHz)
{
    AVL_uchar ucTemp;

    if (m_uiFrequency_MHz < 975)
    {
        ucTemp = 0x60;
    }
    else if (m_uiFrequency_MHz < 1200)
    {
        ucTemp = 0x70;
    }
    else if (m_uiFrequency_MHz < 1250)
    {
        ucTemp = 0xa0;
    }
    else if (m_uiFrequency_MHz < 1450)
    {
        ucTemp = 0xb0;
    }
    else if (m_uiFrequency_MHz < 1600)
    {
        ucTemp = 0xc0;
    }
    else if (m_uiFrequency_MHz < 1800)
    {
        ucTemp = 0xd0;
    }
    else if (m_uiFrequency_MHz < 1950)
    {
        ucTemp = 0xe0;
    }
    else
    {
        ucTemp = 0xf0;
    }

    return ucTemp;
}

AVL_DVBSx_ErrorCode ExtSharpQM1D1C004x_Calc_LF(AVL_uint16 m_uiFrequency_MHz, AVL_uchar *N, AVL_uchar *A, AVL_int32 *sd)
{
    AVL_uint16 M;
    AVL_uint16 alpha;
    AVL_int16 beta;
    AVL_uint16 F_ref;

    if (16 == OSCILLATION_FREQUENCY_MHZ)
    {
        F_ref = OSCILLATION_FREQUENCY_MHZ;
    }
    else
    {
        F_ref = OSCILLATION_FREQUENCY_MHZ / 2;
    }

    M = m_uiFrequency_MHz;
    alpha = (M + F_ref / 2) / F_ref;
    alpha = alpha * F_ref;
    beta = M - alpha;
    *N = (AVL_uchar)(alpha / (4 * F_ref) - 3);
    *A = (AVL_uchar)(alpha / F_ref) - 4 * (*N + 1) - 5;

    /* make it two's complements of 22 bits.*/
    if (beta < 0)
    {
        *sd  = (AVL_int32)((1048576 / F_ref) * (M - alpha));
        *sd += 0x400000;
    }
    else
    {
        *sd = (AVL_int32)((1048576 / F_ref) * (M - alpha));
    }

    return AVL_DVBSx_EC_OK;
}

AVL_uchar ExtSharpQM1D1C004x_LPF_FC(void)
{
    AVL_uchar ucTemp;

#if 0
    ucTemp = (AVL_uchar)(round(OSCILLATION_FREQUENCY_MHZ / 4)) * 2;
#else
    ucTemp = (OSCILLATION_FREQUENCY_MHZ + 2) / 4;
    ucTemp = ucTemp * 2;
#endif
    ucTemp = (ucTemp - 4) / 2;

    return ucTemp;
}

AVL_uchar ExtSharpQM1D1C004x_CSEL_Offset(AVL_uint16 uiLPF_100kHz)
{
    AVL_uint16 uiLPF_MHz;
    AVL_uchar ucTemp;

    uiLPF_MHz = uiLPF_100kHz / 10;

    if ((Chip_ID == QM1D1C0045) || (Chip_ID == QM1D1C0045_2))
    {
        if (uiLPF_MHz < 10)
        {
            ucTemp = 0x60;
        }
        else
        if (uiLPF_MHz < 16)
        {
            ucTemp = 0x40;
        }
        else
        if (uiLPF_MHz < 30)
        {
            ucTemp = 0x20;
        }
        else
        {
            ucTemp = 0x00;
        }
    }
    else
    {
        ucTemp = 0x00;
    }

    return ucTemp;
}

AVL_uchar ExtSharpQM1D1C0045_DIV45_Setting(AVL_uint16 m_uiFrequency_MHz)
{
    AVL_uchar ucTemp;

    if (m_uiFrequency_MHz < 975)
    {
        ucTemp = 0x50;
    }
    else
    if (m_uiFrequency_MHz < 1200)
    {
        ucTemp = 0x50;
    }
    else
    {
        ucTemp = 0x10;
    }

    return ucTemp;
}

#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
static AVL_DVBSx_ErrorCode ExtSharpQM1D1C0042_Reset(struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r;
    AVL_uchar ucTemp[16];

    /* three times soft reset */
    ucTemp[0] = 0x01;
    ucTemp[1] = 0x0c;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );
    if (AVL_DVBSx_EC_OK != r)
    {
        return r;
    }

    AVL_DVBSx_IBSP_Delay(2); /* 2ms */

    /* LPF_Tuning ON */
    ucTemp[0] = 0x0c;
    ucTemp[1] = 0x43;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );
    if (AVL_DVBSx_EC_OK != r)
    {
        return r;
    }

    AVL_DVBSx_IBSP_Delay(20); /* 20ms */

    /* write Fixed Value register */
    ucTemp[0]  = 0x01;
    ucTemp[1]  = 0x1C;
    ucTemp[2]  = 0xA0;
    ucTemp[3]  = 0x10;
    ucTemp[4]  = 0xBC;
    ucTemp[5]  = 0xC5;
    ucTemp[6]  = 0x20;
    ucTemp[7]  = 0x33;
    ucTemp[8]  = 0x06;
    ucTemp[9]  = 0x00;
    ucTemp[10] = 0x00;
    ucTemp[11] = 0x00;
    ucTemp[12] = 0x03;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 13, pTuner->m_pAVLChip );

    ucTemp[0]  = 0x11;
    ucTemp[1]  = 0xFF;
    ucTemp[2]  = 0xF3;
    ucTemp[3]  = 0x00;
    ucTemp[4]  = 0xEA;
    ucTemp[5]  = 0x64;
    ucTemp[6]  = 0xA6;
    ucTemp[7]  = 0x86;
    ucTemp[8]  = 0xCC;
    ucTemp[9]  = 0xCF;
    ucTemp[10] = 0x95;
    ucTemp[11] = 0xF1;
    ucTemp[12] = 0xA8;
    ucTemp[13] = 0xF2;
    ucTemp[14] = 0x09;
    ucTemp[15] = 0x00;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 16, pTuner->m_pAVLChip );
    if (AVL_DVBSx_EC_OK != r)
    {
        return r;
    }

    /* mode setting: normal mode */
    ucTemp[0] = 0x01;
    ucTemp[1] = 0x1c;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x03;
    ucTemp[1] = 0x10;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x05;
    ucTemp[1] = 0xc5;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    return r;
}

AVL_DVBSx_ErrorCode ExtSharpQM1D1C0045_Reset(struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r;
    AVL_uchar ucTemp[16];

    /* One times soft reset */
    ucTemp[0] = 0x01;
    ucTemp[1] = 0x0c;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        return r;
    }

    AVL_DVBSx_IBSP_Delay(2); /* 2ms */

    /* LPF_Tuning ON */
    ucTemp[0] = 0x0c;
    ucTemp[1] = 0x43;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        return r;
    }

    AVL_DVBSx_IBSP_Delay(20); /* 20ms */

    /* write Fixed Value register */
    ucTemp[0]  = 0x01;
    ucTemp[1]  = 0x1C;
    ucTemp[2]  = 0xC0;
    ucTemp[3]  = 0x10;
    ucTemp[4]  = 0xBC;
    ucTemp[5]  = 0xC1;
    ucTemp[6]  = 0x15;
    ucTemp[7]  = 0x34;
    ucTemp[8]  = 0x06;
    ucTemp[9]  = 0x3e;
    ucTemp[10] = 0x00;
    ucTemp[11] = 0x00;
    ucTemp[12] = 0x43;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 13, pTuner->m_pAVLChip);

    ucTemp[0]  = 0x11;
    ucTemp[1]  = 0xFF;
    ucTemp[2]  = 0xF3;
    ucTemp[3]  = 0x00;
    ucTemp[4]  = 0x3E;
    ucTemp[5]  = 0x25;
    ucTemp[6]  = 0x5C;
    ucTemp[7]  = 0xD6;
    ucTemp[8]  = 0x55;
    ucTemp[9]  = 0x8F;
    ucTemp[10] = 0x95;
    ucTemp[11] = 0xFE; /* 0xF6; */
    ucTemp[12] = 0x36;
    ucTemp[13] = 0xF2;
    ucTemp[14] = 0x09;
    ucTemp[15] = 0x00;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 16, pTuner->m_pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        return r;
    }

    /* mode setting: normal mode */
    ucTemp[0] = 0x01;
    ucTemp[1] = 0x1e;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

    ucTemp[0] = 0x03;
    ucTemp[1] = 0x10;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

    ucTemp[0] = 0x05;
    ucTemp[1] = 0xc1;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

    return r;
}


AVL_DVBSx_ErrorCode ExtSharpQM1D1C004x_Initialize(struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r;
    AVL_uchar ucTemp;

    r  = AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_slave_addr_addr, pTuner->m_uiSlaveAddress);
    r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_use_internal_control_addr, 0);
    /* clean up the LPF margin for blind scan. for external driver, this must be zero. */
    r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_LPF_margin_100kHz_addr, 0); 
    /* 
     * Set up the right LPF for blind scan to regulate the freq_step. 
     * This field should corresponding the flat response part of the LPF.
     */
    r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_max_LPF_100kHz_addr, 320); 
    r |= AVL_DVBSx_II2CRepeater_Initialize(pTuner->m_uiI2CBusClock_kHz, pTuner->m_pAVLChip);

    ucTemp = 0x00;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), &ucTemp, 1, pTuner->m_pAVLChip);
    r = AVL_DVBSx_II2CRepeater_ReadData((AVL_uchar)(pTuner->m_uiSlaveAddress), (AVL_puchar)(&Chip_ID), 1,
                                        pTuner->m_pAVLChip);

    if ((Chip_ID != QM1D1C0042) && (Chip_ID != QM1D1C0045) && (Chip_ID != QM1D1C0045_2))
    {
        return AVL_DVBSx_EC_GeneralFail;
    }

    r |= AVL_DVBSx_II2C_Write32(pTuner->m_pAVLChip, 0x640160, 0xF3);
    r |= AVL_DVBSx_IBSP_Delay(5);
    r |= AVL_DVBSx_II2C_Write32(pTuner->m_pAVLChip, 0x640160, 0xD3);
    r |= AVL_DVBSx_IBSP_Delay(5);

    if (Chip_ID == QM1D1C0042)
    {
        r = ExtSharpQM1D1C0042_Reset(pTuner);
    }

    if ((Chip_ID == QM1D1C0045) || (Chip_ID == QM1D1C0045_2))
    {
        r = ExtSharpQM1D1C0045_Reset(pTuner);
    }

    return r;
}

AVL_DVBSx_ErrorCode ExtSharpQM1D1C004x_GetLockStatus(struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r;
    AVL_uchar ucTemp;

    ucTemp = 0x0d;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), &ucTemp, 1, pTuner->m_pAVLChip);

    r = AVL_DVBSx_II2CRepeater_ReadData((AVL_uchar)(pTuner->m_uiSlaveAddress), (AVL_puchar)(&ucTemp), 1,
                                        pTuner->m_pAVLChip);
    if (AVL_DVBSx_EC_OK == r)
    {
        if (0 == (ucTemp & SHARPQM1D1C004x_PLL_LOCK))
        {
            r = AVL_DVBSx_EC_Running;
        }
    }

    return (r);
}

AVL_DVBSx_ErrorCode ExtSharpQM1D1C004x_Lock(struct AVL_Tuner * pTuner)
{
    AVL_DVBSx_ErrorCode r;
    static AVL_uchar ucFlag = 0;
    AVL_uchar ucTemp[4];
    AVL_uchar N;
    AVL_uchar A;
    AVL_int32 sd;

    if (0 == ucFlag) /* first time, case1 */
    {
        ucFlag = 1;

        ucTemp[0] = 0x02;
        ucTemp[1] = ExtSharpQM1D1C004x_VCO_Setting(pTuner->m_uiFrequency_100kHz / 10);
        r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        if ((Chip_ID == QM1D1C0045) || (Chip_ID == QM1D1C0045_2))
        {
            ucTemp[0] = 0x03;
            ucTemp[1] = ExtSharpQM1D1C0045_DIV45_Setting(pTuner->m_uiFrequency_100kHz / 10);
            r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);
        }

        ExtSharpQM1D1C004x_Calc_LF(pTuner->m_uiFrequency_100kHz / 10, &N, &A, &sd);
        ucTemp[0] = 0x06;
        ucTemp[1] = N & 0x3f;
        if (16 != OSCILLATION_FREQUENCY_MHZ)
        {
            ucTemp[1] = ucTemp[1] + 0x80;
        }

        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ucTemp[0] = 0x07;
        ucTemp[1] = (A & 0xf) + 0x30;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ucTemp[0] = 0x08;
        ucTemp[1] = ExtSharpQM1D1C004x_LPF_FC();
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ucTemp[0] = 0x09;
        ucTemp[1] = (sd >> 16) & 0x3f;
        ucTemp[2] = (sd >> 8) & 0xff;
        ucTemp[3] = sd & 0xff;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 4, pTuner->m_pAVLChip);

        ucTemp[0] = 0x0C;
        ucTemp[1] = 0x03;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        AVL_DVBSx_IBSP_Delay(2); /* 2ms */

        ucTemp[0] = 0x0C;
        ucTemp[1] = 0xc3;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        AVL_DVBSx_IBSP_Delay(20); /* 20ms */

        ucTemp[0] = 0x08;
        ucTemp[1] = (((pTuner->m_uiLPF_100kHz / 10) + ((pTuner->m_uiLPF_100kHz / 10) % 2)) - 4) / 2;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ucTemp[0] = 0x13;
        ucTemp[1] = ExtSharpQM1D1C004x_CSEL_Offset(pTuner->m_uiLPF_100kHz);
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);
    }
    else /* case2 */
    {
        ucTemp[0] = 0x02;
        ucTemp[1] = ExtSharpQM1D1C004x_VCO_Setting(pTuner->m_uiFrequency_100kHz / 10);
        r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        if ((Chip_ID == QM1D1C0045) || (Chip_ID == QM1D1C0045_2))
        {
            ucTemp[0] = 0x03;
            ucTemp[1] = ExtSharpQM1D1C0045_DIV45_Setting(pTuner->m_uiFrequency_100kHz / 10);
            r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);
        }

        ucTemp[0] = 0x03;
        ucTemp[1] = ExtSharpQM1D1C0045_DIV45_Setting(pTuner->m_uiFrequency_100kHz / 10);
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ExtSharpQM1D1C004x_Calc_LF(pTuner->m_uiFrequency_100kHz / 10, &N, &A, &sd);
        ucTemp[0] = 0x06;
        ucTemp[1] = N & 0x3f;
        if (16 != OSCILLATION_FREQUENCY_MHZ)
        {
            ucTemp[1] = ucTemp[1] + 0x80;
        }

        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ucTemp[0] = 0x07;
        ucTemp[1] = (A & 0xf) + 0x30;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ucTemp[0] = 0x08;
        ucTemp[1] = (((pTuner->m_uiLPF_100kHz / 10) + ((pTuner->m_uiLPF_100kHz / 10) % 2)) - 4) / 2;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ucTemp[0] = 0x09;
        ucTemp[1] = (sd >> 16) & 0x3f;
        ucTemp[2] = (sd >> 8) & 0xff;
        ucTemp[3] = sd & 0xff;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 4, pTuner->m_pAVLChip);

        ucTemp[0] = 0x13;
        ucTemp[1] = ExtSharpQM1D1C004x_CSEL_Offset(pTuner->m_uiLPF_100kHz);
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        ucTemp[0] = 0x0C;
        ucTemp[1] = 0x43;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        AVL_DVBSx_IBSP_Delay(1); /* 1ms */

        ucTemp[0] = 0x0C;
        ucTemp[1] = 0xC3;
        r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);

        AVL_DVBSx_IBSP_Delay(15); /* 1ms */
    }

    return r;
}
#endif

/*driver for hi3136, beginning*/
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
static HI_S32 sharp_write(HI_U32 u32TunerPort, HI_U8 enI2cChannel,
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

static HI_S32 SharpQM1D1C0042_Reset(HI_U32 u32TunerPort)
{
    //HI_U8 reg_start;
    HI_U8 tuner_address;
    HI_U8 u8Reg[16];
    HI_U8 enI2cChannel = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    enI2cChannel = g_stTunerOps[u32TunerPort].enI2cChannel;
    tuner_address = g_stTunerOps[u32TunerPort].u32TunerAddress;

    u8Reg[0] = 0x01;
    u8Reg[1] = 0x0c;\

    //printk("%s, %d\n", __func__, __LINE__);
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret |= sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret |= sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    msleep_interruptible(2);

    u8Reg[0] = 0x0c;
    u8Reg[1] = 0x43;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret |= sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    msleep_interruptible(20);

    u8Reg[0]  = 0x01;
    u8Reg[1]  = 0x1C;
    u8Reg[2]  = 0xA0;
    u8Reg[3]  = 0x10;
    u8Reg[4]  = 0xBC;
    u8Reg[5]  = 0xC5;
    u8Reg[6]  = 0x20;
    u8Reg[7]  = 0x33;
    u8Reg[8]  = 0x06;
    u8Reg[9]  = 0x00;
    u8Reg[10] = 0x00;
    u8Reg[11] = 0x00;
    u8Reg[12] = 0x03;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret |= sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 12);
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0]  = 0x11;
    u8Reg[1]  = 0xFF;
    u8Reg[2]  = 0xF3;
    u8Reg[3]  = 0x00;
    u8Reg[4]  = 0xEA;
    u8Reg[5]  = 0x64;
    u8Reg[6]  = 0xA6;
    u8Reg[7]  = 0x86;
    u8Reg[8]  = 0xCC;
    u8Reg[9]  = 0xCF;
    u8Reg[10] = 0x95;
    u8Reg[11] = 0xF1;
    u8Reg[12] = 0xA8;
    u8Reg[13] = 0xF2;
    u8Reg[14] = 0x09;
    u8Reg[15] = 0x00;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret |= sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 15);
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0] = 0x01;
    u8Reg[1] = 0x1c;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret |= sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0] = 0x03;
    u8Reg[1] = 0x10;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret |= sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0] = 0x05;
    u8Reg[1] = 0xc5;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret |= sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    return s32Ret;
}

HI_S32 SharpQM1D1C0045_Reset(HI_U32 u32TunerPort)
{
    //HI_U8 reg_start;
    HI_U8 tuner_address;
    HI_U8 u8Reg[16];
    HI_U8 enI2cChannel = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    enI2cChannel = g_stTunerOps[u32TunerPort].enI2cChannel;
    tuner_address = g_stTunerOps[u32TunerPort].u32TunerAddress;
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0] = 0x01;
    u8Reg[1] = 0x0c;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

    msleep_interruptible(2);
    //printk("%s, %d\n", __func__, __LINE__);
    /* LPF_Tuning ON */
    u8Reg[0] = 0x0c;
    u8Reg[1] = 0x43;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    msleep_interruptible(20);

    /* write Fixed Value register */
    u8Reg[0]  = 0x01;
    u8Reg[1]  = 0x1C;
    u8Reg[2]  = 0xC0;
    u8Reg[3]  = 0x10;
    u8Reg[4]  = 0xBC;
    u8Reg[5]  = 0xC1;
    u8Reg[6]  = 0x15;
    u8Reg[7]  = 0x34;
    u8Reg[8]  = 0x06;
    u8Reg[9]  = 0x3e;
    u8Reg[10] = 0x00;
    u8Reg[11] = 0x00;
    u8Reg[12] = 0x43;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 12);
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0]  = 0x11;
    u8Reg[1]  = 0xFF;
    u8Reg[2]  = 0xF3;
    u8Reg[3]  = 0x00;
    u8Reg[4]  = 0x3E;
    u8Reg[5]  = 0x25;
    u8Reg[6]  = 0x5C;
    u8Reg[7]  = 0xD6;
    u8Reg[8]  = 0x55;
    u8Reg[9]  = 0x8F;
    u8Reg[10] = 0x95;
    u8Reg[11] = 0xFE; /* 0xF6; */
    u8Reg[12] = 0x36;
    u8Reg[13] = 0xF2;
    u8Reg[14] = 0x09;
    u8Reg[15] = 0x00;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 15);
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0] = 0x01;
    u8Reg[1] = 0x1e;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0] = 0x03;
    u8Reg[1] = 0x10;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    u8Reg[0] = 0x05;
    u8Reg[1] = 0xc1;
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    //printk("%s, %d\n", __func__, __LINE__);
    return s32Ret;
}

HI_S32 sharp7903_init_tuner(HI_U32 u32TunerPort)
{
    //printk("%s, %d\n", __func__, __LINE__);
    //HI_U8 reg_start;
    HI_U8 tuner_address;
    HI_U8 enI2cChannel = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    enI2cChannel = g_stTunerOps[u32TunerPort].enI2cChannel;
    tuner_address = g_stTunerOps[u32TunerPort].u32TunerAddress;
#if 1
    //printk("%s, %d\n", __func__, __LINE__);
    hi3136_config_i2c_out(u32TunerPort, 1);
    s32Ret = HI_DRV_I2C_Read(enI2cChannel, tuner_address, 0, 1, (HI_U8 *)(&Chip_ID), 1);	
    //printk("%s, %d, %x\n", __func__, __LINE__, Chip_ID);
    if ((Chip_ID != QM1D1C0042) && (Chip_ID != QM1D1C0045) && (Chip_ID != QM1D1C0045_2))
    {
    //printk("%s, %d\n", __func__, __LINE__);
        return HI_FAILURE;
    }
    //printk("%s, %d\n", __func__, __LINE__);
#endif
    if (Chip_ID == QM1D1C0042)
    {
    //printk("%s, %d\n", __func__, __LINE__);
        s32Ret = SharpQM1D1C0042_Reset(u32TunerPort);
    //printk("%s, %d\n", __func__, __LINE__);
    }
    //printk("%s, %d\n", __func__, __LINE__);
    if ((Chip_ID == QM1D1C0045) || (Chip_ID == QM1D1C0045_2))
    {
    //printk("%s, %d\n", __func__, __LINE__);
        s32Ret = SharpQM1D1C0045_Reset(u32TunerPort);
    //printk("%s, %d\n", __func__, __LINE__);
    }
    //printk("%s, %d\n", __func__, __LINE__);
    return s32Ret;
}

HI_S32 sharp7903_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF/*kHz*/)  //freq :unit MHZ, symbol:unit  Khz
{
    static HI_U8 u8Flag = 0;
    HI_U8 u8Reg[4];
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 tuner_address;
    HI_U8 N;
    HI_U8 A;
    HI_S32 sd;

    tuner_address = g_stTunerOps[u32TunerPort].u32TunerAddress;

    if (0 == u8Flag)
    {
        u8Flag = 1;
    //printk("%s, %d\n", __func__, __LINE__);
        u8Reg[0] = 0x02;
        u8Reg[1] = ExtSharpQM1D1C004x_VCO_Setting(u32PuRF / 1000);
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        if ((Chip_ID == QM1D1C0045) || (Chip_ID == QM1D1C0045_2))
        {
            u8Reg[0] = 0x03;
            u8Reg[1] = ExtSharpQM1D1C0045_DIV45_Setting(u32PuRF / 1000);
            hi3136_config_i2c_out(u32TunerPort, 1);
            s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
        }

        ExtSharpQM1D1C004x_Calc_LF(u32PuRF / 1000, &N, &A, &sd);
        u8Reg[0] = 0x06;
        u8Reg[1] = N & 0x3f;
        if (16 != OSCILLATION_FREQUENCY_MHZ)
        {
            u8Reg[1] = u8Reg[1] + 0x80;
        }

        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        u8Reg[0] = 0x07;
        u8Reg[1] = (A & 0xf) + 0x30;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        u8Reg[0] = 0x08;
        u8Reg[1] = ExtSharpQM1D1C004x_LPF_FC();
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        u8Reg[0] = 0x09;
        u8Reg[1] = (sd >> 16) & 0x3f;
        u8Reg[2] = (sd >> 8) & 0xff;
        u8Reg[3] = sd & 0xff;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 3);

        u8Reg[0] = 0x0C;
        u8Reg[1] = 0x03;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        msleep_interruptible(2);

        u8Reg[0] = 0x0C;
        u8Reg[1] = 0xc3;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        msleep_interruptible(20);

        u8Reg[0] = 0x08;
        //u8Reg[1] = (((u32PuRF / 1000) + ((u32PuRF / 1000) % 2)) - 4) / 2;
        u8Reg[1] = (((u32LPF_KHz[u32TunerPort] / 1000) + ((u32LPF_KHz[u32TunerPort] / 1000) % 2)) - 4) / 2;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        u8Reg[0] = 0x13;
        u8Reg[1] = ExtSharpQM1D1C004x_CSEL_Offset(u32PuRF / 100);
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
    }
    else
    {
    //printk("%s, %d\n", __func__, __LINE__);
        u8Reg[0] = 0x02;
        u8Reg[1] = ExtSharpQM1D1C004x_VCO_Setting(u32PuRF / 1000);
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        if ((Chip_ID == QM1D1C0045) || (Chip_ID == QM1D1C0045_2))
        {
            u8Reg[0] = 0x03;
            u8Reg[1] = ExtSharpQM1D1C0045_DIV45_Setting(u32PuRF / 1000);
            hi3136_config_i2c_out(u32TunerPort, 1);
            s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);
        }

        u8Reg[0] = 0x03;
        u8Reg[1] = ExtSharpQM1D1C0045_DIV45_Setting(u32PuRF / 1000);
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        ExtSharpQM1D1C004x_Calc_LF(u32PuRF / 1000, &N, &A, &sd);
        u8Reg[0] = 0x06;
        u8Reg[1] = N & 0x3f;
        if (16 != OSCILLATION_FREQUENCY_MHZ)
        {
            u8Reg[1] = u8Reg[1] + 0x80;
        }

        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        u8Reg[0] = 0x07;
        u8Reg[1] = (A & 0xf) + 0x30;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        u8Reg[0] = 0x08;
        //u8Reg[1] = (((u32PuRF / 1000) + ((u32PuRF / 1000) % 2)) - 4) / 2;
        u8Reg[1] = (((u32LPF_KHz[u32TunerPort] / 1000) + ((u32LPF_KHz[u32TunerPort] / 1000) % 2)) - 4) / 2;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        u8Reg[0] = 0x09;
        u8Reg[1] = (sd >> 16) & 0x3f;
        u8Reg[2] = (sd >> 8) & 0xff;
        u8Reg[3] = sd & 0xff;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 3);

        u8Reg[0] = 0x13;
        u8Reg[1] = ExtSharpQM1D1C004x_CSEL_Offset(u32PuRF / 100);
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        u8Reg[0] = 0x0C;
        u8Reg[1] = 0x43;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        msleep_interruptible(1);

        u8Reg[0] = 0x0C;
        u8Reg[1] = 0xc3;
        hi3136_config_i2c_out(u32TunerPort, 1);
        s32Ret = sharp_write(u32TunerPort, enI2cChannel, tuner_address, u8Reg[0], u8Reg + 1, 1);

        msleep_interruptible(15);
    }

    return s32Ret;
}
#endif

