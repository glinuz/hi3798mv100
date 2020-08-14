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

#define Img201304220007_N   "../res/Img201304220007_N.bits"
#define Img201104270003_N   "../res/Img201104270003_N.bits"

#define HI_TDE_CreateSurfaceByFile(a, b) HI_TDE_CreateSurfaceByFile_D(a, b, __LINE__)
HI_S32  main(void)
{
    TDE_HANDLE handle;
    TDE2_SURFACE_S stForeGround;
    TDE2_SURFACE_S stBackGround;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S stForeGroundRect;
    TDE2_RECT_S stBackGroundRect;
    TDE2_RECT_S stDstRect;
    TDE2_OPT_S stOpt;
    
    HI_BOOL bBlock = HI_TRUE;
    HI_U32 u32TimeOut = 50;
    memset(&stOpt,0,sizeof(TDE2_OPT_S));

    
    /*1.creat source surface */
    HI_TDE_CreateSurfaceByFile(Img201104270003_N, &stBackGround);
    HI_TDE_CreateSurfaceByFile(Img201304220007_N, &stForeGround);
    HI_TDE_DuplicateSurface(&stBackGround, &stDst);
  
   /*2.set the region of operation*/
    stBackGroundRect.s32Xpos = 0;
    stBackGroundRect.s32Ypos = 0;
    stBackGroundRect.u32Height = stBackGround.u32Height;
    stBackGroundRect.u32Width = stBackGround.u32Width;

    stForeGroundRect.s32Xpos = 0;
    stForeGroundRect.s32Ypos = 0;
    stForeGroundRect.u32Height = stForeGround.u32Height;
    stForeGroundRect.u32Width = stForeGround.u32Width;

    stDstRect = stBackGroundRect;
    
    /*3. set option  of colorkey operation:only filter blue */
    stOpt.enColorKeyMode = TDE2_COLORKEY_MODE_FOREGROUND;
    stOpt.enOutAlphaFrom = TDE2_OUTALPHA_FROM_FOREGROUND;
    stOpt.unColorKeyValue.struCkARGB.stBlue.bCompIgnore = HI_FALSE;
    stOpt.unColorKeyValue.struCkARGB.stBlue.bCompOut = HI_FALSE;
    stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMin = 0x80;
    stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMax = 0xff;
    stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMask = 0xff;

    stOpt.unColorKeyValue.struCkARGB.stGreen.bCompIgnore = HI_TRUE;
    stOpt.unColorKeyValue.struCkARGB.stRed.bCompIgnore = HI_TRUE;    
    stOpt.unColorKeyValue.struCkARGB.stAlpha.bCompIgnore = HI_TRUE;

    /*4.open TDE device*/
    HI_TDE2_Open();

    /*5.show source Bitmap*/
    Printf("Showing ForeGround Bitmap\n");
    Printf("Press the Enter key to show the BackGround Bitmap\n");      
    HI_TDE_ShowSurface(&stForeGround, 50, 50);
    Printf("Showing BackGround Bitmap\n");
    Printf("Press the Enter key to show the Result Bitmap\n");  
    HI_TDE_ShowSurface(&stBackGround, 50, 50);

    /*6.begin a job*/
    handle = HI_TDE2_BeginJob();

    /*7.colorkey operation*/
    HI_TDE2_Bitblit(handle, &stBackGround, &stBackGroundRect, &stForeGround, &stForeGroundRect,
                                &stDst, &stDstRect, &stOpt);

    /*8.submit job*/
    HI_TDE2_EndJob(handle, HI_FALSE, bBlock, u32TimeOut);

    /*9.show result*/
    Printf("Showing Result Bitmap\n");
    Printf("Press the Enter key to quit out\n");
    HI_TDE_ShowSurface(&stDst, 50, 50);	

    /*10.destroy source and target surface*/
    HI_TDE_DestroySurface(&stForeGround); 
    HI_TDE_DestroySurface(&stBackGround); 
    HI_TDE_DestroySurface(&stDst);        

    /*11.close TDE device*/
    HI_TDE2_Close();
    
    return HI_SUCCESS;
}

