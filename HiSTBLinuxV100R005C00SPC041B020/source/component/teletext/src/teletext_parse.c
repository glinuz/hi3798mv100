#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


#include "teletext_debug.h"
#include "teletext_data.h"
#include "teletext_parse.h"
#include "teletext_utils.h"

#define TTX_PARSE_LOCK()  \
    do{\
        int ret = pthread_mutex_lock(&pstCurrentPoint->Mutexlock);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_lock(PARSE) failure ret = 0x%x\n",ret);\
        }\
    }while(0)

#define TTX_PARSE_UNLOCK()  \
    do{\
        int ret = pthread_mutex_unlock(&pstCurrentPoint->Mutexlock);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_unlock(PARSE) failure ret = 0x%x\n",ret);\
        }\
    }while(0)


typedef struct tagTTX_PARSE_DRCS_MODE_S
{
    TTX_DRCS_MODE_E enDRCSMode;
    HI_U32* pu32DRCSColorInfo;
    HI_U8 u8PTUNum;
    HI_U32 u32PTUReadLen;
}TTX_PARSE_DRCS_MODE_S;



typedef struct tagTTX_PARSE_DECMOT_INFO_S
{
    HI_U8 u8Object;
    HI_U8 u8DRCS;
    HI_BOOL bHasP27;
    HI_U8* au8Data;
    HI_U8* pu8Line;
    TTX_PAGE_S* pstMOPPage;
}TTX_PARSE_DECMOT_INFO_S;


static HI_U8 s_u8szOddParity[256] = {
    0xFF, 0x01, 0x02, 0xFF, 0x04, 0xFF, 0xFF, 0x07,
    0x08, 0xFF, 0xFF, 0x0B, 0xFF, 0x0D, 0x0E, 0xFF,
    0x10, 0xFF, 0xFF, 0x13, 0xFF, 0x15, 0x16, 0xFF,
    0xFF, 0x19, 0x1A, 0xFF, 0x1C, 0xFF, 0xFF, 0x1F,
    0x20, 0xFF, 0xFF, 0x23, 0xFF, 0x25, 0x26, 0xFF,
    0xFF, 0x29, 0x2A, 0xFF, 0x2C, 0xFF, 0xFF, 0x2F,
    0xFF, 0x31, 0x32, 0xFF, 0x34, 0xFF, 0xFF, 0x37,
    0x38, 0xFF, 0xFF, 0x3B, 0xFF, 0x3D, 0x3E, 0xFF,
    0x40, 0xFF, 0xFF, 0x43, 0xFF, 0x45, 0x46, 0xFF,
    0xFF, 0x49, 0x4A, 0xFF, 0x4C, 0xFF, 0xFF, 0x4F,
    0xFF, 0x51, 0x52, 0xFF, 0x54, 0xFF, 0xFF, 0x57,
    0x58, 0xFF, 0xFF, 0x5B, 0xFF, 0x5D, 0x5E, 0xFF,
    0xFF, 0x61, 0x62, 0xFF, 0x64, 0xFF, 0xFF, 0x67,
    0x68, 0xFF, 0xFF, 0x6B, 0xFF, 0x6D, 0x6E, 0xFF,
    0x70, 0xFF, 0xFF, 0x73, 0xFF, 0x75, 0x76, 0xFF,
    0xFF, 0x79, 0x7A, 0xFF, 0x7C, 0xFF, 0xFF, 0x7F,
    0x00, 0xFF, 0xFF, 0x03, 0xFF, 0x05, 0x06, 0xFF,
    0xFF, 0x09, 0x0A, 0xFF, 0x0C, 0xFF, 0xFF, 0x0F,
    0xFF, 0x11, 0x12, 0xFF, 0x14, 0xFF, 0xFF, 0x17,
    0x18, 0xFF, 0xFF, 0x1B, 0xFF, 0x1D, 0x1E, 0xFF,
    0xFF, 0x21, 0x22, 0xFF, 0x24, 0xFF, 0xFF, 0x27,
    0x28, 0xFF, 0xFF, 0x2B, 0xFF, 0x2D, 0x2E, 0xFF,
    0x30, 0xFF, 0xFF, 0x33, 0xFF, 0x35, 0x36, 0xFF,
    0xFF, 0x39, 0x3A, 0xFF, 0x3C, 0xFF, 0xFF, 0x3F,
    0xFF, 0x41, 0x42, 0xFF, 0x44, 0xFF, 0xFF, 0x47,
    0x48, 0xFF, 0xFF, 0x4B, 0xFF, 0x4D, 0x4E, 0xFF,
    0x50, 0xFF, 0xFF, 0x53, 0xFF, 0x55, 0x56, 0xFF,
    0xFF, 0x59, 0x5A, 0xFF, 0x5C, 0xFF, 0xFF, 0x5F,
    0x60, 0xFF, 0xFF, 0x63, 0xFF, 0x65, 0x66, 0xFF,
    0xFF, 0x69, 0x6A, 0xFF, 0x6C, 0xFF, 0xFF, 0x6F,
    0xFF, 0x71, 0x72, 0xFF, 0x74, 0xFF, 0xFF, 0x77,
    0x78, 0xFF, 0xFF, 0x7B, 0xFF, 0x7D, 0x7E, 0xFF
};

static HI_U8 s_u8aUnhamtab[256] = {
    0x01, 0xff, 0x81, 0x01, 0xff, 0x00, 0x01, 0xff,
    0xff, 0x02, 0x01, 0xff, 0x0a, 0xff, 0xff, 0x07,
    0xff, 0x00, 0x01, 0xff, 0x00, 0x80, 0xff, 0x00,
    0x06, 0xff, 0xff, 0x0b, 0xff, 0x00, 0x03, 0xff,
    0xff, 0x0c, 0x01, 0xff, 0x04, 0xff, 0xff, 0x07,
    0x06, 0xff, 0xff, 0x07, 0xff, 0x07, 0x07, 0x87,
    0x06, 0xff, 0xff, 0x05, 0xff, 0x00, 0x0d, 0xff,
    0x86, 0x06, 0x06, 0xff, 0x06, 0xff, 0xff, 0x07,
    0xff, 0x02, 0x01, 0xff, 0x04, 0xff, 0xff, 0x09,
    0x02, 0x82, 0xff, 0x02, 0xff, 0x02, 0x03, 0xff,
    0x08, 0xff, 0xff, 0x05, 0xff, 0x00, 0x03, 0xff,
    0xff, 0x02, 0x03, 0xff, 0x03, 0xff, 0x83, 0x03,
    0x04, 0xff, 0xff, 0x05, 0x84, 0x04, 0x04, 0xff,
    0xff, 0x02, 0x0f, 0xff, 0x04, 0xff, 0xff, 0x07,
    0xff, 0x05, 0x05, 0x85, 0x04, 0xff, 0xff, 0x05,
    0x06, 0xff, 0xff, 0x05, 0xff, 0x0e, 0x03, 0xff,
    0xff, 0x0c, 0x01, 0xff, 0x0a, 0xff, 0xff, 0x09,
    0x0a, 0xff, 0xff, 0x0b, 0x8a, 0x0a, 0x0a, 0xff,
    0x08, 0xff, 0xff, 0x0b, 0xff, 0x00, 0x0d, 0xff,
    0xff, 0x0b, 0x0b, 0x8b, 0x0a, 0xff, 0xff, 0x0b,
    0x0c, 0x8c, 0xff, 0x0c, 0xff, 0x0c, 0x0d, 0xff,
    0xff, 0x0c, 0x0f, 0xff, 0x0a, 0xff, 0xff, 0x07,
    0xff, 0x0c, 0x0d, 0xff, 0x0d, 0xff, 0x8d, 0x0d,
    0x06, 0xff, 0xff, 0x0b, 0xff, 0x0e, 0x0d, 0xff,
    0x08, 0xff, 0xff, 0x09, 0xff, 0x09, 0x09, 0x89,
    0xff, 0x02, 0x0f, 0xff, 0x0a, 0xff, 0xff, 0x09,
    0x88, 0x08, 0x08, 0xff, 0x08, 0xff, 0xff, 0x09,
    0x08, 0xff, 0xff, 0x0b, 0xff, 0x0e, 0x03, 0xff,
    0xff, 0x0c, 0x0f, 0xff, 0x04, 0xff, 0xff, 0x09,
    0x0f, 0xff, 0x8f, 0x0f, 0xff, 0x0e, 0x0f, 0xff,
    0x08, 0xff, 0xff, 0x05, 0xff, 0x0e, 0x0d, 0xff,
    0xff, 0x0e, 0x0f, 0xff, 0x0e, 0x8e, 0xff, 0x0e,
};

HI_U8*  g_pu8HamData;
HI_U8    g_u8HamBitOffset;
HI_U8    g_u8HamByteOffset;
HI_U8    g_u8HamByteLen;
HI_U8 const g_u8RemapCLUT[] = { 0x00, 0x01, 0x02, 0x11, 0x12, 0x21, 0x22, 0x23 };

HI_BOOL g_bHasM29_0[8];
HI_BOOL g_bHasM29_1[8];
HI_BOOL g_bHasM29_4[8];
TTX_P28_0_M29_0_INFO_S g_stM29_0_Info[8];
TTX_P28_1_M29_1_INFO_S g_stM29_1_Info[8];
TTX_P28_4_M29_4_INFO_S g_stM29_4_Info[8];

HI_U32 g_u32DefaultColorMap[32] =
{
    0x000,/*black*/
    0xf00,/*red*/
    0x0f0,/*green*/
    0xff0,/*yellow*/
    0x00f,/*blue*/
    0xf0f,/*Magenta*/
    0x0ff,/*Cyan*/
    0xfff,/*White*/

    0,/*Transparent*/
    0x700,/*Half red*/
    0x070,/*Half green*/
    0x770,/*Half yellow*/
    0x007,/*Half blue*/
    0x707,/*Half magenta*/
    0x077,/*Half cyan*/
    0x777,/*Grey*/

    0xf05,/**/
    0xf70,/**/
    0x0f7,/**/
    0xffb,/**/
    0x0ca,/**/
    0x500,/**/
    0x652,/**/
    0xc77,/**/

    0x333,/**/
    0xf77,/**/
    0x7f7,/**/
    0xff7,/**/
    0x77f,/**/
    0xf7f,/**/
    0x7ff,/**/
    0xddd /**/
};

HI_U32 g_u32RedefColorMap[32];



static HI_VOID TTX_FixHam24_18(HI_U8 u8Result, HI_U8* pu8Address, HI_U8* pu8Mode, HI_U8* pu8Data)
{
    switch (u8Result)
    {
    case 3:
        pu8Address[0] ^= 0x01;
        break;
    case 5:
        pu8Address[0] ^= 0x02;
        break;
    case 6:
        pu8Address[0] ^= 0x04;
        break;
    case 7:
        pu8Address[0] ^= 0x08;
        break;
    case 9:
        pu8Address[0] ^= 0x10;
        break;
    case 10:
        pu8Address[0] ^= 0x20;
        break;
    
    case 11:
        pu8Mode[0] ^= 0x01;
        break;
    case 12:
        pu8Mode[0] ^= 0x02;
        break;
    case 13:
        pu8Mode[0] ^= 0x04;
        break;
    case 14:
        pu8Mode[0] ^= 0x08;
        break;
    case 15:
        pu8Mode[0] ^= 0x10;
        break;
    
    case 17:
        pu8Data[0] ^= 0x01;
        break;
    case 18:
        pu8Data[0] ^= 0x02;
        break;
    case 19:
        pu8Data[0] ^= 0x04;
        break;
    case 20:
        pu8Data[0] ^= 0x08;
        break;
    case 21:
        pu8Data[0] ^= 0x10;
        break;
    case 22:
        pu8Data[0] ^= 0x20;
        break;
    case 23:
        pu8Data[0] ^= 0x40;
        break;
    default:
        break;
    }

}


/** Decode  Ham24/18  */
HI_BOOL  TTX_DecodeHam24_18  ( HI_U8 * pu8Triplet, HI_U8 *pu8Address, HI_U8 *pu8Mode,
                                             HI_U8 *pu8Data)
{
    HI_BOOL a = HI_FALSE;
    HI_BOOL b = HI_FALSE;
    HI_BOOL c = HI_FALSE; 
    HI_BOOL d = HI_FALSE; 
    HI_BOOL e = HI_FALSE; 
    HI_BOOL f = HI_FALSE;
    HI_U8 u8Result = 0;

    if ((HI_NULL == pu8Triplet) || (HI_NULL == pu8Address) || (HI_NULL == pu8Mode))
    {
        return HI_FALSE;
    }

    /** d1-3   d2-5  d3-6  d4-7  d5-9   d6-10  */
    *pu8Address = ((pu8Triplet[0] & 0x04) >> 2) + ((pu8Triplet[0] & 0x70) >> 3) + ((pu8Triplet[1] & 0x03) << 4);

    /**  d7-11  d8-12  d9-13   d10-14   d11-15  */
    *pu8Mode = ((pu8Triplet[1] & 0x7c) >> 2);

    /**d12-17   d13-18   d14-19   d15-20    d16-21   d17-22   d18-23   */

    *pu8Data = ((pu8Triplet[2] & 0x7f));

    a = (HI_BOOL) (!(HI_BOOL)(pu8Triplet[0] & 0x01) || !(HI_BOOL)(pu8Triplet[0] & 0x04)
                   || !(HI_BOOL)(pu8Triplet[0] & 0x10) || !(HI_BOOL)(pu8Triplet[0] & 0x40)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x01) || !(HI_BOOL)(pu8Triplet[1] & 0x04)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x10) || !(HI_BOOL)(pu8Triplet[1] & 0x40)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x01) || !(HI_BOOL)(pu8Triplet[2] & 0x04)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x10) || !(HI_BOOL)(pu8Triplet[2] & 0x40));

    b = (HI_BOOL) (!(HI_BOOL)(pu8Triplet[0] & 0x02) || !(HI_BOOL)(pu8Triplet[0] & 0x04)
                   || !(HI_BOOL)(pu8Triplet[0] & 0x20) || !(HI_BOOL)(pu8Triplet[0] & 0x40)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x02) || !(HI_BOOL)(pu8Triplet[1] & 0x04)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x20) || !(HI_BOOL)(pu8Triplet[1] & 0x40)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x02) || !(HI_BOOL)(pu8Triplet[2] & 0x04)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x20) || !(HI_BOOL)(pu8Triplet[2] & 0x40));

    c = (HI_BOOL) (!(HI_BOOL)(pu8Triplet[0] & 0x08) || !(HI_BOOL)(pu8Triplet[0] & 0x10)
                   || !(HI_BOOL)(pu8Triplet[0] & 0x20) || !(HI_BOOL)(pu8Triplet[0] & 0x40)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x08) || !(HI_BOOL)(pu8Triplet[1] & 0x10)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x20) || !(HI_BOOL)(pu8Triplet[1] & 0x40)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x08) || !(HI_BOOL)(pu8Triplet[2] & 0x10)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x20) || !(HI_BOOL)(pu8Triplet[2] & 0x40));

    d = (HI_BOOL) (!(HI_BOOL)(pu8Triplet[0] & 0x80) || !(HI_BOOL)(pu8Triplet[1] & 0x01)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x02) || !(HI_BOOL)(pu8Triplet[1] & 0x04)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x08) || !(HI_BOOL)(pu8Triplet[1] & 0x10)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x20) || !(HI_BOOL)(pu8Triplet[1] & 0x40));

    e = (HI_BOOL) (!(HI_BOOL)(pu8Triplet[1] & 0x80) || !(HI_BOOL)(pu8Triplet[2] & 0x01)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x02) || !(HI_BOOL)(pu8Triplet[2] & 0x04)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x08) || !(HI_BOOL)(pu8Triplet[2] & 0x10)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x20) || !(HI_BOOL)(pu8Triplet[2] & 0x40));

    f = (HI_BOOL) (!(HI_BOOL)(pu8Triplet[0] & 0x01) || !(HI_BOOL)(pu8Triplet[0] & 0x02)
                   || !(HI_BOOL)(pu8Triplet[0] & 0x04) || !(HI_BOOL)(pu8Triplet[0] & 0x08)
                   || !(HI_BOOL)(pu8Triplet[0] & 0x10) || !(HI_BOOL)(pu8Triplet[0] & 0x20)
                   || !(HI_BOOL)(pu8Triplet[0] & 0x40) || !(HI_BOOL)(pu8Triplet[0] & 0x80)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x01) || !(HI_BOOL)(pu8Triplet[1] & 0x02)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x04) || !(HI_BOOL)(pu8Triplet[1] & 0x08)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x10) || !(HI_BOOL)(pu8Triplet[1] & 0x20)
                   || !(HI_BOOL)(pu8Triplet[1] & 0x40) || !(HI_BOOL)(pu8Triplet[1] & 0x80)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x01) || !(HI_BOOL)(pu8Triplet[2] & 0x02)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x04) || !(HI_BOOL)(pu8Triplet[2] & 0x08)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x10) || !(HI_BOOL)(pu8Triplet[2] & 0x20)
                   || !(HI_BOOL)(pu8Triplet[2] & 0x40) || !(HI_BOOL)(pu8Triplet[2] & 0x80));

    if (a && b && c && d && e)     /** No errors   or  Error in f --- Accept data */
    {
        return HI_TRUE;
    }

    if (!(a && b && c && d && e) && !f)  /** Single error --- Fix bit error  */
    {
        u8Result = (HI_U8) (!a) + 2 * (HI_U8)(!b) + 4 * (HI_U8)(!c) + 8 * (HI_U8)(!d) + 16 * (HI_U8)(!e);
        
        TTX_FixHam24_18(u8Result, pu8Address, pu8Mode, pu8Data);

        return HI_TRUE;
    }

    return HI_FALSE;/* if (!(a && b && c && d && e) && f)  ---Double error --- Reject data */
}

