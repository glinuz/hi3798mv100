/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_userproc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/01
  Description   : Support user proc function.
  History       :
  1.Date        : 2013/07/01
    Author      : l00185424
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/mman.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/version.h>
#include <asm/atomic.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
#else
#include "../../fs/proc/internal.h"
#endif
/* Unf headers */
#include "hi_module.h"
#include "hi_debug.h"
#include "hi_common.h"
#include "hi_osal.h"
/* Drv headers */
#include "drv_userproc_ioctl.h"
#include "hi_drv_userproc.h"
#include "hi_drv_proc.h"
#include "hi_kernel_adapt.h"
#include "hi_drv_mem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#if !(0 == HI_PROC_SUPPORT)

/***************************** Macro Definition ******************************/

#define UPROC_K_LOCK(sema) do {down(&sema);}while(0)
#define UPROC_K_UNLOCK(sema) do {up(&sema);} while (0)

/*************************** Structure Definition ****************************/

typedef struct tagUMP_ENTRY_S
{
    HI_U32                  entry_name_hash;
    struct rb_node       node;
    struct proc_dir_entry *parent;
    HI_DRV_USRMODEPROC_ENTRY_S stInfo;
    HI_CHAR                entry_name[MAX_PROC_NAME_LEN+1];
}UMP_ENTRY_S;

typedef struct tagUMP_DIR_S
{
    HI_U32                  dir_name_hash;
    struct rb_node        node;
    struct rb_root        entry_root;
    struct file *           pstFile;
    struct proc_dir_entry *entry;
    struct proc_dir_entry *parent;
    HI_CHAR                dir_name[MAX_PROC_NAME_LEN+12];   /* '_' 1 and pid 10 */
}UMP_DIR_S;

typedef struct tagUMP_PARAM_S
{
    struct semaphore stSem; /* Semaphore general */
	struct semaphore stSemReadWrite; /* Semaphore ReadWrite */
    struct rb_root        root;
    wait_queue_head_t wq_for_read;
    wait_queue_head_t wq_for_write;
    int busy;
    HI_DRV_USRMODEPROC_CMD_S current_cmd;
    atomic_t atmOpenCnt;
}UMP_PARAM_S;

/***************************** Global Definition *****************************/

extern struct proc_dir_entry *g_pHisi_proc;
extern struct proc_dir_entry *g_pCMPI_proc;

/***************************** Static Definition *****************************/

static UMP_PARAM_S g_stUProcParam =
{
    .root           = RB_ROOT,
    .atmOpenCnt = ATOMIC_INIT(0),
};

static UMP_DIR_S *g_pstHisiDirent = HI_NULL, *g_pstMspDirent = HI_NULL;

static HI_VOID PROC_RemoveDirForcibly(UMP_DIR_S *pstDir);
static HI_VOID PROC_RemoveEntry(UMP_DIR_S *pstDir, UMP_ENTRY_S * pstEntry);
extern unsigned int full_name_hash(const unsigned char *name, unsigned int len);

/*********************************** Code ************************************/

static UMP_DIR_S * RBTree_Find_Dirent(UMP_DIR_S * pszParent, const HI_CHAR * pszName)
{
    HI_U32 hash = full_name_hash(pszName, strlen(pszName))  & 0x7fffffffU;
    struct rb_node *node = g_stUProcParam.root.rb_node;

    while(node)
    {
        HI_S32 result;
        UMP_DIR_S* this = rb_entry(node, UMP_DIR_S, node);

        if (hash != this->dir_name_hash)
        {
            result = hash - this->dir_name_hash;
        }
        else
        {
            result = HI_OSAL_Strncmp(pszName, this->dir_name, sizeof(this->dir_name));
        }

        if( result < 0)
        {
             node = node->rb_left;
        }
        else if(result > 0)
        {
            node = node->rb_right;
        }
        else
        {
            return this;
        }
    }

    return HI_NULL;
}

static HI_S32 RBTree_Insert_Dirent(UMP_DIR_S * pszParent, UMP_DIR_S*pszDirent)
{
    struct rb_root *root = &g_stUProcParam.root;
    struct rb_node **new = &(root->rb_node), *parent = NULL;

    while(*new)
    {
        HI_S32 result;
        UMP_DIR_S *this = rb_entry(*new, UMP_DIR_S, node);
        parent = *new;

        if (pszDirent->dir_name_hash != this->dir_name_hash)
        {
            result = pszDirent->dir_name_hash - this->dir_name_hash;
        }
        else
        {
            result =  HI_OSAL_Strncmp(pszDirent->dir_name, this->dir_name, sizeof(pszDirent->dir_name));
        }

        if (result < 0)
        {
            new = &((*new)->rb_left);
        }
        else if (result > 0)
        {
            new = &((*new)->rb_right);
        }
        else
        {
            HI_ERR_UPROC("dirent(%s) has existed.", pszDirent->dir_name);
            return HI_FAILURE;
        }
    }

    rb_link_node(&pszDirent->node, parent, new);
    rb_insert_color(&pszDirent->node, root);

    return HI_SUCCESS;
}

static void RBTree_Erase_Dirent(UMP_DIR_S * pszParent, UMP_DIR_S*pszDirent)
{
    struct rb_root *root = &g_stUProcParam.root;

    rb_erase(&(pszDirent->node), root);
}

static UMP_ENTRY_S * RBTree_Find_Entry(UMP_DIR_S * pstDir, const HI_CHAR * pszName)
{
    HI_U32 hash = full_name_hash(pszName, strlen(pszName))  & 0x7fffffffU;
    struct rb_node *node = pstDir->entry_root.rb_node;

    while(node)
    {
        HI_S32 result;
        UMP_ENTRY_S* this = rb_entry(node, UMP_ENTRY_S, node);

        if (hash != this->entry_name_hash)
        {
            result = hash - this->entry_name_hash;
        }
        else
        {
            result = HI_OSAL_Strncmp(pszName, this->entry_name, sizeof(this->entry_name));
        }

        if( result < 0)
        {
             node = node->rb_left;
        }
        else if(result > 0)
        {
            node = node->rb_right;
        }
        else
        {
            return this;
        }
    }

    return HI_NULL;
}

