/*
@ H264/MPEG2/MPEG4 frame type detect source file

@ Author:

@ Created:

@ Modified:
*/

#include <linux/kernel.h>
#include <linux/string.h>
#include <pthread.h>

#include "bitstream.h"
#include "hi_mpi_avplay.h"
#include "hi_mpi_mem.h"
#include "hi_common.h"

#define  RET_OK         0
#define  RET_ERR       -1

#define  H264_NALU_TYPE_SLICE    1
#define  H264_NALU_TYPE_IDR      5
#define  H264_NALU_TYPE_SPS      7
#define  H264_NALU_TYPE_PPS      8

#define   H264_I_SLICE           0
#define   H264_P_SLICE           1
#define   H264_B_SLICE           2

typedef struct
{
    DF_CTX_S        *pDFCtx;
    pthread_mutex_t  Mutex;
}DF_CTX_INFO_S;

DF_CTX_INFO_S  g_DFCtx[DF_MAX_CTX_NUM] = {{HI_NULL, PTHREAD_MUTEX_INITIALIZER}};

static HI_S32 DetFrm_MPEG2(DF_CTX_S *pDFCtx);
static HI_S32 DetFrm_MPEG4(DF_CTX_S *pDFCtx);
static HI_S32 DetFrm_H264(DF_CTX_S *pDFCtx);
//static HI_S32 DetFrm_HEVC(HI_S32 InstIdx);
//static HI_S32 DetFrm_AVS(HI_S32 InstIdx);

#define HI_FATAL_AVPLAY(fmt...)     HI_FATAL_PRINT(HI_ID_AVPLAY, fmt)
#define HI_ERR_AVPLAY(fmt...)       HI_ERR_PRINT(HI_ID_AVPLAY, fmt)
#define HI_WARN_AVPLAY(fmt...)      HI_WARN_PRINT(HI_ID_AVPLAY, fmt)
#define HI_INFO_AVPLAY(fmt...)      HI_INFO_PRINT(HI_ID_AVPLAY, fmt)

#define DF_CTX_INST_LOCK(Id) \
    do \
    { \
        (HI_VOID)pthread_mutex_lock(&g_DFCtx[Id].Mutex); \
    } while(0)

#define DF_CTX_INST_UNLOCK(Id) \
    do \
    { \
        (HI_VOID)pthread_mutex_unlock(&g_DFCtx[Id].Mutex); \
    } while(0)

#define DF_CTX_INST_CHECK_LOCK(Id) \
    do \
    { \
        if (Id >= DF_MAX_CTX_NUM) \
        { \
            HI_ERR_AVPLAY("DFCtx %d is err\n", Id); \
            return RET_ERR; \
        } \
        DF_CTX_INST_LOCK(Id); \
        pDFCtx = g_DFCtx[Id].pDFCtx; \
        if (HI_NULL == pDFCtx) \
        { \
            HI_ERR_AVPLAY("para is null\n"); \
            DF_CTX_INST_UNLOCK(Id); \
            return RET_ERR; \
        } \
    } while(0)

#define  DF_ASSERT_RET( cond, else_print ) \
    do \
    { \
        if( !(cond) ) \
        { \
            HI_ERR_AVPLAY("frame_detect.c,L%d: %s\n", __LINE__, else_print ); \
            return RET_ERR; \
        } \
    }while(0)

#define BITS_2_BYTES(bitn)   (((bitn) + (7))>>(3))

