#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <linux/fb.h>
#include <assert.h>
#include <pthread.h>

#include "hifb.h"
#include "hi_tde_api.h"
#include "hi_type.h"
#include "tde_tools.h"
#include "hi_unf_disp.h"
#include "hi_adp_mpi.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */


HI_S32 Display_Init()
{
    HI_SYS_Init();
    HIADP_MCE_Exit();
    HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    return 0;
}
HI_S32 Display_DeInit()
{
    HIADP_Disp_DeInit();
    HI_SYS_DeInit();

    return  0;
}


#define HI_MMB_SafeDelete(phyAddr) \
    do{\
        HI_S32 Ret;\
        Ret = HI_MMB_Delete(phyAddr);\
        if(HI_SUCCESS != Ret)\
        {\
            HI_MMB_Unmap(phyAddr);\
            Ret = HI_MMB_Delete(phyAddr);\
            if(HI_SUCCESS != Ret)\
            {\
                Printf("Can not delete mmz 0x%x", phyAddr);\
            }\
        }\
    }while(0)

/*****************************************************************************
 Prototype       : ST_TDE_CreateSurfaceByFile
 Description     : 根据位图创建surface
 Input           : file        **
                   pstSurface  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/5/21
    Author       : wming
    Modification : Created function

*****************************************************************************/
HI_S32 HI_TDE_CreateSurfaceByFile_D(char *file, TDE2_SURFACE_S *pstSurface, HI_U32 lineNum)
{
    FILE *fp;
    HI_U32 colorfmt, w, h, stride;
    HI_U8 *pVirt;
    HI_CHAR tmpStr[12];
    snprintf(tmpStr, 11, "%d", lineNum);
    fp = fopen(file, "rb");
    if (NULL == fp)
    {
        Printf("error when open file %s, line:%d\n", file, __LINE__);
        return -1;
    }

    memset(pstSurface, 0, sizeof(TDE2_SURFACE_S));
    fread(&colorfmt, 1, 4, fp);
    fread(&w, 1, 4, fp);
    fread(&h, 1, 4, fp);
    fread(&stride, 1, 4, fp);

    pstSurface->enColorFmt = colorfmt;
    pstSurface->u32Width = w;
    pstSurface->u32Height = h;
    pstSurface->u32Stride = stride;
    pstSurface->u8Alpha0 = 0;
    pstSurface->u8Alpha1 = 0xff;
    pstSurface->bAlphaMax255 = HI_TRUE;
    pstSurface->bAlphaExt1555 = HI_TRUE;


    if ((colorfmt >= TDE2_COLOR_FMT_CLUT1)
        && (colorfmt <= TDE2_COLOR_FMT_ACLUT88))
    {
        pstSurface->pu8ClutPhyAddr = (HI_U8 *)(unsigned long)HI_MMB_New(256 * 4, 1, NULL, "sample_tde");
        if (NULL == pstSurface->pu8ClutPhyAddr)
        {
            fclose(fp);
            Printf("error when call HI_MMB_New, line:%d\n", __LINE__);
            Printf("error when call HI_MMB_New, line:%d\n", __LINE__);
            return -1;
        }

        pVirt = (HI_U8 *)HI_MMB_Map((HI_U32)(unsigned long)pstSurface->pu8ClutPhyAddr, 0);
        fread(pVirt, 1, 256 * 4, fp);
        HI_MMB_Unmap((HI_U32)(unsigned long)pstSurface->pu8ClutPhyAddr);
    }
    else if (TDE2_COLOR_FMT_ARGB1555 == colorfmt)
    {
        pstSurface->u8Alpha0 = 0x0;
        pstSurface->u8Alpha1 = 0xff;
    }


    pstSurface->u32PhyAddr = (HI_U32)(unsigned long)HI_MMB_New((stride) * h, 1, NULL, tmpStr);

    if (0 == pstSurface->u32PhyAddr)
    {
        fclose(fp);
        Printf("error when call HI_MMB_New, size =0x%x line:%d\n", (stride) * h, __LINE__);
        Printf("error when call HI_MMB_New, line:%d\n", __LINE__);
        if (pstSurface->pu8ClutPhyAddr)
        {
            HI_MMB_SafeDelete((HI_U32)(unsigned long)(pstSurface->pu8ClutPhyAddr));
        }
        return -1;
    }
    pVirt = (HI_U8 *)HI_MMB_Map(pstSurface->u32PhyAddr, 0);
    fread(pVirt, 1, stride * h, fp);

    HI_MMB_Unmap(pstSurface->u32PhyAddr);

    fclose(fp);
    HI_MMZ_Flush(0);
    return 0;
}

