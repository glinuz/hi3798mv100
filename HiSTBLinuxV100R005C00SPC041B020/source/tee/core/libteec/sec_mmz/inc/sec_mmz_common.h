#ifndef __SEC_MMZ_COMMON_H
#define __SEC_MMZ_COMMON_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

typedef enum {
	HI_MMZ_NEW = 0x1000,
	HI_MMZ_DEL,
	HI_MMZ_CP_CA2TA,
	HI_MMZ_CP_TA2CA,
	HI_MMZ_CP_TA2TA,
	HI_MMZ_TEST,
	HI_MMZ_DEBUG
} hi_mmz_cmd;

#define SEC_MMZ_UUID 0x0C0C0C0C, 0x0C0C, 0x0C0C, \
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C }

#define SUCCESS 0
#define ERR_IN_SEC_OS -1
#define NO_SEC_OS -2

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __SEC_MMZ_COMMON_H */
