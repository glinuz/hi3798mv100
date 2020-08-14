/*
   (c) Copyright 2002-2011  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2002-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version
   2 of the License, or (at your option) any later version.
*/

//#define FUSION_ENABLE_DEBUG

#include <linux/version.h>
#include <linux/module.h>
#ifdef HAVE_LINUX_CONFIG_H
#include <linux/config.h>
#endif
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18)
#include <linux/devfs_fs_kernel.h>
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 19)
#include <linux/page-flags.h>
#include <linux/mm.h>
#endif
#include <asm/pgtable.h>
#include <linux/mm.h>

#include <linux/vmalloc.h>
#include <linux/mman.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2)
#include <linux/device.h>
#endif

#include <linux/fusion.h>

#include "call.h"
#include "fusiondev.h"
#include "fusionee.h"
#include "property.h"
#include "reactor.h"
#include "ref.h"
#include "skirmish.h"
#include "shmpool.h"

#ifndef FUSION_MAJOR
#define FUSION_MAJOR 250
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Denis Oliver Kropp <dok@directfb.org>");

unsigned long fusion_shm_base = FUSION_SHM_BASE_DEFAULT;
unsigned long fusion_shm_size = FUSION_SHM_SIZE_DEFAULT;

module_param( fusion_shm_base, ulong, 0 );
MODULE_PARM_DESC( fusion_shm_base, "Shared memory address space base" );

module_param( fusion_shm_size, ulong, 0 );
MODULE_PARM_DESC( fusion_shm_size, "Shared memory address space size" );



struct proc_dir_entry *proc_fusion_dir;

static int fusion_major = FUSION_MAJOR;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
static devfs_handle_t devfs_handles[NUM_MINORS];
static inline unsigned iminor(struct inode *inode)
{
     return MINOR(inode->i_rdev);
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13)
static struct class *fusion_class;
#else
static struct class_simple *fusion_class;
#endif
#endif



#define unlocked_copy_from_user(a,b,c)  \
({                                      \
     int ret;                           \
                                        \
     /*fusion_core_unlock( fusion_core );*/ \
                                        \
     ret = copy_from_user( a, b, c );   \
                                        \
     /*fusion_core_lock( fusion_core );*/   \
                                        \
     ret;                               \
})


#define unlocked_copy_to_user(a,b,c)    \
({                                      \
     int ret;                           \
                                        \
     /*fusion_core_unlock( fusion_core );*/ \
                                        \
     ret = copy_to_user( a, b, c );     \
                                        \
     /*fusion_core_lock( fusion_core );*/   \
                                        \
     ret;                               \
})


static FusionShared *shared;

FusionCore            *fusion_core;
struct proc_dir_entry *fusion_proc_dir[NUM_MINORS]   = { 0 };
unsigned int           fusion_local_refs[NUM_MINORS] = { 0 };

/******************************************************************************/

static int
fusiondev_stat_proc_show(struct seq_file *m, void *v)
{
     FusionDev *dev = m->private;

     fusion_core_lock( fusion_core );

     if (!dev->shutdown) {
          if ((dev->api.major != 0) || (dev->api.minor != 0))
               seq_printf(m, "Fusion API:%d.%d\n", dev->api.major, dev->api.minor);
     
          seq_printf(m,
                              "lease/purchase   cede      attach     detach   dispatch      "
                              "ref up   ref down  prevail/swoop dismiss\n");

          seq_printf(m,
                                   "%10d %10d  %10d %10d %10d  %10d %10d  %10d %10d\n",
                                   dev->stat.property_lease_purchase,
                                   dev->stat.property_cede,
                                   dev->stat.reactor_attach,
                                   dev->stat.reactor_detach,
                                   dev->stat.reactor_dispatch,
                                   dev->stat.ref_up,
                                   dev->stat.ref_down,
                                   dev->stat.skirmish_prevail_swoop,
                                   dev->stat.skirmish_dismiss);
     }

     fusion_core_unlock( fusion_core );

     return 0;
}

static int fusiondev_stat_proc_open(struct inode *inode, struct file *file)
{
     return single_open(file, fusiondev_stat_proc_show, PDE_DATA(inode));
}
 
static const struct file_operations fusiondev_stat_proc_fops = {
     .open    = fusiondev_stat_proc_open,
     .read    = seq_read,
     .llseek  = seq_lseek,
     .release = seq_release,
};


/******************************************************************************/

static int fusiondev_init(FusionDev * dev)
{
     int ret;

     if (!dev->refs) {
          dev->shared = shared;

          fusion_core_wq_init( fusion_core, &dev->enter_wait);

          dev->shm_base = fusion_shm_base
#if FUSION_SHM_PER_WORLD_SPACE
                          + dev->index * fusion_shm_size
#endif
                          ;
     }

     ret = fusionee_init(dev);
     if (ret)
          goto error_fusionee;

     ret = fusion_ref_init(dev);
     if (ret)
          goto error_ref;

     ret = fusion_skirmish_init(dev);
     if (ret)
          goto error_skirmish;

     ret = fusion_property_init(dev);
     if (ret)
          goto error_property;

     ret = fusion_reactor_init(dev);
     if (ret)
          goto error_reactor;

     ret = fusion_shmpool_init(dev);
     if (ret)
          goto error_shmpool;

     ret = fusion_call_init(dev);
     if (ret)
          goto error_call;

     proc_create_data("stat", 0, fusion_proc_dir[dev->index],
                       &fusiondev_stat_proc_fops, dev);

     return 0;

error_call:
     fusion_shmpool_deinit(dev);

error_shmpool:
     fusion_reactor_deinit(dev);

error_reactor:
     fusion_property_deinit(dev);

error_property:
     fusion_skirmish_deinit(dev);

error_skirmish:
     fusion_ref_deinit(dev);

error_ref:
     fusionee_deinit(dev);

error_fusionee:
     return ret;
}