static HI_S32 RBTree_Insert_Entry(UMP_DIR_S * pstDir, UMP_ENTRY_S*pszEntry)
{
    struct rb_root *root = &(pstDir->entry_root);
    struct rb_node **new = &(root->rb_node), *parent = NULL;

    while(*new)
    {
        HI_S32 result;
        UMP_ENTRY_S *this = rb_entry(*new, UMP_ENTRY_S, node);
        parent = *new;

        if (pszEntry->entry_name_hash != this->entry_name_hash)
        {
            result = pszEntry->entry_name_hash - this->entry_name_hash;
        }
        else
        {
            result =  HI_OSAL_Strncmp(pszEntry->entry_name, this->entry_name, sizeof(pszEntry->entry_name));
        }

        if (result < 0)
        {
            new = &((*new)->rb_left);
        }
        else if (result > 0)
        {
            new = &((*new)->rb_right);
        }
        else
        {
            HI_ERR_UPROC("entry(%s) has existed.", pszEntry->entry_name);
            return HI_FAILURE;
        }
    }

    rb_link_node(&pszEntry->node, parent, new);
    rb_insert_color(&pszEntry->node, root);

    return HI_SUCCESS;
}

static void RBTree_Erase_Entry(UMP_DIR_S * pstDir, UMP_ENTRY_S*pszEntry)
{
    struct rb_root *root = &(pstDir->entry_root);

    rb_erase(&(pszEntry->node), root);
}

static UMP_ENTRY_S * RBTree_Find_Proc_Entry(struct proc_dir_entry* pstPde)
{
    UMP_DIR_S * pstDir;
    UMP_ENTRY_S *pstEntry;

    if ( !pstPde || !strlen(pstPde->parent->name) || !strlen(pstPde->name) )
    {
        HI_ERR_UPROC("invalid proc entry.");
        goto out;
    }

    pstDir = RBTree_Find_Dirent(HI_NULL, pstPde->parent->name);
    if (!pstDir)
    {
        HI_ERR_UPROC("Can't find dirent:%p\n", pstPde->parent->name);
        goto out;
    }

    pstEntry = RBTree_Find_Entry(pstDir, pstPde->name);
    if(!pstEntry)
    {
        HI_ERR_UPROC("Can't find entry:%p\n", pstPde->name);
        goto out;
    }

    return pstEntry;

out:
    return HI_NULL;
}

static int hi_usrmodeproc_open(struct inode *inode, struct file *file)
{
    HI_INFO_UPROC("Open User Mode Proc:%s,%d\n", current->comm, current->pid);

    if (atomic_inc_return(&g_stUProcParam.atmOpenCnt) == 1)
    {
        memset(&g_stUProcParam.current_cmd, 0, sizeof(HI_DRV_USRMODEPROC_CMD_S));
        init_waitqueue_head(&g_stUProcParam.wq_for_read);
        init_waitqueue_head(&g_stUProcParam.wq_for_write);
        g_stUProcParam.busy = 0;
        HI_INIT_MUTEX(&g_stUProcParam.stSem);
        HI_INIT_MUTEX(&g_stUProcParam.stSemReadWrite);
    }

    file->private_data = &g_stUProcParam;
    return 0;
}

static int hi_usrmodeproc_close(struct inode *inode, struct file *file)
{
    struct rb_node *node;

    if (atomic_dec_return(&g_stUProcParam.atmOpenCnt) >= 0)
    {
scratch_dirent:
        for(node = rb_first(&(g_stUProcParam.root));node; node = rb_next(node))
        {
            UMP_DIR_S * dirent = rb_entry(node, UMP_DIR_S, node);

            if (file == dirent->pstFile)
            {
                PROC_RemoveDirForcibly(dirent);
                goto scratch_dirent;
            }
            else if (HI_NULL == dirent->pstFile) /* for files /proc/msp/xxx or /proc/hisi/xxx */
            {
                struct rb_node * entry_node;

scratch_entry:
                for(entry_node = rb_first(&(dirent->entry_root));entry_node;entry_node = rb_next(entry_node))
                {
                    UMP_ENTRY_S * entry = rb_entry(entry_node, UMP_ENTRY_S, node);
                    if (file == entry->stInfo.pFile)
                    {
                        PROC_RemoveEntry(dirent, entry);
                        goto scratch_entry;
                    }
                }
            }
        }
    }

    HI_INFO_UPROC("Close User Mode Proc\n" );
    return 0;
}

static int ump_seq_show(struct seq_file *m, void *unused)
{
    struct proc_dir_entry* pstPde = (struct proc_dir_entry *)(m->private);
    UMP_PARAM_S *proc = ((struct proc_dir_entry *)(m->private))->data;
    UMP_ENTRY_S *pstEntry;
    HI_S32 ret;
    DEFINE_WAIT(wait);

    UPROC_K_LOCK(g_stUProcParam.stSem);

    pstEntry = RBTree_Find_Proc_Entry(pstPde);
    if(!pstEntry)
    {
        HI_ERR_UPROC("Can't find entry:%p\n", pstPde->name);
        ret = -1;
        goto out;
    }
    else if (!pstEntry->stInfo.pfnShowFunc)
    {
        HI_ERR_UPROC("Entry don't support read.\n");
        ret = -1;
        goto out;
    }

    proc->current_cmd.pEntry = &(pstEntry->stInfo);
    HI_OSAL_Strncpy(proc->current_cmd.aszCmd, HI_UPROC_READ_CMD, sizeof(proc->current_cmd.aszCmd) - 1);

    UPROC_K_UNLOCK(g_stUProcParam.stSem);

    /* Wait write data over */
    prepare_to_wait(&proc->wq_for_read, &wait, TASK_INTERRUPTIBLE);
    schedule();
    finish_wait(&proc->wq_for_read, &wait);

    /* Find it again, pstEntry may be removed when wait event */
    pstEntry = RBTree_Find_Proc_Entry(pstPde);
    if (!pstEntry)
    {
        HI_ERR_UPROC("Can't find entry:%p\n", pstPde->name);
        ret = -1;
        return ret;
    }

    if (HI_NULL != pstEntry->stInfo.pfnShowFunc)
    {
        HI_INFO_UPROC("User Mode Proc Show entry=0x%p, proc=0x%p\n", pstPde, proc);

        UPROC_K_LOCK(g_stUProcParam.stSemReadWrite);
        if (pstEntry->stInfo.Read)
        {
            PROC_PRINT(m, "%s", (HI_CHAR*)pstEntry->stInfo.Read);

            HI_KFREE(HI_ID_PROC, pstEntry->stInfo.Read);
            pstEntry->stInfo.Read = HI_NULL;
        }
        UPROC_K_UNLOCK(g_stUProcParam.stSemReadWrite);
    }

    ret = 0;
	return ret;
 out:
    UPROC_K_UNLOCK(g_stUProcParam.stSem);

    return ret;
}

