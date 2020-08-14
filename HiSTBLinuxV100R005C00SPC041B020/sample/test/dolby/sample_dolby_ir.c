/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: sample_dolby_ir.c
 * Description:
 *       this sample finish the IR control.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2011-07-11   184737
 ******************************************************************************/
#include "dolby_ir.h"

/* key value table */
INPUT_KEY_MAP_S g_stIRMap[MAX_IR_MAP] =
{
    /* common key */           /* real IR value */
    {APP_KEY_POWER,           0x639cff00,},
    {APP_KEY_VOLUME_MUTE,     0x22ddff00,},
    {APP_KEY_TRACK,           0x2ed1ff00,},

    {APP_KEY_MENU,            0x629dff00,},
    {APP_KEY_UP,              0x35caff00,},
    {APP_KEY_LEFT,            0x6699ff00,},
    {APP_KEY_RIGHT,           0x3ec1ff00,},
    {APP_KEY_DOWN,            0x2dd2ff00,},
    {APP_KEY_SELECT,          0x31ceff00,},
    {APP_KEY_BACK,            0x6f90ff00,},
    {APP_KEY_SEATCH,          0x6897ff00,},
    {APP_KEY_DEL,             0x7788ff00,},

    {APP_KEY_NUM1,            0x6d92ff00,},
    {APP_KEY_NUM2,            0x6c93ff00,},
    {APP_KEY_NUM3,            0x33ccff00,},
    {APP_KEY_NUM4,            0x718eff00,},
    {APP_KEY_NUM5,            0x708fff00,},
    {APP_KEY_NUM6,            0x37c8ff00,},
    {APP_KEY_NUM7,            0x758aff00,},
    {APP_KEY_NUM8,            0x748bff00,},
    {APP_KEY_NUM9,            0x3bc4ff00,},
    {APP_KEY_NUM0,            0x7887ff00,},
};

HI_S32 HI_DOLBY_IR_Open(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_IR_Init();
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_IR_EnableKeyUp(HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_UNF_IR_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_IR_EnableRepKey(HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_UNF_IR_DeInit();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DOLBY_IR_Close(HI_VOID)
{
    return  HI_UNF_IR_DeInit();
}

HI_S32 HI_DOLBY_IR_GetValue(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    HI_U64 u64KeyId = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_IR_GetValue((HI_UNF_KEY_STATUS_E *) pu32PressStatus, &u64KeyId , 0);
    *pu32KeyId = (HI_U32)u64KeyId;
    return s32Ret;
}