static void fusiondev_deinit(FusionDev * dev)
{
     fusion_core_unlock( fusion_core );

     remove_proc_entry("stat", fusion_proc_dir[dev->index]);

     fusion_core_lock( fusion_core );

     fusion_call_deinit(dev);
     fusion_shmpool_deinit(dev);
     fusion_reactor_deinit(dev);
     fusion_property_deinit(dev);
     fusion_skirmish_deinit(dev);
     fusion_ref_deinit(dev);
     fusionee_deinit(dev);

     if (!dev->refs && dev->shared_area) {
#ifdef FUSION_CORE_SHMPOOLS
          fusion_core_free( fusion_core, dev->shared_area );
          ClearPageReserved(virt_to_page(dev->shared_area));
#else
          free_page(dev->shared_area);
#endif
     }
}

/******************************************************************************/

static int fusion_open(struct inode *inode, struct file *file)
{
     int ret;
     Fusionee *fusionee;
     int minor = iminor(inode);
     FusionDev *dev = &shared->devs[minor];
     char buf[4];

     FUSION_DEBUG("fusion_open( file %p, f_count %ld ) <- minor %d\n", file, atomic_long_read(&file->f_count), minor);

     snprintf(buf, 4, "%d", minor);

     fusion_core_lock( fusion_core );

     FUSION_DEBUG("  -> refs: %d\n", dev->refs);

     if (!dev->refs) {
          memset( dev, 0, sizeof(FusionDev) );

          dev->index = minor;
     }
     else {
          if (file->f_flags & O_EXCL) {
               if (dev->fusionee.last_id) {
                    fusion_core_unlock( fusion_core );
                    return -EBUSY;
               }
          }
     }

     if (!fusion_local_refs[dev->index]) {

          fusion_proc_dir[minor] = proc_mkdir(buf, proc_fusion_dir);

          ret = fusiondev_init( dev );
          if (ret) {
               remove_proc_entry(buf, proc_fusion_dir);
               fusion_core_unlock( fusion_core );
               return ret;
          }
     }

     ret = fusionee_new(dev, !!(file->f_flags & O_APPEND), &fusionee);
     if (ret) {
          if (!fusion_local_refs[dev->index]) {
               fusiondev_deinit( dev );

               fusion_core_unlock( fusion_core );
               remove_proc_entry( buf, proc_fusion_dir );
          }
          else
               fusion_core_unlock( fusion_core );

          return ret;
     }

     fusion_local_refs[dev->index]++;

     dev->refs++;

     fusion_core_unlock( fusion_core );


     file->private_data = fusionee;

     return 0;
}

static int fusion_release(struct inode *inode, struct file *file)
{
     Fusionee  *fusionee = file->private_data;
     FusionDev *dev      = fusionee->fusion_dev;

     int minor = iminor(inode);
     char buf[4];

     FUSION_DEBUG("fusion_release( %p, %ld )\n", file, atomic_long_read(&file->f_count));

     snprintf(buf, 4, "%d", minor);

     fusion_core_lock( fusion_core );

     fusionee_destroy( dev, fusionee );

     dev->refs--;

     fusion_local_refs[dev->index]--;

     if (!fusion_local_refs[dev->index]) {
          dev->shutdown = 1;

          fusiondev_deinit( dev );

          fusion_core_unlock( fusion_core );

          remove_proc_entry( buf, proc_fusion_dir );

          fusion_core_lock( fusion_core );

          dev->shutdown = 0;
     }

     fusion_core_unlock( fusion_core );

     return 0;
}

static int
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18)
fusion_flush(struct file *file, fl_owner_t id)
#else
fusion_flush(struct file *file)
#endif
{
     Fusionee  *fusionee = file->private_data;
     FusionDev *dev      = fusionee->fusion_dev;

     (void)fusionee;

     FUSION_DEBUG("fusion_flush( %p, %ld, 0x%08lx %d )\n", file,
                  atomic_long_read(&file->f_count), fusionee_id(fusionee), fusion_core_pid( fusion_core ));

     if (current->flags & PF_EXITING) {
          fusion_core_lock( fusion_core );

          fusion_skirmish_dismiss_all_from_pid(dev, fusion_core_pid( fusion_core ));

          fusion_core_unlock( fusion_core );
     }

     return 0;
}

static ssize_t
fusion_read(struct file *file, char *buf, size_t count, loff_t * ppos)
{
     int ret;
     Fusionee  *fusionee = file->private_data;
     FusionDev *dev      = fusionee->fusion_dev;

     FUSION_DEBUG("fusion_read( %p, %ld, %zu )\n", file, atomic_long_read(&file->f_count),
                  count);

     fusion_core_lock( fusion_core );

     ret = fusionee_get_messages(dev, fusionee, buf, count, !(file->f_flags & O_NONBLOCK));

     fusion_core_unlock( fusion_core );

     return ret;
}

/*
static unsigned int fusion_poll(struct file *file, poll_table * wait)
{
     int ret;
     Fusionee  *fusionee = file->private_data;
     FusionDev *dev      = fusionee->fusion_dev;

     FUSION_DEBUG("fusion_poll( %p, %ld )\n", file, atomic_long_read(&file->f_count));

     fusion_core_lock( fusion_core );

     ret = fusionee_poll(dev, fusionee, file, wait);

     fusion_core_unlock( fusion_core );

     return ret;
}
*/

