/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  sv_unf_osal.c
* Description:  Define interfaces of system verification abstract layer.
*
* History:
* Version      Date         Author        DefectNum    Description
* V0.01    20110428    j00174387      NULL      Create this file.
***********************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*cplusplus*/
#include "sv_unf_osal.h"


struct file *SV_klib_fopen(const char *filename, int flags, int mode)
{
        struct file *filp = filp_open(filename, flags, mode);
        return (IS_ERR(filp)) ? NULL : filp;
}

HI_VOID SV_klib_fclose(struct file *filp)
{
        if (filp)
            filp_close(filp, NULL);
}

HI_S32 SV_klib_fread(HI_S8 *buf, HI_U32 len, struct file *filp)
{
        HI_S32 readlen;
        mm_segment_t oldfs;

        if (filp == NULL)
                return -ENOENT;
        if (filp->f_op->read == NULL)
                return -ENOSYS;
        if (((filp->f_flags & O_ACCMODE) & (O_RDONLY | O_RDWR)) != 0)
                return -EACCES;
        oldfs = get_fs();
        set_fs(KERNEL_DS);
        readlen = filp->f_op->read(filp, buf, len, &filp->f_pos);
        set_fs(oldfs);

        return readlen;
}

HI_S32 SV_klib_fwrite(HI_S8 *buf, HI_S32 len, struct file *filp)
{
        HI_S32 writelen;
        mm_segment_t oldfs;

        if (filp == NULL)
                return -ENOENT;
        if (filp->f_op->write == NULL)
                return -ENOSYS;
        if (((filp->f_flags & O_ACCMODE) & (O_WRONLY | O_RDWR)) == 0)
                return -EACCES;
        oldfs = get_fs();
        set_fs(KERNEL_DS);
        writelen = filp->f_op->write(filp, buf, len, &filp->f_pos);
        set_fs(oldfs);

        return writelen;
}

HI_S32 SV_klib_seek(loff_t offset, int origin, struct file *filp)
{    int seeklen;
    mm_segment_t oldfs;

    if (filp == NULL)
        return -ENOENT;

    if (filp->f_op->llseek == NULL)
        return -ENOSYS;

    if (((filp->f_flags & O_ACCMODE) & (O_RDONLY | O_RDWR)) != 0)
        return -EACCES;

    oldfs = get_fs();
    set_fs(KERNEL_DS);
    seeklen = filp->f_op->llseek(filp, offset, origin);
    set_fs(oldfs);

    return seeklen;
}

HI_S32 SV_klib_tell(struct file *filp)
{    return filp->f_pos;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*cplusplus*/

