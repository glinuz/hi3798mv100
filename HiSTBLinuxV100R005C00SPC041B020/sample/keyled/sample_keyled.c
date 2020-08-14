

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hi_unf_keyled.h"

#define LED_DELAY_TIME 1000000
static HI_S32 s_s32TaskRunning = 0;

HI_U8 DigDisCode_74hc164[10]    = {0x03, 0x9F, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09};
HI_U8 DigDisCode_pt6961[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f}; //pt6961_ok(0x00È«²»ÁÁ/0x7fÈ«ÁÁ)
HI_U8 DigDisCode_ct1642[10] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};
HI_U8 DigDisCode_pt6964[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
HI_U8 DigDisCode_fd650[10]  = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

const HI_CHAR keyled_name[6][16] =
{
    "74HC164",
    "PT6961",
    "CT1642",
    "PT6964",
    "FD650",
    "GPIOKEY"
};

HI_U8 DigDisCode[10] = {0};


void * LED_DisplayTask(void *args)
{
    HI_U32 u32Loop = 0;
    HI_S32 s32Ret;
    HI_UNF_KEYLED_TIME_S stLedTime;
    stLedTime.u32Hour = 12;
    stLedTime.u32Minute = 25;
    
    while (s_s32TaskRunning == 1)
    {
        for (u32Loop = 0; u32Loop < sizeof(DigDisCode) && s_s32TaskRunning == 1; u32Loop++)
        {
            /*LED display 0~9 or letters*/
            s32Ret = HI_UNF_LED_Display(DigDisCode[(u32Loop + 3) % sizeof(DigDisCode)] << 24 
                                        | DigDisCode[(u32Loop + 2) % sizeof(DigDisCode)] << 16
                                        | DigDisCode[(u32Loop + 1) % sizeof(DigDisCode)] << 8 
                                        | DigDisCode[u32Loop]);
            if (HI_SUCCESS != s32Ret)
            {
                printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
                break;
            }
			
			s32Ret = HI_UNF_LED_SetFlashPin(u32Loop % 5 + 1);

            usleep(LED_DELAY_TIME);
        }

        (HI_VOID)HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_NONE); 
        
        s32Ret = HI_UNF_LED_DisplayTime(stLedTime);
        usleep(LED_DELAY_TIME * 5);
    }
    s_s32TaskRunning = 0;
    return 0;
}

void * KEY_ReceiveTask(void *args)
{
    HI_S32 s32Ret;
    HI_U32 u32PressStatus, u32KeyId;

    while (s_s32TaskRunning == 1)
    {
        /*get KEY press value & press status*/
        s32Ret = HI_UNF_KEY_GetValue(&u32PressStatus, &u32KeyId);
        if (HI_SUCCESS == s32Ret)
        {
            printf("KEY  KeyId : 0x%x    PressStatus :%d[%s]\n", u32KeyId, u32PressStatus
                , (0 == u32PressStatus) ? "DOWN" 
                : (1 == u32PressStatus) ? "HOLD" : "UP");            
        }
        else
        {
            usleep(50000);
        }
    }

    return 0;
}

int  main(int argc, char *argv[])
{
    HI_S32 s32Ret;
    pthread_t keyTaskid;
    pthread_t ledTaskid;
	HI_UNF_KEYLED_TYPE_E keyled_type;

    printf("Show led and wait key press\n");

    if(argc != 2)
    {
	    printf("usage:sample_keyled [keyled_type]\n"
               "keyled_type = 0: 74HC164 \n"
               "keyled_type = 1: PT6961 \n"
               "keyled_type = 2: CT1642 \n"
               "keyled_type = 3: PT6964 \n"
               "keyled_type = 4: FD650 \n"
               "keyled_type = 5: GPIOKEY \n");
        return HI_FAILURE;
    }
    else
    {
	    keyled_type = atoi(argv[1]);
    }

    if(keyled_type >= HI_UNF_KEYLED_TYPE_BUTT)
    {
       printf("usage:sample_keyled [keyled_type]\n"
               "keyled_type = 0: 74HC164 \n"
               "keyled_type = 1: PT6961 \n"
               "keyled_type = 2: CT1642 \n"
               "keyled_type = 3: PT6964 \n"
               "keyled_type = 4: FD650 \n"
               "keyled_type = 5: GPIOKEY \n");
        return HI_FAILURE;
    }
    printf("Test keyled_type %s\n", keyled_name[keyled_type]);
	
	if(keyled_type == 0)
	{
		memcpy(DigDisCode, DigDisCode_74hc164, sizeof(DigDisCode_74hc164));
	}
	else if(keyled_type == 1)
	{
		memcpy(DigDisCode, DigDisCode_pt6961, sizeof(DigDisCode_pt6961));
	}
	else if(keyled_type == 2)
	{
		memcpy(DigDisCode, DigDisCode_ct1642, sizeof(DigDisCode_ct1642));
	}
	else if(keyled_type == 3)
	{
		memcpy(DigDisCode, DigDisCode_pt6964, sizeof(DigDisCode_pt6964));
	}
	else if(keyled_type == 4)
	{
		memcpy(DigDisCode, DigDisCode_fd650, sizeof(DigDisCode_fd650));
	}


    HI_SYS_Init();
    HI_UNF_KEYLED_Init();


    s32Ret = HI_UNF_KEYLED_SelectType(keyled_type);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    /*open LED device*/
    s32Ret = HI_UNF_LED_Open();
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    /*enable flash*/
    s32Ret = HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_1);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }


    /*config LED flash or not*/
    s32Ret = HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }

    /*open KEY device*/
    s32Ret = HI_UNF_KEY_Open();
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }

    /*config keyup is valid*/
    s32Ret = HI_UNF_KEY_IsKeyUp(1);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR2;
    }

    /*config keyhold is valid*/
    s32Ret = HI_UNF_KEY_IsRepKey(1);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR2;
    }

    s_s32TaskRunning = 1;

    /*create a thread for receive*/
    s32Ret = pthread_create(&keyTaskid, NULL, KEY_ReceiveTask, NULL);
    if (0 != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        perror("pthread_create");
        goto ERR3;
    }

    /*create a thread for led display*/
    s32Ret = pthread_create(&ledTaskid, NULL, LED_DisplayTask, NULL);
    if (0 != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        perror("pthread_create");
        goto ERR3;
    }

    printf("Press any key to exit demo\n");
    getchar();
    s_s32TaskRunning = 0;

    /*exit the two thread that ledTaskid and keyTaskid*/
    pthread_join(ledTaskid, 0);
    pthread_join(keyTaskid, 0);

    HI_UNF_KEY_Close();
    HI_UNF_LED_Close();
    HI_UNF_KEYLED_DeInit();

    HI_SYS_DeInit();

    printf("\nrun keyled demo success\n");
    return HI_SUCCESS;

ERR3:
    s_s32TaskRunning = 0;
ERR2:
    HI_UNF_KEY_Close();
ERR1:
    HI_UNF_LED_Close();

    HI_UNF_KEYLED_DeInit();
    HI_SYS_DeInit();

    printf("run keyled demo failed\n");
    return s32Ret;
}
