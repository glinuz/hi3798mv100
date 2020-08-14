
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


//#define VDP_DEBUG_USE_IOREMAP 1

HI_S32 vdp_k_SaveYUVImg(struct file *pfYUV, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_S32 num)
{
    MMZ_BUFFER_S stMBuf;
    HI_U8 *ptr;
    HI_U8 *pu8Udata;
    HI_U8 *pu8Vdata;
    HI_U8 *pu8Ydata;
    HI_U32 i,j;
    HI_S32 nRet = HI_SUCCESS;

    if ((!pstFrame) || (!pfYUV) )
    {
        HI_ERR_WIN("pstFrame is null!\n");
        return HI_FAILURE;
    }

    memset((void*)&stMBuf, 0, sizeof(MMZ_BUFFER_S));

    stMBuf.u32StartPhyAddr = pstFrame->stBufAddr[0].u32PhyAddr_Y;
    if (!stMBuf.u32StartPhyAddr)
    {
        HI_ERR_WIN("address '0x%x' is null!\n", pstFrame->stBufAddr[0].u32PhyAddr_Y);
        return HI_FAILURE;
    }

#ifdef VDP_DEBUG_USE_IOREMAP
#else
    HI_ERR_WIN("========================map phy addr =0x%x\n", stMBuf.u32StartPhyAddr);

    //nRet = HI_DRV_MMZ_Map(&stMBuf);
    //ptr = (HI_U8 *)stMBuf.u32StartVirAddr);
    ptr = (HI_U8 *)phys_to_virt(stMBuf.u32StartPhyAddr);
    nRet = ptr ? HI_SUCCESS : HI_FAILURE;
#endif
    if (nRet)
    {
        HI_ERR_WIN("address '0x%x' is not valid!\n", pstFrame->stBufAddr[0].u32PhyAddr_Y);
        return HI_FAILURE;
    }

    pu8Udata = (HI_U8 *)DISP_MALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 /2);
    if (!pu8Udata)
        goto EXIT3;
    pu8Vdata = (HI_U8 *)DISP_MALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 /2);
    if ( !pu8Vdata)
        goto EXIT2;
    pu8Ydata = (HI_U8 *)DISP_MALLOC(pstFrame->stBufAddr[0].u32Stride_Y);
    if (!pu8Ydata)
       goto EXIT1;


    /*write Y*/
    for (i=0; i<pstFrame->u32Height; i++)
    {
        memcpy(pu8Ydata, ptr, sizeof(HI_U8)*pstFrame->stBufAddr[0].u32Stride_Y);

        if(pstFrame->u32Width != vdp_k_fwrite(pu8Ydata, pstFrame->u32Width, pfYUV))
        {
            HI_ERR_WIN("line %d: fwrite fail!\n",__LINE__);
        }
        ptr += pstFrame->stBufAddr[0].u32Stride_Y;
    }

    /* U V transfer and save */
    for (i=0; i<pstFrame->u32Height/2; i++)
    {
        for (j=0; j<pstFrame->u32Width/2; j++)
        {
            if(pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
            {
                pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
            }
            else
            {
                pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
            }
        }
        ptr += pstFrame->stBufAddr[0].u32Stride_C;
    }

    /*write U */
    vdp_k_fwrite(pu8Udata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, pfYUV);

    /*write V */
    vdp_k_fwrite(pu8Vdata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, pfYUV);

EXIT1:
   DISP_FREE(pu8Ydata);
EXIT2:
    DISP_FREE(pu8Vdata);
EXIT3:
    DISP_FREE(pu8Udata);

#ifdef VDP_DEBUG_USE_IOREMAP
    iounmap(ptr);
#else
    HI_DRV_MMZ_Unmap(&stMBuf);
#endif

    return HI_SUCCESS;
}


#ifndef HI_ADVCA_FUNCTION_RELEASE
static HI_CHAR u8VdpDebugStr[256];
static HI_CHAR s_VdpSavePath[64] = {'/','m','n','t',0};
#endif

HI_S32 vdp_DebugSaveYUVImg(HI_DRV_VIDEO_FRAME_S *pstCurFrame, HI_CHAR *buffer, HI_U32 count)
{
    HI_S32 nRet = HI_SUCCESS;
#ifndef HI_ADVCA_FUNCTION_RELEASE
    struct file *pfYUV;
    int i,j;
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
        HI_ERR_WIN("******* VDP save path: %s ********\n", u8VdpDebugStr);
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
#endif
    return nRet;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */


