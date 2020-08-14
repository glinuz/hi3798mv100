/*****************Copyright (c) 2010 Hisilicon Tech Co., Ltd.**********************
    project: wish
    subprj:  device driver
    module:  audio codec
    file:    slac.c
    abstract:slac-ic driver
*******************************************************************************/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/kthread.h>

#include "slac_private.h"
#include "slac_public.h"
#include "vp_api.h"
#include "vp_api_common.h"
#include "profiles8.h"
#include "vp890_api.h"
#include "hi_module.h"
#include "hi_debug.h"
#include "hi_drv_proc.h"
#include "hi_kernel_adapt.h"

#ifdef  HI_ADVCA_FUNCTION_RELEASE
    #define TRC(x...)
#else
    #define TRC(x...)  printk(x)
#endif

extern VpDevCtxType devCtx;
extern VpLineCtxType lineCtx;
extern VpStatusType hisi_slac_init(void);

extern void hisi_slac_deinit(void);

extern void HandleDat(void);
extern VpEventType pSlacEvent;
extern void le89116_reg_read(unsigned char addr, unsigned char *val, unsigned char len);

extern void le89116_reg_write(unsigned char addr,unsigned char *val, unsigned char len);
#define VP_NULL_FF 0xffff
/*
* The tone profiles (pointers) in the following list are created from the
* Profile Wizard
*/

VpProfilePtrType tonePtrs[] = {
    TONE_DIAL, TONE_DIAL, TONE_BUSY, TONE_RINGBACK, TONE_ROH
};

/*
* The cadence profiles (pointers) in the following list are created from
* the Profile Wizard
*/
VpProfilePtrType cadencePtrs[] = {
    TONE_CAD_DIAL0, TONE_CAD_DIAL1, TONE_CAD_BUSY0, TONE_CAD_RNGBACK,
    TONE_CAD_ROH
};
/*major device ID*/
//static unsigned int slac_device_major = 0;

static atomic_t   open_cnt = ATOMIC_INIT(0);

/*10ms timer*/
#define SLAC_TIMER_LOOP 0
static struct timer_list slac_timer;
unsigned char wake_handle_type = 0;

static DECLARE_WAIT_QUEUE_HEAD(slac_wait);

/*semphore*/
HI_DECLARE_MUTEX(s_SlacMute);
#if 1
#define SLAC_LOCK()  {if(down_interruptible(&s_SlacMute)) return -1;}
#define SLAC_UNLOCK() up(&s_SlacMute)
#else
#define SLAC_LOCK()
#define SLAC_UNLOCK()
#endif

/*synchronization*/
static int s32SlacTickWaitFlag = 0;
static wait_queue_head_t WaitQueueSlacTick;
static struct task_struct *pSlacTickThread = NULL;

