/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_unf_keyled.c
 * Description:
 *
 * History:
 * Version   Date             Author        DefectNum    Description
 * main\1    2008-10-10   w00142069         NULL         Create this file.
 * main\1    2010-04-15   Jiang Lei 40671   NULL         modify for HD.
 ******************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>

#include "hi_unf_keyled.h"
#include "hi_drv_keyled.h"
#include "drv_keyled_ioctl.h"
#include "hi_drv_struct.h"

static HI_S32 g_s32KEYLEDFd = -1;
static HI_U32 g_u32KeyledInitCount = 0;

static const char *keyled_devname = "/dev/"UMAP_DEVNAME_KEYLED;

static pthread_mutex_t g_KeyledMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_KEYLED_LOCK()    (void)pthread_mutex_lock(&g_KeyledMutex);
#define HI_KEYLED_UNLOCK()  (void)pthread_mutex_unlock(&g_KeyledMutex);

#define CHECK_KEYLED_OPEN()\
do{\
    HI_KEYLED_LOCK();\
    if (g_s32KEYLEDFd < 0)\
    {\
        HI_ERR_KEYLED("keyled not init\n");\
        HI_KEYLED_UNLOCK();\
        return HI_ERR_KEYLED_NOT_INIT;\
    }\
    HI_KEYLED_UNLOCK();\
}while(0)


