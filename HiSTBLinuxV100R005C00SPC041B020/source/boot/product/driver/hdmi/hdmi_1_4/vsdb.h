#include "typedefs.h"
#include "drv_reg_proc.h"
#include "hi_unf_hdmi.h"


#ifndef __VSDB_H__
#define __VSDB_H__


HI_BOOL Is4KFmt(HI_UNF_ENC_FMT_E enHdFmt);
HI_U8   Setting4K(HI_U32 u32Fmt);
void    Config4KInfoFrame(HI_U32 u324KFormat);
    
#endif

