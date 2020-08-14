/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpBiphZzszZeBf+uonI22j8rF7xLojaUDI8o+vcH
5euTam4vFUpxyV17ua0zSxl7eDngLYZh03udo8NSSKusc2z4bBRBJ1eiPadHYqwyQb4ZwA7/
MPP3GTCCu0iVhB3EpBGm9g3dbwgFU1zAEWP3EwqAe81EzjV4M9gmsxfeSPRiVw==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libdtshd_wrap.h"


dtsInt32 HA_DTSDecParser_SAPI_GetSizeOf()
{
    return DTSDecParser_SAPI_GetSizeOf();
}


dtsInt32 HA_DTSDecParser_SAPI_Initialize(dtsParser theParser)
{
    return DTSDecParser_SAPI_Initialize(theParser);
}


dtsInt32 HA_DTSDecParser_SAPI_Create(dtsParser* pParser)
{
    return DTSDecParser_SAPI_Create(pParser);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_GetSizeOf()
{
    return DTSDecFramePlayer_SAPI_GetSizeOf();
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Initialize(dtsFramePlayer theFramePlayer)
{
    return DTSDecFramePlayer_SAPI_Initialize(theFramePlayer);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Create(dtsFramePlayer* pFramePlayer)
{
    return DTSDecFramePlayer_SAPI_Create(pFramePlayer);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Config_Init(dtsFramePlayer theFramePlayer)
{
    return DTSDecFramePlayer_SAPI_Config_Init(theFramePlayer);
}


dtsInt32 HA_DTSDecParser_SAPI_AllowUnalignedSyncWord(dtsParser theParser)
{
    return DTSDecParser_SAPI_AllowUnalignedSyncWord(theParser);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Config_Init_SpkrOut(dtsFramePlayer theFramePlayer,
        dtsUint32 nSpkrOutActMask)
{
    return DTSDecFramePlayer_SAPI_Config_Init_SpkrOut(theFramePlayer, nSpkrOutActMask);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Config_SetOutputBitwidth(dtsFramePlayer theFramePlayer,
        dtsUint16 outputBitWidth)
{
    return DTSDecFramePlayer_SAPI_Config_SetOutputBitwidth(theFramePlayer, outputBitWidth);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Config_Init_DecodeOnlyCore(dtsFramePlayer theFramePlayer)
{
    return DTSDecFramePlayer_SAPI_Config_Init_DecodeOnlyCore(theFramePlayer);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Config_Init_DRC_Percent(dtsFramePlayer theFramePlayer,
        dtsUint32 nDRCPercent)
{
    return DTSDecFramePlayer_SAPI_Config_Init_DRC_Percent(theFramePlayer, nDRCPercent);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Config_Init_EnableTransEncode(dtsFramePlayer theFramePlayer)
{
    return DTSDecFramePlayer_SAPI_Config_Init_EnableTransEncode(theFramePlayer);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Config_Init_EnableTransEncodeMode(dtsFramePlayer theFramePlayer,
        dtsUint32 unTransCodeMode)
{
    return DTSDecFramePlayer_SAPI_Config_Init_EnableTransEncodeMode(theFramePlayer, unTransCodeMode);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Config_Init_EnableSPDIFOutput(dtsFramePlayer theFramePlayer)
{
    return DTSDecFramePlayer_SAPI_Config_Init_EnableSPDIFOutput(theFramePlayer);
}


dtsInt32 HA_DTSDecParser_SAPI_CaptureFrame(dtsParser  theParser,
        dtsUint8*  pcBitstream,
        dtsUint32* pnOffsetInByte,
        dtsUint32* pnLengthInByte,
        dtsUint32* pnFlags,
        dtsUint32** ppnBitstreamFrame,
        dtsUint32* pnFrameSizeInByte
                                                 )
{
    return DTSDecParser_SAPI_CaptureFrame(theParser,
                                          pcBitstream,
                                          pnOffsetInByte,
                                          pnLengthInByte,
                                          pnFlags,
                                          ppnBitstreamFrame,
                                          pnFrameSizeInByte
                                         );
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_DecodeSubFrame(dtsFramePlayer theFramePlayer,
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
                                                        )
{
    return DTSDecFramePlayer_SAPI_DecodeSubFrame(theFramePlayer,
            pnPrimaryBitstream,
            nPrimarySizeInByte,
            pnPrimaryFlags,
#ifndef NOCODEFORSECONDARYAUDIO
            pnSecondaryBitstream,
            nSecondarySizeInByte,
            pnSecondaryFlags,
#endif /* #ifndef NOCODEFORSECONDARYAUDIO */
            ppPCMOutput,
#ifndef NOCODEFORDOWNMIX
            ppPCMOutput51,
            ppPCMOutput20,
#endif /* #ifndef NOCODEFORDOWNMIX */
#ifndef NOCODEFORSPDIFOUTPUT
            ppSPDIFOutput
#endif /* #ifndef NOCODEFORSPDIFOUTPUT */
                                                );
}


dtsInt32 HA_DTSDecParser_SAPI_GetBitstreamInfo(dtsParser          theParser,
        dtsBitstreamInfo** ppBitstreamInfo)
{
    return DTSDecParser_SAPI_GetBitstreamInfo(theParser, ppBitstreamInfo);
}


dtsInt32 HA_DTSDecParser_SAPI_Destroy(dtsParser theParser)
{
    return DTSDecParser_SAPI_Destroy(theParser);
}


dtsInt32 HA_DTSDecParser_SAPI_Deinitialize(dtsParser theParser)
{
    return DTSDecParser_SAPI_Deinitialize(theParser);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Destroy(dtsFramePlayer theFramePlayer)
{
    return DTSDecFramePlayer_SAPI_Destroy(theFramePlayer);
}


dtsInt32 HA_DTSDecFramePlayer_SAPI_Deinitialize(dtsFramePlayer theFramePlayer)
{
    return DTSDecFramePlayer_SAPI_Deinitialize(theFramePlayer);
}


DTSM6_DECODE_S dts_m6_decode_entry =
{
    .DM6ParserGetSize         = HA_DTSDecParser_SAPI_GetSizeOf,
    .DM6ParserInit            = HA_DTSDecParser_SAPI_Initialize,
    .DM6ParserCreate          = HA_DTSDecParser_SAPI_Create,
    .DM6FrmPlayerGetSize      = HA_DTSDecFramePlayer_SAPI_GetSizeOf,
    .DM6FrmPlayerInit         = HA_DTSDecFramePlayer_SAPI_Initialize,
    .DM6FrmPlayerCreate       = HA_DTSDecFramePlayer_SAPI_Create,
    .DM6FrmPlayerCfgInit      = HA_DTSDecFramePlayer_SAPI_Config_Init,
    .DM6ParserAllowUSyncWord  = HA_DTSDecParser_SAPI_AllowUnalignedSyncWord,
    .DM6FrmPlayerSpkInit      = HA_DTSDecFramePlayer_SAPI_Config_Init_SpkrOut,
    .DM6FrmPlayerBitwidthSet  = HA_DTSDecFramePlayer_SAPI_Config_SetOutputBitwidth,
    .DM6FrmPlayerCoreOnly     = HA_DTSDecFramePlayer_SAPI_Config_Init_DecodeOnlyCore,
    .DM6FrmPlayerDRCPercent   = HA_DTSDecFramePlayer_SAPI_Config_Init_DRC_Percent,
    .DM6TransEncodeEnable     = HA_DTSDecFramePlayer_SAPI_Config_Init_EnableTransEncode,
    .DM6TransEncodeModeEnable = HA_DTSDecFramePlayer_SAPI_Config_Init_EnableTransEncodeMode,
    .DM6FrmPlayerSPDIFEnable  = HA_DTSDecFramePlayer_SAPI_Config_Init_EnableSPDIFOutput,
    .DM6ParserCaptureFrm      = HA_DTSDecParser_SAPI_CaptureFrame,
    .DM6FrmPlayerDecSubFrm    = HA_DTSDecFramePlayer_SAPI_DecodeSubFrame,
    .DM6ParserGetInfo         = HA_DTSDecParser_SAPI_GetBitstreamInfo,
    .DM6ParserDestroy         = HA_DTSDecParser_SAPI_Destroy,
    .DM6ParserDeInit          = HA_DTSDecParser_SAPI_Deinitialize,
    .DM6FrmPlayerDestroy      = HA_DTSDecFramePlayer_SAPI_Destroy,
    .DM6FrmPlayerDeInit       = HA_DTSDecFramePlayer_SAPI_Deinitialize,
};


