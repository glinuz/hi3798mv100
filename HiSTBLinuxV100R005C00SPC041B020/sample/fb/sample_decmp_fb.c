/******************************************************************************

  Copyright (C), 2013-2020, Hisilicon. Co., Ltd.

******************************************************************************
File Name       : sample_decmp_fb.c
Version         : Initial Draft
Author          :
Created         : 2015/11/07
Description     : 测试解压功能
Function List   :


History         :
Date                Author              Modification
2015/11/07          y00181162           Created file
******************************************************************************/

/****************************  add include here     ***************************/
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <assert.h>
#include "hifb.h"
#include "hi_unf_disp.h"
#include "hi_adp_data.h"
#include "hi_adp_mpi.h"

/***************************** Macro Definition     ***************************/
#ifndef CONFIG_SUPPORT_CA_RELEASE
#define HIFB_TRACE  printf
#else
#define HIFB_TRACE(x...)
#endif

#if 1
#define DECMP_WIDTH     1280
#define DECMP_HEIGHT    720
#define DENCMP_WIDTH    DECMP_WIDTH
#define DENCMP_HEIGHT   DECMP_HEIGHT
#if defined(CHIP_TYPE_hi3798cv200)
#define DECMP_STRIDE    6656
#define DENCMP_STRIDE   (DECMP_WIDTH * 4)
#else
#define DECMP_STRIDE    2816
#define DENCMP_STRIDE   (DECMP_WIDTH * 4)
#endif
#else
#define DECMP_WIDTH     1920
#define DECMP_HEIGHT    1080
#define DENCMP_WIDTH    DECMP_WIDTH
#define DENCMP_HEIGHT   DECMP_HEIGHT
#if defined(CHIP_TYPE_hi3798cv200)
#define DECMP_STRIDE    9984
#define DENCMP_STRIDE   (DECMP_WIDTH * 4)
#else
#define DECMP_STRIDE    3840
#define DENCMP_STRIDE   (DECMP_WIDTH * 4)
#endif
#endif


#define SYNC_IOC_MAGIC   '>'
#define SYNC_IOC_WAIT    _IOW(SYNC_IOC_MAGIC, 0, __s32)

#if defined(CHIP_TYPE_hi3798cv200)
#define CONIFG_DECOMPRESS_DIRNAME       "../res/version1.0/cmp"
#define CONIFG_UNDECOMPRESS_DIRNAME     "../res/version1.0/ncmp"
#else
#define CONIFG_DECOMPRESS_DIRNAME       "../res/version2.0/cmp"
#define CONIFG_UNDECOMPRESS_DIRNAME     "../res/version2.0/ncmp"
#endif
#define CONFIG_HIFB_DEV0                "/dev/fb0"

/***************************** Structure Definition ***************************/

/********************** Global Variable declaration **************************/
static struct fb_var_screeninfo ghifb_st_argb8888_def_vinfo =
{
    DECMP_WIDTH,DECMP_HEIGHT,DECMP_WIDTH,DECMP_HEIGHT,0,0,
    32,
    0,
    {16, 8, 0},
    { 8, 8, 0},
    { 0, 8, 0},
    {24, 8, 0},
    0,
    FB_ACTIVATE_FORCE,0,0,0,-1,-1,-1,-1,-1,-1,-1,
};


static HI_U32 gs_PhyStreamBuf    = 0;
static HI_CHAR* gs_VirStreamBuf  = NULL;
static HI_S32 gs_fd = -1;
//static HI_S32 gs_FenceFd = -1;

/********************** API forward declarations    **************************/
#ifdef HI_FB_DECOMPRESS_SUPPORT
static inline HI_S32 show_decompress_data(HI_S32 FileSize, HI_U32 Stride);
static inline HI_S32 test_filter_decompressfile(const struct dirent *pentry);
#endif

static inline HI_S32 show_undecompress_data(HI_S32 FileSize, HI_U32 Stride);
static inline HI_S32 test_filter_undecompressfile(const struct dirent *pentry);