HI_VOID TTX_Parse_BeginReadHam24_18Bit(HI_U8* gpData)
{
    g_pu8HamData       = gpData;
    g_u8HamBitOffset   = 0;
    g_u8HamByteOffset  = 0;
    g_u8HamByteLen     = 6;
}

HI_U32 TTX_Parse_ReadHam24_18Bit(HI_U8 u8BitNum)
{
    HI_U8 i = 0;
    HI_U8 u8BitOffset = 0;
    HI_U32 u32Data = 0;
    
    while(u8BitNum > 0)
    {
        /* how many bits to get */
        i = g_u8HamByteLen - g_u8HamBitOffset;
        if (u8BitNum < i)
        {
                i = u8BitNum;
        }

        /* get these bits */
        u32Data |= ((*g_pu8HamData >> g_u8HamBitOffset) & ((1 << i) - 1)) << u8BitOffset;
        u8BitNum -= i;
        u8BitOffset += i;

        /* goto the next byte */
        g_u8HamBitOffset += i;
        if (g_u8HamBitOffset >= g_u8HamByteLen)
        {
            g_pu8HamData++;
            g_u8HamByteOffset++;
            if (g_u8HamByteOffset > 2)
            {
                g_u8HamByteOffset = 0;
            }

            if(0 == g_u8HamByteOffset)
            {
                g_u8HamByteLen = 6;
            }
            else if(1 == g_u8HamByteOffset)
            {
                g_u8HamByteLen = 5;
            }
            else
            {
                g_u8HamByteLen = 7;
            }
            g_u8HamBitOffset = 0;
        }
    }

    return u32Data;
}

/** Decode and record  a directly displayed packet */
static HI_BOOL  TTX_SaveLine(TTX_PAGE_CONTEXT_S  * pstCurrentPoint, HI_U32 u32Row, const HI_U8 *   pu8Line)
{
    HI_U32 i = 0;
    HI_U8 u8Ch = 0;

    if ((HI_NULL == pstCurrentPoint) || (HI_NULL == pu8Line) || (u32Row >= TTX_ROW_NUM))
    {
        HI_ERR_TTX("In HI_TTX_SaveLine, Invalid parament\n");
        return HI_FALSE;
    }

    if (u32Row)              /** row 1-25 */
    {
        for (i = 0; i < TTX_LOP_COL_NUM; i++)
        {
            u8Ch = s_u8szOddParity[*pu8Line++];   /** Odd  parity  */
            if (u8Ch != 0xFF)
            {
                pstCurrentPoint->u32aPageText[u32Row][i] = u8Ch;
            }
        }

        return HI_TRUE;
    }
    else          /** row 0 */
    {
        for (i = 0; i < 8; i++)
        {
            pstCurrentPoint->u32aPageText[u32Row][i] = 0x20;
        }

        pu8Line += 8;
        for (i = 8; i < TTX_LOP_COL_NUM; i++)
        {
            u8Ch = s_u8szOddParity[*pu8Line++];
            if (u8Ch != 0xFF)
            {
                pstCurrentPoint->u32aPageText[u32Row][i] = u8Ch;
            }
        }

        return HI_TRUE;
    }
}

/** Set page active  position  */
HI_VOID  TTX_SetPgActPos (TTX_PAGE_CONTEXT_S  *   pstCurrentPoint, HI_U32 u32Row, HI_U32 u32Column,
                                 HI_U32 u32Code, HI_U32 u32Char,
                                 HI_U32 u32Masks)
{
    if ((HI_NULL == pstCurrentPoint) || (u32Row >= TTX_ROW_NUM) || (u32Column >= TTX_CMB_MAX_COL_NUM)
        || (u32Char < 32) || (u32Char > 127))
    {
        HI_ERR_TTX("In Page_Set_Active_Position, Invalid parament\n");
        return;
    }

    pstCurrentPoint->u32aPageText[u32Row][u32Column] = 128 + u32Char + (u32Code << 8) + (u32Masks << 10);
}

/** Convert  a  char  ,b0<->b7,b1<->b6,b2<->b5,b3<->b4*/
static HI_U8 TTX_ConvertChar(HI_U8 u8Inchar)
{
    u8Inchar = (HI_U8) ((u8Inchar << 4) + (u8Inchar >> 4));
    u8Inchar = ((u8Inchar & 0x33) << 2) + ((u8Inchar & 0xCC) >> 2);
    u8Inchar = ((u8Inchar & 0x55) << 1) + ((u8Inchar & 0xAA) >> 1);
    return u8Inchar;
}

/** Convert  a  line  */
HI_VOID TTX_ConvertLine(HI_U8 * pu8Line, HI_U32 u32Length)
{
    HI_U32 i = 0;

    if ((HI_NULL == pu8Line) || (u32Length < 1))
    {
        HI_ERR_TTX("In ConvertLine, Invalid parament\n");
        return;
    }

    for (i = 0; i < u32Length; i++)
    {
        if ((pu8Line[i] == 0) || (pu8Line[i] == 0xff))    /** 0 and 0xff  don't  need to convert */
        {
            continue;
        }

        pu8Line[i] = TTX_ConvertChar(pu8Line[i]);
    }
}

