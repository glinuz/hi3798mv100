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

#ifdef HAVE_LINUX_CONFIG_H
#include <linux/config.h>
#endif
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35)
#include <linux/smp_lock.h>
#endif
#include <linux/sched.h>

#include <linux/fusion.h>

#include "fusiondev.h"
#include "fusionee.h"
#include "list.h"
#include "hash.h"
#include "skirmish.h"
#include "call.h"

typedef struct {
     FusionLink link;

     Fusionee *caller;

     int ret_val;

     bool executed;
     bool signalled;
     bool interrupted;

     FusionWaitQueue wait;

     int call_id;
     unsigned int serial;
     int          caller_pid;

     unsigned int ret_size;
     unsigned int ret_length;

     /* return data follows */
} FusionCallExecution;

typedef struct {
     FusionEntry entry;

     Fusionee *fusionee;      /* owner */

     void *handler;
     void *ctx;

     FusionLink *executions;

     int count;          /* number of calls ever made */

     unsigned int serial;

     FusionHash *quotas;
} FusionCall;

typedef struct {
     FusionID            fusion_id;

     unsigned int        count;
     unsigned int        limit;

     FusionWaitQueue     wait;
} CallQuota;

/* collection, required for 1-param-only passing */
struct fusion_construct_ctx {
     Fusionee *fusionee;
     FusionCallNew *call_new;
};

/******************************************************************************/

static FusionCallExecution *add_execution(FusionCall * call,
                                          Fusionee * caller,
                                          unsigned int serial,
                                          unsigned int ret_size);
static void remove_execution(FusionCall * call,
                             FusionCallExecution * execution);
static void free_execution(FusionDev * dev,
                           FusionCallExecution * execution);
static void free_all_executions(FusionCall * call);

/******************************************************************************/

static int
fusion_call_construct(FusionEntry * entry, void *ctx, void *create_ctx)
{
     FusionCall *call = (FusionCall *) entry;

     struct fusion_construct_ctx *cc =
     (struct fusion_construct_ctx *)create_ctx;

     call->fusionee = cc->fusionee;
     call->handler = cc->call_new->handler;
     call->ctx = cc->call_new->ctx;

     fusion_hash_create( FHT_PTR, FHT_PTR, 5, &call->quotas );

     cc->call_new->call_id = entry->id;

     return 0;
}

static bool fusion_call_quota_hash_iterator( FusionHash *hash,
                                             void       *key,
                                             void       *value,
                                             void       *ctx )
{
     CallQuota  *quota = value;
     FusionCall *call  = ctx;

     fusionee_remove_message_callbacks( call->fusionee, quota );

     //if (quota->count >= quota->limit/4)
     //     printk( KERN_WARNING "fusion: call quota with count >= limit / 4 during destruction! call_id = %d, fusion_id = %lu, "
     //                          "count = %u/%u\n", call->entry.id, quota->fusion_id, quota->count, quota->limit );

     fusion_core_wq_deinit( fusion_core, &quota->wait );

     fusion_core_free( fusion_core, quota );

     return false;
}

static void fusion_call_destruct(FusionEntry * entry, void *ctx)
{
     FusionCall *call = (FusionCall *) entry;

     free_all_executions(call);

     fusion_hash_iterate( call->quotas, fusion_call_quota_hash_iterator, call );
     fusion_hash_destroy( call->quotas );
}

__attribute__((unused))
static void print_call( FusionCall* call )
{
     FusionEntry *entry;
     FusionLink *e;

     entry = &call->entry;

     printk( KERN_CRIT "%-2d %s, fid:%lu, %d calls)",
             entry->id,
             entry->name[0] ? entry->name : "???",
             call->fusionee->id,
             call->count);

     fusion_list_foreach(e, call->executions) {
          FusionCallExecution *exec = (FusionCallExecution *) e;

          printk( "/%lx:%s",
                  exec->caller   ? fusionee_id(exec->caller) : -1,
                  exec->executed ? "idle" : "busy" );
     }

     printk("\n");
}

typedef struct {
     char       string[900];
     int        offset;
} CallQuotaDumpContext;

static bool fusion_call_quota_hash_dump_iterator( FusionHash *hash,
                                                  void       *key,
                                                  void       *value,
                                                  void       *ctx )
{
     CallQuota            *quota      = value;
     CallQuotaDumpContext *quota_dump = ctx;

     int result;

     result = snprintf( quota_dump->string + quota_dump->offset, sizeof(quota_dump->string) - quota_dump->offset,
                        " 0x%08lx %u/%u ", quota->fusion_id, quota->count, quota->limit );
     if (result > 0)
          quota_dump->offset += result;

     return false;
}

