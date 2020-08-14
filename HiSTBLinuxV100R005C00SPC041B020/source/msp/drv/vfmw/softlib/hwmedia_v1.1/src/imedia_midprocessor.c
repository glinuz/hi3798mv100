/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCO4ir2/uQ7BFDYEYESXdzFFpPTRP1CkEuTQUs7
G00Y4bEQHXqeukQXzDlltu5030LZTdLgpx0KHpnKRxTpRyOU0a04zumXQiTqY9/XaFxzHWEU
9d1pRuefl9rWjGJksq9Z8NrrytAQ1iloYHeVDvi035wXqfC4gxYs+QKNgL1qrw==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/**************************************************************************************************************
Copyright (C), 2009-2012, Huawei Tech. Co., Ltd.
File name: imedia_midprocessor.c
Author & ID: 郭姗+00101841
Version: 1.00
Date:  2009-12-18
Description: 本文件包括了中间处理模块的接口函数及相关结构定义
Function List:
1. IMedia_Image_Create 创建算法通道
2. IMedia_Image_Control 算法通道控制函数
3. IMedia_Image_Process 算法通道处理函数
4. IMedia_Image_Delete 删除算法通道
History:
1.20091218 编码完成 郭姗+00101841
2.20100209 代码review后修改 郭姗+00101841

* Revision 1.01  2010/03/04 16:0:00  guoshan+00101841
* .R.模块启用下，场特性序列采用分场的下采样方法

* Revision 1.02  2010/03/05 16:0:00  guoshan+00101841
* .R.模块启用下，满足resize模块对图像宽度16像素整数倍的需求，用WStride代替原始Width

* Revision 1.03  2010/03/19 10:40:00  郭姗+00101841
* B010版本测试后修改代码
*****************************************************************************************************************/

#include "imedia_midprocessor.h"
#include "iMedia_util.h"
#include "deinterlace.h"
#include "resize.h"

typedef struct STRU_MID_PROCESSOR_CTX
{
    INT32 iCrcMask;      // 防错码
    INT32 iProcessType;  // 同ENUM_IMAGE_PROCESS_TYPE对应
    void *pSubCtx;       // sub module handler

    STRU_IMEDIA_CPU_CAPS stCpuCaps;     // CPU能力集，如MMX、SSE等
    STRU_IMEDIA_MEMBUF   stMemBuf;
    STRU_IMAGE_PARAMS    stImageParams; // 存储信息，便于resize调用

} STRU_MID_PROCESSOR_CTX;

/*guoshan+00101841 20100829*/
/*在KeepAspectRatio模式下，提前检测resize模块的参数是否合法*/
#define WIDTH_ALIGN 16
#define HEIGHT_ALIGN 2