static inline HI_S32  test_set_config(HI_VOID);
static inline HI_VOID test_refresh(HI_U32 Width, HI_U32 Height, HI_U32 Stride, HI_BOOL bDecompress);


/********************** API release ******************************************/


/****************************************************************************
* func          : print_vinfo
* description   : CNcomment: 打印可变信息 CNend\n
* param[in]     : NA
* retval        : NA
* others:       : NA
******************************************************************************/
static void print_vinfo(struct fb_var_screeninfo *vinfo)
{
    HIFB_TRACE("\n=======================================================================\n");
    HIFB_TRACE( "Printing var info:\n");
    HIFB_TRACE( "txres           : %d\n", vinfo->xres);
    HIFB_TRACE( "tyres           : %d\n", vinfo->yres);
    HIFB_TRACE( "txres_virtual   : %d\n", vinfo->xres_virtual);
    HIFB_TRACE( "tyres_virtual   : %d\n", vinfo->yres_virtual);
    HIFB_TRACE( "txoffset        : %d\n", vinfo->xoffset);
    HIFB_TRACE( "tyoffset        : %d\n", vinfo->yoffset);
    HIFB_TRACE( "tbits_per_pixel : %d\n", vinfo->bits_per_pixel);
    HIFB_TRACE( "tgrayscale      : %d\n", vinfo->grayscale);
    HIFB_TRACE( "tnonstd         : %d\n", vinfo->nonstd);
    HIFB_TRACE( "tactivate       : %d\n", vinfo->activate);
    HIFB_TRACE( "theight         : %d\n", vinfo->height);
    HIFB_TRACE( "twidth          : %d\n", vinfo->width);
    HIFB_TRACE( "taccel_flags    : %d\n", vinfo->accel_flags);
    HIFB_TRACE( "tpixclock       : %d\n", vinfo->pixclock);
    HIFB_TRACE( "tleft_margin    : %d\n", vinfo->left_margin);
    HIFB_TRACE( "tright_margin   : %d\n", vinfo->right_margin);
    HIFB_TRACE( "tupper_margin   : %d\n", vinfo->upper_margin);
    HIFB_TRACE( "tlower_margin   : %d\n", vinfo->lower_margin);
    HIFB_TRACE( "thsync_len      : %d\n", vinfo->hsync_len);
    HIFB_TRACE( "tvsync_len      : %d\n", vinfo->vsync_len);
    HIFB_TRACE( "tsync           : %d\n", vinfo->sync);
    HIFB_TRACE( "tvmode          : %d\n", vinfo->vmode);
    HIFB_TRACE( "tred            : %d/%d\n", vinfo->red.length, vinfo->red.offset);
    HIFB_TRACE( "tgreen          : %d/%d\n", vinfo->green.length, vinfo->green.offset);
    HIFB_TRACE( "tblue           : %d/%d\n", vinfo->blue.length, vinfo->blue.offset);
    HIFB_TRACE( "talpha          : %d/%d\n", vinfo->transp.length, vinfo->transp.offset);
    HIFB_TRACE("=======================================================================\n");
}

static void print_finfo(struct fb_fix_screeninfo *finfo)
{
    HIFB_TRACE("\n=======================================================================\n");
    HIFB_TRACE( "Printing finfo  :\n");
    HIFB_TRACE( "tsmem_start     : %p\n", (char *)finfo->smem_start);
    HIFB_TRACE( "tsmem_len       : %d\n", finfo->smem_len);
    HIFB_TRACE( "ttype           : %d\n", finfo->type);
    HIFB_TRACE( "ttype_aux       : %d\n", finfo->type_aux);
    HIFB_TRACE( "tvisual         : %d\n", finfo->visual);
    HIFB_TRACE( "txpanstep       : %d\n", finfo->xpanstep);
    HIFB_TRACE( "typanstep       : %d\n", finfo->ypanstep);
    HIFB_TRACE( "tywrapstep      : %d\n", finfo->ywrapstep);
    HIFB_TRACE( "tline_length    : %d\n", finfo->line_length);
    HIFB_TRACE( "tmmio_start     : %p\n", (char *)finfo->mmio_start);
    HIFB_TRACE( "tmmio_len       : %d\n", finfo->mmio_len);
    HIFB_TRACE( "taccel          : %d\n", finfo->accel);
    HIFB_TRACE("=======================================================================\n");
}