static int ump_seq_open(struct inode *inode, struct file *file)
{
    UMP_PARAM_S *proc = PDE(inode)->data;
    int res;

    HI_INFO_UPROC("ump_seq_open 0x%p,%d\n", proc, proc->busy );

    if (proc->busy)
        return -EAGAIN;

    proc->busy = 1;

    res = single_open(file, ump_seq_show, PDE(inode));

    if( res )
        proc->busy = 0;

    return res;
}

static int ump_seq_release(struct inode *inode, struct file *file)
{
    UMP_PARAM_S *proc = PDE(inode)->data;

    HI_INFO_UPROC("ump_seq_release %d\n", proc->busy );

    proc->busy = 0;
    return single_release(inode, file);
}

static HI_S32 StripString(HI_CHAR *string, HI_U32 size)
{
    HI_CHAR * p = string;
    HI_U32      index = 0;

    if (!string ||  0 ==  size)
        return HI_FAILURE;

    /* strip '\n' as string end character */
    for (; index < size; index++)
    {
        if ( '\n' == *(p + index) )
        {
            *(p + index) = '\0';
        }
    }

    if (strlen(string))
        return HI_SUCCESS;
    else
        return HI_FAILURE;
}

static ssize_t ump_seq_write(struct file *file, const char __user *buf, size_t size, loff_t *pos)
{
    struct proc_dir_entry* pstPde = PDE(file->f_path.dentry->d_inode);
    UMP_PARAM_S *proc = pstPde->data;
    UMP_ENTRY_S *pstEntry ;
    HI_S32 ret;

    DEFINE_WAIT(wait);

    UPROC_K_LOCK(g_stUProcParam.stSem);

    pstEntry = RBTree_Find_Proc_Entry(pstPde);
    if(!pstEntry ||  size > sizeof(proc->current_cmd.aszCmd))
    {
        HI_ERR_UPROC("Can't find entry:%p\n", pstPde->name);
        ret = -1;
        goto out;
    }
    else if (HI_NULL == pstEntry->stInfo.pfnCmdFunc)
    {
        HI_ERR_UPROC("Entry don't support write.\n");
        ret = -1;
        goto out;
    }

    memset(proc->current_cmd.aszCmd, 0, sizeof(proc->current_cmd.aszCmd));
    if (copy_from_user(proc->current_cmd.aszCmd, buf, size))
    {
        HI_ERR_UPROC("get cmd failed.");
        ret = -EIO;
        goto out;
    }
    proc->current_cmd.aszCmd[size > 1 ? size - 1 : 0] = '\0';

    if (HI_FAILURE == StripString(proc->current_cmd.aszCmd, size))
    {
        HI_WARN_UPROC("echo string invalid.");
        ret =  -EINVAL;
        goto out;
    }

    proc->current_cmd.pEntry = &(pstEntry->stInfo);

    UPROC_K_UNLOCK(g_stUProcParam.stSem);

    /* Wait write data over */
    prepare_to_wait(&proc->wq_for_write, &wait, TASK_INTERRUPTIBLE);
    schedule();
    finish_wait(&proc->wq_for_write, &wait);

    /* if buffer not empty , try echo to current terminal */
    pstEntry = RBTree_Find_Proc_Entry(pstPde);
    if (HI_NULL != pstEntry && pstEntry->stInfo.pfnCmdFunc)
    {
        HI_INFO_UPROC("ump_seq_write: proc=%p, entry=%p %d bytes\n", proc, pstPde, size);

        UPROC_K_LOCK(g_stUProcParam.stSemReadWrite);
        if (pstEntry->stInfo.Write)
        {
            if (strlen((HI_CHAR*)pstEntry->stInfo.Write))
            {
                HI_DRV_PROC_EchoHelper((HI_CHAR*)pstEntry->stInfo.Write);
            }

            HI_KFREE(HI_ID_PROC, pstEntry->stInfo.Write);
            pstEntry->stInfo.Write = HI_NULL;
        }
        UPROC_K_UNLOCK(g_stUProcParam.stSemReadWrite);
    }

    return size;

out:
	UPROC_K_UNLOCK(g_stUProcParam.stSem);
    return ret;
}


UMP_DIR_S * PROC_AddDir(const HI_CHAR* pszName, const HI_CHAR* pszParent, struct file *pstFile)
{
    UMP_DIR_S * pstDir;

    /* Check parameter */
    if ((HI_NULL == pszName) || (strlen(pszName) == 0) || (strlen(pszName) > MAX_PROC_NAME_LEN))
    {
        HI_ERR_UPROC("Invalid name\n");
        return HI_NULL;
    }

    /* Find directory node, if exist, return success directlly */
    pstDir = RBTree_Find_Dirent(HI_NULL, pszName);
    if (HI_NULL != pstDir)
    {
        HI_INFO_UPROC("Dir %s exist\n", pszName);
        return pstDir;
    }

    /* Alloc directory resource */
    pstDir = HI_KMALLOC(HI_ID_PROC, sizeof(UMP_DIR_S), GFP_KERNEL);
    if (HI_NULL == pstDir)
    {
        HI_ERR_UPROC("kmalloc fail\n");
        return HI_NULL;
    }

    /* Init directory parameter */
    HI_OSAL_Snprintf(pstDir->dir_name, sizeof(pstDir->dir_name), "%s", pszName);
    pstDir->dir_name_hash = full_name_hash(pszName, strlen(pszName)) & 0x7fffffffU;
    pstDir->entry_root = RB_ROOT;
    pstDir->parent = HI_NULL;
    pstDir->pstFile = pstFile;

    /* Make proc directory */
    pstDir->entry = proc_mkdir(pstDir->dir_name, g_pHisi_proc);
    if (HI_NULL == pstDir->entry)
    {
        HI_ERR_UPROC("proc_mkdir fail\n");
        goto out1;
    }

    HI_INFO_UPROC("Proc add dir %s, file=%p, entry=0x%p\n", pstDir->dir_name, pstFile, pstDir->entry);

    /* Add directory to rbtree */
    if (HI_SUCCESS != RBTree_Insert_Dirent(HI_NULL, pstDir))
    {
        HI_ERR_UPROC("Insert new dirent failed.\n");
        goto out2;
    }

    return pstDir;

 out2:
    remove_proc_entry(pstDir->dir_name, g_pHisi_proc);

 out1:
    HI_KFREE(HI_ID_PROC, pstDir);

    return HI_NULL;
}

