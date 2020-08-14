#ifndef __MXL_DEBUG__
#define __MXL_DEBUG__

#include "MxL_HRCLS_OEM_Defines.h"

#define MXLDBG_ERROR 0x01
#define MXLDBG_ENTER 0x02
#define MXLDBG_EXIT 0x04
#define MXLDBG_API 0x08

#if (MXL_MODULE_DEBUG_LEVEL > 2)
#define MXLDBG3(x) x
#else
#define MXLDBG3(x)
#endif

#if (MXL_MODULE_DEBUG_LEVEL > 1)
#define MXLDBG2(x) x
#else
#define MXLDBG2(x)
#endif

#if (MXL_MODULE_DEBUG_LEVEL > 0)
#define MXLDBG1(x) x
#else
#define MXLDBG1(x)
#endif

#if (MXL_MODULE_DEBUG_OPTIONS & MXLDBG_ERROR)
#define MXLERR(x) x
#else
#define MXLERR(x) 
#endif

#if (MXL_MODULE_DEBUG_OPTIONS & MXLDBG_ENTER)
#define MXLENTER(x) x
#define MXLENTERSTR MXL_MODULE_DEBUG_FCT("+++++ %s \n\r", __FUNCTION__);
#else
#define MXLENTER(x) 
#define MXLENTERSTR
#endif

#if (MXL_MODULE_DEBUG_OPTIONS & MXLDBG_EXIT)
#define MXLEXIT(x) x
#define MXLEXITSTR(retcode) MXL_MODULE_DEBUG_FCT("----- %s (retcode: %d)\n\r", __FUNCTION__, retcode);
#else
#define MXLEXIT(x) 
#define MXLEXITSTR(retcode)
#endif

#if (MXL_MODULE_DEBUG_OPTIONS & MXLDBG_API)
#define MXLENTERAPI(x) x
#define MXLENTERAPISTR(devId) MXL_MODULE_DEBUG_FCT("+++ %s [%d] \n\r", __FUNCTION__, devId);
#define MXLEXITAPI(x) x
#define MXLEXITAPISTR(devId, retcode) MXL_MODULE_DEBUG_FCT("--- %s [%d] (retcode: %d)\n\r", __FUNCTION__, devId, retcode);
#else
#define MXLENTERAPI(x)
#define MXLENTERAPISTR(devId)
#define MXLEXITAPI(x)
#define MXLEXITAPISTR(devId, retcode)
#endif

#endif