static int
lounge_ioctl(FusionDev * dev, Fusionee * fusionee,
             unsigned int cmd, unsigned long arg)
{
     int ret;
     FusionEnter enter;
     FusionKill kill;
     FusionEntryInfo info;
     FusionFork fork = { 0};
     FusionEntryPermissions permissions;

     switch (_IOC_NR(cmd)) {
          case _IOC_NR(FUSION_ENTER):
               if (unlocked_copy_from_user(&enter, (FusionEnter *) arg, sizeof(enter)))
                    return -EFAULT;

               ret = fusionee_enter(dev, &enter, fusionee);
               if (ret)
                    return ret;

               if (unlocked_copy_to_user((FusionEnter *) arg, &enter, sizeof(enter)))
                    return -EFAULT;

               return 0;

          case _IOC_NR(FUSION_UNBLOCK):
               if (fusionee_id(fusionee) != FUSION_ID_MASTER)
                    return -EPERM;

               dev->enter_ok = 1;

               fusion_core_wq_wake( fusion_core, &dev->enter_wait);

               return 0;

          case _IOC_NR(FUSION_KILL):
               if (fusionee_id(fusionee) != FUSION_ID_MASTER)
                    return -EPERM;

               if (unlocked_copy_from_user(&kill, (FusionKill *) arg, sizeof(kill)))
                    return -EFAULT;

               return fusionee_kill(dev, fusionee,
                                    kill.fusion_id, kill.signal,
                                    kill.timeout_ms);

          case _IOC_NR(FUSION_SYNC):
               return fusionee_sync( dev, fusionee );

          case _IOC_NR(FUSION_ENTRY_SET_INFO):
               if (unlocked_copy_from_user
                   (&info, (FusionEntryInfo *) arg, sizeof(info)))
                    return -EFAULT;

               switch (info.type) {
                    case FT_CALL:
                         return fusion_entry_set_info(&dev->call, &info);

                    case FT_SKIRMISH:
                         return fusion_entry_set_info(&dev->skirmish, &info);

                    case FT_PROPERTY:
                         return fusion_entry_set_info(&dev->properties, &info);

                    case FT_REACTOR:
                         return fusion_entry_set_info(&dev->reactor, &info);

                    case FT_REF:
                         return fusion_entry_set_info(&dev->ref, &info);

                    case FT_SHMPOOL:
                         return fusion_entry_set_info(&dev->shmpool, &info);

                    default:
                         return -ENOSYS;
               }

          case _IOC_NR(FUSION_ENTRY_GET_INFO):
               if (unlocked_copy_from_user
                   (&info, (FusionEntryInfo *) arg, sizeof(info)))
                    return -EFAULT;

               switch (info.type) {
                    case FT_CALL:
                         ret = fusion_entry_get_info(&dev->call, &info);
                         break;

                    case FT_SKIRMISH:
                         ret = fusion_entry_get_info(&dev->skirmish, &info);
                         break;

                    case FT_PROPERTY:
                         ret = fusion_entry_get_info(&dev->properties, &info);
                         break;

                    case FT_REACTOR:
                         ret = fusion_entry_get_info(&dev->reactor, &info);
                         break;

                    case FT_REF:
                         ret = fusion_entry_get_info(&dev->ref, &info);
                         break;

                    case FT_SHMPOOL:
                         ret = fusion_entry_get_info(&dev->shmpool, &info);
                         break;

                    default:
                         return -ENOSYS;
               }

               if (ret)
                    return ret;

               if (unlocked_copy_to_user((FusionEntryInfo *) arg, &info, sizeof(info)))
                    return -EFAULT;

               return 0;

          case _IOC_NR(FUSION_FORK):
               if (unlocked_copy_from_user(&fork, (FusionFork *) arg, sizeof(fork)))
                    return -EFAULT;

               ret = fusionee_fork(dev, &fork, fusionee);
               if (ret)
                    return ret;

               if (unlocked_copy_to_user((FusionFork *) arg, &fork, sizeof(fork)))
                    return -EFAULT;

               return 0;

          case _IOC_NR(FUSION_ENTRY_ADD_PERMISSIONS):
               if (unlocked_copy_from_user(&permissions, (FusionEntryPermissions *) arg, sizeof(permissions)))
                    return -EFAULT;

               switch (permissions.type) {
                    case FT_CALL:
                         return fusion_entry_add_permissions(&dev->call, &permissions, fusionee);

                    case FT_REF:
                         return fusion_entry_add_permissions(&dev->ref, &permissions, fusionee);

                    case FT_SKIRMISH:
                         return fusion_entry_add_permissions(&dev->skirmish, &permissions, fusionee);

                    case FT_PROPERTY:
                         return fusion_entry_add_permissions(&dev->properties, &permissions, fusionee);

                    case FT_REACTOR:
                         return fusion_entry_add_permissions(&dev->reactor, &permissions, fusionee);

                    case FT_SHMPOOL:
                         return fusion_entry_add_permissions(&dev->shmpool, &permissions, fusionee);

                    default:
                         return -ENOSYS;
               }

          case _IOC_NR(FUSION_SHM_GET_BASE):
               if (put_user(dev->shm_base, (unsigned long *)arg))
                    return -EFAULT;

               return 0;

          case _IOC_NR(FUSION_GET_FUSIONEE_INFO): {
               FusionGetFusioneeInfo *get_fusionee_info;

               get_fusionee_info = kmalloc( sizeof(FusionGetFusioneeInfo), GFP_KERNEL );
               if (!get_fusionee_info)
                    return -ENOMEM;

               if (unlocked_copy_from_user(get_fusionee_info, (FusionGetFusioneeInfo *) arg, sizeof(FusionGetFusioneeInfo))) {
                    kfree( get_fusionee_info );
                    return -EFAULT;
               }

               ret = fusionee_get_info(dev, get_fusionee_info);
               if (ret) {
                    kfree( get_fusionee_info );
                    return ret;
               }

               if (unlocked_copy_to_user((FusionGetFusioneeInfo *) arg, get_fusionee_info, sizeof(FusionGetFusioneeInfo))) {
                    kfree( get_fusionee_info );
                    return -EFAULT;
               }

               kfree( get_fusionee_info );

               return 0;
          }
     }

     return -ENOSYS;
}

