#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "hi_tde_api.h"
#include "hi_tde_errcode.h"
#include "tde_tools.h"
#include "tde_common.h"
#include "hi_tde_type.h"

#define Img201006300005_N   "../res/Img201006300005_N.bits"

#define HI_TDE_CreateSurfaceByFile(a, b) HI_TDE_CreateSurfaceByFile_D(a, b, __LINE__)

HI_S32 main(void)
{
    TDE_HANDLE handle;
    TDE2_SURFACE_S stSrc;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S stSrcRect;
    TDE2_RECT_S stDstRect;
    HI_BOOL bBlock = HI_TRUE;
    HI_U32 u32TimeOut = 10;

    /*1.creat source surface*/
    HI_TDE_CreateSurfaceByFile(Img201006300005_N, &stSrc);
    HI_TDE_DuplicateSurface(&stSrc, &stDst);

   /*2.set region of operation*/
    stSrcRect.s32Xpos = 0;
    stSrcRect.s32Ypos = 0;
    stSrcRect.u32Height = stSrc.u32Height;
    stSrcRect.u32Width = stSrc.u32Width;

    stDstRect.s32Xpos = 0;
    stDstRect.s32Ypos = 0;
    stDstRect.u32Height = stDst.u32Height;
    stDstRect.u32Width = stDst.u32Width;

    /*3.open TDE device*/	
    HI_TDE2_Open();

   /*4.show source Bitmap*/
    Printf("Showing source Bitmap\n");
    Printf("Press the Enter key to show the Result Bitmap\n");  
    HI_TDE_ShowSurface(&stSrc, 50, 50);
    /*5.begin a job*/
    handle = HI_TDE2_BeginJob();

    /*6.QuickCopy  operation*/
    HI_TDE2_QuickCopy(handle, &stSrc, &stSrcRect, &stDst, &stDstRect);    

    /*7.submit job*/
    HI_TDE2_EndJob(handle, HI_FALSE, bBlock, u32TimeOut);

    /*8.show result*/
    Printf("Showing Result Bitmap\n");
    Printf("Press the Enter key to quit out\n");
    HI_TDE_ShowSurface(&stDst, 50, 50);
    /*9.destroy source surface*/
    HI_TDE_DestroySurface(&stSrc);
    HI_TDE_DestroySurface(&stDst);    

    /*10.close TDE device*/
    HI_TDE2_Close();
    
    return HI_SUCCESS;
}

