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


#ifndef __CRYS_BYPASS_API_H__
#define __CRYS_BYPASS_API_H__

/*! \file crys_bypass_api.h 
    \brief This file include the implementation of the crys_bypass. 
*/
#include "dx_pal_types.h"
#include "crys_bypass_error.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*!
 * Memory copy using HW engines. 
 * The table below describes the supported copy modes that 
 * reference by the data input/output buffers: 
 *  
 *  ----------------------------------------------
 *  |  DataIn_ptr  |         DataOut_ptr         |
 *  |--------------------------------------------|
 *  | SRAM         | DCACHE/SRAM/DLLI/MLLI       |
 *  | ICACHE       | DCACHE/SRAM/DLLI/MLLI       |
 *  | DCACHE       | DCACHE/SRAM/DLLI/MLLI       |
 *  | DLLI         | DCACHE/SRAM/DLLI            |
 *  | MLLI         | DCACHE/SRAM/MLLI            |
 *  ----------------------------------------------
 *   
 * \param DataIn_ptr This is the source buffer which need to copy from. 
 *      	It may be a SeP local address or a DMA Object handle as described
 *      	in the table above. 
 * \param DataSize In bytes
 * \param DataOut_ptr This is the destination buffer which need to copy to. 
 *      	It may be a SeP local address or a DMA Object handle as described
 *      	in the table above. 
 *  
 * Restriction: MLLI refers to DMA oject in System memory space. 
 * 
 * \return CRYSError_t On success CRYS_OK is returned, on failure an error according to
 *                       CRYS_Bypass_error.h
 */
CIMPORT_C CRYSError_t CRYS_Bypass( uint8_t*     DataIn_ptr,        
                                   uint32_t     DataSize,         
                                   uint8_t*     DataOut_ptr);





#ifdef __cplusplus
}
#endif

#endif 

