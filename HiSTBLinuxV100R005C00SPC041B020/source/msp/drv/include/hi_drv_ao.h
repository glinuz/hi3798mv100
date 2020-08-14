/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_drv_ao.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : aiao
Function List :
History       :
* main\1    2012-09-22   z40717     init.
******************************************************************************/
#ifndef __HI_DRV_AO_H__
#define __HI_DRV_AO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_unf_sound.h"

#define  AO_MAX_VIRTUAL_TRACK_NUM  (6)
#define  AO_MAX_REAL_TRACK_NUM     (8)
#define  AO_MAX_TOTAL_TRACK_NUM    AO_MAX_REAL_TRACK_NUM

#define AO_MAX_CAST_NUM (4)

#define AO_MIN_LINEARVOLUME   (0)
#define AO_MAX_LINEARVOLUME   (100)
#define AO_MAX_ABSOLUTEVOLUME (0) /* max 0 dB*/
#define AO_MIN_ABSOLUTEVOLUME (-70) /* min -70 dB*/
#define AO_MAX_ABSOLUTEVOLUMEEXT (18) /* max 18 dB for S5*/
#define AO_MIN_ABSOLUTEVOLUMEEXT (-81) /* min -81 dB for S5*/
#define AO_MIN_DECIMALVOLUME_AMPLIFICATION (1000) /*0.125dB is 125*/
#define AO_MIN_BALANCE        (-50)
#define AO_MAX_BALANCE        (50)
#define AO_MAX_ADJUSTSPEED    (100)  //verify
#define AO_MAX_DELAYMS    (250)  //verify

#define AO_PCM_DF_UNSTALL_THD_FRAMENUM   (3)
#define AO_PCM_MAX_UNSTALL_THD_FRAMENUM  (10)

#define HI_ID_MASTER_SLAVE_TRACK (0x00)
#define HI_ID_LOWLATENCY_TRACK (0x01)
#define HI_ID_CAST            (0x01)
#define HI_ID_AEF             (0x02)
#define AO_TRACK_CHNID_MASK   (0xff)
#define AO_CAST_CHNID_MASK    (0xff)
#define AO_AEF_CHNID_MASK     (0xff)

#define AO_TRACK_AIP_START_LATENCYMS 50
#define PEQ_BANDNUM_MAX  (10)
#define PEQ_BANDNUM_MIN  (1)
#define PEQ_FREQ_HP_MIN (20)
#define PEQ_FREQ_HP_MAX (4000)
#define PEQ_FREQ_LP_MIN (50)
#define PEQ_FREQ_LP_MAX (22000)
#define PEQ_FREQ_LS_MIN (20)
#define PEQ_FREQ_LS_MAX (4000)
#define PEQ_FREQ_HS_MIN (4000)
#define PEQ_FREQ_HS_MAX (22000)
#define PEQ_FREQ_PK_MIN (20)
#define PEQ_FREQ_PK_MAX (22000)

#define PEQ_Q_LP_MIN   (7)
#define PEQ_Q_HP_MIN   (7)
#define PEQ_Q_LP_MAX   (7)
#define PEQ_Q_HP_MAX   (7)
#define PEQ_Q_LS_MIN   (7)
#define PEQ_Q_LS_MAX   (10)
#define PEQ_Q_HS_MIN   (7)
#define PEQ_Q_HS_MAX   (10)
#define PEQ_Q_PK_MIN   (5)
#define PEQ_Q_PK_MAX   (100)

#define PEQ_GAIN_MIN   (-15000)
#define PEQ_GAIN_MAX   (15000)

#define AO_CHECK_PEQQ(q,qmin,qmax) \
    do \
    { \
        if(q<qmin||q>qmax) \
        { \
            HI_ERR_AO("Invalid Q value %d\n", q); \
            return HI_ERR_AO_INVALID_PARA; \
        } \
    }while(0)

#define AO_CHECK_PEQFREQ(freq,freqmin,freqmax) \
    do \
    { \
        if(freq<freqmin||freq>freqmax) \
        { \
            HI_ERR_AO("Invalid Peq Freq value %d\n", freq); \
            return HI_ERR_AO_INVALID_PARA; \
        } \
    }while(0)

