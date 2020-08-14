/*
 * blackbox header file (blackbox: kernel run data recorder.)
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __BB_PUB_H__
#define __BB_PUB_H__

#include <linux/module.h>

#include <linux/hisi/rdr_types.h>
#define STR_MODULENAME_MAXLEN		16
#define STR_EXCEPTIONDESC_MAXLEN	48
#define STR_TASKNAME_MAXLEN		16
#define STR_USERDATA_MAXLEN		64

#define PATH_ROOT           "/data/hisi_logs/"

#define INT_IN_FLAG             0xAAAAUL
#define INT_EXIT_FLAG           0xBBBBUL

#define BBOX_SAVE_DONE_FILENAME "/DONE"    /*异常文件目录log保存完毕的标志文件名字*/

/*异常时，log保存完毕的标志*/
enum SAVE_STEP {
    BBOX_SAVE_STEP1     = 0x1,
    BBOX_SAVE_STEP2     = 0x2,
    BBOX_SAVE_STEP3     = 0x3,
    BBOX_SAVE_STEP_DONE = 0x100
};

enum CORE_LIST {
    RDR_AP      = 0x1,
    RDR_CP      = 0x2,
    RDR_TEEOS   = 0x4,
    RDR_HIFI    = 0x8,
    RDR_LPM3    = 0x10,
    RDR_IOM3    = 0x20,
    RDR_ISP     = 0x40,
    RDR_IVP     = 0x80,
    RDR_EMMC    = 0x100,
    RDR_CORE_MAX = 9
};

enum RDR_DUMPCTRL_NVE {
    RDR_DCTRL_AP        = 0x0,
    RDR_DCTRL_CP        = 0x1,
    RDR_DCTRL_TEEOS     = 0x2,
    RDR_DCTRL_HIFI      = 0x3,
    RDR_DCTRL_LPM3      = 0x4,
    RDR_DCTRL_IOM3      = 0x5,
    RDR_DCTRL_ISP       = 0x6,
    RDR_DCTRL_IVP       = 0x7,
    RDR_DCTRL_MAX       = 0x20
};

/*this is for test*/
enum rdr_except_reason_e {
        RDR_EXCE_WD         = 0x01,/*watchdog timeout*/
        RDR_EXCE_INITIATIVE,       /*initictive call sys_error*/
        RDR_EXCE_PANIC,            /*ARM except(eg:data abort)*/
        RDR_EXCE_STACKOVERFLOW,
        RDR_EXCE_DIE,
        RDR_EXCE_UNDEF,
        RDR_EXCE_MAX
};

