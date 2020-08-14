/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : df_logo_to_app.c
Version         : 
Author          : 
Created         : 2015/08/22
Description     : 开机logo平滑过渡
Function List   :
History         :
Date                        Author                  Modification
2015/08/22                  y00181162               
******************************************************************************/


/*********************************add include here******************************/
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "hi_adp_gfx.h"


/***************************** Macro Definition ******************************/

#define DFBCHECK(x...) \
     {                                                                \
          err = x;                                                    \
          if (err != DFB_OK) {                                        \
               SAMPLE_TRACE("%s <%d>:\n\t", __FILE__, __LINE__ );     \
               DirectFBErrorFatal( #x, err );                         \
          }                                                           \
     }

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/


/******************************* API forward declarations *******************/

/******************************* API realization *****************************/
static int dfbShowPicture(const char* pcPicPath, IDirectFB *dfb, IDirectFBDisplayLayer  *layer)
{
	DFBDisplayLayerConfig         layer_config;
	IDirectFBImageProvider *provider;
	DFBResult err = DFB_FAILURE;
    DirectResult nRe = DR_FAILURE;
	DFBSurfaceDescription surfaceDesc;
    IDirectFBSurface *backgroundSurface;
    
    if(dfb == NULL || layer == NULL){
        SAMPLE_TRACE("QT must be inited at first\n");
        return -4;
    }
    
    if(pcPicPath == NULL){
        SAMPLE_TRACE("FATAL ERROR: dfbShowPicture > picture path is NULL\n");
        return -3;
    }
    
  
    DFBCHECK(layer->GetConfiguration( layer, &layer_config ));  
   
    err = dfb->CreateImageProvider( dfb, pcPicPath, &provider );
    if(DFB_OK != err ){
        SAMPLE_TRACE("Error: CreateImageProvider failed, ret =  %d\n", err);
        return -1;
    }

    DFBCHECK(provider->GetSurfaceDescription( provider, &surfaceDesc ));

    surfaceDesc.flags |= DSDESC_CAPS;
    surfaceDesc.caps   = DSCAPS_SHARED;
    surfaceDesc.width  = layer_config.width;
    surfaceDesc.height = layer_config.height;

	 
    //create a surface, not a window, to avoid a black slash screen when create a window.
    err = dfb->CreateSurface( dfb, &surfaceDesc, &backgroundSurface );
    if(DFB_OK != err ){
        SAMPLE_TRACE("Error: CreateSurface failed, ret =  %d\n", err);
        provider->Release( provider );
        provider = NULL;
        return -1;
    }

    DFBCHECK(provider->RenderTo( provider, backgroundSurface, NULL ));

	 
    //modify the cooperative level to admin, or the next operation may return fail.
    DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE ));
    DFBCHECK(layer->SetBackgroundImage ( layer, backgroundSurface ));
    DFBCHECK(layer->SetBackgroundMode  ( layer, DLBM_IMAGE ));
    //restore the cooperative level.
    DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_SHARED ));
	
    nRe = provider->Release( provider );
    if(DR_OK != nRe){
        SAMPLE_TRACE("Error: ImageProvider release failed, ret =  %d\n", nRe);
    }
    
    nRe = backgroundSurface->Release( backgroundSurface );
    if(DR_OK != nRe){
        SAMPLE_TRACE("Error: Surface release failed, ret =  %d\n", nRe);
    }

    return 0;
}



/*****************************************************************************
* func          : main
* description   : CNcomment: 开机logo平滑过渡 CNend\n
* param[in]     : CNcomment: CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
int main( int argc, char *argv[] )
{
     IDirectFB              *dfb    = NULL;
     IDirectFBDisplayLayer  *layer  = NULL;

     DFBDisplayLayerConfig         layer_config;
     DFBGraphicsDeviceDescription  gdesc;
     
     int err;


     DFBCHECK(DirectFBInit(NULL,NULL));

	 /**
	  **要想平滑过渡，首先不要设置图层背景
	  **/
	 DFBCHECK(DirectFBSetOption("bg-none", NULL));
     //DFBCHECK(DirectFBSetOption("bg-color", "0XFFFFFFFF"));

     //SAMPLE_TRACE("=====%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
     //getchar();
     DFBCHECK(DirectFBCreate( &dfb ));
     
	 //SAMPLE_TRACE("=====%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
     //getchar();
     dfb->GetDeviceDescription( dfb, &gdesc );

	 //SAMPLE_TRACE("=====%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
     //getchar();
     DFBCHECK(dfb->GetDisplayLayer( dfb, DLID_PRIMARY, &layer ));

	 //SAMPLE_TRACE("=====%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
     //getchar();
     layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE );

	 //SAMPLE_TRACE("=====%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
     //getchar();
     layer->GetConfiguration( layer, &layer_config );

	 //SAMPLE_TRACE("=====%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
     //getchar();
	 layer->EnableCursor ( layer, 0 );

	 SAMPLE_TRACE(">>>>>>>>>>>>>>begin to app >>>>>>>>>>>>>>>\n");
     getchar();
     
     dfbShowPicture(PIC0_PATH, dfb, layer);

	 SAMPLE_TRACE(">>>>>>>>>>>>>> show next ui >>>>>>>>>>>>>>>\n");
     getchar();
	 
     usleep(3*1000*1000);

     df_clearScreen(layer);
	 
     dfbShowPicture(PIC1_PATH, dfb, layer);

     usleep(3*1000*1000);
	 	 
     while(1){
     	usleep(1000);
     }


     layer->Release( layer );
     dfb->Release( dfb );

	 
     return 0;

}