/*Modified for sometimes SLIC not responce when be called again and again*/
int Slac_ProcRead(struct seq_file *p, void *v)
{
    VpStatusType res;
    uint32  u32DeviceStatus;
    bool    bLineStatus;
    VpLineStateType stLineState;
    VpInputType input;

    const char * Input2Str[] = {
        "HOOK",                  /* Hook Status (ignoring pulse & flash) */
        "RAW_HOOK",          /* Hook Status (include pulse & flash) */
        "GKEY",                   /* Ground-Key/Fault Status */
        "THERM_FLT",          /* Thermal Fault Status */
        "CLK_FLT",   /* Clock Fault Status */
        "AC_FLT",    /* AC Fault Status */
        "DC_FLT",    /* DC Fault Status */
        "BAT1_FLT",  /* Battery 1 Fault Status */
        "BAT2_FLT",  /* Battery 2 Fault Status */
        "BAT3_FLT",  /* Battery 3 Fault Status */

        "RINGING",   /* Ringing Status */
        "LIU",       /* Line In Use Status */
        "FEED_DIS",  /* Feed Disable Status */
        "FEED_EN",   /* Feed Enable Status */
        "DISCONNECT",/* Feed Disconnect Status */
        "CONNECT",   /* Feed Connect Status */
        "POLREV",    /* Reverse Polarity if TRUE, Normal if FALSE */
        "RES_LEAK",    /* Resistive Leakage on line if TRUE */
    };

    const char * LineState2Str[]={
        "STANDBY",        /**< Low power line feed state */
        "TIP_OPEN",       /**< Tip open circuit state */
        "ACTIVE",         /**< Line Feed w/out VF */
        "ACTIVE_POLREV",  /**< Polarity Reversal Line Feed w/out VF */
        "TALK",           /**< Normal off-hook Active State; Voice Enabled */
        "TALK_POLREV",    /**< Normal Active with reverse polarity; Voice Enabled */

        "OHT",            /**< On-Hook tranmission state */
        "OHT_POLREV",     /**< Polarity Reversal On-Hook tranmission state */

        "DISCONNECT",     /**< Denial of service */
        "RINGING",        /**< Ringing state */
        "RINGING_POLREV", /**< Ringing w/Polarity Reversal */

        "INVALID_STATE",
        "INVALID_STATE",
        "INVALID_STATE",
        "INVALID_STATE",
        "INVALID_STATE",

        "STANDBY_POLREV", /**< Low power polrev line feed state */
        "PARK",           /**< Park mode */
        "RING_OPEN",      /**< Ring open */
        "HOWLER",         /**< Howler */
        "TESTING",        /**< Testing */
        "DISABLED",       /**< Disabled */
        "NULLFEED",       /**< Null-feed */

        "FXO_OHT",        /**< FXO Line providing Loop Open w/VF */
        "FXO_LOOP_OPEN",  /**< FXO Line providing Loop Open w/out VF */
        "FXO_LOOP_CLOSE", /**< FXO Line providing Loop Close w/out VF */
        "FXO_TALK",       /**< FXO Line providing Loop Close w/VF */
        "FXO_RING_GND",   /**< FXO Line providing Ring Ground (GS only)*/

    };

    p += seq_printf(p,"---------Hisilicon Slac Driver Info---------\n");

    SLAC_LOCK();

    if(atomic_read(&open_cnt) == 0)
    {
        SLAC_UNLOCK();
        p += seq_printf(p,"The slac is not opened.\n");
        return 0;
    }

    res = VpGetLineState(&lineCtx, &stLineState);
    if ( res ==  VP_STATUS_SUCCESS )
    {
        if (0 <= stLineState && 28 > stLineState) //for fix MOTO
        {
            p += seq_printf(p,"Vp Line State: %d , %s\n",stLineState,LineState2Str[stLineState]);
        }
    }
    else
    {
        p += seq_printf(p,"VpGetLineState() failure, %d\n",res);
    }

    p += seq_printf(p,"\n");

    for(input = VP_INPUT_HOOK; input < VP_NUM_INPUT_TYPES; input++)
    {
        res = VpGetDeviceStatus(&devCtx, input, &u32DeviceStatus);
        if ( res ==  VP_STATUS_SUCCESS )
        {
            p += seq_printf(p,"Vp Device Status: input %s, status %d\n",Input2Str[input],(int)u32DeviceStatus);
        }
        else
        {
            p += seq_printf(p,"VpGetDeviceStatus() failure, input %s, res %d\n",Input2Str[input],res);
        }
    }

    p += seq_printf(p,"\n");

    for(input = VP_INPUT_HOOK; input < VP_NUM_INPUT_TYPES; input++)
    {
        res = VpGetLineStatus(&lineCtx, input, &bLineStatus);
        if ( res ==  VP_STATUS_SUCCESS )
        {
            p += seq_printf(p,"Vp Line Status: input %s, status %s\n",Input2Str[input],bLineStatus?"TRUE":"FALSE");
        }
        else
        {
            p += seq_printf(p,"VpGetLineStatus() failure, input %s, res %d\n",Input2Str[input],res);
        }
    }
    {
        HI_U8   data[2];
        p += seq_printf(p,"\n");

        le89116_reg_read(0x73, data, 2);    //read chip version and product code number
        p += seq_printf(p,"SLIC 0x73 REG is 0x%x 0x%x\n",data[0],data[1]);

        le89116_reg_read(0x41, data, 1);
        p += seq_printf(p,"SLIC 0x41 REG is 0x%x\n",data[0]);

        le89116_reg_read(0x43, data, 1);
        p += seq_printf(p,"SLIC 0x43 REG is 0x%x\n",data[0]);

        le89116_reg_read(0x61, data, 1);
        p += seq_printf(p,"SLIC 0x61 REG is 0x%x\n",data[0]);

        le89116_reg_read(0x45, data, 1);
        p += seq_printf(p,"SLIC 0x45 REG is 0x%x\n",data[0]);

        le89116_reg_read(0x4b, data, 1);
        p += seq_printf(p,"SLIC 0x4B REG is 0x%x\n",data[0]);
    }

    SLAC_UNLOCK();
    p += seq_printf(p,"\n");
    return 0;
}

int Slac_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)

{
    HI_U8           ProcPara[256] = {0};
    char *p;
    HI_U8   cmd;
    HI_U8   addr;
    HI_U8   value;

    SLAC_LOCK();

    if (copy_from_user(ProcPara, buf, count < sizeof(ProcPara) - 1 ? count : sizeof(ProcPara) - 1))
    {
        SLAC_UNLOCK();
        return -1;
    }

    if(atomic_read(&open_cnt) == 0)
    {
        SLAC_UNLOCK();
        //HI_TRACE();
        TRC("The slac is not opened.\n");
        return -1;
    }

    p = ProcPara;

    cmd = (HI_U8)simple_strtoul(p,&p,16);
    addr = (HI_U8)simple_strtoul(p+1,&p,16);
    if(cmd == 0)    //read reg
    {
        le89116_reg_read(addr, &value, 1);
    }
    else if(cmd == 1)   //write to reg
    {
        value = (HI_U8)simple_strtoul(p+1,&p,16);
        le89116_reg_write(addr, &value, 1);
    }

    SLAC_UNLOCK();
    TRC("%s(), %s slac reg, addr 0x%x, value 0x%x\n",__FUNCTION__,cmd?"write":"read",addr,value);
    return count;
}