static void
fusion_call_print(FusionEntry * entry, void *ctx, struct seq_file *p)
{
     FusionLink *e;
     bool idle = true;
     FusionCall *call = (FusionCall *) entry;
     CallQuotaDumpContext quota_dump = { .string[0] = 0, .offset = 0 };

     if (call->executions)
          idle = ((FusionCallExecution *) call->executions)->executed;

     fusion_hash_iterate( call->quotas, fusion_call_quota_hash_dump_iterator, &quota_dump );

     seq_printf(p, "(%d calls) %s [%s]",
                call->count, idle ? "idle" : "executing", quota_dump.string);

     fusion_list_foreach(e, call->executions) {
          FusionCallExecution *exec = (FusionCallExecution *) e;

          seq_printf(p, "  [0x%08lx]",
                     exec->caller ? fusionee_id(exec->caller) : 0);
     }

     seq_printf(p, "\n");
}

FUSION_ENTRY_CLASS(FusionCall, call, fusion_call_construct,
                   fusion_call_destruct, fusion_call_print)

/******************************************************************************/

int fusion_call_init(FusionDev * dev)
{
     fusion_entries_init(&dev->call, &call_class, dev, dev);

     fusion_entries_create_proc_entry(dev, "calls", &dev->call);

     return 0;
}

void fusion_call_deinit(FusionDev * dev)
{
     fusion_entries_destroy_proc_entry(dev, "calls");

     fusion_entries_deinit(&dev->call);
}

/******************************************************************************/

int fusion_call_new(FusionDev * dev, Fusionee *fusionee, FusionCallNew * call_new)
{
     int id;
     int ret;

     struct fusion_construct_ctx cc = { fusionee, call_new};

     ret = fusion_entry_create(&dev->call, &id, &cc, fusionee_id(fusionee));
     if (ret)
          return ret;

     return 0;
}

