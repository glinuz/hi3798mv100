#ifndef _H263_SOFT_DECODER_ADAPTOR_HEADER_
#define _H263_SOFT_DECODER_ADAPTOR_HEADER_

#ifdef __cplusplus
extern "C" {
#endif

#include "vfmw.h"
//#include "basedef.h"
#include "public.h"
#include "syn_cmn.h"
#include "imedia_viddec.h"


/*######################################################################
    constant definition
  ######################################################################*/
#define  iMediaSDEC_OK             0
#define  iMediaSDEC_ERR           -1
#define  iMediaSDEC_NOT_DEC       -2

#define  iMediaSDEC_NO_SC         -100

#define  iMediaSDEC_MAX_IMAGE_NUM  48

typedef SINT32 iMediaSDEC_INST_HANDLE;

#define  iMediaSDEC_VIR_2_PHY(vir)  (MEM_Vir2Phy(vir))
#define  iMediaSDEC_PHY_2_VIR(phy)  (MEM_Phy2Vir(phy))

#define  macro_iMediaSoftDEC_Create(pCtx, pstInstMem, eVidStd) \
    (g_SdecFunc.pfun_iMediaSoftDEC_Create? g_SdecFunc.pfun_iMediaSoftDEC_Create(pCtx, pstInstMem, eVidStd): iMediaSDEC_ERR)
#define  macro_iMediaSoftDEC_Destroy(pCtx) \
    (g_SdecFunc.pfun_iMediaSoftDEC_Destroy? g_SdecFunc.pfun_iMediaSoftDEC_Destroy(pCtx): iMediaSDEC_ERR)
#define  macro_iMediaSoftDEC_Init(pCtx, pstExtraData) \
    (g_SdecFunc.pfun_iMediaSoftDEC_Init? g_SdecFunc.pfun_iMediaSoftDEC_Init(pCtx, pstExtraData): iMediaSDEC_ERR)
#define  macro_iMediaSoftDEC_Decode(pCtx, pPacket) \
    (g_SdecFunc.pfun_iMediaSoftDEC_Decode? g_SdecFunc.pfun_iMediaSoftDEC_Decode(pCtx, pPacket): iMediaSDEC_ERR)
#define  macro_iMediaSoftDEC_RecycleImage(pCtx, img_id) \
    (g_SdecFunc.pfun_iMediaSoftDEC_RecycleImage? g_SdecFunc.pfun_iMediaSoftDEC_RecycleImage(pCtx, img_id): iMediaSDEC_ERR)
#define  macro_iMediaSoftDEC_GetRemainImg(pCtx) \
    (g_SdecFunc.pfun_iMediaSoftDEC_GetRemainImg? g_SdecFunc.pfun_iMediaSoftDEC_GetRemainImg(pCtx): iMediaSDEC_ERR)
#define  macro_iMediaSoftDEC_GetImageBuffer(pCtx) \
    (g_SdecFunc.pfun_iMediaSoftDEC_GetImageBuffer? g_SdecFunc.pfun_iMediaSoftDEC_GetImageBuffer(pCtx): iMediaSDEC_ERR)

/*######################################################################
    struct & enum
  ######################################################################*/
typedef struct
{
    SINT32   ChanID;
    IMEDIA_CODEC_CTX   CodecInstHandle;
    VID_STD_E  eVidStd;
    VDEC_CHAN_CAP_LEVEL_E eChanCapLevel;
    UADDR    InstMemPhyAddr;
    UINT8    *pu8InstMemVirAddr;
    UINT32   InstMemLength;

    STRU_IVIDEO_PICTURE  stVidPics[iMediaSDEC_MAX_IMAGE_NUM];
    IMAGE    stImgs[iMediaSDEC_MAX_IMAGE_NUM];
    UINT32   ImageUsedFlag[iMediaSDEC_MAX_IMAGE_NUM];

    SYNTAX_EXTRA_DATA_S  *pstExtraData;
    IMAGE_VO_QUEUE  ImageQue;
} iMediaSDEC_CTX_S;

typedef struct
{
    SINT32 (*pfun_iMediaSoftDEC_Create)(iMediaSDEC_CTX_S  *pCtx, MEM_DESC_S *pstInstMem, VID_STD_E eVidStd);
    SINT32 (*pfun_iMediaSoftDEC_Destroy)(iMediaSDEC_CTX_S  *pCtx);
    SINT32 (*pfun_iMediaSoftDEC_Init)(iMediaSDEC_CTX_S  *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
    SINT32 (*pfun_iMediaSoftDEC_Decode)(iMediaSDEC_CTX_S  *pCtx, DEC_STREAM_PACKET_S *pPacket);
    SINT32 (*pfun_iMediaSoftDEC_RecycleImage)(iMediaSDEC_CTX_S  *pCtx, UINT32 img_id);
    SINT32 (*pfun_iMediaSoftDEC_GetRemainImg)(iMediaSDEC_CTX_S  *pCtx);
    SINT32 (*pfun_iMediaSoftDEC_GetImageBuffer)(iMediaSDEC_CTX_S  *pCtx);
} iMediaSDEC_FUNC_S;


/*######################################################################
    functions
  ######################################################################*/
SINT32 iMediaSoftDEC_Create(iMediaSDEC_CTX_S  *pCtx, MEM_DESC_S *pstInstMem, VID_STD_E eVidStd);
SINT32 iMediaSoftDEC_Destroy(iMediaSDEC_CTX_S  *pCtx);
SINT32 iMediaSoftDEC_Init(iMediaSDEC_CTX_S  *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
SINT32 iMediaSoftDEC_Decode(iMediaSDEC_CTX_S  *pCtx, DEC_STREAM_PACKET_S *pPacket);
SINT32 iMediaSoftDEC_RecycleImage(iMediaSDEC_CTX_S  *pCtx, UINT32 img_id);
SINT32 iMediaSoftDEC_GetRemainImg(iMediaSDEC_CTX_S  *pCtx);
SINT32 iMediaSoftDEC_GetImageBuffer(iMediaSDEC_CTX_S  *pCtx);


#ifdef __cplusplus
}
#endif


#endif



