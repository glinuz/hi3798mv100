#include "hi_type.h"
#include "hi_tde_type.h"
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
HI_S32 HI_TDE_CreateSurfaceByFile_D(char *file, TDE2_SURFACE_S *pstSurface, HI_U32 lineNum);
HI_S32 HI_TDE_CreateMBSurfaceByFile_D(char *file, TDE2_MB_S *pstMBSur, HI_U32 lineNum);
HI_S32 HI_TDE_DuplicateSurface(TDE2_SURFACE_S *pstInSurface, TDE2_SURFACE_S *pstOutSurface);
 HI_S32 HI_TDE_DestroySurface(TDE2_SURFACE_S *pstSurface);
HI_S32 HI_TDE_DestroyMBSurface(TDE2_MB_S *pstMBSur);
HI_S32 HI_TDE_ShowSurface(TDE2_SURFACE_S *pstSurface, HI_U32 u32PosX, HI_U32 u32PosY);
  #ifdef __cplusplus
}
#endif  /* __cplusplus */