/*****************************************************************************
* func          : Display_Init
* description   : VO初始化
* param[in]     :
* retval        : NA
* others:       : NA
*****************************************************************************/
static inline HI_S32 Display_Init()
{
    HI_SYS_Init();
    HIADP_MCE_Exit();
    HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    return HI_SUCCESS;
}
/*****************************************************************************
* func          : Display_DeInit
* description   : VO去初始化
* param[in]     :
* retval        : NA
* others:       : NA
*****************************************************************************/
static inline HI_S32 Display_DeInit()
{
    HIADP_Disp_DeInit();
    HI_SYS_DeInit();
    return  0;
}


int main(int argc, char* argv[])
{
    HI_S32 s32Ret  = HI_SUCCESS;
    HI_U32 u32Size = 0;
#ifdef HI_FB_DECOMPRESS_SUPPORT
    HI_BOOL bDecompress = HI_FALSE;
#endif
    Display_Init();

    s32Ret = HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISPLAY1, DECMP_WIDTH, DECMP_HEIGHT);
    if (s32Ret != HI_SUCCESS)
    {
        HIFB_TRACE("HI_UNF_DISP_SetVirtualScreen failure\n");
        goto ERR_EXIT;
    }
    sleep(1);

    gs_fd = open(CONFIG_HIFB_DEV0, O_RDWR, 0);
    if (gs_fd < 0)
    {
        HIFB_TRACE("Unable to open %s\n", CONFIG_HIFB_DEV0);
        goto ERR_EXIT;
    }

#if defined(CHIP_TYPE_hi3798cv200)
    u32Size = DECMP_STRIDE * DECMP_HEIGHT;
#else
    u32Size = ((16 * DECMP_HEIGHT + 0xff) & 0xffffff00) * 2 + (DECMP_STRIDE * DECMP_HEIGHT) * 2;
#endif

    gs_PhyStreamBuf = (unsigned long)HI_MMZ_New(u32Size,16,NULL,"sample_fb");
    if (0 == gs_PhyStreamBuf)
    {
        HIFB_TRACE("HI_MMZ_New stream buffer failure\n");
        goto ERR_EXIT;
    }

    gs_VirStreamBuf = (HI_CHAR*)HI_MMZ_Map(gs_PhyStreamBuf,HI_FALSE);
    if (NULL == gs_VirStreamBuf)
    {
        HIFB_TRACE("HI_MMZ_Map stream buffer failure\n");
        goto ERR_EXIT;
    }

    s32Ret = test_set_config();
    if (s32Ret < 0)
    {
       HIFB_TRACE("test_set_config failure\n");
       goto ERR_EXIT;
    }

#ifdef HI_FB_DECOMPRESS_SUPPORT
    bDecompress = HI_TRUE;
    s32Ret = ioctl(gs_fd,FBIOPUT_DECOMPRESS_HIFB,&bDecompress);
    if (s32Ret < 0)
    {
       HIFB_TRACE("set decompress failure\n");
       goto ERR_EXIT;
    }

    s32Ret = show_decompress_data(u32Size,DECMP_STRIDE);
    if (s32Ret < 0)
    {
       HIFB_TRACE("show_decompress_data failure\n");
       goto ERR_EXIT;
    }

    bDecompress = HI_FALSE;
    s32Ret = ioctl(gs_fd,FBIOPUT_DECOMPRESS_HIFB,&bDecompress);
    if (s32Ret < 0)
    {
       HIFB_TRACE("set undecompress failure\n");
       goto ERR_EXIT;
    }
#endif

    u32Size = DENCMP_STRIDE * DENCMP_HEIGHT;
    s32Ret = show_undecompress_data(u32Size,DENCMP_STRIDE);
    if (s32Ret < 0)
    {
       HIFB_TRACE("show_undecompress_data failure\n");
       goto ERR_EXIT;
    }

