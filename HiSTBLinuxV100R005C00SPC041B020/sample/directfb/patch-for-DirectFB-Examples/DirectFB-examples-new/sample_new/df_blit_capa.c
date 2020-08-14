/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : df_blit_capa.c
Version         : 
Author          : 
Created         : 2015/08/22
Description     : test blit capability 
                  CNcomment:测试1280 *720 分辨率下的blit性能 CNend\n
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
* description   : CNcomment: 测试Blit性能 CNend\n
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
     
	 DFBRectangle rect = { 10,10,30,30 };

     DFBSurfaceDescription dsc;
     

     DFBCHECK(DirectFBInit(NULL,NULL));

     DFBCHECK(DirectFBSetOption("bg-color", "0XFFFFFFFF"));
     
     DFBCHECK(DirectFBCreate( &dfb ));
     
     DFBCHECK(dfb->GetDeviceDescription( dfb, &gdesc ));

     DFBCHECK(dfb->GetDisplayLayer( dfb, DLID_PRIMARY, &layer ));
	  
	 /**
	  **this is very important
	  **/
     DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE ));
     DFBCHECK(layer->EnableCursor ( layer, 0));
     
     if (!((gdesc.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL) && (gdesc.blitting_flags & DSBLIT_BLEND_COLORALPHA  )))
     {
          layer_config.flags      = DLCONF_BUFFERMODE;
          layer_config.buffermode = DLBM_BACKSYSTEM;
          DFBCHECK(layer->SetConfiguration( layer, &layer_config ));
          
		  SAMPLE_TRACE("DisplayLayer Dont support Alhpa blending ...\n");
     }else{
		 SAMPLE_TRACE("DisplayLayer Support Alhpa blending ...\n");
	 }

     DFBCHECK(layer->GetConfiguration( layer, &layer_config ));
	 SAMPLE_TRACE("Layer configure : buffermode  %d ,flags 0x%x , width %d ,height %d ,\n"
	 	          "options : 0x%x , pixelformat : 0x%x,\n"
	 	          "source  : %d   , surface_caps: 0x%x  \n" ,
		          layer_config.buffermode, layer_config.flags    , layer_config.width,
		          layer_config.height    , layer_config.options  , layer_config.pixelformat,
		          layer_config.source    , layer_config.surface_caps);

     df_dec_to_surface(PIC0_PATH, SCREEN_WIDTH,   SCREEN_HEIGHT,   dfb, &Surface0);
	 df_dec_to_surface(PIC1_PATH, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, dfb, &Surface1);

	 df_clearScreen(layer);

 #if 0
	 //modify the cooperative level to admin, or the next operation may return fail.
     DFBCHECK(layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE ));
     DFBCHECK(layer->SetBackgroundImage (layer, Surface1 ));
     DFBCHECK(layer->SetBackgroundMode  (layer, DLBM_IMAGE ));
     //restore the cooperative level.
     DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_SHARED ));
#endif

	 /**
	  **create layer surface, and blit surface to layer surface,after flip to show
	  **/
	 dsc.flags  = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
     dsc.caps   = DSCAPS_PRIMARY | DSCAPS_DOUBLE;
     dsc.width  = SCREEN_WIDTH;
     dsc.height = SCREEN_HEIGHT;
     DFBCHECK(dfb->CreateSurface( dfb, &dsc, &LayerSurface ));
	 df_printSurfaceProp(LayerSurface);


//	LayerSurface->SetBlittingFlags( LayerSurface, (DSBLIT_BLEND_COLORALPHA   ));
	LayerSurface->SetBlittingFlags( LayerSurface, (DSBLIT_BLEND_ALPHACHANNEL  ));



	/** lock surface and CPU draw lines in different color and alhpa **/
	{
		unsigned int *pPixelDat ;
		int	nPitch =0 , ret = 0 , i , j ;

	 	ret = Surface1->Lock(Surface1 , (DSLF_READ |DSLF_WRITE) , (void**)&pPixelDat , &nPitch  );
		if( DR_OK != ret)
		{
			SAMPLE_TRACE("Line %d: surface lock failed %d .\n"  , __LINE__, ret);
			return	-1;
		} else {
			SAMPLE_TRACE("surface locked success ,pPixelDat %p , nPitch %d \n" , pPixelDat , nPitch);
		}

		/// draw line in different colors
		pPixelDat += (SCREEN_WIDTH / 2 )  ;
		for( i = 0 ; i < 300 ; i ++)
		{
			for( j = 0 ; j < 200 ; j++)
			{
				pPixelDat[j] = ((0+i*2)<<24) | (0x00 << 16) | (0x00<<8) | 0xff ;
			}

			pPixelDat += nPitch/COLOR_BYTES  ;
		}
		ret = Surface1->Unlock(Surface1);
	}


     while(1){

     	usleep(3*1000*1000);

		Surface0->SetColor        ( Surface0, 0x00, 0xFF, 0x00, 0xc0 );
		Surface0->FillRectangles  ( Surface0, &rect, 1);

		DF_TINIT();
		DF_TSTART();
		LayerSurface->Blit( LayerSurface, Surface0, NULL, 0, 0 );
		DF_TEND();

		rect.w += 20;
		rect.h += 20;
		rect.x += 10;
		rect.y += 10;
		if((rect.x + rect.w) > SCREEN_WIDTH){
			rect.x = 10;
			rect.w = 30;
		}
		if((rect.y + rect.h) > SCREEN_HEIGHT){
			rect.y = 10;
			rect.h = 30;
		}
		
		Surface1->SetColor      ( Surface1, 0xFF, 0x00, 0x00, (rect.y%256) );
		Surface1->FillRectangles( Surface1, &rect, 1 );
     	
        DF_TSTART();
        LayerSurface->Blit( LayerSurface, Surface1, NULL, SCREEN_WIDTH / 2 -10 , SCREEN_HEIGHT / 2-10 );
		DF_TEND();

		LayerSurface->Flip( LayerSurface, NULL,  DSFLIP_ONSYNC );
		usleep(3*1000);
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
