#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <dirent.h>

#include "hi_type.h"
#include "png.h"
#include "hi_png_api.h"
#include "hi_png_errcode.h"
#include "hi_png_config.h"
#include "hi_tde_type.h"
#include "zlib.h"

#include "hifb.h"
#include "hi_tde_api.h"
#include "hi_unf_disp.h"
#include "hi_adp_mpi.h"

#define PNG_DIR          "../res/png/"
#define PNG_FILE         "../res/png/back1.png"


#define HI_SYS_GET_STRIDE(w)    (((w) + 15) / 16 * 16)

typedef struct
{
    HI_U32 u32PhyAddr;      /* 物理地址*/
    HI_U32 u32Stride;       /* 行宽*/
    HI_U32 u32RowBytes;     /* 行有效字节数*/
    HI_U32 u32Height;       /* 高度*/
} PNG_RESULT_S;

typedef struct
{
    HI_U8 bit_alpha;
    HI_U8 bit_red;
    HI_U8 bit_green;
    HI_U8 bit_blue;
    HI_U8 offset_alpha;
    HI_U8 offset_red;
    HI_U8 offset_green;
    HI_U8 offset_blue;
} HI_PNG_ARGB_S;

HI_PNG_ARGB_S sArgbInfo[] =
{
    /* ARGB4444 */
    {4, 4, 4, 4, 12,  8, 4, 0},
    /* ARGB1555 */
    {1, 5, 5, 5, 15, 10, 5, 0},
    /* RGB565 */
    {0, 5, 6, 5, 16, 11, 5, 0},
    /* RGB555 */
    {0, 5, 5, 5, 15, 10, 5, 0},
    /* RGB444 */
    {0, 4, 4, 4, 12,  8, 4, 0}
};

HI_S32 PNG_ShowSurface(TDE2_SURFACE_S *pstSurface, HI_U32 u32PosX, HI_U32 u32PosY)
{
    int fd;
    struct fb_fix_screeninfo fix;
    struct fb_var_screeninfo var;
    struct fb_bitfield r32 = {16, 8, 0};
    struct fb_bitfield g32 = {8, 8, 0};
    struct fb_bitfield b32 = {0, 8, 0};
    struct fb_bitfield a32 = {24, 8, 0};

    HIFB_POINT_S stPoint = {u32PosX, u32PosY};

    TDE2_SURFACE_S stFbSurface;
    TDE_HANDLE tdeHandle;
    TDE2_RECT_S copyRect = {0, 0, pstSurface->u32Width, pstSurface->u32Height};
    TDE2_OPT_S stOpt = {0};
    HI_BOOL bShow = HI_TRUE;
    HI_S32 ret ;
    HI_SYS_Init();
    HIADP_MCE_Exit();
    ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    if (ret != HI_SUCCESS)
    {
        return 0;
    }

    fd = open("/dev/fb0", O_RDWR);
    if(fd < 0)
    {
        printf("open fb0 failed!\n");
        return -1;
    }
    if (ioctl(fd, FBIOPUT_SCREEN_ORIGIN_HIFB, &stPoint) < 0)
    {
        printf("set screen original show position failed!\n");
        close(fd);
        return -1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0)
    {
    printf("Get variable screen info failed!\n");
        close(fd);
        return -1;
    }

    var.xres = var.xres_virtual = pstSurface->u32Width;
    var.yres = pstSurface->u32Height;
    var.yres_virtual = pstSurface->u32Height;

    var.transp= a32;
    var.red = r32;
    var.green = g32;
    var.blue = b32;
    var.bits_per_pixel = 32;

    if (ioctl(fd, FBIOPUT_VSCREENINFO, &var) < 0)
    {
    printf("Put variable screen info failed!\n");
        close(fd);
        return -1;
    }

        if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0)
    {
    printf("Put variable screen info failed!\n");
        close(fd);
        return -1;
    }


    if (ioctl(fd, FBIOGET_FSCREENINFO, &fix) < 0)
    {
    printf("Get fix screen info failed!\n");
        close(fd);
        return -1;
    }

    stFbSurface.u32PhyAddr = fix.smem_start;
    stFbSurface.bAlphaMax255 = HI_TRUE;
    stFbSurface.bYCbCrClut = 0;
    stFbSurface.enColorFmt = TDE2_COLOR_FMT_ARGB8888;
    stFbSurface.pu8ClutPhyAddr = HI_NULL;
    stFbSurface.u32Height = pstSurface->u32Height;
    stFbSurface.u32Width = pstSurface->u32Width;
    stFbSurface.u32Stride = fix.line_length;

    if (ioctl(fd, FBIOPUT_SHOW_HIFB, &bShow) < 0)
    {
        printf("show failed!\n");
        close(fd);
        return -1;
    }

    ret = HI_TDE2_Open();
    if(ret < 0)
    {
        printf("TDE open failed!!!\n");
        return -1;
    }

    tdeHandle = HI_TDE2_BeginJob();
    if(tdeHandle == HI_ERR_TDE_INVALID_HANDLE)
    {
        printf("TDE BeginJob  failed!!!\n");
        return -1;
    }

    stOpt.bClutReload = HI_TRUE;
    ret = HI_TDE2_Bitblit(tdeHandle, NULL, NULL, pstSurface, &copyRect, &stFbSurface, &copyRect, &stOpt);
    if(ret < 0)
    {
     printf("ret=0x%x\n",ret);
        printf("TDE Blit  failed!!!\n");
        return -1;
    }

    ret = HI_TDE2_EndJob(tdeHandle, HI_FALSE, HI_TRUE, 100);

    if(ret < 0)
    {
        printf("TDE end job  failed!!!\n");
    close(fd);

        return -1;
    }
    printf("Please press Enter to show next picture!\n");
    getchar();
    close(fd);
    return 0;
}