int
fusion_call_execute(FusionDev * dev, Fusionee * fusionee,
                    FusionCallExecute * execute)
{
     int ret;
     FusionCall *call;
     FusionCallExecution *execution = NULL;
     FusionCallMessage message;
     unsigned int serial;
     bool flush = true;

     FUSION_DEBUG( "%s( dev %p, fusionee %p, execute %p, call id %d, serial %u )\n", __FUNCTION__, dev, fusionee, execute,
                   execute->call_id, execute->serial );

restart:
     /* Lookup and lock call. */
     ret = fusion_call_lookup(&dev->call, execute->call_id, &call);
     if (ret)
          return ret;

     FUSION_DEBUG( "  -> call %u '%s'\n", call->entry.id, call->entry.name );

     if (execute->flags & FCEF_RESUMABLE && execute->serial != 0) {
          /* Search for execution, starting with oldest. */
          direct_list_foreach (execution, call->executions) {
               if (execution->serial == execute->serial)
                    break;
          }

          if (!execution) {
               printk( KERN_ERR "%s: resumable execution with serial %u not found!\n", __FUNCTION__, execute->serial );
               direct_list_foreach (execution, call->executions) {
                    printk( KERN_ERR "%s:   having serial %u\n", __FUNCTION__, execution->serial );
               }
               return -EIDRM;
          }
     }
     else {
          CallQuota             *quota    = NULL;
          FusionMessageCallback  callback = FMC_NONE;

          if (call->quotas->nnodes) {
               // TODO: optimise by keeping last quota lookup which is most likely the one we want
               quota = fusion_hash_lookup( call->quotas, (void*)(long) fusionee->id );
               if (quota) {
                    if (quota->count >= quota->limit) {
#ifdef FUSION_CALL_INTERRUPTIBLE
                         fusion_core_wq_wait( fusion_core, &quota->wait, 0, true );

                         if (signal_pending(current)) {
                              FUSION_DEBUG( "  -> woke up waiting for quota, SIGNAL PENDING!\n" );
                              return -EINTR;
                         }
#else
                         fusion_core_wq_wait( fusion_core, &quota->wait, 0, false );
#endif

                         goto restart;
                    }
               }
          }

          do {
               serial = ++call->serial;
          } while (!serial);

          /* Add execution to receive the result. */
          if (!(execute->flags & FCEF_ONEWAY)) {
               execution = add_execution(call, fusionee, serial, 0);
               if (!execution)
                    return -ENOMEM;

               FUSION_DEBUG( "  -> execution %p, serial %u\n", execution, execution->serial );
          }
          else if (execute->flags & FCEF_QUEUE)
               flush = false;

          /* Fill call message. */
          message.handler = call->handler;
          message.ctx = call->ctx;

          message.caller = fusionee ? fusionee_id(fusionee) : 0;

          message.call_arg = execute->call_arg;
          message.call_ptr = execute->call_ptr;

          message.serial = execution ? serial : 0;

          FUSION_DEBUG( "  -> sending call message, caller %u\n", message.caller );

          if (quota && ++quota->count % (quota->limit/4) == 0) {
               callback = FMC_CALL_QUOTA;
               flush    = true;
          }

          /* Put message into queue of callee. */
          ret = fusionee_send_message2(dev, fusionee, call->fusionee, FMT_CALL,
                                       call->entry.id, 0, sizeof(message),
                                       &message, callback, quota, 1, NULL, 0,
                                       flush);
          if (ret) {
               FUSION_DEBUG( "  -> MESSAGE SENDING FAILED! (ret %u)\n", ret );
               if (quota)
                    quota->count--;
               if (execution) {
                    remove_execution(call, execution);
                    free_execution(dev, execution);
               }
               return ret;
          }

          call->count++;
     }

     /* When waiting for a result... */
     if (execution) {
          FUSION_DEBUG( "  -> message sent, transfering all skirmishs...\n" );

          /* Transfer held skirmishs (locks). */
          if (fusionee && (!(execute->flags & FCEF_RESUMABLE) || execute->serial == 0))
               fusion_skirmish_transfer_all(dev, call->fusionee->id,
                                            fusionee_id(fusionee),
                                            fusion_core_pid( fusion_core ),
                                            execution->serial);

          while (!execution->executed) {
               /* Unlock call and wait for execution result. TODO: add timeout? */

               FUSION_DEBUG( "  -> skirmishs transferred, sleeping on call...\n" );

#ifdef FUSION_CALL_INTERRUPTIBLE
               fusion_core_wq_wait( fusion_core, &execution->wait, 0, true );

               if (signal_pending(current)) {
                    FUSION_DEBUG( "  -> woke up, SIGNAL PENDING!\n" );

                    execute->serial = execution->serial;

                    /* Indicate that a signal was received and execution won't be freed by caller. */
                    if (!(execute->flags & FCEF_RESUMABLE)) {
                         execution->signalled = true;

                         printk( KERN_EMERG "FusionIPC: Warning signal_pending(current) -> exit from fusion_call_execute() without FCEF_RESUMABLE. current->pid=%d, fid=0x%08x, call->fusion_id=%ld\n",
                                 current->pid,
                                 fusionee ? (int)(fusionee_id(fusionee)) : 0,
                                 call->fusionee->id );
                    }

                    return -EINTR;
               }
               if (execution->interrupted) {
                    return -EIDRM;
               }
#else
               fusion_core_wq_wait( fusion_core, &execution->wait, 0, false );
#endif
          }

          /* Return result to calling process. */
          execute->ret_val = execution->ret_val;

          /* Remove execution, freeing is up to caller. */
          remove_execution(call, execution);

          /* Free execution, which has already been removed by callee. */
          free_execution( dev, execution);

          FUSION_DEBUG( "  -> woke up, ret val %u, reclaiming skirmishs...\n", execute->ret_val );

          /* Reclaim skirmishs. */
          if (fusionee)
               fusion_skirmish_reclaim_all(dev, fusion_core_pid( fusion_core ));
     }
     else {
          FUSION_DEBUG( "  -> message sent, not waiting.\n" );
     }

     return 0;
}

