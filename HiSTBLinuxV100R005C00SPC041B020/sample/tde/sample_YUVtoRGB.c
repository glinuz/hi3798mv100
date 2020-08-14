#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "hi_tde_api.h"
#include "hi_tde_errcode.h"
#include "tde_tools.h"
#include "tde_common.h"
#include "hi_tde_type.h"

#define YCbCr420MB              "../res/jpg420x1024x684x1024x1024.yuv"
#define Img201104270003_N_1     "../res/Img201104270003_N_1.bits"

#define HI_TDE_CreateSurfaceByFile(a, b) HI_TDE_CreateSurfaceByFile_D(a, b, __LINE__)
#define HI_TDE_CreateMBSurfaceByFile(a, b) HI_TDE_CreateMBSurfaceByFile_D(a, b, __LINE__)

HI_S32 main(void)
{
    TDE_HANDLE handle;
    TDE2_MB_S stForeGround;
    TDE2_SURFACE_S stBackGround;
    TDE2_RECT_S stForeGroundRect;
    TDE2_RECT_S stBackGroundRect;
    TDE2_MBOPT_S stOpt ;   
    HI_BOOL bBlock = HI_TRUE;
    HI_U32 u32TimeOut = 50;
    memset(&stOpt,0,sizeof(TDE2_MBOPT_S));

    /*1.creat source surface*/
    HI_TDE_CreateMBSurfaceByFile(YCbCr420MB, &stForeGround);
    HI_TDE_CreateSurfaceByFile(Img201104270003_N_1, &stBackGround);

    /*2.set the region of operation*/
    stBackGroundRect.s32Xpos = 0;
    stBackGroundRect.s32Ypos = 0;
    stBackGroundRect.u32Height = stBackGround.u32Height;
    stBackGroundRect.u32Width = stBackGround.u32Width;

    stForeGroundRect.s32Xpos = 0;
    stForeGroundRect.s32Ypos = 0;
    stForeGroundRect.u32Height = stForeGround.u32YHeight;
    stForeGroundRect.u32Width = stForeGround.u32YWidth;
    
    /*3.open TDE device*/
    HI_TDE2_Open();
    
    /*4.show  BackGround Bitmap*/
    Printf("Showing BackGround Bitmap\n");
    Printf("Press the Enter key to show the Result Bitmap\n");  
    HI_TDE_ShowSurface(&stBackGround, 50, 50);

    
    /*5.begin a job*/
    handle = HI_TDE2_BeginJob();
    
    /*6.MBblit operation*/
    HI_TDE2_MbBlit(handle, &stForeGround, &stForeGroundRect, &stBackGround, &stBackGroundRect, &stOpt);
    
    /*7.submit job*/
    HI_TDE2_EndJob(handle, HI_FALSE, bBlock, u32TimeOut);

    /*8.show result*/
    Printf("Showing Result Bitmap\n");
    Printf("Press the Enter key to quit out\n");

   
    HI_TDE_ShowSurface(&stBackGround, 50, 50);

    /*9.destroy source surface*/
    HI_TDE_DestroyMBSurface(&stForeGround); 
    HI_TDE_DestroySurface(&stBackGround); 

    /*10.close TDE device */
    HI_TDE2_Close();
    return HI_SUCCESS;
}

