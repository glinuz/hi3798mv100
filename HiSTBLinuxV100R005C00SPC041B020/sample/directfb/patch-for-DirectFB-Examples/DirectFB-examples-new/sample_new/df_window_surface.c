/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : df_cursor.c
Version         : 
Author          : 
Created         : 2015/08/22
Description     : CNcomment:鼠标操作 CNend\n
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
int main( int argc, char *argv[] )
{
	 DFBResult               err;
     IDirectFB              *dfb;
     IDirectFBDisplayLayer  *layer;

     DFBWindowDescription    window_desc;
	 DFBFontDescription      font_desc;
	 DFBSurfaceDescription   surface_desc;
	 
     IDirectFBImageProvider *provider;

     IDirectFBSurface       *bgsurface;

     IDirectFBWindow        *window1;
     IDirectFBWindow        *window2;
     IDirectFBSurface       *window_surface1;
     IDirectFBSurface       *window_surface2;
     IDirectFBFont          *font;

     DFBDisplayLayerConfig         layer_config;
     DFBGraphicsDeviceDescription  gdesc;

     int fontheight;

	 
     DFBCHECK(DirectFBInit               (&argc, &argv)              );
     DFBCHECK(DirectFBCreate             (&dfb)                      );
     DFBCHECK(dfb->GetDeviceDescription  (dfb, &gdesc )              );
     DFBCHECK(dfb->GetDisplayLayer       (dfb,DLID_PRIMARY, &layer)  );
     DFBCHECK(layer->SetCooperativeLevel (layer,DLSCL_ADMINISTRATIVE));
     
     

     if(!((gdesc.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL) && (gdesc.blitting_flags & DSBLIT_BLEND_COLORALPHA  )))
     {
          layer_config.flags      = DLCONF_BUFFERMODE;
          layer_config.buffermode = DLBM_BACKSYSTEM;
          DFBCHECK(layer->SetConfiguration(layer, &layer_config));
     }

     DFBCHECK(layer->GetConfiguration  (layer, &layer_config ));
     DFBCHECK(layer->EnableCursor      (layer, 1 ));

	 /**
	  **font
	  **/
	 memset(&font_desc,0,sizeof(DFBSurfaceDescription));
     font_desc.flags    = DFDESC_HEIGHT;
     font_desc.height   = layer_config.width / 50;
     
     DFBCHECK( dfb->CreateFont ( dfb, FONT,&font_desc, &font));
     DFBCHECK( font->GetHeight ( font, &fontheight ));

     /**
      **创建普通surface
      **/
	 memset(&surface_desc,0,sizeof(DFBSurfaceDescription));

	 DFBCHECK(dfb->CreateImageProvider       (dfb,PIC2_PATH,&provider ));
     DFBCHECK(provider->GetSurfaceDescription( provider, &surface_desc ));
     
     surface_desc.flags |= DSDESC_WIDTH | DSDESC_HEIGHT;
     surface_desc.width  = layer_config.width;
     surface_desc.height = layer_config.height;
     DFBCHECK( dfb->CreateSurface( dfb, &surface_desc, &bgsurface ));

	 /**
      **将该surface设置为背景色
      **/
     DFBCHECK( provider->RenderTo( provider, bgsurface, NULL ));
     DFBCHECK( provider->Release ( provider ));
     
     DFBCHECK( bgsurface->SetFont( bgsurface, font ));

     DFBCHECK( bgsurface->SetColor  ( bgsurface, 0xCF, 0xCF, 0xFF, 0xFF ));
     DFBCHECK( bgsurface->DrawString( bgsurface,
                             "Move the mouse over a window to activate it.",
                             -1, 0, 0, DSTF_LEFT | DSTF_TOP ));

     DFBCHECK( bgsurface->SetColor(   bgsurface, 0xCF, 0xDF, 0xCF, 0xFF ));
     DFBCHECK( bgsurface->DrawString( bgsurface,
                             "Press left mouse button and drag to move the window.",
                             -1, 0, fontheight, DSTF_LEFT | DSTF_TOP ));

     DFBCHECK( bgsurface->SetColor( bgsurface, 0xCF, 0xEF, 0x9F, 0xFF ));
     DFBCHECK( bgsurface->DrawString( bgsurface,
                             "Press middle mouse button to raise/lower the window.",
                             -1, 0, fontheight * 2, DSTF_LEFT | DSTF_TOP ));

     DFBCHECK( bgsurface->SetColor( bgsurface, 0xCF, 0xFF, 0x6F, 0xFF ));
     DFBCHECK( bgsurface->DrawString( bgsurface,
                             "Press right mouse button when you are done.", -1,
                             0, fontheight * 3,
                             DSTF_LEFT | DSTF_TOP ));

     layer->SetBackgroundImage( layer, bgsurface );
     layer->SetBackgroundMode ( layer, DLBM_IMAGE );
	 
	 /**
      **创建window1
      **/
     memset(&window_desc,0,sizeof(DFBWindowDescription));
	 

     window_desc.flags = (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);

     window_desc.caps   = DWCAPS_ALPHACHANNEL;
     window_desc.posx   = 20;
     window_desc.posy   = 120;
     window_desc.width  = 300;
     window_desc.height = 200;

     layer->CreateWindow( layer,   &window_desc, &window1 );
     window1->GetSurface( window1, &window_surface1 );
     window1->SetOpacity( window1, 0xFF );

     window_surface1->SetColor      ( window_surface1, 0x00, 0x30, 0x10,                0xc0 );
     window_surface1->DrawRectangle ( window_surface1, 0,    0,    window_desc.width,   window_desc.height );
     window_surface1->SetColor      ( window_surface1, 0x80, 0xa0, 0x00,                 0x90 );
     window_surface1->FillRectangle ( window_surface1, 1,    1,    window_desc.width-2, window_desc.height-2 );
     window_surface1->Flip          ( window_surface1, NULL, 0 );

     /**
      **创建window2
      **/
     memset(&window_desc,0,sizeof(DFBWindowDescription));

     window_desc.flags  = ( DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS );
     window_desc.posx   = 200;
     window_desc.posy   = 200;
     window_desc.width  = 512;
     window_desc.height = 145;
     window_desc.caps   = DWCAPS_ALPHACHANNEL;

     DFBCHECK( layer->CreateWindow( layer,   &window_desc, &window2 ) );
     DFBCHECK( window2->GetSurface( window2, &window_surface2 ));

     DFBCHECK( dfb->CreateImageProvider( dfb,      PIC3_PATH,       &provider ));
     DFBCHECK( provider->RenderTo      ( provider, window_surface2, NULL ));
	 DFBCHECK( provider->Release       ( provider ));
	 
     DFBCHECK( window_surface2->SetColor      ( window_surface2, 0xFF, 0x20, 0x20,              0x90 ));
     DFBCHECK( window_surface2->DrawRectangle ( window_surface2, 0,    0,    window_desc.width, window_desc.height ));
     DFBCHECK( window_surface2->Flip          ( window_surface2, NULL, 0 ));
	 DFBCHECK( window2->SetOpacity( window2, 0xFF ));

	 while(1){
		sleep(1);
	 }
	 
     font->Release              ( font );
     window_surface2->Release   ( window_surface2 );
     window_surface1->Release   ( window_surface1 );
     window2->Release           ( window2 );
     window1->Release           ( window1 );
     layer->Release             ( layer );
     bgsurface->Release         ( bgsurface );
     dfb->Release               ( dfb );

     return 0;
}
