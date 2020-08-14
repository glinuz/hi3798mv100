#ifndef __SAMPLE_CC__
#define __SAMPLE_CC__

#ifdef __cplusplus
extern "C"{
#endif


typedef HI_S32 (*CC_DATA_CALLBACK_FN)(HI_U32 u32UserData, HI_U8 *pu8Data, HI_U32 u32DataLength);

typedef struct tagCC_DATA_INSTALL_PARAM_S
{
    HI_U32 u32DmxID;
    HI_U16 u16CCPID;
    HI_U32 u32UserData;
    //HI_UNF_CC_DATA_TYPE_E enDataType;
    CC_DATA_CALLBACK_FN pfnCallback;
}CC_DATA_INSTALL_PARAM_S;


HI_S32 CC_Data_Init();

HI_S32 CC_Data_DeInit();

HI_S32 CC_Data_Install(CC_DATA_INSTALL_PARAM_S *pstInstallParam);

HI_S32 CC_Data_Uninstall();


#ifdef __cplusplus
}
#endif

#endif