static unsigned int slac_select(struct file *filp, struct poll_table_struct *wait)
{
    SLAC_LOCK();
    if(wake_handle_type == DAT_WAKEUP)
    {
        SLAC_UNLOCK();
        return POLLIN;
    }
    SLAC_UNLOCK();
    poll_wait(filp, &(slac_wait), wait);
    return 0;
}

/*BEGIN: Modified for sometimes SLIC not responce when be called again and again*/

#if 0
/*slac 10ms timer interrupt*/
static void slac_timer_handle(unsigned long arg)
{
    VpStatusType status;
    VpLineStateType stCurrentState;

    //TRC(" slac HandleDat: driver:timer is occur\n");
    HandleDat();
    if ((wake_handle_type == DAT_WAKEUP) && (pSlacEvent.eventCategory != VP_EVCAT_RESPONSE))
    {
        //        TRC("slac HandleDat: driver:dat event is occur, wake up. cat=0x%x id=0x%x\n",
        //            pSlacEvent.eventCategory, pSlacEvent.eventId);
        if (pSlacEvent.eventCategory == VP_EVCAT_SIGNALING)
        {
            if (pSlacEvent.eventId == VP_LINE_EVID_HOOK_OFF)
            {
                VpSetLineState(&lineCtx, VP_LINE_TALK);

                //VpSetLineTone(&lineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL); //k00144455 remove temporarily
            }
            else if (pSlacEvent.eventId == VP_LINE_EVID_HOOK_ON)
            {
                status = VpGetLineState(&lineCtx, &stCurrentState);
                if (VP_STATUS_SUCCESS == status)
                {
                    if ((VP_LINE_STANDBY != stCurrentState) && (VP_LINE_RINGING != stCurrentState))
                    {
                        VpSetLineState(&lineCtx, VP_LINE_STANDBY);

                        //TRC( "VpInitLine detect  VP_LINE_EVID_HOOK_ON\n");
                    }
                }
            }

            wake_up_interruptible(&slac_wait);
        }
    }

    slac_timer.function = &slac_timer_handle;
    slac_timer.expires = jiffies + SLAC_TIMER_LOOP; //10ms
    add_timer(&slac_timer);

}
#else

/*slac 10ms timer interrupt*/
static void slac_timer_handle(unsigned long arg)
{
    s32SlacTickWaitFlag = 1;
    wake_up_interruptible(&WaitQueueSlacTick);

    slac_timer.function = &slac_timer_handle;
    slac_timer.expires = jiffies + msecs_to_jiffies(10); //10ms
    add_timer(&slac_timer);
}

#endif

static int slac_tick_proc(void * argv)
{
    VpStatusType status;
    VpLineStateType stCurrentState;
    int ret;
//    TRC("%s() start.\n",__FUNCTION__);
    while(s32SlacTickWaitFlag >= 0)
    {

        ret = wait_event_interruptible(WaitQueueSlacTick, (0 != s32SlacTickWaitFlag));

        if(s32SlacTickWaitFlag < 0)
        {
            break;
        }

        if (kthread_should_stop())
        {
            break;
        }

        if(s32SlacTickWaitFlag > 0)
        {

            SLAC_LOCK();

            HandleDat();

            if ((wake_handle_type == DAT_WAKEUP) && (pSlacEvent.eventCategory != VP_EVCAT_RESPONSE))
            {
                //        TRC("slac HandleDat: driver:dat event is occur, wake up. cat=0x%x id=0x%x\n",
                //            pSlacEvent.eventCategory, pSlacEvent.eventId);
                if (pSlacEvent.eventCategory == VP_EVCAT_SIGNALING)
                {
                    if (pSlacEvent.eventId == VP_LINE_EVID_HOOK_OFF)
                    {
                        VpSetLineState(&lineCtx, VP_LINE_TALK);

                        //VpSetLineTone(&lineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, VP_NULL); //k00144455 remove temporarily
                    }
                    else if (pSlacEvent.eventId == VP_LINE_EVID_HOOK_ON)
                    {
                        status = VpGetLineState(&lineCtx, &stCurrentState);
                        if (VP_STATUS_SUCCESS == status)
                        {
                            if ((VP_LINE_STANDBY != stCurrentState) && (VP_LINE_RINGING != stCurrentState))
                            {
                                VpSetLineState(&lineCtx, VP_LINE_STANDBY);

                                //TRC( "VpInitLine detect  VP_LINE_EVID_HOOK_ON\n");
                            }
                        }
                    }

                    wake_up_interruptible(&slac_wait);

                }
            }
            SLAC_UNLOCK();

        }
        s32SlacTickWaitFlag = 0;
    }
    s32SlacTickWaitFlag = 0;
//    TRC("%s() stop.\n",__FUNCTION__);
    return 0;
}

