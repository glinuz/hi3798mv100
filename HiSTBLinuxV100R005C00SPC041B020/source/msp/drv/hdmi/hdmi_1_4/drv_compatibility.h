#include "hi_drv_hdmi.h"

typedef struct hiHDMI_DELAY_TIME_S{
    HI_U8           u8IDManufactureName[4];   /**<Manufacture name*//**<CNcomment:设备厂商标识 */
    HI_U32          u32IDProductCode;         /**<Product code*//**<CNcomment:设备ID */
    HI_U32          u32DelayTimes;            
    HI_U32          u32MuteDelay;
    HI_U8           u8ProductType[32];    /**<Product Type*//**<CNcomment:产品型号 */
}HDMI_DELAY_TIME_S;

HI_S32 GetFormatDelay(HI_UNF_HDMI_ID_E enHdmi,HI_U32 *DelayTime);
HI_S32 GetmuteDelay(HI_UNF_HDMI_ID_E enHdmi,HI_U32 *DelayTime);


void SetGlobalFmtDelay(HI_U32 Delay);
HI_U32 GetGlobalFmtDelay(void);
void SetForceDelayMode(HI_BOOL bforceFmtDelay,HI_BOOL bforceMuteDelay);
HI_BOOL IsForceFmtDelay(void);
HI_BOOL IsForceMuteDelay(void);


void SetGlobalMuteDelay(HI_U32 Delay);
HI_U32 GetGlobalsMuteDelay(void);


void Check1stOE(HI_UNF_HDMI_ID_E enHdmi);

HI_BOOL Is3716Cv200ECVer(void);



