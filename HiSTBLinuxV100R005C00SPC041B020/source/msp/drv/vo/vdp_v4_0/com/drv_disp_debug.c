
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_debug.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/fs.h>
#include <linux/mm.h>

#include <asm/uaccess.h>


#include "drv_disp_debug.h"
#include "drv_disp_com.h"
#include "hi_drv_win.h"
#include "drv_window.h"
#include "hi_osal.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


int vdp_str2val(char *str, unsigned int *data)
{
    unsigned int i, d, dat, weight;

    dat = 0;
    if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        i = 2;
        weight = 16;
    }
    else
    {
        i = 0;
        weight = 10;
    }

    for(; i < 10; i++)
    {
        if(str[i] < 0x20)
        {
            break;
        }
        else if (weight == 16 && str[i] >= 'a' && str[i] <= 'f')
        {
            d = str[i] - 'a' + 10;
        }
        else if (weight == 16 && str[i] >= 'A' && str[i] <= 'F')
        {
            d = str[i] - 'A' + 10;
        }
        else if (str[i] >= '0' && str[i] <= '9')
        {
            d = str[i] - '0';
        }
        else
        {
            return -1;
        }

        dat = dat * weight + d;
    }

    *data = dat;

    return 0;
}



/************************************************************************/
/* file tell position                                                    */
/************************************************************************/
struct file *vdp_k_fopen(const char *filename, int flags, int mode)
{
    struct file *filp = filp_open(filename, flags, mode);
    return (IS_ERR(filp)) ? NULL : filp;
}

void vdp_k_fclose(struct file *filp)
{
    if (filp)
    {
        filp_close(filp, NULL);
    }
}

int vdp_k_fread(char *buf, unsigned int len, struct file *filp)
{
        int readlen;
        mm_segment_t oldfs;

        if (filp == NULL)
                return -ENOENT;
        if (filp->f_op->read == NULL)
                return -ENOSYS;
        if (((filp->f_flags & O_ACCMODE) & (O_RDONLY | O_RDWR)) == 0)
                return -EACCES;

        oldfs = get_fs();

        set_fs(KERNEL_DS);

        readlen = filp->f_op->read(filp, buf, len, &filp->f_pos);

        set_fs(oldfs);

        return readlen;
}

int vdp_k_fwrite(char *buf, int len, struct file *filp)
{
        int writelen;
        mm_segment_t oldfs;

        if (filp == NULL)
                return -ENOENT;
        if (filp->f_op->write == NULL)
                return -ENOSYS;
        if (((filp->f_flags & O_ACCMODE) & (O_WRONLY | O_RDWR)) == 0)
                return -EACCES;
        oldfs = get_fs();
        set_fs(KERNEL_DS);
        writelen = filp->f_op->write(filp, buf, len, &filp->f_pos);
        set_fs(oldfs);

        return writelen;
}

static  HI_U32 Caculate_frame_size( HI_U32 y_stride,
	                                HI_U32 u32width,
	                                HI_U32 u32height,
	                                HI_DRV_PIX_FORMAT_E ePixFormat)
{
	HI_U32 datalen = 0;

	/*don't know why there exists these two branches, do we condidered gstreamer?*/
    if ( HI_DRV_PIX_FMT_NV21 == ePixFormat
        || HI_DRV_PIX_FMT_NV12_TILE_CMP == ePixFormat
        || HI_DRV_PIX_FMT_NV21_TILE_CMP == ePixFormat)
    {
        datalen = u32height * y_stride * 3 / 2 + u32height * 4;
    }
    else if ( HI_DRV_PIX_FMT_NV21_CMP == ePixFormat)
    {
        datalen = u32height * y_stride * 3 / 2 + 16 * u32height * 3/2;
    }
    else if ( HI_DRV_PIX_FMT_NV61_2X1 == ePixFormat)
    {
        datalen = u32height * y_stride * 2 + u32height * 4;
    }
    else
    {
        WIN_ERROR("Invalid Format %d!\n",ePixFormat);
        return 0;
    }

	return datalen;
}

