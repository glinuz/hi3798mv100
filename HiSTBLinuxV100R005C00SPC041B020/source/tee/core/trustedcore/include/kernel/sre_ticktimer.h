#ifndef __TIMER_FRAME_WORK_
#define __TIMER_FRAME_WORK_

#include "sre_base.h"
#include "sre_list.ph"

#define NSECS_PER_SEC   1000000000L
#define MSECS_PER_SEC   1000
/*Seconds will be set to maximum value and the number of nanoseconds
 * will be zero */
#define TIMEVAL_MAX     (((INT64)~((UINT64)1 << 63)) & (~((UINT64)0xFFFFFFFF)))
#define MAX_NUM_OF_TIMERS 2

/*The timer event is Inactive*/
#define    TIMER_STATE_INACTIVE     0x00
/* The timer event is active and is waiting for expiration*/
#define    TIMER_STATE_ACTIVE       0x01
/*The timer is expired and is waiting on the callback list to be executed*/
#define    TIMER_STATE_PENDING      0x02
/* The timer event is currently being executed */
#define    TIMER_STATE_EXECUTING    0x04

#define EPOCH_YEAR      1970
#define SECSPERMIN      60
#define MINSPERHOUR     60
#define HOURSPERDAY     24
#define DAYSPERWEEK     7
#define DAYSPERNYEAR    365
#define DAYSPERLYEAR    366
#define SECSPERHOUR     (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY      (SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR     12

#define INT_MAX 0x7fffffff  /* max value for an int */
#define INT_MIN (-0x7fffffff-1) /* min value for an int */

#define CMD_TIMER_GENERIC  0xDDEA
#define CMD_TIMER_RTC      0xDDEB

typedef struct {
	INT32 seconds;
	INT32 millis;
} TEE_Time_kernel;

typedef struct {
	INT32 seconds;
	INT32 millis;
	INT32 min;
	INT32 hour;
	INT32 day;
	INT32 month;
	INT32 year;
} TEE_Date_Time_kernel;

static const int mon_lengths[2][MONSPERYEAR] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static const int year_lengths[2] = {
	DAYSPERNYEAR, DAYSPERLYEAR
};

#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))


/**
 * @brief
 *  Structure to represent the time
 *  in High resolution format( Resolution in Nano seconds)
 */
typedef union {
	INT64 tval64;
	struct {
		INT32 nsec, sec;
	} tval;
} timeval_t;

/**
* @brief
*/
enum timer_callback_mode {
	/* The handler function should be run in softirq*/
	TIMER_CALLBACK_SOFTIRQ,
	/* The handler function should be run in hardirq context itself*/
	TIMER_CALLBACK_HARDIRQ,
	/* The handler function should be executed in hardirq and it should not
	 * restart the timer*/
	TIMER_CALLBACK_HARDIRQ_NORESTART
};

/**
* @brief
*/
enum timer_cbfn_return_value {
	TIMER_RESTART,
	TIMER_NORESTART
};

/**
* @brief
*/
enum timer_class_type {
	/* timer event using timer10*/
	TIMER_GENERIC,
	/* timer event using RTC*/
	TIMER_RTC,
	TIMER_CLASSIC,
	/* timer event for AntiRoot TA */
	TIMER_ANTIROOT
};


/**
* @brief
*/
struct timer_clock_info {
	struct timer_cpu_info *cpu_info;
	int    clock_id;
	//struct spinlock spin_lock;
	LIST_OBJECT_S active; //list for active timer event
	LIST_OBJECT_S avail;  //list for created timer event
	timeval_t   clock_period;
	timeval_t   timer_period;
	int shift;
	UINT32 mult;
};

/**
* @brief
*/
struct timer_cpu_info {
	struct timer_clock_info clock_info[MAX_NUM_OF_TIMERS];
	timeval_t expires_next;
	LIST_OBJECT_S pending_routines;
	UINT32 num_events;
	int free_run_clock_id;
	int tick_timer_id;
};

extern struct timer_cpu_info g_timer_cpu_info;

typedef struct {
	UINT32 timeLow;
	UINT16 timeMid;
	UINT16 timeHiAndVersion;
	UINT8 clockSeqAndNode[8];
} tee_uuid_kernel;


/*attention:
* timer_private_data_kernel must be the same to TEE:timer_event_private_data
*/
typedef struct {
	UINT32 dev_id;
	tee_uuid_kernel uuid;
	UINT32 session_id;
	UINT32 type;
	UINT32 expire_time;
	UINT32 notify_data_addr;
} timer_private_data_kernel;

typedef struct {
	UINT32 type;      //定时器类型
	UINT32 timer_id;  //定时器ID
	UINT32 timer_class;
	UINT32 reserved2;
} timer_attr_data_kernel;

typedef struct {
	UINT32 dev_id;
	tee_uuid_kernel uuid;
	UINT32 session_id;
	timer_attr_data_kernel property;
	UINT32 expire_time;
} timer_notify_data_kernel;