static INT32 Resize_CheckParam( UINT16 usSrcW, UINT16 usSrcH, UINT16 usSrcPitch, UINT16 usDstW, UINT16 usDstH, UINT16 usDstPitch);
/*=============================================================================================================
* 函数说明：
    创建算法通道。
* 输入参数：
    eType      IMAGE_PROCESS_TYPE，降采样或去交错算法
    params     一些基本信息的结构体指针，包括原始和目标图像的宽高等信息
* 输出参数：
    pctx       算法通道句柄
* 返 回 值：
    0-成功，其他值见返回值定义。
===============================================================================================================*/
EXPORT_API INT32 IMedia_Image_Create(ENUM_IMAGE_PROCESS_TYPE eType, STRU_IMAGE_PARAMS *params, IMEDIA_IMAGE_CTX *pctx)
{
    STRU_MID_PROCESSOR_CTX *pstMidProcCtx = NULL;
    INT32 iRet = IMEDIA_RET_SUCCESS;

    /* 2010/03/05 15:25:00 guoshan+00101841*/
    /* 修改传给resize模块接口的变量，以保证图像usWidth是16的整数倍 */
    /* 2010/03/17 18:55:00 guoshan+00101841*/
    /* 为保持与SRS一致，对真实的width进行范围检测,取消此变量 */
    //     UINT16 usSrcWMaxStride = 0;
    //     UINT16 usDstWMaxStride = 0;

    if (NULL == params)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "image parameters NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "pctx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    *pctx = NULL;

    g_stGlobalInfo.bMallocFxnUsed = TRUE;

    /*create mid-processor channel handler*/
    pstMidProcCtx = IMedia_Malloc(sizeof(STRU_MID_PROCESSOR_CTX));

    if (NULL == pstMidProcCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "failed to malloc mid_ctx memory\n");
        return  IMEDIA_RET_MEM_MALLOC_FAIL;
    }

    IMEDIA_LOG(IMEDIA_INFO, "malloc mid_ctx = %p succeed\n", pstMidProcCtx);

    memset(pstMidProcCtx, 0, sizeof(STRU_MID_PROCESSOR_CTX));
    memcpy(&pstMidProcCtx->stImageParams, params, sizeof(STRU_IMAGE_PARAMS));
    pstMidProcCtx->iCrcMask = CRC_MASK;
    pstMidProcCtx->iProcessType = eType;

    /* 2010/04/29 18:05:00 songxiaogang+00133955 */
    /* 当开启汇编优化时，自动获取CPU指令集 */
    if (!(params->iFlags & IMEDIA_FLAG_DISABLE_ASM))
    {
        iRet = IMedia_GetCpuCaps(&pstMidProcCtx->stCpuCaps);
    }

    /* 2010/03/04 18:55:00 guoshan+00101841*/
    /* 为保证内外接口RESIZE_QUALITY_LEVEL的一致性，外部质量等级需+1后处理 */
    pstMidProcCtx->stImageParams.iSubType = params->iSubType + 1;

    /* 2010/03/17 18:55:00 guoshan+00101841*/
    /* 为保持与SRS一致，对子模块传送真实width值 */
    //     usSrcWMaxStride = IVIDEO_WIDTH_STRIDE(params->usMaxSrcWidth);
    //     usDstWMaxStride = IVIDEO_WIDTH_STRIDE(params->usMaxDstWidth);

    switch (eType)
    {
        case IMAGE_RESIZE:
        {
            /*inquiry about the memory buffer resizing module need*/
            pstMidProcCtx->stMemBuf.uiAlignment = 16;  //16 bit对齐

            /* 2010/03/05 15:25:00 guoshan+00101841*/
            /* 修改传给resize模块接口的变量，以保证图像usWidth是16的整数倍 */
            //             iRet = RESIZE_QueryMem((RESIZE_QUALITY_LEVEL)(params->iSubType + 1), params->usMaxSrcWidth, params->usMaxSrcHeight,
            //                                   params->usMaxDstWidth, params->usMaxDstHeight, (INT32 *)&pstMidProcCtx->stMemBuf.uiSize);
            //             iRet = RESIZE_QueryMem((RESIZE_QUALITY_LEVEL)pstMidProcCtx->stImageParams.iSubType, usSrcWMaxStride, params->usMaxSrcHeight,
            //                                                    usDstWMaxStride, params->usMaxDstHeight, (INT32 *)&pstMidProcCtx->stMemBuf.uiSize);

            /* 2010/03/17 18:55:00 guoshan+00101841*/
            /* 为保持与SRS一致，向子模块传送真实的width值 */
            iRet = RESIZE_QueryMem((RESIZE_QUALITY_LEVEL)pstMidProcCtx->stImageParams.iSubType, params->usMaxSrcWidth, params->usMaxSrcHeight,
                                   params->usMaxDstWidth, params->usMaxDstHeight, (INT32 *)&pstMidProcCtx->stMemBuf.uiSize);

            if (iRet)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] RESIZE_QueryMem(%hu,%hu,%hu,%hu) = %d failed\n",
                           pstMidProcCtx, params->usMaxSrcWidth, params->usMaxSrcHeight, params->usMaxDstWidth, params->usMaxDstHeight, iRet);
                IMEDIA_LOG(IMEDIA_ERROR, "pfnfree mid_ctx = %p\n", pstMidProcCtx);
                IMedia_Free(pstMidProcCtx);
                pstMidProcCtx = NULL;
                return  IMEDIA_RET_OTHER;
            }

            IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] RESIZE_QueryMem(%hu,%hu,%hu,%hu) = %lu succeed\n",
                       pstMidProcCtx, params->usMaxSrcWidth, params->usMaxSrcHeight, params->usMaxDstWidth, params->usMaxDstHeight, pstMidProcCtx->stMemBuf.uiSize);

            /*malloc the memory buffer resizing module need*/
            pstMidProcCtx->stMemBuf.pucbase = IMedia_AlignMalloc(pstMidProcCtx->stMemBuf.uiSize);

            if (NULL == pstMidProcCtx->stMemBuf.pucbase)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] malloc(%lu) failed and free ctx\n", pstMidProcCtx, pstMidProcCtx->stMemBuf.uiSize);
                IMedia_Free(pstMidProcCtx);
                pstMidProcCtx = NULL;
                return  IMEDIA_RET_MEM_MALLOC_FAIL;
            }

            IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] malloc subMemBuf(%p) size(%u)\n", pstMidProcCtx, pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

            // create过程放在process函数中处理
        }
        break;

        case IMAGE_DEINTERLACE:
        {
            /*inquiry about the memory buffer de-interlacing module needed*/
            pstMidProcCtx->stMemBuf.uiAlignment = 16;  //16 bit对齐

            iRet = DEINTERLACE_QueryMem(params->usMaxSrcWidth, params->usMaxSrcHeight, &pstMidProcCtx->stMemBuf.uiSize);

            if (iRet)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] DEINTERLACE_QueryMem(%hu, %hu) = %d failed\n", pstMidProcCtx, params->usMaxSrcWidth, params->usMaxSrcHeight, iRet);
                IMEDIA_LOG(IMEDIA_ERROR, "pfnfree mid_ctx = %p\n", pstMidProcCtx);
                IMedia_Free(pstMidProcCtx);
                pstMidProcCtx = NULL;
                return  IMEDIA_RET_OTHER;
            }

            IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] DEINTERLACE_QueryMem(%hu, %hu) = %lu succeed\n", pstMidProcCtx, params->usMaxSrcWidth, params->usMaxSrcHeight, pstMidProcCtx->stMemBuf.uiSize);

            /*malloc the memory buffer de-interlacing module needed*/
            pstMidProcCtx->stMemBuf.pucbase = IMedia_AlignMalloc(pstMidProcCtx->stMemBuf.uiSize);

            if (NULL == pstMidProcCtx->stMemBuf.pucbase)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] malloc(%lu) failed and free ctx\n", pstMidProcCtx, pstMidProcCtx->stMemBuf.uiSize);
                IMedia_Free(pstMidProcCtx);
                pstMidProcCtx = NULL;
                return  IMEDIA_RET_MEM_MALLOC_FAIL;
            }

            IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] malloc subMemBuf(%p) size(%u)\n", pstMidProcCtx, pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

            /*create de-interlace channel handler*/
            iRet = DEINTERLACE_Create(&pstMidProcCtx->pSubCtx, &pstMidProcCtx->stCpuCaps, params->usMaxSrcWidth, params->usMaxSrcHeight,
                                      pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

            if (NULL == pstMidProcCtx->pSubCtx)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] DEINTERLACE_Create = %d failed and free pucbase[%p] & ctx\n", pstMidProcCtx, iRet, pstMidProcCtx->stMemBuf.pucbase);

                /*free malloc buf*/
                IMedia_AlignFree(pstMidProcCtx->stMemBuf.pucbase);
                pstMidProcCtx->stMemBuf.pucbase = NULL;

                IMedia_Free(pstMidProcCtx);
                pstMidProcCtx = NULL;
                return  IMEDIA_RET_OBJ_CREATE_FAIL;
            }

            IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] DEINTERLACE_Create succeed sub_ctx = %p\n", pstMidProcCtx, pstMidProcCtx->pSubCtx);
        }
        break;

        default:
        {
            IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] eType wrong\n", pstMidProcCtx);
            IMedia_Free(pstMidProcCtx);
            pstMidProcCtx = NULL;
            return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        }

        //    break;
    }

    *pctx = pstMidProcCtx;

    IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] create succeed\n", pstMidProcCtx);

    return IMEDIA_RET_SUCCESS;
}