#define AO_CHECK_PEQGAIN(gain,gainmin,gainmax) \
    do \
    { \
        if(gain<gainmin||gain>gainmax||gain% 125) \
        { \
            HI_ERR_AO("Invalid Peq Gain value %d\n", gain); \
            return HI_ERR_AO_INVALID_PARA; \
        } \
    }while(0)

#define CHECK_AO_SNDCARD_OPEN(enSound) \
    do                                                         \
    {                                                          \
        CHECK_AO_SNDCARD(enSound);                             \
        if (HI_NULL == s_stAoDrv.astSndEntity[enSound].pCard)     \
        {                                                       \
            HI_WARN_AO(" Invalid snd id %d\n", enSound);        \
            return HI_ERR_AO_SOUND_NOT_OPEN;                       \
        }                                                       \
    } while (0)

#define CHECK_AO_TRACK_ID(Track)                          \
    do {                                                    \
            if((Track & 0xffff0000) != (HI_ID_AO << 16))              \
            {                                               \
                HI_ERR_AO("track(0x%x) is not ao handle!\n", Track);  \
                return HI_ERR_AO_INVALID_PARA;                          \
            }                                               \
            if((Track & 0xff00) != (HI_ID_MASTER_SLAVE_TRACK << 8))              \
            {                                                                \
                if((Track & 0xff00) != (HI_ID_LOWLATENCY_TRACK << 8))     \
            {                                               \
                HI_ERR_AO("track(0x%x) is not track handle!\n", Track);  \
                return HI_ERR_AO_INVALID_PARA;                          \
                }                                                       \
            }    \
         } while(0)

#define CHECK_AO_TRACK_OPEN(Track) \
    do                                                         \
    {                                                          \
        CHECK_AO_TRACK(Track);                             \
        if (0 == atomic_read(&s_stAoDrv.astTrackEntity[Track & AO_TRACK_CHNID_MASK].atmUseCnt))   \
        {                                                       \
            HI_WARN_AO(" Invalid track id 0x%x\n", Track);        \
            return HI_ERR_AO_INVALID_PARA;                       \
        }                                                       \
    } while (0)

#define CHECK_AO_AEF_HANDLE(hAef)                          \
    do {                                                    \
        if((hAef & 0xffff0000) != (HI_ID_AO << 16))              \
        {                                               \
            HI_ERR_AO("aef(0x%x) is not ao handle!\n", hAef);  \
            return HI_ERR_AO_INVALID_PARA;                          \
        }                                               \
        if((hAef & 0xff00) != (HI_ID_AEF << 8))              \
        {                                               \
            HI_ERR_AO("aef(0x%x) is not aef handle!\n", hAef);  \
            return HI_ERR_AO_INVALID_PARA;                          \
        }    \
    } while(0)

#define CHECK_AO_NULL_PTR(p)                                \
    do {                                                    \
        if(HI_NULL == p)                                \
        {                                               \
            HI_ERR_AO("NULL pointer \n");               \
            return HI_ERR_AO_NULL_PTR;                          \
        }                                               \
    } while(0)

#define CHECK_AO_CREATE(state)                              \
    do                                                      \
    {                                                       \
        if (0 > state)                                      \
        {                                                   \
            HI_WARN_AO("AO  device not open!\n");           \
            return HI_ERR_AO_DEV_NOT_OPEN;                \
        }                                                   \
    } while (0)

#define CHECK_AO_SNDCARD(card)                                  \
    do                                                          \
    {                                                           \
        if (HI_UNF_SND_BUTT <= card)                            \
        {                                                       \
            HI_WARN_AO(" Invalid snd id %d\n", card);           \
            return HI_ERR_AO_INVALID_ID;                       \
        }                                                       \
    } while (0)
