/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
File Name     : ir_keyboad.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2010/11/15
Description   : ir_keyboad.c header file
History       :
1.Date        : 2010/11/15
Author      : f00104257
Modification: Created file
******************************************************************************/
#ifndef HI_MINIBOOT_SUPPORT
#include <linux/init.h>
#include <linux/input.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/seq_file.h>
#include <linux/ptrace.h>
#include <linux/timer.h>
#include <linux/log2.h>
#include <linux/mii.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <asm/system.h>
#include <asm/byteorder.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/memory.h>
#include <mach/hardware.h>
#endif

/* key value define*/
#define IR_KEY_UP               0x35caff00
#define IR_KEY_DOWN             0x2dd2ff00
#define IR_KEY_LEFT            0x6699ff00
#define IR_KEY_RIGHT             0x3ec1ff00
#define IR_KEY_OK               0x31ceff00
#define IR_KEY_BACK             0x6f90ff00
#define IR_KEY_MENU             0x629dff00
#define IR_KEY_POWER            0x639cff00
#define IR_KEY_HOME             0x34cbff00
#define IR_KEY_VOLADD           0x7f80ff00
#define IR_KEY_VOLSUB           0x7e81ff00
#define IR_KEY_MUTE             0x22ddff00
#define IR_KEY_1                0x6d92ff00
#define IR_KEY_2                0x6c93ff00
#define IR_KEY_3                0x33ccff00
#define IR_KEY_4                0x718eff00
#define IR_KEY_5                0x708fff00
#define IR_KEY_6                0x37c8ff00
#define IR_KEY_7                0x758aff00
#define IR_KEY_8                0x748bff00
#define IR_KEY_9                0x3bc4ff00
#define IR_KEY_0                0x7887ff00
#define IR_KEY_F1               0x7b84ff00
#define IR_KEY_F2               0x7689ff00
#define IR_KEY_F3               0x26d9ff00
#define IR_KEY_F4               0x6996ff00
#define IR_KEY_SEARCH           0x6897ff00
#define IR_KEY_REWIND           0x25daff00
#define IR_KEY_FORWARD          0x29d6ff00
#define IR_KEY_STOP             0x2fd0ff00
#define IR_KEY_SETUP            0x6a95ff00
#define IR_KEY_INFO             0x38c7ff00
#define IR_KEY_AUDIO            0x2ed1ff00
#define IR_KEY_SUBTITLE         0x738cff00
#define IR_KEY_BACKSPACE        0x7788ff00
#define IR_KEY_PLAYPAUSE        0x3cc3ff00
#define IR_KEY_FAVORITES        0x6b94ff00
#define IR_KEY_CHANNELUP        0x7a85ff00
#define IR_KEY_CHANNELDOWN      0x7986ff00
#define IR_KEY_PAGEDOWN         0x6798ff00
#define IR_KEY_PAGEUP           0x30cfff00
#define IR_KEY_IME              0x609fff00
#define IR_KEY_MORE             0x39c6ff00
#define IR_KEY_BTV              0x649bff00
#define IR_KEY_VOD              0x659aff00
#define IR_KEY_NVOD             0x3fc0ff00
#define IR_KEY_NPVR             0x3dc2ff00
#define IR_KEY_SEEK             0x7d82ff00

#define PN_KEY_UP               0x639cf300
#define PN_KEY_DOWN             0x6798f300
#define PN_KEY_RIGHT            0x6b94f300
#define PN_KEY_LEFT             0x6f90f300
#define PN_KEY_OK               0x738cf300
#define PN_KEY_BACK             0x7788f300
#define PN_KEY_MENU             0x7b84f300

