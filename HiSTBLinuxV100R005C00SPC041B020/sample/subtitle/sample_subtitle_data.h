#ifndef __SAMPLE_SUBTITLE__
#define __SAMPLE_SUBTITLE__

#ifdef __cplusplus
extern "C"{
#endif


typedef HI_S32 (*SUBT_DATA_CALLBACK_FN)(HI_U32 u32UserData, HI_U8 *pu8Data, HI_U32 u32DataLength);

typedef struct tagSUBT_DATA_INSTALL_PARAM_S
{
    HI_U32 u32DmxID;
    HI_U16 u16SubtPID;
    HI_U32 u32UserData;
    HI_UNF_SUBT_DATA_TYPE_E enDataType;
    SUBT_DATA_CALLBACK_FN pfnCallback;
}SUBT_DATA_INSTALL_PARAM_S;


HI_S32 SUBT_Data_Init();

HI_S32 SUBT_Data_DeInit();

HI_S32 SUBT_Data_Install(SUBT_DATA_INSTALL_PARAM_S *pstInstallParam, HI_HANDLE *hData);

HI_S32 SUBT_Data_Uninstall(HI_HANDLE hData);


#ifdef __cplusplus
}
#endif

#endif