/* master & slave only */
#define CHECK_AO_TRACK(track)                                  \
    do                                                          \
    {                                                           \
        if (AO_MAX_TOTAL_TRACK_NUM <= (track & AO_TRACK_CHNID_MASK))                            \
        {                                                       \
            HI_WARN_AO(" Invalid Snd Track 0x%x\n", track);           \
            return HI_ERR_AO_INVALID_PARA;                       \
        }                                                       \
    } while (0)

#define CHECK_AO_CAST(cast)                                  \
    do                                                          \
    {                                                           \
        if (AO_MAX_CAST_NUM <= (cast & AO_CAST_CHNID_MASK))                            \
        {                                                       \
            HI_WARN_AO(" Invalid Snd Cast 0x%x\n", cast);           \
            return HI_ERR_AO_INVALID_PARA;                       \
        }                                                       \
    } while (0)

#define CHECK_AO_PORTNUM(num)                                   \
    do                                                          \
    {                                                           \
        if (HI_UNF_SND_OUTPUTPORT_MAX < num)                    \
        {                                                       \
            HI_WARN_AO(" Invalid outport number %d\n", num);       \
            return HI_ERR_AO_INVALID_PARA;                     \
        }                                                       \
    } while (0)

#define CHECK_AO_OUTPORT(port)                                                              \
    do                                                                                      \
    {                                                                                       \
        if (((HI_UNF_SND_OUTPUTPORT_ARC0 < port) && (HI_UNF_SND_OUTPUTPORT_EXT_DAC1 > port)) \
            || ((HI_UNF_SND_OUTPUTPORT_EXT_DAC3 < port) && (HI_UNF_SND_OUTPUTPORT_ALL != port)))    \
        {                                                                                   \
            HI_WARN_AO(" Invalid outport %d\n", port);                                      \
            return HI_ERR_AO_INVALID_PARA;                                                 \
        }                                                                                   \
    } while (0)

#define CHECK_AO_PORTEXIST(num)                                   \
    do                                                          \
    {                                                           \
        if (0 >= num)                                           \
        {                                                       \
            HI_ERR_AO("Sound dont't attach any port!\n");       \
            return HI_FAILURE;                                  \
        }                                                       \
    } while (0)


#define CHECK_AO_TRACKMODE(mode)                                  \
    do                                                          \
    {                                                           \
        if (HI_UNF_TRACK_MODE_BUTT <= mode)                     \
        {                                                       \
            HI_WARN_AO(" Invalid trackmode %d\n", mode);        \
            return HI_ERR_AO_INVALID_PARA;                       \
        }                                                       \
    } while (0)

#define CHECK_AO_HDMIMODE(mode)                                  \
    do                                                          \
    {                                                           \
        if (HI_UNF_SND_HDMI_MODE_BUTT <= mode)                     \
        {                                                       \
            HI_WARN_AO(" Invalid hdmimode %d\n", mode);        \
            return HI_ERR_AO_INVALID_PARA;                       \
        }                                                       \
    } while (0)

#define CHECK_AO_SPDIFMODE(mode)                                  \
    do                                                          \
    {                                                           \
        if (HI_UNF_SND_SPDIF_MODE_BUTT <= mode)                     \
        {                                                       \
            HI_WARN_AO(" Invalid spdifmode %d\n", mode);        \
            return HI_ERR_AO_INVALID_PARA;                       \
        }                                                       \
    } while (0)
#ifdef __DPT__
#define CHECK_AO_ARCMODE(mode)                                  \
    do                                                          \
    {                                                           \
        if (HI_UNF_SND_ARC_AUDIO_MODE_BUTT <= mode)             \
        {                                                       \
            HI_WARN_AO(" Invalid arcmode %d\n", mode);        \
            return HI_ERR_AO_INVALID_PARA;                       \
        }                                                       \
    } while (0)
#endif
#define CHECK_AO_SPDIFSCMSMODE(scmsmode)                        \
    do                                                          \
    {                                                           \
        if (HI_UNF_SND_SPDIF_SCMSMODE_BUTT <= scmsmode)         \
        {                                                       \
            HI_WARN_AO(" Invalid spdifscmsmode %d\n", scmsmode);    \
            return HI_ERR_AO_INVALID_PARA;                      \
        }                                                       \
    } while (0)