static HI_S32 PNG_Decode(HI_CHAR *pszFilepath, PNG_RESULT_S *pstDecResult)
{
    FILE *fp = HI_NULL;
    png_structp png_ptr;
    png_infop info_ptr;
    HI_U32 u32Phyaddr = 0;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    HI_U32 i;
    int number_passes;
    HI_U32 u32RowBytes = 0;
    HI_VOID *pVir = HI_NULL;
    HI_U32 u32ChannelNum = 0;

    fp = fopen(pszFilepath, "rb");

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, HI_NULL, HI_NULL, HI_NULL);

    info_ptr = png_create_info_struct(png_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        if (pstDecResult->u32PhyAddr)
        {
            HI_MMZ_Unmap(pstDecResult->u32PhyAddr);
            HI_MMZ_Delete(pstDecResult->u32PhyAddr);
        }

        fclose(fp);
        return -1;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,&interlace_type, NULL, NULL);

    number_passes = png_set_interlace_handling(png_ptr);

    if (number_passes == 7)
    {
        pstDecResult->u32PhyAddr = 0;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return -1;
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_expand(png_ptr);
    }

    if ((color_type == PNG_COLOR_TYPE_GRAY) && (bit_depth < 8))
    {
        png_set_expand(png_ptr);
    }

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png_ptr);
    }
    else
    {
        png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
    }

    if (bit_depth == 16)
    {
        png_set_strip_16(png_ptr);
    }

    /** GRAY->RGB */
    if ((color_type == PNG_COLOR_TYPE_GRAY)
        || (color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
    {
        png_set_gray_to_rgb(png_ptr);
    }

    /** RGBA->BGRA */
    png_set_bgr(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    png_bytep row_pointers[height];

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);

    u32ChannelNum = png_get_channels(png_ptr, info_ptr);
    u32RowBytes = u32ChannelNum * bit_depth * width >> 3;
    if (u32RowBytes == 0)
    {
        u32RowBytes = 1;
    }
    pstDecResult->u32RowBytes = u32RowBytes;
    //u32RowBytes = (u32RowBytes + 0xf) & ~0xf;
u32RowBytes=HI_SYS_GET_STRIDE(u32RowBytes);
    u32Phyaddr = (HI_U32)(unsigned long)HI_MMZ_New(u32RowBytes * height, 16, HI_NULL, "PNG_IMAGE_BUF");
    if(0 == u32Phyaddr)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        if(NULL != fp)
        {
           fclose(fp);
        }
        return -1;
    }

    pVir = HI_MMZ_Map(u32Phyaddr,0);
    if(NULL == pVir)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        if(NULL != fp)
        {
           fclose(fp);
        }
        return -1;
    }


    pstDecResult->u32PhyAddr = u32Phyaddr;

    for (i = 0; i < height; i++)
    {
        row_pointers[i] = (HI_UCHAR *)pVir + i * u32RowBytes;
    }

    png_read_image(png_ptr, row_pointers);

    png_read_end(png_ptr, info_ptr);

    HI_MMZ_Unmap(u32Phyaddr);

    pstDecResult->u32Stride  = u32RowBytes;
    pstDecResult->u32Height  = height;
    pstDecResult->u32PhyAddr = u32Phyaddr;
    /* 显示*/
    {
        TDE2_SURFACE_S stSur = {0};

        stSur.enColorFmt = TDE2_COLOR_FMT_ARGB8888;

        stSur.u32Stride = u32RowBytes;
        stSur.u32Width   = width;
        stSur.u32Height  = height;
        stSur.u32PhyAddr = u32Phyaddr;
        PNG_ShowSurface(&stSur, 70, 20);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(fp);

    return 0;
}

#if 0
static HI_S32 PNG_SearchPng(const struct dirent *dir)
{

    HI_CHAR *p;
    p = strrchr(dir->d_name, '.');
    if (p == HI_NULL)
    {
        return 0;
    }
    if (strcasecmp(p, ".png") == 0)
    {
        return 1;
    }
    return 0;
}
#endif

HI_S32 main(void)
{
#if 0
    HI_U32 u32ImageNum;
    struct dirent **namelist = HI_NULL;
    HI_U32 i;
    HI_CHAR filepath[128];
    PNG_RESULT_S HwDecResult = {0};

    u32ImageNum = scandir(PNG_DIR, &namelist, PNG_SearchPng, alphasort);


    for (i = 0; i < u32ImageNum; i++)
    {
        memset(filepath, 0, 128);
        strcpy(filepath, PNG_DIR);
        strcat(filepath, namelist[i]->d_name);
        printf("%s\n", namelist[i]->d_name);

        PNG_Decode(filepath, &HwDecResult);

    }
#else
    PNG_RESULT_S HwDecResult = {0};
    PNG_Decode(PNG_FILE, &HwDecResult);
#endif
    return 0;
}