HI_BOOL TTX_IsEraseFlagSet(TTX_PAGE_S* pstpage)
{
    HI_U8 au8Tmp[TTX_LINE_MEM_SIZE];

    memset(au8Tmp, 0, sizeof(au8Tmp));
    memcpy(au8Tmp, pstpage->u8szLines[0], TTX_LINE_MEM_SIZE);

    TTX_ConvertLine(au8Tmp, TTX_LINE_MEM_SIZE);

    if (s_u8aUnhamtab[au8Tmp[5]] & 0x08)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

static HI_S32 TTX_DecodeMOTPageGPOP(TTX_MOT_INFO_S* pstMotInfo, TTX_PARSE_DECMOT_INFO_S* pstDecMotInfo)
{
    HI_U8  i = 0;
    HI_U8  u8PacketNum = 0;
    HI_U32 u32Offset = 0;


    if ((HI_NULL == pstMotInfo) || (HI_NULL == pstDecMotInfo))
    {
        HI_ERR_TTX("Parse MOT,GPOP: args error\n");
        return HI_FAILURE;
    }
    
    /*GPOP*/
    if (pstDecMotInfo->u8Object & 0x8)
    {
        u8PacketNum = 22;
        if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
        {
            u8PacketNum = 19;
            if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
            {
                HI_ERR_TTX("Parse MOT,GPOP packet %d not found\n", u8PacketNum);
                return HI_FAILURE;
            }
        }
        
        u32Offset = 2;
        
        for(i = 0; i < 10; i++)
        {
            if (0xff == (pstDecMotInfo->au8Data[i] = s_u8aUnhamtab[pstDecMotInfo->pu8Line[u32Offset + i]]))
            {
                HI_ERR_TTX("ham data error!\n");
                return HI_FAILURE;
            }
        }
        if (!pstDecMotInfo->bHasP27)
        {
            pstMotInfo->stGPOP.u8MagazineNum = pstDecMotInfo->au8Data[0] &0x7;
            pstMotInfo->stGPOP.u8PageNum = (pstDecMotInfo->au8Data[1] << 4) | (pstDecMotInfo->au8Data[2] & 0xf);
        }

         /* default objects */
        if(pstDecMotInfo->au8Data[5] & 0xf)
        {
            /* default object 1 */
            pstMotInfo->stDefaultObject[0].u8ObjectType = pstDecMotInfo->au8Data[5] & 0x3;
            pstMotInfo->stDefaultObject[0].u8SubPageNum = pstDecMotInfo->au8Data[6] & 0xf;
            pstMotInfo->stDefaultObject[0].u8PointerPos = pstDecMotInfo->au8Data[7] & 0xf;
            pstMotInfo->stDefaultObject[0].u8PointerTab = (pstDecMotInfo->au8Data[7] & 0xf) >> 3;/*0:pointer locate in packt 1  1: pointer locate in packt 2*/
            pstDecMotInfo->au8Data[7] &= 0x6;
            pstMotInfo->stDefaultObject[0].u8TripletNum = (pstDecMotInfo->au8Data[7] >> 1) *3 + pstMotInfo->stDefaultObject[0].u8ObjectType;

            /* default object 2 */
            pstMotInfo->stDefaultObject[1].u8ObjectType = (pstDecMotInfo->au8Data[5] >> 2) & 0x3;
            pstMotInfo->stDefaultObject[1].u8SubPageNum = pstDecMotInfo->au8Data[8] & 0xf;
            pstMotInfo->stDefaultObject[1].u8PointerPos = pstDecMotInfo->au8Data[9] & 0xf;
            pstMotInfo->stDefaultObject[1].u8PointerTab = (pstDecMotInfo->au8Data[9] & 0xf) >> 3;/*0:pointer locate in packt 1  1: pointer locate in packt 2*/
            pstDecMotInfo->au8Data[9] &= 0x6;
            pstMotInfo->stDefaultObject[1].u8TripletNum = (pstDecMotInfo->au8Data[9] >> 1) *3 + pstMotInfo->stDefaultObject[1].u8ObjectType;
        }
    }

    return HI_SUCCESS;
}



static HI_S32 TTX_DecodeMOTPagePOP(TTX_MOT_INFO_S* pstMotInfo, TTX_PARSE_DECMOT_INFO_S* pstDecMotInfo)
{
    HI_U8  i = 0;
    HI_U8  u8PacketNum = 0;
    HI_U32 u32Offset = 0;

    if ((HI_NULL == pstMotInfo) || (HI_NULL == pstDecMotInfo))
    {
        HI_ERR_TTX("Parse MOT,POP: args error\n");
        return HI_FAILURE;
    }

    /*POP*/
    pstDecMotInfo->u8Object &= 0x7;
    
    if(pstDecMotInfo->u8Object)
    {
        u32Offset = 2;
        
        if(3 < pstDecMotInfo->u8Object)
        {
            u8PacketNum = 23;
            if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
            {
                u8PacketNum = 20;
                if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
                {
                    HI_ERR_TTX("Parse MOT, NPOP packet %d not found\n", u8PacketNum);
                    return HI_FAILURE;
                }
            }
            u32Offset += (pstDecMotInfo->u8Object - 4) * 10;
        }
        else
        {
            u8PacketNum = 22;
            if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
            {
                u8PacketNum = 19;
                if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
                {
                    HI_ERR_TTX("Parse MOT, NPOP packet %d not found\n", u8PacketNum);
                    return HI_FAILURE;
                }
            }
            u32Offset += pstDecMotInfo->u8Object * 10;
        }

        for(i = 0; i < 10; i++)
        {
            if (0xff == (pstDecMotInfo->au8Data[i] = s_u8aUnhamtab[pstDecMotInfo->pu8Line[u32Offset + i]]))
            {
                HI_ERR_TTX("ham data error!\n");
                return HI_FAILURE;
            }
        }

        if (!pstDecMotInfo->bHasP27)
        {
            pstMotInfo->stNPOP.u8MagazineNum = pstDecMotInfo->au8Data[0] &0x7;
            pstMotInfo->stNPOP.u8PageNum = (pstDecMotInfo->au8Data[1] << 4) | (pstDecMotInfo->au8Data[2] & 0xf);
        }

         /* default objects */
        if(pstDecMotInfo->au8Data[5] &0xf)
        {
            /* default object 1 */
            pstMotInfo->stDefaultObject[2].u8ObjectType = pstDecMotInfo->au8Data[5] & 0x3;
            pstMotInfo->stDefaultObject[2].u8SubPageNum = pstDecMotInfo->au8Data[6] & 0xf;
            pstMotInfo->stDefaultObject[2].u8PointerPos = pstDecMotInfo->au8Data[7] & 0xf;
            pstMotInfo->stDefaultObject[2].u8PointerTab = (pstDecMotInfo->au8Data[7] & 0xf) >> 3;/*0:pointer locate in packt 1  1: pointer locate in packt 2*/
            pstDecMotInfo->au8Data[7] &= 0x6;
            pstMotInfo->stDefaultObject[2].u8TripletNum = (pstDecMotInfo->au8Data[7] >> 1) *3 + pstMotInfo->stDefaultObject[2].u8ObjectType;

            /* default object 2 */
            pstMotInfo->stDefaultObject[3].u8ObjectType = (pstDecMotInfo->au8Data[5] >> 2) & 0x3;
            pstMotInfo->stDefaultObject[3].u8SubPageNum = pstDecMotInfo->au8Data[8] & 0xf;
            pstMotInfo->stDefaultObject[3].u8PointerPos = pstDecMotInfo->au8Data[9] & 0xf;
            pstMotInfo->stDefaultObject[3].u8PointerTab = (pstDecMotInfo->au8Data[9] & 0xf) >> 3;/*0:pointer locate in packt 1  1: pointer locate in packt 2*/
            pstDecMotInfo->au8Data[9] &= 0x6;
            pstMotInfo->stDefaultObject[3].u8TripletNum = (pstDecMotInfo->au8Data[9] >> 1) *3 + pstMotInfo->stDefaultObject[3].u8ObjectType;
        }
        
        pstMotInfo->bFallback = (pstDecMotInfo->au8Data[4] & 0x1) ? HI_TRUE : HI_FALSE;
        pstMotInfo->bSidePanelLeft = ((pstDecMotInfo->au8Data[4] >> 1) & 0x1) ? HI_TRUE : HI_FALSE;
        pstMotInfo->bSidePanelRight= ((pstDecMotInfo->au8Data[4] >> 2) & 0x1) ? HI_TRUE : HI_FALSE;
        pstMotInfo->bBlackBgSubstitute = ((pstDecMotInfo->au8Data[4] >> 3) & 0x1) ? HI_TRUE : HI_FALSE;

    }

    return HI_SUCCESS;
}

static HI_S32 TTX_DecodeMOTPageGDRCS(TTX_MOT_INFO_S* pstMotInfo, TTX_PARSE_DECMOT_INFO_S* pstDecMotInfo)
{
    HI_U8  i = 0;
    HI_U8  u8PacketNum = 0;
    HI_U32 u32Offset = 0;

    if ((HI_NULL == pstMotInfo) || (HI_NULL == pstDecMotInfo))
    {
        HI_ERR_TTX("Parse MOT,POP: args error\n");
        return HI_FAILURE;
    }


    /*GDRCS*/
    if(pstDecMotInfo->u8DRCS & 0x8)
    {
        u8PacketNum = 24;
        if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
        {
            u8PacketNum = 21;
            if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
            {
                HI_ERR_TTX("Parse MOT, GDRCS packet %d not found\n", u8PacketNum);
                return HI_FAILURE;
            }
        }
        
        u32Offset = 2;
        
        for (i = 0; i < 4; i++)
        {
            if (0xff == (pstDecMotInfo->au8Data[i] = s_u8aUnhamtab[pstDecMotInfo->pu8Line[u32Offset + i]]))
            {
                HI_ERR_TTX("ham data error!\n");
                return HI_FAILURE;
            }
        }
        if (!pstDecMotInfo->bHasP27)
        {
            pstMotInfo->stGDRCS.u8MagazineNum = pstDecMotInfo->au8Data[0] & 0x7;
            pstMotInfo->stGDRCS.u8PageNum = (pstDecMotInfo->au8Data[1] << 4) | (pstDecMotInfo->au8Data[2] & 0xf);
        }

    }

    return HI_SUCCESS;
}



static HI_S32 TTX_DecodeMOTPageDRCS(TTX_MOT_INFO_S* pstMotInfo, TTX_PARSE_DECMOT_INFO_S* pstDecMotInfo)

{
    HI_U8  i = 0;
    HI_U8  u8PacketNum = 0;
    HI_U32 u32Offset = 0;

    if ((HI_NULL == pstMotInfo) || (HI_NULL == pstDecMotInfo))
    {
        HI_ERR_TTX("Parse MOT,POP: args error\n");
        return HI_FAILURE;
    }

    /* DRCS */
    pstDecMotInfo->u8DRCS &= 0x7;
    
    if(pstDecMotInfo->u8DRCS)
    {
        u8PacketNum = 24;
        if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
        {
            u8PacketNum = 21;
            if (!((pstDecMotInfo->pstMOPPage->u32ValidLines >> u8PacketNum) & 0x1))
            {
                HI_ERR_TTX("Parse MOT, DRCS packet %d not found\n", u8PacketNum);
                return HI_FAILURE;
            }
        }
        u32Offset = 2 + (pstDecMotInfo->u8DRCS << 1);
        for(i = 0; i < 4; i++)
        {
            if (0xff == (pstDecMotInfo->au8Data[i] = s_u8aUnhamtab[pstDecMotInfo->pu8Line[u32Offset + i]]))
            {
                HI_ERR_TTX("ham data error!\n");
                return HI_FAILURE;
            }
        }
        if (!pstDecMotInfo->bHasP27)
        {
            pstMotInfo->stNDRCS.u8MagazineNum = pstDecMotInfo->au8Data[0] & 0x7;
            pstMotInfo->stNDRCS.u8PageNum = (pstDecMotInfo->au8Data[1] << 4) | (pstDecMotInfo->au8Data[2] & 0xf);
        }
    }

    return HI_SUCCESS;
}

HI_S32 TTX_DecodeMOTPage(HI_UNF_TTX_PAGE_ADDR_S* pstPgAddr, TTX_MOT_INFO_S* pstMotInfo, HI_BOOL bHasP27)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_U8 u8PacketNum = 0;
    HI_U8 u8Object = 0;
    HI_U8 u8DRCS = 0;
    HI_U8 au8Data[10];

    HI_U32 u32Offset = 0;
    HI_U32 u32PageNumTen = 0;
    HI_U32 u32PageNumUnit = 0;
    
    HI_U8* pu8Line = HI_NULL;
    
    TTX_PAGE_S stMOPPage;
    HI_UNF_TTX_PAGE_ADDR_S stMOP;
    TTX_PARSE_DECMOT_INFO_S stDecMotInfo;

    memset(au8Data, 0x0, sizeof(au8Data));
    memset(&stMOPPage, 0x0, sizeof(TTX_PAGE_S));
    memset(&stMOP, 0x0, sizeof(HI_UNF_TTX_PAGE_ADDR_S));
    memset(&stDecMotInfo, 0x0, sizeof(TTX_PARSE_DECMOT_INFO_S));

    if ((HI_NULL == pstPgAddr) || (HI_NULL == pstMotInfo))
    {
        HI_ERR_TTX("Decode MOT, args error \n");
        return HI_FAILURE;
    }

    stMOP.u8MagazineNum = pstPgAddr->u8MagazineNum;
    stMOP.u8PageNum = 0xfe;
    stMOP.u16PageSubCode = 0;

    if (!bHasP27)
    {
        pstMotInfo->stGPOP.u8PageNum = 0xff;
        pstMotInfo->stNPOP.u8PageNum = 0xff;
        pstMotInfo->stGPOP.u16PageSubCode = 0;
        pstMotInfo->stNPOP.u16PageSubCode = 0;
        pstMotInfo->stGDRCS.u8PageNum = 0xff;
        pstMotInfo->stNDRCS.u8PageNum = 0xff;
        pstMotInfo->stGDRCS.u16PageSubCode = 0;
        pstMotInfo->stNDRCS.u16PageSubCode = 0;
    }


    pstMotInfo->stDefaultObject[0].u8ObjectType = 0;
    pstMotInfo->stDefaultObject[1].u8ObjectType = 0;
    pstMotInfo->stDefaultObject[2].u8ObjectType = 0;
    pstMotInfo->stDefaultObject[3].u8ObjectType = 0;

    pstMotInfo->bFallback = HI_TRUE;
    pstMotInfo->bSidePanelLeft = HI_FALSE;
    pstMotInfo->bSidePanelRight = HI_FALSE;
    pstMotInfo->bBlackBgSubstitute = HI_FALSE;

    /*find the mot page*/
    if(HI_SUCCESS != TTX_Data_FindPage(stMOP.u8MagazineNum, stMOP.u8PageNum, stMOP.u16PageSubCode, &stMOPPage))
    {
        return HI_FAILURE;
    }

    
    u8PacketNum = 0;
    while (u8PacketNum < 25)      /**Convert  normal  packet   */
    {
        if ((stMOPPage.u32ValidLines >> u8PacketNum) & 0x1)          /** Valid line */
        {
            TTX_ConvertLine(stMOPPage.u8szLines[u8PacketNum], TTX_LINE_MEM_SIZE);            /**Convert a  packet  */
        }

        u8PacketNum++;
    }

    u32PageNumTen = (pstPgAddr->u8PageNum >> 4) & 0xf;
    u32PageNumUnit = pstPgAddr->u8PageNum & 0xf;

    /*judge current page defined in which packet*/
    if(9 >= u32PageNumUnit )
    {
        u8PacketNum = (u32PageNumTen >> 1) + 1;
        u32Offset = u32PageNumUnit << 1;
        if(u32PageNumTen & 0x1)
        {
            u32Offset += 20;
        }
    }
    else if((0xa <= u32PageNumUnit) &&(0xf > u32PageNumUnit))
    {
        u8PacketNum = (u32PageNumTen /3) + 9;
        u32Offset = (u32PageNumTen%3) *12 + (u32PageNumUnit - 0xa) * 2;
    }
    else
    {
        u8PacketNum = 14;
        u32Offset = (u32PageNumUnit - 0xa) * 2;
    }

    pu8Line = stMOPPage.u8szLines[u8PacketNum];
    u32Offset +=2;
    
     /* object page link */
    u8Object = s_u8aUnhamtab[pu8Line[u32Offset]];
    if(0xff == u8Object)
    {
        u8Object = 0;
    }
    u8Object &= 0xf;
    /* DRCS page link */
    u8DRCS = s_u8aUnhamtab[pu8Line[u32Offset + 1]];
    if(0xff == u8DRCS)
    {
        u8DRCS = 0;
    }
    
    u8DRCS &= 0xf;

    /*init DecMot info*/
    stDecMotInfo.bHasP27 = bHasP27;
    stDecMotInfo.pstMOPPage = &stMOPPage;
    stDecMotInfo.pu8Line = pu8Line;
    stDecMotInfo.au8Data = au8Data;
    stDecMotInfo.u8Object = u8Object;
    stDecMotInfo.u8DRCS = u8DRCS;


    /*GPOP*/
    s32Ret = TTX_DecodeMOTPageGPOP(pstMotInfo, &stDecMotInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /*POP*/
    s32Ret = TTX_DecodeMOTPagePOP(pstMotInfo, &stDecMotInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /*GDRCS*/
    s32Ret = TTX_DecodeMOTPageGDRCS(pstMotInfo, &stDecMotInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /* DRCS */
    s32Ret = TTX_DecodeMOTPageDRCS(pstMotInfo, &stDecMotInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 TTX_DecodePOPPage(HI_UNF_TTX_PAGE_ADDR_S* pstPgAddr, TTX_OBJECT_POINTER_S* pstObj, HI_U32* pu32TripPos, HI_U8* pu8Line)
{
    TTX_PAGE_S stPage;
    HI_U8   u8PacketNum = 0;
    HI_U32  u32ObjDefineTrip = 0;
    HI_U8   u8DesignCode = 0;
    HI_U8   au8Data[3] = {0, 0, 0};
    HI_U8   au8DataLine[TTX_LINE_MEM_SIZE];

    memset(&stPage, 0x0, sizeof(TTX_PAGE_S));
    memset(au8DataLine, 0x0, sizeof(au8DataLine));
    
    if(HI_SUCCESS == TTX_Data_FindPage(pstPgAddr->u8MagazineNum, pstPgAddr->u8PageNum, pstObj->u8SubPageNum, &stPage))
    {
        u8PacketNum = pstObj->u8PointerTab + 1;
        /*find the packet of pointer table*/
        if(HI_SUCCESS  == TTX_Data_FindPacket(&stPage, u8PacketNum, 0, au8DataLine))
        {
            TTX_ConvertLine(au8DataLine, TTX_LINE_MEM_SIZE);

            if(HI_TRUE == TTX_DecodeHam24_18(au8DataLine + 3 + pstObj->u8TripletNum * 3, au8Data, au8Data + 1, au8Data + 2))
            {
                if(pstObj->u8PointerPos)
                {
                    u32ObjDefineTrip = (au8Data[1] >> 3) | (((HI_U32)au8Data[2]) << 2);
                }
                else
                {
                    u32ObjDefineTrip = ((((HI_U32)au8Data[1]) & 0x3) << 6) | (au8Data[0]);
                }

                if(u32ObjDefineTrip < 507)/*507 is the max triplet num, which = 13 * (25 - 2 + 16)*/
                {
                    u8PacketNum = u32ObjDefineTrip /13 +3;
                    /*the max packet num is 26, but packet 26 may have 16 packet*/
                    if(26 < u8PacketNum )
                    {
                        u8DesignCode = u8PacketNum -26;
                        u8PacketNum = 26;
                    }

                    /*find the packet of POP defintion triplet*/
                    if(HI_SUCCESS  == TTX_Data_FindPacket(&stPage, u8PacketNum, u8DesignCode, pu8Line))
                    {
                        TTX_ConvertLine(pu8Line, TTX_LINE_MEM_SIZE);
                        *pu32TripPos = u32ObjDefineTrip /13;
                        return HI_SUCCESS;
                    }
                }
            }
        }
    }
    return HI_FAILURE;
}


static HI_VOID TTX_DecodeDrcsColor(HI_U32 u32ColorBitPlane, TTX_DCLUT_S* pstDCLUT, TTX_PARSE_DRCS_MODE_S* pstDrcsMode)
{
    HI_U32 u32Entry = 0;
    
    if (TTX_DRCS_MODE_1 == pstDrcsMode->enDRCSMode)
    {

        /*Normal DRCS*/
        if(pstDrcsMode->u8PTUNum & 0x40)
        {
            u32Entry = pstDCLUT->u8DCLUT4Normal[u32ColorBitPlane];
        }
        else/*Global DRCS*/
        {
            u32Entry = pstDCLUT->u8DCLUT4Global[u32ColorBitPlane];
        }
    }
    else if (TTX_DRCS_MODE_2 == pstDrcsMode->enDRCSMode)
    {
        /*Normal DRCS*/
        if(pstDrcsMode->u8PTUNum & 0x40)
        {
            u32Entry = pstDCLUT->u8DCLUT16Normal[u32ColorBitPlane];
        }
        else/*Global DRCS*/
        {
            u32Entry = pstDCLUT->u8DCLUT16Global[u32ColorBitPlane];
        }

    }
    else if (TTX_DRCS_MODE_3 == pstDrcsMode->enDRCSMode)
    {
        /*Normal DRCS*/
        if(pstDrcsMode->u8PTUNum & 0x40)
        {
            u32Entry = pstDCLUT->u8DCLUT16Normal[u32ColorBitPlane];
        }
        else/*Global DRCS*/
        {
            u32Entry = pstDCLUT->u8DCLUT16Global[u32ColorBitPlane];
        }

    }
    
    u32Entry &= 0x1f;
    *(pstDrcsMode->pu32DRCSColorInfo) = g_u32RedefColorMap[u32Entry];
    pstDrcsMode->pu32DRCSColorInfo++;

    return;
}


static HI_S32 TTX_DecodeDRCSMode(TTX_PAGE_CONTEXT_S* pstCurrentPoint, 
                                          TTX_PARSE_DRCS_MODE_S* pstDrcsMode,
                                          HI_U8* pu8DataLine)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 k = 0;
    HI_U8  u8PTUNum = 0;
    HI_U32 u32PTUReadLen = 0;
    HI_U32 u32PacketNum = 0;
    HI_U32 u32ColorBitPlane = 0;
    TTX_DRCS_MODE_E enDRCSMode = 0;
    TTX_DCLUT_S* pstDCLUT = HI_NULL;

    TTX_PAGE_S stPage;
    HI_U8  au8PTUData[4][20];
    

    memset(au8PTUData, 0x0, sizeof(au8PTUData));
    memset(&stPage, 0x0, sizeof(TTX_PAGE_S));

    u8PTUNum = pstDrcsMode->u8PTUNum;
    enDRCSMode = pstDrcsMode->enDRCSMode;
    u32PacketNum = ((u8PTUNum & 0x3f) >> 1) + 1;
    u32PTUReadLen = pstDrcsMode->u32PTUReadLen > 2 ? 2 : pstDrcsMode->u32PTUReadLen;


    for (i = 0; i < u32PTUReadLen; i++)
    {
        for (j = 0; j < 20; j++)
        {
            au8PTUData[i][j] = s_u8szOddParity[pu8DataLine[2 + i * 20 + j]];
        }
    }

    if (u32PTUReadLen > 2)
    {        
        if (HI_SUCCESS != TTX_Data_FindPacket(&stPage, u32PacketNum + 1, 0, pu8DataLine))
        {
            return HI_FAILURE;
        }
        
        TTX_ConvertLine(pu8DataLine, TTX_LINE_MEM_SIZE);

        for (i = 2; i < 4; i++)
        {
            for (j = 0; j < 20; j++)
            {
                au8PTUData[i][j] = s_u8szOddParity[pu8DataLine[2 + (i -2)* 20 + j]];
            }
        }
    }


    if (TTX_DRCS_MODE_0 == enDRCSMode)
    {
        for (i = 0; i < 20; i++)
        {
            for (j = 0; j < 6; j++)
            {
                if(au8PTUData[0][i] & 0x20)
                {
                    *(pstDrcsMode->pu32DRCSColorInfo) = 0; 
                }
                else
                {
                     *(pstDrcsMode->pu32DRCSColorInfo) = 1;
                }
                
                (pstDrcsMode->pu32DRCSColorInfo)++;
                
                au8PTUData[0][i] <<= 1;
            }
        }
    }
    else
    {
        if (pstCurrentPoint->bHasP28_1)
        {
            pstDCLUT = &pstCurrentPoint->stP28_1_Info.stDCLUT;
        }
        else if (g_bHasM29_1[pstCurrentPoint->stCurPgAddr.u8MagazineNum])
        {
            pstDCLUT = &g_stM29_1_Info[pstCurrentPoint->stCurPgAddr.u8MagazineNum].stDCLUT;
        }
        else
        {
            return HI_FAILURE;
        }

        if ((TTX_DRCS_MODE_1 == enDRCSMode) || (TTX_DRCS_MODE_2 == enDRCSMode))
        {
            u32ColorBitPlane = 0;
            HI_U32 u32MaxK = 2;

            if (TTX_DRCS_MODE_2 == enDRCSMode)
            {
                u32MaxK = 4;
            }
            
            for (i = 0; i < 20; i++)
            {
                for (j = 0; j < 6; j++)
                {
                    for(k = 0; k < u32MaxK; k++)
                    {
                        if (au8PTUData[k][i] & 0x20)/*bit5*/
                        {
                            u32ColorBitPlane |= 0x1 << k;
                        }
                        au8PTUData[k][i] <<= 1;
                    }

                    TTX_DecodeDrcsColor(u32ColorBitPlane, pstDCLUT, pstDrcsMode);
                }
            }
        }
        else if (TTX_DRCS_MODE_3 == enDRCSMode)
        {
            u32ColorBitPlane = 0;
            
            for (i = 0; i < 5; i++)
            {
                for (j = 0; j < 6; j++)
                {
                    for (k = 0; k < 4; k++)
                    {
                        if (au8PTUData[0][i * 4 + k] & 0x20)/*bit5*/
                        {
                            u32ColorBitPlane |= 0x1 << k;
                        }
                        
                        au8PTUData[0][i * 4 + k] <<= 1;
                    }
                    
                    TTX_DecodeDrcsColor(u32ColorBitPlane, pstDCLUT, pstDrcsMode);
                }
            }
        }
    }
        
    return HI_SUCCESS;
}

HI_S32 TTX_DecodeDRCSPage(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_UNF_TTX_PAGE_ADDR_S* pstPgAddr, HI_U8 u8PTUNum, TTX_DRCS_MODE_E* penDRCSMode,  HI_U32* pu32DRCSColorInfo)
{
    HI_U8  u8StartBit = 0;
    HI_U32 u32TripNum = 0;
    HI_U32 u32PTU = 0;
    HI_U32 u32PacketNum = 0;
    HI_U32 u32PTUReadLen = 0;
    TTX_DRCS_MODE_E enDRCSMode = TTX_DRCS_MODE_BUTT;
    
    HI_U8  au8HamData[6];
    HI_U8  au8DataLine[TTX_LINE_MEM_SIZE];
    
    TTX_PAGE_S stPage;

    u32PTU = u8PTUNum & 0x3f;
    
    if (48 <= u32PTU)
    {
        HI_ERR_TTX("ptu num invalid...\n");
        return HI_FAILURE;
    }

    if ((HI_NULL == pstCurrentPoint) || 
        (HI_NULL == pstPgAddr) ||
        (HI_NULL == penDRCSMode) ||
        (HI_NULL == pu32DRCSColorInfo))
    {
        HI_ERR_TTX("Args invalid...\n");
        return HI_FAILURE;
    }

    memset(&stPage, 0x0, sizeof(TTX_PAGE_S));
    memset(au8DataLine, 0x0, sizeof(au8DataLine));
    memset(au8HamData, 0x0, sizeof(au8HamData));

    
    /*find the DRCS page*/
    if(HI_SUCCESS != TTX_Data_FindPage(pstPgAddr->u8MagazineNum, pstPgAddr->u8PageNum, pstPgAddr->u16PageSubCode, &stPage))
    {
        return HI_FAILURE;
    }

    if(HI_SUCCESS != TTX_Data_FindPacket(&stPage, 28, 3, au8DataLine))
    {
        return HI_FAILURE;
    }

    u32TripNum = u32PTU * 4 /18 + 1;/*DRCS Downloading Mode Invocation start at the second triplet*/
    u8StartBit = u32PTU * 4 %18;

    TTX_ConvertLine(au8DataLine, TTX_LINE_MEM_SIZE);
    if (HI_FALSE == TTX_DecodeHam24_18(au8DataLine + 3 + 3 * u32TripNum, au8HamData,  au8HamData + 1,  au8HamData + 2))
    {
        return HI_FAILURE;
    }

    if (HI_FALSE == TTX_DecodeHam24_18(au8DataLine + 6 + 3 * u32TripNum, au8HamData + 3,  au8HamData + 4,  au8HamData + 5))
    {
        return HI_FAILURE;
    }

    TTX_Parse_BeginReadHam24_18Bit(au8HamData);
    TTX_Parse_ReadHam24_18Bit(u8StartBit);
    enDRCSMode = (TTX_DRCS_MODE_E)TTX_Parse_ReadHam24_18Bit(4);

    u32PacketNum = (u32PTU >> 1) + 1;
    switch(enDRCSMode)
    {
        case TTX_DRCS_MODE_0:
            u32PTUReadLen = 1;
            break;
        case TTX_DRCS_MODE_1:
            u32PTUReadLen = 2;
            break;
        case TTX_DRCS_MODE_2:
            u32PTUReadLen = 4;
            break;
        case TTX_DRCS_MODE_3:
            u32PTUReadLen = 1;
            break;
        default:
            u32PTUReadLen = 1;
            break;
    }
    
    *penDRCSMode = enDRCSMode;
    
    if (HI_SUCCESS != TTX_Data_FindPacket(&stPage, u32PacketNum, 0, au8DataLine))
    {
        return HI_FAILURE;
    }

    TTX_ConvertLine(au8DataLine, TTX_LINE_MEM_SIZE);

    TTX_PARSE_DRCS_MODE_S stDrcsMode;

    stDrcsMode.enDRCSMode = enDRCSMode;
    stDrcsMode.pu32DRCSColorInfo = pu32DRCSColorInfo;
    stDrcsMode.u32PTUReadLen = u32PTUReadLen;
    stDrcsMode.u8PTUNum = u8PTUNum;


    return TTX_DecodeDRCSMode(pstCurrentPoint, &stDrcsMode, au8DataLine);
    
}

HI_S32 TTX_DecodePacket27CompLink(HI_U8 *pu8Data, TTX_PAGE_CONTEXT_S*  pstCurrentPoint)
{
    HI_U8 u8Address = 0;
    HI_U8 u8Mode = 0;
    HI_U8 u8Data = 0;
    HI_U8 u8LinkFunc = 0;
    HI_U8 u8MagNum = 0;
    HI_U8 u8PgNum = 0;
    HI_U16 u16SubPgNum = 0;
    HI_UNF_TTX_PAGE_ADDR_S *pstPageAddr = HI_NULL;

    if (HI_FALSE == TTX_DecodeHam24_18(pu8Data, &u8Address, &u8Mode, &u8Data))
    {
        return HI_FAILURE;
    }
    u8LinkFunc = u8Address & 0x3;
    if(LINK_FUNC_GPOP == u8LinkFunc)
    {
        pstPageAddr = &pstCurrentPoint->stMOTInfo.stGPOP;
    }
    else if(LINK_FUNC_POP == u8LinkFunc)
    {
        pstPageAddr = &pstCurrentPoint->stMOTInfo.stNPOP;
    }
    else if(LINK_FUNC_GDRCS== u8LinkFunc)
    {
        pstPageAddr = &pstCurrentPoint->stMOTInfo.stGDRCS;
    }
    else if(LINK_FUNC_DRCS == u8LinkFunc)
    {
        pstPageAddr = &pstCurrentPoint->stMOTInfo.stNDRCS;
    }
    u8MagNum ^= u8Data & 0x7;/*bit12--bit14*//*why ^*/
    u8PgNum = (u8Mode & 0xf) | (u8Data & 0xf0);
    u16SubPgNum = 0;

    if(0xff != u8PgNum)
    {
         return TTX_Parse_SetPgAddr(pstPageAddr, u8MagNum, u8PgNum, u16SubPgNum);
    }
    return HI_FAILURE;
}

HI_S32 TTX_DecodeP28_0_M29_0(HI_U8* pu8Data, TTX_P28_0_M29_0_INFO_S* pstInfo)
{
    HI_U32 i = 0;
    HI_U8  au8Data[39];

    memset(au8Data, 0x0, sizeof(au8Data));
    
    for(i = 0; i < 13 ; i++)
    {
        if (HI_FALSE == TTX_DecodeHam24_18(pu8Data + 3 * i, au8Data + 3 * i,  au8Data + 1 + 3 * i,  au8Data + 2 + 3 * i))
        {
            return HI_FAILURE;
        }
    }

    TTX_Parse_BeginReadHam24_18Bit(au8Data);
    i = TTX_Parse_ReadHam24_18Bit(4);/*page function bit*/

    if (PAGE_FUNC_LOP == i)
    {
        TTX_Parse_ReadHam24_18Bit(3);/*page coding*/
        pstInfo->u8DefG0G2 = (HI_U8)TTX_Parse_ReadHam24_18Bit(7);/* default G0/G2 set */
        pstInfo->u82ndG0 = (HI_U8)TTX_Parse_ReadHam24_18Bit(7);/* second G0 set */
        pstInfo->bSidePanelLeft = (HI_U8)TTX_Parse_ReadHam24_18Bit(1);/* left side panel */
        pstInfo->bSidePanelRight = (HI_U8)TTX_Parse_ReadHam24_18Bit(1);/* right side panel */
        pstInfo->bSidePanelStatus = (HI_U8)TTX_Parse_ReadHam24_18Bit(1);/* side panel status */
        pstInfo->u8SidePanelCols = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);/* number of columns in side panels */
        /* CLUT2 and CLUT3 */
        for(i = 0; i < 16; i++)
        {
            pstInfo->u32CLUT2_3[i] = (HI_U32)TTX_Parse_ReadHam24_18Bit(12);
        }
        pstInfo->u8DefScreenColor = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);/*default screen color*/
        pstInfo->u8DefRowColor = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);/* default row color */
        pstInfo->bBlackBgSubstitute = (HI_U8)TTX_Parse_ReadHam24_18Bit(1);/* black background color substitution */

        i = TTX_Parse_ReadHam24_18Bit(3);
        pstInfo->u8ForeCLUT = ((g_u8RemapCLUT[i] >> 4) & 0x7) << 3;
        pstInfo->u8BackCLUT = (g_u8RemapCLUT[i] & 0x7) << 3;

    }
    return HI_SUCCESS;
}

HI_S32 TTX_DecodeP28_1_M29_1(HI_U8* pu8Data, TTX_P28_1_M29_1_INFO_S*   pstInfo)
{
    HI_U32 i = 0;
    HI_U8  au8Data[39];

    memset(au8Data, 0x0, sizeof(au8Data));

    
    for(i = 0; i < 13 ; i++)
    {
        if (HI_FALSE == TTX_DecodeHam24_18(pu8Data + 3 * i, au8Data + 3 * i,  au8Data + 1 + 3 * i,  au8Data + 2 + 3 * i))
        {
            return HI_FAILURE;
        }
    }
    
    TTX_Parse_BeginReadHam24_18Bit(au8Data);
    TTX_Parse_ReadHam24_18Bit(2);
    pstInfo->u8G0Set = (HI_U8)TTX_Parse_ReadHam24_18Bit(7);/*Character Set Code for G0 Table*/
    TTX_Parse_ReadHam24_18Bit(1);
    pstInfo->u8G1Set = (HI_U8)TTX_Parse_ReadHam24_18Bit(7);/*Character Set Code for G1 Table*/
    TTX_Parse_ReadHam24_18Bit(1);

    /*DCLUT4 for Global 12x10x2 DRCS Mode Characters*/
    for(i = 0; i < 4 ; i++)
    {
         pstInfo->stDCLUT.u8DCLUT4Global[i] = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);
    }
    /*DCLUT4 for Normal 12x10x2 DRCS Mode Characters*/
    for(i = 0; i < 4 ; i++)
    {
         pstInfo->stDCLUT.u8DCLUT4Normal[i] = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);
    }

    /*DCLUT16 for Global 12x10x4 and 6x5x4 DRCS Mode Characters*/
    for(i = 0; i < 16 ; i++)
    {
         pstInfo->stDCLUT.u8DCLUT16Global[i] = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);
    }
    /*DCLUT16 for Normal 12x10x4 and 6x5x4 DRCS Mode Characters*/
    for(i = 0; i < 16 ; i++)
    {
         pstInfo->stDCLUT.u8DCLUT16Normal[i] = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);
    }

    return HI_SUCCESS;
}
HI_S32 TTX_DecodeP28_4_M29_4(HI_U8* pu8Data, TTX_P28_4_M29_4_INFO_S*   pstInfo)
{
    HI_U32 i = 0;
    HI_U8  au8Data[39];

    memset(au8Data, 0x0, sizeof(au8Data));

    
    for(i = 0; i < 13 ; i++)
    {
        if (HI_FALSE == TTX_DecodeHam24_18(pu8Data + 3 * i, au8Data + 3 * i,  au8Data + 1 + 3 * i,  au8Data + 2 + 3 * i))
        {
            return HI_FAILURE;
        }
    }

    TTX_Parse_BeginReadHam24_18Bit(au8Data);
    i = TTX_Parse_ReadHam24_18Bit(4);/*page function bit*/

    if (PAGE_FUNC_LOP == i)
    {
        TTX_Parse_ReadHam24_18Bit(3);/*page coding*/
        pstInfo->u8DefG0G2 = (HI_U8)TTX_Parse_ReadHam24_18Bit(7);/* default G0/G2 set */
        pstInfo->u82ndG0 = (HI_U8)TTX_Parse_ReadHam24_18Bit(7);/* second G0 set */
        pstInfo->bSidePanelLeft = (HI_U8)TTX_Parse_ReadHam24_18Bit(1);/* left side panel */
        pstInfo->bSidePanelRight = (HI_U8)TTX_Parse_ReadHam24_18Bit(1);/* right side panel */
        pstInfo->bSidePanelStatus = (HI_U8)TTX_Parse_ReadHam24_18Bit(1);/* side panel status */
        pstInfo->u8SidePanelCols = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);/* number of columns in side panels */
        /* CLUT0 and CLUT1 */
        for(i = 0; i < 16; i++)
        {
            pstInfo->u32CLUT0_1[i] = (HI_U32)TTX_Parse_ReadHam24_18Bit(12);
        }
        pstInfo->u8DefScreenColor = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);/*default screen color*/
        pstInfo->u8DefRowColor = (HI_U8)TTX_Parse_ReadHam24_18Bit(5);/* default row color */
        pstInfo->bBlackBgSubstitute = (HI_U8)TTX_Parse_ReadHam24_18Bit(1);/* black background color substitution */

        i = TTX_Parse_ReadHam24_18Bit(3);
        pstInfo->u8ForeCLUT = (g_u8RemapCLUT[i] >> 4) & 0x7;
        pstInfo->u8BackCLUT = g_u8RemapCLUT[i] & 0x7;

    }
    return HI_SUCCESS;
}
/**dongfuahi add end*/


