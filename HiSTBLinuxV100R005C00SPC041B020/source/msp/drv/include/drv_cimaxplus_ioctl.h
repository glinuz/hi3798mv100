#ifndef __DRV_CIMAXPLUS_IOCTL_H__
#define __DRV_CIMAXPLUS_IOCTL_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_unf_ci.h"
#include "hi_drv_cimaxplus.h"


typedef enum {
  DEVICE_TYPE_CI_READ,
  DEVICE_TYPE_TS_WRITE,
  DEVICE_TYPE_TS_READ
} rw_type_t;

typedef struct
{
  rw_type_t type;
  HI_U8     moduleId;
  HI_U8*    data;
  HI_U32    size;
  HI_U32    copiedSize;
} rw_data_t,*prw_data_t;

typedef struct
{
  HI_U8* txData;
  HI_U32 txSize;
  HI_U8* rxData;
  HI_U32 rxSize;
} ioctl_data_t,*pioctl_data_t;

typedef signed long               __i32;
#define DEVICE_IOC_MAGIC          HI_ID_CIMAXPLUS
#define DEVICE_IOC_SELECT_INTF    _IOWR(DEVICE_IOC_MAGIC,  0, __i32)
#define DEVICE_IOC_CI_WRITE       _IOWR(DEVICE_IOC_MAGIC,  1, ioctl_data_t)
#define DEVICE_IOC_UNLOCK_READ    _IOWR(DEVICE_IOC_MAGIC,  2, __i32)
#define DEVICE_IOC_SET_CONFIG     _IOWR(DEVICE_IOC_MAGIC,  3, ioctl_data_t)
#define DEVICE_IOC_DEV_READY      _IOWR(DEVICE_IOC_MAGIC,  4, __i32)
#define DEVICE_IOC_MAXNR          4


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __DRV_CIMAXPLUS_IOCTL_H__*/