static int
messaging_ioctl(FusionDev * dev, Fusionee * fusionee,
                unsigned int cmd, unsigned long arg)
{
     FusionSendMessage send;

     switch (_IOC_NR(cmd)) {
          case _IOC_NR(FUSION_SEND_MESSAGE):
               if (unlocked_copy_from_user
                   (&send, (FusionSendMessage *) arg, sizeof(send)))
                    return -EFAULT;

               if (send.msg_size <= 0)
                    return -EINVAL;

               /* message data > 64k should be stored in shared memory */
               if (send.msg_size > 0x10000)
                    return -EMSGSIZE;

               return fusionee_send_message(dev, fusionee, send.fusion_id,
                                            FMT_SEND, send.msg_id,
                                            send.msg_channel, send.msg_size,
                                            send.msg_data, FMC_NONE, NULL, 0, NULL, 0);
     }

     return -ENOSYS;
}

static int
call_ioctl(FusionDev * dev, Fusionee * fusionee,
           unsigned int cmd, unsigned long arg)
{
     int                 id;
     int                 ret;
     FusionCallNew       call;
     FusionCallExecute   execute;
     FusionCallExecute2  execute2;
     FusionCallExecute3  execute3;
     FusionCallExecute3 *execute3_bin;
     FusionCallReturn    call_ret;
     FusionCallReturn3   call_ret3;
     FusionCallGetOwner  get_owner;
     FusionCallSetQuota  set_quota;
     FusionID            fusion_id = fusionee_id(fusionee);

     switch (_IOC_NR(cmd)) {
          case _IOC_NR(FUSION_CALL_NEW):
               if (unlocked_copy_from_user(&call, (FusionCallNew *) arg, sizeof(call)))
                    return -EFAULT;

               ret = fusion_call_new(dev, fusionee, &call);
               if (ret)
                    return ret;

               if (put_user(call.call_id, (int *)arg)) {
                    fusion_call_destroy(dev, fusionee, call.call_id);
                    return -EFAULT;
               }
               return 0;

          case _IOC_NR(FUSION_CALL_EXECUTE):
               if (unlocked_copy_from_user
                   (&execute, (FusionCallExecute *) arg, sizeof(execute)))
                    return -EFAULT;

               if (dev->api.major < 9)
                    swap(execute.call_id, execute.ret_val);

               ret = fusion_call_execute(dev, fusionee, &execute);
               if (ret)
                    return ret;

               if (dev->api.major < 9)
                    swap(execute.call_id, execute.ret_val);

               if (unlocked_copy_to_user((FusionCallExecute *) arg, &execute, sizeof(execute)))
                    return -EFAULT;
               return 0;

          case _IOC_NR(FUSION_CALL_RETURN):
               if (unlocked_copy_from_user
                   (&call_ret, (FusionCallReturn *) arg, sizeof(call_ret)))
                    return -EFAULT;

               return fusion_call_return(dev, fusion_id, &call_ret);

          case _IOC_NR(FUSION_CALL_DESTROY):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_call_destroy(dev, fusionee, id);

          case _IOC_NR(FUSION_CALL_EXECUTE2):
               if (unlocked_copy_from_user
                   (&execute2, (FusionCallExecute2 *) arg, sizeof(execute2)))
                    return -EFAULT;

               if (dev->api.major < 9)
                    swap(execute.call_id, execute.ret_val);

               ret = fusion_call_execute2(dev, fusionee, &execute2);
               if (ret)
                    return ret;

               if (dev->api.major < 9)
                    swap(execute.call_id, execute.ret_val);

               if (unlocked_copy_to_user((FusionCallExecute2 *) arg, &execute2, sizeof(execute2)))
                    return -EFAULT;
               return 0;

          case _IOC_NR(FUSION_CALL_EXECUTE3):
               execute3_bin = (FusionCallExecute3 *) arg;

               while (1) {
                    if (unlocked_copy_from_user(&execute3, execute3_bin, sizeof(execute3)))
                         return -EFAULT;

                    if (!(execute3.flags & FCEF_DONE)) {
                         if (execute3.flags & FCEF_ERROR) {
                            #if 0
                              printk( KERN_ERR "fusion: FUSION_CALL_EXECUTE3 with errorneous call (failed on previous ioctl call), "
                                               "call id %d, flags 0x%08x, arg %d, length %u, serial %u,  %d\n",
                                      execute3.call_id, execute3.flags, execute3.call_arg, execute3.length, execute3.ret_length,
                                      (execute3_bin - (FusionCallExecute3 *) arg) );
                             #endif
                              return -EIO;
                         }

                         ret = fusion_call_execute3(dev, fusionee, &execute3);
                         if (ret) {
                              if (ret != -EINTR)
                                   execute3.flags |= FCEF_ERROR;

                              // TODO: OPTIMIZE: copy whole array at once outside of while loop
                              if (unlocked_copy_to_user(execute3_bin, &execute3, sizeof(execute3)))
                                   return -EFAULT;

                              return ret;
                         }

                         execute3.flags |= FCEF_DONE;

                         // TODO: OPTIMIZE: copy whole array at once outside of while loop
                         if (unlocked_copy_to_user(execute3_bin, &execute3, sizeof(execute3)))
                              return -EFAULT;
                    }

                    if (!(execute3_bin->flags & FCEF_FOLLOW))
                         break;

                    execute3_bin++;
               }
               return 0;

          case _IOC_NR(FUSION_CALL_RETURN3):
               if (unlocked_copy_from_user
                   (&call_ret3, (FusionCallReturn3 *) arg, sizeof(call_ret3)))
                    return -EFAULT;

               return fusion_call_return3(dev, fusion_id, &call_ret3);

          case _IOC_NR(FUSION_CALL_GET_OWNER):
               if (unlocked_copy_from_user
                   (&get_owner, (FusionCallGetOwner *) arg, sizeof(get_owner)))
                    return -EFAULT;

               ret = fusion_call_get_owner(dev, get_owner.call_id, &get_owner.fusion_id);
               if (ret)
                    return ret;

               if (unlocked_copy_to_user
                   ((FusionCallGetOwner *) arg, &get_owner, sizeof(get_owner)))
                    return -EFAULT;
               return 0;

          case _IOC_NR(FUSION_CALL_SET_QUOTA):
               if (unlocked_copy_from_user
                   (&set_quota, (FusionCallSetQuota *) arg, sizeof(set_quota)))
                    return -EFAULT;

               ret = fusion_call_set_quota(dev, &set_quota);
               if (ret)
                    return ret;
               return 0;
     }

     return -ENOSYS;
}

