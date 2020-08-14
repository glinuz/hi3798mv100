/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_config.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                                 Created file        
******************************************************************************/
#ifndef  __TDE_CONFIG_H__
#define  __TDE_CONFIG_H__


#ifdef __cplusplus
      #if __cplusplus
extern "C"
{
      #endif
#endif /* __cplusplus */


#ifdef HI_ADVCA_FUNCTION_RELEASE
#define CONFIG_TDE_VERSION_DISABLE
#define CONFIG_TDE_DEBUG_DISABLE
#endif

#if (0 == HI_PROC_SUPPORT)
#define CONFIG_TDE_PROC_DISABLE
#endif

#ifdef __cplusplus
        #if __cplusplus
}
        #endif
   #endif /* __cplusplus */
#endif /*__TDE_CONFIG_H__ */