static HI_VOID TTX_DecodeControlCode(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U8* pu8Line)
{
    pstCurrentPoint->u32Flag = 0;
    
    if (s_u8aUnhamtab[pu8Line[ 5]] & 0x08)
    {
        pstCurrentPoint->u32Flag |= 0x0001;
    }                                                                                   /** flag C4  */
    if (s_u8aUnhamtab[pu8Line[7]] & 0x04)
    {
        pstCurrentPoint->u32Flag |= 0x0002;
    }                                                                                   /** flag C5  */
    if (s_u8aUnhamtab[pu8Line[7]] & 0x08)
    {
        pstCurrentPoint->u32Flag |= 0x0004;
    }                                                                                   /** flag C6  */
    if (s_u8aUnhamtab[pu8Line[8]] & 0x01)
    {
        pstCurrentPoint->u32Flag |= 0x0008;
    }                                                                                   /** flag C7  */
    if (s_u8aUnhamtab[pu8Line[8]] & 0x02)
    {
        pstCurrentPoint->u32Flag |= 0x0010;
    }                                                                                   /** flag C8  */
    if (s_u8aUnhamtab[pu8Line[8]] & 0x04)
    {
        pstCurrentPoint->u32Flag |= 0x0020;
    }                                                                                 /** flag C9  */
    if (s_u8aUnhamtab[pu8Line[8]] & 0x08)
    {
        pstCurrentPoint->u32Flag |= 0x0040;
    }                                                                                  /** flag C10  */
    if (s_u8aUnhamtab[pu8Line[9]] & 0x01)
    {
        pstCurrentPoint->u32Flag |= 0x0080;
    }                                                                                   /** flag C11  */
    if (s_u8aUnhamtab[pu8Line[9]] & 0x02)
    {
        pstCurrentPoint->u32Flag |= 0x0400;
    }                                                                                 /** flag C12  */
    if (s_u8aUnhamtab[pu8Line[9]] & 0x04)
    {
        pstCurrentPoint->u32Flag |= 0x0200;
    }                                                                                   /** flag C13  */
    if (s_u8aUnhamtab[pu8Line[9]] & 0x08)
    {
        pstCurrentPoint->u32Flag |= 0x0100;
    }                                                                                 /** flag C14  */

    return;
}

