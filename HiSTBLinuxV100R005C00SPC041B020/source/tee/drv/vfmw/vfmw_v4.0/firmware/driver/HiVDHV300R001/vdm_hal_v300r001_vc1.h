/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAZ8tsS4we+PBWC6a/UNlzCWIaw+Ujkv9NAY+as0
fg7WZPJ30hFwkDWII+4y8WBbflMLIaHBsMttdR+445yVcqUJ8N6MpGma5lryiyorqDxbYA6B
HpWihuaMVn72hJiAq9F4IpcXwEEnbuOOEHcaXKWFLibfvUV2DIyoewaDQAUlWw==*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/*********************************************************************** 
*
* Copyright (c) 2009 Hisilicon - All Rights Reserved
*
* File: $vdm_hal_mpeg2.h$
* Date: $2009/09/01$
* Revision: $v1.0$
* Purpose: HiVDHV100 IP MPEG2 VDM HAL Module.
*
* Change History:
*
* Date       Author            Change
* ======     ======            ======
* 20090901   q45134            Original
*
************************************************************************/

#ifndef __VDM_HAL_V300R001_VC1_HEADER__
#define __VDM_HAL_V300R001_VC1_HEADER__

//VC1 msg down
typedef union
{
    struct
    {
        USIGN ptype      : 2;
        USIGN Reserved_2 : 2;
        USIGN profile    : 2;
        USIGN Reserved_1 : 8;
        USIGN fcm        : 2;
        USIGN Reserved_0 : 16;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D0;
typedef union
{
    struct
    {
        USIGN mbwidth    : 8;
        USIGN Reserved_0 : 8;
        USIGN mbheight   : 8;
        USIGN Reserved_1 : 8;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D1;

typedef union
{
    struct
    {
        USIGN loopfilter  : 1;
        USIGN issecondfld : 1;
        USIGN curparity   : 1;
        USIGN numref      : 1;
        USIGN fwd_fcm     : 2;
        USIGN bwd_fcm     : 2;
        USIGN Reserved_0  : 24;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D2;

typedef union
{
    struct
    {
        USIGN rndctrl    : 1;
        USIGN fastuvmc   : 1;
        USIGN overlap    : 1;
        USIGN Reserved_1 : 1;
        USIGN condover   : 2;
        USIGN Reserved_0 : 26;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D3;

typedef union
{
    struct
    {
        USIGN pquant     : 5;
        USIGN Reserved_3 : 3;
        USIGN pqindex    : 5;
        USIGN Reserved_2 : 3;
        USIGN altpquant  : 5;
        USIGN Reserved_1 : 3;
        USIGN halfqp     : 1;
        USIGN Reserved_0 : 7;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D4;

typedef union
{
    struct
    {
        USIGN buniform   : 1;
        USIGN usealtqp   : 1;
        USIGN dquant     : 2;
        USIGN Reserved_4 : 4;
        USIGN dqprofile  : 2;
        USIGN Reserved_3 : 2;
        USIGN dqbilevel  : 1;
        USIGN Reserved_2 : 1;
        USIGN dquantfrm  : 1;
        USIGN Reserved_1 : 1;
        USIGN quantmode  : 4;
        USIGN Reserved_0 : 12;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D5;

typedef union
{
    struct
    {
        USIGN mvmode     : 3;
        USIGN Reserved_1 : 1;
        USIGN mvmode2    : 2;
        USIGN Reserved_0 : 26;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D6;

typedef union
{
    struct
    {
        USIGN curishalfpel : 1;
        USIGN colishalfpel : 1;
        USIGN mvrange      : 2;
        USIGN refdist      : 5;
        USIGN Reserved_1   : 3;
        USIGN dmvrange     : 2;
        USIGN reffiled     : 1;
        USIGN Reserved_0   :17;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D7;

typedef union
{
    struct
    {
        USIGN transdctab  : 1;
        USIGN vstransform : 1;
        USIGN ttmbf       : 1;
        USIGN Reserved_1  : 1;
        USIGN transacfrm  : 2;
        USIGN transacfrm2 : 2;
        USIGN ttfrm       : 2;
        USIGN Reserved_0  : 22;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D8;

typedef union
{
    struct
    {
        USIGN forwardmbmodeisskipraw : 1;
        USIGN directmbmodeisskipraw  : 1;
        USIGN mvtypembmodeisskipraw  : 1;
        USIGN fieldtxmodeisskipraw   : 1;
        USIGN skipmbmodeisskipraw    : 1;
        USIGN acpredmodeisskipraw    : 1;
        USIGN overflagsmodeisskipraw : 1;
        USIGN Reserved_0             : 25;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D9;

typedef union
{
    struct
    {
        USIGN mvtab      : 3;
        USIGN Reserved_2 : 5;
        USIGN cbptab     : 3;
        USIGN Reserved_1 : 5;
        USIGN bfraction  : 7;
        USIGN Reserved_0 : 9;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D10;

typedef union
{
    struct
    {
        USIGN mbmodetab    : 3;
        USIGN Reserved_3   : 1;
        USIGN twomvbptab   : 2;
        USIGN Reserved_2   : 2;
        USIGN fourmvbptab  : 2;
        USIGN Reserved_1   : 2;
        USIGN fourmvswtich : 1;
        USIGN Reserved_0   : 19;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D11;

typedef union
{
    struct
    {
        USIGN ScaleFactor   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D12;

typedef union
{
    struct
    {
        USIGN FRFD   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D13;

typedef union
{
    struct
    {
        USIGN BRFD   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D14;

typedef union
{
    struct
    {
        USIGN totalslicenum   : 16;
        USIGN Reserved_0      : 4;
        USIGN rangemapyflag   : 1;
        USIGN rangemapy       : 3;
        USIGN rangemapuvflag  : 1;
        USIGN rangemapuv      : 3;
        USIGN Reserved_1      : 4;
    } bits;
    UINT32 u32;
}VC1DNMSG_V300R001_D15;

typedef union
{
    struct
    {
        USIGN CurFrmImgLuma2dPhyAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D16;

typedef union
{
    struct
    {
        USIGN FwdRefAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D17;

typedef union
{
    struct
    {
        USIGN BwdRefAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D18;

typedef union
{
    struct
    {
        USIGN CurrColmbAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D19;

typedef union
{
    struct
    {
        USIGN BwdColmbAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D20;

typedef union
{
    struct
    {
        USIGN SedTopAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D21;

typedef union
{
    struct
    {
        USIGN PmvTopAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D22;

typedef union
{
    struct
    {
        USIGN ItransTopAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D23;

typedef union
{
    struct
    {
        USIGN DblkTopAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D24;

typedef union
{
    struct
    {
        USIGN BitplaneInfoAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D25;

typedef union
{
    struct
    {
        USIGN IntensityConvTabPhyAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D26;

typedef union
{
	struct
	{
		USIGN DispPicWidth  : 16;
		USIGN DispPicHeight : 16;
	} bits;
	UINT32 u32;
} VC1DNMSG_V300R001_D27;

typedef union
{
    struct
    {
        USIGN Reserved_0   : 20;
        USIGN RANGEREDFRM  : 1;
        USIGN RANGEREDFRM0 : 1;
        USIGN RANGEREDFRM1 : 1;
        USIGN Reserved_1   : 1;
        USIGN PostCresent  : 1;
        USIGN CodecVersion  : 3;
        USIGN Reserved_2   : 4;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D28;


typedef union
{
    struct
    {
        USIGN BPD_STRIDE   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D29;

typedef union
{
    struct
    {
        USIGN MVTYPEMB_ADDR   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D30;

typedef union
{
    struct
    {
        USIGN SKIPMB_ADDR   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D31;

typedef union
{
    struct
    {
        USIGN DIRECTMB_ADDR   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D32;

typedef union
{
    struct
    {
        USIGN ACPRED_ADDR   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D33;

typedef union
{
    struct
    {
        USIGN OVERFLAGS_ADDR   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D34;

typedef union
{
    struct
    {
        USIGN FIELDTX_ADDR   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D35;

typedef union
{
    struct
    {
        USIGN FORWARDMB_ADDR   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D36;

typedef union
{
    struct
    {
        USIGN SliceInfoPhyAddr   : 32;
    } bits;
    UINT32 u32;
} VC1DNMSG_V300R001_D63;


typedef struct
{
    VC1DNMSG_V300R001_D0     d0;
    VC1DNMSG_V300R001_D1     d1;
    VC1DNMSG_V300R001_D2     d2;
    VC1DNMSG_V300R001_D3     d3;
    VC1DNMSG_V300R001_D4     d4;
    VC1DNMSG_V300R001_D5     d5;
    VC1DNMSG_V300R001_D6     d6;
    VC1DNMSG_V300R001_D7     d7;
    VC1DNMSG_V300R001_D8     d8;
    VC1DNMSG_V300R001_D9     d9;
    VC1DNMSG_V300R001_D10    d10;
    VC1DNMSG_V300R001_D11    d11;
    VC1DNMSG_V300R001_D12    d12;
    VC1DNMSG_V300R001_D13    d13;
    VC1DNMSG_V300R001_D14    d14;
    VC1DNMSG_V300R001_D15    d15;
    VC1DNMSG_V300R001_D16    d16;
    VC1DNMSG_V300R001_D17    d17;
    VC1DNMSG_V300R001_D18    d18;
    VC1DNMSG_V300R001_D19    d19;
    VC1DNMSG_V300R001_D20    d20;
    VC1DNMSG_V300R001_D21    d21;
    VC1DNMSG_V300R001_D22    d22;
    VC1DNMSG_V300R001_D23    d23;
    VC1DNMSG_V300R001_D24    d24;
    VC1DNMSG_V300R001_D25    d25;
    VC1DNMSG_V300R001_D26    d26;
    VC1DNMSG_V300R001_D27    d27;
    VC1DNMSG_V300R001_D28    d28; 
	
    VC1DNMSG_V300R001_D29    d29; 
    VC1DNMSG_V300R001_D30    d30; 
    VC1DNMSG_V300R001_D31    d31; 
    VC1DNMSG_V300R001_D32    d32; 
    VC1DNMSG_V300R001_D33    d33; 
    VC1DNMSG_V300R001_D34    d34; 
    VC1DNMSG_V300R001_D35    d35; 
    VC1DNMSG_V300R001_D36    d36; 
								
    VC1DNMSG_V300R001_D63    d63;
}VC1_V300R001_DNMSGINF_S;

#ifdef __cplusplus
extern "C" {
#endif

SINT32 VC1HAL_V300R001_InitHal( SINT32 CodeTabAddr );
SINT32 VC1HAL_V300R001_StartDec(VC1_DEC_PARAM_S  *pVc1DecParam, SINT32 VdhId );
//VOID VC1HAL_DnMsgInit(VC1_V200R003_DNMSGINF_S *pDnMsgInf);
#ifdef __cplusplus
}
#endif


#endif
