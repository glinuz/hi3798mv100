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
Description     : CNcomment:Êó±ê²Ù×÷ CNend\n
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
	 DFBResult              err;
     IDirectFB              *dfb;
     IDirectFBDisplayLayer  *layer;
	 
     DFBCHECK(DirectFBInit               (&argc, &argv)              );
     DFBCHECK(DirectFBCreate             (&dfb)                      );
     DFBCHECK(dfb->GetDisplayLayer       (dfb,DLID_PRIMARY, &layer)  );
     DFBCHECK(layer->SetCooperativeLevel (layer,DLSCL_ADMINISTRATIVE));


     layer->EnableCursor ( layer, 1 );
	 while(1){
		sleep(1);
	 }

     layer->Release(layer);
     dfb->Release(dfb);
	 
     return 0;
}