#define CHECK_AO_CATEGORYCODE(categorycode)                           \
    do                                                                \
    {                                                                 \
        switch(categorycode)                                          \
        {                                                             \
            case HI_UNF_SND_SPDIF_CATEGORY_GENERAL:                       \
            case HI_UNF_SND_SPDIF_CATEGORY_BROADCAST_JP:                  \
            case HI_UNF_SND_SPDIF_CATEGORY_BROADCAST_USA:                 \
            case HI_UNF_SND_SPDIF_CATEGORY_BROADCAST_EU:                  \
            case HI_UNF_SND_SPDIF_CATEGORY_PCM_CODEC:                     \
            case HI_UNF_SND_SPDIF_CATEGORY_DIGITAL_SNDSAMPLER:            \
            case HI_UNF_SND_SPDIF_CATEGORY_DIGITAL_MIXER:                 \
            case HI_UNF_SND_SPDIF_CATEGORY_DIGITAL_SNDPROCESSOR:          \
            case HI_UNF_SND_SPDIF_CATEGORY_SRC:                           \
            case HI_UNF_SND_SPDIF_CATEGORY_MD:                            \
            case HI_UNF_SND_SPDIF_CATEGORY_DVD:                           \
            case HI_UNF_SND_SPDIF_CATEGORY_SYNTHESISER:                   \
            case HI_UNF_SND_SPDIF_CATEGORY_MIC:                           \
            case HI_UNF_SND_SPDIF_CATEGORY_DAT:                           \
            case HI_UNF_SND_SPDIF_CATEGORY_DCC:                           \
            case HI_UNF_SND_SPDIF_CATEGORY_VCR:                           \
                break;                                                    \
            default:                                                      \
                HI_WARN_AO("Invalid category code 0x%x\n", categorycode); \
                return HI_ERR_AO_INVALID_PARA;                            \
        }                                                             \
    } while (0)

#define CHECK_AO_FRAME_NOSTANDART_SAMPLERATE(inrate)                   \
    do                                                  \
    {                                                   \
        if(inrate > HI_UNF_SAMPLE_RATE_192K || inrate < HI_UNF_SAMPLE_RATE_8K)  \
        {                                                                       \
            HI_INFO_AO("don't support this insamplerate(%d)\n", inrate);        \
            return HI_SUCCESS;                                                  \
        }                                                                       \
    } while (0)

#define CHECK_AO_FRAME_SAMPLERATE(inrate)                   \
    do                                                  \
    {                                                   \
        switch (inrate)                                \
        {                                               \
            case  HI_UNF_SAMPLE_RATE_8K:                    \
            case  HI_UNF_SAMPLE_RATE_11K:                   \
            case  HI_UNF_SAMPLE_RATE_12K:                   \
            case  HI_UNF_SAMPLE_RATE_16K:                   \
            case  HI_UNF_SAMPLE_RATE_22K:                   \
            case  HI_UNF_SAMPLE_RATE_24K:                   \
            case  HI_UNF_SAMPLE_RATE_32K:                   \
            case  HI_UNF_SAMPLE_RATE_44K:                   \
            case  HI_UNF_SAMPLE_RATE_48K:                   \
            case  HI_UNF_SAMPLE_RATE_88K:                   \
            case  HI_UNF_SAMPLE_RATE_96K:                   \
            case  HI_UNF_SAMPLE_RATE_176K:                  \
            case  HI_UNF_SAMPLE_RATE_192K:                  \
                break;                                      \
            default:                                        \
                HI_INFO_AO("don't support this insamplerate(%d)\n", inrate);    \
                return HI_SUCCESS;                        \
        }                                                       \
    } while (0)

