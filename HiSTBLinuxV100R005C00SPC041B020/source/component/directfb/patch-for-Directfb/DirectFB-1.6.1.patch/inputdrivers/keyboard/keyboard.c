/*
   (c) Copyright 2001-2009  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <neo@directfb.org>,
              Ville SyrjÃ¤lÃ¤ <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>

#include <asm/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <sys/vt.h>

#include <errno.h>

#include <linux/keyboard.h>

#include <termios.h>

#include <directfb.h>

#include <core/coredefs.h>
#include <core/coretypes.h>

#include <core/input.h>
#include <core/system.h>

#include <misc/conf.h>

#include <direct/debug.h>
#include <direct/mem.h>
#include <direct/messages.h>
#include <direct/thread.h>

#include <core/input_driver.h>
#include <directfb_keyboard.h>


#include "hi_type.h"
#include "hi_unf_ecs.h"
#include "hi_common.h"
#include "hi_unf_keyled.h"

#define HI_KEY_NUM 127


DFB_INPUT_DRIVER( HI_KEYBOARD )

typedef enum hiHI_KEYBOARDCODE_E
{
    HI_KEY_1 = 1,    
    HI_KEY_2,            
    HI_KEY_3,            
    HI_KEY_4,            
    HI_KEY_5,            
    HI_KEY_6,            
    HI_KEY_7,            
    HI_KEY_8,            
    HI_KEY_9,            
    HI_KEY_0, 
    
/*ÔÚÒ£¿ØÆ÷ÖÐÒÔÏÂ¿ªÊ¼°´¼ü¸´ÓÃÊý×Ö°´¼ü*/
    HI_KEY_A = 11,
    HI_KEY_B,
    HI_KEY_C,
    HI_KEY_D,
    HI_KEY_E,
    HI_KEY_F,
    HI_KEY_G,
    HI_KEY_H,
    HI_KEY_I,
    HI_KEY_J, 
    HI_KEY_K,
    HI_KEY_L,
    HI_KEY_M,
    HI_KEY_N,
    HI_KEY_O,
    HI_KEY_P,
    HI_KEY_Q,
    HI_KEY_R,
    HI_KEY_S,
    HI_KEY_T,
    HI_KEY_U,
    HI_KEY_V,
    HI_KEY_W,
    HI_KEY_X,
    HI_KEY_Y,
    HI_KEY_Z = 36,

    HI_KEY_a = 37,
    HI_KEY_b,
    HI_KEY_c,
    HI_KEY_d,
    HI_KEY_e,
    HI_KEY_f,
    HI_KEY_g,
    HI_KEY_h,
    HI_KEY_i,
    HI_KEY_j, 
    HI_KEY_k,
    HI_KEY_l,
    HI_KEY_m,
    HI_KEY_n,
    HI_KEY_o,
    HI_KEY_p,
    HI_KEY_q,
    HI_KEY_r,
    HI_KEY_s,
    HI_KEY_t,
    HI_KEY_u,
    HI_KEY_v,
    HI_KEY_w,
    HI_KEY_x,
    HI_KEY_y,
    HI_KEY_z = 62,
    HI_KEY_SPACE = 63,
    HI_KEY_DOT = 64,
    HI_KEY_COLON = 65,