ERR_EXIT:

    if (gs_fd >= 0)
    {
        close(gs_fd);
        gs_fd = -1;
    }

    if (NULL != gs_VirStreamBuf)
    {
        HI_MMZ_Unmap(gs_PhyStreamBuf);
        gs_VirStreamBuf = NULL;
    }

    if (0 != gs_PhyStreamBuf)
    {
        HI_MMZ_Delete(gs_PhyStreamBuf);
        gs_PhyStreamBuf = 0;
    }

    Display_DeInit();

    return 0;
}


#ifdef HI_FB_DECOMPRESS_SUPPORT
static inline HI_S32 show_decompress_data(HI_S32 FileSize, HI_U32 Stride)
{
    HI_S32 FileNum = -1;
    HI_U64 ReadFileSize = 0;
    HI_CHAR FileName[256] = {'\0'};
    FILE* pInFile = NULL;
    struct dirent **pFilelist = NULL;

    HIFB_TRACE("\n================================================\n");
    HIFB_TRACE("show decompress\n");
    HIFB_TRACE("================================================\n");

    FileNum = scandir(CONIFG_DECOMPRESS_DIRNAME, &pFilelist, test_filter_decompressfile, alphasort);
    if (FileNum < 0)
    {
        HIFB_TRACE("===%s %s %d failure\n",__FILE__,__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    while (FileNum--)
    {
        sprintf(FileName, "%s/%s", CONIFG_DECOMPRESS_DIRNAME, pFilelist[FileNum]->d_name);

        pInFile = fopen(FileName,"rb");
        if (NULL == pInFile)
        {
           HIFB_TRACE("open decompress file failure");
           free(pFilelist[FileNum]);
           continue;
        }

        ReadFileSize = fread(gs_VirStreamBuf,1,FileSize,pInFile);
        if (ReadFileSize != FileSize)
        {
            HIFB_TRACE("read decompress file failure [%lld %d]\n",ReadFileSize,FileSize);
            fclose(pInFile);
            pInFile = NULL;
            free(pFilelist[FileNum]);
            continue;
        }

        test_refresh(DECMP_WIDTH, DECMP_HEIGHT, Stride, HI_TRUE);

        fclose(pInFile);
        pInFile = NULL;
        free(pFilelist[FileNum]);
        usleep(200 * 1000);
    }

    free(pFilelist);
    pFilelist = NULL;

    return HI_SUCCESS;
}

static inline HI_S32 test_filter_decompressfile(const struct dirent *pentry)
{
    HI_CHAR *pos = NULL;
    pos = strrchr(pentry->d_name, '.');
    if (NULL == pos)
    {
       return 0;
    }

    pos++;
    if (strcasecmp(pos, "cmp") != 0)
    {
        return 0;
    }

    return 1;
}
#endif

static inline HI_S32 show_undecompress_data(HI_S32 FileSize, HI_U32 Stride)
{
    HI_S32 FileNum = -1;
    HI_U64 ReadFileSize = 0;
    HI_CHAR FileName[256] = {'\0'};
    FILE* pInFile = NULL;
    struct dirent **pFilelist = NULL;

    HIFB_TRACE("\n================================================\n");
    HIFB_TRACE("show un decompress\n");
    HIFB_TRACE("================================================\n");
    FileNum = scandir(CONIFG_UNDECOMPRESS_DIRNAME, &pFilelist, test_filter_undecompressfile, alphasort);
    if (FileNum < 0)
    {
        HIFB_TRACE("===%s %s %d failure\n",__FILE__,__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    while (FileNum--)
    {
        sprintf(FileName, "%s/%s", CONIFG_UNDECOMPRESS_DIRNAME, pFilelist[FileNum]->d_name);

        pInFile = fopen(FileName,"rb");
        if (NULL == pInFile)
        {
           HIFB_TRACE("open undecompress file failure");
           free(pFilelist[FileNum]);
           continue;
        }

        ReadFileSize = fread(gs_VirStreamBuf,1,FileSize,pInFile);
        if (ReadFileSize != FileSize)
        {
            HIFB_TRACE("read undecompress file failure [%lld %d]\n",ReadFileSize,FileSize);
            fclose(pInFile);
            pInFile = NULL;
            free(pFilelist[FileNum]);
            continue;
        }

        test_refresh(DENCMP_WIDTH, DENCMP_HEIGHT, Stride, HI_FALSE);

        fclose(pInFile);
        pInFile = NULL;
        free(pFilelist[FileNum]);
        usleep(200 * 1000);
    }

    free(pFilelist);
    pFilelist = NULL;

    return HI_SUCCESS;
}


static inline HI_S32 test_filter_undecompressfile(const struct dirent *pentry)
{
    HI_CHAR *pos = NULL;
    pos = strrchr(pentry->d_name, '.');
    if (NULL == pos)
    {
       return 0;
    }

    pos++;
    if (strcasecmp(pos, "ncmp") != 0)
    {
        return 0;
    }

    return 1;
}


static inline HI_S32 test_set_config(HI_VOID)
{
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    HIFB_ALPHA_S stAlpha;
    HIFB_LAYER_INFO_S layerinfo;
    HI_U32 u32SmemStartPhy = 0;
    HIFB_HWC_LAYERINFO_S stHwcLayerInfo;


    if (ioctl(gs_fd, FBIOGET_SMEMSTART_PHY, &u32SmemStartPhy) < 0)
    {
        HIFB_TRACE("Unable to set variable screeninfo!\n");
        return HI_FAILURE;
    }

    if (ioctl(gs_fd, FBIOPUT_VSCREENINFO, &ghifb_st_argb8888_def_vinfo) < 0)
    {
        HIFB_TRACE("Unable to set variable screeninfo!\n");
        return HI_FAILURE;
    }

    if (ioctl(gs_fd, FBIOGET_FSCREENINFO, &finfo) < 0)
    {
        HIFB_TRACE("Couldn't get console hardware info\n");
        return HI_FAILURE;
    }

    HIFB_TRACE("\n=====================================================\n");
    HIFB_TRACE("u32SmemStartPhy  = 0x%x\n",u32SmemStartPhy);
    HIFB_TRACE("finfo.smem_start = 0x%lx\n",finfo.smem_start);
    HIFB_TRACE("=====================================================\n");

    print_finfo(&finfo);

    if (ioctl(gs_fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
        HIFB_TRACE("Couldn't get console pixel format\n");
        return HI_FAILURE;
    }
    print_vinfo(&vinfo);


    stAlpha.bAlphaEnable  = HI_TRUE;
    stAlpha.bAlphaChannel = HI_FALSE;
    stAlpha.u8Alpha0      = 0xff;
    stAlpha.u8Alpha1      = 0xff;
    if (ioctl(gs_fd, FBIOPUT_ALPHA_HIFB, &stAlpha) < 0)
    {
        HIFB_TRACE("Couldn't set alpha\n");
        return HI_FAILURE;
    }

    memset(&stHwcLayerInfo,0x0,sizeof(stHwcLayerInfo));
    stHwcLayerInfo.eFmt = HIFB_FMT_ARGB8888;
    ioctl(gs_fd,FBIO_HWC_GETLAYERINFO,&stHwcLayerInfo);

    layerinfo.u32Mask           = 0;
    layerinfo.eAntiflickerLevel = HIFB_LAYER_ANTIFLICKER_HIGH;
    layerinfo.BufMode           = HIFB_LAYER_BUF_NONE;
    layerinfo.u32CanvasWidth    = vinfo.xres;
    layerinfo.u32CanvasHeight   = vinfo.yres;
    layerinfo.u32Mask |= HIFB_LAYERMASK_BUFMODE;
    layerinfo.u32Mask |= HIFB_LAYERMASK_ANTIFLICKER_MODE;
    layerinfo.u32Mask |= HIFB_LAYERMASK_CANVASSIZE;

    if (ioctl(gs_fd, FBIOPUT_LAYER_INFO, &layerinfo) < 0)
    {
        HIFB_TRACE("put layer info failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#if 1
static inline HI_VOID test_refresh(HI_U32 Width, HI_U32 Height, HI_U32 Stride, HI_BOOL bDecompress)
{
    HIFB_BUFFER_S CanvasBuf;

    memset(&CanvasBuf, 0x0, sizeof(HIFB_BUFFER_S));

    ioctl(gs_fd, FBIOGET_CANVAS_BUFFER, &CanvasBuf);

    CanvasBuf.stCanvas.u32PhyAddr =  gs_PhyStreamBuf;
    CanvasBuf.stCanvas.u32Width   =  Width;
    CanvasBuf.stCanvas.u32Height  =  Height;
    CanvasBuf.stCanvas.u32Pitch   =  Stride;
    CanvasBuf.stCanvas.enFmt      =  HIFB_FMT_ARGB8888;
    CanvasBuf.UpdateRect.x = 0;
    CanvasBuf.UpdateRect.y = 0;
    CanvasBuf.UpdateRect.w = CanvasBuf.stCanvas.u32Width;
    CanvasBuf.UpdateRect.h = CanvasBuf.stCanvas.u32Height;

    HIFB_TRACE("\n=====================================================\n");
    HIFB_TRACE("disp ddr    = 0x%x\n", gs_PhyStreamBuf);
    HIFB_TRACE("disp width  = %d\n", Width);
    HIFB_TRACE("disp height = %d\n", Height);
    HIFB_TRACE("disp stride = %d\n", Stride);
    HIFB_TRACE("=====================================================\n");
    ioctl(gs_fd, FBIO_REFRESH, &CanvasBuf);

    sleep(10);

    return;
}
#else
static inline HI_VOID test_refresh(HI_U32 Width, HI_U32 Height, HI_U32 Stride, HI_BOOL bDecompress)
{
    HI_S32 TimeOut = -1;
    HIFB_HWC_LAYERINFO_S HwcLayerInfo;

    memset(&HwcLayerInfo, 0x0, sizeof(HIFB_HWC_LAYERINFO_S));

    HwcLayerInfo.u32LayerAddr   = gs_PhyStreamBuf;
    HwcLayerInfo.u32Stride      = Stride;
    HwcLayerInfo.u32NoCmpStride = Stride;
    HwcLayerInfo.u32Alpha = 0xff;
    HwcLayerInfo.bPreMul = HI_FALSE;
    HwcLayerInfo.bDeCompress = bDecompress;
    HwcLayerInfo.eFmt = HIFB_FMT_ARGB8888;
    HwcLayerInfo.s32AcquireFenceFd = -1;

    HwcLayerInfo.stInRect.x = 0;
    HwcLayerInfo.stInRect.y = 0;
    HwcLayerInfo.stInRect.w = Width;
    HwcLayerInfo.stInRect.h = Height;

    HIFB_TRACE("\n=====================================================\n");
    HIFB_TRACE("disp ddr    = 0x%x\n", gs_PhyStreamBuf);
    HIFB_TRACE("disp width  = %d\n", Width);
    HIFB_TRACE("disp height = %d\n", Height);
    HIFB_TRACE("disp stride = %d\n", Stride);
    sleep(1);

    if (gs_FenceFd >= 0)
    {
       HIFB_TRACE("waite sync\n");
       ioctl(gs_FenceFd, SYNC_IOC_WAIT, &TimeOut);
       close(gs_FenceFd);
       gs_FenceFd = -1;
    }

    ioctl(gs_fd, FBIO_HWC_REFRESH, &HwcLayerInfo);
    gs_FenceFd = HwcLayerInfo.s32ReleaseFenceFd;

    HIFB_TRACE("release fence fd = %d\n", HwcLayerInfo.s32ReleaseFenceFd);
    HIFB_TRACE("=====================================================\n");

    sleep(10);

    return;
}
#endif
