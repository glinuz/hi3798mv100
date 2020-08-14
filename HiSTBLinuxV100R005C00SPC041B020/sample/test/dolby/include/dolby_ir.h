#ifndef     _DOLBY_IR_H
#define     _DOLBY_IR_H

#include <string.h>
#include "hi_type.h"
#include "hi_unf_common.h"
#include "hi_unf_ecs.h"

#define MAX_IR_MAP     22

/* definition of key-press event */
typedef enum Roc_Event_E
{
    APP_KEY_NUM0      = 0,
    APP_KEY_NUM1 		 ,
    APP_KEY_NUM2 		 ,
    APP_KEY_NUM3 		 ,
    APP_KEY_NUM4 		 ,
    APP_KEY_NUM5 		 ,
    APP_KEY_NUM6 		 ,
    APP_KEY_NUM7 		 ,
    APP_KEY_NUM8 		 ,
    APP_KEY_NUM9 		 ,
    APP_KEY_EXIT 	     ,
    APP_KEY_SEATCH       ,
    APP_KEY_DEL          ,
    APP_KEY_CHANNEL_DOWN ,
    APP_KEY_CHANNEL_UP   ,
    APP_KEY_RED          , /*On the remote control function keys COLORED_KEY_0, RocME means the red button*/
    APP_KEY_YELLOW       , /*On the remote control function keys COLORED_KEY_0, RocME means the yellow button*/
    APP_KEY_BLUE         , /*On the remote control function keys COLORED_KEY_0, RocME means the blue button*/
    APP_KEY_GREEN        , /*On the remote control function keys COLORED_KEY_0, RocME means the green button*/
    APP_KEY_EPG          ,  /* guide key */
    APP_KEY_INFO         ,
    APP_KEY_MENU         ,
    APP_KEY_VOLUME_MUTE  ,
    APP_KEY_SELECT       ,
    APP_KEY_VOLUME_DOWN  ,
    APP_KEY_VOLUME_UP    ,
    APP_KEY_DOWN         ,
    APP_KEY_UP           ,
    APP_KEY_LEFT         ,
    APP_KEY_RIGHT        ,
    APP_KEY_POWER        ,
    APP_KEY_PAGE_UP      ,
    APP_KEY_PAGE_DOWN    ,
    APP_KEY_TRACK        ,  /* audio track key*/
    APP_KEY_LIKE         ,  /* Subtitle button or channel favorite key, that is, TELETEXT*/
    APP_KEY_BACK         ,  /* Return key, that is, TOGGLE*/
    APP_KEY_PROGRAM_LIST ,  /* Channel list button or contact key, that is PROG_LIST*/
    APP_KEY_TV_RADIO     ,  /* Instructions "TV / Audio Broadcast" button*/
    APP_KEY_NVOD         ,  /* Card information or on-demand, interactive keys, namely CARD_INFO */ 
    APP_KEY_MAIL         ,  /* E-mail button*/
}Roc_Event_e;

typedef struct hi_INPUT_KEY_MAP_S
{
    HI_U32 u32Event ;
    HI_U32 u32KeyValue;
} INPUT_KEY_MAP_S;


/*---------------------------------Common Interface--------- ---------------------*/

/**
 * open IR
 * @param: none
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_IR_Open(HI_VOID);

/**
 * close IR
 * @param: none
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_IR_Close(HI_VOID);

/**
 * gat value of IR
 * @param pu32PressStatus: pointer to press status
 * @param pu32KeyId: pointer to key ID
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_IR_GetValue(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId);

#endif
