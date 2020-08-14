/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_proc_ext.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/08/02
  Description   :
  History       :
  1.Date        : 2006/08/02
    Author      : c42025
    Modification: Created file

******************************************************************************/
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/ctype.h>

#include "hi_drv_module.h"
#include "hi_drv_proc.h"
#include "hi_drv_userproc.h"
#include "hi_osal.h"

#define MAX_PROC_ENTRIES 256

struct proc_dir_entry *g_pCMPI_proc = NULL;
struct proc_dir_entry *g_pHisi_proc = NULL;
static DRV_PROC_ITEM_S s_proc_items[MAX_PROC_ENTRIES];

static DEFINE_MUTEX(s_proc_mutex_lock);

static int CMPI_proc_open(struct inode *inode, struct file *file)
{
    DRV_PROC_ITEM_S *item = PDE_DATA(inode);

    if (item && item->read)
        return single_open(file, item->read, item);

    return -ENOSYS;
}

static ssize_t CMPI_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
#ifdef HI_PROC_WRITE_SUPPORT
    struct seq_file *s = file->private_data;
    DRV_PROC_ITEM_S *item = s->private;

    if (item->write)
        return item->write(file, buf, count, ppos);
#endif
    return -ENOSYS;
}

static long CMPI_ProcUnlockedIoctl(struct file * pFile, unsigned int cmd, unsigned long arg)
{
    struct seq_file *s = pFile->private_data;
    DRV_PROC_ITEM_S *item = s->private;

    if (item->ioctl)
    {
        return item->ioctl(s, cmd, arg);
    }

    return 0;
}

#ifdef CONFIG_COMPAT
static long CMPI_ProcCompatIoctl(struct file * pFile, unsigned int cmd, unsigned long arg)
{
    return -ENOSYS;
}
#endif

static struct file_operations CMPI_proc_ops __attribute__((unused))	 = {
    .owner   = THIS_MODULE,
    .open    = CMPI_proc_open,
    .read    = seq_read,
    .write   = CMPI_ProcWrite,
    .llseek  = seq_lseek,
    .release = single_release,
    .unlocked_ioctl   = CMPI_ProcUnlockedIoctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = CMPI_ProcCompatIoctl,
#endif
};

static HI_BOOL IsHex(char *ptr)
{
    int i, strlength;
    strlength = strlen(ptr);

    if(strlength<=2)
    {
        return HI_FALSE;
    }

    ptr += 2;
    for (i = 0; i < strlength -2; i++)
    {
        if((ptr[i]<'0') || ptr[i]>'f')
        {
            return HI_FALSE;
        }

        if((ptr[i]>'9') && ptr[i]<'a')
        {
            return HI_FALSE;
        }
    }
    return HI_TRUE;
}

static HI_S32 String2Digit(unsigned char  *ptr, HI_U32 *pu32Para)
{
    int i, strlength;
    HI_U32 u32Value = 0;

    strlength = strlen(ptr);
    if(strlength<=2)
    {
        return HI_FAILURE;
    }

    ptr += 2;
    for(i=0;i<strlength-2;i++)
    {
        u32Value <<= 4;
        if(ptr[i] <= '9')
            u32Value += ptr[i] - '0';
        else
            u32Value += ptr[i] - 'a' + 0x0a;
    }
    *pu32Para = u32Value;

    return HI_SUCCESS;
}

HI_S32 CMPI_PROC_PaserCommand(char *ptr, HI_U32 *pu32Para1, HI_U32 *pu32Para2)
{
    int i;
    HI_CHAR the_args[2][64];

    while(*ptr==' ' && *ptr++ != '\0');

    /* covert into lowercase string */
    for (i = 0; i < strlen(ptr); i++)
    {
        ptr[i] = tolower(ptr[i]);
    }

    for(i=strlen(ptr);i>0;i--)
    {
        if((*(ptr+i-1) < '0') || (*(ptr+i-1) > 'f'))
        {
            *(ptr+i-1) = '\0';
        }
        else if((*(ptr+i-1) > '9') && (*(ptr+i-1) < 'a'))
        {
            *(ptr+i-1) = '\0';
        }
        else
            break;
    }

    for(i=0;i<2;i++)
    {
        int j = 0;
        while(*ptr==' ' && *ptr++ != '\0');
        while((*ptr !=' ') && (*ptr!='\0'))
        {
            the_args[i][j++] = *ptr++;
        }
        the_args[i][j] = '\0';
        if('\0' == *ptr)
        {
            i++;
            break;
        }
        the_args[i][j] = '\0';
    }

    if(('0' != the_args[0][0]) ||('x' != the_args[0][1]) ||
        ('0' != the_args[1][0]) ||('x' != the_args[1][1]))
    {
        return -1;
    }
    if((!IsHex(the_args[0])) || (!IsHex(the_args[1])))
    {
        return -1;
    }

    if(HI_SUCCESS != String2Digit(the_args[0], pu32Para1))
    {
        return -1;
    }

    if(HI_SUCCESS != String2Digit(the_args[1], pu32Para2))
    {
        return -1;
    }

    return i;
}

static HI_VOID WriteProcShowHelp(HI_VOID)
{
    HI_DRV_PROC_EchoHelper("\nPLS type \"echo 0xxxxxxxxx 0xxxxxxxxx > /proc/msp/demux\"\n");
    HI_DRV_PROC_EchoHelper("E.g.: \"echo 0x40002 0x4010 > /proc/msp/demux\"\n");
}

