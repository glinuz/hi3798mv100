#include <linux/string.h>
#include <linux/time.h>

#include "hi_drv_mem.h"
#include "audio_util.h"

//#define AO_MEM_TRACE_DEBUG            /* HI_ID_AO   memory debug trace  */
//#define AIAO_MEM_TRACE_DEBUG          /* HI_ID_AIAO memory debug trace  */

#ifdef AIAO_MEM_TRACE_DEBUG
static int g_u32MallocCount_AIAO = 0;
#endif

#ifdef AIAO_MEM_TRACE_DEBUG
static int g_u32MallocCount_AO = 0;
#endif

HI_VOID* AUTIL_AIAO_MALLOC(HI_U32 u32ModuleID, HI_U32 u32Size, HI_S32 flag)
{
    HI_VOID* pMemAddr = NULL;

#ifdef __KERNEL__
    pMemAddr = HI_KMALLOC(u32ModuleID, u32Size, flag);
#else
    pMemAddr = malloc(u32Size);
#endif

#ifdef AIAO_MEM_TRACE_DEBUG
    if (HI_ID_AIAO != u32ModuleID)
    {
        HI_ERR_AO("u32ModuleID(0x%x) should be(0x%x) \n", u32ModuleID, HI_ID_AIAO);
    }

    HI_ERR_AO("malloc u32ModuleID(0x%.4x), size(0x%.6x),pMemAddr(%p), aiao_cnt(%d) \n", u32ModuleID, u32Size, pMemAddr,g_u32MallocCount_AIAO);
    g_u32MallocCount_AIAO++;
#endif

    return pMemAddr;
}

HI_VOID AUTIL_AIAO_FREE(HI_U32 u32ModuleID, HI_VOID* pMemAddr)
{
    if (NULL != pMemAddr)
    {
#ifdef AIAO_MEM_TRACE_DEBUG
        g_u32MallocCount_AIAO--;
        if (HI_ID_AIAO != u32ModuleID)
        {
            HI_ERR_AO("u32ModuleID(0x%x) should be(0x%x) \n", u32ModuleID, HI_ID_AIAO);
        }

        HI_ERR_AO("free u32ModuleID(0x%x), pMemAddr(%p), aiao_cnt(%d) \n", u32ModuleID, pMemAddr,g_u32MallocCount_AIAO);
#endif
#ifdef __KERNEL__
        HI_KFREE(u32ModuleID, pMemAddr);
#else
        free(pMemAddr);
#endif
    }

    return;
}

HI_VOID* AUTIL_AO_MALLOC(HI_U32 u32ModuleID, HI_U32 u32Size, HI_S32 flag)
{
    HI_VOID* pMemAddr = NULL;

#ifdef __KERNEL__
    pMemAddr = HI_KMALLOC(u32ModuleID, u32Size, flag);
#else
    pMemAddr = malloc(u32Size);
#endif

#ifdef AO_MEM_TRACE_DEBUG
    if (HI_ID_AO != u32ModuleID)
    {
        HI_ERR_AO("u32ModuleID(0x%x) should be(0x%x) \n", u32ModuleID, HI_ID_AO);
    }

    HI_ERR_AO("malloc u32ModuleID(0x%.4x), size(0x%.6x),pMemAddr(%p), ao_cnt(%d) \n", u32ModuleID, u32Size, pMemAddr,g_u32MallocCount_AO);
    g_u32MallocCount_AO++;
#endif

    return pMemAddr;
}

HI_VOID AUTIL_AO_FREE(HI_U32 u32ModuleID, HI_VOID* pMemAddr)
{
    if (NULL != pMemAddr)
    {
#ifdef AO_MEM_TRACE_DEBUG
        g_u32MallocCount_AO--;
        if (HI_ID_AO != u32ModuleID)
        {
            HI_ERR_AO("u32ModuleID(0x%x) should be(0x%x) \n", u32ModuleID, HI_ID_AO);
        }

        HI_ERR_AO("free u32ModuleID(0x%x), pMemAddr(%p), ao_cnt(%d) \n", u32ModuleID, pMemAddr,g_u32MallocCount_AO);
#endif
#ifdef __KERNEL__
        HI_KFREE(u32ModuleID, pMemAddr);
#else
        free(pMemAddr);
#endif
    }

    return;
}


/*
HDMI 1.3 above supports High bit rate audio (HBR) stream which rate is more than 6.144Mbps for Dolby MAT and
DTS-HD Master audio. They are shared with 4 I2S inputs.
 */
static HI_U32 g_u32IEC61937HbrDataType[] =
{
    IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS,

    IEC61937_DATATYPE_DTS_TYPE_IV,

    IEC61937_DATATYPE_DOLBY_TRUE_HD,
};