static HI_S32 TTX_DecodeLine27(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U8* pu8Line)
{

    HI_U32 i = 0;

    /**page numbers , magazine numbers  */
    HI_U32 u32page1 = 0;
    HI_U32 u32page2 = 0;
    HI_U32 u32page3 = 0;


    /**subcode numbers  */
    HI_U32 u32Subpg1 = 0;
    HI_U32 u32Subpg2 = 0;
    HI_U32 u32Subpg3 = 0;
    HI_U32 u32Subpg4 = 0;  

    HI_U8 u8PgNum = 0;
    HI_U8 u8MagNum = 0;
    HI_U16 u16SubPgNum = 0;


    HI_U32 u32DesignCode = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    u32DesignCode = s_u8aUnhamtab[pu8Line[2]] & 0x0f;
    
    HI_INFO_TTX("Decode X/27/%d  packet\n", u32DesignCode);
    
    if (0 == u32DesignCode)
    {
        for (i = 1; i <= 6; i++)
        {
            u32page1 = s_u8aUnhamtab[pu8Line[6 * i - 3]] & 0x0f;
            u32page2 = s_u8aUnhamtab[pu8Line[6 * i - 2]] & 0x0f;
            u32page3 = (((s_u8aUnhamtab[pu8Line[6 * i]] & 0x08) >> 3) |((s_u8aUnhamtab[pu8Line[6 * i + 2]] & 0x0c) >> 1)) & 0x7;
    
            u32Subpg1 = s_u8aUnhamtab[pu8Line[6 * i - 1]] & 0x0f;/** subcode s1 (4 bits)  */
            u32Subpg2 = s_u8aUnhamtab[pu8Line[6 * i]] & 0x07;/** subcode s2 (3 bits)  */
            u32Subpg3 = s_u8aUnhamtab[pu8Line[6 * i + 1]] & 0x0f;/** subcode s3 (4 bits)  */
            u32Subpg4 = s_u8aUnhamtab[pu8Line[6 * i + 2]] & 0x03;/** subcode s4 (2 bits)  */
    
            if ((u32page1 == 0xF) && (u32page2 == 0xF))
            {
                continue;
            }
            else
            {
                u8MagNum = pstCurrentPoint->stCurPgAddr.u8MagazineNum ^((HI_U8)u32page3);
                u8PgNum = (HI_U8) (u32page1 + u32page2 * 16);
                
                if ((u32Subpg1 == 0x0f) && (u32Subpg2 == 0x07) && (u32Subpg3 == 0x0f) && (u32Subpg4 == 0x03))
                {
                    u16SubPgNum = 0;
                }
                else
                {
                    u16SubPgNum = (HI_U16) ((u32Subpg1 > 9 ? 0 : u32Subpg1) + u32Subpg2 * 10);
                }
    
                s32Ret |= TTX_Parse_SetPgAddr(&(pstCurrentPoint->astLink27[i - 1]), u8MagNum, u8PgNum, u16SubPgNum);
            }
        }
    
        pstCurrentPoint->bShowP24 = HI_FALSE;
    
        if (( s_u8aUnhamtab[pu8Line[39]] & 0x08) && (HI_TRUE == pstCurrentPoint->bHasP24))
        {
            pstCurrentPoint->bShowP24 = HI_TRUE;
        }
    }
    else if (4 == u32DesignCode)
    {
        for (i = 0; i < 6; i++)
        {
            if (HI_SUCCESS == TTX_DecodePacket27CompLink(pu8Line + 3 + 6 * i, pstCurrentPoint))
            {
                pstCurrentPoint->bHasP27_4= HI_TRUE;
            }
            else
            {
                s32Ret = HI_FAILURE;
            }
        }
    }
    else if (5 == u32DesignCode)
    {
        for (i = 0; i < 2; i++)
        {
            if (HI_SUCCESS == TTX_DecodePacket27CompLink(pu8Line + 3 + 6 * i, pstCurrentPoint))
            {
                pstCurrentPoint->bHasP27_5 = HI_TRUE;
            }
            else
            {
                s32Ret = HI_FAILURE;
            }
        }
    }
    else
    {
        HI_INFO_TTX("Packet X/27/* for furture use\n");
        s32Ret = HI_SUCCESS;
    }

    return HI_SUCCESS;

}



static HI_S32 TTX_DecodeLine28(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U8* pu8Line)
{
    HI_VOID * pstInfo = HI_NULL;
    
    HI_U32 u32DesignCode = 0;
    HI_S32 s32Ret = HI_SUCCESS;


    u32DesignCode = s_u8aUnhamtab[pu8Line[2]] & 0x0f;
    HI_INFO_TTX("Decode X/28/%d  packet\n", u32DesignCode);
    if (0 == u32DesignCode)
    {
        pstInfo = (TTX_P28_0_M29_0_INFO_S*)(&pstCurrentPoint->stP28_0_Info);
        if (HI_SUCCESS == TTX_DecodeP28_0_M29_0(pu8Line + 3, pstInfo))
        {
            pstCurrentPoint->bHasP28_0 = HI_TRUE;
            return HI_SUCCESS;
        }
        
        return HI_FAILURE;
    }
    else if (1 == u32DesignCode)
    {
        pstInfo = (TTX_P28_1_M29_1_INFO_S*)(&pstCurrentPoint->stP28_1_Info);
        if (HI_SUCCESS == TTX_DecodeP28_1_M29_1(pu8Line + 3, pstInfo))
        {
            pstCurrentPoint->bHasP28_1 = HI_TRUE;
            return HI_SUCCESS;
        }
        return HI_FAILURE;
    }
    else if(4 == u32DesignCode)
    {
        pstInfo = (TTX_P28_4_M29_4_INFO_S*)(&pstCurrentPoint->stP28_4_Info);
        if (HI_SUCCESS == TTX_DecodeP28_4_M29_4(pu8Line + 3, pstInfo))
        {
            pstCurrentPoint->bHasP28_4 = HI_TRUE;
            return HI_SUCCESS;
        }
        return HI_FAILURE;
    }
    else
    {
        HI_INFO_TTX("Packet X/28/* for furture use\n");
        s32Ret = HI_SUCCESS;
    }

    
    return s32Ret;
}


static HI_S32 TTX_DecodeLine29(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U8* pu8Line)
{
    HI_VOID * pstInfo = HI_NULL;
    
    HI_U32 u32DesignCode = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8MagNum = 0;

    u8MagNum = pstCurrentPoint->stCurPgAddr.u8MagazineNum;
    u32DesignCode = s_u8aUnhamtab[pu8Line[2]] & 0x0f;
    
    HI_INFO_TTX("Decode M/29/%d  packet\n", u32DesignCode);
    
    if (0 == u32DesignCode)
    {
        pstInfo = (TTX_P28_0_M29_0_INFO_S*)(g_stM29_0_Info + u8MagNum);
        if (HI_SUCCESS == TTX_DecodeP28_0_M29_0(pu8Line + 3, pstInfo))
        {
            g_bHasM29_0[u8MagNum]= HI_TRUE;
            return HI_SUCCESS;
        }
        return HI_FAILURE;
    }
    else if (1 == u32DesignCode)
    {
        pstInfo = (TTX_P28_1_M29_1_INFO_S*)(g_stM29_1_Info + u8MagNum);
        if (HI_SUCCESS == TTX_DecodeP28_1_M29_1(pu8Line + 3, pstInfo))
        {
            g_bHasM29_1[u8MagNum] = HI_TRUE;
            return HI_SUCCESS;
        }
        return HI_FAILURE;
    }
    else if(4 == u32DesignCode)
    {
        pstInfo = (TTX_P28_4_M29_4_INFO_S*)(g_stM29_4_Info + u8MagNum);
        if (HI_SUCCESS == TTX_DecodeP28_4_M29_4(pu8Line + 3, pstInfo))
        {
            g_bHasM29_4[u8MagNum] = HI_TRUE;
            return HI_SUCCESS;
        }
        return HI_FAILURE;
    }
    else
    {
        HI_INFO_TTX("Packet M/29/* for furture use\n");
        s32Ret = HI_SUCCESS;
    }

    return s32Ret;
}

