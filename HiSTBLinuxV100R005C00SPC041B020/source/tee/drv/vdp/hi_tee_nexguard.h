#ifndef __TEE_VDP_NXG__
#define __TEE_VDP_NXG__




struct SettingStruct
{
    char settingArray[21];
};


HI_VOID NG_Reset(HI_VOID);                                   /* to perform a software reset of the WM IP core */ 
HI_VOID NG_EnableIn(HI_BOOL enableIn);                    /*   1-bit, globally enables/disables watermarking */
HI_VOID NG_DebugEnableIn(HI_BOOL debugEnableIn);          /*   1-bit, visible test pattern enable/disable */
HI_VOID NG_SetKeyInSeed(HI_U32 keyInSeed);       /*  32-bit 每 KeyIn */



HI_VOID NG_SetOperatorId(HI_U8 operatorId);    /*   8-bit 每 Payload[55:48] */
HI_VOID NG_SetSubscriberId(HI_U32 subScriberId); /*  32-bit 每 Payload[47:16] */
HI_VOID NG_SetTimeCode(HI_U16 timeCode);   /*  16-bit 每 Payload[15:0]  */


HI_VOID NG_SetSetting(struct SettingStruct* sStruct);  /* 165-bit packed in 21 bytes
                                                         setting[  7:  0] =  settingArray[20]
                                                         setting[ 15:  8] =  settingArray[19]
                                                                ＃
                                                         setting[164:160] = (settingArray[0] & 0x1F)
                                                   */
                                                   

                                                   
HI_VOID NXG_HW_SetParameters(HI_BOOL bEnable);
#endif