HI_U32  AUTIL_IEC61937DataType(HI_U16 *pu16IecData, HI_U32 u32IecDataSize)
{
    HI_U32 u32IEC61937DataType;    /* Value of PC bit 0-6 */

    u32IEC61937DataType = IEC61937_DATATYPE_NULL;
    if ((HI_NULL != pu16IecData) && (u32IecDataSize > 8 * sizeof(HI_U16)))
    {
        if ((0 == pu16IecData[0]) && (0 == pu16IecData[1]) && (0 == pu16IecData[2]) && (0 == pu16IecData[3]))
        {
            pu16IecData += 4;          /* 4 word16 Burst spacing */
        }

        if (IEC_61937_SYNC1 == pu16IecData[0]) //&& (IEC_61937_SYNC2 == pu16IecData[1])
        {
            u32IEC61937DataType = pu16IecData[2] & 0x3f;
        }
        else
        {
            u32IEC61937DataType = IEC61937_DATATYPE_DTSCD;
        }
    }
    else
    {
        u32IEC61937DataType = IEC61937_DATATYPE_NULL;
    }

    return u32IEC61937DataType;
}

HI_U32 AUTIL_GetPassthFormat(HI_U32 u32Format)
{
    return u32Format & PASSTHROUGH_FORMAT_MASK;
}

HI_BOOL AUTIL_IsLbrFormat(HI_U32 u32LbrFormat)
{
    return INT_TO_BOOL(u32LbrFormat & PASSTHROUGH_FORMAT_MASK);
}

HI_BOOL AUTIL_IsHbrFormat(HI_U32 u32HbrFormat)
{
    return INT_TO_BOOL(u32HbrFormat & PASSTHROUGH_FORMAT_MASK);
}

HI_BOOL AUTIL_IsArcSupportHbr(HI_U32 u32HbrFormat)
{
    return (IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS == (u32HbrFormat & PASSTHROUGH_FORMAT_MASK));
}

static HI_BOOL IsIEC61937Hbr(HI_U32 u32IEC61937DataType, HI_U32 uSourceRate)
{
    HI_S32 n;
    HI_BOOL bHbr = HI_FALSE;

    /* check whether or not IEC61937 HBR Data */
    for (n = 0; n < (HI_S32)(sizeof(g_u32IEC61937HbrDataType) / sizeof(HI_S32)); n++)
    {
        if (g_u32IEC61937HbrDataType[n] == u32IEC61937DataType)
        {
            bHbr = HI_TRUE;
            break;
        }
    }

    if (IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS == u32IEC61937DataType)
    {
        /* discard ddp 32kHz */
        if (!((48000 == uSourceRate) || (44100 == uSourceRate)))
        {
            bHbr = HI_FALSE;
        }
    }

    return bHbr;
}

HI_BOOL  AUTIL_isIEC61937Lbr(HI_U32 u32IEC61937DataType, HI_U32 u32SourceRate)
{
    if (IEC61937_DATATYPE_NULL == u32IEC61937DataType)
    {
        return HI_FALSE;
    }

    return !IsIEC61937Hbr(u32IEC61937DataType, u32SourceRate);
}

HI_BOOL  AUTIL_isIEC61937Hbr(HI_U32 u32IEC61937DataType, HI_U32 u32SourceRate)
{
    if (IEC61937_DATATYPE_NULL == u32IEC61937DataType)
    {
        return HI_FALSE;
    }

    return IsIEC61937Hbr(u32IEC61937DataType, u32SourceRate);
}

static HI_BOOL GetCompressFlag(HI_U32 u32BitsBytesPerFrame)
{
    return INT_TO_BOOL(u32BitsBytesPerFrame & AOFRAME_PASSTHROUGH_COMPRESS_MASK);
}

static HI_U32 GetCompressIECDataType(HI_U32 u32IECDataType)
{
    return (u32IECDataType | TRACKATTR_PASSTHROUGH_COMPRESS_MASK);
}

HI_U32 AUTIL_GetIECDataType(HI_U32 u32IECDataType, HI_U32 u32BitsBytesPerFrame)
{
    HI_BOOL bCompressed = GetCompressFlag(u32BitsBytesPerFrame);

    if (IEC61937_DATATYPE_DOLBY_TRUE_HD == u32IECDataType)
    {
        return u32IECDataType;
    }

    if (HI_TRUE == bCompressed)
    {
        u32IECDataType = GetCompressIECDataType(u32IECDataType);
    }

    return u32IECDataType;
}

HI_BOOL AUTIL_IsTruehd(HI_U32 u32IECDataType)
{
    return INT_TO_BOOL(u32IECDataType & TRUEHD_DATATYPE_MASK);
}

HI_U32 AUTIL_GetTruehdDataType(HI_U32 u32IECDataType)
{
    return (u32IECDataType & TRUEHD_DATATYPE_MASK);
}

HI_U32 AUTIL_CalcFrameSize(HI_U32 uCh, HI_U32 uBitDepth)
{
    HI_U32 uFrameSize = 0;

    switch (uBitDepth)
    {
    case 16:
        uFrameSize = ((HI_U32)uCh) * sizeof(HI_U16);
        break;
    case 24:
        uFrameSize = ((HI_U32)uCh) * sizeof(HI_U32);
        break;
    }

    return uFrameSize;
}