/*=============================================================================================================
* 函数说明：
    通道控制函数。
* 输入参数：
    ctx     算法通道句柄
    cmd     控制参数的命令码
    IMEDIA_GET_VERSION    返回版本信息         wparam(STRU_IMEDIA_VERSION*)          lparam(NULL)
    wparam  每种命令对应各自类型，可作为输入或输出
    lparam  每种命令对应各自类型，可作为输入或输出
* 输出参数：
    wparam  每种命令对应各自类型，可作为输入或输出
    lparam  每种命令对应各自类型，可作为输入或输出
* 返 回 值：
    0-成功，其他值见返回值定义。
===============================================================================================================*/
EXPORT_API INT32 IMedia_Image_Control(IMEDIA_IMAGE_CTX ctx, ENUM_IMEDIA_CMD cmd, void *wparam, void *lparam)
{
    switch (cmd)
    {
            /*返回版本信息*/
        case IMEDIA_GET_VERSION:
        {
            STRU_IMEDIA_VERSION *pstVersion = (STRU_IMEDIA_VERSION *)wparam;

            if (NULL == pstVersion)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "wparam NULL \n");
                return IMEDIA_RET_PARAM_NULL;
            }

            strncpy(pstVersion->cVersionChar, "iMedia MS-MID V100R001P301", IMEDIA_VERSION_LENGTH);
            strncpy(pstVersion->cReleaseTime, __TIME__ " " __DATE__, IMEDIA_TIME_LENGTH);
#if defined(_MSC_VER)
            pstVersion->uiCompileVersion = IMEDIA_FOURCC('V', 'S', '9', '0');
#else
            pstVersion->uiCompileVersion = IMEDIA_FOURCC('G', 'C', 'C', '4');
#endif
        }
        break;

        default:
        {
            IMEDIA_LOG(IMEDIA_ERROR, "invalid cmd\n");
            return IMEDIA_RET_CMD_INVALID;
        }

        //    break;
    }

    return IMEDIA_RET_SUCCESS;
}