static int
ref_ioctl(FusionDev * dev, Fusionee * fusionee,
          unsigned int cmd, unsigned long arg)
{
     int              id;
     int              ret;
     int              refs;
     FusionRefWatch   watch;
     FusionRefInherit inherit;
     FusionRefThrow   throw_;
     FusionID         fusion_id = fusionee_id(fusionee);

     switch (_IOC_NR(cmd)) {
          case _IOC_NR(FUSION_REF_NEW):
               ret = fusion_ref_new(dev, fusionee, &id);
               if (ret)
                    return ret;

               if (put_user(id, (int *)arg)) {
                    fusion_ref_destroy(dev, id);
                    return -EFAULT;
               }
               return 0;

          case _IOC_NR(FUSION_REF_UP):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_up(dev, id, fusion_id);

          case _IOC_NR(FUSION_REF_UP_GLOBAL):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_up(dev, id, 0);

          case _IOC_NR(FUSION_REF_DOWN):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_down(dev, id, fusion_id);

          case _IOC_NR(FUSION_REF_DOWN_GLOBAL):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_down(dev, id, 0);

          case _IOC_NR(FUSION_REF_ZERO_LOCK):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_zero_lock(dev, id, fusion_id);

          case _IOC_NR(FUSION_REF_ZERO_TRYLOCK):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_zero_trylock(dev, id, fusion_id);

          case _IOC_NR(FUSION_REF_UNLOCK):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_zero_unlock(dev, id, fusion_id);

          case _IOC_NR(FUSION_REF_STAT):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               ret = fusion_ref_stat(dev, id, &refs);
               if (ret)
                    return ret;

               return refs;

          case _IOC_NR(FUSION_REF_WATCH):
               if (unlocked_copy_from_user
                   (&watch, (FusionRefWatch *) arg, sizeof(watch)))
                    return -EFAULT;

               return fusion_ref_watch(dev, watch.id, watch.call_id,
                                       watch.call_arg);

          case _IOC_NR(FUSION_REF_INHERIT):
               if (unlocked_copy_from_user
                   (&inherit, (FusionRefInherit *) arg, sizeof(inherit)))
                    return -EFAULT;

               return fusion_ref_inherit(dev, inherit.id, inherit.from);

          case _IOC_NR(FUSION_REF_SET_SYNC):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_set_sync(dev, id);

          case _IOC_NR(FUSION_REF_DESTROY):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_destroy(dev, id);

          case _IOC_NR(FUSION_REF_CATCH):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_ref_catch(dev, id, fusion_id);

          case _IOC_NR(FUSION_REF_THROW):
               if (unlocked_copy_from_user
                   (&throw_, (FusionRefThrow *) arg, sizeof(throw_)))
                    return -EFAULT;

               return fusion_ref_throw(dev, throw_.id, fusion_id, throw_.catcher);
     }

     return -ENOSYS;
}

static int
skirmish_ioctl(FusionDev * dev, Fusionee * fusionee,
               unsigned int cmd, unsigned long arg)
{
     int id;
     int ret;
     int lock_count;
     FusionSkirmishWait wait;
     FusionID fusion_id = fusionee_id(fusionee);

     switch (_IOC_NR(cmd)) {
          case _IOC_NR(FUSION_SKIRMISH_NEW):
               ret = fusion_skirmish_new(dev, fusionee, &id);
               if (ret)
                    return ret;

               if (put_user(id, (int *)arg)) {
                    fusion_skirmish_destroy(dev, id);
                    return -EFAULT;
               }
               return 0;

          case _IOC_NR(FUSION_SKIRMISH_PREVAIL):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_skirmish_prevail(dev, id, fusion_id);

          case _IOC_NR(FUSION_SKIRMISH_SWOOP):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_skirmish_swoop(dev, id, fusion_id);

          case _IOC_NR(FUSION_SKIRMISH_DISMISS):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_skirmish_dismiss(dev, id, fusion_id);

          case _IOC_NR(FUSION_SKIRMISH_DESTROY):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_skirmish_destroy(dev, id);

          case _IOC_NR(FUSION_SKIRMISH_LOCK_COUNT):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               ret =
               fusion_skirmish_lock_count(dev, id, fusion_id, &lock_count);
               if (put_user(lock_count, ((int *)arg) + 1))
                    return -EFAULT;

               return ret;

          case _IOC_NR(FUSION_SKIRMISH_WAIT):
               if (unlocked_copy_from_user
                   (&wait, (FusionSkirmishWait *) arg, sizeof(wait)))
                    return -EFAULT;

               ret = fusion_skirmish_wait_(dev, &wait, fusion_id);
               if (unlocked_copy_to_user
                   ((FusionSkirmishWait *) arg, &wait, sizeof(wait)))
                    return -EFAULT;

               return ret;

          case _IOC_NR(FUSION_SKIRMISH_NOTIFY):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_skirmish_notify_(dev, id, fusion_id);
     }

     return -ENOSYS;
}

static int
property_ioctl(FusionDev * dev, Fusionee * fusionee,
               unsigned int cmd, unsigned long arg)
{
     int id;
     int ret;
     FusionID fusion_id = fusionee_id(fusionee);

     switch (_IOC_NR(cmd)) {
          case _IOC_NR(FUSION_PROPERTY_NEW):
               ret = fusion_property_new(dev, fusionee, &id);
               if (ret)
                    return ret;

               if (put_user(id, (int *)arg)) {
                    fusion_property_destroy(dev, id);
                    return -EFAULT;
               }
               return 0;

          case _IOC_NR(FUSION_PROPERTY_LEASE):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_property_lease(dev, id, fusion_id);

          case _IOC_NR(FUSION_PROPERTY_PURCHASE):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_property_purchase(dev, id, fusion_id);

          case _IOC_NR(FUSION_PROPERTY_CEDE):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_property_cede(dev, id, fusion_id);

          case _IOC_NR(FUSION_PROPERTY_HOLDUP):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_property_holdup(dev, id, fusionee);

          case _IOC_NR(FUSION_PROPERTY_DESTROY):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_property_destroy(dev, id);
     }

     return -ENOSYS;
}