HI_U32 AUTIL_LatencyMs2ByteSize(HI_U32 u32LatencyMs, HI_U32 u32FrameSize, HI_U32 u32SampleRate)
{
    return (u32SampleRate * u32LatencyMs / 1000 * u32FrameSize );
}

HI_U32 AUTIL_ByteSize2LatencyMs(HI_U32 u32DataBytes, HI_U32 u32FrameSize, HI_U32 u32SampleRate)
{
    if (!u32FrameSize || !u32SampleRate)
    {
        return 0;
    }
    else
    {
        return (u32DataBytes * 1000) / (u32FrameSize * u32SampleRate);
    }
}

/* Table of normalised fixed point common logarithms */
static HI_U16 FXlog10_table[] = {
    0x0000, 0x00DD, 0x01B9, 0x0293, 0x036B, 0x0442, 0x0517, 0x05EB,
    0x06BD, 0x078E, 0x085D, 0x092A, 0x09F6, 0x0AC1, 0x0B8A, 0x0C51,
    0x0D18, 0x0DDD, 0x0EA0, 0x0F63, 0x1024, 0x10E3, 0x11A2, 0x125F,
    0x131B, 0x13D5, 0x148F, 0x1547, 0x15FE, 0x16B4, 0x1769, 0x181C,
    0x18CF, 0x1980, 0x1A30, 0x1ADF, 0x1B8D, 0x1C3A, 0x1CE6, 0x1D91,
    0x1E3B, 0x1EE4, 0x1F8C, 0x2033, 0x20D9, 0x217E, 0x2222, 0x22C5,
    0x2367, 0x2409, 0x24A9, 0x2548, 0x25E7, 0x2685, 0x2721, 0x27BD,
    0x2858, 0x28F3, 0x298C, 0x2A25, 0x2ABD, 0x2B54, 0x2BEA, 0x2C7F,
    0x2D14, 0x2DA8, 0x2E3B, 0x2ECD, 0x2F5F, 0x2FF0, 0x3080, 0x310F,
    0x319E, 0x322C, 0x32B9, 0x3345, 0x33D1, 0x345C, 0x34E7, 0x3571,
    0x35FA, 0x3682, 0x370A, 0x3792, 0x3818, 0x389E, 0x3923, 0x39A8,
    0x3A2C, 0x3AB0, 0x3B32, 0x3BB5, 0x3C36, 0x3CB7, 0x3D38, 0x3DB8,
    0x3E37, 0x3EB6, 0x3F34, 0x3FB2, 0x402F, 0x40AC, 0x4128, 0x41A3,
    0x421E, 0x4298, 0x4312, 0x438C, 0x4405, 0x447D, 0x44F5, 0x456C,
    0x45E3, 0x4659, 0x46CF, 0x4744, 0x47B9, 0x482E, 0x48A2, 0x4915,
    0x4988, 0x49FB, 0x4A6D, 0x4ADE, 0x4B50, 0x4BC0, 0x4C31, 0x4CA0,
    0x4D10,
};

/* Table of fixed point common logarithms for the powers of 2 */

static HI_U32 FXlog2_table[] = {
    0x00000000L, 0x00004D10L, 0x00009A20L, 0x0000E730L,
    0x00013441L, 0x00018151L, 0x0001CE61L, 0x00021B72L,
    0x00026882L, 0x0002B592L, 0x000302A3L, 0x00034FB3L,
    0x00039CC3L, 0x0003E9D3L, 0x000436E4L, 0x000483F4L,
    0x0004d104L,
};

typedef HI_S32 FXFixed;            /* 16.16 format                 */

static FXFixed FHLL_mul(FXFixed f, FXFixed g)

/****************************************************************************
*
* Function:     FHLL_mul
* Parameters:   f   - FXFixed point mutiplicand
*               g   - FXFixed point number to multiply by
* Returns:      Result of the multiplication.
*
* Description:  Multiplies two fixed point number in 16.16 format together
*               and returns the result. We cannot simply multiply the
*               two 32 bit numbers together since we need to shift the
*               64 bit result right 16 bits, but the result of a FXFixed
*               multiply is only ever 32 bits! Thus we must resort to
*               computing it from first principles (this is slow and
*               should ideally be re-coded in assembler for the target
*               machine).
*
*               We can visualise the fixed point number as having two
*               parts, a whole part and a fractional part:
*
*                   FXFixed = (whole + frac * 2^-16)
*
*               Thus if we multiply two of these numbers together we
*               get a 64 bit result:
*
*               (a_whole + a_frac * 2^-16) * (b_whole + b_frac * 2^-16)
*
*                 = (a_whole * b_whole) +
*                   (a_whole * b_frac)*2^-16 +
*                   (b_whole * a_frac)*2^-16 +
*                   (a_frac * b_frac)*2^-32
*
*               To convert this back to a 64 bit fixed point number to 32
*               bit format we simply shift it right by 16 bits (we can round
*               it by adding 2^-17 before doing this shift). The formula
*               with the shift integrated is what is used below. Natrually
*               you can alleviate most of this if the target machine can
*               perform a native 32 by 32 bit multiplication (since it
*               will produce a 64 bit result).
*
****************************************************************************/
{
    FXFixed a_whole, b_whole;
    FXFixed a_frac, b_frac;

    // Extract the whole and fractional parts of the numbers. We strip the
    // sign bit from the fractional parts but leave it intact for the
    // whole parts. This ensures that the sign of the result will be correct.

    a_frac  = f & 0x0000FFFF;
    a_whole = f >> 16;
    b_frac  = g & 0x0000FFFF;
    b_whole = g >> 16;

    // We round the result by adding 2^(-17) before we shift the
    // fractional part of the result left 16 bits.

    return ((a_whole * b_whole) << 16) +
           (a_whole * b_frac) +
           (a_frac * b_whole) +
           ((a_frac * b_frac + 0x8000) >> 16);
}