HI_S32 PROC_RemoveDir(UMP_DIR_S * pstDir)
{
    /* Check parameter */
    if (HI_NULL == pstDir)
    {
        HI_ERR_UPROC("Invalid name\n");
        return HI_FAILURE;
    }

    /* If there are entries in this directory, remove fail */
    if (pstDir->entry_root.rb_node)
    {
        HI_ERR_UPROC("dir %s non-null\n", pstDir->dir_name);
        return HI_FAILURE;
    }

    /* Remove proc directory */
    remove_proc_entry(pstDir->dir_name, g_pHisi_proc);

    HI_INFO_UPROC("Proc remove dir %s\n", pstDir->dir_name);

    /* Remove directory from rbtree */
    RBTree_Erase_Dirent(HI_NULL, pstDir);

    /* Free directory resource */
    HI_KFREE(HI_ID_PROC, pstDir);

    return HI_SUCCESS;
}

HI_S32 PROC_RemoveDirByName(const HI_CHAR* pszName)
{
    HI_CHAR aszDir[MAX_PROC_NAME_LEN+12];
    UMP_DIR_S * pstDir = HI_NULL;

    /* Check parameter */
    if ((HI_NULL == pszName) || (strlen(pszName) == 0) ||  (strlen(pszName) > MAX_PROC_NAME_LEN))
    {
        HI_ERR_UPROC("Invalid name\n");
        return HI_FAILURE;
    }

    /* Make directory name */
    HI_OSAL_Snprintf(aszDir, sizeof(aszDir), "%s", pszName);

    /* Find directory node */
    pstDir = RBTree_Find_Dirent(HI_NULL, aszDir);
    if (HI_NULL == pstDir )
    {
        HI_ERR_UPROC("Find dir %s fail\n", aszDir);
        return HI_FAILURE;
    }

    return PROC_RemoveDir(pstDir);
}

UMP_DIR_S * PROC_AddPrivateDir(const HI_CHAR* pszName, struct proc_dir_entry *pstEntry)
{
    UMP_DIR_S * pstDir;

    /* Check parameter */
    if ((HI_NULL == pszName) || (strlen(pszName) > MAX_PROC_NAME_LEN) || (HI_NULL == pstEntry))
    {
        goto out;
    }

    /* Alloc directory resource */
    pstDir = HI_KMALLOC(HI_ID_PROC, sizeof(UMP_DIR_S), GFP_KERNEL);
    if (HI_NULL == pstDir)
    {
        HI_ERR_UPROC("kmalloc fail\n");
        goto out;
    }

    /* Init other parameter */
    HI_OSAL_Strncpy(pstDir->dir_name, pszName, sizeof(pstDir->dir_name)-1);
    pstDir->dir_name_hash = full_name_hash(pszName, strlen(pszName)) & 0x7fffffffU;
    pstDir->entry_root = RB_ROOT;
    pstDir->entry = pstEntry;
    pstDir->parent = HI_NULL;
    pstDir->pstFile = HI_NULL;

    /* Add directory to rbtree */
    if (HI_SUCCESS != RBTree_Insert_Dirent(HI_NULL, pstDir))
    {
        HI_ERR_UPROC("Insert new dirent failed.\n");
        goto out1;
    }

    return pstDir;

out1:

    HI_KFREE(HI_ID_PROC, pstDir);

out:

    return HI_NULL;
}

HI_S32 PROC_RemovePrivateDir(const HI_CHAR* pszName)
{
    UMP_DIR_S * pstDir = HI_NULL;

    /* Check parameter */
    if (HI_NULL == pszName)
    {
        return HI_FAILURE;
    }

    /* Find directory node */
    pstDir = RBTree_Find_Dirent(HI_NULL, pszName);
    if (HI_NULL == pstDir)
    {
        HI_ERR_UPROC("Find dir %s fail\n", pszName);
        return HI_FAILURE;
    }

    /* Remove directory from rbtree */
    RBTree_Erase_Dirent(HI_NULL, pstDir);

    /* Free directory resource */
    HI_KFREE(HI_ID_PROC, pstDir);

    return HI_SUCCESS;
}

static struct file_operations ump_seq_fops;