HI_S32 DetFrm_Create(HI_HANDLE *hDFCtx, HI_S32 VidStd)
{
    HI_U32    Id;
    DF_CTX_S *pDFCtx = HI_NULL;

    switch( VidStd )
    {
        case STD_H264:
        case STD_MPEG2:
        case STD_MPEG4:
            break;
        default:
            HI_ERR_AVPLAY("para %d is invalid\n", VidStd);
            return RET_ERR;
    }

    for (Id = 0; Id < DF_MAX_CTX_NUM; Id++)
    {
        DF_CTX_INST_LOCK(Id);
        if (HI_NULL == g_DFCtx[Id].pDFCtx)
        {
            g_DFCtx[Id].pDFCtx = (DF_CTX_S *)HI_MALLOC(HI_ID_AVPLAY, sizeof(DF_CTX_S));
            if (HI_NULL == g_DFCtx[Id].pDFCtx)
            {
                DF_CTX_INST_UNLOCK(Id);
                HI_ERR_AVPLAY("DFCtx malloc failed\n");
                return RET_ERR;
            }

            memset(g_DFCtx[Id].pDFCtx, 0, sizeof(DF_CTX_S));

            DF_CTX_INST_UNLOCK(Id);

            break;
        }
        DF_CTX_INST_UNLOCK(Id);
    }

    if (Id >= DF_MAX_CTX_NUM)
    {
        HI_ERR_AVPLAY("DFCtx num is max\n");
        return RET_ERR;
    }

    DF_CTX_INST_LOCK(Id);

    pDFCtx = g_DFCtx[Id].pDFCtx;

    pDFCtx->eVidStd   = VidStd;
    pDFCtx->eCtxState = 1;

    *hDFCtx = Id;

    DF_CTX_INST_UNLOCK(Id);
    return RET_OK;
}

HI_S32 DetFrm_Destroy(HI_HANDLE hDFCtx)
{
    DF_CTX_S *pDFCtx = HI_NULL;

    DF_CTX_INST_CHECK_LOCK(hDFCtx);

    memset(pDFCtx, 0, sizeof(DF_CTX_S));

    HI_FREE(HI_ID_AVPLAY, pDFCtx);

    g_DFCtx[hDFCtx].pDFCtx = HI_NULL;

    DF_CTX_INST_UNLOCK(hDFCtx);

    return RET_OK;
}

HI_S32 DetFrm_Start(HI_HANDLE hDFCtx)
{
    HI_S32    Ret;
    DF_CTX_S *pDFCtx = HI_NULL;

    DF_CTX_INST_CHECK_LOCK(hDFCtx);

    if (pDFCtx->eCtxState != 1)
    {
        HI_ERR_AVPLAY("hDFCtx %d not create\n", hDFCtx);
        DF_CTX_INST_UNLOCK(hDFCtx);
        return RET_ERR;
    }

    switch( pDFCtx->eVidStd )
    {
        case STD_H264:
            Ret = DetFrm_H264(pDFCtx);
            break;
        case STD_MPEG2:
            Ret = DetFrm_MPEG2(pDFCtx);
            break;
        case STD_MPEG4:
            Ret = DetFrm_MPEG4(pDFCtx);
            break;
        default:
            Ret = RET_ERR;
            break;
    }

    DF_CTX_INST_UNLOCK(hDFCtx);
    return Ret;
}

HI_S32 DetFrm_Stop(HI_HANDLE hDFCtx)
{
    DF_CTX_S *pDFCtx = HI_NULL;

    DF_CTX_INST_CHECK_LOCK(hDFCtx);

    if (pDFCtx->eCtxState == 0)
    {
        HI_INFO_AVPLAY("hDFCtx %d already stop\n", hDFCtx);
        DF_CTX_INST_UNLOCK(hDFCtx);
        return RET_OK;
    }

    pDFCtx->eCtxState = 0;

    DF_CTX_INST_UNLOCK(hDFCtx);
    return RET_OK;
}

HI_S32 DetFrm_Check(HI_HANDLE hDFCtx)
{
    DF_CTX_S *pDFCtx = HI_NULL;
    HI_S32    FrmType;

    DF_CTX_INST_CHECK_LOCK(hDFCtx);

    FrmType = pDFCtx->s32NewFrmType;

    DF_CTX_INST_UNLOCK(hDFCtx);
    return FrmType;
}