#define FXmul(f, g) FHLL_mul(f, g)

static FXFixed FHLL_log10(FXFixed f)

/****************************************************************************
*
* Function:     FHLL_log10
* Parameters:   f   - Number to take the square root of
* Returns:      Approximate square root of the number f
*
* Description:  Caculates the common logarithm of a fixed point number
*               using table lookup and linear interpolation.
*
*               First we isolate the first 8 bits of the mantissa in our
*               fixed point number. We do this by scanning along until we
*               find the first 1 bit in the number, and shift it all right
*               until this is in bit position 7. Since IEEE floating point
*               numbers have an implied 1 bit in the mantissa, we mask this
*               bit out and use the 7 bits as an index into the table. We
*               then look up this value, and add in the appropriate logarithm
*               for the power for two represented by the numbers exponent.
*
*               Because of the linear interpolation, this routine will
*               provide a common logarithm of any 16.16 fixed point
*               number that is as good as you can get given the precision
*               of fixed point (approx 1e-4 deviation).
*
****************************************************************************/
{
    HI_S16 e, eindex;                // Exponent and index into table
    FXFixed r, diff, interpolant;

    if (f <= 0)
    {
        // Check for -ve and zero
        return 0;
    }

    // Search for the index of the first 1 bit in the number (start of
    // the mantissa. Note that we are only working with positive numbers
    // here, so we ignore the sign bit (bit 31).
    e = 14;                         // Exponent for number with 1 in bit
                                    // position 30
    while ((f & 0x40000000) == 0)
    {
        // Isolate first bit
        f <<= 1;                    // Shift all left 1
        e--;                        // Decrement exponent for number
    }

    // At this stage our number is in the following format:
    //
    //   bits 23-30        15-22       0-14
    //  +-------------+-------------+---------+
    //  |.table index.|.interpolant.|.ignored.|
    //  +-------------+-------------+---------+
    //
    // We compute the index into the table by shifting the mantissa
    // so that the first 1 bit ends up in bit position 7, and mask it
    // out. The interpolant factor that we use is the bottom 16
    // bits left in the original number after the index is extracted out,
    // and is used to linearly interpolate the results between the two
    // consecutive entries in the table.
    eindex = (HI_S16)(f >> 23) & 0x7F;
    interpolant = (f >> 7) & 0xFFFF;

    // Lookup the values for the 7 bits of mantissa in the table, and
    // linearly interpolate between the two entries.
    diff = FXlog10_table[eindex + 1] - (r = FXlog10_table[eindex]);
    r += FXmul(diff, interpolant);

    // Now find the appropriate power of 2 logarithm to add to the final
    // result.
    if (e < 0)
    {
        r -= FXlog2_table[-e];
    }
    else
    {
        r += FXlog2_table[e];
    }

    return r;
}

HI_U32 AUTIL_VolumeLinear2RegdB(HI_U32 u32Linear)
{
    HI_U32 RegdB;

    if (u32Linear > AO_MAX_LINEARVOLUME)
    {
         HI_ERR_AO("invalid LinearVolume(%d)\n", u32Linear);
         return HI_ERR_AO_INVALID_PARA;
    }

    /* if mute(volume=0), avoid tolerance in calculate FHLL_log10 */
    if ((u32Linear == 0))
    {
        RegdB = VOLUME_infdB;
    }
    else
    {
        /* 20*log(alpha1/100.0) + VOLUME_0dB; */
        RegdB = (((HI_U32) ((20 * FHLL_log10(u32Linear << 16)) + 0x8000) >> 16) - 40) + VOLUME_0dB;
    }

    return RegdB;
}

HI_U32 AUTIL_VolumedB2RegdB(HI_S32 dBVol)
{
    return (HI_U32)(dBVol + VOLUME_0dB);
}

HI_S32 AUTIL_DecimalVolumedB2RegdB(HI_S32 dBDecVol)
{
    return dBDecVol * VOLUME_PRECISION_VALUE / 1000;
}

