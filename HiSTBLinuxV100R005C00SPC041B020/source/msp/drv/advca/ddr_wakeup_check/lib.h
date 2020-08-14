#ifndef __LIB_H__
#define __LIB_H__

#include "types.h"

#undef reg_get
#define reg_get(addr) (*(volatile unsigned int *)(addr))

#undef reg_set
#define reg_set(addr, val) (*(volatile unsigned int *)(addr) = (val))

#endif /*__LIB_H__*/