/***********************************************************************************
 *  Function:       HI_UNF_KEYLED_Init
 *  Description:    open key devide
 *  Calls:
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEYLED_Init(HI_VOID)
{
    HI_S32 flags = O_RDWR | O_NONBLOCK;

    HI_KEYLED_LOCK();

    g_u32KeyledInitCount++;

    if (g_s32KEYLEDFd < 0)
    {
        g_s32KEYLEDFd = open(keyled_devname, flags, 0);
        if (g_s32KEYLEDFd < 0)
        {
            HI_ERR_KEYLED("Init KEYLED error dev:%s\n", keyled_devname);
            HI_KEYLED_UNLOCK();
            return HI_FAILURE;
        }
    }

    HI_KEYLED_UNLOCK();

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_KEYLED_DeInit
 *  Description:    close key
 *  Calls:
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEYLED_DeInit(HI_VOID)
{
    HI_S32 s32Ret = 0;

    HI_KEYLED_LOCK();

    if (g_s32KEYLEDFd < 0)
    {
        HI_ERR_KEYLED("keyled not init\n");
        HI_KEYLED_UNLOCK();
        return HI_SUCCESS;
    }

    g_u32KeyledInitCount--;

    if (0 == g_u32KeyledInitCount)
    {
        s32Ret = close(g_s32KEYLEDFd);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_KEYLED("DeInit KEYLED err.%d \n", g_s32KEYLEDFd);
            HI_KEYLED_UNLOCK();
            return HI_FAILURE;
        }

        g_s32KEYLEDFd = -1;
    }

    HI_KEYLED_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_UNF_KEYLED_SelectType(HI_UNF_KEYLED_TYPE_E enKeyLedType)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (enKeyLedType >= HI_UNF_KEYLED_TYPE_BUTT)
    {
        HI_ERR_KEYLED("invalid keyled type.\n");
        return HI_ERR_KEYLED_INVALID_PARA;
    }

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3719mv100_a)
    if (HI_UNF_KEYLED_TYPE_74HC164 == enKeyLedType)
    {
        HI_ERR_KEYLED("Don't support this keyled type.\n");
        return HI_ERR_KEYLED_INVALID_PARA;
    }
#endif

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_SELECT_CMD, &enKeyLedType);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled select keyled type failed s32Result = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_KEY_Open
 *  Description:    open key devide
 *  Calls:
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEY_Open(HI_VOID)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_KEY_OPEN_CMD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled enable failed, ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_KEY_Close
 *  Description:    close key
 *  Calls:
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEY_Close(HI_VOID)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_KEY_CLOSE_CMD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled Disable failed, ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_KEY_Reset
 *  Description:    reset key
 *  Calls:          HI_KEYLED_ClearBuf
  *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEY_Reset(HI_VOID)
{
    HI_S32 s32Ret;

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_KEY_RESET_CMD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled clear buf failed, ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_KEY_GetValue
 *  Description:    get the value of key
 *  Calls:          HI_KEYLED_GetValue
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEY_GetValue(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    HI_S32 s32Ret = 0;
    GET_KEY_VALUE_S stPara = {0};

    if (NULL == pu32PressStatus)
    {
        HI_ERR_KEYLED("Input parameter(pu32PressStatus) invalid\n");
        return HI_ERR_KEYLED_NULL_PTR;
    }

    if (NULL == pu32KeyId)
    {
        HI_ERR_KEYLED("Input parameter(pu32KeyId) invalid\n");
        return HI_ERR_KEYLED_NULL_PTR;
    }

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_KEY_GET_VALUE_CMD, &stPara);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_KEYLED("keyled get key error:0x%x \n", s32Ret);
        return s32Ret;
    }

    *pu32KeyId = stPara.u32KeyCode;
    *pu32PressStatus = stPara.u32KeyStatus;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_KEY_SetBlockTime(HI_U32 u32BlockTimeMs)
{
    HI_S32 s32Ret = HI_SUCCESS;

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_SET_BLOCK_TIME_CMD, &u32BlockTimeMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled set block time failed, ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/***********************************************************************************
 *  Function:       HI_UNF_KEY_SetRepTime
 *  Description:    report the time interval of the sustain status of  the key
 *  Calls:          HI_UNF_KEY_RepKeyTimeoutVal
  *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEY_RepKeyTimeoutVal(HI_U32 u32RepTimeMs)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    if (u32RepTimeMs < 108)
    {
        u32RepTimeMs = 108;
    }
    else if (u32RepTimeMs > 65536)
    {
        u32RepTimeMs = 65536;
    }

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_SET_REPKEY_TIME_CMD, &u32RepTimeMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled u32RepTimeMs  Setup Error ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_KEY_IsRepKey
 *  Description:   whether to enable the repeat function of the  key
 *  Calls:          HI_KEYLED_IsRepKey
  *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEY_IsRepKey(HI_U32 u32IsRepKey)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    if (u32IsRepKey > 1)
    {
        HI_ERR_KEYLED("Input parameter(u32IsRepKey) invalid:%d\n", u32IsRepKey);
        return HI_ERR_KEYLED_INVALID_PARA;
    }
    else
    {
        s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_SET_IS_REPKEY_CMD, &u32IsRepKey);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_KEYLED("keyled isrepkey setup error ret = 0x%x \n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_KEY_IsKeyUp
 *  Description:    whether to open the key up  function
 *  Calls:          HI_KEYLED_IsKeyUp
  *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_KEY_IsKeyUp(HI_U32 u32IsKeyUp)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    if (u32IsKeyUp > 1)
    {
        HI_ERR_KEYLED("Input parameter(u32IsKeyUp) invalid:%d\n", u32IsKeyUp);
        return HI_ERR_KEYLED_INVALID_PARA;
    }
    else
    {
        s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_SET_IS_KEYUP_CMD, &u32IsKeyUp);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_KEYLED("keyled iskeyup setup error ret = 0x%x \n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_LED_Open
 *  Description:    open led devide
 *  Calls:        HI_KEYLED_Open
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_LED_Open(HI_VOID)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_LED_OPEN_CMD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled enable failed ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_LED_Close
 *  Description:    close led
 *  Calls:          HI_KEYLED_Close
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_LED_Close(HI_VOID)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_LED_CLOSE_CMD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled Disable failed ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_LED_Display
 *  Description:    display of lLED
 *  Calls:          HI_KEYLED_Display
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_LED_Display(HI_U32 u32CodeValue)
{
    HI_S32 s32Ret = HI_SUCCESS;

    CHECK_KEYLED_OPEN();

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_DISPLAY_CODE_CMD, &u32CodeValue);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled display ioctl error ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
 *  Function:       HI_UNF_LED_DisplayTime
 *  Description:    the display time of LED
 *  Calls:          HI_KEYLED_DisplayTime
 *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_LED_DisplayTime(HI_UNF_KEYLED_TIME_S stLedTime)
{
    HI_S32 s32Ret = HI_SUCCESS;

    CHECK_KEYLED_OPEN();

    if ((stLedTime.u32Hour > 23) || (stLedTime.u32Minute > 59))
    {
        HI_ERR_KEYLED("Input parameter stLedTime invalid:%d-%d\n", stLedTime.u32Hour, stLedTime.u32Minute);
        return HI_ERR_KEYLED_INVALID_PARA;
    }

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_DISPLAY_TIME_CMD, &stLedTime);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled display time ioctl error ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/***********************************************************************************
 *  Function:       HI_UNF_LED_SetFlashPin
 *  Description:    set the flash pin of the LED
 *  Calls:          HI_KEYLED_SetFlashPin
  *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_E enPin)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    if ((enPin < HI_UNF_KEYLED_LIGHT_1) || (enPin > HI_UNF_KEYLED_LIGHT_NONE))
    {
        HI_ERR_KEYLED("Input Flash Pin error:%d\n", enPin);
        return HI_ERR_KEYLED_INVALID_PARA;
    }

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_SET_FLASH_PIN_CMD, &enPin);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled set flash pin error ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}



/***********************************************************************************
 *  Function:       HI_UNF_LED_SetFlashFreq
 *  Description:    set the flash frequency of the LED
 *  Calls:       HI_KEYLED_SetFlashFreq
  *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          NA
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_E enLevel)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();

    if ((enLevel < HI_UNF_KEYLED_LEVEL_1) || (enLevel > HI_UNF_KEYLED_LEVEL_5))
    {
        HI_ERR_KEYLED("Input Flash Freq Level error:%d\n", enLevel);
        return HI_ERR_KEYLED_INVALID_PARA;
    }

    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_CONFIG_FLASH_FREQ_CMD, &enLevel);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("Set Flash Freq error ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/***********************************************************************************
 *  Function:       HI_UNF_LED_SetLockPin
 *  Description:   whether enable the lockfreq led
 *  Calls:          HI_KEYLED_SetFlashPin
  *  Data Accessed:  NA
 *  Data Updated:   NA
 *  Input:          setLock set dp pin to 0 or 1
 *  Output:         NA
 *  Return:      ErrorCode(reference to document)
 *  Others:         NA
 ***********************************************************************************/
HI_S32 HI_UNF_LED_SetLockPin(HI_BOOL setLock)
{
    HI_S32 s32Ret = 0;

    CHECK_KEYLED_OPEN();
	
	if (setLock != (HI_BOOL)0 && setLock != (HI_BOOL)1)
	{
		HI_ERR_KEYLED("Input parameter setLock error:%d\n", setLock);
        return HI_ERR_KEYLED_INVALID_PARA;
	}
	
    s32Ret = ioctl(g_s32KEYLEDFd, HI_KEYLED_SETLOCK_CMD, (HI_BOOL)&setLock);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_KEYLED("keyled HI_UNF_LED_SetLockPin error ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