#define ML_IR_KEY_UP            0xbc439f00
#define ML_IR_KEY_DOWN          0xf50a9f00
#define ML_IR_KEY_LEFT          0xf9069f00
#define ML_IR_KEY_RIGHT         0xf10e9f00
#define ML_IR_KEY_OK            0xfd029f00
#define ML_IR_KEY_BACK          0xb04f9f00
#define ML_IR_KEY_MENU          0xe9169f00
#define ML_IR_KEY_POWER         0xa8579f00
#define ML_IR_KEY_HOME          0xb8479f00
#define ML_IR_KEY_VOLADD        0xff009f00
#define ML_IR_KEY_VOLSUB        0xa25d9f00
#define ML_IR_KEY_MUTE          0xa35c9f00
#define ML_IR_KEY_1             0xe51a9f00
#define ML_IR_KEY_2             0xfe019f00
#define ML_IR_KEY_3             0xba459f00
#define ML_IR_KEY_4             0xa6599f00
#define ML_IR_KEY_5             0xb24d9f00
#define ML_IR_KEY_6             0xbf409f00
#define ML_IR_KEY_7             0xad529f00
#define ML_IR_KEY_8             0xac539f00
#define ML_IR_KEY_9             0xbe419f00
#define ML_IR_KEY_0             0xa55a9f00
#define ML_IR_KEY_F1            0xaa559f00
#define ML_IR_KEY_F2            0xb14e9f00
#define ML_IR_KEY_F3            0xb6499f00
#define ML_IR_KEY_F4            0xb7489f00
#define ML_IR_KEY_SEARCH        0xf20d9f00
#define ML_IR_KEY_REWIND        0xf8079f00
#define ML_IR_KEY_FORWARD       0xfc039f00
#define ML_IR_KEY_STOP          0xec139f00
#define ML_IR_KEY_SETUP         0xf6099f00
#define ML_IR_KEY_INFO          0xef109f00
#define ML_IR_KEY_AUDIO         0xbb449f00
#define ML_IR_KEY_SUBTITLE      0xb9469f00
#define ML_IR_KEY_PLAYPAUSE     0xaf509f00
#define ML_IR_KEY_FAVORITES     0xea159f00
#define ML_IR_KEY_PAGEDOWN      0xf40b9f00
#define ML_IR_KEY_PAGEUP        0xf00f9f00
#define ML_IR_KEY_TVSYS         0xfb049f00
#define ML_IR_KEY_REPEAT        0xf7089f00
#define ML_IR_KEY_FOCUS         0xee119f00
#define ML_IR_KEY_HELP          0xfa059f00
#define ML_IR_KEY_EJECT         0xeb149f00

#define ML_IR_KEY_MOVIE			0xab549f00
#define ML_IR_KEY_MYAPP			0xa45b9f00
#define ML_IR_KEY_BROWSER		0xe8179f00
#define ML_IR_KEY_ZOOMIN        0xa7589f00
#define ML_IR_KEY_ZOOMOUT       0xf30c9f00

/* input event */
typedef struct
{
	HI_U64 SrcKeyId;
	HI_U32 linux_key_code;

}IR_KEYMAP_S;

