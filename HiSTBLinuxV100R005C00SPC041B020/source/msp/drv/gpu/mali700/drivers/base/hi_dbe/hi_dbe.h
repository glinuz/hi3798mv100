/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_dbe.h
Version             : Initial Draft
Author              : 
Created             : 2015/09/29
Description         :
Function List       : 
History             :
Date                       Author                   Modification
2015/09/29                 00270170                 Created file        
******************************************************************************/

#ifndef _HI_DMA_BUF_EXPORTER_H_
#define _HI_DMA_BUF_EXPORTER_H_

#include <linux/types.h>
#include <asm/ioctl.h>

struct hidbe_ioctl_wrap
{
	__u64 dbe_phyaddr;
	__u64 dbe_size; /* size of buffer to allocate, in pages */
};

#define DBE_IOCTL_BASE 'E'

#define DBE_COMMAND_WRAP            _IOR(DBE_IOCTL_BASE, 0x00, struct hidbe_ioctl_wrap)

#endif /* _HI_DMA_BUF_EXPORTER_H_ */