HI_S32 ParseFileName(char *file, TDE2_MB_S *pstMBSur)
{
    HI_U32 i, j;
    char width[8];
    char height[8];
    char format[8];
    char ystride[8];
    char cstride[8];

    i = j = 0;

    file += strlen("../res/");

    while (!((file[i] == 'X') || (file[i] == 'x')))
    {
        format[j++] =  file[i++];
    }

    format[j] = '\0';
    i++;
    j = 0;
    while (!((file[i] == 'X') || (file[i] == 'x')))
    {
        width[j++] = file[i++];
    }
    width[j] = '\0';

    i++;
    j = 0;
    while (!((file[i] == 'X') || (file[i] == 'x')))
    {
        height[j++] = file[i++];
    }
    height[j] = '\0';

    i++;
    j = 0;
    while (!((file[i] == 'X') || (file[i] == 'x')))
    {
        ystride[j++] = file[i++];
    }
    ystride[j] = '\0';

    i++;
    j = 0;
    while (file[i] != '.')
    {
        cstride[j++] = file[i++];
    }
    cstride[j] = '\0';
    pstMBSur->u32YWidth = strtoul(width, (char **)NULL, 10);
    pstMBSur->u32YHeight = strtoul(height, (char **)NULL, 10);
    pstMBSur->u32YStride = strtoul(ystride, (char **)NULL, 10);
    pstMBSur->u32CbCrStride = strtoul(cstride, (char **)NULL, 10);
    if (!strncasecmp(format, "JPG400", 6))
    {
        pstMBSur->enMbFmt = TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP;
    }
    else if (!strncasecmp(format, "JPG420", 6))
    {
        pstMBSur->enMbFmt = TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP;
    }
    else if (!strncasecmp(format, "JPG422H", 7))
    {
        pstMBSur->enMbFmt = TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP;
    }
    else if (!strncasecmp(format, "JPG422V", 7))
    {
        pstMBSur->enMbFmt = TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP;
    }
    else if (!strncasecmp(format, "JPG444", 6))
    {
        pstMBSur->enMbFmt = TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP;
    }
    else
    {
        Printf("unsupported format!\n");
    }

    return 0;
}

