/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	    : jdatasrc.h
Version		    : Initial Draft
Author		    : 
Created		    : 2013/06/20
Description	    : this file is add, move the structure from jdatasrc.c to here.
                  because hard dec file of jpeg_hdec_api.c will use this struct data.
                  CNcomment: 这个文件是新增，把对码流处理的结构从jdatasrc.c文件中
                             移到这个新增的头文件，因为在jpeg_hdec_api.c使用了这个
                             结构体变量及读码流的宏 CNend\n
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2013/06/20		    y00181162  		    Created file      	
******************************************************************************/
#ifndef __JDATASRC_H__
#define __JDATASRC_H__


/*********************************add include here******************************/

#include "jinclude.h"

/*****************************************************************************/


#ifdef __cplusplus
      #if __cplusplus
   
extern "C" 
{

      #endif
#endif /* __cplusplus */

    /***************************** Macro Definition ******************************/

    #define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */

    /*************************** Structure Definition ****************************/

	typedef struct
    {
    
		  char* img_buffer;         /** the user's input stream buffer     **/
		  int buffer_size;          /** the stream buffer size             **/
		  unsigned int pos;          /** the stream read pos                **/
          JOCTET EndMarkBuf[2];     /** the data that the end of jpeg file **/
		  
	}BUFF_JPG;


	/** Expanded data source object for stdio input */
	typedef struct 
    {
    
		  struct jpeg_source_mgr pub;	/* public fields */
		  union
		  {
		    BUFF_JPG stBufStream;		/* jpeg image buffer */
		    FILE * infile;		        /* source stream */
		  };

		  JOCTET * buffer;		        /* start of buffer */
		  boolean start_of_file;	    /* have we gotten any data yet? */
	  
	} my_source_mgr;

	typedef my_source_mgr * my_src_ptr;

    /***************************  The enum of Jpeg image format  ******************/


    /********************** Global Variable declaration **************************/

   

    /******************************* API declaration *****************************/


    #ifdef __cplusplus
    
        #if __cplusplus


      
}
      
        #endif
        
   #endif /* __cplusplus */

#endif /* __JDATASRC_H__*/