/*END: Modified for sometimes SLIC not responce when be called again and again*/

#if 0
static void timeout_handler(unsigned long data);
static DEFINE_TIMER(timeout_timer, timeout_handler, 0, 0);
static void timeout_handler(unsigned long data)
{
    del_timer_sync(&timeout_timer);
    hiir_dev.repkey_checkflag = 0;
}
#endif
#if 0
/*slac IO interrutp*/
static irqreturn_t slac_ioirq_handler(int irq, void *dev_id ,struct pt_regs * regs)
{
	if(SLAC_IOINT_STATUS())
	{
        wake_up_interruptible(&slac_wait);
        SLAC_IOINT_CLEAR();
        return IRQ_HANDLED;
	}
	return IRQ_NONE;
}
#endif
static int slac_open(struct inode * inode, struct file * file)
{
    int ret;

    SLAC_LOCK();

    if (1 == atomic_inc_return(&open_cnt))
    {
        ret = hisi_slac_init();

        if(ret != VP_STATUS_SUCCESS)
        {
            atomic_dec(&open_cnt);
            SLAC_UNLOCK();

            /* BEGIN: Modified for kernel error when AIAO initialize since no SLIC chip*/
            return -1;
            /* END: Modified for kernel error when AIAO initialize since no SLIC chip*/
        }

#if 1
        /*BEGIN: Modified for sometimes SLIC not responce when be called again and again*/
        s32SlacTickWaitFlag = 0;
        init_waitqueue_head(&WaitQueueSlacTick);
        pSlacTickThread = kthread_create(slac_tick_proc, NULL, "SlacTick");
        if (IS_ERR(pSlacTickThread))
        {
            TRC("create kthread failed\n");
            atomic_dec(&open_cnt);
            SLAC_UNLOCK();
            return -1;
        }
        wake_up_process(pSlacTickThread);

		/*END: Modified for sometimes SLIC not responce when be called again and again*/

        init_timer(&slac_timer);
        slac_timer.function = &slac_timer_handle;
        slac_timer.expires = jiffies + msecs_to_jiffies(10);//10ms
        add_timer(&slac_timer);

        init_waitqueue_head(&slac_wait);
#endif

    }
    SLAC_UNLOCK();

    return 0;
}

static int slac_release(struct inode *inode ,struct file *file)
{
    s32SlacTickWaitFlag = -1;   //avoid slac_tick_proc SLAC_LOCK deadlock
    SLAC_LOCK();


    if (atomic_dec_and_test(&open_cnt))
    {
#if 1
        del_timer_sync(&slac_timer);

		/*BEGIN: Modified for sometimes SLIC not responce when be called again and again*/
        s32SlacTickWaitFlag = -1;
        wake_up_interruptible(&WaitQueueSlacTick);

        kthread_stop(pSlacTickThread);

        pSlacTickThread = NULL;

        /*END: Modified for sometimes SLIC not responce when be called again and again*/
#endif
    }
    SLAC_UNLOCK();

    return 0;
}

static ssize_t slac_read(struct file *file, char __user *buf, size_t buflen, loff_t *ppos)
{
    READ_SLAC_TYPE read_buf;

    SLAC_LOCK();

    if((wake_handle_type == 0) || (buflen != sizeof(READ_SLAC_TYPE)))
    {
        SLAC_UNLOCK();
        return -1;
    }
    VpMemCpy(&(read_buf.pSlacEvent), &pSlacEvent, sizeof(VpEventType));
    read_buf.pcadencePtrs = cadencePtrs;
    read_buf.ptonePtrs = tonePtrs;
    wake_handle_type = 0;
    if(copy_to_user(buf, (unsigned char *)&read_buf, buflen))
    {
        SLAC_UNLOCK();
        return -1;
    }
    SLAC_UNLOCK();
    return 0;
}
static ssize_t slac_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
    SLAC_LOCK();

    SLAC_UNLOCK();
    return 0;
}

typedef struct VpInitLine_TYPE{
    VpLineCtxType *pLineCtx;
    VpProfilePtrType pAcProfile;
    VpProfilePtrType pDcFeedOrFxoCfgProfile;
    VpProfilePtrType pRingProfile;
}VpInitLine_TYPE;