int
fusion_call_execute2(FusionDev * dev, Fusionee * fusionee,
                     FusionCallExecute2 * execute)
{
     int ret;
     FusionCall *call;
     FusionCallExecution *execution = NULL;
     FusionCallMessage message;
     unsigned int serial;
     bool flush = true;

     FUSION_DEBUG( "%s( dev %p, fusionee %p, execute %p, call id %d, serial %u )\n", __FUNCTION__, dev, fusionee, execute,
                   execute->call_id, execute->serial );

restart:
     /* Lookup and lock call. */
     ret = fusion_call_lookup(&dev->call, execute->call_id, &call);
     if (ret)
          return ret;

     FUSION_DEBUG( "  -> call %u '%s'\n", call->entry.id, call->entry.name );

     if (execute->flags & FCEF_RESUMABLE && execute->serial != 0) {
          /* Search for execution, starting with oldest. */
          direct_list_foreach (execution, call->executions) {
               if (execution->serial == execute->serial)
                    break;
          }

          if (!execution) {
               printk( KERN_ERR "%s: resumable execution with serial %u not found!\n", __FUNCTION__, execute->serial );
               direct_list_foreach (execution, call->executions) {
                    printk( KERN_ERR "%s:   having serial %u\n", __FUNCTION__, execution->serial );
               }
               return -EIDRM;
          }
     }
     else {
          CallQuota             *quota    = NULL;
          FusionMessageCallback  callback = FMC_NONE;

          if (call->quotas->nnodes) {
               // TODO: optimise by keeping last quota lookup which is most likely the one we want
               quota = fusion_hash_lookup( call->quotas, (void*)(long) fusionee->id );
               if (quota) {
                    if (quota->count >= quota->limit) {
#ifdef FUSION_CALL_INTERRUPTIBLE
                         fusion_core_wq_wait( fusion_core, &quota->wait, 0, true );

                         if (signal_pending(current)) {
                              FUSION_DEBUG( "  -> woke up waiting for quota, SIGNAL PENDING!\n" );
                              return -EINTR;
                         }
#else
                         fusion_core_wq_wait( fusion_core, &quota->wait, 0, false );
#endif

                         goto restart;
                    }
               }
          }

          do {
               serial = ++call->serial;
          } while (!serial);

          /* Add execution to receive the result. */
          if (!(execute->flags & FCEF_ONEWAY)) {
               execution = add_execution(call, fusionee, serial, 0);
               if (!execution)
                    return -ENOMEM;

               FUSION_DEBUG( "  -> execution %p, serial %u\n", execution, execution->serial );
          }
          else if (execute->flags & FCEF_QUEUE)
               flush = false;

          /* Fill call message. */
          message.handler = call->handler;
          message.ctx = call->ctx;

          message.caller = fusionee ? fusionee_id(fusionee) : 0;

          message.call_arg = execute->call_arg;
          message.call_ptr = NULL;

          message.serial = execution ? serial : 0;

          FUSION_DEBUG( "  -> sending call message, caller %u\n", message.caller );

          if (quota && ++quota->count % (quota->limit/4) == 0) {
               callback = FMC_CALL_QUOTA;
               flush    = true;
          }

          /* Put message into queue of callee. */
          ret = fusionee_send_message2(dev, fusionee, call->fusionee, FMT_CALL,
                                       call->entry.id, 0, sizeof(FusionCallMessage),
                                       &message, callback, quota, 1, execute->ptr, execute->length,
                                       flush);
          if (ret) {
               FUSION_DEBUG( "  -> MESSAGE SENDING FAILED! (ret %u)\n", ret );
               if (quota)
                    quota->count--;
               if (execution) {
                    remove_execution(call, execution);
                    free_execution( dev, execution);
               }
               return ret;
          }

          call->count++;
     }

     /* When waiting for a result... */
     if (execution) {
          FUSION_DEBUG( "  -> message sent, transfering all skirmishs...\n" );

          /* Transfer held skirmishs (locks). */
          if (fusionee && (!(execute->flags & FCEF_RESUMABLE) || execute->serial == 0))
               fusion_skirmish_transfer_all(dev, call->fusionee->id,
                                            fusionee_id(fusionee),
                                            fusion_core_pid( fusion_core ),
                                            execution->serial);

          while (!execution->executed) {
               /* Unlock call and wait for execution result. TODO: add timeout? */

               FUSION_DEBUG( "  -> skirmishs transferred, sleeping on call...\n" );

#ifdef FUSION_CALL_INTERRUPTIBLE
               fusion_core_wq_wait( fusion_core, &execution->wait, 0, true );

               if (signal_pending(current)) {
                    FUSION_DEBUG( "  -> woke up, SIGNAL PENDING!\n" );

                    execute->serial = execution->serial;

                    /* Indicate that a signal was received and execution won't be freed by caller. */
                    if (!(execute->flags & FCEF_RESUMABLE)) {
                         execution->signalled = true;

                         printk( KERN_EMERG "FusionIPC: Warning signal_pending(current) -> exit from fusion_call_execute() without FCEF_RESUMABLE. current->pid=%d, fid=0x%08x, call->fusion_id=%ld\n",
                                 current->pid,
                                 fusionee ? (int)(fusionee_id(fusionee)) : 0,
                                 call->fusionee->id );
                    }

                    return -EINTR;
               }
               if (execution->interrupted) {
                    return -EIDRM;
               }
#else
               fusion_core_wq_wait( fusion_core, &execution->wait, 0, false );
#endif
          }

          /* Return result to calling process. */
          execute->ret_val = execution->ret_val;

          /* Remove execution, freeing is up to caller. */
          remove_execution(call, execution);

          /* Free execution, which has already been removed by callee. */
          free_execution( dev, execution);

          FUSION_DEBUG( "  -> woke up, ret val %u, reclaiming skirmishs...\n", execute->ret_val );

          /* Reclaim skirmishs. */
          if (fusionee)
               fusion_skirmish_reclaim_all(dev, fusion_core_pid( fusion_core ));
     }
     else {
          FUSION_DEBUG( "  -> message sent, not waiting.\n" );
     }

     return 0;
}