/*½áÊø°´¼ü¸´ÓÃÊý×Ö°´¼ü*/
    
    HI_KEY_ESC  = 66,    
    HI_KEY_CAPS,    
    
    HI_KEY_DEL  = 70,    
    HI_KEY_IME,    
    HI_KEY_FAV,   
    HI_KEY_MORE,   
    HI_KEY_PREV,   
    HI_KEY_REC,
    HI_KEY_NEXT,
    HI_KEY_SLOW,
    HI_KEY_RADIO,
    HI_KEY_LIVE,

    HI_KEY_SEARCH = 80,

    HI_KEY_UP = 81,
    HI_KEY_DOWN,
    HI_KEY_LEFT,
    HI_KEY_RIGHT,
    HI_KEY_ENTER,
    HI_KEY_EXIT,
    HI_KEY_MENU,

    HI_KEY_PLAY = 88,
    HI_KEY_STOP,
    HI_KEY_REW,
    HI_KEY_FF,

    HI_KEY_EPG = 92,
    HI_KEY_PGUP,
    HI_KEY_PGDOWN,
    HI_KEY_BACK,

    HI_KEY_NVOD = 96,
    HI_KEY_BTV,
    HI_KEY_VOD,
    HI_KEY_NPVR,
    HI_KEY_HELP,
    HI_KEY_STOCK,

    HI_KEY_GUIDE = 102,
    HI_KEY_MAIL,
    HI_KEY_NEWS,
    HI_KEY_IPANEL,

    HI_KEY_P1 = 106,
    HI_KEY_P2,

    HI_KEY_F1 = 108,
    HI_KEY_F2,
    HI_KEY_F3,
    HI_KEY_F4,
    
    HI_KEY_RED,
    HI_KEY_YELLOW,
    HI_KEY_BLUE,
    HI_KEY_GREEN,   

    HI_KEY_SET     = 116,
    HI_KEY_SUB,
    HI_KEY_SEEK,
    HI_KEY_INFO,
    HI_KEY_POWER,
    HI_KEY_AUDIO,
    HI_KEY_MUTE,
    HI_KEY_CHNDOWN,
    HI_KEY_CHNUP,
    HI_KEY_VOLDOWN,
    HI_KEY_VOLUP
}HI_KEYBOARD_CODE_E;

typedef struct 
{
     HI_S32                         s32Code;                  /* hardware key code */
     HI_S32			      s32VirKey;		  /*º£Ë¼¼üÖµ*/
} HI_KEYBOARD_MAP;

HI_KEYBOARD_MAP g_KeyBoardMap[HI_KEY_NUM] = 
{
	/*{s32Code,s32VirKey}   ÓÃ»§¸ü»»Ò£¿ØÆ÷Ê±ºòÖ»ÐèÒª¸Ä±äcode Óò*/
	{0x6d92ff00, HI_KEY_1},
	{0x6c93ff00, HI_KEY_2},
	{0x33ccff00, HI_KEY_3},
	{0x718eff00, HI_KEY_4},
	{0x708fff00, HI_KEY_5},
	{0x37c8ff00, HI_KEY_6},
	{0x758aff00, HI_KEY_7},
	{0x748bff00, HI_KEY_8},
	{0x3bc4ff00, HI_KEY_9},
	{0x7887ff00, HI_KEY_0},
	{0x35caff00, HI_KEY_UP},
	{0x6, HI_KEY_DOWN},
	{0x2, HI_KEY_LEFT},
	{0x4, HI_KEY_RIGHT},
	{0x3, HI_KEY_ENTER}, //ok
	{0x629dff00, HI_KEY_EPG},//epg
	{0x30cfff00, HI_KEY_PGUP},
	{0x6798ff00, HI_KEY_PGDOWN},
	{0x6f90ff00, HI_KEY_BACK},//back
	{0x649bff00, HI_KEY_BTV},//btv
	{0x659aff00, HI_KEY_VOD},//vod
	{0x3fc0ff00, HI_KEY_NVOD},//nvod
	{0x3dc2ff00, HI_KEY_NPVR},//npvr
	{0x3cc3ff00, HI_KEY_PLAY},//play/pause
	{0x25daff00, HI_KEY_REW},//¿ìÍË
	{0x29d6ff00, HI_KEY_FF},//¿ì½ø
	{0x2fd0ff00, HI_KEY_STOP},//stop
	{0x7b84ff00, HI_KEY_F1},
	{0x7689ff00, HI_KEY_F2},
	{0x26d9ff00, HI_KEY_F3},
	{0x6996ff00, HI_KEY_F4},
	{0x6a95ff00, HI_KEY_SET},//set
	{0x738cff00, HI_KEY_SUB},//sub
	{0x7d82ff00, HI_KEY_SEEK},//seek
	{0x5, HI_KEY_INFO},//info
	{0x7, HI_KEY_POWER},//power
	{0x2ed1ff00, HI_KEY_AUDIO},//audio
	{0x22ddff00, HI_KEY_MUTE},//mute
	{0x7986ff00, HI_KEY_CHNDOWN},//ch -
	{0x7a85ff00, HI_KEY_CHNUP},//ch +
	{0x7e81ff00, HI_KEY_VOLDOWN},//vol -
	{0x7f80ff00, HI_KEY_VOLUP},//vol+
	{0x609fff00, HI_KEY_IME},//ime
	{0x6b94ff00, HI_KEY_FAV},//fav
	{0x39c6ff00, HI_KEY_MORE},//more
	{0x6897ff00, HI_KEY_SEARCH},//seatch
	{0x7788ff00, HI_KEY_DEL},//del
	{0x34cbff00, HI_KEY_COLON},//-/--
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{}
};	

