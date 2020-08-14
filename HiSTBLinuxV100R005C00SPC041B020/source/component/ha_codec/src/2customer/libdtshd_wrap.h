
/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : libdtshd_wrap.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2014/01/14
Last Modified :
Description   : define dtsm6 interface structure
  History       :
  1.Date        : 2014/01/14
    Author      :
    Modification: Created file
******************************************************************************/

/**
 * \file
 * \brief 提供DTSM6软解码的相关接口信息
 */


#ifndef __LIBDTSHD_WRAP_H__
#define __LIBDTSHD_WRAP_H__

#include "dtshd_frame_player_api.h"
#include "dtshd_parser_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



typedef struct hiDTSM6_DECODE_S
{
    dtsInt32 (*DM6ParserGetSize)(void);

    dtsInt32 (*DM6ParserInit)(dtsParser theParser);

    dtsInt32 (*DM6ParserCreate)(dtsParser* pParser);

    dtsInt32 (*DM6FrmPlayerGetSize)(void);

    dtsInt32 (*DM6FrmPlayerInit)(dtsFramePlayer theFramePlayer);

    dtsInt32 (*DM6FrmPlayerCreate)(dtsFramePlayer* pFramePlayer);

    dtsInt32 (*DM6FrmPlayerCfgInit)(dtsFramePlayer theFramePlayer);

    dtsInt32 (*DM6ParserAllowUSyncWord)(dtsParser theParser);

    dtsInt32 (*DM6FrmPlayerSpkInit)(dtsFramePlayer theFramePlayer,
                                    dtsUint32 nSpkrOutActMask);

    dtsInt32 (*DM6FrmPlayerBitwidthSet)(dtsFramePlayer theFramePlayer,
                                        dtsUint16 outputBitWidth);

    dtsInt32 (*DM6FrmPlayerCoreOnly)(dtsFramePlayer theFramePlayer);

    dtsInt32 (*DM6FrmPlayerDRCPercent)(dtsFramePlayer theFramePlayer,
                                       dtsUint32 nDRCPercent);

    dtsInt32 (*DM6TransEncodeEnable)(dtsFramePlayer theFramePlayer);

    dtsInt32 (*DM6TransEncodeModeEnable)(dtsFramePlayer theFramePlayer,
                                         dtsUint32 unTransCodeMode);

    dtsInt32 (*DM6FrmPlayerSPDIFEnable)(dtsFramePlayer theFramePlayer);

    dtsInt32 (*DM6ParserCaptureFrm)(dtsParser  theParser,
                                    dtsUint8*  pcBitstream,
                                    dtsUint32* pnOffsetInByte,
                                    dtsUint32* pnLengthInByte,
                                    dtsUint32* pnFlags,
                                    dtsUint32** ppnBitstreamFrame,
                                    dtsUint32* pnFrameSizeInByte
                                   );

    dtsInt32 (*DM6FrmPlayerDecSubFrm)(dtsFramePlayer theFramePlayer,
                                      dtsUint32* pnPrimaryBitstream,
                                      dtsUint32  nPrimarySizeInByte,
                                      dtsUint32* pnPrimaryFlags,
#ifndef NOCODEFORSECONDARYAUDIO
                                      dtsUint32* pnSecondaryBitstream,
                                      dtsUint32  nSecondarySizeInByte,
                                      dtsUint32* pnSecondaryFlags,
#endif /* #ifndef NOCODEFORSECONDARYAUDIO */
                                      dtsDecoderPCMOutput** ppPCMOutput,
#ifndef NOCODEFORDOWNMIX
                                      dtsDecoderPCMOutput** ppPCMOutput51,
                                      dtsDecoderPCMOutput** ppPCMOutput20,
#endif /* #ifndef NOCODEFORDOWNMIX */
#ifndef NOCODEFORSPDIFOUTPUT
                                      dtsDecoderPCMOutput** ppSPDIFOutput
#endif /* #ifndef NOCODEFORSPDIFOUTPUT */
                                     );

    dtsInt32 (*DM6ParserGetInfo)(dtsParser theParser,
                                 dtsBitstreamInfo** ppBitstreamInfo);

    dtsInt32 (*DM6ParserDestroy)(dtsParser theParser);

    dtsInt32 (*DM6ParserDeInit)(dtsParser theParser);

    dtsInt32 (*DM6FrmPlayerDestroy)(dtsFramePlayer theFramePlayer);

    dtsInt32 (*DM6FrmPlayerDeInit)(dtsFramePlayer theFramePlayer);

} DTSM6_DECODE_S;



/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* #ifndef __LIBDTSHD_WRAP_H__ */











