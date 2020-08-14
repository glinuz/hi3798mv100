/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : sample_fb.c
Version         : Initial Draft
Author          :
Created         : 2016/08/10
Description     :
Function List   :


History         :
Date                Author                Modification
2016/08/10           y00181162              Created file
******************************************************************************/


/*********************************add include here******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <assert.h>


#include "hifb.h"
#include "hi_unf_disp.h"
#include "hi_adp_data.h"
#include "hi_adp_mpi.h"

#ifndef CONFIG_SUPPORT_CA_RELEASE
#define Printf  printf
#else
#define Printf(x...)
#endif

static struct fb_var_screeninfo ghifb_st_def_vinfo =
{
    1280,    //visible resolution xres
    720, // yres
    1280, //virtual resolution xres_virtual
    720, //yres_virtual
    0, //xoffset
    0, //yoffset
    32, //bits per pixel
    0, //grey levels, 1 means black/white
    {16, 8, 0}, //fb_bitfiled red
    { 8, 8, 0}, // green
    { 0, 8, 0}, //blue
    {24, 8, 0}, // transparency
    0,  //non standard pixel format
    FB_ACTIVATE_FORCE,
    0, //height of picture in mm
    0, //width of picture in mm
    0, //acceleration flags
    -1, //pixclock
    -1, //left margin
    -1, //right margin
    -1, //upper margin
    -1, //lower margin
    -1, //hsync length
    -1, //vsync length
        //sync: FB_SYNC
        //vmod: FB_VMOD
        //reserved[6]: reserved for future use
};

static void print_vinfo(struct fb_var_screeninfo *vinfo)
{
    Printf( "Printing vinfo:\n");
    Printf("txres: %d\n", vinfo->xres);
    Printf( "tyres: %d\n", vinfo->yres);
    Printf( "txres_virtual: %d\n", vinfo->xres_virtual);
    Printf( "tyres_virtual: %d\n", vinfo->yres_virtual);
    Printf( "txoffset: %d\n", vinfo->xoffset);
    Printf( "tyoffset: %d\n", vinfo->yoffset);
    Printf( "tbits_per_pixel: %d\n", vinfo->bits_per_pixel);
    Printf( "tgrayscale: %d\n", vinfo->grayscale);
    Printf( "tnonstd: %d\n", vinfo->nonstd);
    Printf( "tactivate: %d\n", vinfo->activate);
    Printf( "theight: %d\n", vinfo->height);
    Printf( "twidth: %d\n", vinfo->width);
    Printf( "taccel_flags: %d\n", vinfo->accel_flags);
    Printf( "tpixclock: %d\n", vinfo->pixclock);
    Printf( "tleft_margin: %d\n", vinfo->left_margin);
    Printf( "tright_margin: %d\n", vinfo->right_margin);
    Printf( "tupper_margin: %d\n", vinfo->upper_margin);
    Printf( "tlower_margin: %d\n", vinfo->lower_margin);
    Printf( "thsync_len: %d\n", vinfo->hsync_len);
    Printf( "tvsync_len: %d\n", vinfo->vsync_len);
    Printf( "tsync: %d\n", vinfo->sync);
    Printf( "tvmode: %d\n", vinfo->vmode);
    Printf( "tred: %d/%d\n", vinfo->red.length, vinfo->red.offset);
    Printf( "tgreen: %d/%d\n", vinfo->green.length, vinfo->green.offset);
    Printf( "tblue: %d/%d\n", vinfo->blue.length, vinfo->blue.offset);
    Printf( "talpha: %d/%d\n", vinfo->transp.length, vinfo->transp.offset);
}

static void print_finfo(struct fb_fix_screeninfo *finfo)
{
    Printf( "Printing finfo:\n");
    Printf( "tsmem_start = %p\n", (char *)finfo->smem_start);
    Printf( "tsmem_len = %d\n", finfo->smem_len);
    Printf( "ttype = %d\n", finfo->type);
    Printf( "ttype_aux = %d\n", finfo->type_aux);
    Printf( "tvisual = %d\n", finfo->visual);
    Printf( "txpanstep = %d\n", finfo->xpanstep);
    Printf( "typanstep = %d\n", finfo->ypanstep);
    Printf( "tywrapstep = %d\n", finfo->ywrapstep);
    Printf( "tline_length = %d\n", finfo->line_length);
    Printf( "tmmio_start = %p\n", (char *)finfo->mmio_start);
    Printf( "tmmio_len = %d\n", finfo->mmio_len);
    Printf( "taccel = %d\n", finfo->accel);
}

void* mapped_mem = HI_NULL;
//void* mapped_io  = HI_NULL;
static unsigned long mapped_offset = 0;
static unsigned long mapped_memlen = 0;
//static unsigned long mapped_iolen  = 0;

#define BLUE   0xff0000ff
#define RED    0xffff0000
#define GREEN  0xff00ff00
#define WHITE  0xffffffff
#define YELLOW 0xffffff00

static HI_BOOL PRINTCMDINFO = HI_FALSE;
void PrintfCMDInfo()
{
    if (PRINTCMDINFO)
    {
        return;
    }

    Printf("u can execute this sample like this:\n");
    Printf("\r\r ./sample_fb /dev/fb0 TAB 32\n");
    Printf("/dev/fb0 --> framebuffer device\n");
    Printf("TAB      --> stereo mode top and bottom\n");
    Printf("32       --> pixel format ARGB8888\n");
    Printf("for more information, please refer to the document readme.txt\n");

    PRINTCMDINFO = HI_TRUE;
}

#define ARGB8888TOARGB1555(color) (((color & 0x80000000) >> 16) | ((color & 0xf80000) >> 9) | ((color & 0xf800) >> 6) | ((color & 0xf8) >> 3))
void DrawBox(HIFB_RECT *pstRect, HI_U32 stride,
             HI_U8* pmapped_mem, HI_U32 color, HI_U32 u32Bpp)
{
    HI_U8 *pMem;
    HI_U32 column,row;
    HI_U32 u32Color = color;

    if (u32Bpp != 4 && u32Bpp != 2)
    {
        Printf("DrawBox just support pixelformat ARGB1555&ARGB8888");
        return;
    }

    if (2 == u32Bpp)
    {
        u32Color = ARGB8888TOARGB1555(color);
    }

    for (column = pstRect->y;column < (pstRect->y+pstRect->h);column++)
    {
        pMem =     pmapped_mem + column*stride;
        for (row = pstRect->x;row < (pstRect->x+pstRect->w);row++)
        {
            if (2 == u32Bpp)
            {
                *(HI_U16*)(pMem + row*u32Bpp) = (HI_U16)u32Color;
            }
            else if (4 == u32Bpp)
            {
                *(HI_U32*)(pMem + row*u32Bpp) = u32Color;
            }
        }
    }
}

void ParseAndSet3DMode(char *pCmd)
{
    HI_UNF_DISP_3D_E en3DFMT = HI_UNF_DISP_3D_NONE;
    HI_UNF_ENC_FMT_E enEncFormat =  HI_UNF_ENC_FMT_720P_50;

    if (strncmp("sbs", pCmd, 3) == 0 ||
        strncmp("SBS", pCmd, 3) == 0)
    {
        en3DFMT = HI_UNF_DISP_3D_SIDE_BY_SIDE_HALF;
        enEncFormat = HI_UNF_ENC_FMT_1080i_50;
    }
    else if (strncmp("TAB", pCmd, 3) == 0 ||
        strncmp("tab", pCmd, 3) == 0)
    {
        en3DFMT = HI_UNF_DISP_3D_TOP_AND_BOTTOM;
        enEncFormat = HI_UNF_ENC_FMT_720P_50;
    }
    else if (strncmp("FP", pCmd, 3) == 0 ||
        strncmp("fp", pCmd, 3) == 0)
    {
        en3DFMT = HI_UNF_DISP_3D_FRAME_PACKING;
        enEncFormat = HI_UNF_ENC_FMT_720P_50_FRAME_PACKING;
    }
    else if (strncmp("NULL", pCmd, 3) == 0 ||
        strncmp("null", pCmd, 3) == 0)
    {
        return;
    }
    else
    {
        PrintfCMDInfo();
    }

    if (en3DFMT != HI_UNF_DISP_3D_NONE)
    {
        HI_UNF_DISP_Set3DMode(HI_UNF_DISPLAY1, en3DFMT, enEncFormat);
    }
}

/* ARGB1555 */
static struct fb_bitfield g_r16 = {10, 5, 0};
static struct fb_bitfield g_g16 = {5, 5, 0};
static struct fb_bitfield g_b16 = {0, 5, 0};
static struct fb_bitfield g_a16 = {15, 1, 0};