/**
* @brief
*/
typedef struct stTimerEvent {
	LIST_OBJECT_S node; //node for active timer event
	LIST_OBJECT_S callback_entry;
	LIST_OBJECT_S c_node; //node for created timer event

	timeval_t   expires;
	struct timer_clock_info *clk_info;
	int (*handler)(void *);
	int state;
	int callback_mode;
	int timer_class;         //0:timer60, 1:RTC
	timer_private_data_kernel timer_attr;
	void *data;
} timer_event;

/**
 * @brief
 * The elements of this structure are useful
 * for implementing the sw_timer
 */
typedef struct stSwtmrinfo {

	timeval_t sw_timestamp;
	UINT64 abs_cycles_count;
	UINT64 cycles_count_old;
	UINT64 cycles_count_new;

	timeval_t timer_period;
	timeval_t clock_period;
} stSwtmrinfo;

typedef UINT32(*sw_timer_event_handler)(timer_event *);

/**
 * @ingroup sre_timer
 * SRE_timer错误码: 指针参数为空。
 *
 * 值: 0x02002e01
 *
 * 解决方案: 查看入参指针是否为空
 */
#define OS_ERRNO_TIMER_INPUT_PTR_NULL                           SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x01)

/**
 * @ingroup sre_timer
 * SRE_timer错误码: 定时器回调函数为空。
 *
 * 值: 0x02002e02
 *
 * 解决方案: 查看定时器回调函数是否为空
 */
#define OS_ERRNO_TIMER_PROC_FUNC_NULL                           SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x02)

/**
 * @ingroup sre_timer
 * SRE_timer错误码: 定时器句柄非法。
 *
 * 值: 0x02002e03
 *
 * 解决方案: 检查输入的定时器句柄是否正确。
 */
#define OS_ERRNO_TIMER_HANDLE_INVALID                           SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x03)

/**
 * @ingroup sre_timer
 * SRE_timer错误码: 定时器定时时间参数非法。
 *
 * 值: 0x02002e04
 *
 * 解决方案: 定时器定时时间参数非法。
 */
#define OS_ERRNO_TIMER_INTERVAL_INVALID                         SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x04)

/**
 * @ingroup sre_timer
 * SRE_timer错误码: 定时器工作模式参数非法。
 *
 * 值: 0x02002e05
 *
 * 解决方案: 查看定时器工作模式参数是否正确，参照工作模式配置枚举定义#TIMER_MODE_E。
 */
#define OS_ERRNO_TIMER_MODE_INVALID                             SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x05)

/**
 * @ingroup sre_timer
 * SRE_timer错误码: 定时器类型参数非法。
 *
 * 值: 0x02002e06
 *
 * 解决方案: 查看定时器类型参数是否正确，参照类型配置枚举定义#TIMER_TYPE_E。
 */
#define OS_ERRNO_TIMER_CREATE_INVALID                             SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x06)

/**
 * @ingroup sre_timer
 * SRE_timer错误码: 定时器类型参数非法。
 *
 * 值: 0x02002e06
 *
 * 解决方案: 查看定时器类型参数是否正确，参照类型配置枚举定义#TIMER_TYPE_E。
 */
#define OS_ERRNO_DELAY_MSEC_VALUE_INVALID                             SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x07)

/**
 * @ingroup sre_timer
 * SRE_timer错误码: 定时器类型参数非法。
 *
 * 值: 0x02002e06
 *
 * 解决方案: 查看定时器类型参数是否正确，参照类型配置枚举定义#TIMER_TYPE_E。
 */
#define OS_ERRNO_DELAY_SEC_VALUE_INVALID                             SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x08)

#define OS_ERRNO_EVENT_DESTROY_FAILED				     SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x09)

#define OS_ERRNO_TIMER_EVENT_NOT_AVAILABLE			     SRE_ERRNO_OS_ERROR(OS_MID_TIMER, 0x0a)

//#define KERNEL_TIME_DEBUG
#ifdef KERNEL_TIME_DEBUG
#define KERNEL_TIME_DEBUG_TAG "[kernel_time_debug]"
#define KERNEL_TIME_INFO(fmt, args...) uart_printf_func("%s %s: " fmt "", KERNEL_TIME_DEBUG_TAG, __FUNCTION__, ## args)
#else
#define KERNEL_TIME_INFO(fmt, args...)
#endif

timer_event *SRE_TimerEventCreate(sw_timer_event_handler handler, int timer_class, void *priv_data);
INT32 SRE_TimerEventDestroy(timer_event *pstTevent);
UINT32 SRE_TimerEventStart(timer_event *pstTevent, timeval_t *time);
VOID SRE_FreeRunningCntrIntr(VOID);
UINT64 SRE_ReadTimestamp(VOID);
VOID SRE_TimerInterrupt(VOID);
VOID SRE_InitSwTimer(VOID);
VOID SRE_TimerEventStop(timer_event *pstTevent);
UINT64 SRE_TimerGetExpire(timer_event *pstTevent);
void get_sys_date_time(TEE_Date_Time_kernel *time_date);
#endif
