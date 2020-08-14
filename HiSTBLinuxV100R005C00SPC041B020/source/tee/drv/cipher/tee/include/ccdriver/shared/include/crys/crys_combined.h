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

/** @file
 *  \brief This file contains all of the enums and definitions that are used for the 
 *         CRYS combined and tunneling modes APIs, as well as the APIs themselves. 
 *
 */
#ifndef CRYS_COMBINED_H
#define CRYS_COMBINED_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "dx_pal_types.h"
#include "crys_error.h"

#include "crys_aes.h"
#include "crys_hash.h"

#ifdef __cplusplus
extern "C"
{
#endif

/************************ Defines ******************************/
#define CRYS_COMBINED_MAX_NODES 4


/************************ Enums ********************************/
/*!
 engine source input
*/
typedef enum {
	INPUT_NULL = 0,     /* no input */
	INPUT_ENGINE_1 = 1, /* input from output of first cipher engine in sequence */
	INPUT_ENGINE_2 = 2, /* input from output of second cipher engine in sequence */
	INPUT_DIN = 15,     /* input from DIN */
	INPUT_ENGINE_RESERVE32B = INT32_MAX,
} CrysCombinedEngineSource_e;

/************************* Typedefs ****************************/
/*!
 Context types for crys combined operations
*/
typedef union CrysCombinedContextPointer {
	CRYS_AESUserContext_t *pAesContext;
	CRYS_HASHUserContext_t *pHashContext;
} CrysCombinedContextPointer_u;

/*!
 Configuration Node which accommodate the crys context
 pointer and the engine source input.
*/
typedef struct CrysCombinedNodeConfig {
	CrysCombinedContextPointer_u *pContext;
	CrysCombinedEngineSource_e engineSrc;
} CrysCombinedNodeConfig_t;

/*!
 User array of nodes. Each node in the array relate other node
 which reflect the user combined scheme.
*/
typedef struct CrysCombinedConfig {
	CrysCombinedNodeConfig_t node[CRYS_COMBINED_MAX_NODES];
} CrysCombinedConfig_t;

/************************* MACROS ******************************/
/*!
 Sets a configuration node properties:
	- 
 \param pConfig [in]: A pointer to configuraton node object (type of CrysCombinedConfig_t).
 \param engIdx [in]: The current engine index in the NodesConfig array.
 \param engSrc [in]: The engine source input to set (type of CrysCombinedEngineSource_e), i.e.
                     number of other engine which output should be transferred by DMA to 
		     the input of current engine. 
 \param ctxPtr [in]: A context pointer to set (type of CrysCombinedContextPointer_u).
*/
#define CrysCombinedConfigSet(pConfig, engIdx, engSrc, ctxPtr) \
	do { \
		(pConfig)->node[(engIdx)].pContext = (void *)(ctxPtr); \
		(pConfig)->node[(engIdx)].engineSrc = (engSrc); \
	} while (0)

/*!
 Gets a configuration node source input (type of CrysCombinedConfig_t).
 \param pConfig [in]: A pointer to configuraton node object (type of CrysCombinedNodeConfig_t).
 \param engineIdx [in]: The engine index in the NodesConfig array.
 \return Configuration Node's source input.
*/
#define CrysCombinedEngineSrcGet(pConfig, engineIdx) \
	((pConfig)->node[(engineIdx)].engineSrc)



/************************************************************************/ 
/*************************** Public Functions ***************************/
/************************************************************************/ 

/************************************************************************/ 
/*!
 * Clears the configuration nodes.
 * 
 * \param pConfig A pointer to the configuration scheme array
 */
CIMPORT_C void CrysCombinedConfigInit(CrysCombinedConfig_t *pConfig);

/************************************************************************/ 
/*!
 * This function is used to initilize the combined (tunneling) operations.
 *  
 * The function initializes user's combined mode configuration structure, 
 * used during current process sequence (Init-Block-Finish). 
 *  
 * Before processing the combined mode and calling this function the user must 
 * initialize all needed sub operations contexts (AES, AES-MAC, HASH) included into 
 * this combined operation and perform the following: 
 *
 *      - allocate combined mode configuration structure of type CrysCombinedConfig_t;
 *        The structure containing MAX_NUM_NODES (see define) nodes of type CrysCombinedNodeConfig_t, 
 *	  each of them contains pointer to sub operation context and input data source
 *        indication according to DMA data flow in configuration, as follows: 
 *	     { {pCtxt1; src1;},  {pCtxt2; src2}, .... {NULL; srcForDout} }, where:
 *              - pCtxtX - pointer to context of current sub operation X or NULL for indication
 *                of DOUT (DMA output) or end node in the configuration;
 *              - srcX is number of previous engine (sub operation), which output is used 
 *                as input to engine (or DOUT) X; 
 *      - allocate CRYS contexts for each sub operation (AES...HASH) included into
 *        combined configuration - maximum count of operations is MAX_NUM_NODES-1 (one node - for
 *        DMA autput or indication of end of the configuration);
 *      - call appropriate CRYS Init functions for each sub operation (CRYS_AES_Init or 
 *	  CRYS_HASH_Init) according to combined mode and given operations parameters;
 *	- initialize (clean) the configuration structure by calling macro CrysCombinedConfigInit();
 *      - set all needed configuration nodes by calling macro CrysCombinedConfigSet() for each sub
 *	  operation involved into combined mode; the input source indication for each engine 
 *        should be set according to engines order defined by the user in the configuration 
 *	  structure. 
 *	- set indication of ending (last) node and DMA output source by calling the same macro  
 *        CrysCombinedConfigSet() with appropriate input parameters:
 *	  	- context pointer ctxPtrX = NULL;
 *	  	- indication (type: CrysCombinedEngineSource_e) of engine, which output 
 *                should be transferred to DMA DOUT; if DMA output is not used there, 
 *                then srcX = INPUT_NULL;                                     
 *        
 *  Then the user may call this function to initialize the allocated combined
 *  user context.
 *  
 *  
 * \param pConfig A pointer to the Configuration Nodes array (NodesConfig). 
 * 			This array represents the user combined scheme. 
 * 
 * \return CIMPORT_C CRYSError_t On success the value CRYS_OK is returned, 
 * 			and on failure - a value from crys_combined_error.h
 */
CIMPORT_C CRYSError_t CRYS_Combined_Init(CrysCombinedConfig_t *pConfig);


/***************************************************************************/ 
/*!
 * This function is used to process block of data in the combined (tunneling) mode.
 * 
 * \param pConfig A pointer to the Configuration Nodes array. 
 * 			This array represents the user combined scheme. 
 * \param cipherOffset Relevant in cases where the authenticated  data resides in 
 *      		a different offset from the cipher data.
 *      		Note: currently an error returned for any value other than zero.
 * \param pDataIn A pointer to a block of input data ready for processing.
 * \param dataInSize The size of the input data.
 * \param pDataOut A pointer to output data. Could be the same as input data 
 *      		pointer (for inplace operations) or NULL if there is
 *      		only authentication for output.
 * 
 * \return CIMPORT_C CRYSError_t On success the value CRYS_OK is returned, 
 * 			and on failure - a value from crys_combined_error.h
 */
CIMPORT_C CRYSError_t CRYS_Combined_Process(
		CrysCombinedConfig_t *pConfig,
		uint32_t cipherOffset,
		uint8_t *pDataIn,
		uint32_t dataInSize,
		uint8_t *pDataOut);


/***************************************************************************/ 
/*!
 * This function is used to finish the combined or tunneling operations
 * It releases all used contexts (including suboperation ones).
 * 
 * \param pConfig A pointer to the Configuration Nodes array. 
 * 		This array represents the user combined scheme. 
 * \param cipherOffset Relevant in cases where the authenticated  data resides in 
 *      		a different offset from the cipher data.
 *      		Note: currently an error returned for any value other than zero.
 * \param pDataIn A pointer on a block of input data ready for processing.
 * \param dataInSize The size of the input data.
 * \param pDataOut A pointer to output data. Could be the same as input data pointer 
 *      		(for inplace operations) or NULL if there is only
 *      		authentication result for output.
 * \param pAuthData A pointer to authentication or digest result output.
 * \param pAuthDataSize A pointer to size of user given output buffer [in] 
 * 			and actual size [out] of authentication or digest result . 
 * 
 * \return CIMPORT_C CRYSError_t On success the value CRYS_OK is returned, 
 * 			and on failure - a value from crys_combined_error.h
 */
CIMPORT_C CRYSError_t CRYS_Combined_Finish(
		CrysCombinedConfig_t *pConfig,
		uint32_t cipherOffset,
		uint8_t *pDataIn,
		uint32_t dataInSize,
		uint8_t *pDataOut,
		uint8_t *pAuthData,
		uint32_t *pAuthDataSize);

/*!
 * This function is used to perform the combined or tunneling operations with 
 * one function call.
 * 
 * \param pConfig A pointer to the Configuration Nodes array. 
 * 		This array represents the user combined scheme. 
 * \param cipherOffset Relevant in cases where the authenticated  data resides in 
 *      		a different offset from the cipher data.
 *      		Note: currently an error returned for any value other than zero.
 * \param pDataIn A pointer on a block of input data ready for processing.
 * \param dataInSize The size of the input data.
 * \param pDataOut A pointer to output data. Could be the same as input data pointer 
 *      		(for inplace operations) or NULL if there is only
 *      		authentication result for output.
 * \param pAuthData A pointer to authentication or digest result output.
 * \param pAuthDataSize A pointer to size of user given output buffer [in] 
 * 			and actual size [out] of authentication or digest result . 
 * 
 * \return CIMPORT_C CRYSError_t On success the value CRYS_OK is returned, 
 * 			and on failure - a value from crys_combined_error.h
 */
CIMPORT_C CRYSError_t CRYS_Combined(
		CrysCombinedConfig_t *pConfig,
		uint32_t cipherOffset,
		uint8_t *pDataIn,
		uint32_t dataInSize,
		uint8_t *pDataOut,
		uint8_t *pAuthData,
		uint32_t *pAuthDataSize);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CRYS_COMBINED_H */ 