HI_S32 DetFrm_GetDFCtx(HI_HANDLE hDFCtx, DF_CTX_S **pCtx)
{
    DF_CTX_S *pDFCtx = HI_NULL;

    if (pCtx == HI_NULL)
    {
        HI_ERR_AVPLAY("para is invalid\n");
        return RET_ERR;
    }

    DF_CTX_INST_CHECK_LOCK(hDFCtx);

    *pCtx = pDFCtx;

    DF_CTX_INST_UNLOCK(hDFCtx);

    return RET_OK;
}

static HI_U32 data_ue_v( HI_U32 Code, HI_U32 *BitNum )
{
    HI_U32 Val;
    HI_U32 zeros;

    zeros = (HI_S32)ZerosMS_32(Code);
    if (zeros < 16)
    {
        *BitNum = (zeros<<1) + 1;
        Val = (Code>>(32-(*BitNum))) - 1;
    }
    else
    {
        *BitNum = 0;
        Val = 0xffffffff;
    }

    return Val;
}

static HI_S32 DetFrm_MPEG2(DF_CTX_S *pDFCtx)
{
    HI_S32    Ret = RET_ERR;
    DF_CTX_S *pstCtx = pDFCtx;
    HI_U8    *pu8Data = pstCtx->pNalAddr;
    HI_U8     picture_coding_type;

    //clear pre frame info
    if(pstCtx->s32NewFrmFlag == 1)
    {
        pstCtx->s32NewFrmFlag = 0;
        pstCtx->s32NewFrmType = FRAME_TYPE_N;
    }

    if(pstCtx->s32CurFrmType != FRAME_TYPE_N)
    {
        //pre frame end and next frame start
        if( 0xb3==pu8Data[0] || 0x00==pu8Data[0] )
        {
            //output one frame info
            pstCtx->s32NewFrmFlag = 1;
            pstCtx->s32NewFrmType = pstCtx->s32CurFrmType;
            pstCtx->s32CurFrmType = FRAME_TYPE_N;
            Ret = RET_OK;
        }
    }

    if( 0xb3 == pu8Data[0] )  /*! sequence header */
    {
    }
    else if (0x00 == pu8Data[0])  /*! picture header */
    {
        picture_coding_type = (pu8Data[2] >> 3) & 7;

        switch( picture_coding_type )
        {
            case 1:
                pstCtx->s32CurFrmType = FRAME_TYPE_I;
                break;
            case 2:
                pstCtx->s32CurFrmType = FRAME_TYPE_P;
                break;
            case 3:
                pstCtx->s32CurFrmType = FRAME_TYPE_B;
                break;
            default:
                pstCtx->s32CurFrmType = FRAME_TYPE_N;
                break;
        }
    }

    return Ret;
}

static HI_S32 DetFrm_MPEG4(DF_CTX_S *pDFCtx)
{
    HI_S32    Ret = RET_ERR;
    DF_CTX_S *pstCtx = pDFCtx;
    HI_U8    *pu8Data = pstCtx->pNalAddr;
    HI_U8     picture_coding_type;

    //clear pre frame info
    if(pstCtx->s32NewFrmFlag == 1)
    {
        pstCtx->s32NewFrmFlag = 0;
        pstCtx->s32NewFrmType = FRAME_TYPE_N;
    }

    if(pstCtx->s32CurFrmType != FRAME_TYPE_N)
    {
        //pre frame end and next frame start
        if( 0xb2 != pu8Data[0] )
        {
            //output one frame info
            pstCtx->s32NewFrmFlag = 1;
            pstCtx->s32NewFrmType = pstCtx->s32CurFrmType;
            pstCtx->s32CurFrmType = FRAME_TYPE_N;
            Ret = RET_OK;
        }
    }

    if( 0xb0 == pu8Data[0] )    /* Visual_Object_Sequence */
    {
    }
    else if ( 0xb5 == pu8Data[0] )  /* b5, Visual_Object */
    {
    }
    else if ( pu8Data[0] <= 0x1f )  /* Video_Object: 0x00 ~ 0x1f */
    {
    }
    else if ( pu8Data[0] >= 0x20 && pu8Data[0] <= 0x2f )  /* Video_Object_Layer */
    {
    }
    else if (0xb6 == pu8Data[0])  /* VOP: picture header */
    {
        picture_coding_type = (pu8Data[1] >> 6) & 3;

        switch( picture_coding_type )
        {
            case 0:
                pstCtx->s32CurFrmType = FRAME_TYPE_I;
                break;
            case 1:
                pstCtx->s32CurFrmType = FRAME_TYPE_P;
                break;
            case 2:
                pstCtx->s32CurFrmType = FRAME_TYPE_B;
                break;
            default:
                pstCtx->s32CurFrmType = FRAME_TYPE_N;
                break;
        }
    }
	return Ret;
}