HI_S32 HI_TDE_CreateMBSurfaceByFile_D(char *file, TDE2_MB_S *pstMBSur, HI_U32 lineNum)
{
    FILE *fp;
    HI_U8 *pVirt;
    HI_CHAR tmpStr[12];
    memset(pstMBSur, 0, sizeof(TDE2_MB_S));

    if (ParseFileName(file, pstMBSur) < 0)
    {
        return -1;
    }

    fp = fopen(file, "rb");
    if (NULL == fp)
    {
        Printf("error when open file %s, line:%d\n", file, __LINE__);
        return -1;
    }
    snprintf(tmpStr, 11, "%d", lineNum);
    pstMBSur->u32YPhyAddr = (HI_U32)(unsigned long)HI_MMB_New(pstMBSur->u32YStride * pstMBSur->u32YHeight, 4, NULL, tmpStr);
    if (0 == pstMBSur->u32YPhyAddr)
    {
        fclose(fp);
        Printf("error when call HI_MMB_New, line:%d\n", __LINE__);
        return -1;
    }
    pVirt = (HI_U8 *)HI_MMB_Map(pstMBSur->u32YPhyAddr, 0);

    fread(pVirt, 1, pstMBSur->u32YStride * pstMBSur->u32YHeight, fp);
    HI_MMB_Unmap(pstMBSur->u32YPhyAddr);

    if (pstMBSur->u32CbCrStride)
    {
        HI_U32 u32CHeight;
        if ((pstMBSur->enMbFmt == TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP)
            || (pstMBSur->enMbFmt == TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP))
        {
            u32CHeight = pstMBSur->u32YHeight / 2;
        }
        else
        {
            u32CHeight = pstMBSur->u32YHeight;
        }
        pstMBSur->u32CbCrPhyAddr = (HI_U32)(unsigned long)HI_MMB_New(pstMBSur->u32CbCrStride * u32CHeight, 4, NULL, "sample_tde");
        if (0 == pstMBSur->u32YPhyAddr)
        {
            HI_MMB_SafeDelete(pstMBSur->u32YPhyAddr);
            fclose(fp);
            Printf("error when call HI_MMB_New, line:%d\n", __LINE__);
            return -1;
        }

        pVirt = (HI_U8 *)HI_MMB_Map(pstMBSur->u32CbCrPhyAddr, 0);

        fread(pVirt, 1, pstMBSur->u32CbCrStride * u32CHeight, fp);

#ifdef CONVERT_CB_CR
        {
            HI_U32 ii, jj;
            HI_U16 *pData;
            HI_U32 CWidth;

            switch (pstMBSur->enMbFmt)
            {
                case TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP:
                {
                    CWidth = 0;
                    break;
                }
                case TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP:
                {
                    CWidth = pstMBSur->u32YWidth >> 1;
                    break;
                }
                case TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP:
                case TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP:
                case TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI:
                case TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP:
                {
                    CWidth = pstMBSur->u32YWidth >> 1;
                    HI_MMB_Unmap(pstMBSur->u32CbCrPhyAddr);
                    fclose(fp);
                    return 0;
                    break;
                }
                case TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP:
                case TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP:
                {
                    CWidth = pstMBSur->u32YWidth;
                    break;
                }
                default:
                    return HI_FAILURE;
            }
            for ( ii = 0;  ii < u32CHeight; ii++ )
            {
                pData = (HI_U16 *)(pVirt + pstMBSur->u32CbCrStride * ii);
                for ( jj = 0; jj < CWidth; jj++ )
                {
                    *pData = (((*pData >> 8) & 0xff )) | ((*pData & 0xff ) << 8);
                    pData++;
                }
            }
        }
#endif
        HI_MMB_Unmap(pstMBSur->u32CbCrPhyAddr);
    }
    fclose(fp);

    return 0;
}


HI_S32 HI_TDE_DuplicateSurface(TDE2_SURFACE_S *pstInSurface, TDE2_SURFACE_S *pstOutSurface)
{
    HI_U8 *pVirtOut;

    memcpy(pstOutSurface, pstInSurface, sizeof(TDE2_SURFACE_S));
    pstOutSurface->pu8ClutPhyAddr = NULL;
    pstOutSurface->enColorFmt = TDE2_COLOR_FMT_ARGB8888;
    pstOutSurface->u32Stride = pstOutSurface->u32Width * 4;
    pstOutSurface->u32PhyAddr = (HI_U32)(unsigned long)HI_MMB_New(pstOutSurface->u32Stride * pstOutSurface->u32Height, 4, NULL, "sample_tde");
    if (0 == pstOutSurface->u32PhyAddr)
    {
        Printf("error when call HI_MMB_New, line:%d\n", __LINE__);
        return -1;
    }

    pVirtOut = (HI_U8 *)HI_MMB_Map(pstOutSurface->u32PhyAddr, 0);
    memset(pVirtOut, 0, pstOutSurface->u32Stride * pstOutSurface->u32Height);
    HI_MMB_Unmap(pstOutSurface->u32PhyAddr);

    return 0;
}