/*Add, please keep the same as definition in reboot_reason.h in fastboot !!!!*/
typedef enum
{
    AP_S_COLDBOOT      = 0x0,
    BOOTLOADER         = 0x01,
    RECOVERY           = 0x02,
    RESETFACTORY       = 0x03,
    RESETUSER          = 0x04,
    SDUPDATE           = 0x05,
    CHARGEREBOOT       = 0x06,
    RESIZE             = 0x07,
    ERECOVERY          = 0x08,
    USBUPDATE          = 0x09,
    CUST               = 0x0a,
    USERSDUPDATE       = 0x0b,
    OEM_RTC            = 0x0c,
    RESERVED5          = 0x0d,
    MOUNTFAIL          = 0x0e,
    HUNGDETECT         = 0x0f,
    COLDBOOT           = 0x10,
    RESERVED1          = 0x11,
    AP_S_FASTBOOTFLASH = 0x13,
    AP_S_ABNORMAL      = 0x14,
    AP_S_TSENSOR0      = 0x15,
    AP_S_TSENSOR1      = 0x16,
    AP_S_AWDT          = 0x17,
    LPM3_S_GLOBALWDT   = 0x18,
    G3D_S_G3DTSENSOR   = 0x19,
    LPM3_S_LPMCURST    = 0x1a,
    CP_S_CPTSENSOR     = 0x1b,
    IOM3_S_IOMCURST    = 0x1c,
    ASP_S_ASPWD        = 0x1d,
    CP_S_CPWD          = 0x1e,
    IVP_S_IVPWD        = 0x1f,
    ISP_S_ISPWD        = 0x20,
    AP_S_DDR_SELFREFLASH = 0x21,
    AP_S_DDR_FATAL_INTER = 0X22,
    RESERVED2          = 0x23,
    AP_S_PANIC         = 0x24,
    AP_S_NOC           = 0x25,
    AP_S_PMU           = 0x26,
    AP_S_DDRC_SEC      = 0x27,
    AP_S_GPIO          = 0x28,
    AP_S_COMBINATIONKEY = 0x29,
    AP_S_DIE           = 0x2a,
    AP_S_MAILBOX       = 0x2b,
    AP_S_CSI           = 0x2c,
    AP_S_PAM           = 0x2d,
    RESERVED3          = 0x2e,
    CP_S_EXCEPTION     = 0x2f,
    CP_S_RESETFAIL     = 0x30,
    CP_S_NORMALRESET   = 0x31,
    LPM3_S_EXCEPTION   = 0x32,
    HIFI_S_EXCEPTION   = 0x33,
    HIFI_S_RESETFAIL   = 0x34,
    ISP_S_EXCEPTION    = 0x35,
    IVP_S_EXCEPTION    = 0x36,
    IOM3_S_EXCEPTION   = 0x37,
    TEE_S_EXCEPTION    = 0x38,
    MMC_S_EXCEPTION    = 0x39,
    RESERVED4          = 0x40,
    BR_KEY_VOLUMN_DOWN_UP_UPDATE_USB      = 0x41,
    BR_KEY_VOLUMN_DOWN_UP_UPDATE_SD_FORCE = 0x42,
    BR_KEY_VOLUMN_UP                      = 0x43,
    BR_KEY_POWERON_PRESS_1S               = 0x44,
    BR_KEY_POWERON_PRESS_10S              = 0x45,
    BR_CHECKPOINT_SDUPDATE                = 0x46,
    BR_CHECKPOINT_USBUPDATE               = 0x47,
    BR_CHECKPOINT_RESETFACTORY            = 0x48,
    BR_CHECKPOINT_HOTAUPDATE              = 0x49,
    BR_CHECKPOINT_USERSDUPDATE            = 0x4a,
    BR_POWERON_BY_USB_NO_BAT              = 0x4b,
    BR_NOGUI                              = 0x4c,
    BR_FACTORY_VERSION                    = 0x4d,
    BR_RESET_HAPPEN                       = 0x4e,
    BR_POWEROFF_ALARM                     = 0x4f,
    BR_POWEROFF_CHARGE                    = 0x50,
    BR_POWERON_BY_SMPL                    = 0x51,
} EXCH_SOURCE;

enum PROCESS_PRI {
    RDR_ERR      = 0x01,
    RDR_WARN,
    RDR_OTHER,
    RDR_PPRI_MAX
};

enum REBOOT_PRI {
    RDR_REBOOT_NOW      = 0x01,
    RDR_REBOOT_WAIT,
    RDR_REBOOT_NO,
    RDR_REBOOT_MAX
};

enum REENTRANT {
    RDR_REENTRANT_ALLOW = 0xff00da00,
    RDR_REENTRANT_DISALLOW
};

enum UPLOAD_FLAG {
    RDR_UPLOAD_YES = 0xff00fa00,
    RDR_UPLOAD_NO
};


enum RDR_RETURN {
    RDR_SUCCESSED                   = 0x9f000000,
    RDR_FAILD                       = 0x9f000001,
    RDR_NULLPOINTER                 = 0x9f0000ff
};

typedef void (*rdr_e_callback)( u32, void* );

/*
 *   struct list_head   e_list;
 *   u32 modid,			    exception id;
 *		if modid equal 0, will auto generation modid, and return it.
 *   u32 modid_end,		    can register modid region. [modid~modid_end];
		need modid_end >= modid,
 *		if modid_end equal 0, will be register modid only,
		but modid & modid_end cant equal 0 at the same time.
 *   u32 process_priority,	exception process priority
 *   u32 reboot_priority,	exception reboot priority
 *   u64 save_log_mask,		need save log mask
 *   u64 notify_core_mask,	need notify other core mask
 *   u64 reset_core_mask,	need reset other core mask
 *   u64 from_core,		    the core of happen exception
 *   u32 reentrant,		    whether to allow exception reentrant
 *   u32 exce_type,		    the type of exception
 *   char* from_module,		    the module of happen excption
 *   char* desc,		        the desc of happen excption
 *   rdr_e_callback callback,	will be called when excption has processed.
 *   u32 reserve_u32;		reserve u32
 *   void* reserve_p		    reserve void *
 */
