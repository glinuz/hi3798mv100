#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/list.h>

#include "hi_drv_ir.h"
#include "drv_ir_protocol.h"
#include "drv_ir_utils.h"

#include "drv_ir_protocols_descript.c"

#include "hi_osal.h"

static struct list_head ir_prot_head;
DEFINE_SPINLOCK(irlock);
//static unsigned long flag;

#if 0
void ir_protocols_lock(unsigned long flag)
{
    spin_lock_irqsave(&lock, flag);
}

void ir_protocols_unlock(unsigned long flag)
{
    spin_unlock_irqrestore(&lock, flag);
}
#endif

/* store the max & min value of pluse and space to minx and maxx */
#define mm_ps(phase, minp, maxp, mins, maxs)            \
    do \
    {                                                    \
        u32 pluse, space, factor;           \
        pluse  = (u32)(phase)->pluse;            \
        space  = (u32)(phase)->space;            \
        factor = (phase)->factor;                       \
        minp = (pluse) * (100 - (factor)) / 100;        \
        maxp = (pluse) * (100 + (factor)) / 100;        \
        mins = (space) * (100 - (factor)) / 100;        \
        maxs = (space) * (100 + (factor)) / 100;        \
    } while (0)


int ir_protocol_check(struct ir_protocol *ip)
{
    struct list_head *c;

    struct ir_protocol *ci;

    if (((ip->node.next != NULL) && (ip->node.prev != NULL))
        && ((ip->node.next != &ip->node)
            && (ip->node.prev != &ip->node)))
    {
        hiir_info("protocol may be link to somewhere," \
                   "ip->node.next:%p,ip->node.prev:%p!\n",
                   ip->node.next, ip->node.prev);
        return -1;
    }

    if (!ip->ir_code_name || !ip->match || !ip->handle
        || (ip->idx >= IR_PROT_BUTT))
    {
        return -1;
    }

    if ((ip->flag & INFR_HAS_ERR_HANDLE)
        && (!ip->match_error || !ip->parse_error))
    {
        return -1;
    }

#if 0
    if (ip->idx >= IR_PROT_BUTT)
    {
        return -1;
    }
#endif

    if (list_empty(&ir_prot_head))
    {
        return 0;
    }

    list_for_each(c, &ir_prot_head)
    {
        ci = list_entry(c, struct ir_protocol, node);

        if (((ci->idx == ip->idx) && (ci->priv == ip->priv)))
        {
            hiir_info("protocol(%s)'s private data have been used by %s!\n",
                       ip->ir_code_name,
                       ci->ir_code_name);
            return -1;
        }

        if (!HI_OSAL_Strncmp(ci->ir_code_name, ip->ir_code_name, strlen(ip->ir_code_name)))
        {
            hiir_info("%s have been registered or used!\n", ip->ir_code_name);
            return -1;
        }
    }

    return 0;
}

static void inline init_mmps(struct ir_protocol *ip)
{
    mm_ps(&ip->attr.header, ip->attr.header.minp,
          ip->attr.header.maxp, ip->attr.header.mins,
          ip->attr.header.maxs);

    if (ip->attr.second_header.pluse || ip->attr.second_header.space)
    {
        mm_ps(&ip->attr.second_header, ip->attr.second_header.minp,
              ip->attr.second_header.maxp,
              ip->attr.second_header.mins,
              ip->attr.second_header.maxs);
    }

    mm_ps(&ip->attr.b0, ip->attr.b0.minp,
          ip->attr.b0.maxp, ip->attr.b0.mins,
          ip->attr.b0.maxs);

    mm_ps(&ip->attr.b1, ip->attr.b1.minp,
          ip->attr.b1.maxp, ip->attr.b1.mins,
          ip->attr.b1.maxs);

    mm_ps(&ip->attr.burst, ip->attr.burst.minp,
          ip->attr.burst.maxp, ip->attr.burst.mins,
          ip->attr.burst.maxs);

    if (ip->attr.repeat.pluse || ip->attr.repeat.space)
    {
        mm_ps(&ip->attr.repeat, ip->attr.repeat.minp,
              ip->attr.repeat.maxp, ip->attr.repeat.mins,
              ip->attr.repeat.maxs);
    }
}