/*=======================================================================================================================
* 函数说明：
    算法通道处理函数。
* 输入参数：
    ctx        算法通道句柄
    pstInArgs  输入参数信息（输入图像地址及帧场信息等）
* 输出参数：
    pstOutArgs 输出参数信息（输出图像地址及帧场信息等）
* 返 回 值：
    0-成功，其他值见返回值定义。
=========================================================================================================================*/
EXPORT_API INT32 IMedia_Image_Process(IMEDIA_IMAGE_CTX ctx, STRU_IMAGE_IN_ARGS *pstInArgs, STRU_IMAGE_OUT_ARGS *pstOutArgs)
{
    STRU_MID_PROCESSOR_CTX *pstMidProcCtx = (STRU_MID_PROCESSOR_CTX *)ctx;

    INT32  iRet = IMEDIA_RET_SUCCESS;

    /*输入参数合法性检测*/
    /*handler非空*/
    if (NULL == pstMidProcCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "pMidProcCtx NULL \n");
        return IMEDIA_RET_PARAM_NULL;
    }

    /*handler正确*/
    if (CRC_MASK != pstMidProcCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] iCrcMask[0x%x] wrong\n", pstMidProcCtx, pstMidProcCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    /*输入/输出参数结构体非空*/
    if (NULL == pstInArgs || NULL == pstOutArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] inargs or outargs NULL\n", pstMidProcCtx);
        return IMEDIA_RET_PARAM_NULL;
    }

    /* 2010/04/24 17:05:00 songxiaogang+00133955 */
    /* 保证输出参数的值始终有效 */
    memset(pstOutArgs, 0, sizeof(STRU_IMAGE_OUT_ARGS));

    /*输入图像非空*/
    if (NULL == pstInArgs->pstSource)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] source NULL\n", pstMidProcCtx);
        return IMEDIA_RET_PARAM_IN_VALUE_NULL;
    }

    /*输入图像Y,U,V平面非空*/
    /* 2010/03/19 10:15:00 guoshan+00101841*/
    /* 增加对pstInArgs->pstSource->apucBuf的防错检测 */
    if (NULL == pstInArgs->pstSource->apucBuf || NULL == pstInArgs->pstSource->apucBuf[0] || NULL == pstInArgs->pstSource->apucBuf[1] || NULL == pstInArgs->pstSource->apucBuf[2])
    {
        IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] source Y || U || V buffer NULL\n", pstMidProcCtx);
        return IMEDIA_RET_PARAM_IN_VALUE_NULL;
    }

    /*输出图像空*/
    if (NULL == pstInArgs->pucTargetBuf)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] inArgs->pucTargetBuf = NULL\n", pstMidProcCtx);
        return IMEDIA_RET_PARAM_IN_VALUE_NULL;
    }

    /*指明输出图像Y,U,V平面的位置*/
    pstOutArgs->stTarget.apucBuf[0] = pstInArgs->pucTargetBuf;
    pstOutArgs->stTarget.apucBuf[1] = pstOutArgs->stTarget.apucBuf[0] + IVIDEO_Y_SIZE(pstInArgs->usTargetWidth, pstInArgs->usTargetHeight);
    pstOutArgs->stTarget.apucBuf[2] = pstOutArgs->stTarget.apucBuf[1] + IVIDEO_UV_SIZE(pstInArgs->usTargetWidth, pstInArgs->usTargetHeight);
    pstOutArgs->stTarget.eContentType = pstInArgs->pstSource->eContentType;
    pstOutArgs->stTarget.usWidth = pstInArgs->usTargetWidth;
    pstOutArgs->stTarget.usHeight = pstInArgs->usTargetHeight;
    pstOutArgs->stTarget.usWidthPitch = IVIDEO_WIDTH_STRIDE(pstInArgs->usTargetWidth);
    pstOutArgs->stTarget.stAspectRatio.usSarWidth = pstInArgs->pstSource->stAspectRatio.usSarWidth;
    pstOutArgs->stTarget.stAspectRatio.usSarHeight = pstInArgs->pstSource->stAspectRatio.usSarHeight;

    switch (pstMidProcCtx->iProcessType)
    {
        case IMAGE_DEINTERLACE:
        {
            if ((pstInArgs->usTargetWidth != pstInArgs->pstSource->usWidth) || (pstInArgs->usTargetHeight != pstInArgs->pstSource->usHeight))
            {
                IMEDIA_LOG(IMEDIA_ERROR, "outW[%d] != inW[%d]) || outH[%d] != inH[%d])\n",
                           pstInArgs->usTargetWidth, pstInArgs->pstSource->usWidth, pstInArgs->usTargetHeight, pstInArgs->pstSource->usHeight );
                //    return IMEDIA_RET_PARAM_INVALID;
                iRet = IMEDIA_RET_PARAM_INVALID;
                break;
            }

            if (pstInArgs->pstSource->eContentType != IVIDEO_INTERLACED)
            {
                //    IMEDIA_LOG(IMEDIA_WARNING, "non-interlaced frames, de-interlace continue...\n");

                /* 2010/03/17 15:50:00 guoshan+00101841*/
                /* 应产品要求，deinterlace模块启用时，发现非interlaced序列，报错后退出（不处理） */
                IMEDIA_LOG(IMEDIA_ERROR, "de-interlacing non-interlaced frames...\n");
                iRet = IMEDIA_RET_PARAM_INVALID;
                break;
            }

            /*调用deinterlace处理主函数*/
            iRet = DEINTERLACE_Process(pstMidProcCtx->pSubCtx, pstInArgs->pstSource->apucBuf, pstOutArgs->stTarget.apucBuf, pstInArgs->pstSource->usWidth,
                                       pstInArgs->pstSource->usHeight, pstInArgs->pstSource->usWidthPitch, pstOutArgs->stTarget.usWidthPitch);

            /*经过de-interlace处理后的图像一定是IVIDEO_PROGRESSIVE*/
            pstOutArgs->stTarget.eContentType = IVIDEO_PROGRESSIVE;
        }
        break;

        case IMAGE_RESIZE:
        {
            INT32 iCpuCap = 0;
            /*20100826 16:50:00 guoshan+00101841*/
            /*根据pstInArgs->bKeepSar进行resize*/
            UINT16 usDstWidth = pstOutArgs->stTarget.usWidth;
            UINT16 usDstHeight = pstOutArgs->stTarget.usHeight;
            UINT16 usDeltaDstWHalf = 0;
            UINT16 usDeltaDstHHalf = 0;
            UINT8 *apucOutYUVBuf[3];

            apucOutYUVBuf[0] = pstOutArgs->stTarget.apucBuf[0];
            apucOutYUVBuf[1] = pstOutArgs->stTarget.apucBuf[1];
            apucOutYUVBuf[2] = pstOutArgs->stTarget.apucBuf[2];

            /*增加WidthStride传给resize模块*/
            /* 2010/03/19 11:15:00 guoshan+00101841*/
            /* 保证与SRS的一致性，取消此修订 */
            //             UINT16 usSrcWMaxStride = IVIDEO_WIDTH_STRIDE(pstInArgs->pstSource->usWidth);
            //             UINT16 usDstWMaxStride = IVIDEO_WIDTH_STRIDE(pstInArgs->usTargetWidth);

            if (pstInArgs->pstSource->usWidth > pstMidProcCtx->stImageParams.usMaxSrcWidth ||
                pstInArgs->pstSource->usHeight > pstMidProcCtx->stImageParams.usMaxSrcHeight ||
                pstInArgs->usTargetWidth > pstMidProcCtx->stImageParams.usMaxDstWidth ||
                pstInArgs->usTargetHeight > pstMidProcCtx->stImageParams.usMaxDstHeight)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "inW[%d] > maxInW[%d] || inH[%d] > maxInH[%d]) || outW[%d] > maxOutW[%d] || outH[%d] > maxOutH[%d]\n",
                           pstInArgs->pstSource->usWidth, pstMidProcCtx->stImageParams.usMaxSrcWidth,
                           pstInArgs->pstSource->usHeight, pstMidProcCtx->stImageParams.usMaxSrcHeight,
                           pstInArgs->usTargetWidth, pstMidProcCtx->stImageParams.usMaxDstWidth,
                           pstInArgs->usTargetHeight, pstMidProcCtx->stImageParams.usMaxDstHeight);
                //    return IMEDIA_RET_PARAM_INVALID;
                iRet = IMEDIA_RET_PARAM_INVALID;
                break;
            }

            // 目前resize只有两条路径，可以先按这种方式简单处理，后续支持指令集有扩充的话该种方法存在问题 s00133955
            if (pstMidProcCtx->stCpuCaps.uiCpuFlag & IMEDIA_CPU_SSE4)
            {
                /* 2010/03/08 16:25:00 songxg+00133955 */
                /* 与resize.c模块中的宏处理方式保持一致 */
                iCpuCap = RESIZE_CPU_MMX | RESIZE_CPU_SSE | RESIZE_CPU_SSE2 | RESIZE_CPU_SSE3 | RESIZE_CPU_SSSE3 | RESIZE_CPU_SSE4;
            }

            if ( TRUE == pstInArgs->bKeepSar)
            {
                /*20100826 16:50:00 guoshan+00101841*/
                UINT32 uiDstW = 0;
                UINT32 uiDstH = 0;
                UINT16 usSarWidth = pstInArgs->pstSource->usWidth;
                UINT16 usSarHeight = pstInArgs->pstSource->usHeight;

                iRet = Resize_CheckParam(pstInArgs->pstSource->usWidth, pstInArgs->pstSource->usHeight, pstInArgs->pstSource->usWidthPitch,
                                         pstOutArgs->stTarget.usWidth, pstOutArgs->stTarget.usHeight, pstOutArgs->stTarget.usWidthPitch);

                if (iRet)
                {
                    IMEDIA_LOG(IMEDIA_ERROR, "Resize params invalid,iRet = [%d]\n", iRet);
                    break;
                }

                memset(pstOutArgs->stTarget.apucBuf[0], 0, IVIDEO_Y_SIZE(pstInArgs->usTargetWidth, pstInArgs->usTargetHeight));
                memset(pstOutArgs->stTarget.apucBuf[1], 128, IVIDEO_UV_SIZE(pstInArgs->usTargetWidth, pstInArgs->usTargetHeight));
                memset(pstOutArgs->stTarget.apucBuf[2], 128, IVIDEO_UV_SIZE(pstInArgs->usTargetWidth, pstInArgs->usTargetHeight));

                uiDstW = usDstHeight * usSarWidth / usSarHeight;

                if ((UINT16)uiDstW > usDstWidth)
                {
                    uiDstH = usDstWidth * usSarHeight / usSarWidth;

                    if ((UINT16)uiDstH <= usDstHeight) //要为上、下边缘同时填充，至少要>=2行
                    {
                        usDeltaDstHHalf = (usDstHeight - (UINT16)uiDstH) >> 1;
                        usDeltaDstHHalf = (usDeltaDstHHalf >> 1) << 1;

                        usDstHeight -= usDeltaDstHHalf << 1;
                    }
                }
                else
                {
                    if ((UINT16)uiDstW <= usDstWidth) //要为左、右边缘同时填充，每边至少要>=16列
                    {
                        usDeltaDstWHalf = (usDstWidth - (UINT16)uiDstW) >> 1;
                        usDeltaDstWHalf = (usDeltaDstWHalf >> 4) << 4;
                        usDstWidth -= usDeltaDstWHalf << 1;
                    }
                }

                apucOutYUVBuf[0] += usDeltaDstWHalf + usDeltaDstHHalf * pstOutArgs->stTarget.usWidthPitch;
                apucOutYUVBuf[1] += (usDeltaDstWHalf >> 1) + ((usDeltaDstHHalf * pstOutArgs->stTarget.usWidthPitch) >> 2);
                apucOutYUVBuf[2] += (usDeltaDstWHalf >> 1) + ((usDeltaDstHHalf * pstOutArgs->stTarget.usWidthPitch) >> 2);
            }
            else
            {
                // 经过resize后的sar为1:1
                pstOutArgs->stTarget.stAspectRatio.usSarWidth = 1;
                pstOutArgs->stTarget.stAspectRatio.usSarHeight = 1;
            }

            /*根据实际输入图像和目标图像的尺寸创建resize通道handler, 节省IMedia_Image_Create的调用次数*/
            /* 2010/03/05 15:25:00 guoshan+00101841*/
            /* RESIZE_Zoom()函数对图像高度有2的整数倍要求，分两场调用时，需要图像高度是4的整数倍 */
            if ((pstInArgs->pstSource->eContentType == IVIDEO_INTERLACED)
                && (pstInArgs->pstSource->usWidthPitch % 16 == 0)
                && (pstOutArgs->stTarget.usWidthPitch % 16 == 0)
                && (pstInArgs->pstSource->usHeight % 4 == 0)
                && (usDstHeight % 4 == 0))  //2010/08/19 guoshan+00101841 增加对目标高度的判断
            {
                UINT8  *apucSrcBottomBuf[3] = {NULL};
                UINT8  *apucDstBottomBuf[3] = {NULL};

                /* 2010/03/05 15:25:00 guoshan+00101841*/
                /* 修改传给resize模块接口的变量，以保证图像usWidth是16的整数倍 */
                /* 2010/03/19 11:15:00 guoshan+00101841*/
                /* 保证与SRS的一致性，取消此修订 */
                //                 iRet = RESIZE_Create(&pstMidProcCtx->pSubCtx,iCpuCap,(RESIZE_QUALITY_LEVEL)pstMidProcCtx->stImageParams.iSubType,
                //                     pstInArgs->pstSource->usWidth, pstInArgs->pstSource->usHeight>>1, pstInArgs->pstSource->usWidthPitch<<1,
                //                     pstOutArgs->stTarget.usWidth, pstOutArgs->stTarget.usHeight>>1, pstOutArgs->stTarget.usWidthPitch<<1,
                //                     pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

                //                 iRet = RESIZE_Create(&pstMidProcCtx->pSubCtx,iCpuCap,(RESIZE_QUALITY_LEVEL)pstMidProcCtx->stImageParams.iSubType,
                //                             usSrcWMaxStride, pstInArgs->pstSource->usHeight>>1, pstInArgs->pstSource->usWidthPitch<<1,
                //                             usDstWMaxStride, pstOutArgs->stTarget.usHeight>>1, pstOutArgs->stTarget.usWidthPitch<<1,
                //                             pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

                /* 2010/08/29 guoshan+00101841*/
                /* 统一用usDstWidth,usDstHeight代替stTarget.usWidth/usHeight进行接口调用 */
                iRet = RESIZE_Create(&pstMidProcCtx->pSubCtx, iCpuCap, (RESIZE_QUALITY_LEVEL)pstMidProcCtx->stImageParams.iSubType,
                                     pstInArgs->pstSource->usWidth, pstInArgs->pstSource->usHeight >> 1, pstInArgs->pstSource->usWidthPitch << 1,
                                     usDstWidth, usDstHeight >> 1, pstOutArgs->stTarget.usWidthPitch << 1,
                                     pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

                if (NULL == pstMidProcCtx->pSubCtx)
                {
                    memset(pstOutArgs, 0, sizeof(STRU_IMAGE_OUT_ARGS));
                    IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] failed to create sub_mid_ctx resize handler,iRet = [%d]\n", pstMidProcCtx, iRet);
                    //    return  IMEDIA_RET_OBJ_CREATE_FAIL;
                    break;
                }

                IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] RESIZE_Create succeed sub_ctx = %p\n", pstMidProcCtx, pstMidProcCtx->pSubCtx);

                // 如果图像是场需要分两次调用
                apucSrcBottomBuf[0] = pstInArgs->pstSource->apucBuf[0] +  pstInArgs->pstSource->usWidthPitch;
                apucSrcBottomBuf[1] = pstInArgs->pstSource->apucBuf[1] +  (pstInArgs->pstSource->usWidthPitch >> 1);
                apucSrcBottomBuf[2] = pstInArgs->pstSource->apucBuf[2] +  (pstInArgs->pstSource->usWidthPitch >> 1);

                //                 apucDstBottomBuf[0] = pstOutArgs->stTarget.apucBuf[0] +  pstOutArgs->stTarget.usWidthPitch;
                //                 apucDstBottomBuf[1] = pstOutArgs->stTarget.apucBuf[1] +  (pstOutArgs->stTarget.usWidthPitch >> 1);
                //                 apucDstBottomBuf[2] = pstOutArgs->stTarget.apucBuf[2] +  (pstOutArgs->stTarget.usWidthPitch >> 1);
                apucDstBottomBuf[0] = apucOutYUVBuf[0] +  pstOutArgs->stTarget.usWidthPitch;
                apucDstBottomBuf[1] = apucOutYUVBuf[1] +  (pstOutArgs->stTarget.usWidthPitch >> 1);
                apucDstBottomBuf[2] = apucOutYUVBuf[2] +  (pstOutArgs->stTarget.usWidthPitch >> 1);

                //                iRet = RESIZE_Zoom(pstMidProcCtx->pSubCtx, pstInArgs->pstSource->apucBuf, pstOutArgs->stTarget.apucBuf); // add return value
                /* 2010/08/29 guoshan+00101841*/
                /* 统一用apucOutYUVBuf代替stTarget.apucBuf进行接口调用 */
                iRet = RESIZE_Zoom(pstMidProcCtx->pSubCtx, pstInArgs->pstSource->apucBuf, apucOutYUVBuf);

                if (iRet)
                {
                    IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] failed to resize,iRet = [%d]\n", pstMidProcCtx, iRet);
                    pstMidProcCtx->pSubCtx = NULL;
                    break;
                }

                iRet = RESIZE_Zoom(pstMidProcCtx->pSubCtx, apucSrcBottomBuf, apucDstBottomBuf);

            }
            else
            {
                /* 2010/03/05 15:25:00 guoshan+00101841*/
                /* 修改传给resize模块接口的变量，以保证图像usWidth是16的整数倍 */
                //                 iRet = RESIZE_Create(&pstMidProcCtx->pSubCtx,iCpuCap,(RESIZE_QUALITY_LEVEL)pstMidProcCtx->stImageParams.iSubType,
                //                     pstInArgs->pstSource->usWidth, pstInArgs->pstSource->usHeight, pstInArgs->pstSource->usWidthPitch,
                //                     pstOutArgs->stTarget.usWidth, pstOutArgs->stTarget.usHeight, pstOutArgs->stTarget.usWidthPitch,
                //                     pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

                /* 2010/03/19 11:15:00 guoshan+00101841*/
                /* 保证与SRS的一致性，取消此修订 */
                //                 iRet = RESIZE_Create(&pstMidProcCtx->pSubCtx,iCpuCap,(RESIZE_QUALITY_LEVEL)pstMidProcCtx->stImageParams.iSubType,
                //                                       usSrcWMaxStride, pstInArgs->pstSource->usHeight, pstInArgs->pstSource->usWidthPitch,
                //                                       usDstWMaxStride, pstOutArgs->stTarget.usHeight, pstOutArgs->stTarget.usWidthPitch,
                //                                       pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

                /* 2010/08/29 guoshan+00101841*/
                /* 统一用usDstWidth,usDstHeight代替stTarget.usWidth/usHeight进行接口调用 */
                iRet = RESIZE_Create(&pstMidProcCtx->pSubCtx, iCpuCap, (RESIZE_QUALITY_LEVEL)pstMidProcCtx->stImageParams.iSubType,
                                     pstInArgs->pstSource->usWidth, pstInArgs->pstSource->usHeight, pstInArgs->pstSource->usWidthPitch,
                                     usDstWidth, usDstHeight, pstOutArgs->stTarget.usWidthPitch,
                                     pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);

                if (NULL == pstMidProcCtx->pSubCtx)
                {
                    memset(pstOutArgs, 0, sizeof(STRU_IMAGE_OUT_ARGS));
                    IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] failed to create sub_mid_ctx resize handler,iRet = [%d]\n", pstMidProcCtx, iRet);
                    //return  IMEDIA_RET_OBJ_CREATE_FAIL;
                    break;
                }

                IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] RESIZE_Create succeed sub_ctx = %p\n", pstMidProcCtx, pstMidProcCtx->pSubCtx);

                // 如果图像是帧仅需要调用1次
                //                iRet = RESIZE_Zoom(pstMidProcCtx->pSubCtx, pstInArgs->pstSource->apucBuf, pstOutArgs->stTarget.apucBuf); // add return value
                /* 2010/08/29 guoshan+00101841*/
                /* 统一用apucOutYUVBuf代替stTarget.apucBuf进行接口调用 */
                iRet = RESIZE_Zoom(pstMidProcCtx->pSubCtx, pstInArgs->pstSource->apucBuf, apucOutYUVBuf);
            }

            pstMidProcCtx->pSubCtx = NULL;

            if (iRet)
            {
                /* 2010/03/16 12:00:00 guoshan+00101841*/
                /* 处理失败时打印返回码信息 */
                IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] failed to resize, iRet = [%d]\n", pstMidProcCtx, iRet);
                //    return iRet;
            }

        }
        break;

        default:
        {
            iRet = IMEDIA_RET_OBJ_INVALID;
        }
        break;
    }

    if (IMEDIA_RET_SUCCESS != iRet)
    {
        memset(pstOutArgs, 0, sizeof(STRU_IMAGE_OUT_ARGS));
    }

    return iRet;
}