static HI_S32 TTX_DecodeLine30(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U8* pu8Line)
{
    /**page numbers , magazine numbers  */
    HI_U32 u32page1 = 0;
    HI_U32 u32page2 = 0;
    HI_U32 u32page3 = 0;


    /**subcode numbers  */
    HI_U32 u32Subpg1 = 0;
    HI_U32 u32Subpg2 = 0;
    HI_U32 u32Subpg3 = 0;
    HI_U32 u32Subpg4 = 0;  


    HI_U8 u8PgNum = 0;
    HI_U8 u8MagNum = 0;
    HI_U16 u16SubPgNum = 0;

    HI_S32 s32Ret = HI_SUCCESS;

    HI_INFO_TTX("Decode 8/30 packet\n");
    
    u32page1 = s_u8aUnhamtab[pu8Line[3]] & 0x0f;
    u32page2 = s_u8aUnhamtab[pu8Line[4]] & 0x0f;
    u32page3 = ((s_u8aUnhamtab[pu8Line[6]] & 0x08) >> 3) + ((s_u8aUnhamtab[pu8Line[8]] & 0x0c) >> 1);
    
    u32Subpg1 = s_u8aUnhamtab[pu8Line[5]] & 0x0f;/** subcode s1 (4 bits) */
    u32Subpg2 = s_u8aUnhamtab[pu8Line[6]] & 0x07;/** subcode s2 (3 bits)  */
    u32Subpg3 = s_u8aUnhamtab[pu8Line[7]] & 0x0f; /** subcode s3 (4 bits)*/
    u32Subpg4 = s_u8aUnhamtab[pu8Line[8]] & 0x03; /** subcode s4 (2 bits) */
    
    if ((u32Subpg1 + (u32Subpg2 << 4) + (u32Subpg3 << 7) + (u32Subpg4 << 11)) == 0x01fff)/** Index page */
    {
        s32Ret |= TTX_Parse_SetPgAddr(&(pstCurrentPoint->stIndexPgAddr), 1, 0, 0);
    }
    else
    {
        u8MagNum = u32page3 % 8;
        u8PgNum = (HI_U8) (u32page1 + u32page2 * 16);
        u16SubPgNum = (HI_U16) ((u32Subpg1 > 9 ? 0 : u32Subpg1) + u32Subpg2 * 10);
        s32Ret |= TTX_Parse_SetPgAddr(&(pstCurrentPoint->stIndexPgAddr), u8MagNum, u8PgNum, u16SubPgNum);
    }
    
    if (((s_u8aUnhamtab[pu8Line[2]] & 0x0e) >> 1) == 0x0)
    {
        HI_INFO_TTX("Format 1 8/30 packet\n");
    }
    
    if (((s_u8aUnhamtab[pu8Line[2]] & 0x0e) >> 1) == 0x1)
    {
        HI_INFO_TTX("Format 2 8/30 packet\n");
    }

    return s32Ret;
}


/**  Decode  a  packet  and  store  it into  page context */
static HI_S32 TTX_DecodeLine(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U8* pu8Line)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bRet = HI_TRUE;
    HI_U32 u32Row = 0;
    HI_U32 i = 0;
    
    /**page numbers , magazine numbers  */
    HI_U32 u32page1 = 0;
    HI_U32 u32page2 = 0;


    /**subcode numbers  */
    HI_U32 u32Subpg1 = 0;
    HI_U32 u32Subpg2 = 0;
    HI_U32 u32Subpg3 = 0;
    HI_U32 u32Subpg4 = 0;  

    HI_S32 s32FoundPage = HI_FAILURE;
    HI_U8 u8MagNum = 0, u8PgNum = 0;
    HI_U16 u16SubPgNum = 0;

    if ((HI_NULL == pstCurrentPoint) || (HI_NULL == pu8Line))
    {
        return HI_FAILURE;
    }

    if (s_u8aUnhamtab [pu8Line[0]] == 0xFF)   /** decode error */
    {
        return HI_FAILURE;
    }

    if (s_u8aUnhamtab[pu8Line[1]] == 0xFF)   /** decode error */
    {
        return HI_FAILURE;
    }                                                                                 /** Packet  NO */
    u32Row = ((s_u8aUnhamtab[pu8Line[0]] & 0x08) >> 3) + ((s_u8aUnhamtab[pu8Line[1]] & 0x0f) * 2);

    if (u32Row == 0)           /** Packet  0 */
    {
        pstCurrentPoint->stCurPgAddr.u8MagazineNum = (s_u8aUnhamtab[pu8Line[0]] & 0x07);    /** Magazine NO */
        for (i = 2; i <= 9; i++)
        {
            if (s_u8aUnhamtab[pu8Line[i]] == 0xff)
            {
                return HI_FAILURE;
            }
        }

        u32page1 = s_u8aUnhamtab[pu8Line[2]] & 0x0f;
        u32page2 = s_u8aUnhamtab[pu8Line[3]] & 0x0f;
        if ((u32page1 > 9) || (u32page2 > 9))    /** decode error */
        {
            return HI_FAILURE;
        }

        pstCurrentPoint->stCurPgAddr.u8PageNum = (HI_U8) (u32page1 + 16 * u32page2);    /** Page NO */

        u32Subpg1 = s_u8aUnhamtab[pu8Line[4]] & 0x0f;/** subcode s1 (4 bits)  */
        u32Subpg2 = s_u8aUnhamtab[pu8Line[5]] & 0x07;/** subcode s2 (3 bits)   */
        u32Subpg3 = s_u8aUnhamtab[pu8Line[6]] & 0x0f;/**  subcode s3 (4 bits)   */
        u32Subpg4 = s_u8aUnhamtab[pu8Line[7]] & 0x03;/**  subcode s4 (2 bits)  */

        if ((u32Subpg1 == 0x0f) && (u32Subpg2 == 0x07) && (u32Subpg3 == 0x0f) && (u32Subpg4 == 0x03))
        {
            pstCurrentPoint->stCurPgAddr.u16PageSubCode = 0;
        }
        else
        {
            pstCurrentPoint->stCurPgAddr.u16PageSubCode = (HI_U16)  ((u32Subpg1 > 9 ? 0 : u32Subpg1) + u32Subpg2 * 10);
        }


        HI_INFO_TTX("SubPage:%d\n", pstCurrentPoint->stCurPgAddr.u16PageSubCode);

        for (i = 0; i < 4; i++)   /** Get newly four valid page */
        {
            s32Ret |= TTX_Parse_GetPgAddr(pstCurrentPoint->stCurPgAddr, &u8MagNum, &u8PgNum, &u16SubPgNum);
            s32FoundPage = TTX_Data_GetValidPage(&u8MagNum, &u8PgNum, &u16SubPgNum,
                                                 (TTX_SEARCH_DIRECTION_E)(TTX_NEXT2_PAGE + i));
            if (s32FoundPage == HI_SUCCESS)
            {
                s32Ret |= TTX_Parse_SetPgAddr(&(pstCurrentPoint->astLink27[i]), u8MagNum, u8PgNum, u16SubPgNum);
            }
            else
            {
                pstCurrentPoint->astLink27[i] = pstCurrentPoint->stCurPgAddr;
            }
        }

        if (HI_SUCCESS != s32Ret)
        {
            HI_INFO_TTX("Call TTX_Parse_GetPgAddr or TTX_Parse_SetPgAddr failed\n");
            return HI_FAILURE;
        }


        TTX_DecodeControlCode(pstCurrentPoint, pu8Line);

    }

    if ((u32Row < TTX_ROW_NUM))      /** Packet 0-24, directly  displayed packets */
    {
        bRet = TTX_SaveLine(pstCurrentPoint, u32Row, pu8Line + 2);
        if (!bRet)
        {
            HI_INFO_TTX("Call TTX_SaveLine failed\n");
            return HI_FAILURE;
        }
    }

    if (24 == u32Row)
    {
        pstCurrentPoint->bHasP24 = HI_TRUE;
    }
    else if (u32Row == 27)           /** Packet 27 , link  page  */
    {
        s32Ret = TTX_DecodeLine27(pstCurrentPoint, pu8Line);
    }
    else if (u32Row == 28)      /**Packet 28, enhanced  packet   */
    {    
        s32Ret = TTX_DecodeLine28(pstCurrentPoint, pu8Line); 
    }
    else if (u32Row == 29)     /**Packet 29, enhanced  packet   */
    {  
        s32Ret = TTX_DecodeLine29(pstCurrentPoint, pu8Line); 
    }
    else if (u32Row == 30)               /**Packet 30 ,service packet  */
    {    
        s32Ret = TTX_DecodeLine30(pstCurrentPoint, pu8Line); 
    }

    return s32Ret;
}


static HI_U8 TTX_UpdateSubpage_FindStartPosition(TTX_PAGE_CONTEXT_S*  pstCurrentPoint, HI_U8* au8szSubPage)
{
    HI_U8 u8StartPosition = 0;
    HI_U16 u16SubPage = pstCurrentPoint->stCurPgAddr.u16PageSubCode;
    
    for (u8StartPosition = 0; u8StartPosition < TTX_MAX_SUBPAGENO; u8StartPosition += 8)
    {
        if (u16SubPage >= au8szSubPage[u8StartPosition % TTX_MAX_SUBPAGENO])
        {
            if ((u16SubPage < au8szSubPage[(u8StartPosition + 8) % TTX_MAX_SUBPAGENO])
                || (au8szSubPage[(u8StartPosition + 8) % TTX_MAX_SUBPAGENO] == 0))
            {
                break;
            }
        }
    }
    
    return u8StartPosition;
}


static HI_VOID TTX_Update_SubpageNavigation(TTX_PAGE_CONTEXT_S*  pstCurrentPoint, 
                                                      HI_UNF_TTX_PAGEAREA_S* pstPageArea,
                                                      HI_U8 u8NumOfSubPg,
                                                      HI_U8 u8StartPosition)
                                                      
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;   /**  Draw char  param*/
    HI_U32 u32ColNum = pstCurrentPoint->u32ColumnNum;

    stDrawCharParam.pstCharAttr = &stCharAttr;

    
    if (pstCurrentPoint->bNavigation)
    {
        s32Ret |= TTX_Show_SetArea(pstPageArea, TTX_ROW_NUM + TTX_NAVIGATION_BAR, u32ColNum - 1, 1, 1);
    }
    else
    {
        s32Ret |= TTX_Show_SetArea(pstPageArea, TTX_ROW_NUM, u32ColNum - 1, 1, 1);
    }
    
    stDrawCharParam.u32Foreground = 0xff00ff00;
    
    if (pstCurrentPoint->bTtxMix)
    {
        stDrawCharParam.u32Background = 0x00000000;
    }
    else
    {
        stDrawCharParam.u32Background = 0xff000000;
    }
    
    if (u8StartPosition + 8 < u8NumOfSubPg)    /**  ">>" */
    {
        TTX_Show_SetCharAttr(&stCharAttr, 0x3b, HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_CHARSET_G2, 0);
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID *)&stDrawCharParam);
    }
    
    if ((u8NumOfSubPg > 8) && (u8StartPosition + 8 > u8NumOfSubPg))    /**  "<<" */
    {
        TTX_Show_SetCharAttr(&stCharAttr, 0x2b, HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_CHARSET_G2, 0);
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID *)&stDrawCharParam);
    }

    
    return;    
}


/** Set the address  of   a  page   */
HI_S32   TTX_Parse_SetPgAddr(HI_UNF_TTX_PAGE_ADDR_S  * pstPgAddr, HI_U8 u8MagNum, HI_U8 u8PgNum, HI_U16 u16SubPgNum)
{
    if (HI_NULL == pstPgAddr)       /** Invalid param, just return */
    {
        return HI_FAILURE;
    }

    if ((0xff != u8MagNum) && (u8MagNum > 8))    /** Invalid param, just return */
    {
        return HI_FAILURE;
    }
#if 0
    if ((0xff != u8PgNum) /*&& (u8PgNum > 99)*/)     /** Invalid param, just return */
    {
        return HI_FAILURE;
    }
#endif

    if ((u16SubPgNum > 0x3f7f))    /** Invalid param, just return */
    {
        return HI_FAILURE;
    }

    pstPgAddr->u8MagazineNum = u8MagNum;
    pstPgAddr->u8PageNum = u8PgNum;
    pstPgAddr->u16PageSubCode = u16SubPgNum;

    return HI_SUCCESS;
}

/**  Get the address  of  a  page */
HI_S32   TTX_Parse_GetPgAddr(const HI_UNF_TTX_PAGE_ADDR_S pstPgAddr, HI_U8 * pu8MagNum, HI_U8 * pu8PgNum,
                             HI_U16 * pu16SubPgNum)
{
    if ((HI_NULL == pu8MagNum) || (HI_NULL == pu8PgNum) || (HI_NULL == pu16SubPgNum))   /** Invalid param, just return */
    {
        return HI_FAILURE;
    }

    *pu8MagNum = pstPgAddr.u8MagazineNum;
    *pu8PgNum = pstPgAddr.u8PageNum;
    *pu16SubPgNum = pstPgAddr.u16PageSubCode;

    return HI_SUCCESS;
}

/**    Push  the  data  of  a  page  into the queue */

HI_S32 TTX_Parse_GetPgData(const HI_VOID* hDataParse, TTX_SEGMENT_S_PTR pstSegment)
{
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = HI_NULL;
    HI_S32 s32Ret = HI_FAILURE;

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S *)hDataParse;

    if ((HI_NULL == pstCurrentPoint) || (HI_NULL == pstSegment))    /** Invalid param, just return */
    {
        HI_ERR_TTX("Invalid handle\n");
        return HI_FAILURE;
    }

    s32Ret = TTX_PesQueue_En(pstCurrentPoint->pstPAGEQueue, pstSegment);

    if (HI_FAILURE == s32Ret)
    {
        HI_INFO_TTX("EnPesQueue failure\n");
    }
    return s32Ret;
}