static int
reactor_ioctl(FusionDev * dev, Fusionee * fusionee,
              unsigned int cmd, unsigned long arg)
{
     int id;
     int ret;
     FusionReactorAttach attach;
     FusionReactorDetach detach;
     FusionReactorDispatch dispatch;
     FusionReactorSetCallback callback;
     FusionID fusion_id = fusionee_id(fusionee);

     switch (_IOC_NR(cmd)) {
          case _IOC_NR(FUSION_REACTOR_NEW):
               ret = fusion_reactor_new(dev, fusionee, &id);
               if (ret)
                    return ret;

               if (put_user(id, (int *)arg)) {
                    fusion_reactor_destroy(dev, id);
                    return -EFAULT;
               }
               return 0;

          case _IOC_NR(FUSION_REACTOR_ATTACH):
               if (dev->api.major <= 4) {
                    if (get_user(id, (int *)arg))
                         return -EFAULT;

                    attach.reactor_id = id;
                    attach.channel = 0;
               }
               else {
                    if (unlocked_copy_from_user(&attach,
                                       (FusionReactorAttach *) arg,
                                       sizeof(attach)))
                         return -EFAULT;
               }

               return fusion_reactor_attach(dev, attach.reactor_id,
                                            attach.channel, fusion_id);

          case _IOC_NR(FUSION_REACTOR_DETACH):
               if (dev->api.major <= 4) {
                    if (get_user(id, (int *)arg))
                         return -EFAULT;

                    detach.reactor_id = id;
                    detach.channel = 0;
               }
               else {
                    if (unlocked_copy_from_user(&detach,
                                       (FusionReactorDetach *) arg,
                                       sizeof(detach)))
                         return -EFAULT;
               }

               return fusion_reactor_detach(dev, detach.reactor_id,
                                            detach.channel, fusion_id);

          case _IOC_NR(FUSION_REACTOR_DISPATCH):
               if (unlocked_copy_from_user(&dispatch,
                                  (FusionReactorDispatch *) arg,
                                  sizeof(dispatch)))
                    return -EFAULT;

               if (dispatch.msg_size <= 0)
                    return -EINVAL;

               /* message data > 64k should be stored in shared memory */
               if (dispatch.msg_size > 0x10000)
                    return -EMSGSIZE;

               if (dev->api.major <= 4)
                    dispatch.channel = 0;

               return fusion_reactor_dispatch(dev, dispatch.reactor_id,
                                              dispatch.channel,
                                              dispatch.self ? NULL : fusionee,
                                              dispatch.msg_size,
                                              dispatch.msg_data);

          case _IOC_NR(FUSION_REACTOR_DESTROY):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_reactor_destroy(dev, id);

          case _IOC_NR(FUSION_REACTOR_SET_DISPATCH_CALLBACK):
               if (unlocked_copy_from_user(&callback,
                                  (FusionReactorSetCallback *) arg,
                                  sizeof(callback)))
                    return -EFAULT;

               return fusion_reactor_set_dispatch_callback(dev,
                                                           callback.reactor_id,
                                                           callback.call_id,
                                                           callback.call_ptr);
     }

     return -ENOSYS;
}

static int
shmpool_ioctl(FusionDev * dev, Fusionee * fusionee,
              unsigned int cmd, unsigned long arg)
{
     int id;
     int ret;
     FusionSHMPoolNew pool;
     FusionSHMPoolAttach attach;
     FusionSHMPoolDispatch dispatch;
     FusionID fusion_id = fusionee_id(fusionee);

     switch (_IOC_NR(cmd)) {
          case _IOC_NR(FUSION_SHMPOOL_NEW):
               if (unlocked_copy_from_user
                   (&pool, (FusionSHMPoolNew *) arg, sizeof(pool)))
                    return -EFAULT;

               ret = fusion_shmpool_new(dev, fusionee, &pool);
               if (ret)
                    return ret;

               if (unlocked_copy_to_user((FusionSHMPoolNew *) arg, &pool, sizeof(pool))) {
                    fusion_shmpool_destroy(dev, pool.pool_id);
                    return -EFAULT;
               }

               return 0;

          case _IOC_NR(FUSION_SHMPOOL_ATTACH):
               if (unlocked_copy_from_user(&attach,
                                  (FusionSHMPoolAttach *) arg, sizeof(attach)))
                    return -EFAULT;

               ret = fusion_shmpool_attach(dev, &attach, fusion_id);
               if (ret)
                    return ret;

               if (unlocked_copy_to_user
                   ((FusionSHMPoolAttach *) arg, &attach, sizeof(attach))) {
                    fusion_shmpool_detach(dev, attach.pool_id, fusion_id);
                    return -EFAULT;
               }

               return 0;

          case _IOC_NR(FUSION_SHMPOOL_DETACH):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_shmpool_detach(dev, id, fusion_id);

          case _IOC_NR(FUSION_SHMPOOL_DISPATCH):
               if (unlocked_copy_from_user(&dispatch,
                                  (FusionSHMPoolDispatch *) arg,
                                  sizeof(dispatch)))
                    return -EFAULT;

               return fusion_shmpool_dispatch(dev, &dispatch, fusionee);

          case _IOC_NR(FUSION_SHMPOOL_DESTROY):
               if (get_user(id, (int *)arg))
                    return -EFAULT;

               return fusion_shmpool_destroy(dev, id);

          case _IOC_NR(FUSION_SHMPOOL_GET_BASE):
               if (put_user(dev->shm_base, (unsigned long *)arg))
                    return -EFAULT;

               return 0;
     }

     return -ENOSYS;
}

