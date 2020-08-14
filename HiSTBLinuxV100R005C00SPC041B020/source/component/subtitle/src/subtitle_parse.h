#ifndef __SUBTITLE_DATA_PARSE_H__
#define __SUBTITLE_DATA_PARSE_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C"{
#endif

/* Ref[Subtitling system.pdf] section 7.2.1 Table 3, for page_state*/
typedef enum tagSUBT_PAGE_STATE_E
{
    SUBTITLE_PAGE_NORMAL_CASE       = 0,    /* page update, use previous page instance to display */
    SUBTITLE_PAGE_ACQUISITION_POINT,        /* page refresh, use next page instance to display */
    SUBTITLE_PAGE_MODE_CHANGE,              /* new page */
    SUBTITLE_RESERVED                       /* reserved for future use */
}SUBT_PAGE_STATE_E;

/* Ref[Subtitling system.pdf], section 7.2.2 table 6, for object type */
typedef enum tagSUBT_OBJ_TYPE_E
{
    SUBT_OBJ_TYPE_BITMAP = 0x00,    /* basic object, bitmap */
    SUBT_OBJ_TYPE_CHARACTER,        /* basic object, character */
    SUBT_OBJ_TYPE_STRING,           /* composite object, string of charaters */
    SUBT_OBJ_TYPE_UNKOWN            /* reserved */
}SUBT_OBJ_TYPE_E;

typedef struct tagSUBT_Display_ITEM_S
{
    HI_U16 u16XPos;
    HI_U16 u16YPos;
    HI_U16 u16Width;
    HI_U16 u16Heigth;

    HI_VOID*  pvRegionClut;
    HI_U8  u8BitDepth;
    HI_U16  u16PaletteItem;

    HI_U32 u32PTS;

    HI_U32 u32Timeout; /* in millisecond */

    HI_U32 u32RegionDataSize;
    HI_U8* pu8ShowData;
    HI_U8  u8DataType;
    HI_U8  u8FrontClr;
    HI_U8  u8BackClr;

    SUBT_PAGE_STATE_E enPageState;

    HI_U16 u16DisplayWidth;
    HI_U16 u16DisplayHeight;
}SUBT_Display_ITEM_S;

typedef HI_S32 (*SUBT_DATAPARSE_CALLBACK_FN)(HI_U32 u32UserData, HI_VOID *pstDisplayDate);


/*
 *@brief: Initialize this module
 *@param[in] None.
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS, upon successfully.
 *@retval ::HI_FAILURE, failed.
 */
HI_S32 SUBT_DataParse_Init(HI_VOID);
/*
 *@brief: Destroy this module
 *@param[in] None.
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS, upon successfully.
 *@retval ::HI_FAILURE, failed.
 */
HI_S32 SUBT_DataParse_DeInit(HI_VOID);

/*
 *@brief:create data parse module.
 *
 *@param[out] pphDataParse This module handle.
 *
 *@retval ::HI_SUCCESS, upon successfully.
 *@retval ::HI_FAILURE, failed.
 */
HI_S32 SUBT_DataParse_Create(HI_VOID** pphDataParse);

/*
 *@brief:Destroy data parse module.
 *
 *@param[in] hDataParse This module handle.
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS, upon successful
 *@retval ::HI_FAILURE, failed
 */
HI_S32 SUBT_DataParse_Destroy(HI_VOID* hDataParse);

/*
 *@brief:Reset parse module data
 *
 *@param[in] hDataParse This module handle.
 *@param[in] bParseFlag After resetting, whether parsing data immediately or not.
 *@param[out] None.
 *
 *@retval ::None.
 */
HI_S32 SUBT_DataParse_Reset(HI_VOID* hDataParse, HI_BOOL bParseFlag);

/*
 *@brief: To parse PES data packet.
 *
 *@param[in] hDataParse This module handle.
 *@param[in] pu8DataSrc The data to be resolved
 *@param[in] u32Len        The data length.
 *@param[in] u16PageID   The page id
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS, upon successful
 *@retval ::HI_FAILURE, failed
 */
HI_S32 SUBT_DataParse_ParsePESPacket(HI_VOID* hDataParse, HI_U8 *pu8DataSrc , HI_U32 u32Len, HI_U16 u16PageID, HI_U16 u16AncillaryID);

/*
 *@brief: update callback info.
 *
 *@param[in] hDataParse This module handle.
 *
 *@param[out] pstRegionItem the subtitling data.
 *
 *@retval ::HI_SUCCESS, upon successful
 *@retval ::HI_FAILURE, failed
 */
HI_S32 SUBT_DataParse_Update(HI_VOID* hDataParse, SUBT_DATAPARSE_CALLBACK_FN pfnCallback, HI_U32 u32UserData);

#ifdef __cplusplus
}
#endif

#endif