HI_VOID TTX_Parse_UpdateTime(TTX_PAGE_CONTEXT_S * pstContextHead)
{
    HI_U8 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_PAGE_CONTEXT_S *  pstCurrentPoint = HI_NULL;
    HI_U8 u8aTimeCode[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    HI_U32 u32aTimeNum[8] = {0, 0, 0, 0, 0, 0, 0, 0};           /**Time,hh:mm:ss */
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;   /**  Draw char  param*/
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;
    HI_U8 u8Ch = 0xFF;

    pstCurrentPoint = pstContextHead;
    if (HI_NULL == pstCurrentPoint)
    {
        return;
    }

    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));


    if (HI_SUCCESS != TTX_Data_Get_TimeCode(u8aTimeCode))
    {
        return;
    }

    TTX_ConvertLine(u8aTimeCode, 8);

    for (i = 0; i < 8; i++)
    {
        u8Ch = s_u8szOddParity[u8aTimeCode[i]];
        if (u8Ch == 0xFF)
        {
            return;
        }
        else
        {
            u32aTimeNum[i] = u8Ch;
        }
    }

    TTX_PARSE_LOCK();

    if (pstContextHead->u32ZoomFlag == 2)
    {
        TTX_PARSE_UNLOCK();
        return;
    }


    for (i = 0; i < 8; i++)                /**  Draw char */
    {
        TTX_Show_SetCharAttr(&stCharAttr, u32aTimeNum[i], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                             HI_UNF_TTX_CHARSET_G0, 0);
        stDrawCharParam.pstCharAttr = &stCharAttr;
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 32 + i, 1, 1);
        stDrawCharParam.pstPageArea   = &stPageArea;
        stDrawCharParam.u32Foreground = 0xffffff00;
        if (pstCurrentPoint->bTtxMix)
        {
            stDrawCharParam.u32Background = 0x00000000;
        }
        else
        {
            stDrawCharParam.u32Background = 0xff000000;
        }

        s32Ret |= TTT_Show_DrawChar(pstCurrentPoint, &stDrawCharParam);
    }


    s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 32, 1, 8);
    stRefrshLayerParam.pstPageArea = &stPageArea;

    s32Ret |= TTT_Show_Refresh(pstCurrentPoint, &stRefrshLayerParam);

    TTX_PARSE_UNLOCK();

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_Parse_UpdateTime, Errors occur while call functions\n");
    }
}

HI_VOID TTX_Parse_Flash(TTX_PAGE_CONTEXT_S * pstContextHead)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8RowCount = 0;
    HI_U8 u8ColCount = 0;
    static HI_U32 u32FlashSwitch = 0;
    TTX_FLAAREA_S  *pstFlashArea = HI_NULL;            /** Flash area */
    TTX_PAGE_CONTEXT_S*  pstCurrentPoint = HI_NULL;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;   /**  Draw char  param*/
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;

    pstCurrentPoint = pstContextHead;
    if (HI_NULL == pstCurrentPoint)
    {
        return;
    }

    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));


    u32FlashSwitch++;

    TTX_PARSE_LOCK();
    if (pstCurrentPoint->bFlash && pstCurrentPoint->pFlaAreaHead)
    {
        pstFlashArea = pstCurrentPoint->pFlaAreaHead;
        while (HI_NULL != pstFlashArea)
        {
            u8ColCount = ((pstFlashArea->u8DoubleWidth || pstFlashArea->u8DoubleSize) ? 2 : 1);
            u8RowCount = ((pstFlashArea->u8DoubleHeight || pstFlashArea->u8DoubleSize) ? 2 : 1);
            stDrawCharParam.pstCharAttr = &(pstFlashArea->stCharAttr);
            s32Ret |= TTX_Show_SetArea(&stPageArea, pstFlashArea->u8LineNum, pstFlashArea->u8ColNum, u8RowCount,
                                       u8ColCount);
            stDrawCharParam.pstPageArea = &stPageArea;

            /** Set background color  */
            if (pstCurrentPoint->bTtxMix)
            {
                stDrawCharParam.u32Background = 0x00000000;
            }
            else
            {
                stDrawCharParam.u32Background = pstFlashArea->BackgdCol;
            }

            /** Change the  foreground color to flashs */
            if (u32FlashSwitch % 2)
            {
                stDrawCharParam.u32Foreground = pstFlashArea->ForegdCol;
            }
            else
            {
                if (pstCurrentPoint->bTtxMix)
                {
                    stDrawCharParam.u32Foreground = 0x00000000;
                }
                else
                {
                    stDrawCharParam.u32Foreground = pstFlashArea->BackgdCol;
                }
            }

            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID*)&stDrawCharParam);
            pstFlashArea = pstFlashArea->pstNext;
        }

        s32Ret |= TTX_Show_SetArea(&stPageArea, 1, 0, TTX_ROW_NUM - 1, pstCurrentPoint->u32ColumnNum);
        stRefrshLayerParam.pstPageArea = &stPageArea;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
    }

    TTX_PARSE_UNLOCK();

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_Parse_Flash, Errors occur while call functions\n");
    }
}

HI_VOID TTX_Parse_InputNumber(TTX_PAGE_CONTEXT_S * pstContextHead)
{
    HI_U32 j = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_PAGE_CONTEXT_S *  pstCurrentPoint = HI_NULL;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;  /**  Draw char  param*/
    HI_UNF_TTX_FILLRECT_S stFillRectParam;
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;
    TTX_INPUT_NUM_S * pstInputNum = HI_NULL;

    pstCurrentPoint = pstContextHead;
    if (HI_NULL == pstCurrentPoint)
    {
        return;
    }

    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));


    TTX_PARSE_LOCK();

    pstInputNum = &pstCurrentPoint->stInputNum;
    if (pstCurrentPoint->bHold)
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 1, 1, 6);
        stFillRectParam.pstPageArea = &stPageArea;
        if (pstCurrentPoint->bTtxMix)
        {
            stFillRectParam.u32Color = 0x00000000;
        }
        else
        {
            stFillRectParam.u32Color = 0xff000000;
        }

        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID *) &stFillRectParam);

        TTX_Show_SetCharAttr(&stCharAttr, 0x4E, HI_FALSE, HI_UNF_TTX_G0SET_GREEK, HI_UNF_TTX_G2SET_GREEK,
                                 HI_UNF_TTX_CHARSET_G0, 0);
        stDrawCharParam.pstCharAttr = &stCharAttr;
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 3, 1, 1);
        stDrawCharParam.pstPageArea   = &stPageArea;
        stDrawCharParam.u32Foreground = 0xffffffff;
        if (pstCurrentPoint->bTtxMix)
        {
            stDrawCharParam.u32Background = 0x00000000;
        }
        else
        {
            stDrawCharParam.u32Background = 0xff000000;
        }

        s32Ret |= TTT_Show_DrawChar(pstCurrentPoint, &stDrawCharParam);

        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 1, 1, 6);
        stRefrshLayerParam.pstPageArea = &stPageArea;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);

    }
    else if (TTX_InputNumIsOnGoing(&pstCurrentPoint->stInputNum))
    {
        for (j = 0; j < 3; j++)
        {
            TTX_Show_SetCharAttr(&stCharAttr, pstInputNum->au8Buf[j], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                                 HI_UNF_TTX_CHARSET_G0, 0);
            stDrawCharParam.pstCharAttr = &stCharAttr;
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, j + 1, 1, 1);
            stDrawCharParam.pstPageArea   = &stPageArea;
            stDrawCharParam.u32Foreground = 0xff00ff00;
            if (pstCurrentPoint->bTtxMix)
            {
                stDrawCharParam.u32Background = 0x00000000;
            }
            else
            {
                stDrawCharParam.u32Background = 0xff000000;
            }

            s32Ret |= TTT_Show_DrawChar(pstCurrentPoint, &stDrawCharParam);
        }

        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 4, 1, 3);
        stFillRectParam.pstPageArea = &stPageArea;
        if (pstCurrentPoint->bTtxMix)
        {
            stFillRectParam.u32Color = 0x00000000;
        }
        else
        {
            stFillRectParam.u32Color = 0xff000000;
        }

        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID *) &stFillRectParam);

        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 1, 1, 6);
        stRefrshLayerParam.pstPageArea = &stPageArea;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
    }

    if (TTX_IsLastFigureInputted(pstInputNum, TTX_DIGIT_NUM_OF_PAGE_NUM))  /** Input three number */
    {
        TTX_ResetInputNum(pstInputNum);
    }

    TTX_PARSE_UNLOCK();

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_Parse_InputNumber, Errors occur while call functions\n");
    }
}

HI_U32 TTX_GetRandValue(HI_VOID)
{
    HI_S32 s32Fd = -1;
    HI_U32 u32RandValue = 0;

    s32Fd = open("/dev/urandom",  O_RDONLY);
    if (-1 == s32Fd)
    {
        HI_INFO_TTX("open /dev/urandom failed\n");
        return u32RandValue;
    }
    
    if (-1 == read(s32Fd, &u32RandValue, sizeof(u32RandValue)))
    {
        u32RandValue = 0;
        HI_INFO_TTX("read /dev/urandom failed\n");
    }
    
    if (-1 == close(s32Fd))
    {
        HI_INFO_TTX("close /dev/urandom failed\n");
    }
    
    return u32RandValue;
}

HI_VOID TTX_Parse_InvalidRequest(TTX_PAGE_CONTEXT_S * pstContextHead)
{
    HI_U32 j = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8szPageAddr[4] = {0, 0, 0, 0};
    TTX_PAGE_CONTEXT_S *  pstCurrentPoint = HI_NULL;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;   /**  Draw char  param*/
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;

    pstCurrentPoint = pstContextHead;
    
    if (HI_NULL == pstCurrentPoint)
    {
        return;
    }

    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));



    TTX_PARSE_LOCK();
    if (pstCurrentPoint->bInvalidReq)
    {
        u8szPageAddr[0] = (HI_U8) (TTX_GetRandValue() % 8 + 0x31);
        u8szPageAddr[1] = (HI_U8) (TTX_GetRandValue() % 10 + 0x30);
        u8szPageAddr[2] = (HI_U8) (TTX_GetRandValue() % 10 + 0x30);

        for (j = 0; j < 3; j++)
        {
            TTX_Show_SetCharAttr(&stCharAttr, u8szPageAddr[j], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                                 HI_UNF_TTX_CHARSET_G0, 0);
            stDrawCharParam.pstCharAttr = &stCharAttr;
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, j + 17, 1, 1);
            stDrawCharParam.pstPageArea   = &stPageArea;
            stDrawCharParam.u32Foreground = 0xff00ff00;
            if (pstCurrentPoint->bTtxMix)
            {
                stDrawCharParam.u32Background = 0x00000000;
            }
            else
            {
                stDrawCharParam.u32Background = 0xff000000;
            }

            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID *)&stDrawCharParam);
        }

        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 17, 1, 3);
        stRefrshLayerParam.pstPageArea = &stPageArea;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
    }

   TTX_PARSE_UNLOCK();

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_Parse_InvalidRequest, Errors occur while call functions\n");
    }
}

HI_VOID TTX_Parse_UpdateSubpage(TTX_PAGE_CONTEXT_S * pstContextHead)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 i = 0;
    HI_U8 j = 0;
    HI_U32 u32ColNum = 0;
    HI_U16 u16SubPage = 0;
    HI_U8 u8StartPosition = 0;
    HI_U8 au8AutoChar[5] = {0x41, 0x55, 0x54, 0x4f, 0};   /** 'A'  'U'  'T'  'O'  */
    HI_U8 au8SubPage[TTX_MAX_SUBPAGENO] = {0};
    HI_U8 u8NumOfSubPg = TTX_MAX_SUBPAGENO;       /** Assume  the most NO  of subpage is 32*/
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;   /**  Draw char  param*/
    HI_UNF_TTX_FILLRECT_S stFillRectParam;
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = HI_NULL;

    pstCurrentPoint = pstContextHead;
    if (HI_NULL == pstCurrentPoint)
    {
        return;
    }

    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));


    memset(au8SubPage, '\0', TTX_MAX_SUBPAGENO);


    if (HI_TRUE == pstCurrentPoint->bTtxHiden)
    {
        return;
    }

    
    u8NumOfSubPg = TTX_MAX_SUBPAGENO;
    u32ColNum = pstCurrentPoint->u32ColumnNum;        
    
    s32Ret |= TTX_Data_GetSubpageNum(pstCurrentPoint->stCurPgAddr.u8MagazineNum, 
                                     pstCurrentPoint->stCurPgAddr.u8PageNum, au8SubPage, &u8NumOfSubPg);

    TTX_PARSE_LOCK();
    if (u8NumOfSubPg != 0)    /** Subpage be present*/
    {
        if (pstCurrentPoint->bTtxMix)
        {
            stFillRectParam.u32Color = 0x00000000;
        }
        else
        {
            stFillRectParam.u32Color = 0xff000000;
        }

        if (pstCurrentPoint->bNavigation)
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM + TTX_NAVIGATION_BAR, 0, TTX_SUBPAGE_BAR, u32ColNum);
        }
        else
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, 0, TTX_SUBPAGE_BAR, u32ColNum);
        }

        stFillRectParam.pstPageArea = &stPageArea;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID *) &stFillRectParam);

        for (j = 0; j < 4; j++)
        {
            TTX_Show_SetCharAttr(&stCharAttr, au8AutoChar[j], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                                 HI_UNF_TTX_CHARSET_G0, 0);
                                 
            stDrawCharParam.pstCharAttr = &stCharAttr;
            
            if (pstCurrentPoint->bNavigation)
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM + TTX_NAVIGATION_BAR, j + 1, 1, 1);
            }
            else
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, j + 1, 1, 1);
            }

            stDrawCharParam.pstPageArea = &stPageArea;
            
            if (pstCurrentPoint->bAutoPlay)
            {
                stDrawCharParam.u32Foreground = 0xff00ff00;
            }
            else
            {
                stDrawCharParam.u32Foreground = 0xffffffff;
            }

            if (pstCurrentPoint->bTtxMix)
            {
                stDrawCharParam.u32Background = 0x00000000;
            }
            else
            {
                stDrawCharParam.u32Background = 0xff000000;
            }

            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID *)&stDrawCharParam);
        }

        /**  Find start position */
        u8StartPosition = TTX_UpdateSubpage_FindStartPosition(pstCurrentPoint, au8SubPage);

        
        for (i = u8StartPosition, j = 0; i < u8StartPosition + 8; i++, j++)
        {
            if (au8SubPage[i % TTX_MAX_SUBPAGENO] != 0)    /** Draw subpage */
            {
                TTX_Show_SetCharAttr(&stCharAttr, au8SubPage[i % TTX_MAX_SUBPAGENO] / 10 + 0x30, HI_FALSE,
                                     HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                                     HI_UNF_TTX_CHARSET_G0, 0);
                                     
                stDrawCharParam.pstCharAttr = &stCharAttr;
                
                if (pstCurrentPoint->bNavigation)
                {
                    s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM + TTX_NAVIGATION_BAR, 8 + 4 * j, 1, 1);
                }
                else
                {
                    s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, 8 + 4 * j, 1, 1);
                }

                stDrawCharParam.pstPageArea = &stPageArea;
                
                if (au8SubPage[i % TTX_MAX_SUBPAGENO] == u16SubPage)
                {
                    stDrawCharParam.u32Foreground = 0xff00ff00;
                }
                else
                {
                    stDrawCharParam.u32Foreground = 0xffffffff;
                }

                if (pstCurrentPoint->bTtxMix)
                {
                    stDrawCharParam.u32Background = 0x00000000;
                }
                else
                {
                    stDrawCharParam.u32Background = 0xff000000;
                }

                s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID *)&stDrawCharParam);
                stPageArea.u32Column = 8 + 4 * j + 1;
                TTX_Show_SetCharAttr(&stCharAttr, au8SubPage[i % TTX_MAX_SUBPAGENO] % 10 + 0x30, HI_FALSE,
                                     HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                                     HI_UNF_TTX_CHARSET_G0, 0);
                s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID *)&stDrawCharParam);
            }
        }


        TTX_Update_SubpageNavigation(pstCurrentPoint, &stPageArea, u8NumOfSubPg, u8StartPosition);

        
        if (pstCurrentPoint->bNavigation)
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM + TTX_NAVIGATION_BAR, 0, TTX_SUBPAGE_BAR, u32ColNum);
        }
        else
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, 0, TTX_SUBPAGE_BAR, u32ColNum);
        }
        
        stRefrshLayerParam.pstPageArea = &stPageArea;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
    }

    TTX_PARSE_UNLOCK();

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_Parse_UpdateSubpage, Errors occur while call functions\n");
    }
}

