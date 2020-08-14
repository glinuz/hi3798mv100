/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : df_flip_capa.c
Version         : 
Author          : 
Created         : 2015/08/22
Description     : CNcomment:测试1280 * 720分辨率下的flip性能 CNend\n
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
/*****************************************************************************
* func          : main
* description   : CNcomment: 测试Flip性能 CNend\n
* param[in]     : CNcomment: 可以带入参数 --dfb:help等类型CNend\n
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
     IDirectFBSurface *Surface1     = NULL;
     IDirectFBSurface *LayerSurface = NULL;

     DFBCHECK(DirectFBInit(NULL,NULL));

     DFBCHECK(DirectFBSetOption("bg-color", "0XFFFFFFFF"));
     
     DFBCHECK(DirectFBCreate( &dfb ));

     DFBCHECK(dfb->GetDeviceDescription( dfb, &gdesc ));

     DFBCHECK(dfb->GetDisplayLayer( dfb, DLID_PRIMARY, &layer ));

     DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE ));

     if (!((gdesc.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL) && (gdesc.blitting_flags & DSBLIT_BLEND_COLORALPHA  )))
     {
          layer_config.flags      = DLCONF_BUFFERMODE;
          layer_config.buffermode = DLBM_BACKSYSTEM;
          DFBCHECK(layer->SetConfiguration( layer, &layer_config ));
     }
     
     DFBCHECK(layer->GetConfiguration( layer, &layer_config ));
     
	 DFBCHECK(layer->EnableCursor ( layer, 1 ));

     
     df_dec_to_surface(PIC0_PATH, SCREEN_WIDTH,   SCREEN_HEIGHT,   dfb, &Surface0);
	 df_dec_to_surface(PIC1_PATH, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, dfb, &Surface1);

	 df_clearScreen(layer);

	 DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE ));
	 DFBCHECK(layer->GetSurface (layer, &LayerSurface));
	 
     while(1){
     	usleep(3*1000*1000);
	 	
		LayerSurface->Blit( LayerSurface, Surface0, NULL, 0, 0 );
		LayerSurface->Blit( LayerSurface, Surface1, NULL, 200, 200 );
		
		DF_TINIT();
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

     if(NULL != Surface1){
     	nRe = Surface1->Release( Surface1 );
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