const IR_KEYMAP_S Key_Code[] =
{
	{IR_KEY_UP,            KEY_UP           },   //0x35caff00
	{IR_KEY_DOWN,          KEY_DOWN         },   //0x2dd2ff00
	{IR_KEY_RIGHT,         KEY_RIGHT         },   //0x6699ff00
	{IR_KEY_LEFT,          KEY_LEFT        },   //0x3ec1ff00
	{IR_KEY_OK,            KEY_ENTER        },   //0x31ceff00
	{IR_KEY_BACK,          KEY_BACK         },   //0x6f90ff00
	{IR_KEY_MENU,          KEY_MENU         },   //0x629dff00
	{IR_KEY_POWER,         KEY_POWER        },   //0x639cff00
	{IR_KEY_HOME,          KEY_HOME         },   //0x34cbff00
	{IR_KEY_VOLADD,        KEY_VOLUMEUP     },   //0x7f80ff00
	{IR_KEY_VOLSUB,        KEY_VOLUMEDOWN   },   //0x7e81ff00
	{IR_KEY_MUTE,          KEY_MUTE         },   //0x22ddff00
	{IR_KEY_1,             KEY_1			},	 //0x6d92ff00
	{IR_KEY_2,             KEY_2			},	 //0x6c93ff00
	{IR_KEY_3,             KEY_3			},	 //0x33ccff00
	{IR_KEY_4,             KEY_4			},	 //0x718eff00
	{IR_KEY_5,             KEY_5			},	 //0x708fff00
	{IR_KEY_6,             KEY_6			},	 //0x37c8ff00
	{IR_KEY_7,             KEY_7			},	 //0x758aff00
	{IR_KEY_8,             KEY_8			},	 //0x748bff00
	{IR_KEY_9,             KEY_9			},	 //0x3bc4ff00
	{IR_KEY_0,             KEY_0			},	 //0x7887ff00
	{IR_KEY_F1,            KEY_F1			},	 //0x7b84ff00
	{IR_KEY_F2,            KEY_F2			},	 //0x7689ff00
	{IR_KEY_F3,            KEY_F3			},	 //0x26d9ff00
	{IR_KEY_F4,            KEY_F4			},	 //0x6996ff00
	{IR_KEY_SEARCH,        KEY_SEARCH       },   //0x6897ff00
	{IR_KEY_REWIND,        KEY_REWIND       },   //0x25daff00
	{IR_KEY_FORWARD,       KEY_FORWARD      },   //0x29d6ff00
	{IR_KEY_STOP,          KEY_STOP         },   //0x2fd0ff00
	{IR_KEY_SETUP,         KEY_SETUP        },   //0x6a95ff00
	{IR_KEY_INFO,          KEY_INFO         },   //0x38c7ff00
	{IR_KEY_AUDIO,         KEY_AUDIO        },   //0x2ed1ff00
	{IR_KEY_SUBTITLE,      KEY_SUBTITLE     },   //0x738cff00
	{IR_KEY_BACKSPACE,     KEY_BACKSPACE	},	 //0x7788ff00
	{IR_KEY_PLAYPAUSE,     KEY_PLAYPAUSE	},	 //0x3cc3ff00
	{IR_KEY_FAVORITES,     KEY_FAVORITES	},	 //0x6b94ff00
	{IR_KEY_CHANNELUP,     KEY_CHANNELUP	},	 //0x7a85ff00
	{IR_KEY_CHANNELDOWN,   KEY_CHANNELDOWN  },   //0x7986ff00
	{IR_KEY_PAGEDOWN,      KEY_PAGEDOWN     },   //0x6798ff00
	{IR_KEY_PAGEUP,        KEY_PAGEUP       },   //0x30cfff00
	{IR_KEY_IME,           KEY_FN_1         },   //0x609fff00
	{IR_KEY_MORE,          KEY_FN_2         },   //0x39c6ff00
	{IR_KEY_BTV,           KEY_FN_D			},	 //0x649bff00
	{IR_KEY_VOD,           KEY_FN_E			},	 //0x659aff00
	{IR_KEY_NVOD,          KEY_FN_F			},	 //0x3fc0ff00
	{IR_KEY_NPVR,          KEY_FN_S			},	 //0x3dc2ff00
	{IR_KEY_SEEK,          KEY_FN_B			},	 //0x7d82ff00

	{PN_KEY_UP,            KEY_UP           },   //0x639cf300
	{PN_KEY_DOWN,          KEY_DOWN         },   //0x6798f300
	{PN_KEY_RIGHT,         KEY_LEFT         },   //0x6f90f300
	{PN_KEY_LEFT,          KEY_RIGHT        },   //0x6b94f300
	{PN_KEY_OK,            KEY_ENTER        },   //0x738cf300
	{PN_KEY_BACK,          KEY_BACK			},	 //0x7788f300
	{PN_KEY_MENU,          KEY_MENU			},	 //0x7b84f300

	{ML_IR_KEY_UP,         KEY_UP           },   //0xbc439f00
	{ML_IR_KEY_DOWN,       KEY_DOWN         },   //0xf50a9f00
	{ML_IR_KEY_RIGHT,      KEY_RIGHT        },
	{ML_IR_KEY_LEFT,       KEY_LEFT         },
	{ML_IR_KEY_OK,         KEY_ENTER        },   //0xfd029f00
	{ML_IR_KEY_BACK,       KEY_BACK         },   //0xb04f9f00
	{ML_IR_KEY_MENU,       KEY_MENU         },   //0xe9169f00
	{ML_IR_KEY_POWER,      KEY_POWER        },   //0xa8579f00
	{ML_IR_KEY_HOME,       KEY_HOME         },   //0xb8479f00
	{ML_IR_KEY_VOLADD,     KEY_VOLUMEUP     },   //0xe8179f00
	{ML_IR_KEY_VOLSUB,     KEY_VOLUMEDOWN   },   //0xab549f00
	{ML_IR_KEY_MUTE,       KEY_MUTE         },   //0xa45b9f00
	{ML_IR_KEY_1,          KEY_1			},	 //0xfa059f00
	{ML_IR_KEY_2,          KEY_2			},	 //0xf6099f00
	{ML_IR_KEY_3,          KEY_3			},	 //0xea159f00
	{ML_IR_KEY_4,          KEY_4			},	 //0xfb049f00
	{ML_IR_KEY_5,          KEY_5			},	 //0xf7089f00
	{ML_IR_KEY_6,          KEY_6			},	 //0xeb149f00
	{ML_IR_KEY_7,          KEY_7			},	 //0xff009f00
	{ML_IR_KEY_8,          KEY_8			},	 //0xef109f00
	{ML_IR_KEY_9,          KEY_9			},	 //0xf30c9f00
	{ML_IR_KEY_0,          KEY_0			},	 //0xee119f00
	{ML_IR_KEY_F1,         KEY_F1			},	 //0xaa559f00
	{ML_IR_KEY_F2,         KEY_F2			},	 //0xb14e9f00
	{ML_IR_KEY_F3,         KEY_F3			},	 //0xb6499f00
	{ML_IR_KEY_F4,         KEY_F4			},	 //0xb7489f00
	{ML_IR_KEY_SEARCH,     KEY_SEARCH       },   //0xbe419f00
	{ML_IR_KEY_REWIND,     KEY_REWIND       },   //0xf8079f00
	{ML_IR_KEY_FORWARD,    KEY_FORWARD      },   //0xfc039f00
	{ML_IR_KEY_STOP,       KEY_STOP         },   //0xec139f00
	{ML_IR_KEY_SETUP,      KEY_SETUP        },   //0xe51a9f00
	{ML_IR_KEY_INFO,       KEY_INFO         },
	{ML_IR_KEY_AUDIO,      KEY_AUDIO        },
	{ML_IR_KEY_SUBTITLE,   KEY_SUBTITLE     },
	{ML_IR_KEY_PLAYPAUSE,  KEY_PLAYPAUSE    },
	{ML_IR_KEY_FAVORITES,  KEY_FAVORITES    },
	{ML_IR_KEY_PAGEDOWN,   KEY_PAGEDOWN     },
	{ML_IR_KEY_PAGEUP,     KEY_PAGEUP       },
	{ML_IR_KEY_TVSYS,      KEY_TV           },
	{ML_IR_KEY_REPEAT,     KEY_REDO         },
	{ML_IR_KEY_FOCUS,      KEY_F5			},
	{ML_IR_KEY_HELP,       KEY_HELP         },
	{ML_IR_KEY_EJECT,      KEY_EJECTCLOSECD },
	{ML_IR_KEY_MYAPP,      KEY_COMPUTER     },
	{ML_IR_KEY_MOVIE,      KEY_MEDIA        },
	{ML_IR_KEY_BROWSER,    KEY_WWW          },
	{ML_IR_KEY_ZOOMOUT,    KEY_ZOOMOUT      },
	{ML_IR_KEY_ZOOMIN,     KEY_ZOOMIN       },
};