/** The thread to update time , flash, show user's input and so on */

static HI_S32 TTX_OSD_Output(TTX_PAGE_CONTEXT_S *pstCurrentPoint, TTX_PAGE_S_PTR pstPage)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 u32Bit = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8* pu8Line = HI_NULL;
    HI_U32 u32PacketNUm = 0;
    HI_U32 u32LineLen = TTX_LINE_MEM_SIZE;
    
    HI_S64 s64CurrentPts = 0;
    HI_S64 s64MaxPts = 0;
    HI_S64 s64MinPts = 0;
    
    HI_HANDLE hDispalyHandle = HI_INVALID_HANDLE;
    TTX_MSG_ACTION_E enMsgAction = TTX_MSG_ACTION_BUTT;

    if ((HI_NULL == pstCurrentPoint) || (HI_NULL == pstPage))
    {
        return HI_FAILURE;
    }

    if (pstPage->u32PTS != 0)
    {
        while (pstCurrentPoint->bShowTaskStart)
        {
            if (-2 == TTX_PesQueue_IsMemTensity(pstCurrentPoint->pstPAGEQueue))
            {
                break;
            }

            if (HI_SUCCESS != TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_GETPTS, (HI_VOID*)&s64CurrentPts))
            {
                break;
            }

            s64MaxPts = (HI_S64)pstPage->u32PTS + pstCurrentPoint->u32MaxIntervalMs;
            s64MinPts = (HI_S64)pstPage->u32PTS - pstCurrentPoint->u32MaxIntervalMs;

            if ((s64CurrentPts < s64MinPts) || (s64CurrentPts > s64MaxPts))
            {
                break;            /** The discrepancy  of video pts and subtitle pts is too much */
            }
            else
            {
                s64MaxPts = (HI_S64)pstPage->u32PTS + 200;
                s64MinPts = (HI_S64)pstPage->u32PTS - 200;
                if (s64CurrentPts < s64MinPts)
                {
                    usleep(20 * 1000);   /** subtitle pts > video pts , continue  */
                    continue;
                }

                if ((s64CurrentPts > s64MinPts) && (s64CurrentPts <= s64MaxPts))
                {
                    break; /** Subtitle  and video  sync */
                }

                if (s64CurrentPts > s64MaxPts)
                {
                    break;    /** video pts >(subtitle pts +200ms),  handle next page */
                }
            }
        }
    }

    pstCurrentPoint->u32ValLine = pstPage->u32ValidLines;
    pstCurrentPoint->u32ExtValLineH = pstPage->u32ExtenValidLineH;
    pstCurrentPoint->u32ExtValLineL = pstPage->u32ExtenValidLineL;

    for (i = 0; i < TTX_ROW_NUM; i++)
    {
        for (j = 0; j < TTX_CMB_MAX_COL_NUM; j++)
        {
            pstCurrentPoint->u32aPageText[i][j] = 0xff;
        }
    }

    pstCurrentPoint->bHasP24 = HI_FALSE;
    pstCurrentPoint->bShowP24 = HI_FALSE;
    pstCurrentPoint->bBlackBgSubstitute = HI_FALSE;
    pstCurrentPoint->bHasP27_4 = HI_FALSE;
    pstCurrentPoint->bHasP27_5 = HI_FALSE;
    pstCurrentPoint->bHasP28_0 = HI_FALSE;
    pstCurrentPoint->bHasP28_1 = HI_FALSE;
    pstCurrentPoint->bHasP28_4 = HI_FALSE;

    memset(&pstCurrentPoint->stP28_0_Info, 0x00, sizeof(TTX_P28_0_M29_0_INFO_S));
    memset(&pstCurrentPoint->stMOTInfo, 0x00, sizeof(TTX_MOT_INFO_S));

    u32PacketNUm = 0;
    while (u32PacketNUm < TTX_MAX_LINES)      /**Decode  normal  packet   */
    {
        u32Bit = (pstCurrentPoint->u32ValLine >> u32PacketNUm) & 0x1;
        if (u32Bit)           /** Valid line */
        {
            pu8Line = pstPage->u8szLines[u32PacketNUm];
            TTX_ConvertLine(pu8Line, u32LineLen);            /**Convert a  packet  */
            s32Ret |= TTX_DecodeLine(pstCurrentPoint, pu8Line);       /**Decode a  packet  */
        }

        u32PacketNUm++;
    }

    u32PacketNUm = 0;
    while (u32PacketNUm < (TTX_PACKET26_NUM + TTX_PACKET27_NUM + TTX_PACKET28_NUM + TTX_PACKET29_NUM))
    {
        if (32 > u32PacketNUm)
        {
            u32Bit = (pstCurrentPoint->u32ExtValLineL >> u32PacketNUm) & 0x1;
        }
        else
        {
            u32Bit = (pstCurrentPoint->u32ExtValLineH >> (u32PacketNUm - 32)) & 0x1;
        }
        if (u32Bit)
        {
            pu8Line = pstPage->u8szExtenLines[u32PacketNUm];
            TTX_ConvertLine(pu8Line, u32LineLen);
            s32Ret |= TTX_DecodeLine(pstCurrentPoint, pu8Line);
        }

        u32PacketNUm++;
    }

    pstCurrentPoint->pstCurPage = pstPage;

    pstCurrentPoint->bProcessed = HI_FALSE;
    enMsgAction = TTX_MSG_ACTION_SHOWPAGE;     /** Send message to show a page */
    hDispalyHandle = 0x7f7f0000 | enMsgAction;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG, (HI_VOID *) &hDispalyHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_OSD_Output, Errors occur while call functions\n");
    }

    return s32Ret;
}

HI_VOID  TTX_Parse_UpdateTask(HI_VOID* pvData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_U32 u32TimeCount = 0;
    
    HI_U32 u32TTXSearchTime = 0;
    HI_BOOL bFirst = HI_TRUE;
    HI_U8 u8NumOfSubPg = TTX_MAX_SUBPAGENO;
    HI_HANDLE hDispalyHandle = HI_INVALID_HANDLE;    
    TTX_MSG_ACTION_E enMsgAction = TTX_MSG_ACTION_BUTT;
    
    TTX_PAGE_S* pstProPage = NULL;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = HI_NULL;
    
    TTX_SEGMENT_S stTtxSegment;
    HI_U8 au8szSubPage[TTX_MAX_SUBPAGENO];

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S  *)pvData;
    if (HI_NULL == pstCurrentPoint)         /** Invalid param , just return  */
    {
        return;
    }

    memset(&stTtxSegment, 0x0, sizeof(TTX_SEGMENT_S));
    memset(au8szSubPage, 0x0, sizeof(au8szSubPage));


    while (pstCurrentPoint->bStart)
    {
        if (pstCurrentPoint->bShowTaskStart)/** Receive  data and show it */
        {
            if (!pstCurrentPoint->bProcessed)/** The previous page not be processed, just wait*/
            {
                usleep(30 * 1000);
                continue;
            }
            if (HI_SUCCESS == TTX_PesQueue_De(pstCurrentPoint->pstPAGEQueue, &stTtxSegment))
            {
                pstProPage = (TTX_PAGE_S * )(HI_VOID *)stTtxSegment.pu8SegmentData;
                s32Ret |= TTX_OSD_Output(pstCurrentPoint, pstProPage);
            }
        }

        u32TimeCount++;
        usleep(50 * 1000);

        TTX_PARSE_LOCK();
        if (!(pstCurrentPoint->bTtxHiden) && (pstCurrentPoint->bUpdateTime) && (HI_FALSE == pstCurrentPoint->bTTXSearching))
        {
            /** Update time, near to every 1 second */
            if ((0 == (u32TimeCount % 20)) || bFirst)
            {
                bFirst = HI_FALSE;
                enMsgAction = TTX_MSG_ACTION_UPDATETIME;
                hDispalyHandle = 0x7f7f0000 | enMsgAction;
                s32Ret |= TTX_Show_CallBack( pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG, (HI_VOID * )&hDispalyHandle);
            }

            /**Update Flash  area, near to every 1 second  */
            if (0 == (u32TimeCount % 16))
            {
                if (pstCurrentPoint->bFlash && pstCurrentPoint->pFlaAreaHead)
                {
                    enMsgAction = TTX_MSG_ACTION_FLASH;
                    hDispalyHandle = 0x7f7f0000 | enMsgAction;
                    s32Ret |= TTX_Show_CallBack( pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG,
                                                (HI_VOID * )&hDispalyHandle);
                }
            }

            /**  Update user's input, every 200ms */
            if (0 == (u32TimeCount % 4))
            {
                if (TTX_InputNumIsOnGoing(&pstCurrentPoint->stInputNum) || pstCurrentPoint->bHold)
                {
                    enMsgAction = TTX_MSG_ACTION_INPUTNUMBER;
                    hDispalyHandle = 0x7f7f0000 | enMsgAction;
                    s32Ret |= TTX_Show_CallBack( pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG,
                                                 (HI_VOID * )&hDispalyHandle);
                }
            }

            /** Update received  subpage, every 300ms */
            if (0 == (u32TimeCount % 6))
            {
                u8NumOfSubPg = TTX_MAX_SUBPAGENO;
                s32Ret |= TTX_Data_GetSubpageNum(pstCurrentPoint->stCurPgAddr.u8MagazineNum,
                                                 pstCurrentPoint->stCurPgAddr.u8PageNum,
                                                 au8szSubPage, &u8NumOfSubPg);
                if (u8NumOfSubPg != 0)  /** Subpage be present*/
                {
                    enMsgAction = TTX_MSG_ACTION_UPDATESUBPAGE;
                    hDispalyHandle = 0x7f7f0000 | enMsgAction;
                    s32Ret |= TTX_Show_CallBack( pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG,
                                                 (HI_VOID * )&hDispalyHandle);
                }
            }
        }

        /** Invail input,show random number to  remaind user, every 200ms*/
        if (0 == (u32TimeCount % 4))
        {
            if (pstCurrentPoint->bInvalidReq)
            {
                enMsgAction = TTX_MSG_ACTION_INVALIDREQUEST;
                hDispalyHandle = 0x7f7f0000 | enMsgAction;
                s32Ret |= TTX_Show_CallBack( pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG,
                                             (HI_VOID * )&hDispalyHandle);
            }
        }

        if (HI_TRUE == pstCurrentPoint->bTTXSearching)
        {
            u32TTXSearchTime++;
            if(u32TTXSearchTime >= 120)/**6s*/
            {
                pstCurrentPoint->bTTXSearching = HI_FALSE;
                TTX_ShowPromptTimeOutMsg(pstCurrentPoint);
            }
        }
        else
        {
            u32TTXSearchTime = 0;
        }

        TTX_PARSE_UNLOCK();

        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_TTX("TTX_Parse_UpdateTask, Errors occur while call functions\n");
        }
    }
}

HI_S32 TTX_Show_Zoom(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_PAGEAREA_S * pstPageArea)
{
    TTX_PAGE_CONTEXT_S * pstCurrentPoint = pstContextHead;

    if (pstCurrentPoint->u32ZoomFlag != 0)
    {
        pstPageArea->u32RowCount *= 2;
    }

    if (pstCurrentPoint->u32ZoomFlag == 1)
    {
        pstPageArea->u32Row *= 2;
    }
    else if (pstCurrentPoint->u32ZoomFlag == 2)
    {
        pstPageArea->u32Row = (pstPageArea->u32Row - (TTX_ROW_NUM >> 1)) * 2;
    }

    return HI_SUCCESS;
}

HI_S32 TTT_Show_DrawChar(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_DRAWCAHR_S * pstDrawCharParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    s32Ret = TTX_Show_Zoom(pstContextHead, pstDrawCharParam->pstPageArea);

    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID *)pstDrawCharParam);

    return s32Ret;
}

HI_S32 TTT_Show_FillRect(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_FILLRECT_S * pstFillRectParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    s32Ret = TTX_Show_Zoom(pstContextHead, pstFillRectParam->pstPageArea);
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID *)pstFillRectParam);

    return s32Ret;
}

HI_S32 TTT_Show_Refresh(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_REFRESHLAYER_S* pstRefrshLayerParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    s32Ret = TTX_Show_Zoom(pstContextHead, pstRefrshLayerParam->pstPageArea);
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID *)pstRefrshLayerParam);

    return s32Ret;
}