static int DoVpInitLine(unsigned long arg)
{
#if 0
    VpInitLine_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpInitLine_TYPE)) == 0 )
    {
        status = VpInitLine(temp.pLineCtx, temp.pAcProfile, temp.pDcFeedOrFxoCfgProfile, temp.pRingProfile);
        TRC( "VpInitLine completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpConfigLine_TYPE{
    VpLineCtxType *pLineCtx;
    VpProfilePtrType pAcProfile;
    VpProfilePtrType pDcFeedOrFxoCfgProfile;
    VpProfilePtrType pRingProfile;
}VpConfigLine_TYPE;

static int DoVpConfigLine(unsigned long arg)
{
#if 0
    VpConfigLine_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpConfigLine_TYPE)) == 0 )
    {
        status = VpConfigLine(temp.pLineCtx, temp.pAcProfile, temp.pDcFeedOrFxoCfgProfile, temp.pRingProfile);
        TRC( "VpConfigLine completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpCalCodec_TYPE{
    VpLineCtxType *pLineCtx;
    VpDeviceCalType mode;
}VpCalCodec_TYPE;

static int DoVpCalCodec(unsigned long arg)
{
#if 0
    VpCalCodec_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpCalCodec_TYPE)) == 0 )
    {
        status = VpCalCodec(temp.pLineCtx, temp.mode);
        TRC( "VpCalCodec completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpInitRing_TYPE{
    VpLineCtxType *pLineCtx;
    VpProfilePtrType pCadProfile;
    VpProfilePtrType pCidProfile;
}VpInitRing_TYPE;

static int DoVpInitRing(unsigned long arg)
{
#if 0
    VpInitRing_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpInitRing_TYPE)) == 0 )
    {
        status = VpInitRing(temp.pLineCtx, temp.pCadProfile, temp.pCidProfile);
        TRC( "VpInitRing completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpInitCid_TYPE{
    VpLineCtxType *pLineCtx;
    uint8 length;
    uint8p pCidData;
}VpInitCid_TYPE;

static int DoVpInitCid(unsigned long arg)
{

    VpInitCid_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpInitCid_TYPE)) == 0 )
    {
        status = VpInitCid(&lineCtx, temp.length, temp.pCidData);
        TRC( "VpInitCid completed status = 0x%x.\n", status);
        return(status);
    }
    return(-1);
}

typedef struct VpInitMeter_TYPE{
    VpLineCtxType *pLineCtx;
    VpProfilePtrType pMeterProfile;
}VpInitMeter_TYPE;

static int DoVpInitMeter(unsigned long arg)
{
#if 0
    VpInitMeter_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpInitMeter_TYPE)) == 0 )
    {
        status = VpInitMeter(temp.pLineCtx, temp.pMeterProfile);
        TRC( "VpInitMeter completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpInitProfile_TYPE{
    VpDevCtxType *pDevCtx;
    VpProfileType type;
    VpProfilePtrType pProfileIndex;
    VpProfilePtrType pProfile;
}VpInitProfile_TYPE;

static int DoVpInitProfile(unsigned long arg)
{
#if 0
    VpInitProfile_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpInitProfile_TYPE)) == 0 )
    {
        status = VpInitProfile(temp.pDevCtx, temp.type, temp.pProfileIndex, temp.pProfile);
        TRC( "VpInitProfile completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpSetLineState_TYPE{
    VpLineCtxType *pLineCtx;
    VpLineStateType state;
}VpSetLineState_TYPE;

static int DoVpSetLineState(unsigned long arg)
{
     VpSetLineState_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpSetLineState_TYPE)) == 0 )
    {
        status = VpSetLineState(&lineCtx, temp.state);
        //TRC( "VpSetLineState completed status = 0x%x.\n", status);
        return(status);
    }
    return(-1);
}
typedef struct VpSetLineTone_TYPE{
    VpLineCtxType *pLineCtx;
    int pToneProfile;
    int pCadProfile;
    VpDtmfToneGenType *pDtmfControl;
}VpSetLineTone_TYPE;

static int DoVpSetLineTone(unsigned long arg)
{
    VpSetLineTone_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpSetLineTone_TYPE)) == 0 )
    {
        if((temp.pToneProfile == VP_NULL_FF)&& (temp.pCadProfile == VP_NULL_FF))
        {
            status = VpSetLineTone(&lineCtx, VP_PTABLE_NULL, VP_PTABLE_NULL, temp.pDtmfControl);
        }
        else
        {
            status = VpSetLineTone(&lineCtx, tonePtrs[temp.pToneProfile], cadencePtrs[temp.pCadProfile], temp.pDtmfControl);
        }
        TRC( "VpSetLineTone completed status = 0x%x.\n", status);
        return(status);
    }
    return(-1);
}

typedef struct VpSetRelayState_TYPE{
    VpLineCtxType *pLineCtx;
    VpRelayControlType rState;
}VpSetRelayState_TYPE;