/*****************************************************************************
 Prototype       : ST_TDE_DestroySurface
 Description     : 销毁surface，主要释放物理内存
 Input           : pstSurface  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/5/21
    Author       : wming
    Modification : Created function

*****************************************************************************/
HI_S32 HI_TDE_DestroySurface(TDE2_SURFACE_S *pstSurface)
{
    if (pstSurface->u32PhyAddr)
    {
        HI_MMB_SafeDelete(pstSurface->u32PhyAddr);
        pstSurface->u32PhyAddr = 0;
    }

    if (pstSurface->pu8ClutPhyAddr)
    {
        HI_MMB_SafeDelete((HI_U32)(unsigned long)(pstSurface->pu8ClutPhyAddr));
        pstSurface->pu8ClutPhyAddr = NULL;
    }

    return 0;
}

HI_S32 HI_TDE_DestroyMBSurface(TDE2_MB_S *pstMBSur)
{
    if (pstMBSur->u32YPhyAddr)
    {
        HI_MMB_SafeDelete(pstMBSur->u32YPhyAddr);
        pstMBSur->u32YPhyAddr = 0;
    }

    if (pstMBSur->u32CbCrPhyAddr)
    {
        HI_MMB_SafeDelete((HI_U32)(unsigned long)(pstMBSur->u32CbCrPhyAddr));
        pstMBSur->u32CbCrPhyAddr = 0;
    }

    return 0;
}

HI_S32 HI_TDE_ShowSurface( TDE2_SURFACE_S *pstSurface, HI_U32 u32PosX, HI_U32 u32PosY)
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
    HI_S32 ret ;
    ret = Display_Init();
    if (HI_SUCCESS != ret)
    {
        return -1;
    }

    fd = open("/dev/fb0", O_RDWR);
    if (fd < 0)
    {
        Printf("open fb0 failed!\n");
        return -1;
    }
    if (ioctl(fd, FBIOPUT_SCREEN_ORIGIN_HIFB, &stPoint) < 0)
    {
        Printf("set screen original show position failed!\n");
        close(fd);
        return -1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0)
    {
        Printf("Get variable screen info failed!\n");
        close(fd);
        return -1;
    }

    var.xres = var.xres_virtual = pstSurface->u32Width;
    var.yres = pstSurface->u32Height;
    var.yres_virtual = pstSurface->u32Height;

    var.transp = a32;
    var.red = r32;
    var.green = g32;
    var.blue = b32;
    var.bits_per_pixel = 32;

    if (ioctl(fd, FBIOPUT_VSCREENINFO, &var) < 0)
    {
        Printf("Put variable screen info failed!\n");
        close(fd);
        return -1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0)
    {
        Printf("Put variable screen info failed!\n");
        close(fd);
        return -1;
    }


    if (ioctl(fd, FBIOGET_FSCREENINFO, &fix) < 0)
    {
        Printf("Get fix screen info failed!\n");
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

    if (ioctl(fd, FBIOPAN_DISPLAY, &var) < 0)
    {
        printf("pan_display failed!\n");
        close(fd);
        return -1;
    }

    ret = HI_TDE2_Open();
    if (ret < 0)
    {
        Printf("%s, line %d, ret = 0x%x",  __FUNCTION__, __LINE__, ret);
        return -1;
    }

    tdeHandle = HI_TDE2_BeginJob();
    if (tdeHandle == HI_ERR_TDE_INVALID_HANDLE)
    {
        Printf("%s, line %d, tdeHandle = 0x%x",  __FUNCTION__, __LINE__, tdeHandle);
        return -1;
    }

    stOpt.bClutReload = HI_TRUE;
    ret = HI_TDE2_Bitblit(tdeHandle, NULL, NULL, pstSurface, &copyRect, &stFbSurface, &copyRect, &stOpt);
    if (ret < 0)
    {
        Printf("%s, line %d, ret = 0x%x",  __FUNCTION__, __LINE__, ret);
        return -1;
    }

    ret = HI_TDE2_EndJob(tdeHandle, HI_FALSE, HI_TRUE, 100);

    if (ret < 0)
    {
        Printf("%s, line %d, ret = 0x%x",  __FUNCTION__, __LINE__, ret);
        close(fd);

        return -1;
    }
    getchar();
    close(fd);
    Display_DeInit();
    return 0;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