int ir_register_protocol(struct ir_protocol *ip)
{
    struct list_head *curr;
    struct list_head *prev;
    struct ir_protocol *p;
	unsigned long flag;

    if (ir_protocol_check(ip))
    {
        return -1;
    }

    spin_lock_irqsave(&irlock, flag);

    INIT_LIST_HEAD(&ip->node);

    init_mmps(ip);

    if (strlen(ip->ir_code_name) >= PROTOCOL_NAME_SZ)
    {
        ip->ir_code_name[PROTOCOL_NAME_SZ - 1 ] = '\0';
    }

    if (list_empty_careful(&ir_prot_head))
    {
        list_add(&ip->node, &ir_prot_head);
        goto out;
    }

    /* We need to do some sort.
     * Sort rules:
     * 1. Some protocol has obvious header/burst and
     * fix number of bits data, and has second header, will be set
     * in the first part of the list.
     *
     * 2. Some protocol has obvious header/burst and
     * fix number of bits data, will be insert in the second part
     * of the list.
     *
     * 3. rc6 will be insert in the third part.
     * 4. rc5 at the last.
     * 5. others at the end.
     */
    if (ip->idx < IR_PROT_RC5)
    {
        if (ip->attr.second_header.pluse
            || ip->attr.second_header.space)
        {
            list_add(&ip->node, &ir_prot_head);
        }
        else
        {
            prev = ir_prot_head.next;
            list_for_each(curr, &ir_prot_head)
            {
                p = list_entry(curr, struct ir_protocol, node);
                if (!p->attr.second_header.pluse
                    && !p->attr.second_header.space)
                {
                    break;
                }

                prev = curr;
            }
            list_add(&ip->node, prev);
        }
    }
    else if (ip->idx == IR_PROT_RC6)
    {
        prev = ir_prot_head.next;
        list_for_each(curr, &ir_prot_head)
        {
            p = list_entry(curr, struct ir_protocol, node);
            if (p->idx < IR_PROT_RC5)
            {
                continue;
            }

            if (((p->idx == IR_PROT_RC6) && (p->attr.wanna_bits <= ip->attr.wanna_bits))
                || ((p->idx == IR_PROT_RC5) || (p->idx == IR_PROT_RC5X)))
            {
                break;
            }

            prev = curr;
        }
        list_add(&ip->node, prev);
    }
    else if ((ip->idx == IR_PROT_RC5) || (ip->idx == IR_PROT_RC5X))
    {
        prev = ir_prot_head.next;
        list_for_each(curr, &ir_prot_head)
        {
            p = list_entry(curr, struct ir_protocol, node);
            if ((p->idx != IR_PROT_RC5) && (p->idx != IR_PROT_RC5X))
            {
                continue;
            }

            if (p->attr.wanna_bits <= ip->attr.wanna_bits)
            {
                break;
            }

            prev = curr;
        }
        list_add(&ip->node, prev);
    }
    else
    {
        list_add_tail(&ip->node, &ir_prot_head);
    }

    if (strlen(ip->ir_code_name) >= PROTOCOL_NAME_SZ)
    {
        ip->ir_code_name[PROTOCOL_NAME_SZ - 1 ] = '\0';
    }

out:
     spin_unlock_irqrestore(&irlock, flag);
    return 0;
}

int ir_unregister_protocol(struct ir_protocol *ip)
{
	unsigned long flag;

    spin_lock_irqsave(&irlock, flag);

    list_del(&ip->node);

     spin_unlock_irqrestore(&irlock, flag);
    return 0;
}

int ir_protocol_init(void)
{
    int i, cnt;
    struct ir_protocol *ip;

    INIT_LIST_HEAD(&ir_prot_head);

    cnt = ARRAY_SIZE(ir_protocols);
    
    for (i = 0 ; i < cnt; i++)
    {
        ip = &ir_protocols[i];

        if (!ip->ir_code_name || !ip->match || !ip->handle
            || (ip->idx == IR_PROT_BUTT))
        {
            break;
        }

        if (ir_register_protocol(ip))
        {
            hiir_error("fail to regist protocol %s\n", ip->ir_code_name);
        }
    }

    if (!i)
    {
        hiir_info("no protocols registered!\n");
        return -1;
    }

    return 0;
}

int ir_protocol_exit(void)
{
    struct ir_protocol *ip;

	while(!list_empty_careful(&ir_prot_head))
	{
		ip = list_first_entry(&ir_prot_head, struct ir_protocol, node);

		(void)ir_unregister_protocol(ip);
	}

    return 0;
}

struct ir_protocol *ir_prot_first(void)
{
    struct ir_protocol *ip;

    if (list_empty_careful(&ir_prot_head))
    {
        return NULL;
    }

    ip = list_first_entry(&ir_prot_head, struct ir_protocol, node);

    return ip;
}

struct ir_protocol *ir_prot_next(struct ir_protocol *curr)
{
    struct list_head *c = &curr->node;
    struct ir_protocol *ip;

    if (list_is_last(c, &ir_prot_head))
    {
        return NULL;
    }

    c  = curr->node.next;
    ip = list_entry(c, struct ir_protocol, node);

    return ip;
}

int ir_prot_valid(struct ir_protocol *ip)
{
    return (ip && ip->ir_code_name && ip->idx != IR_PROT_BUTT);
}

/* Simply check the pluse and space of this key
 * is fall in the special phase's pluse and space.
 * return 0 while match, others not match.
 * NOTE: call this routine is not correct
 * if phase is not exist or is not completely exist
 * (both pluse and space) in a symbol sequence.
 */
int key_match_phase(struct key_attr *key, struct phase_attr *phase)
{
    u32 min, max;

    min = phase->minp;
    max = phase->maxp;
    if (data_fallin((u32)key->lower, min, max))
    {
        min = phase->mins;
        max = phase->maxs;
        if (data_fallin((u32)key->upper, min, max))
        {
            return 0;
        }
    }

    return 1;
}

EXPORT_SYMBOL(ir_register_protocol);
EXPORT_SYMBOL(ir_unregister_protocol);