#define CHECK_AO_SAMPLERATE(outrate )                   \
    do                                                  \
    {                                                   \
        switch (outrate)                                \
        {                                               \
            case  HI_UNF_SAMPLE_RATE_8K:                    \
            case  HI_UNF_SAMPLE_RATE_11K:                   \
            case  HI_UNF_SAMPLE_RATE_12K:                   \
            case  HI_UNF_SAMPLE_RATE_16K:                   \
            case  HI_UNF_SAMPLE_RATE_22K:                   \
            case  HI_UNF_SAMPLE_RATE_24K:                   \
            case  HI_UNF_SAMPLE_RATE_32K:                   \
            case  HI_UNF_SAMPLE_RATE_44K:                   \
            case  HI_UNF_SAMPLE_RATE_48K:                   \
            case  HI_UNF_SAMPLE_RATE_88K:                   \
            case  HI_UNF_SAMPLE_RATE_96K:                   \
            case  HI_UNF_SAMPLE_RATE_176K:                  \
            case  HI_UNF_SAMPLE_RATE_192K:                  \
                break;                                      \
            default:                                        \
                HI_WARN_AO("invalid sample out rate %d\n", outrate);    \
                return HI_ERR_AO_INVALID_PARA;                        \
        }                                                       \
    } while (0)


#define CHECK_AO_FRAME_BITDEPTH(inbitdepth)                 \
    do                                              \
    {                                               \
        switch (inbitdepth)                         \
        {                                           \
            case  HI_UNF_BIT_DEPTH_24:                  \
            case  HI_UNF_BIT_DEPTH_16:                  \
            case  HI_UNF_BIT_DEPTH_8:                   \
                break;                                  \
            default:                                    \
                HI_INFO_AO("don't support this bit depth(%d)\n", inbitdepth);   \
                return HI_SUCCESS;                      \
        }                                           \
    } while (0)

#define CHECK_AO_LINEARVOLUME(linvolume)                \
    do                                                  \
    {                                                   \
        if ((linvolume < AO_MIN_LINEARVOLUME) || (linvolume > AO_MAX_LINEARVOLUME))                   \
        {                                               \
            HI_WARN_AO("invalid LinearVolume(%d), Min(%d) Max(%d)\n", linvolume, AO_MIN_LINEARVOLUME, AO_MAX_LINEARVOLUME);   \
            return HI_ERR_AO_INVALID_PARA;            \
        }                                               \
    } while (0)

#define CHECK_AO_ABSLUTEVOLUME(absvolume)               \
    do                                                  \
    {                                                   \
        if ((absvolume < AO_MIN_ABSOLUTEVOLUME) || (absvolume > AO_MAX_ABSOLUTEVOLUME))      \
        {                                               \
            HI_WARN_AO("invalid AbsouluteVolume(%d), min(%d), max(%d)\n", absvolume, AO_MIN_ABSOLUTEVOLUME, AO_MAX_ABSOLUTEVOLUME);   \
            return HI_ERR_AO_INVALID_PARA;            \
        }                                               \
    } while (0)

#define CHECK_AO_ABSLUTEPRECIVOLUME(absintvolume, absdecvolume)               \
    do                                                  \
    {                                                   \
        if((absintvolume > 0 && absdecvolume < 0) || (absintvolume < 0 && absdecvolume > 0))        \
        {                                               \
            HI_WARN_AO("invalid precision volume decimal\n");  \
            return HI_ERR_AO_INVALID_PARA;                                           \
        }                                           \
        if ((absdecvolume < -AO_MIN_DECIMALVOLUME_AMPLIFICATION) || (absdecvolume > AO_MIN_DECIMALVOLUME_AMPLIFICATION) || (0 != absdecvolume % 125))      \
        {                                               \
            HI_WARN_AO("invalid precision volume decimal part(%d), min(%d), max(%d), step(125)\n", absdecvolume, -AO_MIN_DECIMALVOLUME_AMPLIFICATION, AO_MIN_DECIMALVOLUME_AMPLIFICATION);   \
            return HI_ERR_AO_INVALID_PARA;            \
        }     \
        if ((absintvolume < AO_MIN_ABSOLUTEVOLUMEEXT) || (absintvolume > AO_MAX_ABSOLUTEVOLUMEEXT))      \
        {                                               \
            HI_WARN_AO("invalid precision volume, min(%d), max(%d)\n", AO_MIN_ABSOLUTEVOLUMEEXT, AO_MAX_ABSOLUTEVOLUMEEXT);   \
            return HI_ERR_AO_INVALID_PARA;            \
        }                                               \
        if(absdecvolume < 0)     \
        {          \
            if (absintvolume - 1 < AO_MIN_ABSOLUTEVOLUMEEXT)      \
            {                                               \
                HI_WARN_AO("invalid precision volume, min(%d)\n", AO_MIN_ABSOLUTEVOLUMEEXT);   \
                return HI_ERR_AO_INVALID_PARA;            \
            }         \
        }   \
        if(absdecvolume > 0)     \
        {          \
            if (absintvolume + 1 > AO_MAX_ABSOLUTEVOLUMEEXT)      \
            {                                               \
                HI_WARN_AO("invalid precision volume, max(%d)\n", AO_MAX_ABSOLUTEVOLUMEEXT);   \
                return HI_ERR_AO_INVALID_PARA;            \
            }         \
        }   \
    } while (0)