int
fusion_call_return(FusionDev * dev, int fusion_id, FusionCallReturn * call_ret)
{
     int ret;
     FusionCall *call;
     FusionCallExecution *execution;

     FUSION_DEBUG( "%s( dev %p, fusion_id %d )\n", __FUNCTION__, dev, fusion_id );

     if ( (dev->api.major >= 4) && (call_ret->serial == 0) )
          return -EOPNOTSUPP;

     /* Lookup and lock call. */
     ret = fusion_call_lookup(&dev->call, call_ret->call_id, &call);
     if (ret)
          return ret;

     FUSION_DEBUG( "  -> call %u '%s'\n", call->entry.id, call->entry.name );

     /* Search for execution, starting with oldest. */
     direct_list_foreach (execution, call->executions) {
          if ((execution->executed)
              || (execution->call_id != call_ret->call_id)
              || ((dev->api.major >= 4)
                  && (execution->serial != call_ret->serial))) {
               continue;
          }

          /*
           * Check if caller received a signal while waiting for the result.
           *
           * TODO: This is not completely solved. Restarting the system call
           * should be possible without causing another execution.
           */
          if (execution->signalled) {
               /* Remove and free execution. */
               remove_execution(call, execution);
               free_execution(dev, execution);
               return -EIDRM;
          }

          /* Write result to execution. */
          execution->ret_val = call_ret->val;
          execution->executed = true;

          /* FIXME: Caller might still have received a signal since check above. */
          FUSION_ASSERT(!execution->signalled);

          /* Return skirmishs. */
          fusion_skirmish_return_all(dev, fusion_id, execution->caller_pid, execution->serial);

          /* Wake up caller. */
          fusion_core_wq_wake( fusion_core, &execution->wait);

          return 0;
     }

     /* DirectFB 1.0.x does not handle one-way-calls properly */
     if (dev->api.major <= 3)
          return 0;

     return -ENOMSG;
}