typedef struct {
	struct input_dev *input;
	int irq;
	unsigned int addr;
	char *name;
}hi_event_dev;

static hi_event_dev *g_irkeypad_edev  = NULL;

#define MOUSE_MODE_EN             1
#define MOUSE_MAX_STEP            100
#define MOUSE_MIN_STEP            10

static int    g_mouse_move_step = MOUSE_MIN_STEP;
#define KPC_EV_KEY_PRESS          (0x1)
#define KPC_EV_KEY_RELEASE        (0x0)




/******************************added by wkf45780 for ir module suspend and resume start **/
static unsigned int IR_CFG_Val;
static unsigned int IR_LEADS_Val;
static unsigned int IR_LEADE_Val;
static unsigned int IR_SLEADE_Val;
static unsigned int IR_B0_Val;
static unsigned int IR_B1_Val;
static unsigned int IR_DATAH_Val;
static unsigned int IR_DATAL_Val;
/******************************added by wkf45780 for ir module suspend and resume end **/



/*************************************************************
Function:       HI_IR_GetValue
Description:    get the value and status of key
Calls:
Data Accessed:
Data Updated:   NA
Input:          u32TimeOut: overtime value with unit of ms : 0 means no block while 0xFFFFFFFF means block forever
Output:         pu32PressStatus:    status of the key
                   0:  means press or hold
                   1:  means release
                pu32KeyId:          value of the key

Return:         HI_SUCCESS
                HI_FAILURE
Others:         NA
*************************************************************/
HI_S32 HI_IR_GetValue(HI_UNF_KEY_STATUS_E *penPressStatus, struct key_attr Irkey, HI_U64 *pu64KeyId)
{
	*penPressStatus = Irkey.key_stat;
	*pu64KeyId = Irkey.lower;
	  return HI_SUCCESS;
}