#define CHECK_AO_ABSLUTEVOLUMEEXT(absvolume)               \
    do                                                  \
    {                                                   \
        if ((absvolume < AO_MIN_ABSOLUTEVOLUMEEXT) || (absvolume > AO_MAX_ABSOLUTEVOLUMEEXT))      \
        {                                               \
            HI_WARN_AO("invalid AbsouluteVolume(%d), min(%d), max(%d)\n", absvolume, AO_MIN_ABSOLUTEVOLUMEEXT, AO_MAX_ABSOLUTEVOLUMEEXT);   \
            return HI_ERR_AO_INVALID_PARA;            \
        }                                               \
    } while (0)

#define CHECK_AO_BALANCE(balance)               \
    do                                                  \
    {                                                   \
        if ((balance < AO_MIN_BALANCE) || (balance > AO_MAX_BALANCE))      \
        {                                               \
            HI_WARN_AO("Invalid Balance(%d), min(%d), max(%d)\n", balance, AO_MIN_BALANCE, AO_MAX_BALANCE);   \
            return HI_ERR_AO_INVALID_PARA;            \
        }                                               \
    } while (0)

#define   CHECK_AO_SPEEDADJUST(speed)                   \
    do                                                  \
    {                                                   \
        if ((-AO_MAX_ADJUSTSPEED > speed)               \
            || (speed > AO_MAX_ADJUSTSPEED))            \
        {                                               \
            HI_WARN_AO("invalid AO SpeedAdjust(%d) min(%d), max(%d)!\n", speed, -AO_MAX_ADJUSTSPEED, AO_MAX_ADJUSTSPEED); \
            return HI_ERR_AO_INVALID_PARA;            \
        }                                               \
    } while (0)

#define   CHECK_AO_DRCATTR(pstDrcAttr)\
    do\
    {\
        if((pstDrcAttr->u32AttackTime < 20) || (pstDrcAttr->u32AttackTime > 2000))\
        {\
            HI_ERR_AO("Invalid Drc AttackTime! AttackTime(%d)!\n",pstDrcAttr->u32AttackTime);\
            return HI_ERR_AO_INVALID_PARA;\
        }    \
        if((pstDrcAttr->u32ReleaseTime < 20) || (pstDrcAttr->u32ReleaseTime > 2000))\
        {\
            HI_ERR_AO("Invalid Drc ReleaseTime! ReleaseTime(%d)!\n",pstDrcAttr->u32ReleaseTime);\
            return HI_ERR_AO_INVALID_PARA;\
        }\
        if(pstDrcAttr->s32Thrhd >= pstDrcAttr->s32Limit) \
        {\
            HI_ERR_AO("s32Thrhd must less than s32Limit! Thrhd(%d),Limit(%d)!\n",pstDrcAttr->s32Thrhd,pstDrcAttr->s32Limit);\
            return HI_ERR_AO_INVALID_PARA;\
        }    \
        if((pstDrcAttr->s32Thrhd < -80) || (pstDrcAttr->s32Thrhd > -2))\
        {\
            HI_ERR_AO("Invalid Drc Thrhd! Thrhd(%d)!\n",pstDrcAttr->s32Thrhd);\
            return HI_ERR_AO_INVALID_PARA;\
        }    \
        if((pstDrcAttr->s32Limit < -79) || (pstDrcAttr->s32Limit > -1))\
        {\
            HI_ERR_AO("Invalid Drc Limit! Limit(%d)!\n",pstDrcAttr->s32Limit);\
            return HI_ERR_AO_INVALID_PARA;\
        }    \
    } while (0)