int
fusion_call_execute3(FusionDev * dev, Fusionee * fusionee,
                     FusionCallExecute3 * execute)
{
     int ret;
     FusionCall *call;
     FusionCallExecution *execution = NULL;
     FusionCallMessage3 message;
     unsigned int serial;
     bool flush = true;

     FUSION_DEBUG( "%s( dev %p, fusionee %p, execute %p, call id %d, serial %u )\n", __FUNCTION__, dev, fusionee, execute,
                   execute->call_id, execute->serial );

restart:
     /* Lookup and lock call. */
     ret = fusion_call_lookup(&dev->call, execute->call_id, &call);
     if (ret)
          return ret;

     FUSION_DEBUG( "  -> call %u '%s'\n", call->entry.id, call->entry.name );

     if (execute->flags & FCEF_RESUMABLE && execute->serial != 0) {
          /* Search for execution, starting with oldest. */
          direct_list_foreach (execution, call->executions) {
               if (execution->serial == execute->serial)
                    break;
          }

          if (!execution) {
               printk( KERN_ERR "%s: resumable execution with serial %u not found!\n", __FUNCTION__, execute->serial );
               direct_list_foreach (execution, call->executions) {
                    printk( KERN_ERR "%s:   having serial %u\n", __FUNCTION__, execution->serial );
               }
               return -EIDRM;
          }
     }
     else {
          CallQuota             *quota    = NULL;
          FusionMessageCallback  callback = FMC_NONE;

          if (call->quotas->nnodes) {
               // TODO: optimise by keeping last quota lookup which is most likely the one we want
               quota = fusion_hash_lookup( call->quotas, (void*)(long) fusionee->id );
               if (quota) {
                    if (quota->count >= quota->limit) {
                         fusionee->wait_on_call_quota = execute->call_id;

#ifdef FUSION_CALL_INTERRUPTIBLE
                         fusion_core_wq_wait( fusion_core, &quota->wait, 0, true );

                         if (signal_pending(current)) {
                              FUSION_DEBUG( "  -> woke up waiting for quota, SIGNAL PENDING!\n" );
                              fusionee->wait_on_call_quota = 0;
                              return -EINTR;
                         }
#else
                         fusion_core_wq_wait( fusion_core, &quota->wait, 0, false );
#endif
                         fusionee->wait_on_call_quota = 0;

                         goto restart;
                    }
               }
          }

          do {
               serial = ++call->serial;
          } while (!serial);

          /* Add execution to receive the result. */
          if (!(execute->flags & FCEF_ONEWAY)) {
               execution = add_execution(call, fusionee, serial, execute->ret_length);
               if (!execution)
                    return -ENOMEM;

               FUSION_DEBUG( "  -> execution %p, serial %u\n", execution, execution->serial );
          }
          else if (execute->flags & FCEF_QUEUE)
               flush = false;

          /* Fill call message. */
          message.handler = call->handler;
          message.ctx = call->ctx;

          message.caller = fusionee ? fusionee_id(fusionee) : 0;

          message.call_arg    = execute->call_arg;
          message.call_ptr    = NULL;
          message.call_length = execute->length;
          message.ret_length  = execute->ret_length;

          message.serial = execution ? serial : 0;

          FUSION_DEBUG( "  -> sending call message, caller %u, ptr %p, length %u\n", message.caller, execute->ptr, execute->length );

          if (quota/* && ++quota->count % (quota->limit/4+1) == 0*/) {
               ++quota->count;
               callback = FMC_CALL_QUOTA;
               //flush    = true;
          }

          /* Put message into queue of callee. */
          ret = fusionee_send_message2(dev, fusionee, call->fusionee, FMT_CALL3,
                                       call->entry.id, 0, sizeof(FusionCallMessage3),
                                       &message, callback, quota, 1, execute->ptr, execute->length,
                                       flush);
          if (ret) {
               FUSION_DEBUG( "  -> MESSAGE SENDING FAILED! (ret %u)\n", ret );
               if (quota)
                    quota->count--;
               if (execution) {
                    remove_execution(call, execution);
                    free_execution(dev, execution);
               }
               return ret;
          }

          call->count++;
     }

     /* When waiting for a result... */
     if (execution) {
          FUSION_DEBUG( "  -> message sent, transfering all skirmishs...\n" );

          /* Transfer held skirmishs (locks). */
          if (fusionee && (!(execute->flags & FCEF_RESUMABLE) || execute->serial == 0))
               fusion_skirmish_transfer_all(dev, call->fusionee->id,
                                            fusionee_id(fusionee),
                                            fusion_core_pid( fusion_core ),
                                            execution->serial);

          while (!execution->executed) {
               /* Unlock call and wait for execution result. TODO: add timeout? */

               FUSION_DEBUG( "  -> skirmishs transferred, sleeping on call...\n" );

#ifdef FUSION_CALL_INTERRUPTIBLE
               fusion_core_wq_wait( fusion_core, &execution->wait, 0, true );

               if (signal_pending(current)) {
                    FUSION_DEBUG( "  -> woke up, SIGNAL PENDING!\n" );

                    execute->serial = execution->serial;

                    /* Indicate that a signal was received and execution won't be freed by caller. */
                    if (!(execute->flags & FCEF_RESUMABLE)) {
                         execution->signalled = true;

                         printk( KERN_EMERG "FusionIPC: Warning signal_pending(current) -> exit from fusion_call_execute() without FCEF_RESUMABLE. current->pid=%d, fid=0x%08x, call->fusion_id=%ld\n",
                                 current->pid,
                                 fusionee ? (int)(fusionee_id(fusionee)) : 0,
                                 call->fusionee->id );
                    }

                    return -EINTR;
               }
               if (execution->interrupted) {
                    return -EIDRM;
               }
#else
               fusion_core_wq_wait( fusion_core, &execution->wait, 0, false );
#endif
          }

          /* Return result to calling process. */
          if (execution->ret_length) {
               FUSION_DEBUG( "  -> ret_length %u, ret_size %u, ret_ptr %p\n", execution->ret_length, execution->ret_size, execute->ret_ptr );

               FUSION_ASSERT( execution->ret_length <= execution->ret_size );

               if (copy_to_user( execute->ret_ptr, execution + 1, execution->ret_length )) {
                    FUSION_DEBUG( "  -> ERROR COPYING RETURN DATA TO USER!\n" );
                    ret = -EFAULT;
               }
          }
          else
               ret = -ENODATA;

          execute->ret_length = execution->ret_length;

          /* Remove execution, freeing is up to caller. */
          remove_execution(call, execution);

          /* Free execution, which has already been removed by callee. */
          free_execution( dev, execution );

          FUSION_DEBUG( "  -> woke up, ret length %u, reclaiming skirmishs...\n", execute->ret_length );

          /* Reclaim skirmishs. */
          if (fusionee)
               fusion_skirmish_reclaim_all(dev, fusion_core_pid( fusion_core ));
     }
     else {
          FUSION_DEBUG( "  -> message sent, not waiting.\n" );
     }

     return ret;
}

