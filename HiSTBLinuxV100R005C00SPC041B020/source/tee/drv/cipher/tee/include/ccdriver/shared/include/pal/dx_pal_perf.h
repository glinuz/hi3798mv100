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

#ifndef _DX_PAL_PERF_H_
#define _DX_PAL_PERF_H_

#ifdef LIB_PERF
#include "dx_pal_perf_plat.h"
#endif

typedef enum {
	PERF_TEST_TYPE_CRYS_AES_INTGR = 0x1,
	PERF_TEST_TYPE_CRYS_AES_INIT = 	0x2,
	PERF_TEST_TYPE_CRYS_AES_BLOCK = 0x3,
	PERF_TEST_TYPE_CRYS_AES_FIN = 	0x4,
	PERF_TEST_TYPE_HW_CMPLT = 	0x5,
	PERF_TEST_TYPE_PAL_MAP = 	0x6,
	PERF_TEST_TYPE_PAL_UNMAP = 	0x7,
	PERF_TEST_TYPE_MLLI_BUILD = 	0x8,
	PERF_TEST_TYPE_SYM_DRV_INIT = 	0x9,
	PERF_TEST_TYPE_SYM_DRV_PROC = 	0xA,
	PERF_TEST_TYPE_SYM_DRV_FIN =	0xB,
	PERF_TEST_TYPE_CRYS_HASH_INIT = 0xC,
	PERF_TEST_TYPE_CRYS_HASH_UPDATE=0xD,
	PERF_TEST_TYPE_CRYS_HASH_FIN = 	0xE,
	PERF_TEST_TYPE_CRYS_HMAC_INIT = 0xF,
	PERF_TEST_TYPE_CRYS_HMAC_UPDATE=0x10,
	PERF_TEST_TYPE_CRYS_HMAC_FIN = 	0x11,
	PERF_TEST_TYPE_CMPLT_SLEEP   = 0x12,
	PERF_TEST_TYPE_PKA_ModExp 	= 0x30,
	PERF_TEST_TYPE_TEST_BASE =	0x100,
	PERF_TEST_TYPE_MAX,
	PERF_TEST_TYPE_RESERVE32 = 	0x7FFFFFFF
}DX_PAL_PerfType_t;


#ifdef LIB_PERF 
#define DX_PAL_PERF_INIT  DX_PAL_PerfInit
#define DX_PAL_PERF_OPEN_NEW_ENTRY(num, type) \
		num = DX_PAL_PerfOpenNewEntry(type)

#define DX_PAL_PERF_CLOSE_ENTRY(num, type) \
		DX_PAL_PerfCloseEntry(num, type)
#define DX_PAL_PERF_DUMP DX_PAL_PerfDump
#define DX_PAL_PERF_FIN  DX_PAL_PerfFin

/** 
 * @brief   initialize performance test mechanism
 *         
 * @param[in] 
 * *
 * @return None
 */ 	
void DX_PAL_PerfInit(void);
 

/** 
 * @brief   opens new entry in perf buffer to record new entry
 *         
 * @param[in] entryType -  entry type (defined in dx_pal_perf.h) to be recorded in buffer 
 *
 * @return Returns a non-zero value in case of failure
 */ 	
DX_PAL_PerfData_t DX_PAL_PerfOpenNewEntry(DX_PAL_PerfType_t entryType);
 

/** 
 * @brief   closes entry in perf buffer previously opened by DX_PAL_PerfOpenNewEntry
 *         
 * @param[in] idx -  index of the entry to be closed, the return value of DX_PAL_PerfOpenNewEntry
 * @param[in] entryType -  entry type (defined in dx_pal_perf.h) to be recorded in buffer 
 *
 * @return Returns a non-zero value in case of failure
 */ 	
void DX_PAL_PerfCloseEntry(DX_PAL_PerfData_t idx, DX_PAL_PerfType_t entryType);
 

/** 
 * @brief   dumps the performance buffer
 *         
 * @param[in] None
 *
 * @return None
 */ 	
void DX_PAL_PerfDump(void); 


/** 
 * @brief   terminates resources used for performance tests
 *         
 * @param[in] 
 * *
 * @return None
 */ 	
void DX_PAL_PerfFin(void);
 
#else  //LIB_PERF
#define DX_PAL_PERF_INIT() 
#define DX_PAL_PERF_OPEN_NEW_ENTRY(num, type)  (num=num)
#define DX_PAL_PERF_CLOSE_ENTRY(num, type) 
#define DX_PAL_PERF_DUMP()
#define DX_PAL_PERF_FIN()


typedef uint32_t DX_PAL_PerfData_t;

#endif  //LIB_PERF


#endif /*_DX_PAL_PERF_H__*/