static int
check_permission( FusionEntries *entries,
                  Fusionee      *fusionee,
                  unsigned int   cmd,
                  unsigned long  arg )
{
     int      ret;
     FusionID fusion_id = fusionee_id( fusionee );
     int      nr        = _IOC_NR( cmd );
     int      entry_id;

     /* Master can do everything */
     if (fusion_id == FUSION_ID_MASTER)
          return 0;

     /* Allow _NEW for now */
     if (!nr)
          return 0;

     /* Get Entry ID */
     ret = get_user( entry_id, (int*) arg );
     if (ret)
          return ret;

     /* Lookup entry and check permission bits */
     return fusion_entry_check_permissions( entries, entry_id, fusion_id, nr );
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
static long
fusion_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#else
static int
fusion_ioctl(struct inode *inode, struct file *file,
             unsigned int cmd, unsigned long arg)
#endif
{
     int        ret      = -ENOSYS;
     Fusionee  *fusionee = file->private_data;
     FusionDev *dev      = fusionee->fusion_dev;

//     FUSION_DEBUG("fusion_ioctl (0x%08x)\n", cmd);

     fusion_core_lock( fusion_core );

     fusionee_ref( fusionee );

     switch (_IOC_TYPE(cmd)) {
          case FT_LOUNGE:
               ret = lounge_ioctl(dev, fusionee, cmd, arg);
               break;

          case FT_MESSAGING:
               ret = messaging_ioctl(dev, fusionee, cmd, arg);
               break;

          case FT_CALL:
               if (dev->secure) {
                    ret = check_permission( &dev->call, fusionee, cmd, arg );
                    if (ret)
                         break;
               }
               ret = call_ioctl(dev, fusionee, cmd, arg);
               break;

          case FT_REF:
               if (dev->secure) {
                    ret = check_permission( &dev->ref, fusionee, cmd, arg );
                    if (ret)
                         break;
               }
               ret = ref_ioctl(dev, fusionee, cmd, arg);
               break;

          case FT_SKIRMISH:
               if (dev->secure) {
                    ret = check_permission( &dev->skirmish, fusionee, cmd, arg );
                    if (ret)
                         break;
               }
               ret = skirmish_ioctl(dev, fusionee, cmd, arg);
               break;

          case FT_PROPERTY:
               if (dev->secure) {
                    ret = check_permission( &dev->properties, fusionee, cmd, arg );
                    if (ret)
                         break;
               }
               ret = property_ioctl(dev, fusionee, cmd, arg);
               break;

          case FT_REACTOR:
               if (dev->secure) {
                    ret = check_permission( &dev->reactor, fusionee, cmd, arg );
                    if (ret)
                         break;
               }
               ret = reactor_ioctl(dev, fusionee, cmd, arg);
               break;

          case FT_SHMPOOL:
               if (dev->secure && cmd != FUSION_SHMPOOL_GET_BASE) {
                    ret = check_permission( &dev->shmpool, fusionee, cmd, arg );
                    if (ret)
                         break;
               }
               ret = shmpool_ioctl(dev, fusionee, cmd, arg);
               break;
     }

     fusionee_unref( fusionee );

     fusion_core_unlock( fusion_core );

     return ret;
}

#ifdef FUSION_CORE_SHMPOOLS
static int fusion_mmap(struct file *file, struct vm_area_struct *vma)
{
     int           ret;
     unsigned int  size;
     Fusionee     *fusionee = file->private_data;
     FusionDev    *dev      = fusionee->fusion_dev;

     fusion_core_lock( fusion_core );

     // FIXME: compile switch!
     vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

     if (vma->vm_pgoff != 0) {
          ret = fusion_shmpool_map(dev, vma);
     }
     else {
          size = vma->vm_end - vma->vm_start;
          if (!size || size > PAGE_SIZE) {
               fusion_core_unlock( fusion_core );
               return -EINVAL;
          }

          if (!dev->shared_area) {
               if (fusionee_id(fusionee) != FUSION_ID_MASTER) {
                    fusion_core_unlock( fusion_core );
                    return -EPERM;
               }

               dev->shared_area = fusion_core_malloc( fusion_core, PAGE_SIZE );
               if (!dev->shared_area) {
                    fusion_core_unlock( fusion_core );
                    return -ENOMEM;
               }
          }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
          ret = remap_pfn_range(vma, vma->vm_start,
                                virt_to_phys((void *)dev->
                                             shared_area) >> PAGE_SHIFT,
                                PAGE_SIZE, vma->vm_page_prot);
#else
          ret = io_remap_page_range(vma->vm_start,
                                    virt_to_phys((void *)dev->shared_area),
                                    PAGE_SIZE, vma->vm_page_prot);
#endif
     }

     fusion_core_unlock( fusion_core );

     return ret;
}
#else
static int fusion_mmap(struct file *file, struct vm_area_struct *vma)
{
     int ret;
     Fusionee  *fusionee = file->private_data;
     FusionDev *dev      = fusionee->fusion_dev;
     unsigned int size;

     if (vma->vm_pgoff != 0)
          return -EINVAL;

     size = vma->vm_end - vma->vm_start;
     if (!size || size > PAGE_SIZE)
          return -EINVAL;

     if (fusionee_id(fusionee) != FUSION_ID_MASTER && (vma->vm_flags & VM_WRITE))
          return -EPERM;

     fusion_core_lock( fusion_core );

     if (!dev->shared_area) {
          if (fusionee_id(fusionee) != FUSION_ID_MASTER) {
               fusion_core_unlock( fusion_core );
               return -EPERM;
          }

          dev->shared_area = get_zeroed_page(GFP_ATOMIC);
          if (!dev->shared_area) {
               fusion_core_unlock( fusion_core );
               return -ENOMEM;
          }

          SetPageReserved(virt_to_page((void*)dev->shared_area));
     }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
     ret = remap_pfn_range(vma, vma->vm_start,
                           virt_to_phys((void *)dev->
                                        shared_area) >> PAGE_SHIFT,
                           PAGE_SIZE, vma->vm_page_prot);
#else
     ret = io_remap_page_range(vma->vm_start,
                               virt_to_phys((void *)dev->shared_area),
                               PAGE_SIZE, vma->vm_page_prot);
#endif

     fusion_core_unlock( fusion_core );

     return ret;
}
#endif

static struct file_operations fusion_fops = {
     .owner = THIS_MODULE,
     .open = fusion_open,
     .flush = fusion_flush,
     .release = fusion_release,
     .read = fusion_read,
//     .poll = fusion_poll,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
     .unlocked_ioctl = fusion_ioctl,
#else
     .ioctl = fusion_ioctl,
#endif
#ifdef CONFIG_COMPAT
     .compat_ioctl = fusion_ioctl,
#endif
     .mmap = fusion_mmap
};

/******************************************************************************/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
static int __init register_devices(void)
{
     int i;

     fusion_major = FUSION_MAJOR;

     if (register_chrdev(fusion_major, "fusion", &fusion_fops)) {
          fusion_major = register_chrdev(0, "fusion", &fusion_fops);
          if (fusion_major <= 0) {
               printk (KERN_ERR "fusion: unable to register fusion device\n");
               return -EIO;
          }
          printk(KERN_ERR "fusion: unable to register major %d. Registered %d instead\n", FUSION_MAJOR, fusion_major);
     }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13)
     fusion_class = class_create(THIS_MODULE, "fusion");
#else
     fusion_class = class_simple_create(THIS_MODULE, "fusion");
#endif
     if (IS_ERR(fusion_class)) {
          unregister_chrdev(fusion_major, "fusion");
          return PTR_ERR(fusion_class);
     }
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18)
     devfs_mk_dir("fusion");
