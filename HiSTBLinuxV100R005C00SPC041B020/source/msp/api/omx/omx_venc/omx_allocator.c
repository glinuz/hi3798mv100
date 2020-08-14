#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "omx_dbg.h"

#include "omx_allocator.h"

#include "hi_common.h"

static char ETB_MMZ_name_table[][16]={"OMXVENC_ETB_1","OMXVENC_ETB_2","OMXVENC_ETB_3","OMXVENC_ETB_4","OMXVENC_ETB_5","OMXVENC_ETB_6"};
static char FTB_MMZ_name_table[][16]={"OMXVENC_FTB_1","OMXVENC_FTB_2","OMXVENC_FTB_3","OMXVENC_FTB_4","OMXVENC_FTB_5","OMXVENC_FTB_6",
	                                  "OMXVENC_FTB_7","OMXVENC_FTB_8","OMXVENC_FTB_9","OMXVENC_FTB_10","OMXVENC_FTB_11","OMXVENC_FTB_12",
	                                  "OMXVENC_FTB_13","OMXVENC_FTB_14","OMXVENC_FTB_15","OMXVENC_FTB_16","OMXVENC_FTB_17","OMXVENC_FTB_18",
	                                  "OMXVENC_FTB_19","OMXVENC_FTB_20"};
static OMX_S32 etb_mmz_cnt = 0;
static OMX_S32 ftb_mmz_cnt = 0;

OMX_S32 alloc_contigous_buffer(OMX_U32 buf_size, OMX_U32 align,venc_user_info *pvenc_buf)
{
    HI_MMZ_BUF_S stBuf;
    OMX_S32 ret;
    venc_user_info *puser_buf = pvenc_buf;
    if (!puser_buf)
	{
		DEBUG_PRINT_ERROR("%s() invalid param\n", __func__);
		return -1;
	}
    buf_size = (buf_size + align - 1) & ~(align - 1);
    if ( PORT_DIR_INPUT == puser_buf->user_buf.dir )
    {
       snprintf(stBuf.bufname,16,"%s",ETB_MMZ_name_table[etb_mmz_cnt++]);
    }
    else
    {
       snprintf(stBuf.bufname,16,"%s",FTB_MMZ_name_table[ftb_mmz_cnt++]);
    }
  
    stBuf.bufsize = buf_size; 
    
    ret = HI_MMZ_Malloc(&stBuf);
    if(0 != ret)
    {
        DEBUG_PRINT("~~~~~~ERROR: HI_MMZ_Malloc Failed!! Ret:%ld\n",ret);
        return -1;
    }
	puser_buf->ion_handle     = NULL;
	puser_buf->bufferaddr	  = stBuf.user_viraddr;
    puser_buf->user_buf.bufferaddr_Phy = stBuf.phyaddr;
    puser_buf->user_buf.buffer_size    = stBuf.bufsize;
    
    puser_buf->user_buf.data_len  	  = 0;
    puser_buf->user_buf.offset	      = 0;
    puser_buf->user_buf.pmem_fd	      = 0;
    puser_buf->user_buf.mmaped_size	  = stBuf.bufsize;
	/*DEBUG_PRINT("%s() , PHYaddr: %p, bufsize: %d\n",
			__func__, puser_buf->bufferaddr_Phy, puser_buf->buffer_size);*/
	return 0;

}

void free_contigous_buffer(venc_user_info*puser_buf)

{
    HI_MMZ_BUF_S stBuf;
    OMX_S32 ret;

	if (!puser_buf)
	{
		DEBUG_PRINT_ERROR("%s() invalid param\n", __func__);
		return;
	}
    stBuf.user_viraddr = puser_buf->bufferaddr;
    stBuf.phyaddr      = puser_buf->user_buf.bufferaddr_Phy; 
	stBuf.bufsize      = puser_buf->user_buf.buffer_size;

    ret = HI_MMZ_Free(&stBuf);
    if( 0 != ret)
    {
        DEBUG_PRINT_ERROR("~~~~~~ERROR: HI_MMZ_Free (%s) Failed!! Ret:%ld\n",stBuf.bufname,ret);
    }

    return ;  
}


