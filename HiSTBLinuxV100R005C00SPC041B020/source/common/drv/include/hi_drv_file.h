#ifndef __HI_DRV_FILE_H__
#define __HI_DRV_FILE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <linux/fs.h>
#include "hi_type.h"

/** @addtogroup H_FILE */
/** @{ */

struct file* HI_DRV_FILE_Open(const HI_S8* ps8FileName, HI_S32 s32Flags);
HI_VOID HI_DRV_FILE_Close(struct file * pFile);

HI_S32 HI_DRV_FILE_Read(struct file * pFile,  HI_U8* ps8Buf, HI_U32 u32Len);
HI_S32 HI_DRV_FILE_Write(struct file* pFile, HI_S8* ps8Buf, HI_U32 u32Len);

HI_S32 HI_DRV_FILE_Lseek(struct file *pFile, HI_S32 s32Offset, HI_S32 s32Whence);

HI_S32 HI_DRV_FILE_GetStorePath(HI_S8* ps8Buf, HI_U32 u32Len);


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __HI_DRV_FILE_H__ */
