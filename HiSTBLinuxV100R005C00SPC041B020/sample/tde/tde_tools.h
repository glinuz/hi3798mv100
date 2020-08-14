#ifndef __TDE_TOOLS_H__
#define __TDE_TOOLS_H__

#include "hi_common.h"


#define HI_MMB_New      HI_MMZ_New
#define HI_MMB_Map      HI_MMZ_Map
#define HI_MMB_Unmap    HI_MMZ_Unmap
#define HI_MMB_Delete   HI_MMZ_Delete

#define Printf  HI_PRINT

typedef struct hiTDE_ADDR
{
    char* vir;
    unsigned int phy;
} TDE_ADDR;

static inline  int TDE_MALLOC(TDE_ADDR *pAddr, unsigned int size)
{
    pAddr->phy = (unsigned long)HI_MMB_New(size, 4, NULL, "sample_tde");
    if(0 == pAddr->phy)
    {
        Printf("HI_MMB_New Fail, line:%d\n", __LINE__);
        pAddr->vir = NULL;
        return -1;
    }
    pAddr->vir =  (char*)HI_MMB_Map((HI_U32)pAddr->phy, 0);
    return 0;
}

static inline void TDE_FREE(TDE_ADDR* pAddr)
{
    HI_MMB_Unmap((HI_U32)pAddr->phy);
    HI_MMB_Delete((HI_U32)pAddr->phy);
}

#endif /*__TDE_TOOLS_H__*/