static HI_S32  Mmap_frame_virtual_addr(DISP_MMZ_BUF_S *pstMBuf,
	                                          HI_U32 y_stride,
            	                              HI_U32 u32width,
            	                              HI_U32 u32height,
                                              HI_DRV_PIX_FORMAT_E ePixFormat,
                                              HI_U32 u32PhyAddr_Y)
{
	HI_U32 datalen = 0;
	HI_S32 nRet = HI_FAILURE;

    
	datalen = Caculate_frame_size(y_stride,u32width, u32height, ePixFormat);
#ifdef  CFG_VDP_MMU_SUPPORT
	pstMBuf->bSmmu = HI_TRUE;
#else
    pstMBuf->bSmmu = HI_FALSE;
#endif

	pstMBuf->u32Size = datalen;
    pstMBuf->u32StartPhyAddr = u32PhyAddr_Y;
	
    if (!pstMBuf->u32StartPhyAddr)
    {
        HI_ERR_WIN("address '0x%x' is null!\n", u32PhyAddr_Y);
        return HI_FAILURE;
    }
	
    printk("========================map phy addr =0x%x\n", pstMBuf->u32StartPhyAddr);
    nRet = DISP_OS_MMZ_Map(pstMBuf);
    if (nRet)
    {
        HI_ERR_WIN("address '0x%x' is not valid!\n", u32PhyAddr_Y);
        return HI_FAILURE;
    }

	return HI_SUCCESS;
	
}

static HI_VOID  Unmap_frame_virtual_addr(DISP_MMZ_BUF_S *pstMBuf)
{
     DISP_OS_MMZ_UnMap(pstMBuf);
	 return;
}

HI_S32 vdp_k_SaveYUVImg(struct file *pfYUV, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_S32 num)
{
    DISP_MMZ_BUF_S stMBuf;
    HI_U8 *ptr;
    HI_U8 *pu8Udata;
    HI_U8 *pu8Vdata;
    HI_U8 *pu8Ydata;
    HI_S32 nRet = HI_SUCCESS;
	HI_U32 y_stride = 0,c_stride = 0,u32height = 0, u32width = 0;
	HI_U32 u32PhyAddr_Y = 0;
	HI_DRV_PIX_FORMAT_E ePixFormat = HI_DRV_PIX_BUTT;
	
    if ((!pstFrame) || (!pfYUV) )
    {
        HI_ERR_WIN("pstFrame is null!\n");
        return HI_FAILURE;
    }

    y_stride    = pstFrame->stBufAddr[0].u32Stride_Y;
	c_stride    = pstFrame->stBufAddr[0].u32Stride_C;
	u32height   = pstFrame->u32Height;
    u32width    = pstFrame->u32Width;
	ePixFormat  = pstFrame->ePixFormat;
	u32PhyAddr_Y = pstFrame->stBufAddr[0].u32PhyAddr_Y;
	
    memset((void*)&stMBuf, 0, sizeof(MMZ_BUFFER_S));
	
	if ((nRet = Mmap_frame_virtual_addr(&stMBuf, 
		                   y_stride,
		                   u32width,
		                   u32height,
		                   ePixFormat,
		                   u32PhyAddr_Y)))
	{
		return nRet;
	}

	ptr = stMBuf.pu8StartVirAddr;
	
    pu8Udata = (HI_U8 *)DISP_MALLOC(u32width * u32height / 2 /2);
    if (!pu8Udata)
    {
        HI_ERR_WIN("MALLOC U data fail.\n");
        goto EXIT3;
    }
    
    pu8Vdata = (HI_U8 *)DISP_MALLOC(u32width * u32height / 2 /2);
    if ( !pu8Vdata)
    {
        HI_ERR_WIN("MALLOC V data fail.\n");
        goto EXIT2;
    }
    
    pu8Ydata = (HI_U8 *)DISP_MALLOC(y_stride);
    if (!pu8Ydata)
    {
        HI_ERR_WIN("MALLOC Y data fail.\n");
        goto EXIT1;
    }
	
#if 1
    {
        HI_U32 i,j;
    		/*write Y*/
        for (i=0; i< u32height; i++)
        {
            memcpy(pu8Ydata, ptr, sizeof(HI_U8)*y_stride);
    
            if(u32width != vdp_k_fwrite(pu8Ydata, u32width, pfYUV))
            {
                HI_ERR_WIN("line %d: fwrite fail!\n",__LINE__);
            }
            ptr += y_stride;
        }
    
        /* U V transfer and save */
        for (i=0; i<u32height/2; i++)
        {
            for (j=0; j<u32width/2; j++)
            {
                if(ePixFormat == HI_DRV_PIX_FMT_NV21)
                {
                    pu8Vdata[i*u32width/2+j] = ptr[2*j];
                    pu8Udata[i*u32width/2+j] = ptr[2*j+1];
                }
                else
                {
                    pu8Udata[i*u32width/2+j] = ptr[2*j];
                    pu8Vdata[i*u32width/2+j] = ptr[2*j+1];
                }
            }
            ptr += c_stride;
        }
    
        /*write U */
        vdp_k_fwrite(pu8Udata, u32width * u32height / 2 /2, pfYUV);
    
        /*write V */
        vdp_k_fwrite(pu8Vdata, u32width * u32height / 2 /2, pfYUV);
    }
    
#else
    {
		HI_U32 datalen = 0;		
    	datalen = Caculate_frame_size(y_stride,
            		                  u32width,
            		                  u32height,
            		                  ePixFormat);
		
        if(datalen != vdp_k_fwrite(ptr, datalen, pfYUV))
        {
			HI_ERR_WIN("line %d: fwrite fail!\n",__LINE__);
        }    
    }

#endif

    
    DISP_FREE(pu8Ydata);
EXIT1:
    DISP_FREE(pu8Vdata);
EXIT2:
    DISP_FREE(pu8Udata);
EXIT3:
	
    Unmap_frame_virtual_addr(&stMBuf);
    return HI_SUCCESS;
}



