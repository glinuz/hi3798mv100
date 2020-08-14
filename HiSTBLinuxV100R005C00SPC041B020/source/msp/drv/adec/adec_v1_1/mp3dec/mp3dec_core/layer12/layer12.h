/**************************************************************************************
* layer12.h 
**************************************************************************************/

#ifndef __PVMP3_LAYER12_H__
#define __PVMP3_LAYER12_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined (MPEG_LAYER12_SUPPORT)
 #include "pvmp3_getbits.h"

int LAYER12_DecodeLayer2(tmp3Bits *bsi, int mode, int version_x, int layer_description, int bitrate_index, int fs_index,
						  int mode_ext, int 	  *outBuf[2]);
int LAYER12_DecodeLayer1(tmp3Bits *bsi, int mode, int version_x, int layer_description, int bitrate_index, int fs_index,
					      int mode_ext, int	   *outBuf[2]);


#endif

#ifdef __cplusplus
}
#endif

#endif  /* __PVMP3_LAYER12_H__ */