#if 0
static HI_S32 DetFrm_H264SPS( HI_U8 *pu8Data, HI_S32 s32DataLen, HI_S32 *p32SpsID )
{
    HI_U32  u32Code, u32Val, u32BitNum;

    u32Code = ((HI_U32)pu8Data[4]<<24) | ((HI_U32)pu8Data[5]<<16) |
        ((HI_U32)pu8Data[6]<<8) | ((HI_U32)pu8Data[7]);

    u32Val = data_ue_v( u32Code, &u32BitNum );
    DF_ASSERT_RET( u32Val<=31, "SPS ID out of range!" );
    DF_ASSERT_RET( ((u32BitNum+7)>>3)<=(HI_U32)(s32DataLen-4), "data NOT enough for SPS" );

    *p32SpsID = (HI_S32)u32Val;
    return RET_OK;
}

static HI_S32  DetFrm_H264PPS( HI_U8 *pu8Data, HI_S32 s32DataLen, HI_S32 *ps32SPSID, HI_S32 *ps32PPSID )
{
    HI_S32 s32Code;
    HI_U32 u32Val, u32BitNum;
    BS Bs;

    BsInit(&Bs, pu8Data, s32DataLen);
    BsSkip(&Bs,8);

    /* pic_parameter_set_id */
    s32Code = BsShow(&Bs, 32);
    u32Val = data_ue_v((HI_U32)s32Code,&u32BitNum );
    DF_ASSERT_RET( u32Val<=255, "PPS ID out of range!" );
    DF_ASSERT_RET((HI_S32)BITS_2_BYTES((HI_U32)BsPos(&Bs))<= s32DataLen, "data NOT enough for PPS" );
    *ps32PPSID = (HI_S32)u32Val;
    BsSkip(&Bs, (HI_S32)u32BitNum );

    /* seq_parameter_set_id */
    s32Code = BsShow(&Bs, 32);
    u32Val = data_ue_v( (HI_U32)s32Code, &u32BitNum );
    DF_ASSERT_RET( u32Val<=31, "SPS ID out of range!" );
    DF_ASSERT_RET( (HI_S32)BITS_2_BYTES((HI_U32)BsPos(&Bs))<=s32DataLen, "data NOT enough for SPS" );
    *ps32SPSID = (HI_S32)u32Val;

    return RET_OK;
}
#endif

