/***************************************************************
*  Copyright 2014 (c) Discretix Technologies Ltd.              *
*  This software is protected by copyright, international      *
*  treaties and various patents. Any copy, reproduction or     *
*  otherwise use of this software must be authorized in a      *
*  license agreement and include this Copyright Notice and any *
*  other notices specified in the license agreement.           *
*  Any redistribution in binary form must be authorized in the *
*  license agreement and include this Copyright Notice and     *
*  any other notices specified in the license agreement and/or *
*  in materials provided with the binary distribution.         *
****************************************************************/



#ifndef _DX_PAL_ERROR_H
#define _DX_PAL_ERROR_H


#ifdef __cplusplus
extern "C"
{
#endif

#define DX_PAL_BASE_ERROR                0x0F000000

/* Memory error returns */
#define DX_PAL_MEM_BUF1_GREATER          DX_PAL_BASE_ERROR + 0x01UL
#define DX_PAL_MEM_BUF2_GREATER          DX_PAL_BASE_ERROR + 0x02UL

/* Semaphore error returns */
#define DX_PAL_SEM_CREATE_FAILED         DX_PAL_BASE_ERROR + 0x03UL
#define DX_PAL_SEM_DELETE_FAILED         DX_PAL_BASE_ERROR + 0x04UL
#define DX_PAL_SEM_WAIT_TIMEOUT          DX_PAL_BASE_ERROR + 0x05UL                   
#define DX_PAL_SEM_WAIT_FAILED           DX_PAL_BASE_ERROR + 0x06UL                   
#define DX_PAL_SEM_RELEASE_FAILED        DX_PAL_BASE_ERROR + 0x07UL                   

#define DX_PAL_ILLEGAL_ADDRESS		 DX_PAL_BASE_ERROR + 0x08UL

#ifdef __cplusplus
}
#endif

#endif


