#ifndef     _DOLBY_WIN_H
#define     _DOLBY_WIN_H

#include <string.h>
#include "hi_type.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "hi_unf_ecs.h"
#include "dolby_service.h"




#define GRPH_FONT_SIZE 29

/**** program search language deifne****/
#define DOLBY_L_TILE_SEARCHSTYLE  " manual search "
#define DOLBY_L_TITLE_FREQ        " search freq(MHz) "
#define DOLBY_L_TITLE_SYM_RATE    " symbol rate(KS/s)  "
#define DOLBY_L_TITLE_MODULATE    " modulate type:  "
#define DOLBY_L_TITLE_TUNERDEV    " tuner type: "

#define DOLBY_L_BUTTON_START      "    start     "

/*for Prog Search faild Window*/
#define DOLBY_L_SEARCH_MSG        " Search failed! Please check parameter and make sure the cable is connected! "
#define DOLBY_L_SEARCH_MSG_OK     " Please press 'ok' to reset parameter, then search program. "

#define NO_BUTTON_SELECT 3
#define UPDATA_CANCEL 2
#define START_SELECT 1
#define EXIT_SELECT 0


typedef struct hi_POSITIONXY_S
{
    HI_U32 u32x;    /* coordinate X */
    HI_U32 u32y;    /* coordinate Y */
}POSITIONXY_S;

typedef enum hi_WINDTYPE_E
{
    APP_WIND_NOTES,     /* notes window, excalmatory point on the left while use TIANBAI */
                        /* CNcomment: 提示窗口，针对天柏是左侧有感叹号*/
    APP_WIND_INPUT      /* input window */
}WINDTYPE_E;

typedef enum hi_WMEMBTYPE_E
{
    APP_WMEMB_INPUT,     /* character input frame */
    APP_WMEMB_BUTTOM,    /* buttom frame */
    APP_WMEMB_TRANS,     /* transfer frame */
    APP_WMEMB_TRANS_2,
    APP_WMEMB_PASSWD     /* password frame */
} WMEMBTYPE_E;

typedef enum hi_CABLE_MOD_E
{
    QAM16  = 0x00,      /* 16QAM */
    QAM32  = 0x01,      /* 32QAM */
    QAM64  = 0x02,      /* 64QAM */
    QAM128 = 0x03,      /* 128QAM */
    QAM256 = 0x04       /* 256QAM */
} CABLE_MOD_E;

typedef struct hi_WINDMEMBER_S
{
    HI_VOID*      pMembHandle;       /* use structure pointer as window ID */
    WMEMBTYPE_E   enMembType;        /* window member type: message input or press event */
    HI_BOOL       bIsOnFocus;        /* tag of on focus */
    HI_BOOL       bIsOnEdit;         /* tag of on edit */
    POSITIONXY_S  stMembPos;         /* position in window */
    HI_CHAR*      pcDefString;       /* input character, can only be changed in input frame */
    HI_U8         u8TotalLen;        /* total length of character input, only valid in input frame */
    HI_U8         u8SubFocusIndex;   /* current focus index (in position of input character of current focus) */
    CABLE_MOD_E   enQamNumber;
    HI_UNF_TUNER_DEV_TYPE_E enTunerDevType;
    struct hi_WINDMEMBER_S *pstNext; /* point to next member */
} WINDMEMBER_S;

typedef struct hi_WINDOW_S
{
    HI_VOID*         pWindHandle;    /* use structure pointer as window ID */
    WINDTYPE_E       enWindType;     /* window type */
    POSITIONXY_S     stWindPos;      /* window position in region */
    HI_U32           u32WindLength;  /* window length */
    HI_U32           u32WindWidth;   /* window width */
    HI_HANDLE        hMemSurface;    /* window picture surface */
    POSITIONXY_S     stFocusPos;     /* focus position of current member ( WindPos for input menber ) */
    WINDMEMBER_S*    pstMemberList;  /* member list */
} WINDOW_S;

/*---------------------------------Common Interface-------- ---------------------*/

/**
 * Create layer
 * @param: none
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_CreatLayer(HI_VOID);

/**
 * Create surface
 * @param stWindPos: window position
 * @param u32Length: window length
 * @param u32Width: window width
 * @param enWindType: window type
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_VOID* HI_DOLBY_CreateSurface(POSITIONXY_S* pstWindPos, HI_U32 u32Length, HI_U32 u32Width, WINDTYPE_E enWindType);

/**
 * Create program search window
 * @param  pWindHandle: window handle
 * @param  pstProgSearch: pointer to program search structure
 * @return: none
*/
HI_S32 HI_DOLBY_CreatPrgSearch_Wind(HI_VOID* pWindHandle,PROGSEARCH_S *pstProgSearch);

/**
 * Destory window member list
 * @param  pWindHandle: window handle
 * @return: none
*/
HI_VOID HI_DOLBY_DestroyWindMemberList(HI_VOID* pWindHandle);

/**
 * Hide program search window
 * @param pWindHandle: window handle
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_HidePrgSearch_Wind(HI_VOID* pWindHandle);

/**
 * Build and paste window
 * @param pWindHandle: window handle
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_BuildAndPasteWind(HI_VOID* pWindHandle);

/**
 * Show and refresh window
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_ShowWind(HI_VOID);

/**
 * Clean program search window
 * @param pWindHandle: window handle
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_ClearPrgSearch_Wind(HI_VOID* pWindHandle);

/**
 * Display channel bar window
 * @param pWindHandle: window handle
 * @param stChnbar: channel bar
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_DispChnBarWin(HI_VOID* pWindHandle, CHNBAR_S* pstChnbar);

/**
 * Hide channel bar
 * @param pWindHandle: window handle
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_HideChnBar(HI_VOID* pWindHandle);

/**
 * Display channel number window
 * @param pWindHandle: window handle
 * @param  s32ProgNum: program number
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 DOLBY_DispChnNumWin(HI_VOID* pWindHandle, HI_S32 s32ProgNum);

/**
 * Create pass through window
 * @param pWindHandle: window handle
 * @param  pstPassProp: pointer to pass through param
 * @return: none
*/
HI_VOID HI_DOLBY_CreatPassThrough_WIN(HI_VOID* pWindHandle, PASSTHROUGH_S* pstPassProp);

/**
 * Destroy pass through window
 * @param pWindHandle: window handle
 * @return: none
*/
HI_VOID HI_DOLBY_DestroyPassThrough_WIN(HI_VOID* pWindHandle);

/**
 * Create search failed, draw search message
 * @param pWindHandle: window handle
 * @return: none
*/
HI_VOID HI_DOLBY_DrawSearchMesg(HI_VOID* pWindHandle);


/**
 * Clean search failed, draw search message
 * @param pWindHandle: window handle
 * @return: none
*/
HI_VOID   HI_DOLBY_ClearSearchMesg(HI_VOID* pWindHandle);


#endif