struct rdr_exception_info_s {
	struct list_head e_list;
	u32	e_modid;
	u32	e_modid_end;
	u32	e_process_priority;
	u32	e_reboot_priority;
	u64	e_notify_core_mask;
	u64	e_reset_core_mask;
	u64	e_from_core;
	u32	e_reentrant;
    u32 e_exce_type;
    u32 e_upload_flag;
	u8	e_from_module[MODULE_NAME_LEN];
	u8	e_desc[STR_EXCEPTIONDESC_MAXLEN];
	u32	e_reserve_u32;
	void*	e_reserve_p;
	rdr_e_callback e_callback;
};

/*
 * func name: pfn_cb_dump_done
 * func args:
 *    u32   modid
 *      exception id
 *    u64   coreid
 *      which core done
 * return value		null
 */
typedef void (*pfn_cb_dump_done)( u32 modid, u64 coreid);

/*
 * func name: pfn_dump
 * func args:
 *    u32   modid
 *      exception id
 *    u64   coreid
 *      exception core
 *    u32   etype
 *      exception type
 *    char*     logpath
 *      exception log path
 *    pfn_cb_dump_done fndone
 * return       mask bitmap.
 */
typedef void (*pfn_dump)( u32 modid, u32 etype, u64 coreid,
                char* logpath, pfn_cb_dump_done fndone);
/*
 * func name: pfn_reset
 * func args:
 *    u32   modid
 *      exception id
 *    u32   coreid
 *      exception core
 *    u32   e_type
 *      exception type
 * return value		null
 */
typedef void (*pfn_reset)( u32 modid, u32 etype, u64 coreid);

struct rdr_module_ops_pub {
    pfn_dump    ops_dump;
    pfn_reset   ops_reset;
};

struct rdr_register_module_result {
    u64   log_addr;
    u32     log_len;
    RDR_NVE nve;
};

#ifdef CONFIG_HISI_BB
/*
 * func name: rdr_register_exception_type
 * func args:
 *    struct rdr_exception_info_pub* s_e_type
 * return value		e_modid
 *	< 0 error
 *	>=0 success
 */
u32 rdr_register_exception(struct rdr_exception_info_s* e);

/*
 * func name: bb_unregister_exception_type
 * func args:
 *   u32 modid,			exception id;
 * return
 *	< 0 fail
 *	>=0 success
 * u32 bb_unregister_exception(u32 modid);
 */

/*
  * func name: hisi_bbox_map
  * func args:
  *    @paddr: physical address in black box
  *    @size: size of memory
  * return:
  *    success: virtual address
  *    fail: NULL or -ENOMEM
  */
void *hisi_bbox_map(phys_addr_t paddr, size_t size);

/*
  * func name: hisi_bbox_unmap
  * func args:
  *    @addr: virtual address that alloced by hisi_bbox_map
  */
void hisi_bbox_unmap(const void *vaddr);

/*
 * func name: rdr_register_module_ops
 * func args:
 *   u32 coreid,       core id;
 *      .
 *   struct rdr_module_ops_pub* ops;
 *   struct rdr_register_module_result* retinfo
 * return value		e_modid
 *	< 0 error
 *	>=0 success
 */
int rdr_register_module_ops(
        u64 coreid,
        struct rdr_module_ops_pub* ops,
        struct rdr_register_module_result* retinfo
		);

/*
 * func name: bb_unregister_exception_type
 * func args:
 *   u64 coreid,			core id;
 * return
 *	< 0 fail
 *	>=0 success
    u64 rdr_unregister_module_ops_info(u64 coreid);
 */

/*
 * func name: rdr_system_error
 * func args:
 *   u32 modid,			modid( must be registered);
 *   u32 arg1,			arg1;
 *   u32 arg2,			arg2;
 *   char *  data,			short message.
 *   u32 length,		len(IMPORTANT: <=4k)
 * return void
 */
void rdr_system_error(u32 modid, u32 arg1, u32 arg2);

void rdr_syserr_process_for_ap(u32 modid, u64 arg1, u64 arg2);
#else
static inline void *hisi_bbox_map(phys_addr_t paddr, size_t size){ return NULL; }
static inline u32 rdr_register_exception(struct rdr_exception_info_s* e){ return 0;}
static inline int rdr_register_module_ops(
        u64 coreid,
        struct rdr_module_ops_pub* ops,
        struct rdr_register_module_result* retinfo
		){ return -1; }
static inline void rdr_system_error(u32 modid, u32 arg1, u32 arg2){}
static inline void rdr_syserr_process_for_ap(u32 modid, u64 arg1, u64 arg2){}
#endif

void get_exception_info(unsigned long *buf, unsigned long *buf_len);
#define RDR_REBOOTDUMPINFO_FLAG     0xdd140607

#endif/* End #define __BB_PUB_H__ */