ssize_t HI_DRV_PROC_ModuleWrite(struct file * file,
    const char __user *buf, size_t count, loff_t *ppos, PROC_CTRL fun_ctl)
{
    char szBuffer[64];
    char *ptrBuf = szBuffer;
    HI_U32 para1, para2;

    if(count >= sizeof(szBuffer))
    {
        WriteProcShowHelp();
        goto out;
    }

    memset(szBuffer, 0, sizeof(szBuffer));
    if(copy_from_user(ptrBuf, buf, count))
        return -EFAULT;

    szBuffer[count > 1 ? count -1 : 0] = '\0' ;

    //HI_PRINT("proc command:%s\n", szBuffer);
    if (CMPI_PROC_PaserCommand(szBuffer, &para1, &para2) > 0)
    {
        //HI_PRINT("value:0x%x, 0x%x\n", para1,  para2);
        fun_ctl(para1, para2);
    }
    else
    {
        WriteProcShowHelp();
    }

out:
    *ppos = count;
    return count;
}


static DRV_PROC_ITEM_S *proc_addModule(char *entry_name, DRV_PROC_EX_S* pFnOp, void * data)
{
#if !(0 == HI_PROC_SUPPORT)
    struct proc_dir_entry *entry;
    int i;

    if ((HI_NULL == entry_name) || (strlen(entry_name) > MAX_ENTRY_NAME_LEN))
    {
        return NULL;
    }

    mutex_lock(&s_proc_mutex_lock);
    for (i = 0; i < MAX_PROC_ENTRIES; i++)
        if (! s_proc_items[i].entry)
            break;

    if (MAX_PROC_ENTRIES == i)
    {
        mutex_unlock(&s_proc_mutex_lock);
        HI_PRINT("ERROR: add proc entry %s over LIMIT:%#x\n",  entry_name, MAX_PROC_ENTRIES);
        return NULL;
    }
    HI_OSAL_Strncpy(s_proc_items[i].entry_name, entry_name, sizeof(s_proc_items[i].entry_name) - 1);

    if (pFnOp != NULL)
    {
        s_proc_items[i].read = pFnOp->fnRead;
        s_proc_items[i].write = pFnOp->fnWrite;
        s_proc_items[i].ioctl = pFnOp->fnIoctl;
    }
    else
    {
        s_proc_items[i].read = NULL;
        s_proc_items[i].write = NULL;
        s_proc_items[i].ioctl = NULL;
    }

    s_proc_items[i].data = data;

    entry = proc_create_data(entry_name, 0, g_pCMPI_proc, &CMPI_proc_ops, &s_proc_items[i]);
    if (!entry)
    {
        mutex_unlock(&s_proc_mutex_lock);
        return NULL;
    }

    s_proc_items[i].entry = entry;

    mutex_unlock(&s_proc_mutex_lock);

    //HI_PRINT("add: i:%#x, entry_name:%s\n", i, s_proc_items[i].entry_name);

    return &s_proc_items[i];
#else
    return &s_proc_items[0];
#endif
}

static HI_VOID proc_removeModule(char *entry_name)
{
#if !(0 == HI_PROC_SUPPORT)
    int i;

    mutex_lock(&s_proc_mutex_lock);
    for (i = 0; i < MAX_PROC_ENTRIES; i++)
    {
        if (! HI_OSAL_Strncmp(s_proc_items[i].entry_name, entry_name, sizeof(s_proc_items[i].entry_name)))
            break;
    }

    if (MAX_PROC_ENTRIES == i)
    {
        mutex_unlock(&s_proc_mutex_lock);
        HI_PRINT("Not find the entry:%s\n", entry_name);
        return ;
    }
    remove_proc_entry(s_proc_items[i].entry_name, g_pCMPI_proc);
    //HI_PRINT("remove: i:%#x, entry_name:%s, %s\n", i, entry_name, s_proc_items[i].entry_name);
    s_proc_items[i].entry = NULL;

    mutex_unlock(&s_proc_mutex_lock);

#endif
}

static DRV_PROC_INTFPARAM  procparam = {
    .addModulefun = proc_addModule,
    .rmvModulefun = proc_removeModule,
};

HI_S32 HI_DRV_PROC_Init(HI_VOID)
{
    HI_S32 ret;

    ret = HI_DRV_MODULE_Register(HI_ID_PROC, "HI_PROC", HI_NULL);
    if (HI_SUCCESS != ret)
    {
        return -1;
    }

    memset(s_proc_items, 0x00, sizeof(s_proc_items));
    HI_DRV_PROC_RegisterParam(&procparam);

#if !(0 == HI_PROC_SUPPORT)
    g_pHisi_proc = proc_mkdir("hisi", NULL);
    g_pCMPI_proc = proc_mkdir("msp", g_pHisi_proc);

    proc_mkdir("graphics", g_pHisi_proc);
    proc_symlink("msp", NULL, "hisi/msp");
    proc_symlink("graphics", NULL, "hisi/graphics");
#endif
    return HI_SUCCESS;
}

HI_VOID HI_DRV_PROC_Exit(HI_VOID)
{
    HI_DRV_PROC_UnRegisterParam();

#if !(0 == HI_PROC_SUPPORT)
    remove_proc_entry("msp", NULL);
    remove_proc_entry("graphics", NULL);
    remove_proc_entry("msp", g_pHisi_proc);
    remove_proc_entry("graphics", g_pHisi_proc);
    remove_proc_entry("hisi", NULL);
#endif

    HI_DRV_MODULE_UnRegister(HI_ID_PROC);

    return;
}

EXPORT_SYMBOL(HI_DRV_PROC_ModuleWrite);

