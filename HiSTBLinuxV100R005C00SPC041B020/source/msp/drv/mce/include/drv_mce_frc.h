#ifndef __MCE_FRC_H__
#define __MCE_FRC_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AVPLAY_ALG_FRC_PRECISION  32
#define AVPLAY_ALG_FRC_BASE_PLAY_RATIO  256

/* maximum display rate:16 */
#define AVPLAY_ALG_FRC_MAX_PLAY_RATIO   AVPLAY_ALG_FRC_BASE_PLAY_RATIO * 256

/* minimum display rate: 1/16 */
#define AVPLAY_ALG_FRC_MIN_PLAY_RATIO   1

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define AVPLAY_ALG_ABS(x)     (((x) < 0) ? -(x) : (x))

typedef struct
{
	/* frame rate conversion state for progressive frame : <0-drop; ==0-natrual play; >0-repeat time */
	HI_S32 s32FrmState;
} AVPLAY_FRC_CTRL_S;

typedef struct
{
	HI_U32  u32InRate;      /* unit: frame */
	HI_U32  u32OutRate;     /* fresh rate */
	HI_U32  u32PlayRate;    /* play rate */
} AVPLAY_FRC_CFG_S;

typedef struct
{
	HI_U32  u32InRate;      /* unit: frame */
	HI_U32  u32OutRate;     /* fresh rate */
	HI_U32  u32PlayRate;    /* play rate */
    HI_U32  u32CurID;       /* current insert or drop position in a FRC cycle */
	HI_U32  u32InputCount;  /* input counter */
} AVPLAY_ALG_FRC_S;

HI_VOID MCE_FrcCreate(AVPLAY_ALG_FRC_S *FrcCalAlg, AVPLAY_FRC_CFG_S *FrcParamCfg, AVPLAY_FRC_CTRL_S *FrcCtrlInfo);
HI_VOID MCE_FrcDestroy(AVPLAY_ALG_FRC_S *FrcCalAlg);
HI_VOID MCE_FrcReset(AVPLAY_ALG_FRC_S *hFrc);
HI_VOID MCE_FrcCalculate(AVPLAY_ALG_FRC_S *hFrc, AVPLAY_FRC_CFG_S *pstFrcCfg, AVPLAY_FRC_CTRL_S *pstFrcCtrl);

#ifdef __cplusplus
}
#endif

#endif