void SetNewColorFMT(int fd, char *pCmd)
{
    struct fb_var_screeninfo vinfo;

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
        Printf ( "fail to get vscreeninfo!\n");
        return;
    }

    if (strncmp("16", pCmd, 2) == 0)
    {
        vinfo.bits_per_pixel = 16;
        vinfo.blue = g_b16;
        vinfo.red = g_r16;
        vinfo.transp= g_a16;
        vinfo.green = g_g16;

        if (ioctl(fd, FBIOPUT_VSCREENINFO, &vinfo) < 0)
        {
            Printf ( "fail to put vscreeninfo!\n");
            return;
        }
    }
    else if (strncmp("32", pCmd, 2) != 0)
    {
        PrintfCMDInfo();
    }
}

int main(int argc, char* argv[])
{
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    HIFB_LAYER_INFO_S layerinfo;
    HIFB_BUFFER_S CanvasBuf;
    HI_U32 u32BufSize = 0;
    HI_U32 u32DisPlayBufSize = 0;
    HI_U32 u32LineLen = 0;
    HIFB_ALPHA_S stAlpha;
    int console_fd;
    HIFB_RECT stRect;
    int ret = 0;
    HI_U32 u32Bpp;
#ifdef CFG_HIFB_ANDRIOD
    HI_BOOL bDecmp = HI_FALSE;
#endif
    const char* file = "/dev/fb0";

    /* open fb device */
    if (argc >= 2)
    {
        file = argv[1];
    }

#ifndef CFG_HIFB_ANDRIOD
    HI_SYS_Init();

    HIADP_MCE_Exit();

    ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    if (ret != HI_SUCCESS)
    {
        return 0;
    }

    if (argc >= 3)
    {
        ParseAndSet3DMode(argv[2]);
    }
#else
    file = "/dev/graphics/fb0";
#endif

    console_fd = open(file, O_RDWR, 0);
    if (console_fd < 0)
    {
        Printf ( "Unable to open %s\n", file);
        PrintfCMDInfo();
        return (-1);
    }

    /* set color format ARGB8888, screen size: 1280*720 */
    if (ioctl(console_fd, FBIOPUT_VSCREENINFO, &ghifb_st_def_vinfo) < 0)
    {
        Printf ( "Unable to set variable screeninfo!\n");
        ret = -1;
        goto CLOSEFD;
    }

    /*set new color format*/
    if (argc >= 4)
    {
        SetNewColorFMT(console_fd, argv[3]);
    }

    /* Get the fix screen info of hardware */
    if (ioctl(console_fd, FBIOGET_FSCREENINFO, &finfo) < 0)
    {
        Printf ( "Couldn't get console hardware info\n");
        ret = -1;
        goto CLOSEFD;
    }

    print_finfo(&finfo);

    mapped_memlen = finfo.smem_len + mapped_offset;
    u32DisPlayBufSize = ghifb_st_def_vinfo.xres_virtual*ghifb_st_def_vinfo.yres_virtual*(ghifb_st_def_vinfo.bits_per_pixel/8);
    if (mapped_memlen != 0 && mapped_memlen >= u32DisPlayBufSize)
    {
        u32BufSize = finfo.smem_len;
        u32LineLen = finfo.line_length;
        mapped_mem = mmap(NULL, mapped_memlen,
                          PROT_READ | PROT_WRITE, MAP_SHARED, console_fd, 0);
        if (mapped_mem == (char *)-1)
        {
            Printf ( "Unable to memory map the video hardware\n");
            mapped_mem = NULL;
            ret = -1;
            goto CLOSEFD;
        }
    }

    /* Determine the current screen depth */
    if (ioctl(console_fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
        Printf ( "Couldn't get vscreeninfo\n");
        ret = -1;
        goto UNMAP;
    }

    u32Bpp = vinfo.bits_per_pixel/8;
    print_vinfo(&vinfo);

    /* set alpha */
    stAlpha.bAlphaEnable  = HI_TRUE;
    stAlpha.u8Alpha0 = 0xff;
    stAlpha.u8Alpha1 = 0xff;

    /* set global alpha */
    stAlpha.bAlphaChannel = HI_TRUE;
    stAlpha.u8GlobalAlpha = 0xff;//0xf0,0x80..

    if (ioctl(console_fd, FBIOPUT_ALPHA_HIFB, &stAlpha) < 0)
    {
        Printf ( "Couldn't set alpha\n");
        ret = -1;
        goto UNMAP;
    }

#ifdef CFG_HIFB_ANDRIOD
    ioctl(console_fd,FBIOPUT_DECOMPRESS_HIFB,&bDecmp);
#endif

    /*µ¥bufferÄ£Ê½*/
    if (mapped_memlen == 0 || mapped_memlen < u32DisPlayBufSize)
    {

        if (ioctl(console_fd, FBIOGET_LAYER_INFO, &layerinfo) < 0)
        {
            printf("Get layer info failed!\n");
            goto CLOSEFD;
        }

         layerinfo.u32Mask=0;
         layerinfo.eAntiflickerLevel=HIFB_LAYER_ANTIFLICKER_HIGH;
         layerinfo.BufMode=HIFB_LAYER_BUF_NONE;
         layerinfo.u32CanvasWidth = vinfo.xres;
         layerinfo.u32CanvasHeight = vinfo.yres;
         layerinfo.u32Mask |= HIFB_LAYERMASK_BUFMODE;
         layerinfo.u32Mask |= HIFB_LAYERMASK_ANTIFLICKER_MODE;
         layerinfo.u32Mask |= HIFB_LAYERMASK_CANVASSIZE;
        if (ioctl(console_fd, FBIOPUT_LAYER_INFO, &layerinfo) < 0)
        {
            printf("put layer info failed!\n");
            goto CLOSEFD;
        }

        if (ioctl(console_fd, FBIOGET_CANVAS_BUFFER, &CanvasBuf) < 0)
        {
            printf("get canvas buffer failed\n");
            goto CLOSEFD;
        }

        if (CanvasBuf.stCanvas.u32PhyAddr == 0)
        {
             printf("CanvasBuf is NULL\n");
             goto CLOSEFD;
        }

        u32LineLen = CanvasBuf.stCanvas.u32Pitch;
        u32BufSize = CanvasBuf.stCanvas.u32Height * CanvasBuf.stCanvas.u32Pitch;

        mapped_mem = (void *)HI_MEM_Map(CanvasBuf.stCanvas.u32PhyAddr, u32BufSize);
        if (mapped_mem == NULL)
        {
             printf("map canvas buffer failed\n");
             goto CLOSEFD;
        }
    }


    memset(mapped_mem, 0x00, u32BufSize);

    Printf("fill box\n");

    stRect.x = 100;
    stRect.y = 0;
    stRect.w = 100;
    stRect.h = 100;

    DrawBox(&stRect, u32LineLen, mapped_mem, BLUE, u32Bpp);
    stRect.y += 110;
    DrawBox(&stRect, u32LineLen, mapped_mem, RED, u32Bpp);
    stRect.y += 110;
    DrawBox(&stRect, u32LineLen, mapped_mem, GREEN, u32Bpp);
    stRect.y += 110;
    DrawBox(&stRect, u32LineLen, mapped_mem, YELLOW, u32Bpp);
    stRect.y += 110;
    DrawBox(&stRect, u32LineLen, mapped_mem, WHITE, u32Bpp);

    if (mapped_memlen == 0 ||mapped_memlen < u32DisPlayBufSize)
    {
        CanvasBuf.UpdateRect.x = 0;
        CanvasBuf.UpdateRect.y = 0;
        CanvasBuf.UpdateRect.w = CanvasBuf.stCanvas.u32Width;
        CanvasBuf.UpdateRect.h = CanvasBuf.stCanvas.u32Height;
        if (ioctl(console_fd, FBIO_REFRESH, &CanvasBuf) < 0)
        {
            printf("refresh buffer info failed!\n");
        }
    }
    else
    {
        if (ioctl(console_fd, FBIOPAN_DISPLAY, &vinfo) < 0)
        {
            printf("pan_display failed!\n");
        }
    }

    getchar();
UNMAP:
    if (mapped_memlen != 0)
    {
        munmap(mapped_mem, mapped_memlen);
    }
    else
    {
        HI_MEM_Unmap(mapped_mem);
    }

CLOSEFD:
    close(console_fd);


#ifndef CFG_HIFB_ANDRIOD
    HIADP_Disp_DeInit();
    HI_SYS_DeInit();
#endif
    return ret;
}
