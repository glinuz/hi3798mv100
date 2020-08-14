#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hi_type.h"
#include "subtitle_debug.h"
#include "scte_subt_data.h"
#include "scte_subt_parse.h"
#include "scte_subt_display.h"

#define SUBT_COLOR_RGB888_SIZE (32)
#define SCTE_SUBT_BPP 4
#define SCTE_SUBT_MAX_AREA  (0xFFE001) //4095 * 4095
#define PTS_UNITS 90   /*see Ref.P15*/
#define PTS_OFFSET (0x80000000/45) /*(0x100000000/PTS_UNITS)*//*see Ref.P15*/

#define CheckIndex(index) if(index >= 14) {\
        HI_ERR_SUBT("Index too large\n"); \
        return HI_FAILURE; \
    }

typedef struct tagSCTE_SUBT_PARSE_INFO_S
{
    HI_VOID*          hDisplay;
    HI_UNF_SUBT_GETPTS_FN pfnGetPts;
    HI_VOID* pUserData;
    SCTE_SUBT_OUTPUT_S stOutputData;
} SCTE_SUBT_PARSE_INFO_S;

static HI_U32 YUV2RGB(HI_U8*  pu8Data)
{
    HI_U8 Y   = 0, Cr = 0, Cb = 0, Alpha = 0;
    HI_U16 R, G, B, C, D, E;
    HI_U32 Clr = 0;

    Y  = (pu8Data[0] >> 3) & 0x1f;
    Cr = (pu8Data[0] << 3 | pu8Data[1] >> 5) & 0x1f;
    Cb = pu8Data[1] & 0x1f;
    Alpha = (pu8Data[0] >> 2) & 0x01;

    C = Y - 16;   //Y
    D = Cr - 128; //U
    E = Cb - 128; //V

    R = (((298 * C) + (409 * E) + 128) >> 8);
    G = (((298 * C) - (100 * D) - (208 * E) + 128) >> 8);
    B = (((298 * C) + (516 * D) + 128) >> 8);


    if (R > 255)
    {
        R = 255;
    }

    if (G > 255)
    {
        G = 255;
    }

    if (B > 255)
    {
        B = 255;
    }

    Clr = (HI_U32)((R << 16) | (G << 8) | B);

    if (Alpha == 1)
    {
        Clr |= 0xFF000000;   //opacity
    }
    else
    {
        Clr |= 0x7F000000;   //half transparent
    }

    return Clr;
}

static HI_VOID FillSubtClr(HI_U32 u32LineNumber, HI_U32 u32RowNumber, HI_U32 u32Width, HI_U8 u8PixOn, SCTE_SUBT_PARSE_INFO_S* pstParseInfo )
{
    HI_U32 i = 0;
    HI_U32 u32CLr = 0;

    u32CLr = pstParseInfo->stOutputData.u32SubtColor;

    for (i = SCTE_SUBT_BPP * (u32LineNumber * u32Width + u32RowNumber);
         i < SCTE_SUBT_BPP * (u32LineNumber * u32Width + u32RowNumber + u8PixOn); i++)
    {
        if (i >= pstParseInfo->stOutputData.u32BitmapDataLen)
        {
            HI_INFO_SUBT("Subtitle data too long...\n");
            break;
        }

        if (0 == i % SCTE_SUBT_BPP)
        {
            pstParseInfo->stOutputData.pu8SCTESubtData[i] = (HI_U8)((u32CLr >> 24) & 0xFF);
        }
        else if (1 == i % SCTE_SUBT_BPP)
        {
            pstParseInfo->stOutputData.pu8SCTESubtData[i] = (HI_U8)((u32CLr >> 16) & 0xFF);
        }
        else if (2 == i % SCTE_SUBT_BPP)
        {
            pstParseInfo->stOutputData.pu8SCTESubtData[i] = (HI_U8)((u32CLr >> 8) & 0xFF);
        }
        else
        {
            pstParseInfo->stOutputData.pu8SCTESubtData[i] = (HI_U8)(u32CLr & 0xFF);
        }
    }

    return;
}