typedef struct 
{
     HI_S32                         s32Code;                  /* hardware key code */
     DFBInputDeviceKeyIdentifier enIdentifier;            /* basic mapping */
     DFBInputDeviceKeySymbol     enSymbols; 		/* advanced key*/
} HI_KEYBOARDMAP_ENTRY;

typedef struct 
{
	CoreInputDevice *device;
	DirectThread    *thread;
	struct termios   old_ts;
	HI_S32              vt_fd;
} HI_KEYBOARD_DATA;


HI_KEYBOARDMAP_ENTRY g_stKeyBoardArray[HI_KEY_NUM] =
{
	{HI_KEY_1, DIKI_1, DIKS_1},
	{HI_KEY_2, DIKI_2, DIKS_2},
	{HI_KEY_3, DIKI_3, DIKS_3},
	{HI_KEY_4, DIKI_4, DIKS_4},
	{HI_KEY_5, DIKI_5, DIKS_5},
	{HI_KEY_6, DIKI_6, DIKS_6},
	{HI_KEY_7, DIKI_7, DIKS_7},
	{HI_KEY_8, DIKI_8, DIKS_8},
	{HI_KEY_9, DIKI_9, DIKS_9},
	{HI_KEY_0, DIKI_0, DIKS_0},
	{HI_KEY_UP, DIKI_UP, DIKS_CURSOR_UP},
	{HI_KEY_DOWN, DIKI_DOWN, DIKS_CURSOR_DOWN},
	{HI_KEY_LEFT, DIKI_LEFT, DIKS_CURSOR_LEFT},
	{HI_KEY_RIGHT, DIKI_RIGHT, DIKS_CURSOR_RIGHT},
	{HI_KEY_ENTER, DIKI_ENTER, DIKS_OK},
	{HI_KEY_EPG, DIKI_HOME, DIKS_EPG},//epg
	{HI_KEY_PGUP, DIKI_PAGE_UP, DIKS_PAGE_UP},
	{HI_KEY_PGDOWN, DIKI_PAGE_DOWN, DIKS_PAGE_DOWN},
	{HI_KEY_BACK, DIKI_BACKSPACE, DIKS_BACKSPACE},//back
	{HI_KEY_BTV, 0, DIKS_TV},//btv
	{HI_KEY_VOD, 0, 0},//vod
	{HI_KEY_NVOD, 0, 0},//nvod
	{HI_KEY_NPVR, 0, DIKS_PVR},//npvr
	{HI_KEY_PLAY, DIKI_PAUSE, DIKS_PLAYPAUSE},//play/pause
	{HI_KEY_REW, DIKI_SUPER_L, DIKS_REWIND},//¿ìÍË
	{HI_KEY_FF, DIKI_SUPER_R, DIKS_SHUFFLE},//¿ì½ø
	{HI_KEY_STOP, DIKI_END, DIKS_STOP},//stop
	{HI_KEY_F1, DIKI_F1, DIKS_F1},
	{HI_KEY_F2, DIKI_F2, DIKS_F2},
	{HI_KEY_F3, DIKI_F3, DIKS_F3},
	{HI_KEY_F4, DIKI_F4, DIKS_F4},
	{HI_KEY_SET, DIKI_INSERT, DIKS_SETUP},//set
	{HI_KEY_SUB, 0, DIKS_SUBTITLE},//sub
	{HI_KEY_SEEK, 0, 0},//seek
	{HI_KEY_INFO, 0, DIKS_INFO},//info
	{HI_KEY_POWER, DIKI_ESCAPE, DIKS_POWER},//power
	{HI_KEY_AUDIO, 0, DIKS_AUDIO},//audio
	{HI_KEY_MUTE, 0, DIKS_MUTE},//mute
	{HI_KEY_CHNDOWN, DIKI_SHIFT_L, DIKS_CHANNEL_DOWN},//ch -
	{HI_KEY_CHNUP, DIKI_SHIFT_R, DIKS_CHANNEL_UP},//ch +
	{HI_KEY_VOLDOWN, DIKI_CONTROL_L, DIKS_VOLUME_DOWN},//vol -
	{HI_KEY_VOLUP, DIKI_CONTROL_R, DIKS_VOLUME_UP},//vol+
	{HI_KEY_IME, DIKI_ALT_L, DIKS_INSERT},//ime
	{HI_KEY_FAV, DIKI_ALT_R, DIKS_FAVORITES},//fav
	{HI_KEY_MORE, DIKI_META_L, DIKS_OPTION},//more
	{HI_KEY_SEARCH, DIKI_META_R, DIKS_GOTO},//seatch
	{HI_KEY_DEL, DIKI_DELETE, DIKS_DELETE},//del
	{HI_KEY_COLON, DIKI_TAB, DIKS_COLON},//-/--
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{}
};

