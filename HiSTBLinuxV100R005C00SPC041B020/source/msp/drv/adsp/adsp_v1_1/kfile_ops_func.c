/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpBeIL3vAuLvtpwmY1JB/WFe97hRqOpHstvurNiV
m9r5rnX+fojtV310j8+7oDBOTrwOlKPqiJy62XNh+1U0Jc6KP3213vv53E/iCZMsvUumVHSl
9fl9x1Fujrt4NIyHVRo7oo1sa+pzMs27c4mfhnmne9Jj9IUPUwqneCa8vX+DVw==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/
 #include <asm/unistd.h>

#include <linux/module.h>
//#include <linux/config.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#if 1
#include <linux/semaphore.h>
#else
#include <asm/semaphore.h>
#endif
#include <linux/wait.h>
#include <asm/uaccess.h>
#include <asm/io.h>

struct file *kfile_open(const char *filename, int flags, int mode)
{
    struct file *filp = filp_open(filename, flags, mode);
    return (IS_ERR(filp)) ? NULL : filp;
}

void kfile_close(struct file *filp)
{
    if (filp)
        filp_close(filp, NULL);
}

int kfile_read(char *buf, unsigned int len, struct file *filp)
{
    int readlen;
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

int kfile_write(char *buf, int len, struct file *filp)
{
    int writelen;
    mm_segment_t oldfs;

    if (NULL == buf)
    {
        return -ENOENT;
    }

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

int kfile_seek(loff_t offset, int origin, struct file *filp)
{
    int seeklen;
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


void dump(unsigned char data[], int len)
{
    int i;
    for(i=0; i<len; i++)
    {
    }
}