static HI_BOOL GetNextUChar(HI_U8* pu8Data, HI_U16* u16CurData, HI_U32* u32Index, HI_U8* u8Curlen, HI_U32 u32DataLen)
{
    (*u32Index)++;

    if (*u32Index == u32DataLen)
    {
        return HI_TRUE;
    }

    (*u8Curlen)    += 8;
    (*u16CurData) <<= 8;
    (*u16CurData)  += pu8Data[*u32Index];
    return HI_FALSE;
}

static HI_S32 DecompressBitmap(SCTE_SUBT_PARSE_INFO_S* pstParseInfo, HI_U8* pu8DataSrc, HI_U32 u32DataLen)
{
    HI_S32 s32Ret   = HI_SUCCESS;
    HI_U8* pu8Data = pu8DataSrc;

    HI_U8 u8PixOn = 0;
    HI_U8 u8PixOff = 0;
    HI_U8 u8Curlen = 8;
    HI_U16 u16Temp = 0;
    HI_U16 u16Result = 0;
    HI_U16 u16CurData = 0;
    HI_U32 u32Index = 0;
    HI_U16 pow[] = {0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191};

    HI_U32 u32LineNumber = 0;
    HI_U32 u32RowNumber = 0;
    HI_U32 u32Width = 0;

    if ((HI_NULL == pstParseInfo) || (HI_NULL == pu8DataSrc) || (0 == u32DataLen))
    {
        HI_ERR_SUBT("param invalid...\n");
        return HI_FAILURE;
    }

    u32Width   = pstParseInfo->stOutputData.u32BottomXPos - pstParseInfo->stOutputData.u32TopXPos + 1;
    u16CurData = (HI_U8)(*pu8Data);

    if (SCTE_SUBT_BACKGROUD_FRAMED == pstParseInfo->stOutputData.enBackgroundStyle)
    {
        u32LineNumber = pstParseInfo->stOutputData.u32TopYPos - pstParseInfo->stOutputData.stFramed.u32TopYPos;
        u32RowNumber = pstParseInfo->stOutputData.u32TopXPos - pstParseInfo->stOutputData.stFramed.u32TopXPos;
        u32Width = pstParseInfo->stOutputData.stFramed.u32BottomXPos - pstParseInfo->stOutputData.stFramed.u32TopXPos;
    }

    pstParseInfo->stOutputData.u32BitWidth = SUBT_COLOR_RGB888_SIZE;

    while (u32Index < u32DataLen)
    {
        /*首先就是要可以判断出四种情况，根据一个token的前三位来进行分类，000/001/01X/1XX*/
        if (u8Curlen < 3)
        {
            if (HI_TRUE == GetNextUChar(pu8Data, &u16CurData, &u32Index, &u8Curlen, u32DataLen))
            {
                break;
            }
        }

        u16Temp   = 7;
        u16Temp <<= (u8Curlen - 3);
        u16Temp &= u16CurData;
        u16Temp >>= (u8Curlen - 3);

        switch (u16Temp)
        {
            case 0:         //      token size is 5
                if (u8Curlen < 5)
                {
                    if (HI_TRUE == GetNextUChar(pu8Data, &u16CurData, &u32Index, &u8Curlen, u32DataLen))
                    {
                        break;
                    }
                }

                u8Curlen -= 5;
                u16Result = u16CurData >> (u8Curlen);

                switch (u16Result)
                {
                    case 0:
                        break;

                    case 1:                //00001
                        u32LineNumber++;

                        if (u32RowNumber > u32Width)
                        {
                            HI_ERR_SUBT("Write subt data error!\n");
                        }

                        u32RowNumber = (pstParseInfo->stOutputData.enBackgroundStyle
                                        == SCTE_SUBT_BACKGROUD_FRAMED) ? (pstParseInfo->stOutputData.u32TopXPos
                                                - pstParseInfo->stOutputData.stFramed.u32TopXPos) : 0;
                        break;

                    default:
                        break;
                }

                CheckIndex(u8Curlen);
                u16CurData &= pow[u8Curlen];
                break;

            case 1:         //      token size is 7(001XXXX)
                if (u8Curlen < 7)
                {
                    if (HI_TRUE == GetNextUChar(pu8Data, &u16CurData, &u32Index, &u8Curlen, u32DataLen))
                    {
                        break;
                    }
                }

                u8Curlen -= 7;
                u16Result = u16CurData >> (u8Curlen);
                u8PixOn = u16Result & 15;

                if (0 == u8PixOn)
                {
                    u8PixOn = 16;
                }

                CheckIndex(u8Curlen);
                u16CurData &= pow[u8Curlen];
                FillSubtClr(u32LineNumber, u32RowNumber, u32Width, u8PixOn, pstParseInfo );
                u32RowNumber += (HI_U32) u8PixOn;
                break;

            case 2:
            case 3:         //      token size is 8(01XXXXXX)
                if (u8Curlen < 8)
                {
                    if (HI_TRUE == GetNextUChar(pu8Data, &u16CurData, &u32Index, &u8Curlen, u32DataLen))
                    {
                        break;
                    }
                }

                u8Curlen -= 8;
                u16Result = u16CurData >> (u8Curlen);
                u8PixOff = u16Result & 63;

                if (0 == u8PixOff)
                {
                    u8PixOff = 64;
                }

                CheckIndex(u8Curlen);
                u16CurData   &= pow[u8Curlen];
                u32RowNumber += (HI_U32) u8PixOff;
                break;

            default:        //      token size is 9(1XXXYYYYY )
                if (u8Curlen < 9)
                {
                    if (HI_TRUE == GetNextUChar(pu8Data, &u16CurData, &u32Index, &u8Curlen, u32DataLen))
                    {
                        break;
                    }
                }

                u8Curlen -= 9;
                u16Result = u16CurData >> (u8Curlen);
                u8PixOff = (HI_U8) (u16Result & 31);
                u8PixOn = (HI_U8)((u16Result & 224) >> 5);

                if (0 == u8PixOff)
                {
                    u8PixOff = 32;
                }

                if (0 == u8PixOn)
                {
                    u8PixOn = 8;
                }

                CheckIndex(u8Curlen);
                u16CurData &= pow[u8Curlen];
                FillSubtClr(u32LineNumber, u32RowNumber, u32Width, u8PixOn, pstParseInfo );
                u32RowNumber += (HI_U32) u8PixOn;
                u32RowNumber += (HI_U32) u8PixOff;
                break;
        }
    }

    return s32Ret;
}