static HI_CHAR u8VdpDebugStr[256];
static HI_CHAR s_VdpSavePath[64] = {'/','m','n','t',0};

HI_S32 vdp_DebugSaveYUVImg(HI_DRV_VIDEO_FRAME_S *pstCurFrame, HI_CHAR *buffer, HI_U32 count)
{
    struct file *pfYUV;
    int i,j;
    HI_S32 nRet = HI_SUCCESS;
    struct tm now;

    time_to_tm(get_seconds(), 0, &now);

    i = 0;
    j = 0;

    for(; i < count; i++)
    {
        if(j==0 && buffer[i]==' ')continue;
        if(buffer[i] > ' ')u8VdpDebugStr[j++] = buffer[i];
        if(j>0 && buffer[i]<=' ')break;
    }

    u8VdpDebugStr[j] = 0;

    if (u8VdpDebugStr[0] == '/')
    {
        if(u8VdpDebugStr[j-1] == '/')
        {
            u8VdpDebugStr[j-1] = 0;
        }
        printk("******* VDP save path: %s ********\n", u8VdpDebugStr);
        HI_OSAL_Strncpy(s_VdpSavePath, u8VdpDebugStr, j);
    }

    // open file in kernel
    HI_OSAL_Snprintf(u8VdpDebugStr, 256, "%s/vdp_0x%x_%d_%d_%02u_%02u_%02u.yuv", s_VdpSavePath,
                                          pstCurFrame->stBufAddr[0].u32PhyAddr_Y,
                                          pstCurFrame->u32Width,
                                          pstCurFrame->u32Height,
                                          now.tm_hour,
                                          now.tm_min,
                                          now.tm_sec);

    pfYUV = vdp_k_fopen(u8VdpDebugStr, O_RDWR|O_CREAT|O_APPEND, 0);

    if (pfYUV)
    {
        // save yuv data
        nRet = vdp_k_SaveYUVImg(pfYUV, pstCurFrame, 1);
        vdp_k_fclose(pfYUV);
        HI_PRINT("save image '%s' = 0x%x\n", u8VdpDebugStr, nRet);
    }
    else
    {
        HI_ERR_WIN("open file '%s' fail!\n", u8VdpDebugStr);
        nRet = HI_FAILURE;
    }

    return nRet;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */


