/**
* @file sre_module.h
*
* Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
*
* ÃèÊö£ºÄ£¿éID±àºÅ¡£ \n
*/

#ifndef _SRE_MODULE_H
#define _SRE_MODULE_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

enum OS_MOUDLE_ID
{
	OS_MID_SYS                  = 0x0,

	OS_MID_MEM                  = 0x1,
	OS_MID_FSCMEM               = 0x2,
	OS_MID_DYNMEM               = 0x3,
	OS_MID_MCMEM                = 0x4,

	OS_MID_TSK                  = 0x8,
	OS_MID_SWI                  = 0x9,
	OS_MID_SWTMR                = 0xa,
	OS_MID_TICK                 = 0xb,
	OS_MID_CPUP                 = 0xc,

	OS_MID_MSG                  = 0xd,
	OS_MID_QUE                  = 0xe,
	OS_MID_SEM                  = 0xf,
	OS_MID_FSEM                 = 0x10,
	OS_MID_MBOX                 = 0x11,
	OS_MID_MCLCK                = 0x12,
	OS_MID_MCSEM                = 0x13,

	OS_MID_HWI                  = 0x14,
	OS_MID_HWWDG                = 0x15,
	OS_MID_CACHE                = 0x16,
	OS_MID_HWTMR                = 0x17,
	OS_MID_MMU                  = 0x18,

	OS_MID_LOG                  = 0x19,
	OS_MID_BOX                  = 0x1a,
	OS_MID_ERR                  = 0x1b,

	OS_MID_EXC                  = 0x1c,
	OS_MID_CSTK                 = 0x1d,
	OS_MID_PAT                  = 0x1e,

	OS_MID_HUNT                 = 0x1f,
	OS_MID_ECC                  = 0x20,
	OS_MID_MPU                  = 0x21,
	OS_MID_NMHWI                = 0x22,
	OS_MID_MCCOM                = 0x23,
	OS_MID_HWSEM                = 0x24,
	OS_MID_TRACE                = 0x25,
	OS_MID_KNLSTAT              = 0x26,
	OS_MID_EVTTIME              = 0x27,
	OS_MID_THRDCPUP             = 0x28,
	OS_MID_IPC                  = 0x29,
	OS_MID_STKMON               = 0x2a,
	OS_MID_MSGM                 = 0x2b,
	OS_MID_SHOW                 = 0x2c,
	OS_MID_SHELL                = 0x2d,
	OS_MID_TIMER                = 0x2e,
	OS_MID_RESLEAKMON           = 0x2f,
	OS_MID_INSERT               = 0x30,
	OS_MID_BUTT
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_MODULE_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