HI_S32 IR_GetMapKey(IR_KEYMAP_S *pIRKeyMap,
HI_U32 IRKeyNum,HI_U64 SrcKeyId,HI_U32 *pUsrKeyId)
{
	HI_S32 i;

	for ( i = 0 ; i < IRKeyNum ; i++ )
	{
		if (SrcKeyId == pIRKeyMap[i].SrcKeyId)
		{
		    *pUsrKeyId = pIRKeyMap[i].linux_key_code;
		    return HI_SUCCESS;
		}
	}

	return HI_FAILURE;
}

static int android_ir_resume(void)
{
	hi_event_dev *edev = g_irkeypad_edev;
	input_event(edev->input, EV_KEY, KEY_WAKEUP, KPC_EV_KEY_PRESS);
	input_event(edev->input, EV_KEY, KEY_WAKEUP, KPC_EV_KEY_RELEASE);
	input_sync(edev->input);
	return 0;
}

static int android_ir_suspend(void)
{
	return 0;
}

static ssize_t android_ir_put(const struct key_attr irkey_to_user)
{
	HI_U64 keyval;
	HI_U32 flg;
	HI_S32 Ret;
	HI_U32	linux_key_code;
	static HI_U32 u32MouseMode = 0;
	hi_event_dev *edev = g_irkeypad_edev;
	HI_UNF_KEY_STATUS_E PressStatus;

	flg = 1;
	HI_IR_GetValue(&PressStatus, irkey_to_user, &keyval);

	/* 3. check and report keys*/

#if MOUSE_MODE_EN == 1		//this code is only for mouse mode

	if((IR_KEY_NPVR == keyval) || (ML_IR_KEY_FOCUS == keyval)) {
		if(HI_UNF_KEY_STATUS_UP == PressStatus) {
			printk("2222\n");
			u32MouseMode = !u32MouseMode;
			input_event(edev->input, EV_KEY, KEY_PVR, u32MouseMode);
		}
	}

	if(u32MouseMode) {
		if(PressStatus !=HI_UNF_KEY_STATUS_HOLD) {
			g_mouse_move_step = MOUSE_MIN_STEP;
		}
		else {
			g_mouse_move_step = ((g_mouse_move_step+10)>MOUSE_MAX_STEP ? MOUSE_MAX_STEP : (g_mouse_move_step+10));
		}

		if((IR_KEY_UP == keyval) || (ML_IR_KEY_UP == keyval)) {			     //key up
			flg = 0;
			input_event(edev->input, EV_REL, REL_Y, -g_mouse_move_step);
		}
		else if((IR_KEY_DOWN == keyval) || (ML_IR_KEY_DOWN == keyval)) {	 //key down
			flg = 0;
			input_event(edev->input, EV_REL, REL_Y, g_mouse_move_step);
		}
		else if((IR_KEY_LEFT == keyval) || (ML_IR_KEY_LEFT == keyval)) {	//key left
			flg = 0;
			input_event(edev->input, EV_REL, REL_X, -g_mouse_move_step);
		}
		else if((IR_KEY_RIGHT == keyval) || (ML_IR_KEY_RIGHT == keyval)) {	 //key right
			flg = 0;
			input_event(edev->input, EV_REL, REL_X, g_mouse_move_step);
		}
		else if((IR_KEY_OK == keyval) || (ML_IR_KEY_OK == keyval)) {	     //key ok
			flg = 0;
			if(HI_UNF_KEY_STATUS_BUTT == PressStatus) {
				input_event(edev->input, EV_KEY, BTN_LEFT, 0x01);
				input_event(edev->input, EV_KEY, BTN_LEFT, 0x00);
			}
			else if(HI_UNF_KEY_STATUS_DOWN == PressStatus) {
				input_event(edev->input, EV_KEY,BTN_LEFT, 0x01);
			}
			else if(HI_UNF_KEY_STATUS_UP == PressStatus){
				input_event(edev->input, EV_KEY,BTN_LEFT, 0x00);
			}
		}
	}

	if(flg) {

		Ret=IR_GetMapKey(Key_Code,sizeof(Key_Code)/sizeof(Key_Code[0]),keyval,&linux_key_code);
		if(Ret == HI_FAILURE)
		{
		     printk("there is no key for \n");
		     return -1;
		}
		if(PressStatus ==HI_UNF_KEY_STATUS_BUTT)
		{
		   	printk("!!!!KEY ERR!!!!\n");
		}
		else if(PressStatus ==HI_UNF_KEY_STATUS_DOWN)
		{
		       input_event(edev->input, EV_KEY, linux_key_code,0x01);
		}
		else if(PressStatus ==HI_UNF_KEY_STATUS_UP)
		{
		       input_event(edev->input, EV_KEY, linux_key_code,0x00);
		}

	}

#endif

#if MOUSE_MODE_EN == 0

		Ret=IR_GetMapKey(Key_Code,sizeof(Key_Code)/sizeof(Key_Code[0]),keyval,&linux_key_code);
		if(Ret == HI_FAILURE)
		{
		     printk("there is no key for \n");
		     return -1;
		}
		if(PressStatus ==HI_UNF_KEY_STATUS_BUTT)
		{
		   	printk("!!!!KEY ERR!!!!\n");
		}
		else if(PressStatus ==HI_UNF_KEY_STATUS_DOWN)
		{
		       input_event(edev->input, EV_KEY, linux_key_code,0x01);
		}
		else if(PressStatus ==HI_UNF_KEY_STATUS_UP)
		{
		       input_event(edev->input, EV_KEY, linux_key_code,0x00);
		}


#endif




//	printk("#####keyval=0x%llx\n",keyval);
//	printk("#####linux_key_code=0x%x\n",linux_key_code);
//	printk("#####PressStatus=0x%x\n",PressStatus);
/**/
	/* 4. clear interrupt only we involved */
	input_sync(edev->input);

	return 0;

}