UMP_ENTRY_S* PROC_AddEntry(const HI_DRV_USRMODEPROC_ENTRY_S* pstParam, HI_BOOL bUsrMode)
{
    UMP_ENTRY_S * pstEntry = HI_NULL;
    UMP_DIR_S * pstDir = HI_NULL;
    HI_CHAR aszDir[MAX_PROC_NAME_LEN+12];
    HI_U32 u32EntryLen;

    /* Check parameter */
    if (HI_NULL == pstParam)
    {
        return HI_NULL;
    }

    u32EntryLen = strlen(pstParam->aszName);
    if ((0 == u32EntryLen) || (u32EntryLen > MAX_PROC_NAME_LEN))
    {
        HI_ERR_UPROC("Invalid name\n");
        return HI_NULL;
    }

    /* Make parent directory name */
    if (0 == strlen(pstParam->aszParent))
    {
        strlcpy(aszDir, "hisi", sizeof(aszDir) - 1);
    }
    else
    {
        HI_OSAL_Snprintf(aszDir, sizeof(aszDir), "%s", pstParam->aszParent);
    }

    /* Find directory node, if don't exist, return fail */
    pstDir = RBTree_Find_Dirent(HI_NULL, aszDir);
    if (HI_NULL == pstDir)
    {
        HI_ERR_UPROC("Dir %s don't exist\n", pstParam->aszParent);
        goto out;
    }

    /* Find entry in the directory, if exist, return success directlly */
    pstEntry = RBTree_Find_Entry(pstDir, pstParam->aszName);
    if (HI_NULL != pstEntry)
    {
        HI_INFO_UPROC("Entry %s exist\n", pstParam->aszName);
        goto out;
    }

    /* Alloc entry resource */
    pstEntry = HI_KMALLOC(HI_ID_PROC, sizeof(UMP_ENTRY_S), GFP_KERNEL);
    if (HI_NULL == pstEntry)
    {
        HI_ERR_UPROC("kmalloc fail\n");
        goto out;
    }
    memset(pstEntry, 0, sizeof(UMP_ENTRY_S));

    /* Create proc entry */
    pstEntry->stInfo.pEntry = proc_create(pstParam->aszName, 0, pstDir->entry, &ump_seq_fops);
    if (HI_NULL == pstEntry->stInfo.pEntry)
    {
        HI_FATAL_UPROC("create_proc_entry fail\n");
        goto out1;
    }

    HI_INFO_UPROC("Proc add entry %s, file=%p, entry=0x%p\n", pstParam->aszName, pstParam->pFile, pstEntry->stInfo.pEntry);

    /* Init other parameter */
    HI_OSAL_Strncpy(pstEntry->entry_name, pstParam->aszName, sizeof(pstEntry->entry_name)-1);
    pstEntry->entry_name_hash = full_name_hash(pstParam->aszName, strlen(pstParam->aszName)) & 0x7fffffffU;
    pstEntry->parent = pstDir->entry;

    pstEntry->stInfo.pFile          = pstParam->pFile;
    pstEntry->stInfo.pfnShowFunc    = pstParam->pfnShowFunc;
    pstEntry->stInfo.pfnCmdFunc     = pstParam->pfnCmdFunc;
    pstEntry->stInfo.pPrivData      = pstParam->pPrivData;
    pstEntry->stInfo.Read           = HI_NULL;
    pstEntry->stInfo.Write          = HI_NULL;

    /* Add entry to rbtree */
    if (HI_SUCCESS != RBTree_Insert_Entry(pstDir, pstEntry))
    {
        HI_ERR_UPROC("Insert new file entry failed.\n");
        goto out2;
    }

    return pstEntry;

out2:
    remove_proc_entry(pstEntry->entry_name, pstEntry->parent);
out1:
    HI_KFREE(HI_ID_PROC, pstEntry);
out:
    return HI_NULL;
}

HI_VOID PROC_RemoveEntry(UMP_DIR_S *pstDir, UMP_ENTRY_S * pstEntry)
{
    /* Check parameter */
    if (HI_NULL == pstEntry || HI_NULL == pstDir)
    {
        return;
    }

    /* Remove proc entry */
    remove_proc_entry(pstEntry->entry_name, pstEntry->parent);

    HI_INFO_UPROC("Proc remove entry %s\n", pstEntry->entry_name);

    if (HI_NULL != pstEntry->stInfo.Read)
    {
        HI_KFREE(HI_ID_PROC, pstEntry->stInfo.Read);
        pstEntry->stInfo.Read = HI_NULL;
    }

    if (HI_NULL != pstEntry->stInfo.Write)
    {
        HI_KFREE(HI_ID_PROC, pstEntry->stInfo.Write);
        pstEntry->stInfo.Write = HI_NULL;
    }

    /* Remove entry from rbtree */
    RBTree_Erase_Entry(pstDir, pstEntry);

    /* If current command belongs to this entry, clear it. */
    if (g_stUProcParam.current_cmd.pEntry == (HI_VOID*)&(pstEntry->stInfo))
    {
        g_stUProcParam.current_cmd.pEntry = HI_NULL;
        memset(&g_stUProcParam.current_cmd, 0, sizeof(g_stUProcParam.current_cmd));
    }

    /* Free resource */
    HI_KFREE(HI_ID_PROC, pstEntry);

    return;
}

HI_S32 PROC_RemoveEntryByName(const HI_CHAR* pszName, const HI_CHAR* pszParent)
{
    UMP_ENTRY_S * pstEntry = HI_NULL;
    UMP_DIR_S * pstDir = HI_NULL;
    HI_CHAR aszDir[MAX_PROC_NAME_LEN+12];

    /* Check parameter */
    if ((HI_NULL == pszName) || (strlen(pszName) > MAX_PROC_NAME_LEN))
    {
        HI_ERR_UPROC("Invalid name\n");
        return HI_FAILURE;
    }
    if ((HI_NULL == pszParent) || (strlen(pszParent) > MAX_PROC_NAME_LEN))
    {
        HI_ERR_UPROC("Invalid parent name\n");
        return HI_FAILURE;
    }

    /* Make parent directory name */
    if (0 == strlen(pszParent))
    {
        strlcpy(aszDir, "hisi", sizeof(aszDir) - 1);
    }
    else
    {
        HI_OSAL_Snprintf(aszDir, sizeof(aszDir), "%s", pszParent);
    }

    /* Find directory node, if don't exist, return fail */
    pstDir = RBTree_Find_Dirent(HI_NULL, aszDir);
    if (HI_NULL == pstDir)
    {
        HI_ERR_UPROC("Dir %s don't exist\n", pszParent);
        return HI_FAILURE;
    }

    /* Find entry in the directory, if don't exist, return fail */
    pstEntry = RBTree_Find_Entry(pstDir, pszName);
    if (HI_NULL == pstEntry)
    {
        HI_WARN_UPROC("Entry %s don't exist\n", pszName);
        return HI_FAILURE;
    }

    /* Remove entry */
    PROC_RemoveEntry(pstDir, pstEntry);

    return HI_SUCCESS;
}

HI_VOID PROC_RemoveDirForcibly(UMP_DIR_S *pstDir)
{
    struct rb_node *node;
    UMP_ENTRY_S * this;

    /* Check parameter */
    if (HI_NULL == pstDir)
    {
        return;
    }

    HI_INFO_UPROC("Proc remove dir: %s\n", pstDir->dir_name);

    while (pstDir->entry_root.rb_node)
    {
        node = rb_first(&(pstDir->entry_root));
        this = rb_entry(node, UMP_ENTRY_S, node);
        PROC_RemoveEntry(pstDir, this);
    }

    PROC_RemoveDir(pstDir);
}

static struct file_operations ump_seq_fops = {
        .open = ump_seq_open,
        .read = seq_read,
        .write   = ump_seq_write,
        .llseek = seq_lseek,
        .release = ump_seq_release,
};

