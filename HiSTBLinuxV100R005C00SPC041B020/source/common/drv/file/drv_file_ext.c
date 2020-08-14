#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>

#include "hi_type.h"
#include "hi_drv_log.h"
#include "hi_drv_file.h"
#include "drv_log.h"

struct file* HI_DRV_FILE_Open(const HI_S8* ps8FileName, HI_S32 s32Flags)
{
    struct file *pFile = NULL;
	
	if (NULL == ps8FileName)
	{
		return NULL;
	}

    if (s32Flags == 0)
    {
        s32Flags = O_RDONLY;
    }
    else
    {
        s32Flags = O_WRONLY | O_CREAT | O_APPEND;
    }

    pFile = filp_open(ps8FileName, s32Flags | O_LARGEFILE, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	
    return (IS_ERR(pFile)) ? NULL : pFile;
}

HI_VOID HI_DRV_FILE_Close(struct file * pFile)
{
    if ( NULL != pFile )
    {
        filp_close(pFile, NULL);
    }
    
	return;
}

HI_S32 HI_DRV_FILE_Read(struct file * pFile,  HI_U8* ps8Buf, HI_U32 u32Len)
{
	HI_S32 s32ReadLen = 0;
    mm_segment_t stOld_FS = {0};

    if (pFile == NULL || NULL == ps8Buf)
    {
        return -ENOENT; /* No such file or directory */
    }

    if (pFile->f_op->read == NULL)
    {
        return -ENOSYS; /* Function not implemented */
    }

    if (((pFile->f_flags & O_ACCMODE) & (O_RDONLY | O_RDWR)) != 0)
    {
        return -EACCES; /* Permission denied */
    }

	/* saved the original file space */
    stOld_FS = get_fs();
	
	/* extend to the kernel data space */
    set_fs(KERNEL_DS);
	
    s32ReadLen = pFile->f_op->read(pFile, ps8Buf, u32Len, &pFile->f_pos);
    
	/* Restore the original file space */
	set_fs(stOld_FS);

    return s32ReadLen;
}
HI_S32 HI_DRV_FILE_Write(struct file* pFile, HI_S8* ps8Buf, HI_U32 u32Len)
{
    HI_S32 s32WriteLen = 0;
    mm_segment_t stOld_FS = {0};

    if (pFile == NULL || ps8Buf == NULL)
    {
        return -ENOENT; /* No such file or directory */
    }

    if (pFile->f_op->write == NULL)
    {
        return -ENOSYS; /* Function not implemented */
    }

    if (((pFile->f_flags & O_ACCMODE) & (O_WRONLY | O_RDWR)) == 0)
    {
        return -EACCES; /* Permission denied */
    }

    stOld_FS = get_fs();
    set_fs(KERNEL_DS);
	
    s32WriteLen = pFile->f_op->write(pFile, ps8Buf, u32Len, &pFile->f_pos);
	
    set_fs(stOld_FS);

    return s32WriteLen;
}

HI_S32 HI_DRV_FILE_Lseek(struct file *pFile, HI_S32 s32Offset, HI_S32 s32Whence)
{
	HI_S32 s32Ret;

	loff_t res = vfs_llseek(pFile, s32Offset, s32Whence);
	s32Ret = res;
	if (res != (loff_t)s32Ret)
		s32Ret = -EOVERFLOW;
	
	return s32Ret;
}

HI_S32 HI_DRV_FILE_GetStorePath(HI_S8* ps8Buf, HI_U32 u32Len)
{
    if (ps8Buf == NULL || u32Len == 0)
    {
        return HI_FAILURE;
    }

    memset(ps8Buf, 0, u32Len);

    return HI_DRV_LOG_GetStorePath(ps8Buf, u32Len);
}


EXPORT_SYMBOL(HI_DRV_FILE_Open);
EXPORT_SYMBOL(HI_DRV_FILE_Close);
EXPORT_SYMBOL(HI_DRV_FILE_Read);
EXPORT_SYMBOL(HI_DRV_FILE_Write);
EXPORT_SYMBOL(HI_DRV_FILE_Lseek);
EXPORT_SYMBOL(HI_DRV_FILE_GetStorePath);