int
fusion_call_return3(FusionDev * dev, int fusion_id, FusionCallReturn3 * call_ret)
{
     int ret;
     FusionCall *call;
     FusionCallExecution *execution;

     FUSION_DEBUG( "%s( dev %p, fusion_id %d ) <- serial %d\n", __FUNCTION__, dev, fusion_id, call_ret->serial );

     if ( (dev->api.major >= 4) && (call_ret->serial == 0) )
          return -EOPNOTSUPP;

     /* Lookup and lock call. */
     ret = fusion_call_lookup(&dev->call, call_ret->call_id, &call);
     if (ret)
          return ret;

     FUSION_DEBUG( "  -> call %u '%s'\n", call->entry.id, call->entry.name );

     /* Search for execution, starting with oldest. */
     direct_list_foreach (execution, call->executions) {
          if ((execution->executed)
              || (execution->call_id != call_ret->call_id)
              || ((dev->api.major >= 4)
                  && (execution->serial != call_ret->serial))) {
               continue;
          }

          /*
           * Check if caller received a signal while waiting for the result.
           *
           * TODO: This is not completely solved. Restarting the system call
           * should be possible without causing another execution.
           */
          if (execution->signalled) {
               /* Remove and free execution. */
               remove_execution(call, execution);
               free_execution(dev, execution);
               return -EIDRM;
          }

          if (execution->ret_size < call_ret->length) {
               /* Remove and free execution. */
               remove_execution(call, execution);
               free_execution(dev, execution);
               return -E2BIG;
          }

          /* Write result to execution. */
          if (copy_from_user( execution + 1, call_ret->ptr, call_ret->length )) {
               /* Remove and free execution. */
               remove_execution(call, execution);
               free_execution(dev, execution);
               return -EFAULT;
          }

          execution->ret_length = call_ret->length;
          execution->executed = true;

          /* FIXME: Caller might still have received a signal since check above. */
          FUSION_ASSERT(!execution->signalled);

          /* Return skirmishs. */
          fusion_skirmish_return_all(dev, fusion_id, execution->caller_pid, execution->serial);

          /* Wake up caller. */
          fusion_core_wq_wake( fusion_core, &execution->wait);

          return 0;
     }

     /* DirectFB 1.0.x does not handle one-way-calls properly */
     if (dev->api.major <= 3)
          return 0;

     return -ENOMSG;
}

int fusion_call_get_owner(FusionDev * dev, int call_id, FusionID *ret_fusion_id)
{
     int         ret;
     FusionCall *call;

     /* Lookup and lock call. */
     ret = fusion_call_lookup(&dev->call, call_id, &call);
     if (ret)
          return ret;

     *ret_fusion_id = call->entry.creator;

     return 0;
}

int fusion_call_set_quota(FusionDev * dev, FusionCallSetQuota *set_quota)
{
     int         ret;
     FusionCall *call;
     CallQuota  *quota;

     FUSION_DEBUG( "%s( dev %p, call_id %d, fusion_id %lu, limit %u )\n", __FUNCTION__,
                   dev, set_quota->call_id, set_quota->fusion_id, set_quota->limit );

     /* Lookup and lock call. */
     ret = fusion_call_lookup(&dev->call, set_quota->call_id, &call);
     if (ret)
          return ret;

     quota = fusion_hash_lookup( call->quotas, (void*)(long) set_quota->fusion_id );
     if (!quota) {
          quota = fusion_core_malloc( fusion_core, sizeof(CallQuota) );
          if (!quota)
               return -ENOMEM;

          quota->fusion_id = set_quota->fusion_id;
          quota->limit     = set_quota->limit;

          fusion_core_wq_init( fusion_core, &quota->wait );

          fusion_hash_insert( call->quotas, (void*)(long) set_quota->fusion_id, quota );
     }
     else {
          quota->limit = set_quota->limit;

          fusion_core_wq_wake( fusion_core, &quota->wait );
     }

     return 0;
}

int fusion_call_destroy(FusionDev * dev, Fusionee *fusionee, int call_id)
{
     int ret;
     FusionCall *call;
     FusionCallExecution *execution;

     do {
          ret = fusion_call_lookup(&dev->call, call_id, &call);
          if (ret)
               return ret;

          /* in secure mode, check if we own the call. */
          if (dev->secure && call->fusionee != fusionee)
               return -EIO;

          /* If an execution is pending... */
          execution = (FusionCallExecution *) call->executions;
          if (execution) {
               /* Unlock call and wait for execution. TODO: add timeout? */
               fusion_core_wq_wait( fusion_core, &execution->wait, 0, true);

               if (signal_pending(current))
                    return -EINTR;
          }
     } while (execution);

     fusion_entry_destroy_locked(call->entry.entries, &call->entry);

     return 0;
}