static HI_S32  DetFrm_H264Slice(
                HI_U8  *pu8Data,
                HI_SIZE_T DataLen,
                HI_S32 *ps32FirstMBInSlice,
                HI_S32 *ps32SliceType,
                HI_S32 *ps32PPSId
    )
{
    HI_S32  s32Code;
    HI_U32  u32Val, u32BitNum;
    static  HI_S32  SlcType2FrmType[10] = {1,2,0,1,0,1,2,0,1,0};
    BS Bs;

    BsInit(&Bs, pu8Data, DataLen);
    BsSkip(&Bs,8);

    /* first_mb_in_slice */
    s32Code = BsShow( &Bs, 32 );
    u32Val = data_ue_v((HI_U32)s32Code, &u32BitNum );
    DF_ASSERT_RET( u32Val<=8192, "'first_mb_in_slice' out of range!" );
    DF_ASSERT_RET( (HI_S32)BITS_2_BYTES((HI_U32)BsPos(&Bs))<=DataLen, "data NOT enough for 'first_mb_in_slice'" );
    *ps32FirstMBInSlice = (HI_S32)u32Val;
    BsSkip( &Bs, (HI_S32)u32BitNum );

    /* slice_type */
    s32Code = BsShow( &Bs, 32 );
    u32Val = data_ue_v((HI_U32)s32Code, &u32BitNum );
    DF_ASSERT_RET( (u32Val<=2 || (u32Val>=5&&u32Val<=7) ), "'slice_type' out of range!" );
    DF_ASSERT_RET( (HI_S32)BITS_2_BYTES((HI_U32)BsPos(&Bs))<=DataLen, "data NOT enough for 'slice_type'" );
    *ps32SliceType = (HI_S32)SlcType2FrmType[u32Val];
    BsSkip( &Bs, (HI_S32)u32BitNum );

    /* pic_parameter_set_id */
    s32Code = BsShow( &Bs, 32 );
    u32Val = data_ue_v( (HI_U32)s32Code, &u32BitNum );
    DF_ASSERT_RET( (u32Val<=255), "'pic_parameter_set_id' out of range!" );
    DF_ASSERT_RET( (HI_S32)BITS_2_BYTES((HI_U32)BsPos(&Bs))<=DataLen, "data NOT enough for 'pic_parameter_set_id'" );
    *ps32PPSId = (HI_S32)u32Val;

    return RET_OK;
}

HI_S32 DetFrm_H264(DF_CTX_S *pDFCtx)
{
    HI_S32    Ret = RET_ERR;
    HI_S32    NalUnitType;
    HI_S32    s32PpsID,s32FirstMb, s32SliceType;
    DF_CTX_S *pstCtx = pDFCtx;
    HI_U8    *pu8Data = pstCtx->pNalAddr;
    HI_SIZE_T DataLen = pstCtx->NalLen;

    if (0 != (pu8Data[0] & 0x80) )
    {
        return RET_ERR;
    }

    //clear prev frame info
    if (pstCtx->s32NewFrmFlag == 1)
    {
        pstCtx->s32NewFrmFlag = 0;
        pstCtx->s32NewFrmType = FRAME_TYPE_N;
    }

    NalUnitType = (HI_S32)(pu8Data[0] & 31);

    if (H264_NALU_TYPE_IDR == NalUnitType || H264_NALU_TYPE_SLICE == NalUnitType )
    {
        if( RET_OK == DetFrm_H264Slice(pu8Data, DataLen, &s32FirstMb, &s32SliceType, &s32PpsID) )
        {
            if(s32FirstMb == 0)
            {
                //detect new frame start, and process previous frame
                if(pstCtx->s32CurFrmType != FRAME_TYPE_N)
                {
                    pstCtx->s32NewFrmFlag = 1;
                    pstCtx->s32NewFrmType = pstCtx->s32CurFrmType;
                    pstCtx->s32CurFrmType = FRAME_TYPE_N;
                    Ret = RET_OK;
                }
            }

            //update frame type
            if(s32SliceType == H264_B_SLICE)
            {
                pstCtx->s32CurFrmType = FRAME_TYPE_B;
            }
            else if(s32SliceType == H264_P_SLICE)
            {
                if(pstCtx->s32CurFrmType != FRAME_TYPE_B)
                {
                    pstCtx->s32CurFrmType = FRAME_TYPE_P;
                }
            }
            else if(s32SliceType == H264_I_SLICE)
            {
                if(pstCtx->s32CurFrmType == FRAME_TYPE_N)
                {
                    pstCtx->s32CurFrmType = FRAME_TYPE_I;
                }
            }
            else
            {
                pstCtx->s32CurFrmType = FRAME_TYPE_N;
            }
        }
    }

    return Ret;
}