static int DoVpSetRelayState(unsigned long arg)
{
#if 0
    VpSetRelayState_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpSetRelayState_TYPE)) == 0 )
    {
        status = VpSetRelayState(temp.pLineCtx, temp.rState);
        TRC( "VpSetRelayState completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpSendSignal_TYPE{
    VpLineCtxType *pLineCtx;
    VpSendSignalType signalType;
    void *pSignalData;
}VpSendSignal_TYPE;

static int DoVpSendSignal(unsigned long arg)
{
#if 0
    VpSendSignal_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpSendSignal_TYPE)) == 0 )
    {
        status = VpSendSignal(temp.pLineCtx, temp.signalType, temp.pSignalData);
        TRC( "VpSendSignal completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpSendCid_TYPE{
    VpLineCtxType *pLineCtx;
    uint8 length;
    VpProfilePtrType pCidProfile;
    uint8p pCidData;
}VpSendCid_TYPE;

static int DoVpSendCid(unsigned long arg)
{
#if 0
    VpSendCid_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpSendCid_TYPE)) == 0 )
    {
        status = VpSendCid(temp.pLineCtx, temp.length, temp.pCidProfile, temp.pCidData);
        TRC( "VpSendCid completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpStartMeter_TYPE{
    VpLineCtxType *pLineCtx;
    uint16 onTime;
    uint16 offTime;
    uint16 numMeters;
}VpStartMeter_TYPE;

static int DoVpStartMeter(unsigned long arg)
{
#if 0
    VpStartMeter_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpStartMeter_TYPE)) == 0 )
    {
        status = VpStartMeter(temp.pLineCtx, temp.onTime, temp.offTime, temp.numMeters);
        TRC( "VpStartMeter completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpSetOption_TYPE{
    VpLineCtxType *pLineCtx;
    VpDevCtxType *pDevCtxParam;
    VpOptionIdType optionId;
    void *value;
}VpSetOption_TYPE;

static int DoVpSetOption(unsigned long arg)
{
#if 0
    VpSetOption_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpSetOption_TYPE)) == 0 )
    {
        status = VpSetOption(temp.pLineCtx, temp.pDevCtxParam, temp.optionId, temp.value);
        TRC( "VpSetOption completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpDeviceIoAccess_TYPE{
    VpDevCtxType *pDevCtx;
    VpDeviceIoAccessDataType *pDeviceIoData;
}VpDeviceIoAccess_TYPE;

static int DoVpDeviceIoAccess(unsigned long arg)
{
#if 0
    VpDeviceIoAccess_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpDeviceIoAccess_TYPE)) == 0 )
    {
        status = VpDeviceIoAccess(temp.pDevCtx, temp.pDeviceIoData);
        TRC( "VpDeviceIoAccess completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpGetEvent_TYPE{
    VpDevCtxType *pDevCtx;
    VpEventType *pEvent;
}VpGetEvent_TYPE;

static int DoVpGetEvent(unsigned long arg)
{
#if 0
    VpGetEvent_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpGetEvent_TYPE)) == 0 )
    {
        status = VpGetEvent(temp.pDevCtx, temp.pEvent);
        TRC( "VpGetEvent completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpGetLineStatus_TYPE{
    VpLineCtxType *pLineCtx;
    VpInputType input;
    bool *pStatus;
}VpGetLineStatus_TYPE;

static int DoVpGetLineStatus(unsigned long arg)
{
#if 0
    VpGetLineStatus_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpGetLineStatus_TYPE)) == 0 )
    {
        status = VpGetLineStatus(temp.pLineCtx, temp.input, temp.pStatus);
        TRC( "VpGetLineStatus completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}

static int DoVpGetLineState(unsigned long arg)
{
#if 1
    VpSetLineState_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpSetLineState_TYPE)) == 0)
    {
        status = VpGetLineState(&lineCtx, &temp.state);
        if(copy_to_user((void *)arg, &temp, sizeof(VpSetLineState_TYPE)))
        {
            return -1;
        }
        //TRC( "VpGetLineState completed status = 0x%x.\n", status);
        return (status);
    }
#endif

    return (-1);
}
typedef struct VpFlushEvents_TYPE{
    VpDevCtxType *pDevCtx;
}VpFlushEvents_TYPE;

static int DoVpFlushEvents(unsigned long arg)
{
#if 0
    VpFlushEvents_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpFlushEvents_TYPE)) == 0 )
    {
        status = VpFlushEvents(temp.pDevCtx);
        TRC( "VpFlushEvents completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpGetResults_TYPE{
    VpEventType *pEvent;
    void *pResults;
}VpGetResults_TYPE;

static int DoVpGetResults(unsigned long arg)
{
#if 0
    VpGetResults_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpGetResults_TYPE)) == 0 )
    {
        status = VpGetResults(temp.pEvent, temp.pResults);
        TRC( "VpGetResults completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpClearResults_TYPE{
    VpDevCtxType *pDevCtx;
}VpClearResults_TYPE;

static int DoVpClearResults(unsigned long arg)
{
#if 0
    VpClearResults_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpClearResults_TYPE)) == 0 )
    {
        status = VpClearResults(temp.pDevCtx);
        TRC( "VpClearResults completed status = 0x%x.\n", status);
        return(status);
    }