HI_S32 AUTIL_SetBitZeroOrOne(HI_U32* pu32Val, HI_U32 u32Bit, HI_U32 u32ZeroOrOne)
{
    if (0 == u32ZeroOrOne)
    {
        *pu32Val &= ~((HI_U32)1L << u32Bit);
    }
    else if (1 == u32ZeroOrOne)
    {
        *pu32Val |= (HI_U32)1L << u32Bit;
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

const AIAO_TRACK_MODE_E AUTIL_TrackModeTransform(HI_UNF_TRACK_MODE_E enMode)
{
    switch (enMode)
    {
       case HI_UNF_TRACK_MODE_STEREO:
           return AIAO_TRACK_MODE_STEREO;

       case HI_UNF_TRACK_MODE_DOUBLE_MONO:
           return AIAO_TRACK_MODE_DOUBLE_MONO;

       case HI_UNF_TRACK_MODE_DOUBLE_LEFT:
           return AIAO_TRACK_MODE_DOUBLE_LEFT;

      case HI_UNF_TRACK_MODE_DOUBLE_RIGHT:
           return AIAO_TRACK_MODE_DOUBLE_RIGHT;

       case HI_UNF_TRACK_MODE_EXCHANGE:
           return AIAO_TRACK_MODE_EXCHANGE;

       case HI_UNF_TRACK_MODE_ONLY_RIGHT:
           return AIAO_TRACK_MODE_ONLY_RIGHT;

       case HI_UNF_TRACK_MODE_ONLY_LEFT:
           return AIAO_TRACK_MODE_ONLY_LEFT;

       case HI_UNF_TRACK_MODE_MUTED:
           return AIAO_TRACK_MODE_MUTED;

       default:
           return (AIAO_TRACK_MODE_E)enMode;
    }
}

#if (1 == HI_PROC_SUPPORT)
const HI_CHAR *AUTIL_Port2Name(HI_UNF_SND_OUTPUTPORT_E enPort)
{
    switch(enPort)
    {
        case HI_UNF_SND_OUTPUTPORT_DAC0:
            return "DAC0";
        case HI_UNF_SND_OUTPUTPORT_I2S0:
            return "I2S0";
        case HI_UNF_SND_OUTPUTPORT_I2S1:
            return "I2S1";
        case HI_UNF_SND_OUTPUTPORT_SPDIF0:
            return "SPDIF0";
        case HI_UNF_SND_OUTPUTPORT_HDMI0:
            return "HDMI0";
        case HI_UNF_SND_OUTPUTPORT_ARC0:
            return "ARC0";
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC1:
            return "DAC1";
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC2:
            return "DAC2";
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC3:
            return "DAC3";
        default:
            return "UnknownPort";
    }

}

const HI_UNF_SND_OUTPUTPORT_E AUTIL_PortName2Port(HI_CHAR *pcName)
{
    if(!strncmp(pcName, "DAC0", 4))
        return HI_UNF_SND_OUTPUTPORT_DAC0;
    else if(!strncmp(pcName, "I2S0", 4))
        return HI_UNF_SND_OUTPUTPORT_I2S0;
    else if(!strncmp(pcName, "I2S1", 4))
        return HI_UNF_SND_OUTPUTPORT_I2S1;
    else if(!strncmp(pcName, "SPDIF0", 6))
        return HI_UNF_SND_OUTPUTPORT_SPDIF0;
    else if(!strncmp(pcName, "HDMI0", 5))
        return HI_UNF_SND_OUTPUTPORT_HDMI0;
    else if(!strncmp(pcName, "ARC0", 4))
        return HI_UNF_SND_OUTPUTPORT_ARC0;
    else if(!strncmp(pcName, "DAC1", 4))
        return HI_UNF_SND_OUTPUTPORT_EXT_DAC1;
    else if(!strncmp(pcName, "DAC2", 4))
        return HI_UNF_SND_OUTPUTPORT_EXT_DAC2;
    else if(!strncmp(pcName, "DAC3", 4))
        return HI_UNF_SND_OUTPUTPORT_EXT_DAC3;
    else if(!strncmp(pcName, "ALLPORT", 3))
        return HI_UNF_SND_OUTPUTPORT_ALL;
    else
        return HI_UNF_SND_OUTPUTPORT_BUTT;
}

const HI_CHAR *AUTIL_AiPort2Name(HI_UNF_AI_E enAiPort)
{
    switch(enAiPort)
    {
        case HI_UNF_AI_I2S0:
            return "I2S0";
        case HI_UNF_AI_I2S1:
            return "I2S1";
        case HI_UNF_AI_ADC0:
            return "ADC0";
        case HI_UNF_AI_ADC1:
            return "ADC1";
        case HI_UNF_AI_ADC2:
            return "ADC2";
        case HI_UNF_AI_ADC3:
            return "ADC3";
        case HI_UNF_AI_ADC4:
            return "ADC4";
        case HI_UNF_AI_SIF0:
            return "SIF0";
        case HI_UNF_AI_HDMI0:
            return "HDMI0";
        case HI_UNF_AI_HDMI1:
            return "HDMI1";
        case HI_UNF_AI_HDMI2:
            return "HDMI2";
        case HI_UNF_AI_HDMI3:
            return "HDMI3";
        default:
            return "UnknownPort";
    }
}

const HI_CHAR *AUTIL_TrackMode2Name(HI_UNF_TRACK_MODE_E enMode)
{
    const HI_CHAR *apcName[HI_UNF_TRACK_MODE_BUTT] =
    {
        "STEREO",
        "DOULBE MONO",
        "DOULBE LEFT",
        "DOULBE RIGHT",
        "EXCHANGE",
        "ONLY RIGHT",
        "ONLY LEFT",
        "MUTE",
    };

    if (enMode < HI_UNF_TRACK_MODE_BUTT)
    {
        return apcName[enMode];
    }

    return "Unknown";
}

const HI_CHAR *AUTIL_CategoryCode2Name(HI_UNF_SND_SPDIF_CATEGORYCODE_E enCategory)
{
    switch(enCategory)
    {
        case HI_UNF_SND_SPDIF_CATEGORY_GENERAL:
            return "General";
        case HI_UNF_SND_SPDIF_CATEGORY_BROADCAST_JP:
            return "Broadcast_Japan";
        case HI_UNF_SND_SPDIF_CATEGORY_BROADCAST_USA:
            return "Broadcast_USA";
        case HI_UNF_SND_SPDIF_CATEGORY_BROADCAST_EU:
            return "Broadcast_Europe";
        case HI_UNF_SND_SPDIF_CATEGORY_PCM_CODEC:
            return "PCM_Enc_Dec";
        case HI_UNF_SND_SPDIF_CATEGORY_DIGITAL_SNDSAMPLER:
            return "Sound_Sampler";
        case HI_UNF_SND_SPDIF_CATEGORY_DIGITAL_MIXER:
            return "Signal_Mixer";
        case HI_UNF_SND_SPDIF_CATEGORY_DIGITAL_SNDPROCESSOR:
            return "Sound_Processor";
        case HI_UNF_SND_SPDIF_CATEGORY_SRC:
            return "SRC";
        case HI_UNF_SND_SPDIF_CATEGORY_MD:
            return "MiniDisc";
        case HI_UNF_SND_SPDIF_CATEGORY_DVD:
            return "DVD";
        case HI_UNF_SND_SPDIF_CATEGORY_SYNTHESISER:
            return "Synthesiser";
        case HI_UNF_SND_SPDIF_CATEGORY_MIC:
            return "Microphone";
        case HI_UNF_SND_SPDIF_CATEGORY_DAT:
            return "DAT";
        case HI_UNF_SND_SPDIF_CATEGORY_DCC:
            return "DCC";
        case HI_UNF_SND_SPDIF_CATEGORY_VCR:
            return "VCR";
        default:
            return "UnknownCategory";
    }
}

const HI_CHAR *AUTIL_ScmsMode2Name(HI_UNF_SND_SPDIF_SCMSMODE_E enScms)
{
    switch(enScms)
    {
        case HI_UNF_SND_SPDIF_SCMSMODE_COPYALLOW:
            return "CopyAllow";
        case HI_UNF_SND_SPDIF_SCMSMODE_COPYONCE:
            return "CopyOnce";
        case HI_UNF_SND_SPDIF_SCMSMODE_COPYNOMORE:
            return "CopyNoMore";
        case HI_UNF_SND_SPDIF_SCMSMODE_COPYPROHIBITED:
            return "CopyDefy";
        default:
            return "UnknownScms";
    }
}

const HI_CHAR *AUTIL_HdmiMode2Name(HI_UNF_SND_HDMI_MODE_E enMode)
{
    const HI_CHAR *apcName[HI_UNF_SND_HDMI_MODE_BUTT] =
    {
        "PCM",
        "RAW",
        "HBR2LBR",
        "AUTO",
    };

    if (enMode < HI_UNF_SND_HDMI_MODE_BUTT)
    {
        return apcName[enMode];
    }

    return "Unknown";
}

const HI_CHAR *AUTIL_SpdifMode2Name(HI_UNF_SND_SPDIF_MODE_E enMode)
{
    const HI_CHAR *apcName[HI_UNF_SND_SPDIF_MODE_BUTT] =
    {
        "PCM",
        "RAW",
    };

    if (enMode < HI_UNF_SND_SPDIF_MODE_BUTT)
    {
        return apcName[enMode];
    }

    return "Unknown";
}

const HI_CHAR *AUTIL_Track2Name(HI_UNF_SND_TRACK_TYPE_E     enTrackType)
{
    const HI_CHAR *trackName[HI_UNF_SND_TRACK_TYPE_BUTT] =
    {
        "master",
        "slave",
        "virtual",
        "lowLatency",
    };

    if (enTrackType < HI_UNF_SND_TRACK_TYPE_BUTT)
    {
        return trackName[enTrackType];
    }

    return "Unknown";
}

const HI_CHAR *AUTIL_Engine2Name(SND_ENGINE_TYPE_E enEngine)
{
    const HI_CHAR *apcName[SND_ENGINE_TYPE_BUTT] =
    {
        "PCM",
        "SPDIF RAW",
        "HDMI RAW",
    };

    if (enEngine < SND_ENGINE_TYPE_BUTT)
    {
        return apcName[enEngine];
    }

    return "Unknown";
}

const HI_CHAR *AUTIL_Aip2Name(SND_AIP_TYPE_E enAip)
{
    const HI_CHAR *apcName[SND_AIP_TYPE_BUTT] =
    {
        "PCM",
        "MC PCM",
        "ASSOC PCM",
        "SPDIF RAW",
        "HDMI RAW",
    };

    if (enAip < SND_AIP_TYPE_BUTT)
    {
        return apcName[enAip];
    }

    return "Unknown";
}

const HI_CHAR *AUTIL_Format2Name(HI_U32 u32Format)
{
    switch (u32Format & 0xff)
    {
        case IEC61937_DATATYPE_NULL:
            return "PCM";

        case IEC61937_DATATYPE_DOLBY_DIGITAL:
            return (u32Format & 0xff00) ? "DD*" : "DD";

        case IEC61937_DATATYPE_DTS_TYPE_I:
        case IEC61937_DATATYPE_DTS_TYPE_II:
        case IEC61937_DATATYPE_DTS_TYPE_III:
            return (u32Format & 0xff00) ? "DTS*" : "DTS";
        case IEC61937_DATATYPE_DTSCD:
            return "DTSCD";

        case IEC61937_DATATYPE_DTS_TYPE_IV:
            return (u32Format & 0xff00) ? "DTSHD*" : "DTSHD";

        case IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS:
            return (u32Format & 0xff00) ? "DDP*" : "DDP";

        case IEC61937_DATATYPE_DOLBY_TRUE_HD:
            return "TRUEHD";

        case IEC61937_DATATYPE_71_LPCM:
            return "7.1PCM";

        case IEC61937_DATATYPE_20_LPCM:
            return "2.0PCM";

        default:
            return "Unknown";
    }
}

#ifdef __DPT__
const HI_CHAR *AUTIL_ArcMode2Name(HI_UNF_SND_ARC_AUDIO_MODE_E enMode)
{
    const HI_CHAR *apcName[HI_UNF_SND_ARC_AUDIO_MODE_BUTT] =
    {
        "PCM",
        "RAW",
        "HBR2LBR",
        "AUTO",
    };

    if (enMode < HI_UNF_SND_ARC_AUDIO_MODE_BUTT)
    {
        return apcName[enMode];
    }

    return "Unknown";
}

const HI_CHAR *AUTIL_EdidFormat2Name(HI_UNF_EDID_AUDIO_FORMAT_CODE_E enFmt)
{
    const HI_CHAR *apcName[HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT] =
    {
        "Reserved",
        "PCM",
        "DD",
        "MPEG1",
        "MP3",
        "MPEG2",
        "AAC",
        "DTS",
        "ATRAC",
        "ONEBIT",
        "DDP",
        "DTSHD",
        "TRUEHD",
        "DST",
        "WMAPRO",
    };

    if (enFmt < HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT)
    {
        return apcName[enFmt];
    }

    return "Unknown";
}

const HI_CHAR *AUTIL_PEQType2Name(HI_UNF_PEQ_FILTER_TYPE_E enFltType)
{
    const HI_CHAR *apcName[HI_UNF_PEQ_FILTER_TYPE_BUTT] =
    {
        "HP",
        "LS",
        "PK",
        "HS",
        "LP",
    };

    if (enFltType < HI_UNF_PEQ_FILTER_TYPE_BUTT)
    {
        return apcName[enFltType];
    }

    return "NO";
}

const HI_UNF_PEQ_FILTER_TYPE_E AUTIL_PEQTypeName2Type(HI_CHAR *pcName)
{
    if(!strncmp(pcName, "HP", 2))
        return HI_UNF_PEQ_FILTER_TYPE_HP;
    else if(!strncmp(pcName, "LS", 2))
        return HI_UNF_PEQ_FILTER_TYPE_LS;
    else if(!strncmp(pcName, "PK", 2))
        return HI_UNF_PEQ_FILTER_TYPE_PK;
    else if(!strncmp(pcName, "HS", 2))
        return HI_UNF_PEQ_FILTER_TYPE_HS;
    else if(!strncmp(pcName, "LP", 2))
        return HI_UNF_PEQ_FILTER_TYPE_LP;
    else
        return HI_UNF_PEQ_FILTER_TYPE_BUTT;
}

const HI_CHAR *AUTIL_DataType2Name(HI_U32 u32DataType)
{
    switch (u32DataType)
    {
        case HI_UNF_AI_HDMI_DATA_DD:
            return "DD";

        case HI_UNF_AI_HDMI_DATA_DDP:
            return "DDP";

        case HI_UNF_AI_HDMI_DATA_DTS:
            return "DTS";

        case HI_UNF_AI_HDMI_DATA_DTSHD:
            return "DTSHD";

        case HI_UNF_AI_HDMI_DATA_TRUEHD:
            return "TRUEHD";

        default:
            return "Unknown";
    }
}

const HI_CHAR *AUTIL_TrackSource2Name(HI_UNF_SND_SOURCE_E enSource)
{
    const HI_CHAR *apcName[HI_UNF_SND_SOURCE_BUTT] =
    {
        "ALSA",
        "ATV",
        "AV",
        "VGA",
        "YPBPR",
        "HDMI",
        "DTV",
        "MEDIA",
        "SCART",
    };

    if (enSource < HI_UNF_SND_SOURCE_BUTT)
    {
        return apcName[enSource];
    }

    return "not set";
}
#endif
#endif
HI_U32 AUTIL_BclkFclkDiv(HI_UNF_I2S_MCLK_SEL_E enMclkSel, HI_UNF_I2S_BCLK_SEL_E enBclkSel)
{
    HI_U32 u32Mclk_DIV;

    switch (enMclkSel)
    {
    case HI_UNF_I2S_MCLK_128_FS:
        u32Mclk_DIV = 128;
        break;
    case HI_UNF_I2S_MCLK_256_FS:
        u32Mclk_DIV = 256;
        break;
    case HI_UNF_I2S_MCLK_384_FS:
        u32Mclk_DIV = 384;
        break;
    case HI_UNF_I2S_MCLK_512_FS:
        u32Mclk_DIV = 512;
        break;
    case HI_UNF_I2S_MCLK_768_FS:
        u32Mclk_DIV = 768;
        break;
    case HI_UNF_I2S_MCLK_1024_FS:
        u32Mclk_DIV = 1024;
        break;
    default:
        return 0;
    }

    if (!(u32Mclk_DIV % enBclkSel))
    {
        return u32Mclk_DIV / enBclkSel;
    }

    return 0;
}

HI_U32 AUTIL_MclkFclkDiv(HI_UNF_I2S_MCLK_SEL_E enMclkSel)
{
    switch (enMclkSel)
    {
    case HI_UNF_I2S_MCLK_128_FS:
        return 128;
    case HI_UNF_I2S_MCLK_256_FS:
        return 256;
    case HI_UNF_I2S_MCLK_384_FS:
        return 384;
    case HI_UNF_I2S_MCLK_512_FS:
        return 512;
    case HI_UNF_I2S_MCLK_768_FS:
        return 768;
    case HI_UNF_I2S_MCLK_1024_FS:
        return 1024;
    default:
        return 0;
    }
}

HI_VOID AUTIL_OS_GetTime(HI_U32 *t_ms)
{
#ifdef __KERNEL__
    struct timeval tv;

    do_gettimeofday(&tv);

    *t_ms = (tv.tv_usec/1000) + tv.tv_sec * 1000;
#endif
    return;
}

AIAO_I2S_CHNUM_E AUTIL_CHNUM_UNF2AIAO(HI_UNF_I2S_CHNUM_E enChannel)
{
    switch (enChannel)
    {
        case HI_UNF_I2S_CHNUM_1:
            return AIAO_I2S_CHNUM_1;
        case HI_UNF_I2S_CHNUM_2:
            return AIAO_I2S_CHNUM_2;
        case HI_UNF_I2S_CHNUM_8:
            return AIAO_I2S_CHNUM_8;
        default:
            return AIAO_I2S_CHNUM_2;  //TODO
    }
}

AIAO_BITDEPTH_E AUTIL_BITDEPTH_UNF2AIAO(HI_UNF_I2S_BITDEPTH_E enBitDepth)
{
    switch (enBitDepth)
    {
        case HI_UNF_I2S_BIT_DEPTH_16:
            return AIAO_BIT_DEPTH_16;
        case HI_UNF_I2S_BIT_DEPTH_24:
            return AIAO_BIT_DEPTH_24;
        default:
            return AIAO_BIT_DEPTH_16;  //TODO
    }
}

AIAO_I2S_MODE_E AUTIL_I2S_MODE_UNF2AIAO(HI_UNF_I2S_MODE_E enI2sMode)
{
    switch (enI2sMode)
    {
        case HI_UNF_I2S_STD_MODE:
            return AIAO_MODE_I2S;
        case HI_UNF_I2S_PCM_MODE:
            return AIAO_MODE_PCM;
        default:
            return AIAO_MODE_BUTT;
    }
}

AUTIL_CHIP_PLATFORM_E  AUTIL_GetChipPlatform(HI_VOID)
{
    AUTIL_CHIP_PLATFORM_E enChipPlatform;

#if    defined (CHIP_TYPE_hi3751v100) \
    || defined (CHIP_TYPE_hi3751v200) \
    || defined(CHIP_TYPE_hi3796cv100_a) \
    || defined(CHIP_TYPE_hi3796cv100) \
    || defined(CHIP_TYPE_hi3798cv100_a) \
    || defined(CHIP_TYPE_hi3798cv100)
    enChipPlatform = AUTIL_CHIP_TYPE_PLATFORM_S5;
#elif    defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    enChipPlatform = AUTIL_CHIP_TYPE_PLATFORM_S28;
#else
    enChipPlatform = AUTIL_CHIP_TYPE_PLATFORM_S40;
#endif

   return enChipPlatform;
}