#define HI_FATAL_AO(fmt...) \
    HI_FATAL_PRINT(HI_ID_AO, fmt)

#define HI_ERR_AO(fmt...) \
    HI_ERR_PRINT(HI_ID_AO, fmt)

#define HI_WARN_AO(fmt...) \
    HI_WARN_PRINT(HI_ID_AO, fmt)

#define HI_INFO_AO(fmt...) \
    HI_INFO_PRINT(HI_ID_AO, fmt)

#define HI_FATAL_AIAO(fmt...) HI_FATAL_PRINT  (HI_ID_AIAO, fmt)
#define HI_ERR_AIAO(fmt...)   HI_ERR_PRINT    (HI_ID_AIAO, fmt)
#define HI_WARN_AIAO(fmt...)  HI_WARN_PRINT   (HI_ID_AIAO, fmt)
#define HI_INFO_AIAO(fmt...)  HI_INFO_PRINT   (HI_ID_AIAO, fmt)

/* the type of Adjust Audio */
typedef enum
{
    AO_SND_SPEEDADJUST_SRC,     /**<samplerate convert */
    AO_SND_SPEEDADJUST_PITCH,   /**<Sola speedadjust, reversed */
    AO_SND_SPEEDADJUST_MUTE,    /**<mute */
    AO_SND_SPEEDADJUST_BUTT
} AO_SND_SPEEDADJUST_TYPE_E;

/* the type of Adjust Audio */
typedef struct
{
    HI_U32                  u32AefId;
    HI_UNF_SND_AEF_TYPE_E   enAefType;
    HI_CHAR                 szName[32];
    HI_CHAR                 szDescription[32];
    HI_BOOL                 bEnable;
} AO_AEF_PROC_ITEM_S;

/* the type of Debug Audio */
typedef enum
{
    AO_SND_DEBUG_TYPE_AEF,     /**<samplerate convert */
    AO_SND_DEBUG_TYPE_BUTT
} AO_SND_DEBUG_TYPE_E;

typedef struct
{
    HI_PHYS_ADDR_T tPhySettingAddr;
    HI_U32         u32SettingSize;
} AEF_DEBUG_ATTR_S;

typedef struct
{
    AO_SND_DEBUG_TYPE_E enDebugType;

    union
    {
        AEF_DEBUG_ATTR_S stAefDebugAttr;
    } unDebugAttr;

} AO_DEBUG_ATTR_S;

typedef struct
{
    HI_PHYS_ADDR_T tPhyInBufAddr;
    HI_PHYS_ADDR_T tPhyOutBufAddr;
    HI_U32         u32InBufSize;
    HI_U32         u32OutBufSize;
    HI_U32         u32InBufFrameSize;
    HI_U32         u32OutBufFrameSize;
    HI_U32         u32InBufFrameNum;          /* 2/4/8 */
    HI_U32         u32OutBufFrameNum;         /* 2/4/8 */
    HI_PHYS_ADDR_T tPhyEngineAddr;
    AEF_DEBUG_ATTR_S stAefDebugAddr;
} AO_AEF_BUF_ATTR_S;

typedef struct
{
    HI_U32         u32FrameDelay;       /*input:  The Frame Delay of Aef */
    HI_BOOL        bMcSupported;        /*input:  Indicate whether support mc process of Aef */
} AO_AEF_ATTR_S;

typedef struct
{
    HI_U32  u32BufPhyAddr;
    HI_VIRT_ADDR_T  tBufVirAddr;
    HI_U32  u32BufSize;
} AO_BUF_ATTR_S;

