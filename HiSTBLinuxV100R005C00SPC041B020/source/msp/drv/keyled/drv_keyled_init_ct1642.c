/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: init_keyled_display.c
* Description:
*
* History:
* Version   Date                Author       DefectNum    Description
* main\1    2011-11-15        chenqiang     NULL           Create this file.
***********************************************************************************/

#include <linux/errno.h>
#include <linux/stddef.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/relay.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/cpu.h>
#include <linux/splice.h>
#include <asm/io.h>
#include <mach/platform.h>
#include <mach/hardware.h>
#include <asm/memory.h>
#include <asm/setup.h>

#include "hi_type.h"
//#include "common_mem.h"
//#include "hi_mmz.h"
#include "drv_base_ext_k.h"
#include "drv_gpio_ext.h"
#include "hi_drv_keyled.h"
#include "hi_unf_keyled.h"
#include "drv_keyled_ct1642.h"

extern HI_S32 KEYLED_KEY_Open_CT1642(HI_VOID);
extern HI_S32 KEYLED_KEY_Close_CT1642(HI_VOID);

extern HI_S32 KEYLED_LED_Open_CT1642(HI_VOID);
extern HI_S32 KEYLED_LED_Close_CT1642(HI_VOID);
extern HI_S32 KEYLED_LED_Display_CT1642(HI_U32 u32CodeValue);
extern HI_S32 KEYLED_LED_DisplayTime_CT1642(HI_UNF_KEYLED_TIME_S stKeyLedTime);
extern HI_S32 KEYLED_LED_SetFlashPin_CT1642(HI_UNF_KEYLED_LIGHT_E enPin);
extern HI_S32 KEYLED_LED_SetFlashFreq_CT1642(HI_UNF_KEYLED_LEVEL_E enLevel);

/*     --7--
 *    |     |
 *   2|     |6  
 *    |--1--    0
 *    |     |     
 *   3|     |5
 *     --4-- 
 */


HI_U8 sNumTbl[]={0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};//ok  

/*only 9 of the capital letters are effective ('C' 'E' 'F' 'H' 'L' 'O' 'P' 'S' 'U')*/
/*CNcomment:大写字母只显示9个有效('C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U')*/
HI_U8 sChrTblUp[]={       
    0xee, 0xfe, 0x9c, 0xfc, 0x9e, 0x8e, 0x00, 
    0x6e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 
    0xfc, 0xce, 0x00, 0x00, 0xb6, 0x00, 0x7c, 
    0x00, 0x00, 0x00, 0x00, 0x00
};

/*only 15 of the smalll letters are effective (b c d E, g h i, l n o, P q S(5) t, u)*/
/*CNcomment:小写字母只显示15个有效(b c d E, g h i, l n o, P q S(5) t, u) */
HI_U8 sChrTblLow[]={   
    0x00, 0x3e, 0x1a, 0x7a, 0xde, 0x00, 0xf6, 
    0x2e, 0x20, 0x00, 0x00, 0x60, 0x00, 0x2a, 
    0x3a, 0xce, 0xe6, 0x00, 0xb6, 0x1e, 0x38, 
    0x00, 0x00, 0x00, 0x00, 0x00
};

static HI_CHAR LedDisplayStr[5]={"boot"};

HI_S32 KEYLED_GetDisplayPara(void)
{
	HI_S8 *s;
	HI_S8 *p = HI_NULL;
	HI_S8 *cmdline = HI_NULL;
	cmdline = kmalloc(COMMAND_LINE_SIZE, GFP_KERNEL);
//	kmalloc(ADEC_MAX_INPUT_BLOCK_SIZE, GFP_KERNEL);
	if(!cmdline)
	{
	    HI_ERR_KEYLED("KEYLED_GetDisplayPara request mem failure \n");	
		return HI_FAILURE;
	}
	strlcpy(cmdline, saved_command_line, COMMAND_LINE_SIZE);
	p = strstr(cmdline, "LedDisplay=");
	if (p)
	{
		s = strsep((char **)&p, "=");
		if (s)
		{
			s = strsep((char **)&p, " ");

			if (s)
			{
				memcpy(LedDisplayStr, s, sizeof(LedDisplayStr));
				//HI_INFO_KEYLED("---LedDisplayStr = %s \n",LedDisplayStr);
			}
		}
	}

	
	kfree(cmdline);
	cmdline = HI_NULL;

	return HI_SUCCESS;
}

