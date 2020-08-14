/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_stat.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/12/16
  Last Modified :
  Description   : mpi_stat.c header file
  Function List :
  History       :
  1.Date        : 2008/12/16
    Author      : z42136
    Modification: Created file

******************************************************************************/
/** @defgroup USR_MODE In user mode 
    @brief The following interfaces should be only called in user mode. */

/** @{ */

/** @defgroup H_MPI_MEM Memory operation
    @brief Memory operation interfaces for other CBB in user mode */
    
/** @defgroup H_MPI_STAT Stat opertation 
    @brief Simple stat operation interfaces in user mode */

/** @} */



#ifndef __HI_MPI_STAT_H__
#define __HI_MPI_STAT_H__

#include "hi_drv_stat.h"

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup H_MPI_STAT */
/** @{ */
typedef char THREAD_NAME[64];

/* for userspace stat */
typedef struct
{
    HI_BOOL bUsed;
    HI_U32 min_time;    // us
    HI_U32 max_time;    // us
    HI_U32 avg_time;    // us

    THREAD_NAME name;
}STAT_USERSPACE_S;

typedef struct
{
    HI_U32 stat_thread_phyaddr;
    STAT_USERSPACE_S * stat_thread_uvirtaddr;

    HI_U32 counter;
    struct timeval tv_last;
    HI_U64 time_total;    // us
}*HI_STAT_HANDLE, HI_STAT_HANDLE_S;

//typedef HI_U32 HI_STAT_HANDLE;
#define HI_STAT_INVALID_HANDLE (-1)

extern HI_S32 HI_MPI_STAT_Init(HI_VOID);
extern HI_S32 HI_MPI_STAT_DeInit(HI_VOID);

extern HI_S32 HI_MPI_STAT_ThreadReset(HI_STAT_HANDLE handle);
extern HI_S32 HI_MPI_STAT_ThreadResetAll(HI_VOID);

extern HI_S32 HI_MPI_STAT_ThreadRegister(char * name, HI_STAT_HANDLE * pHandle);
extern HI_S32 HI_MPI_STAT_ThreadUnregister(HI_STAT_HANDLE * pHandle);

extern HI_S32 HI_MPI_STAT_ThreadProbe(HI_STAT_HANDLE handle);

extern HI_S32 HI_MPI_STAT_Event(STAT_EVENT_E enEvent, HI_U32 Value);
extern HI_U32 HI_MPI_STAT_GetTick(HI_VOID);
extern HI_S32 HI_MPI_STAT_NotifyLowDelayEvent (HI_LD_Event_S * evt);

/** @} */

#ifdef __cplusplus
}
#endif


#endif /* __HI_MPI_STAT_H__ */

