#ifndef __TEE_EXTERNEL_API_H_
#define __TEE_EXTERNEL_API_H_

#include "sre_typedef.h"

void ASM_MemMove(void* dest, void* src, UINT32 size);
void ASM_MemCopy(void* dest, void* src, UINT32 size);

#endif
