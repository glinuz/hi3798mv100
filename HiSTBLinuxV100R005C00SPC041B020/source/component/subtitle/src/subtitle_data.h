#ifndef __SUBTITLE_DATA_H__
#define __SUBTITLE_DATA_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C"{
#endif


/*
 *@brief: Initialize this module
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed.
 */
HI_S32 SUBT_DataRecv_Init(HI_VOID);
/*
 *@brief: Destroy this module
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed.
 */
 HI_S32 SUBT_DataRecv_DeInit(HI_VOID);

/*
 *@brief: Create the data receiving module.
 *
 *@param[in] u16PageID - page ID
 *@param[in] u16AncillaryID - ancillary page ID
 *@param[out] pphDataRecv - this module handle.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed.
 */
HI_S32 SUBT_DataRecv_Create(HI_U16 u16PageID, HI_U16 u16AncillaryID, HI_VOID** pphDataRecv);

/*
 *@brief:Destroy data receiving moudle.
 *
 *@param[in] hDataRecv - this module handle
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS - upon successful
 *@retval ::HI_FAILURE - failed.
 */
HI_S32 SUBT_DataRecv_Destroy(HI_VOID* hDataRecv);

/*
 *@brief:Reset data receiving moudle.
 *
 *@param[in] hDataRecv - this module handle
 *@param[in] bRecvFlag - whether recv data or not
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS - upon successful
 *@retval ::HI_FAILURE - failed to reset.
 */
HI_S32 SUBT_DataRecv_Reset(HI_VOID* hDataRecv, HI_BOOL bRecvFlag);

/*
 *@brief:updata page id and ancillary id.
 *
 *@param[in] hDataRecv - this module handle
 *@param[in] u16PageID - page id
 *@param[in] u16AncillaryID - ancillary id
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS - upon successful
 *@retval ::HI_FAILURE - failed to updata page id.
 */
HI_S32 SUBT_DataRecv_Updata(HI_VOID* hDataRecv, HI_U16 u16PageID, HI_U16 u16AncillaryID);

/*
 *@brief:bind parsing module.
 *
 *@param[in] hDataRecv - this module handle
 *@param[in] hDataParse - the handle of data parsing module
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to bind parsing module.
 */
HI_S32 SUBT_DataRecv_BindParsing(HI_VOID* hDataRecv, HI_VOID* hDataParse);

/*
 *@brief:unbind parsing module.
 *
 *@param[in] hDataRecv - this module handle
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to unbind.
 */
HI_S32 SUBT_DataRecv_UnbindParsing(HI_VOID* hDataRecv);

/*
 *@brief:redo the operation with cache data.
 *
 *@param[in] hDataRecv - this module handle
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to redo.
 */
HI_S32 SUBT_DataRecv_Redo(HI_VOID* hDataRecv);

/*
 *@brief:inject data.
 *
 *@param[in] hDataRecv - this module handle
 *@param[in] pu8Data - the pes packet data for subtitle
 *@param[in] u32DataSize - pes data len
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS - upon successful, have finished parsing pes data.
 *@retval ::HI_FAILURE - cannot finish pes data parsing.
 */
HI_S32 SUBT_DataRecv_Inject(HI_VOID* hDataRecv, HI_U8 *pu8Data, HI_U32 u32DataSize);


#ifdef __cplusplus
}
#endif

#endif