#endif
    return(-1);
}
typedef struct VpLowLevelCmd_TYPE{
    VpLineCtxType *pLineCtx;
    uint8 *pCmdData;
    uint8 len;
    uint16 handle;
}VpLowLevelCmd_TYPE;

static int DoVpLowLevelCmd(unsigned long arg)
{
    VpLowLevelCmd_TYPE __user *argp = (VpLowLevelCmd_TYPE __user *)arg;
    VpLowLevelCmd_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;
    Vp890DeviceObjectType   *pDevObj  = devCtx.pDevObj;
    if (copy_from_user(&temp, (void *)arg, sizeof(VpLowLevelCmd_TYPE)) == 0)
    {
        status = VpLowLevelCmd(&lineCtx, temp.pCmdData, temp.len, temp.handle);
        VpMemCpy(temp.pCmdData, &(pDevObj->mpiData[0]), temp.len);
        if(copy_to_user(argp, &temp, sizeof(VpLowLevelCmd_TYPE)))
        {
            return -1;
        }
//        TRC( "VpLowLevelCmd completed status = 0x%x.\n", status);
        return(status);
    }
    return(-1);
}
typedef struct VpContinueCid_TYPE{
    VpLineCtxType *pLineCtx;
    uint8 length;
    uint8p pCidData;
}VpContinueCid_TYPE;

static int DoVpContinueCid(unsigned long arg)
{

    VpContinueCid_TYPE temp;
    VpStatusType status = VP_STATUS_SUCCESS;

    if (copy_from_user(&temp, (void *)arg, sizeof(VpContinueCid_TYPE)) == 0 )
    {
        status = VpInitCid(&lineCtx, temp.length, temp.pCidData);
        TRC( "VpContinueCid completed status = 0x%x.\n", status);
        return(status);
    }
    return(-1);
}
static long slac_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
   int ret = -EINVAL;

   if(cmd >= 0 && cmd < IOCTL_MAX_INDEX)
   {
        static FN_IOCTL IoctlFuncs[] = {
    		DoVpInitLine,
    		DoVpConfigLine,
    		DoVpCalCodec,
    		DoVpInitRing,
    		DoVpInitCid,
    		DoVpInitMeter,
    		DoVpInitProfile,

    		DoVpSetLineState,
    		DoVpSetLineTone,
    		DoVpSetRelayState,
    		DoVpSendSignal,
    		DoVpSendCid,
    		DoVpStartMeter,
    		DoVpSetOption,
    		DoVpDeviceIoAccess,

    		DoVpGetEvent,
    		DoVpGetLineStatus,
    		DoVpGetLineState,
    		DoVpFlushEvents,
    		DoVpGetResults,
    		DoVpClearResults,
    		DoVpLowLevelCmd,
    		DoVpContinueCid,
        };
        unsigned int cmdnr = _IOC_NR(cmd);

        SLAC_LOCK();
		ret = (*IoctlFuncs[cmdnr]) (arg);
        SLAC_UNLOCK();
		return (long)ret;
   }
   else
   {
        unsigned int __user *argp = (unsigned int __user *)arg;
        SLAC_REG_TYPE temp;
        Vp890DeviceObjectType   *pDevObj  = devCtx.pDevObj;
        unsigned int i;

        SLAC_LOCK();
        if(argp != NULL)
        {
            if(copy_from_user(&temp,argp,sizeof(SLAC_REG_TYPE)))
            {
                SLAC_UNLOCK();
                return -EFAULT;
            }
        }
        switch(cmd)
        {
            case SLAC_INITIAL:
                //    hisi_slac_init();
                break;
            case SLAC_REG_READ:
                if(temp.mpireg & 0x01)
                {
                    temp.status = VpLowLevelCmd(&lineCtx, &(temp.mpireg), temp.mpiLen, 0);
                    VpMemCpy(temp.mpiData, &(pDevObj->mpiData[0]), temp.mpiLen);
                    TRC("slac ioctl: read_status = 0x%x; len = 0x%x; register(0x%x) =\n", temp.status, temp.mpiLen, temp.mpireg);
                    for(i = 0; i < temp.mpiLen; i++)
                    {
                        TRC("slac ioctl: = 0x%x ", temp.mpiData[i]);
                    }
                    TRC("slac ioctl end\n");
                    if(copy_to_user(argp, &temp, sizeof(SLAC_REG_TYPE)))
                    {
                        TRC("slac ioctl: A error is occured when copy_to_user return read value!!\n");
                    }
                }
                else
                {
                    TRC("slac ioctl: register address is not an address which can be readed.\n");
                }
                break;
            case SLAC_REG_WRITE:
                if((temp.mpireg & 0x01) == 0)
                {
                    temp.status = VpLowLevelCmd(&lineCtx, &(temp.mpireg), temp.mpiLen, 0);
                    TRC("slac ioctl: write stauts = 0x%x end\n", temp.status);
                }
                else
                {
                    TRC("slac ioctl: register address is not an address which can be readed.\n");
                }
                break;
#if 0
    	    case SIO0_REG_DUMP:
    	        SEND_EDGE_DOWN();
                TRC("slac ioctl: SIO0_MODE = 0x%x ;\n", HI_REG(SIO0_MODE));
                TRC("slac ioctl: PCM_CT_SET = 0x%x ;\n", HI_REG(PCM_CT_SET));
                TRC("slac ioctl: I2S_CT_SET = 0x%x ;\n", HI_REG(I2S_CT_SET));
                TRC("slac ioctl: SIO_SIGNED_EXT = 0x%x ;\n", HI_REG(SIO_SIGNED_EXT));
                TRC("slac ioctl: I2S_ICD = 0x%x ;\n", HI_REG(I2S_ICD));
    		    break;
#endif
            default:
                break;
        }
        SLAC_UNLOCK();
        return 0;
    }
}


