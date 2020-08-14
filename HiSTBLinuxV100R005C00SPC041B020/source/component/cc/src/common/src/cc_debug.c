#ifndef __CC_DEBUG_H__
#define __CC_DEBUG_H__

#include "hi_debug.h"
#include "hi_module.h"
#include "cc_debug.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

HI_U8 DBG_CLR_OUT[9][10]=
{
    "default",
    "black",
    "white",
    "red",
    "green",
    "blue",
    "yellow",
    "megenta",
    "cyan",
};

HI_U8 DBG_OPA_OUT[5][12]=
{
    "default",
    "solid",
    "flash",
    "translucent",
    "transparent",
};



#ifdef __cplusplus
 #if __cplusplus
 }
 #endif
#endif

#endif
