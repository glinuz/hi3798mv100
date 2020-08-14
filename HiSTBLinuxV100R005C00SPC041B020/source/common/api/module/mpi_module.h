/******************************************************************************
Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_module_mgr.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The module manager interfaces.
                Every module should be registered before using other interfaces.
Function List : None.
History       :
******************************************************************************/
/** @defgroup USER_MODE In user mode
    @brief The following interfaces should be only called in user mode. */

/** @{ */

/** @defgroup iMODULE_MGR Module pool
    @brief Module manager pool interfaces just only for innter */
/** @defgroup oMODULE_MGR Module manager
    @brief Module manager interfaces for other modules */

/** @defgroup iMEM Memory pool
    @brief Memory pool manager initialize interfaces */
/** @defgroup oMEM Memory
    @brief System heap memory allocate interfaces for other modules */

/** @defgroup iMMZ MMZ pool
    @brief MMZ pool interfaces just only for inner modules */
/** @defgroup oMMZ MMZ
    @brief MMZ memory allocate interfaces for other modules. */

/** @} */

/** @addtogroup oMODULE_MGR */
/** @{ */
#ifndef __HI_MODULE_MGR_H__
#define __HI_MODULE_MGR_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "hi_type.h"

/**
@brief Initialize this manager moudle .
@attention Before calling other interfaces of this module, calling this interface.
@param[in] None
@param[out] None
@retval ::HI_SUCCESS Success
@retval ::HI_FAILURE Failure
@see \n
N/A
*/
HI_S32 HI_MODULE_Init(HI_VOID);

/**
@brief Terminate this manager moudle
@attention None
@param[in] None
@param[out] None
@retval ::HI_SUCCESS Success
@retval ::HI_FAILURE Failure
@see \n
N/A
*/
HI_S32 HI_MODULE_DeInit(HI_VOID);

/**
@brief Get the module ID that has been registered. 
@attention None
@param[in] pu8ModuleName The module name
@param[out] None
@retval ::The valid module ID, which has been registered
@retval ::HI_FAILURE Failure
@see \n
N/A
*/
HI_S32 HI_MODULE_GetModuleID(const HI_U8* pu8ModuleName);

#ifdef __cplusplus
}
#endif

#endif