static struct file_operations slac_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= slac_ioctl,
	.open			= slac_open,
	.release		= slac_release,
	.read       	= slac_read,
	.write      	= slac_write,
	.poll       	= slac_select
};

static struct miscdevice slac_device_major =
{
    MISC_DYNAMIC_MINOR,
    SLAC_DEVICE_NAME,
    &slac_fops,
};

int slac_init(void)
{
    int res = 0;
    DRV_PROC_ITEM_S  *pProcItem;

    TRC("Slac driver version : %d.%d.%d\n",VP_API_VERSION_MAJOR_NUM,VP_API_VERSION_MINOR_NUM,VP_API_VERSION_MINI_NUM);

    //  hisi_slac_init();

//    TRC(KERN_INFO OSDRV_MODULE_VERSION_STRING "\n");
    //res = register_chrdev(slac_device_major, SLAC_DEVICE_NAME, &slac_fops);
    res = misc_register(&slac_device_major);
    if(res < 0)
    {
        TRC(KERN_DEBUG "SLAC Error: can't register\n");
        res = -EAGAIN;
        return res;
    }
    #if 0
    if(0 == slac_device_major)
    {
        slac_device_major = res;
    }
    #endif
#if 0
    res = request_irq(SLAC_IOIRQ_NO, &slac_ioirq_handler, SA_SHIRQ|SA_INTERRUPT, SLAC_DEVICE_NAME, &slac_ioirq_handler);
    if(res)
    {
        TRC(KERN_DEBUG "SLAC Error: request IRQ(%d) failed\n", SLAC_IOIRQ_NO);
        unregister_chrdev(slac_device_major, SLAC_DEVICE_NAME);
        return res;
    }
#endif
/*
    init_timer(&slac_timer);
    slac_timer.function = &slac_timer_handle;
    slac_timer.expires = jiffies + SLAC_TIMER_LOOP;//10ms
    add_timer(&slac_timer);

    init_waitqueue_head(&slac_wait);
*/
    #ifdef CONFIG_DEVFS_FS
        devfs_mk_cdev(MKDEV(slac_device_major, 0), S_IFCHR|S_IRUGO|S_IWUSR, SLAC_DEVICE_NAME);
    #endif

    /*register SLAC PROC*/
    pProcItem = HI_DRV_PROC_AddModule("slac", NULL, NULL);
    if (!pProcItem)
    {
        TRC("add SLAC proc failed.\n");
        misc_deregister(&slac_device_major);
        return (-EAGAIN);
    }
    pProcItem->read = Slac_ProcRead;
    pProcItem->write = Slac_ProcWrite;
#ifdef MODULE
    TRC("Load hi_slic.ko success. res=%d  \t(%s)\n", res, VERSION_STRING);
#endif

    return 0;
}

void slac_exit(void)
{
   HI_DRV_PROC_RemoveModule("slac");
//    del_timer_sync(&slac_timer);
#if 0
    free_irq(SLAC_IOIRQ_NO, &slac_ioirq_handler);
#endif
    #ifdef CONFIG_DEVFS_FS
        devfs_remove(SLAC_DEVICE_NAME);
    #endif

    //unregister_chrdev(slac_device_major, SLAC_DEVICE_NAME);
    misc_deregister(&slac_device_major);
    TRC("SLAC release ok. \n");
}

#ifdef MODULE
module_init(slac_init);
module_exit(slac_exit);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("WISH SLAC Device Driver");
MODULE_AUTHOR("HISILICON");