/*==================================================================================================================
* 函数说明：
    删除算法通道。
* 输入参数：
    ctx    算法通道句柄
* 输出参数：
    无
* 返 回 值：
    0-成功，其他值见返回值定义。
====================================================================================================================*/
EXPORT_API INT32 IMedia_Image_Delete(IMEDIA_IMAGE_CTX ctx)
{
    STRU_MID_PROCESSOR_CTX *pstMidProcCtx = (STRU_MID_PROCESSOR_CTX *)ctx;

    /*check pMidProcCtx validity*/
    if (NULL == pstMidProcCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "pMidProcCtx NULL\n");
        return  IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstMidProcCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "mid_ctx[%p] iCrcMask[0x%x] wrong\n", pstMidProcCtx, pstMidProcCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    /*free algorithm buffer*/
    if (NULL != pstMidProcCtx->stMemBuf.pucbase)
    {
        IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] free subMemBuf(%p) size(%u)\n", pstMidProcCtx, pstMidProcCtx->stMemBuf.pucbase, pstMidProcCtx->stMemBuf.uiSize);
        IMedia_AlignFree(pstMidProcCtx->stMemBuf.pucbase);
        pstMidProcCtx->stMemBuf.pucbase = NULL;
        pstMidProcCtx->stMemBuf.uiSize = 0;
        pstMidProcCtx->pSubCtx = NULL;
    }

    /*free pMidProcCtx structure buffer*/
    IMEDIA_LOG(IMEDIA_INFO, "mid_ctx[%p] free ctx\n", pstMidProcCtx);
    pstMidProcCtx->iCrcMask = 0;
    IMedia_Free(pstMidProcCtx);
    pstMidProcCtx = NULL;

    return IMEDIA_RET_SUCCESS;
}