static HI_S32 ParseBitmapHead(SCTE_SUBT_PARSE_INFO_S* pstParseInfo, HI_U8* pu8DataSrc, HI_U32 u32DataSize)
{
    HI_U32 i = 0;
    HI_S32 s32Ret  = HI_SUCCESS;
    HI_U8* pu8Data = pu8DataSrc;
    HI_U16 u16BitmapHeadLen = 0;
    HI_U16 u16BitmapLen   = 0;
    HI_U32 u32SubtDataLen = 0;
    HI_U32 u32SubtArea = 0;
    HI_U16 u16Width = 0;
    HI_U16 u16Height = 0;

    if ((HI_NULL == pstParseInfo) || (HI_NULL == pu8DataSrc) || (0 == u32DataSize))
    {
        HI_ERR_SUBT("param invalid...\n");
        return HI_FAILURE;
    }

    pstParseInfo->stOutputData.enBackgroundStyle = SCTE_SUBT_BACKGROUD_TRANSPARENT;
    pstParseInfo->stOutputData.enOutlineStyle = SCTE_SUBT_OUTLINE_RESERVED;

    if (pu8Data[0] & 0x04) //framed
    {
        pstParseInfo->stOutputData.enBackgroundStyle = SCTE_SUBT_BACKGROUD_FRAMED;
    }

    if (pu8Data[0] & 0x01) //outlined
    {
        pstParseInfo->stOutputData.enOutlineStyle = SCTE_SUBT_OUTLINE_OUTLINED;
    }
    else if (pu8Data[0] & 0x02) //dropshadow
    {
        pstParseInfo->stOutputData.enOutlineStyle = SCTE_SUBT_OUTLINE_DROPSHADOW;
    }

    pu8Data += 1;
    u16BitmapHeadLen += 1;
    pstParseInfo->stOutputData.u32SubtColor = YUV2RGB(pu8Data); //subtitle color

    pu8Data += 2;
    u16BitmapHeadLen += 2;
    pstParseInfo->stOutputData.u32TopXPos = ((pu8Data[0] << 8) | pu8Data[1]) >> 4; //BTH
    pstParseInfo->stOutputData.u32TopYPos = ((pu8Data[1] << 8) | pu8Data[2]) & 0x0fff; //BTV
    pstParseInfo->stOutputData.u32BottomXPos = ((pu8Data[3] << 8) | pu8Data[4]) >> 4; //BBH
    pstParseInfo->stOutputData.u32BottomYPos = ((pu8Data[4] << 8) | pu8Data[5]) & 0x0fff; //BBV

    pu8Data += 6;
    u16BitmapHeadLen += 6;

    /*before malloc pu8SCTESubtData,free it*/
    if (pstParseInfo->stOutputData.pu8SCTESubtData)
    {
        free((void*)pstParseInfo->stOutputData.pu8SCTESubtData);
        pstParseInfo->stOutputData.pu8SCTESubtData = NULL;
    }

    if (pstParseInfo->stOutputData.enBackgroundStyle == SCTE_SUBT_BACKGROUD_FRAMED)
    {
        pstParseInfo->stOutputData.stFramed.u32TopXPos = ((pu8Data[0] << 8) | pu8Data[1]) >> 4; //FTH
        pstParseInfo->stOutputData.stFramed.u32TopYPos = ((pu8Data[1] << 8) | pu8Data[2]) & 0x0fff; //FTV
        pstParseInfo->stOutputData.stFramed.u32BottomXPos = ((pu8Data[3] << 8) | pu8Data[4]) >> 4; //FBH
        pstParseInfo->stOutputData.stFramed.u32BottomYPos = ((pu8Data[4] << 8) | pu8Data[5]) & 0x0fff; //FBV
        pu8Data += 6;
        u16BitmapHeadLen += 6;
        pstParseInfo->stOutputData.stFramed.u32FrameColor = YUV2RGB(pu8Data);

        u16Width = (HI_U16)(pstParseInfo->stOutputData.stFramed.u32BottomXPos
                            - pstParseInfo->stOutputData.stFramed.u32TopXPos + 1);

        u16Height = (HI_U16)(pstParseInfo->stOutputData.stFramed.u32BottomYPos
                             - pstParseInfo->stOutputData.stFramed.u32TopYPos + 1);

        u32SubtArea = u16Width * u16Height;

        if (u32SubtArea > SCTE_SUBT_MAX_AREA)
        {
            HI_ERR_SUBT("area error, w:h = [%d, %d]...\n", u16Width, u16Height);
            return HI_FAILURE;
        }

        u32SubtDataLen = SCTE_SUBT_BPP * u32SubtArea;

        pstParseInfo->stOutputData.u32BitmapDataLen = u32SubtDataLen;
        pstParseInfo->stOutputData.pu8SCTESubtData = (HI_U8*)malloc(u32SubtDataLen);

        if (HI_NULL == pstParseInfo->stOutputData.pu8SCTESubtData)
        {
            HI_ERR_SUBT("malloc SCTESubtData datastructure error...\n");
            return HI_FAILURE;
        }

        for (i = 0; i < u32SubtDataLen; i++)
        {
            if (0 == i % SCTE_SUBT_BPP)
            {
                pstParseInfo->stOutputData.pu8SCTESubtData[i] =
                    (HI_U8)((pstParseInfo->stOutputData.stFramed.u32FrameColor >> 24) & 0xFF);
            }
            else if (1 == i % SCTE_SUBT_BPP)
            {
                pstParseInfo->stOutputData.pu8SCTESubtData[i] =
                    (HI_U8)((pstParseInfo->stOutputData.stFramed.u32FrameColor >> 16) & 0xFF);
            }
            else if (2 == i % SCTE_SUBT_BPP)
            {
                pstParseInfo->stOutputData.pu8SCTESubtData[i] =
                    (HI_U8)((pstParseInfo->stOutputData.stFramed.u32FrameColor >> 8) & 0xFF);
            }
            else
            {
                pstParseInfo->stOutputData.pu8SCTESubtData[i] =
                    (HI_U8)(pstParseInfo->stOutputData.stFramed.u32FrameColor & 0xFF);
            }
        }

        /*end*/
        pu8Data += 2;
        u16BitmapHeadLen += 2;
    }
    else
    {


        u16Width = (HI_U16)(pstParseInfo->stOutputData.u32BottomXPos
                            - pstParseInfo->stOutputData.u32TopXPos + 1);

        u16Height = (HI_U16)(pstParseInfo->stOutputData.u32BottomYPos
                             - pstParseInfo->stOutputData.u32TopYPos + 1);

        u32SubtArea = u16Width * u16Height;

        if (u32SubtArea > SCTE_SUBT_MAX_AREA)
        {
            HI_ERR_SUBT("area error, w:h = [%d, %d]...\n", u16Width, u16Height);
            return HI_FAILURE;
        }

        /*Allocate the memery of subtData*/
        u32SubtDataLen = SCTE_SUBT_BPP * u32SubtArea;
        pstParseInfo->stOutputData.pu8SCTESubtData = (HI_U8*)malloc(u32SubtDataLen);

        if (HI_NULL == pstParseInfo->stOutputData.pu8SCTESubtData)
        {
            HI_ERR_SUBT("malloc SCTESubtData datastructure error...\n");
            return HI_FAILURE;
        }

        pstParseInfo->stOutputData.u32BitmapDataLen = u32SubtDataLen;
        memset(pstParseInfo->stOutputData.pu8SCTESubtData, 0, u32SubtDataLen);
        /*end*/
    }

    if (pstParseInfo->stOutputData.enOutlineStyle == SCTE_SUBT_OUTLINE_OUTLINED)
    {
        pstParseInfo->stOutputData.unOutlineStyle.stOutline.u16OutlineThickness = pu8Data[0] & 0x0f;
        pu8Data += 1;
        u16BitmapHeadLen += 1;
        pstParseInfo->stOutputData.unOutlineStyle.stOutline.u32OutlineColor = YUV2RGB(pu8Data);

        pu8Data += 2;
        u16BitmapHeadLen += 2;
    }
    else if (pstParseInfo->stOutputData.enOutlineStyle == SCTE_SUBT_OUTLINE_DROPSHADOW)
    {
        pstParseInfo->stOutputData.unOutlineStyle.stDropshadow.u32ShadowRight  = (pu8Data[0] & 0xf0) >> 4;
        pstParseInfo->stOutputData.unOutlineStyle.stDropshadow.u32ShadowBottom = pu8Data[0] & 0x0f;
        pu8Data += 1;
        u16BitmapHeadLen += 1;
        pstParseInfo->stOutputData.unOutlineStyle.stDropshadow.u32ShadowColor = YUV2RGB( pu8Data);

        pu8Data += 2;
        u16BitmapHeadLen += 2;
    }
    else if (pstParseInfo->stOutputData.enOutlineStyle == SCTE_SUBT_OUTLINE_RESERVED)
    {
        pu8Data += 3;
        u16BitmapHeadLen += 3;
    }

    u16BitmapLen = pu8Data[0] << 8 | pu8Data[1];
    pu8Data += 2;
    u16BitmapHeadLen += 2;

    if ((pu8Data != (&pu8DataSrc[u16BitmapHeadLen])))
    {
        HI_ERR_SUBT("\nerror, pu8Data addr:%p, 0x%02x, &pDataSrc[u8PesHeadLength] addr:%p, 0x%02x\n",
                    pu8Data, pu8Data[0], &pu8DataSrc[u16BitmapHeadLen], pu8DataSrc[u16BitmapHeadLen]);

        return HI_FAILURE;
    }

    s32Ret = DecompressBitmap( pstParseInfo, &pu8DataSrc[u16BitmapHeadLen], (HI_U32)u16BitmapLen);

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_SUBT("failed to DecompressBitmap  ...\n");
        return HI_FAILURE;
    }

    HI_INFO_SUBT(" SCTE_SUBT_Parse_ParseSection success  ...\n");

    return s32Ret;
}

