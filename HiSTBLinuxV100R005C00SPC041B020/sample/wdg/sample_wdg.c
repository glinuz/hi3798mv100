/*****************************************
* the sample based on hisi demo 
enable wdg, execute clear wdg for five times, can not reset system
close wdg, can not reset system
printf "now enable wdg and wait reset board"
reopen wdg, system will reset when timeout
*
*****************************************/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "hi_unf_wdg.h"

#define TEST_WDG_NO	0

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 s32Ret;
    HI_U32 u32Loop;
    HI_U32 u32Value;

    /* Open WDG*/
    s32Ret = HI_UNF_WDG_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    /* Set WDG TimeOut */
    u32Value = 2000;    //ms
    s32Ret = HI_UNF_WDG_SetTimeout(TEST_WDG_NO, u32Value);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }


    /* Enable WDG */
    s32Ret = HI_UNF_WDG_Enable(TEST_WDG_NO);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }

    sleep(1);
    for (u32Loop = 0; u32Loop < 5; u32Loop++)
    {
        /* Clear WDG during timeout, can not reset system */
        s32Ret = HI_UNF_WDG_Clear(TEST_WDG_NO);
        if (HI_SUCCESS != s32Ret)
        {
            printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
            goto ERR1;
        }

        printf("Clear wdg Success\n");
        sleep(1);
    }

    printf("wdg disabled\n");

    /* Disable WDG, cat not reset system */
    s32Ret = HI_UNF_WDG_Disable(TEST_WDG_NO);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }

    sleep(5);
    printf("\nwdg doesn't reset board,demo passed\n");
    printf("now enable wdg and wait reset boardaaaa\n\n");

    /* Enable WDG*/
    s32Ret = HI_UNF_WDG_Enable(TEST_WDG_NO);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }
	printf("system reset at onceaa\n");
    /* After timeout, system reset at once */
    sleep(5);
	printf("system reset at once\n");

ERR1:
    HI_UNF_WDG_DeInit();
    printf("run wdg sample failed\n");
    return s32Ret;
}
