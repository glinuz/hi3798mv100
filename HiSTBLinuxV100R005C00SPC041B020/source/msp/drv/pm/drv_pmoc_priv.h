#ifndef __DRV_PMOC_PRIV_H__
#define __DRV_PMOC_PRIV_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define PM_CMD_MAX_NUM (2)
#define PM_CMD_MAX_CMD_LEN (32)
#define PM_CMD_MAX_VLAUE_LEN (32)
#define PM_CMD_AVS          "avs"
#define PM_CMD_ON           "on"
#define PM_CMD_OFF          "off"
#define PM_CMD_VOLT         "volt"
#define PM_CMD_HPMOFFSET    "hpmoffset"
#define PM_CMD_FREQ         "freq"
#define PM_CMD_HELP         "help"
#define PM_CMD_WAKEUPRESET  "reset"
#define PM_CMD_DEBUG        "debug"
#define PM_CMD_ENTERSMARTSTANDBY "entersmart"
#define PM_CMD_QUITSMARTSTANDBY  "quitsmart"
#define PM_CMD_TEMPCTRL     "tempctrl"
#define PM_CMD_TEMPDOWN     "tempdown"
#define PM_CMD_TEMPUP       "tempup"
#define PM_CMD_TEMPREBOOT   "tempreboot"

typedef struct tagPM_CMD_DATA_S
{
    HI_U8 aszCmd[PM_CMD_MAX_CMD_LEN];
    HI_U8 aszValue[PM_CMD_MAX_VLAUE_LEN];
}PM_CMD_DATA_S;

HI_S32 PMParseCmd(HI_U8* pu8Cmd, HI_U32 u32Len, PM_CMD_DATA_S** pstCmd, HI_U32* pu32Num);
HI_U32 PMParseValue(HI_U8* pu8Value);

HI_S32 PMEnterSmartStandby(HI_U32 u32HoldModules);
HI_S32 PMQuitSmartStandby(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __DRV_PMOC_PRIV_H__ */