/*guoshan+00101841 20100829*/
/*在KeepAspectRatio模式下，提前检测resize模块的参数是否合法*/
static INT32 Resize_CheckParam( UINT16 usSrcW, UINT16 usSrcH, UINT16 usSrcPitch, UINT16 usDstW, UINT16 usDstH, UINT16 usDstPitch)
{
    // 检查输入宽度
    if (usSrcW < IVIDEO_MIN_WIDTH || usSrcW > IVIDEO_MAX_WIDTH || usSrcW % WIDTH_ALIGN != 0)
    {
        IMEDIA_LOG( IMEDIA_ERROR, "usSrcW = %d is wrong!\n", usSrcW);
        return IMEDIA_RET_PARAM_INVALID;
    }

    // 检查输入跨度
    if (usSrcPitch < usSrcW || usSrcPitch % WIDTH_ALIGN != 0)
    {
        IMEDIA_LOG( IMEDIA_ERROR, " usSrcPitch = %d!\n", usSrcPitch);
        return IMEDIA_RET_PARAM_INVALID;
    }

    // 检查输入高度
    if (usSrcH < IVIDEO_MIN_HEIGHT || usSrcH > IVIDEO_MAX_HEIGHT || usSrcH % HEIGHT_ALIGN != 0)
    {
        IMEDIA_LOG( IMEDIA_ERROR, "usSrcH = %d is wrong!\n", usSrcH);
        return IMEDIA_RET_PARAM_INVALID;
    }

    // 检查输出宽度
    if (usDstW < IVIDEO_MIN_WIDTH || usDstW > IVIDEO_MAX_WIDTH || usDstW % WIDTH_ALIGN != 0)
    {
        IMEDIA_LOG( IMEDIA_ERROR, "usDstW = %d is wrong!\n", usDstW);
        return IMEDIA_RET_PARAM_INVALID;
    }

    // 检查输出跨度
    if (usDstPitch < usDstW || usDstPitch % WIDTH_ALIGN != 0)
    {
        IMEDIA_LOG( IMEDIA_ERROR, " usDstPitch = %d is wrong!\n", usDstPitch);
        return IMEDIA_RET_PARAM_INVALID;
    }

    // 检查输出高度
    if (usDstH < IVIDEO_MIN_HEIGHT || usDstH > IVIDEO_MAX_HEIGHT || usDstH % HEIGHT_ALIGN != 0)
    {
        IMEDIA_LOG( IMEDIA_ERROR, "usDstH = %d is wrong!\n", usDstH);
        return IMEDIA_RET_PARAM_INVALID;
    }

    // 检查是否是下采样
    if (usSrcW < usDstW || usSrcH < usDstH)
    {
        IMEDIA_LOG( IMEDIA_ERROR, "usSrcW[%d] < usDstW[%d] || usSrcH[%d] < usDstH[%d]!\n", usSrcW, usDstW, usSrcH, usDstH);
        return IMEDIA_RET_PARAM_INVALID;
    }

    return IMEDIA_RET_SUCCESS;
}

/************************************************************************************************************
* $Log$
************************************************************************************************************/
