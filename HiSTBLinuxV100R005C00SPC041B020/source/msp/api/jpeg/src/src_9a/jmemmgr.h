/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	    : jmemmgr.h
Version		    : Initial Draft
Author		    : 
Created		    : 2013/06/20
Description	    : use the sdk alloc mem add structure file
                  CNcomment: 使用SDK分配内存增加得结构体头文件,将jmemmgr.c中得结构体
                             挪动这个头文件来 CNend\n
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2013/06/20		    y00181162  		    Created file      	
******************************************************************************/
#ifndef __JMEMMGR_H__
#define __JMEMMGR_H__



/*********************************add include here******************************/

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" 
{
#endif
#endif /* __cplusplus */



    /***************************** Macro Definition ******************************/

   #ifndef ALIGN_TYPE		/* so can override from jconfig.h */
   #define ALIGN_TYPE  double
   #endif
    /*************************** Structure Definition ****************************/

   typedef union large_pool_struct FAR * large_pool_ptr;

    typedef union large_pool_struct {
      struct {
        large_pool_ptr next;	/* next in list of pools */
        size_t bytes_used;		/* how many bytes already used within pool */
        size_t bytes_left;		/* bytes still available in this pool */

        size_t PhysAddr;
            
      } hdr;
      ALIGN_TYPE dummy;		/* included in union to ensure alignment */
    } large_pool_hdr;


    /********************** Global Variable declaration **************************/



    /******************************* API declaration *****************************/



    /****************************************************************************/

#ifdef __cplusplus
#if __cplusplus   
}
      
#endif
#endif /* __cplusplus */

#endif /* __JMEMMGR_H__ */
