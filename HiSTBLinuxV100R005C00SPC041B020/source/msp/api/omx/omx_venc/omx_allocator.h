#ifndef __OMX_ALLOCATOR_H__
#define	__OMX_ALLOCATOR_H__

#include "hi_drv_venc.h"
#include "OMX_Types.h"

OMX_S32 alloc_contigous_buffer(OMX_U32 buf_size, OMX_U32 align,venc_user_info *pvenc_buf);

void free_contigous_buffer(venc_user_info *puser_buf);
#endif