static HI_S32
keytable_map( HI_S32 code, DFBInputDeviceKeyIdentifier *penIdentifier, DFBInputDeviceKeySymbol *penSymbol)
{
	HI_S32 i;
	HI_S32 s32Ret = 0;
	/*Ó³Éäµ½DFB¼üÖµ*/
	for(i = 0;i <HI_KEY_NUM; i++ )
	{
		if(code == g_stKeyBoardArray[i].s32Code)
		{
			*penIdentifier = g_stKeyBoardArray[i].enIdentifier;
			*penSymbol =  g_stKeyBoardArray[i].enSymbols;	
			return 0;
		}
	}
	
	return -1;
}

 

static void* keyboardEventThread( DirectThread *thread, void *driver_data )
{
	HI_S32 i;
	HI_S32             s32Ret;
	HI_UNF_KEY_STATUS_E penPressStatus;
	HI_U64 pu64KeyId = 0;
	HI_S32 s32VCode = 0;
	HI_U32 u32TimeoutMs = 1000;
	HI_KEYBOARD_DATA  *pstData = (HI_KEYBOARD_DATA*) driver_data;

	HI_UNF_KEY_SetBlockTime(u32TimeoutMs);

	while ( pstData)
	{
		DFBInputEvent stEvt;
		s32Ret =  HI_UNF_KEY_GetValue(&penPressStatus, &pu64KeyId);
		if(s32Ret)
		{
			 usleep(50000);
			 continue;
		}

		if(HI_UNF_KEY_STATUS_DOWN == penPressStatus)
		{
			stEvt.type     =DIET_KEYPRESS;
		}
		else if(HI_UNF_KEY_STATUS_UP == penPressStatus)
		{
			stEvt.type     =DIET_KEYRELEASE;
		}
		else
		{
			//stEvt.type     =DIET_AXISMOTION;
		}

		stEvt.flags    =  DIEF_KEYCODE;
		/*Ó³Éäµ½º£Ë¼¼üÖµ*/
		for(i = 0; i < HI_KEY_NUM; i++)
		{
			if(g_KeyBoardMap[i].s32Code == pu64KeyId )
			{
				s32VCode = g_KeyBoardMap[i].s32VirKey;
				break;
			}
		}
		stEvt.key_code = s32VCode;

		dfb_input_dispatch( pstData->device, &stEvt );
	}

	return NULL;
}


static int driver_get_available( void )
{
     return 1;
}

static void driver_get_info( InputDriverInfo *info )
{
     snprintf( info->name, DFB_INPUT_DRIVER_INFO_NAME_LENGTH, "HI_IR Driver" );
     snprintf( info->vendor,DFB_INPUT_DRIVER_INFO_VENDOR_LENGTH, "directfb.org" );
	 
     info->version.major = 0;
     info->version.minor = 3;
}

