/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : df_layer_surface.c
Version         : 
Author          : 
Created         : 2015/08/22
Description     : one canvas surface, layer surface get from layer and disable cursor
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
#define SCREEN_720_WIDTH         1280
#define SCREEN_576_HEIGHT        720
#define JPEG_PIC_PATH	         "./../share/directfb-examples/res/logo.jpg"


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
/*****************************************************************************
* func          : main
* description   : CNcomment: ¶àsurfaceµþ¼Ó CNend\n
* param[in]     : CNcomment: CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
int main( int argc, char *argv[] )
{
	 int err        = 0;
     IDirectFB              *dfb    = NULL;
     IDirectFBDisplayLayer  *layer  = NULL;
	 DirectResult nRe               = DR_FAILURE;
     DFBDisplayLayerConfig         layer_config;
     DFBGraphicsDeviceDescription  gdesc;
     IDirectFBSurface *Surface0     = NULL;
     IDirectFBSurface *LayerSurface = NULL;


     DFBCHECK(DirectFBInit(NULL,NULL));
	 
	 DFBCHECK(DirectFBSetOption("bg-none", NULL));
   

     DFBCHECK(DirectFBCreate( &dfb ));

     DFBCHECK(dfb->GetDeviceDescription( dfb, &gdesc ));

     DFBCHECK(dfb->GetDisplayLayer( dfb, DLID_PRIMARY, &layer ));

     DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE ));

     
     DFBCHECK(layer->GetConfiguration( layer, &layer_config ));
     
	 DFBCHECK(layer->EnableCursor ( layer, 0 ));

     
     df_dec_to_surface(JPEG_PIC_PATH, SCREEN_720_WIDTH, SCREEN_576_HEIGHT, dfb, &Surface0);


	 df_clearScreen(layer);
 
	 /**
	  **create layer surface, and blit surface to layer surface,after flip to show
	  **/
     DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE ));
     DFBCHECK(layer->GetSurface (layer, &LayerSurface));

     while(1){
     	usleep(3*1000*1000);
     	DF_TINIT();
		DF_TSTART();
		LayerSurface->Blit( LayerSurface, Surface0, NULL, 0, 0 );
		DF_TEND();
		DF_TSTART();
		LayerSurface->Flip( LayerSurface, NULL,DSFLIP_WAITFORSYNC);
		DF_TEND();
     }

     usleep(3*1000*1000);
	 SAMPLE_TRACE(">>>>>>>>>>>>>> release >>>>>>>>>>>>>>>\n");
	 
	 if(NULL != Surface0){
     	nRe = Surface0->Release( Surface0 );
	 }
     if(DR_OK != nRe){
        SAMPLE_TRACE("Error: Surface release failed, ret =  %d\n", nRe);
     }
     
     if(DR_OK != nRe){
        SAMPLE_TRACE("Error: Surface release failed, ret =  %d\n", nRe);
     }

     if(NULL != LayerSurface){
		nRe = LayerSurface->Release( LayerSurface );
     }
     if(DR_OK != nRe){
        SAMPLE_TRACE("Error: Surface release failed, ret =  %d\n", nRe);
     }
     
     layer->Release( layer );
     dfb->Release( dfb );

     return 0;
}