void fusion_call_destroy_all(FusionDev * dev, Fusionee *fusionee)
{
     FusionLink *l;

     FUSION_DEBUG( "%s( dev %p, fusion_id %lu )\n", __FUNCTION__, dev, fusionee->id );

     l = dev->call.list;

     while (l) {
          FusionLink *next = l->next;
          FusionCall *call = (FusionCall *) l;

          if (call->fusionee == fusionee) {
               FusionCallExecution *execution;

               /* If an execution is pending... */
               direct_list_foreach (execution, call->executions) {
                    /* indicate to caller that fusionee did quit */
                    execution->interrupted = true;
                    /* Wake up uninterruptibly(!) waiting caller! */
                    fusion_core_wq_wake( fusion_core, &execution->wait );
               }

               fusion_entry_destroy_locked(call->entry.entries,
                                           &call->entry);
          }
          else
          {
               /* Cleanup call executions initiated and not finished by fusionee */
               FusionCallExecution *execution, *next;
               direct_list_foreach_safe (execution, next, call->executions) {
                    if (execution->caller == fusionee) {
                         /* Remove and free execution */
                         remove_execution(call, execution);
                         free_execution(dev, execution);
                    }
               }
          }

          l = next;
     }
}

/******************************************************************************/

static FusionCallExecution *add_execution(FusionCall * call,
                                          Fusionee * caller,
                                          unsigned int serial,
                                          unsigned int ret_size)
{
     FusionCallExecution *execution;

     FUSION_DEBUG( "%s( call %p [%u], caller %p [%lu], serial %i )\n", __FUNCTION__, call, call->entry.id, caller, caller ? caller->id : 0, serial );

     /* Allocate execution. */

     if (ret_size < 20 && call->entry.entries->dev->execution_free_list_num > 0) {
          execution = (FusionCallExecution *)call->entry.entries->dev->execution_free_list;
          direct_list_remove( &call->entry.entries->dev->execution_free_list, &execution->link );
          call->entry.entries->dev->execution_free_list_num--;
     }
     else {
          execution = fusion_core_malloc( fusion_core, sizeof(FusionCallExecution) + (ret_size > CACHE_EXECUTIONS_DATA_LEN ? ret_size : CACHE_EXECUTIONS_DATA_LEN) );
     }
     if (!execution)
          return NULL;

     /* Initialize execution. */
     memset(execution, 0, sizeof(FusionCallExecution));

     execution->caller = caller;
     execution->caller_pid = fusion_core_pid( fusion_core );
     execution->call_id = call->entry.id;
     execution->serial = serial;
     execution->ret_size = ret_size;

     fusion_core_wq_init( fusion_core, &execution->wait);

     /* Add execution. */
     direct_list_append(&call->executions, &execution->link);

     return execution;
}

static void remove_execution( FusionCall * call, FusionCallExecution * execution )
{
     FUSION_DEBUG( "%s( call %p [%u], execution %p )\n", __FUNCTION__, call, call->entry.id, execution );

     fusion_list_remove( &call->executions, &execution->link );

     fusion_core_wq_wake( fusion_core, &execution->wait );
}

static void free_execution( FusionDev * dev, FusionCallExecution * execution )
{
     FUSION_DEBUG( "%s( execution %p )\n", __FUNCTION__, execution );

     if (execution->ret_size <= CACHE_EXECUTIONS_DATA_LEN && dev->execution_free_list_num < CACHE_EXECUTIONS_NUM) {
          direct_list_append( &dev->execution_free_list, &execution->link );

          dev->execution_free_list_num++;
     }
     else
          fusion_core_free( fusion_core, execution );
}

static void free_all_executions(FusionCall * call)
{
     FusionCallExecution *execution, *next;

     FUSION_DEBUG( "%s( call %p [%u] )\n", __FUNCTION__, call, call->entry.id );

     direct_list_foreach_safe (execution, next, call->executions) {
          remove_execution( call, execution );

          if (!execution->caller)
               free_execution( call->entry.entries->dev,  execution );
     }
}

void
fusion_call_quota_message_callback(FusionDev * dev, int id, void *ctx, int arg)
{
     CallQuota *quota = ctx;

     quota->count--;// -= quota->limit / 4 + 1;

     fusion_core_wq_wake( fusion_core, &quota->wait );
}