long hi_usrmodeproc_ioctl(struct file *file,
                unsigned int cmd, unsigned long arg)
{
    HI_S32 ret = HI_SUCCESS;
    UMP_PARAM_S *proc = file->private_data;
    HI_DRV_USRMODEPROC_ENTRY_S ump_entry;
    HI_CHAR aszName[MAX_PROC_NAME_LEN+1];
    UMP_DIR_S * pstDir;
    UMP_ENTRY_S* pstEntry;
    HI_DRV_USRMODEPROC_CMDINFO_S *pstCmdInfo;

    switch (cmd)
    {
        case UMPIOC_ADD_ENTRY:
        {
            if (copy_from_user(&ump_entry, (void __user *)arg, sizeof(ump_entry)))
            {
                ret = HI_FAILURE;
                break;
            }
            ump_entry.aszName[sizeof(ump_entry.aszName)-1] = 0;
            ump_entry.aszParent[sizeof(ump_entry.aszParent)-1] = 0;
            ump_entry.pFile = (HI_VOID*)file;

            UPROC_K_LOCK(g_stUProcParam.stSem);
            pstEntry = PROC_AddEntry(&ump_entry, HI_TRUE);
            UPROC_K_UNLOCK(g_stUProcParam.stSem);
            if (HI_NULL == pstEntry)
            {
                ret = HI_FAILURE;
                break;
            }

            ((struct proc_dir_entry *)pstEntry->stInfo.pEntry)->data = proc;

            break;
        }

        case UMPIOC_REMOVE_ENTRY:
        {
            if (copy_from_user(&ump_entry, (void __user *)arg, sizeof(ump_entry)))
            {
                ret = HI_FAILURE;
                break;
            }
            ump_entry.aszName[sizeof(ump_entry.aszName)-1] = 0;
            ump_entry.aszParent[sizeof(ump_entry.aszParent)-1] = 0;

            UPROC_K_LOCK(g_stUProcParam.stSem);
            /* Removed by name now, can be removed by ump_entry.pEntry  */
            ret = PROC_RemoveEntryByName((HI_CHAR*)ump_entry.aszName, (HI_CHAR*)ump_entry.aszParent);
            UPROC_K_UNLOCK(g_stUProcParam.stSem);
            break;
        }

        case UMPIOC_ADD_DIR:
        {
            HI_CHAR *ptrDirName = aszName;
            if (copy_from_user(ptrDirName, (void __user *)arg, sizeof(HI_PROC_DIR_NAME_S)))
            {
                ret = HI_FAILURE;
                break;
            }
            aszName[sizeof(aszName)-1] = 0;

            UPROC_K_LOCK(g_stUProcParam.stSem);
            pstDir = PROC_AddDir(aszName, HI_NULL, file);
            if (HI_NULL == pstDir)
            {
                ret = HI_FAILURE;
            }
            UPROC_K_UNLOCK(g_stUProcParam.stSem);
            break;
        }
        case UMPIOC_REMOVE_DIR:
        {
            HI_CHAR *ptrDirName = aszName;

            if (copy_from_user(ptrDirName, (void __user *)arg, sizeof(HI_PROC_DIR_NAME_S)))
            {
                ret = HI_FAILURE;
                break;
            }
            aszName[sizeof(aszName)-1] = 0;

            UPROC_K_LOCK(g_stUProcParam.stSem);
            ret = PROC_RemoveDirByName(aszName);
            UPROC_K_UNLOCK(g_stUProcParam.stSem);
            break;
        }
        case UMPIOC_GETCMD:
        {
            pstCmdInfo = (HI_DRV_USRMODEPROC_CMDINFO_S*)arg;

            UPROC_K_LOCK(g_stUProcParam.stSem);
                /* If there is a command */
            if ((strlen(proc->current_cmd.aszCmd) > 0) &&
                /* and it must belong to a entry */
                (HI_NULL != proc->current_cmd.pEntry) &&
                /* and the entry must belong to this file(this process). */
                ((HI_VOID*)file == ((HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry)->pFile))
            {
                if (copy_to_user((void __user *)&(pstCmdInfo->stCmd),
                    &(proc->current_cmd), sizeof(HI_DRV_USRMODEPROC_CMD_S)))
                {
                    UPROC_K_UNLOCK(g_stUProcParam.stSem);
                    return -EFAULT;
                }
                if (copy_to_user((void __user *)&(pstCmdInfo->stEntry),
                    proc->current_cmd.pEntry, sizeof(HI_DRV_USRMODEPROC_ENTRY_S)))
                {
                    UPROC_K_UNLOCK(g_stUProcParam.stSem);
                    return -EFAULT;
                }

                memset(proc->current_cmd.aszCmd, 0, sizeof(proc->current_cmd.aszCmd));
            }
            UPROC_K_UNLOCK(g_stUProcParam.stSem);
            break;
        }

        case UMPIOC_WAKE_READ_TASK:
        {
            HI_PROC_SHOW_BUFFER_S       ShowBuf;
            HI_DRV_USRMODEPROC_ENTRY_S *ProcEntry = (HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry;
            if (HI_NULL == ProcEntry)
            {
                HI_ERR_UPROC("ProcEntry[0x%p] invalid!\n", ProcEntry);
                ret = HI_FAILURE;
                break;
            }

            if (0 == copy_from_user(&ShowBuf, (void __user *)arg, sizeof(ShowBuf)))
            {
                if (ShowBuf.u32Size <= HI_PROC_BUFFER_SIZE)
                {
                    UPROC_K_LOCK(g_stUProcParam.stSemReadWrite);
                    ProcEntry->Read = HI_KMALLOC(HI_ID_PROC, ShowBuf.u32Size, GFP_KERNEL);
                    if (ProcEntry->Read)
                    {
                        if (copy_from_user(ProcEntry->Read, (void __user *)ShowBuf.pu8Buf, ShowBuf.u32Size))
                        {
                            HI_KFREE(HI_ID_PROC, ProcEntry->Read);
                            ProcEntry->Read = HI_NULL;
                        }
                    }
                    UPROC_K_UNLOCK(g_stUProcParam.stSemReadWrite);
                }
            }

            wake_up_interruptible(&(proc->wq_for_read));
            break;
        }

        case UMPIOC_WAKE_WRITE_TASK:
        {
            HI_PROC_SHOW_BUFFER_S       ShowBuf;
            HI_DRV_USRMODEPROC_ENTRY_S *ProcEntry = (HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry;
            if (HI_NULL == ProcEntry)
            {
                HI_ERR_UPROC("ProcEntry[0x%p] invalid!\n", ProcEntry);
                ret = HI_FAILURE;
                break;
            }

            if (0 == copy_from_user(&ShowBuf, (void __user *)arg, sizeof(ShowBuf)))
            {
                if (ShowBuf.u32Size <= HI_PROC_BUFFER_SIZE)
                {
                    UPROC_K_LOCK(g_stUProcParam.stSemReadWrite);
                    ProcEntry->Write = HI_KMALLOC(HI_ID_PROC, ShowBuf.u32Size, GFP_KERNEL);
                    if (ProcEntry->Write)
                    {
                        if (copy_from_user(ProcEntry->Write, (void __user *)ShowBuf.pu8Buf, ShowBuf.u32Size))
                        {
                            HI_KFREE(HI_ID_PROC, ProcEntry->Write);
                            ProcEntry->Write = HI_NULL;
                        }
                    }
                    UPROC_K_UNLOCK(g_stUProcParam.stSemReadWrite);
                }
            }

            wake_up_interruptible(&(proc->wq_for_write));
            break;
        }

        default:
            ret = HI_FAILURE;
            HI_ERR_UPROC("Unknow cmd[%#x]!\n", cmd);
            break;
    }

    return ret;
}

#ifdef CONFIG_COMPAT
long hi_usrmodeproc_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    HI_S32 ret = HI_SUCCESS;
    UMP_PARAM_S *proc = file->private_data;
    HI_DRV_USRMODEPROC_ENTRY_S ump_entry = {{0}};
    HI_DRV_USRMODEPROC_Compat_ENTRY_S compat_ump_entry;
    UMP_ENTRY_S* pstEntry;
    HI_DRV_USRMODEPROC_Compat_CMDINFO_S *pstCmdInfo;

    switch (cmd)
    {
        case UMPIOC_ADD_Compat_ENTRY:
        {
            if (copy_from_user(&compat_ump_entry, (void __user *)arg, sizeof(compat_ump_entry)))
            {
                ret = HI_FAILURE;
                break;
            }
            memcpy(ump_entry.aszName, compat_ump_entry.aszName, sizeof(ump_entry.aszName));
            memcpy(ump_entry.aszParent, compat_ump_entry.aszParent, sizeof(ump_entry.aszParent));
            ump_entry.aszName[sizeof(ump_entry.aszName)-1] = 0;
            ump_entry.aszParent[sizeof(ump_entry.aszParent)-1] = 0;
            ump_entry.pfnShowFunc = (HI_PROC_SHOW_FN)compat_ptr(compat_ump_entry.pfnShowFunc);
            ump_entry.pfnCmdFunc = (HI_PROC_CMD_FN)compat_ptr(compat_ump_entry.pfnCmdFunc);
            ump_entry.pPrivData = (HI_VOID*)compat_ptr(compat_ump_entry.pPrivData);
            ump_entry.pEntry = (HI_VOID*)compat_ptr(compat_ump_entry.pEntry);
            ump_entry.pFile = (HI_VOID*)file;
            ump_entry.Read = (HI_VOID*)compat_ptr(compat_ump_entry.Read);
            ump_entry.Write = (HI_VOID*)compat_ptr(compat_ump_entry.Write);

            UPROC_K_LOCK(g_stUProcParam.stSem);
            pstEntry = PROC_AddEntry(&ump_entry, HI_TRUE);
            UPROC_K_UNLOCK(g_stUProcParam.stSem);
            if (HI_NULL == pstEntry)
            {
                ret = HI_FAILURE;
                break;
            }

            ((struct proc_dir_entry *)pstEntry->stInfo.pEntry)->data = proc;

            break;
        }

        case UMPIOC_REMOVE_Compat_ENTRY:
        {
            if (copy_from_user(&compat_ump_entry, (void __user *)arg, sizeof(compat_ump_entry)))
            {
                ret = HI_FAILURE;
                break;
            }
            memcpy(ump_entry.aszName, compat_ump_entry.aszName, sizeof(ump_entry.aszName));
            memcpy(ump_entry.aszParent, compat_ump_entry.aszParent, sizeof(ump_entry.aszParent));
            ump_entry.aszName[sizeof(ump_entry.aszName)-1] = 0;
            ump_entry.aszParent[sizeof(ump_entry.aszParent)-1] = 0;
            ump_entry.pfnShowFunc = (HI_PROC_SHOW_FN)compat_ptr(compat_ump_entry.pfnShowFunc);
            ump_entry.pfnCmdFunc = (HI_PROC_CMD_FN)compat_ptr(compat_ump_entry.pfnCmdFunc);
            ump_entry.pPrivData = (HI_VOID*)compat_ptr(compat_ump_entry.pPrivData);
            ump_entry.pEntry = (HI_VOID*)compat_ptr(compat_ump_entry.pEntry);
            ump_entry.pFile = (HI_VOID*)file;
            ump_entry.Read = (HI_VOID*)compat_ptr(compat_ump_entry.Read);
            ump_entry.Write = (HI_VOID*)compat_ptr(compat_ump_entry.Write);

            UPROC_K_LOCK(g_stUProcParam.stSem);
            /* Removed by name now, can be removed by ump_entry.pEntry  */
            ret = PROC_RemoveEntryByName((HI_CHAR*)ump_entry.aszName, (HI_CHAR*)ump_entry.aszParent);
            UPROC_K_UNLOCK(g_stUProcParam.stSem);
            break;
        }

        case UMPIOC_Compat_GETCMD:
        {
            pstCmdInfo = (HI_DRV_USRMODEPROC_Compat_CMDINFO_S*)arg;

            UPROC_K_LOCK(g_stUProcParam.stSem);
                /* If there is a command */
            if ((strlen(proc->current_cmd.aszCmd) > 0) &&
                /* and it must belong to a entry */
                (HI_NULL != proc->current_cmd.pEntry) &&
                /* and the entry must belong to this file(this process). */
                ((HI_VOID*)file == ((HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry)->pFile))
            {
                HI_DRV_USRMODEPROC_Compat_CMD_S TmpCmdInfo = {0};
                HI_DRV_USRMODEPROC_Compat_ENTRY_S TmpEntryInfo ={{0}};

                memcpy(TmpCmdInfo.aszCmd, proc->current_cmd.aszCmd, sizeof(proc->current_cmd.aszCmd));

                if (copy_to_user((void __user *)&(pstCmdInfo->stCmd),
                    &TmpCmdInfo, sizeof(HI_DRV_USRMODEPROC_Compat_CMD_S)))
                {
                    UPROC_K_UNLOCK(g_stUProcParam.stSem);
                    return -EFAULT;
                }

                TmpEntryInfo.pfnShowFunc = ptr_to_compat(((HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry)->pfnShowFunc);
                TmpEntryInfo.pfnCmdFunc = ptr_to_compat(((HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry)->pfnCmdFunc);
                TmpEntryInfo.pPrivData = ptr_to_compat(((HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry)->pPrivData);

                if (copy_to_user((void __user *)&(pstCmdInfo->stEntry),
                    &TmpEntryInfo, sizeof(HI_DRV_USRMODEPROC_Compat_ENTRY_S)))
                {
                    UPROC_K_UNLOCK(g_stUProcParam.stSem);
                    return -EFAULT;
                }

                memset(proc->current_cmd.aszCmd, 0, sizeof(proc->current_cmd.aszCmd));
            }
            UPROC_K_UNLOCK(g_stUProcParam.stSem);
            break;
        }

        case UMPIOC_Compat_WAKE_READ_TASK:
        {
            HI_PROC_Compat_SHOW_BUFFER_S       ShowBuf;
            HI_DRV_USRMODEPROC_ENTRY_S *ProcEntry = (HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry;
            if (HI_NULL == ProcEntry)
            {
                HI_ERR_UPROC("ProcEntry[0x%p] invalid!\n", ProcEntry);
                ret = HI_FAILURE;
                break;
            }

            if (0 == copy_from_user(&ShowBuf, (void __user *)arg, sizeof(ShowBuf)))
            {
                if (ShowBuf.u32Size <= HI_PROC_BUFFER_SIZE)
                {
                    UPROC_K_LOCK(g_stUProcParam.stSemReadWrite);
                    ProcEntry->Read = HI_KMALLOC(HI_ID_PROC, ShowBuf.u32Size, GFP_KERNEL);
                    if (ProcEntry->Read)
                    {
                        if (copy_from_user(ProcEntry->Read,
                            (void __user *)compat_ptr(ShowBuf.pu8Buf), ShowBuf.u32Size))
                        {
                            HI_KFREE(HI_ID_PROC, ProcEntry->Read);
                            ProcEntry->Read = HI_NULL;
                        }
                    }
                    UPROC_K_UNLOCK(g_stUProcParam.stSemReadWrite);
                }
            }

            wake_up_interruptible(&(proc->wq_for_read));
            break;
        }

        case UMPIOC_Compat_WAKE_WRITE_TASK:
        {
            HI_PROC_Compat_SHOW_BUFFER_S       ShowBuf;
            HI_DRV_USRMODEPROC_ENTRY_S *ProcEntry = (HI_DRV_USRMODEPROC_ENTRY_S*)proc->current_cmd.pEntry;
            if (HI_NULL == ProcEntry)
            {
                HI_ERR_UPROC("ProcEntry[0x%p] invalid!\n", ProcEntry);
                ret = HI_FAILURE;
                break;
            }

            if (0 == copy_from_user(&ShowBuf, (void __user *)arg, sizeof(ShowBuf)))
            {
                if (ShowBuf.u32Size <= HI_PROC_BUFFER_SIZE)
                {
                    UPROC_K_LOCK(g_stUProcParam.stSemReadWrite);
                    ProcEntry->Write = HI_KMALLOC(HI_ID_PROC, ShowBuf.u32Size, GFP_KERNEL);
                    if (ProcEntry->Write)
                    {
                        if (copy_from_user(ProcEntry->Write,
                            (void __user *)compat_ptr(ShowBuf.pu8Buf), ShowBuf.u32Size))
                        {
                            HI_KFREE(HI_ID_PROC, ProcEntry->Write);
                            ProcEntry->Write = HI_NULL;
                        }
                    }
                    UPROC_K_UNLOCK(g_stUProcParam.stSemReadWrite);
                }
            }

            wake_up_interruptible(&(proc->wq_for_write));
            break;
        }

        default:
            return hi_usrmodeproc_ioctl(file, cmd, arg);
    }

    return ret;
}
#endif

static struct file_operations hi_usrmodeproc_fops =
{
    .open    = hi_usrmodeproc_open,
    .release   = hi_usrmodeproc_close,
    .unlocked_ioctl = hi_usrmodeproc_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = hi_usrmodeproc_compat_ioctl,
#endif
};

static struct miscdevice hi_usrmodeproc_dev =
{
    MISC_DYNAMIC_MINOR,
    HI_USERPROC_DEVNAME,
    &hi_usrmodeproc_fops
};

HI_S32 USRPROC_DRV_ModInit(HI_VOID)
{
    HI_S32 ret;

    ret = misc_register(&hi_usrmodeproc_dev);
    if (ret)
    {
        HI_ERR_UPROC("%s device register failed\n", HI_USERPROC_DEVNAME);
    }

    g_pstHisiDirent = PROC_AddPrivateDir("hisi", g_pHisi_proc);
    if (!g_pstHisiDirent)
    {
        HI_ERR_UPROC("add 'hisi' directory failed.\n");
        ret = -1;
        goto out;
    }

    g_pstMspDirent = PROC_AddPrivateDir("msp", g_pCMPI_proc);
    if (!g_pstMspDirent)
    {
        HI_ERR_UPROC("add 'msp' directory failed.\n");
        ret = -1;
        goto out;
    }

out:
    return ret;
}

HI_VOID USRPROC_DRV_ModExit(HI_VOID)
{
    if (g_pstMspDirent)
    {
        PROC_RemovePrivateDir(g_pstMspDirent->dir_name);
        g_pstMspDirent = HI_NULL;
    }

    if (g_pstHisiDirent)
    {
        PROC_RemovePrivateDir(g_pstHisiDirent->dir_name);
        g_pstHisiDirent = HI_NULL;
    }

    misc_deregister(&hi_usrmodeproc_dev);
}

MODULE_DESCRIPTION("Hisilicon User Mode Proc Driver");
MODULE_LICENSE("GPL");

#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
