#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdio.h>

#include "hi_tde_api.h"
#include "hi_tde_errcode.h"
#include "tde_tools.h"
#include "tde_common.h"
#include "hi_tde_type.h"

#define Img201108050007_N   "../res/Img201006300005_N.bits"

#define HI_TDE_CreateSurfaceByFile(a, b) HI_TDE_CreateSurfaceByFile_D(a, b, __LINE__)
HI_S32  main(void)
{
    TDE_HANDLE handle;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S stDstRect;
    HI_BOOL bSync = HI_FALSE;
    HI_BOOL bBlock = HI_TRUE;
    HI_U32 u32TimeOut = 500;
    
    /*1. set fillcolor :blue */
    HI_U32 u32FillData = 0xff0000ff; 
    
    /*2.creat target surface*/
    HI_TDE_CreateSurfaceByFile(Img201108050007_N, &stDst);
    stDstRect.s32Xpos = 0;
    stDstRect.s32Ypos = 0;
    stDstRect.u32Height = stDst.u32Height/2;
    stDstRect.u32Width = stDst.u32Width/2;

    /*3.open TDEdevice*/
    HI_TDE2_Open();

    /*4.show source Bitmap*/
    Printf("Showing source Bitmap\n");
    Printf("Press the Enter key to show the Result Bitmap\n");  
    HI_TDE_ShowSurface(&stDst, 50, 50);
    
    /*5.begin a job*/
    handle = HI_TDE2_BeginJob();
    
    /*6.TDE QuicFill operation*/
    HI_TDE2_QuickFill(handle, &stDst, &stDstRect, u32FillData);
    
     /* 7. submit job */
    HI_TDE2_EndJob(handle, bSync, bBlock, u32TimeOut);
     
     /*8.show result*/
    Printf("Showing Result Bitmap\n");
    Printf("Press the Enter key to quit out\n");
    HI_TDE_ShowSurface(&stDst, 50,50);

     /*9.destroy target surface*/
    HI_TDE_DestroySurface(&stDst);  
     
    /*10.close TDE device*/
    HI_TDE2_Close();
    
    return HI_SUCCESS;
}

