#ifndef __HI_TEE_ERRCODE_H__
#define __HI_TEE_ERRCODE_H__

#include "hi_type.h"
/********************************Error Code Definition********************************/
/** \addtogroup      TEE */
/** @{ */  /** <!-- [TEE] */

#define HI_ERR_NULL_PTR             ((HI_S32)0x81000001)
#define HI_ERR_INVALID_PARAM        ((HI_S32)0x81000002)
#define HI_ERR_NO_MEMORY            ((HI_S32)0x81000003)
#define HI_ERR_NO_DEV               ((HI_S32)0x81000004)
#define HI_ERR_NOT_SUPPORT          ((HI_S32)0x81000005)
#define HI_ERR_NOT_INIT             ((HI_S32)0x81000006)


/*
 * synchronize with source/msp/include/hi_error_mpi.h
 */
#define HI_ERR_DMX_INVALID_PARA             (HI_S32)(0x80150002)
#define HI_ERR_DMX_NOT_SUPPORT              (HI_S32)(0x80150004)
#define HI_ERR_DMX_UNMATCH_CHAN             (HI_S32)(0x8015000b)
#define HI_ERR_DMX_NOATTACH_KEY             (HI_S32)(0x80150013)
#define HI_ERR_DMX_UNMATCH_KEY              (HI_S32)(0x80150014)
#define HI_ERR_DMX_NOFREE_KEY               (HI_S32)(0x80150015)
#define HI_ERR_DMX_ATTACHED_KEY             (HI_S32)(0x80150016)
#define HI_ERR_DMX_OCCUPIED_PID             (HI_S32)(0x80150020)
#define HI_ERR_DMX_BUSY_CHAN                (HI_S32)(0x80150037)
#define HI_ERR_DMX_BUSY_KEY                 (HI_S32)(0x80150039)
/** @} */  /** <!-- ====End of Error Code Definition==== */
#endif