static HI_S32 android_ir_register_inputdevice()
{
	hi_event_dev *edev;
	int i;
	int retval;

	//1. alloc input device resource
	edev = kzalloc(sizeof(hi_event_dev), GFP_KERNEL);
	if(!edev) {
		printk("events edev or hi_keypad_ input_dev alloc fail!\n");
		return -1;
	}

	edev->name = "ir_android";
	edev->input = input_allocate_device();

	// 2. indicate that we generate key events
	set_bit(EV_KEY, edev->input->evbit);
	set_bit(EV_REL, edev->input->evbit);

	for(i=1; i<= 0x1ff; i++) {
		set_bit(i, edev->input->keybit);
	}

	set_bit(EV_SYN, edev->input->relbit);
	set_bit(EV_KEY, edev->input->relbit);

	//3. set input date and register device
	//platform_set_drvdata(pdev, edev);

	edev->input->name = edev->name;
	edev->input->id.bustype = BUS_HOST;
	edev->input->id.vendor = 0x0001;
	edev->input->id.product = 0x0001;
	edev->input->id.version = 0x0100;

	if(input_register_device(edev->input)) {
		printk("input_register_device fail!\n");
		input_free_device(edev->input);
		kfree(edev);
		return -1;
	}
	g_irkeypad_edev = edev;
	printk("input_register sucfully\n");
	return 0;

}

HI_S32 android_ir_unregister_inputdevice()
{
	hi_event_dev *edev = g_irkeypad_edev;
	input_unregister_device(edev->input);
	kfree(edev);
	printk("input_unregister  sucfully\n");
}
