static DFBResult driver_open_device( CoreInputDevice  *device, unsigned int      number,
                    InputDeviceInfo  *info, void  **driver_data )
{
	HI_S32             nRet;
	HI_KEYBOARD_DATA   *pstData;

       HI_SYS_Init();
	   
	nRet = HI_UNF_KEYLED_Init();
	if(nRet)
	{
		D_PERROR( "DirectFB/KEYLED: HI_UNF_KEYLED_Init FAILD!\n" );
		HI_SYS_DeInit();
		return DFB_INIT;
	}
	
    nRet = HI_UNF_KEYLED_SelectType(HI_UNF_KEYLED_TYPE_CT1642);
    if(HI_SUCCESS != nRet)
    {
        D_PERROR( "DirectFB/KEYLED: HI_UNF_KEYLED_SelectType FAILD!\n" );
        goto ERR1;
    }

	nRet = HI_UNF_LED_Open();
	if (HI_SUCCESS != nRet)
	{
		D_PERROR( "DirectFB/KEYLED: HI_UNF_KEYLED_Open FAILD!\n" );
		goto ERR1;
	}
	
	nRet = HI_UNF_KEY_Open();
	if(nRet)
	{
		D_PERROR( "DirectFB/KEYBOAD: Could not open() !\n" );
		goto ERR2;
	}
	
	/*ÉèÖÃ²»ÉÏ±¨°´¼üµ¯Æð×´Ì¬*/
	nRet = HI_UNF_KEY_IsKeyUp(0);
	if(nRet)
	{
		D_PERROR( "DirectFB/HI_IR: Could not SET EnableKeyUp() !\n" );
		goto ERR2;
	}

	/*ÉèÖÃÖØ¸´°´¼üÉÏ±¨¼ä¸ôÊ±¼ä*/
	nRet =  HI_UNF_KEY_RepKeyTimeoutVal(2048);
	if(nRet)
	{
		D_PERROR( "DirectFB/HI_IR: SetRepKeyTimeoutAttr FAILT !\n" );
		goto ERR2;
	}

	pstData = D_CALLOC( 1, sizeof(HI_KEYBOARD_DATA) );
	if (!pstData) 
	{
		goto ERR2;
	}

	pstData->device = device;

	snprintf( info->desc.name, DFB_INPUT_DEVICE_DESC_NAME_LENGTH, "HI_KEYBOARD" );
	snprintf( info->desc.vendor, DFB_INPUT_DEVICE_DESC_VENDOR_LENGTH, "Unknown" );

	info->prefered_id = DIDID_REMOTE;
	info->desc.type   = DIDTF_REMOTE;
	info->desc.caps   = DICAPS_KEYS;
	info->desc.min_keycode = 0;
	info->desc.max_keycode = 127;

	pstData->thread = direct_thread_create( DTT_INPUT, keyboardEventThread, pstData, "HI_KEYBOARD" );
	*driver_data = pstData;

	return DFB_OK;

	ERR2:	
	HI_UNF_KEY_Close();
	ERR1:
	HI_UNF_LED_Close();
	HI_UNF_KEYLED_DeInit();
	HI_SYS_DeInit();
		
	return DFB_INIT;
	
}

static DFBResult  driver_get_keymap_entry( CoreInputDevice               *device,
                         void                      *driver_data,
                         DFBInputDeviceKeymapEntry *entry )
{
	HI_S32 s32Ret;
	DFBInputDeviceKeyIdentifier enIdentifier;
	DFBInputDeviceKeySymbol enSymbol;
	HI_S32 s32Code = entry->code;

	s32Ret = keytable_map(s32Code,&enIdentifier,&enSymbol);
	if(s32Ret)
	{
		return DFB_INVARG;
	}

	if (enIdentifier >= DIKI_KP_DECIMAL && enIdentifier <= DIKI_KP_9)
	{
		entry->locks |= DILS_NUM;
	}

	entry->identifier = enIdentifier;
	entry->symbols[DIKSI_BASE] =enSymbol;

	return DFB_OK;
}

static void driver_close_device( void *driver_data )
{
	HI_KEYBOARD_DATA *pstData = (HI_KEYBOARD_DATA*) driver_data;
 
	direct_thread_cancel( pstData->thread );
	direct_thread_join( pstData->thread );
	direct_thread_destroy( pstData->thread );

	HI_UNF_KEY_Close();
	HI_UNF_LED_Close();
	HI_UNF_KEYLED_DeInit();
	HI_SYS_DeInit();

	D_FREE( pstData );
}

