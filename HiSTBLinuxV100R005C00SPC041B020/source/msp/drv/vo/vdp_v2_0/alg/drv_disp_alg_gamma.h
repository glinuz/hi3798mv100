
#ifndef __DRV_DISP_ALG_GMM_H__
#define __DRV_DISP_ALG_GMM_H__
#include "hi_type.h"
#include "hi_drv_mmz.h"

#define GMM_COEF_NUM   257
#define CoefCntNum     39*4
#define GMM_COEF_MODE  4

#define VouBitValue(a)	(a)

/*
typedef struct
{
    HI_U32	bits_0	 :	10	;
    HI_U32	bits_1	 :	10	;
    HI_U32	bits_2	 :	10	;
    HI_U32	bits_32	 :	2	;

    HI_U32	bits_38	 :	8	;
    HI_U32	bits_4	 :	10	;
    HI_U32	bits_5	 :	10	;
    HI_U32	bits_64	 :	4	;

    HI_U32	bits_66	 :	6	;
    HI_U32	bits_7	 :	10	;
    HI_U32	bits_8	 :	10	;
    HI_U32	bits_96	 :	6	;

    HI_U32	bits_94	 :	4	;
    HI_U32	bits_10	 :	10	;
    HI_U32	bits_11	 :	10	;
    HI_U32	bits_12B :	5	;
    HI_U32	bits_13	 :	3	;
} GMM_COEF_BIT_S_ODD;//ÆæÊý

typedef struct
{
    HI_U32	bits_0T	 :	5	;
    HI_U32	bits_1	 :	10	;
    HI_U32	bits_2	 :	10	;
    HI_U32	bits_37	 :	7	;

    HI_U32	bits_33	 :	3	;
    HI_U32	bits_4	 :	10	;
    HI_U32	bits_5	 :	10	;
    HI_U32	bits_69	 :	9	;

    HI_U32	bits_61	 :	1	;
    HI_U32	bits_7	 :	10	;
    HI_U32	bits_8	 :	10	;
    HI_U32	bits_9	 :	10	;
    HI_U32	bits_101 :	1	;

    HI_U32	bits_109 :	9	;
    HI_U32	bits_11	 :	10	;
    HI_U32	bits_12	 :	10	;
    HI_U32	bits_13	 :	3	;
} GMM_COEF_BIT_S_EVEN; //Å¼Êý
*/

typedef struct
{
    //enen first
    HI_U32	E_bits_0T	 :	5	;
    HI_U32	E_bits_1	 :	10	;
    HI_U32	E_bits_2	 :	10	;
    HI_U32	E_bits_37	 :	7	;

    HI_U32	E_bits_33	 :	3	;
    HI_U32	E_bits_4	 :	10	;
    HI_U32	E_bits_5	 :	10	;
    HI_U32	E_bits_69	 :	9	;

    HI_U32	E_bits_61	 :	1	;
    HI_U32	E_bits_7	 :	10	;
    HI_U32	E_bits_8	 :	10	;
    HI_U32	E_bits_9	 :	10	;
    HI_U32	E_bits_101   :	1	;

    HI_U32	E_bits_109   :  9	;
    HI_U32	E_bits_11	 :	10	;
    HI_U32	E_bits_12	 :	10	;
    HI_U32	E_bits_13	 :	3	;

    //odd after
    HI_U32	O_bits_0	 :	10	;
    HI_U32	O_bits_1	 :	10	;
    HI_U32	O_bits_2	 :	10	;
    HI_U32	O_bits_32	 :	2	;

    HI_U32	O_bits_38	 :	8	;
    HI_U32	O_bits_4	 :	10	;
    HI_U32	O_bits_5	 :	10	;
    HI_U32	O_bits_64	 :	4	;

    HI_U32	O_bits_66	 :	6	;
    HI_U32	O_bits_7	 :	10	;
    HI_U32	O_bits_8	 :	10	;
    HI_U32	O_bits_96	 :	6	;

    HI_U32	O_bits_94	 :	4	;
    HI_U32	O_bits_10	 :	10	;
    HI_U32	O_bits_11	 :	10	;
    HI_U32	O_bits_12B   :	5	;
    HI_U32	O_bits_13	 :	3	;
} GMM_COEF_BIT_S;

typedef struct
{
    HI_U32          u32Size;
    GMM_COEF_BIT_S  stBit[CoefCntNum];
} GMM_COEF_BITARRAY_S;

typedef struct
{
    HI_U32 u32StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
}ALG_BUFFER_S;

typedef enum
{
    GMM_MODE_0         = 0,
    GMM_MODE_1            ,
    GMM_MODE_2            ,
    GMM_MODE_3            ,
    GMM_MODE_CLOSE,
    GMM_MODE_BUTT

} ALG_GMM_MODE_E;

typedef enum
{
    GMM_CS_RGB         = 0,
    GMM_CS_YUV            ,
    GMM_CS_MODE_BUTT

} ALG_GMM_CS_MODE_E;

typedef struct
{

    HI_U32 u32GmmCoefAddrRGB0;
    HI_U32 u32GmmCoefAddrRGB1;
    HI_U32 u32GmmCoefAddrRGB2;
    HI_U32 u32GmmCoefAddrRGB3;

    HI_U32 u32GmmCoefAddrYUV0;
    HI_U32 u32GmmCoefAddrYUV1;
    HI_U32 u32GmmCoefAddrYUV2;
    HI_U32 u32GmmCoefAddrYUV3;

} ALG_GMM_COEF_ADDR_S;

typedef struct
{
    ALG_BUFFER_S stMBufRGB;
    ALG_BUFFER_S stMBufYUV;
    ALG_GMM_COEF_ADDR_S stGmmCoefAddr;
} ALG_GMM_MEM_S;


typedef struct
{
    HI_BOOL bGmmEn;
    HI_U32 u32GmmCoefAddr; // coef address
} ALG_GMM_RTL_PARA_S;

typedef struct
{
    HI_BOOL bGmmEn;
    ALG_GMM_MODE_E  enGmmMode;
    ALG_GMM_CS_MODE_E enCSMode;
} ALG_GMM_DRV_PARA_S;


HI_S32 ALG_GmmInit(ALG_GMM_MEM_S* pstGmmCoefMemid);
HI_S32 ALG_GmmDeInit(ALG_GMM_MEM_S* pstGmmCoefMem);
HI_S32 UpdateGmmCoef(ALG_GMM_MEM_S* pstGmmCoefMem, const HI_U16* pu16GmmCoef, ALG_GMM_CS_MODE_E enCSMode);
HI_S32 GetGmmCoef(ALG_GMM_MEM_S* pstGmmCoefMem,  HI_U16* pu16GmmCoef,  ALG_GMM_CS_MODE_E enCSMode);
HI_S32 ALG_GmmThdSet(ALG_GMM_MEM_S* pstGmmCoefMem, ALG_GMM_DRV_PARA_S* pstGmmDrvPara, ALG_GMM_RTL_PARA_S* pstGmmRtlPara);


#endif