#endif

     for (i = 0; i < NUM_MINORS; i++) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 27)
          device_create(fusion_class,
                        NULL,
                        MKDEV(fusion_major, i), NULL, "fusion%d", i);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 26)
          device_create(fusion_class,
                        NULL, MKDEV(fusion_major, i), "fusion%d", i);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 15)
          class_device_create(fusion_class,
                              NULL,
                              MKDEV(fusion_major, i),
                              NULL, "fusion%d", i);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13)
          class_device_create(fusion_class,
                              MKDEV(fusion_major, i),
                              NULL, "fusion%d", i);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2)
          class_simple_device_add(fusion_class,
                                  MKDEV(fusion_major, i),
                                  NULL, "fusion%d", i);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18)
          devfs_mk_cdev(MKDEV(fusion_major, i),
                        S_IFCHR | S_IRUSR | S_IWUSR, "fusion/%d", i);
#endif
     }

     return 0;
}
#else
static int __init register_devices(void)
{
     int i;
     char buf[16];

     if (devfs_register_chrdev(fusion_major, "fusion", &fusion_fops)) {
          printk(KERN_ERR "fusion: unable to get major %d\n",
                 fusion_major);
          return -EIO;
     }

     for (i = 0; i < NUM_MINORS; i++) {
          snprintf(buf, 16, "fusion/%d", i);

          devfs_handles[i] = devfs_register(NULL, buf, DEVFS_FL_DEFAULT,
                                            fusion_major, i,
                                            S_IFCHR | S_IRUSR | S_IWUSR,
                                            &fusion_fops, NULL);
     }

     return 0;
}
#endif


ulong cpu;

module_param( cpu, ulong, 0 );
MODULE_PARM_DESC( cpu, "CPU index");

int __init fusion_init(void)
{
     int ret;

     printk( KERN_INFO "Starting fusion driver v%d.%d.%d\n",
             FUSION_API_MAJOR_PROVIDED,
             FUSION_API_MINOR_PROVIDED,
             FUSION_API_MICRO_PROVIDED);

     fusion_core_enter( cpu, &fusion_core );

     if (cpu) {
          shared = (FusionShared*) fusion_core_get_pointer( fusion_core, 0 );

          printk( KERN_INFO "  -> joining shared area at %p\n", shared );
     }
     else {
          shared = fusion_core_malloc( fusion_core, sizeof(FusionShared) );
          if (!shared)
               return -ENOMEM;

          printk( KERN_INFO "  -> initializing shared area at %p\n", shared );

          memset( shared, 0, sizeof(FusionShared) );

#if !FUSION_SHM_PER_WORLD_SPACE
          shared->addr_base = (void*) fusion_shm_base + 0x80000;
#endif

          fusion_core_set_pointer( fusion_core, 0, shared );
     }

     ret = register_devices();
     if (ret)
          return ret;

     proc_fusion_dir = proc_mkdir("fusion", NULL);

     return 0;
}

/******************************************************************************/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
static void __exit deregister_devices(void)
{
     int i;

     unregister_chrdev(fusion_major, "fusion");

     for (i = 0; i < NUM_MINORS; i++) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 26)
          device_destroy(fusion_class, MKDEV(fusion_major, i));
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13)
          class_device_destroy(fusion_class, MKDEV(fusion_major, i));
#else
          class_simple_device_remove(MKDEV(fusion_major, i));
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18)
          devfs_remove("fusion/%d", i);
#endif
     }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13)
     class_destroy(fusion_class);
#else
     class_simple_destroy(fusion_class);
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18)
     devfs_remove("fusion");
#endif
}
#else
static void __exit deregister_devices(void)
{
     int i;

     devfs_unregister_chrdev(fusion_major, "fusion");

     for (i = 0; i < NUM_MINORS; i++)
          devfs_unregister(devfs_handles[i]);
}
#endif

void __exit fusion_exit(void)
{
     deregister_devices();

     remove_proc_entry("fusion", NULL);

     fusion_core_free( fusion_core, shared );

     fusion_core_exit( fusion_core );
}

module_init(fusion_init);
module_exit(fusion_exit);