HI_S32 SCTE_SUBT_Parse_Init(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 SCTE_SUBT_Parse_DeInit(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 SCTE_SUBT_Parse_Create(HI_VOID* hDisplay, HI_VOID** pphParse)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SCTE_SUBT_PARSE_INFO_S* pstParseInfo = HI_NULL;

    pstParseInfo = (SCTE_SUBT_PARSE_INFO_S*)malloc(sizeof(SCTE_SUBT_PARSE_INFO_S));

    if (HI_NULL == pstParseInfo)
    {
        HI_ERR_SUBT("malloc parse datastructure error...\n");
        return HI_FAILURE;
    }

    memset(pstParseInfo, 0, sizeof(SCTE_SUBT_PARSE_INFO_S));
    pstParseInfo->hDisplay = hDisplay;
    *pphParse = pstParseInfo;

    HI_INFO_SUBT("SCTE_SUBT_Parse_Create success, with handle:0x%08x!\n", *pphParse);

    return s32Ret;
}

HI_S32 SCTE_SUBT_Parse_RegGetPtsCb(HI_VOID* hParse, HI_UNF_SUBT_GETPTS_FN pfnGetPts, HI_VOID* pUserData)
{
    SCTE_SUBT_PARSE_INFO_S* pstParseInfo = (SCTE_SUBT_PARSE_INFO_S*)hParse;

    if (HI_NULL == pstParseInfo)
    {
        HI_ERR_SUBT("param is HI_NULL...\n");
        return HI_FAILURE;
    }

    pstParseInfo->pfnGetPts = pfnGetPts;
    pstParseInfo->pUserData = pUserData;

    return HI_SUCCESS;

}

HI_S32 SCTE_SUBT_Parse_Destroy(HI_VOID* hParse)
{
    SCTE_SUBT_PARSE_INFO_S* pstParseInfo = (SCTE_SUBT_PARSE_INFO_S*)hParse;

    if (HI_NULL == pstParseInfo)
    {
        HI_ERR_SUBT("param is HI_NULL...\n");
        return HI_FAILURE;
    }

    HI_INFO_SUBT("begin to use handle:0x%08x!\n", pstParseInfo);

    if (pstParseInfo->stOutputData.pu8SCTESubtData)
    {
        free((void*)pstParseInfo->stOutputData.pu8SCTESubtData);
        pstParseInfo->stOutputData.pu8SCTESubtData = HI_NULL;
    }

    free((void*)pstParseInfo);
    pstParseInfo = HI_NULL;
    HI_INFO_SUBT("success!\n");

    return HI_SUCCESS;
}

HI_S32 SCTE_SUBT_Parse_ParseSection(HI_VOID* hParse, HI_U8* pu8DataSrc, HI_U32 u32DataLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8*  pu8Data = pu8DataSrc;
    HI_S64 s64CurPts = 0;
    HI_U32 u32PTSValue = 0;
    HI_U32 u32Duration = 0;
    HI_U8 u8ImmediateFlag = 0;
    HI_U8 u8PreClearFlag = 0;
    HI_U8 u8HeadLength = 0;
    HI_U8 u8Dis_standard = 0;
    HI_U8 u8FrameRate = 0;

    SCTE_SUBT_PARSE_INFO_S* pstParseInfo = (SCTE_SUBT_PARSE_INFO_S*)hParse;

    if (HI_NULL == pstParseInfo)
    {
        HI_ERR_SUBT("param is HI_NULL!!!\n");

        return HI_FAILURE;
    }

    /*skip section first seven bytes:table_id: 8bits
                            zero: 2bits
                            reserved: 2bits
                            section length: 12bits
                            zero: 1bit
                            segmention overlay includes: 1bit
                            protocal version: 6bis
                            ISO_639_language: 24bits
     *********************************************/

    //printf("section length is %#x\n",(pu8Data[1] <<8|pu8Data[2])  & 0x0fff);
    pu8Data += 7;
    u8HeadLength += 7;

    u8PreClearFlag  = pu8Data[0] & 0x80;
    u8ImmediateFlag = pu8Data[0] & 0x40;
    u8Dis_standard = pu8Data[0] & 0x1f;

    switch (u8Dis_standard)
    {
        case 0x00:
            u8FrameRate = 30;
            pstParseInfo->stOutputData.enDispStandard = STANDARD_720_480_30;
            break;

        case 0x01:
            u8FrameRate = 25;
            pstParseInfo->stOutputData.enDispStandard = STANDARD_720_576_25;
            break;

        case 0x02:
            u8FrameRate = 60;
            pstParseInfo->stOutputData.enDispStandard = STANDARD_1280_720_60;
            break;

        case 0x03:
            u8FrameRate = 60;
            pstParseInfo->stOutputData.enDispStandard = STANDARD_1920_1080_60;
            break;

        default:
            u8FrameRate = 25;
            pstParseInfo->stOutputData.enDispStandard = STANDARD_720_576_25;
            break;
    }

    pu8Data += 1;
    u8HeadLength += 1;
    u32PTSValue = (pu8Data[0] << 24) | (pu8Data[1] << 16) | (pu8Data[2] << 8) | pu8Data[3];
    u32PTSValue /= PTS_UNITS;/*in units of 90Khz*/

    if (pstParseInfo->pfnGetPts)
    {
        (HI_VOID)pstParseInfo->pfnGetPts(pstParseInfo->pUserData, &s64CurPts);

        if (u32PTSValue < (HI_U32)s64CurPts )
        {
            u32PTSValue += (HI_U32)PTS_OFFSET;
        }
    }

    /*operation*/
    if (u8PreClearFlag)
    {
        pstParseInfo->stOutputData.enDISP = SCTE_SUBT_DISP_PRECLEAR;
    }
    else
    {
        pstParseInfo->stOutputData.enDISP = SCTE_SUBT_DISP_NORMAL;
    }

    if (u8ImmediateFlag)
    {
        pstParseInfo->stOutputData.u32PTS = (HI_U32)s64CurPts;
    }
    else
    {
        pstParseInfo->stOutputData.u32PTS = u32PTSValue;
    }

    /*skip display_in_PTS:32bits*/
    pu8Data += 4;
    u8HeadLength += 4;

    u32Duration = (pu8Data[0] | pu8Data[1]) & 0x07ff;
    /*unit:ms*/
    u32Duration = (u32Duration * 1000) / u8FrameRate;
    pstParseInfo->stOutputData.u32Duration = u32Duration;

    pu8Data += 2;
    u8HeadLength += 2;

    pu8Data += 2;
    u8HeadLength += 2;

    if ((pu8Data != (&pu8DataSrc[u8HeadLength])))
    {
        HI_ERR_SUBT("\nerror, pu8Data addr:%p, 0x%02x, &pDataSrc[u8PesHeadLength] addr:%p, 0x%02x\n",
                    pu8Data, pu8Data[0], &pu8DataSrc[u8HeadLength], pu8DataSrc[u8HeadLength]);

        return HI_FAILURE;
    }

    s32Ret = ParseBitmapHead(pstParseInfo, &pu8DataSrc[u8HeadLength], u32DataLen - (HI_U32)u8HeadLength);

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_SUBT("failed in SCTE_SUBT_Parse_ParseBitmap !!!\n");

        return HI_FAILURE;
    }

    /*Display*/
    s32Ret = SCTE_SUBT_Display_DisplaySubt(pstParseInfo->hDisplay, &pstParseInfo->stOutputData);

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_SUBT("failed in SCTE_SUBT_Display_DisplaySubt !!!\n");

        return HI_FAILURE;
    }

    HI_INFO_SUBT(" SCTE_SUBT_Parse_ParseSection success  ...\n");

    return s32Ret;
}