HI_S32 KEYLED_DisplayString(HI_CHAR *fpString)
{
    unsigned int tmpData = 0;
    unsigned char DispCode[5] = {0};
    unsigned char j;

    if (NULL == fpString)
    {
        return HI_FAILURE;
    }

    memcpy(DispCode, fpString, 4);
	
   // HI_INFO_KEYLED("led discode is  %s \n" , DispCode );
	
    for (j = 0; j < 4; j++)
    {
        if ((DispCode[j] >= 'A') && (DispCode[j] <= 'Z'))
        {
            DispCode[j] = sChrTblUp[DispCode[j] - 'A'];
        }
        else if ((DispCode[j] >= 'a') && (DispCode[j] <= 'z'))
        {
            DispCode[j] = sChrTblLow[DispCode[j] - 'a'];
        }
        else if ((DispCode[j] >= '0') && (DispCode[j] <= '9'))
        {
            DispCode[j] = sNumTbl[DispCode[j] - '0'];
        }
        else if (DispCode[j] == '-')
        {
             DispCode[j] = 0x02; 
        }
        else if (DispCode[j] == '_')
        {
            DispCode[j] = 0x10;
        }
        else if (DispCode[j] == ':' ||DispCode[j] == '.' )
        {
            DispCode[j]=0x01;
        }
        else  /* FIXME: may be should add symbol ':' and '.' */
        {
            DispCode[j] = 0x0;  /* default: not display */
        }
    }
    

     tmpData = (DispCode[3]<<24 )|(DispCode[2]<<16 )|(DispCode[1]<<8 )|(DispCode[0]);
    
     KEYLED_LED_Display_CT1642(tmpData);	  
     return HI_SUCCESS;
	  
}

int  keyled_Param_Init(void)
{
    HI_S32 Ret;
    Ret = KEYLED_KEY_Open_CT1642();
        
    Ret |= KEYLED_LED_Open_CT1642();

	Ret |= KEYLED_GetDisplayPara();

    return Ret;
}

//extern void fastplay_Param_deInit(void);

//extern int mce_create_play(void);
extern int keyled_display_begin(void);

static int keyled_display_init(void)
{
    int ret;
    
    //HI_INFO_KEYLED(">>>>>> keyled_display begin >>>>>>\n");

#ifndef HI_MCE_SUPPORT
    ret = HI_DRV_CommonInit();
    if(ret)
    {
        HI_ERR_KEYLED("CommonInit init err \n");
    }
    else
    {
        HI_INFO_KEYLED("CommonInit init ok \n");	
    }

    ret = HI_DRV_GPIO_Init();
    if(ret)
    {
        HI_ERR_KEYLED("gpio init err \n");
    }
    else
    {
        HI_INFO_KEYLED("gpio init ok \n");
    }
#endif

    ret = keyled_Param_Init();
    if(ret)
    {
        HI_ERR_KEYLED("keyled Param Init err !");
    }
    else
    {
        HI_INFO_KEYLED("keyled Param Init ok\n");
    }
#if 0
//  keyled_display_begin();
    ret = KEYLED_LED_Display_CT1642(DisplayVal);
    if(ret){
        HI_ERR_KEYLED("keyled display param err !");
    }
#endif

    ret = KEYLED_DisplayString(LedDisplayStr);
    if(ret)
    {
        HI_ERR_KEYLED("keyled display string param err !");
    }

    HI_INFO_KEYLED("<<<<<< keyled_display   end <<<<<<\n");
    return HI_SUCCESS;
}

static void keyled_display_exit(void)
{
    return;
}

//early_initcall(keyled_display_init);
core_initcall(keyled_display_init);