typedef struct
{
    /** s32BitPerSample: (PCM) Data depth, and format of storing the output data
          If the data depth is 16 bits, 16-bit word memory is used.
          If the data depth is greater than 16 bits, 32-bit word memory is used, and data is stored as left-aligned data. That is, the valid data is at upper bits.
     */
    /**CNcomment: s32BitPerSample: (PCM) 数据位宽设置. 输出存放格式
          等于16bit:   占用16bit word内存
          大于16bit:   占用32bit word内存, 数据左对齐方式存放(有效数据在高位)
     */
    HI_S32  s32BitPerSample;       /**<Data depth*/ /**<CNcomment: 数据位宽*/
    HI_BOOL bInterleaved;          /**<Whether the data is interleaved*/ /**<CNcomment: 数据是否交织*/
    HI_U32  u32SampleRate;         /**<Sampling rate*/ /**<CNcomment: 采样率*/
    HI_U32  u32Channels;           /**<Number of channels*/ /**<CNcomment: 通道数量*/
    HI_U32  u32PtsMs;              /**<Presentation time stamp (PTS)*/ /**<CNcomment: 时间戳*/
    HI_VIRT_ADDR_T  tPcmBuffer;         /**<Pointer to the buffer for storing the pulse code modulation (PCM) data*/ /**<CNcomment: PCM数据缓冲指针*/
    HI_VIRT_ADDR_T  tBitsBuffer;        /**<Pointer to the buffer for storing the stream data*/ /**<CNcomment: 码流数据缓冲指针*/
    HI_U32  u32PcmSamplesPerFrame; /**<Number of sampling points of the PCM data*/ /**<CNcomment: PCM数据采样点数量*/
    HI_U32  u32BitsBytesPerFrame;  /**<IEC61937 data size*/ /**<CNcomment: IEC61937数据长度*/
    HI_U32  u32FrameIndex;         /**<Frame ID*/ /**<CNcomment: 帧序号 */
    HI_U32  u32IECDataType;        /**<IEC61937 Data Type*/ /**<CNcomment: IEC61937数据类型标识，低8bit为IEC数据类型 */
} AO_FRAMEINFO_S;

/**Defines  Audio outport attribute */
/**CNcomment:定义音频输出端口属性*/
typedef struct
{
    HI_UNF_SND_OUTPUTPORT_E enOutPort;
    union
    {
        HI_U32                u32DacAttr;
        HI_UNF_SND_I2S_ATTR_S stI2sAttr;
        HI_U32                u32SpdifAttr;
        HI_U32                u32HDMIAttr;
        HI_U32                u32ArcAttr;
    } unAttr;
} DRV_SND_OUTPORT_S;

/**Defines  Audio Sound device attribute */
/**CNcomment:定义Sound设备属性*/
typedef struct
{
    HI_U32                  u32PortNum;  /**<Outport number attached sound*/ /**<CNcomment:绑定到Sound设备的输出端口数*/
    DRV_SND_OUTPORT_S       stOutport[HI_UNF_SND_OUTPUTPORT_MAX];  /**<Outports attached sound*/ /**<CNcomment:绑定到Sound设备的输出端口*/
    HI_UNF_SAMPLE_RATE_E    enSampleRate;       /**<Sound samplerate*/ /**<CNcomment:Sound设备输出采样率*/
    HI_U32                  u32MasterOutputBufSize; /**<Sound master channel buffer size*/ /**<CNcomment:Sound设备主输出通道缓存大小*/
    HI_U32                  u32SlaveOutputBufSize;  /**<Sound slave channel buffer size*/ /**<CNcomment:Sound设备从输出通道缓存大小*/
} DRV_SND_ATTR_S;

typedef struct
{
    HI_UNF_SND_OUTPUTPORT_E enOutPort;
    HI_PHYS_ADDR_T tPhyDma;
    HI_PHYS_ADDR_T tPhyWptr;
    HI_PHYS_ADDR_T tPhyRptr;
    HI_U32 u32Size;
} SND_PORT_KERNEL_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
