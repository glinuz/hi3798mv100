/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_codec.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/08/15
  Description   : Functions implement of HI_CODEC.
  History       :
  1.Date        : 2012/08/15
    Author      : l00185424
    Modification: Created file

 *******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <pthread.h>
#include <dlfcn.h>
#include <string.h>

/* Unf headers */
#include "hi_video_codec.h"

/* Common headers */
#include "hi_mpi_mem.h"
#include "hi_module.h"
#include "hi_debug.h"

/* Mpi headers */
#include "hi_codec.h"

#include "list.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

/****************************** Macro Definition *****************************/

#if (HI_VDEC_REG_CODEC_SUPPORT == 1)
#define MAX_CODEC_LIB_LEN (256)
#endif

#define CODEC_MAKE_HANDLE(hCodec, hOriInst) ((hCodec<<8) | hOriInst)

#define CODEC_LOCK(Mutex) pthread_mutex_lock(&Mutex)
#define CODEC_UNLOCK(Mutex) pthread_mutex_unlock(&Mutex)

#define HI_MALLOC_CODEC(size) HI_MALLOC(HI_ID_VDEC, size)
#define HI_FREE_CODEC(addr) HI_FREE(HI_ID_VDEC, addr)

#define HI_ERR_CODEC(fmt...) \
    HI_ERR_PRINT(HI_ID_VDEC, fmt)
#define HI_WARN_CODEC(fmt...) \
    HI_WARN_PRINT(HI_ID_VDEC, fmt)
#define HI_INFO_CODEC(fmt...) \
    HI_INFO_PRINT(HI_ID_VDEC, fmt)

#define CODEC_FIND_CODEC_BY_STRUCT(pCodec, pCodecParam) \
    { \
        HI_U32 u32Ret; \
        struct list_head* pos; \
        struct list_head* n; \
        CODEC_PARAM_S* pstTmp; \
        u32Ret = CODEC_LOCK(s_stParam.stMutex); \
        if (u32Ret != 0) \
        { \
             HI_ERR_CODEC("CODEC_LOCK err:%d\n", u32Ret); \
        } \
        if (!list_empty(&s_stParam.stCodecHead)) \
        { \
            list_for_each_safe(pos, n, &s_stParam.stCodecHead) \
            { \
                pstTmp = list_entry(pos, CODEC_PARAM_S, stCodecNode); \
                if (pCodec == pstTmp->pstCodec) \
                { \
                    pCodecParam = pstTmp; \
                    break; \
                } \
            } \
        } \
        u32Ret = CODEC_UNLOCK(s_stParam.stMutex); \
        if (0!=u32Ret)\
        { \
            HI_ERR_CODEC("CODEC_UNLOCK err:%d\n",u32Ret); \
        } \
    }


#define CODEC_FIND_CODEC_BY_LIBNAME(pszLib, pCodecParam) \
    { \
        HI_U32 u32Ret; \
        struct list_head* pos; \
        struct list_head* n; \
        CODEC_PARAM_S* pstTmp; \
        u32Ret = CODEC_LOCK(s_stParam.stMutex); \
        if (u32Ret != 0) \
        { \
             HI_ERR_CODEC("CODEC_LOCK err:%d\n", u32Ret); \
        } \
        if (!list_empty(&s_stParam.stCodecHead)) \
        { \
            list_for_each_safe(pos, n, &s_stParam.stCodecHead) \
            { \
                pstTmp = list_entry(pos, CODEC_PARAM_S, stCodecNode); \
                if ((HI_NULL != pstTmp->pszLibName) && (0 == strncmp(pszLib, pstTmp->pszLibName, strlen(pszLib)))) \
                { \
                    pCodecParam = pstTmp; \
                    break; \
                } \
            } \
        } \
        u32Ret = CODEC_UNLOCK(s_stParam.stMutex); \
        if (0!=u32Ret)\
        { \
            HI_ERR_CODEC("CODEC_UNLOCK err:%d\n",u32Ret); \
        } \
    }

#define CODEC_FIND_CODEC_BY_INST(hInst, pCodecParam) \
    { \
        HI_U32 u32Ret; \
        struct list_head* pos; \
        struct list_head* n; \
        HI_HANDLE hCodec = HI_CODEC_LIB_HANDLE(hInst); \
        CODEC_PARAM_S* pstTmp; \
        u32Ret = CODEC_LOCK(s_stParam.stMutex); \
        if (u32Ret != 0) \
        { \
             HI_ERR_CODEC("CODEC_LOCK err:%d\n", u32Ret); \
        } \
        list_for_each_safe(pos, n, &s_stParam.stCodecHead) \
        { \
            pstTmp = list_entry(pos, CODEC_PARAM_S, stCodecNode); \
            if (hCodec == pstTmp->hCodec) \
            { \
                pCodecParam = pstTmp; \
                break; \
            } \
        } \
        u32Ret = CODEC_UNLOCK(s_stParam.stMutex); \
        if (0!=u32Ret) \
        { \
            HI_ERR_CODEC("CODEC_UNLOCK err:%d\n",u32Ret); \
        } \
    }

#define CODEC_FIND_INST(hInst, pCodecParam, pInst) \
    { \
        struct list_head* pos; \
        struct list_head* n; \
        CODEC_INST_S* pstTmp; \
        list_for_each_safe(pos, n, &pCodecParam->stInstHead) \
        { \
            pstTmp = list_entry(pos, CODEC_INST_S, stInstNode); \
            if (hInst == pstTmp->hInst) \
            { \
                pInst = pstTmp; \
                break; \
            } \
        } \
    }

#define CODEC_FREE_INST_LIST(pCodecParam) \
    {  \
        struct list_head* pos; \
        struct list_head* n; \
        CODEC_INST_S* pstInst; \
        if (!list_empty(&pCodecParam->stInstHead)) \
        { \
            list_for_each_safe(pos, n, &pCodecParam->stInstHead) \
            { \
                pstInst = list_entry(pos, CODEC_INST_S, stInstNode); \
                (HI_VOID)pCodecParam->pstCodec->Destroy(HI_CODEC_INST_HANDLE(pstInst->hInst)); \
                list_del(pos); \
                HI_FREE_CODEC(pstInst); \
            } \
        } \
    }

#define CODEC_CHECK_INIT \
    if (!s_stParam.bInited) \
    { \
        HI_CODEC_Init(); \
    }

/************************ Static Structure Definition ************************/

/* Describe a codec instance */
typedef struct tagCODEC_INST_S
{
    HI_HANDLE          hInst;          /* Codec instance handle, its value is ((hCodec<<16) | hOriInst) */
    struct list_head   stInstNode;     /* Codec instance node */
} CODEC_INST_S;

/* Describe a codec */
typedef struct tagCODEC_S
{
#if (HI_VDEC_REG_CODEC_SUPPORT == 1)
    HI_VOID*            pDllModule;     /* Handle of dlopen() */
    HI_CHAR*            pszLibName;     /* Dll lib name(include path) */
#endif
    HI_CODEC_S*         pstCodec;       /* Code structure(HI_CODEC_S) */
    HI_HANDLE           hCodec;         /* Handle */
    pthread_mutex_t     stMutex;        /* Codec mutex */
    HI_BOOL             bRegByLib;      /* Registered by lib */
    struct list_head    stInstHead;     /* Codec instance head */
    struct list_head    stCodecNode;    /* Codec list node */
} CODEC_PARAM_S;

/* Global parameter of this layer */
typedef struct tagCODEC_GLOBAL_S
{
    HI_BOOL             bCodecAlloc[HI_CODEC_MAX_NUMBER];
    HI_BOOL             bInited;            /* Had been inited or not */
    pthread_mutex_t     stMutex;            /* Global mutex */
    struct list_head    stCodecHead;        /* Codec list head */
    HI_U16              u16CodecNum;
} CODEC_GLOBAL_S;

/***************************** Global Definition *****************************/


/***************************** Static Definition *****************************/

static CODEC_GLOBAL_S s_stParam;

/*********************************** Code ************************************/

/* Check parameter */
static HI_BOOL CODEC_CheckMember(const HI_CODEC_S* pstCodec)
{
    if (HI_NULL == pstCodec)
    {
        return HI_FALSE;
    }

    if ((HI_NULL == pstCodec->GetCap)
       || (HI_NULL == pstCodec->Create)
       || (HI_NULL == pstCodec->Destroy))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

/* Get first usable codec */
static CODEC_PARAM_S* CODEC_FindUsableCodec(HI_CODEC_TYPE_E enType, HI_CODEC_ID_E enID)
{
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    HI_CODEC_CAP_S stCap;
    HI_CODEC_SUPPORT_S* pstSupport;
    struct list_head* pstPos = HI_NULL;
    struct list_head* pstN = HI_NULL;

    (HI_VOID)CODEC_LOCK(s_stParam.stMutex);
    if (!list_empty(&s_stParam.stCodecHead))
    {
        /* Check every registered codec */
        list_for_each_safe(pstPos, pstN, &s_stParam.stCodecHead)
        {
            pstCodecParam = list_entry(pstPos, CODEC_PARAM_S, stCodecNode);

            /* Get capability, if fail, pass this codec */
            if (HI_SUCCESS != pstCodecParam->pstCodec->GetCap(&stCap))
            {
                continue;
            }

            pstSupport = stCap.pstSupport;
            while (HI_NULL != pstSupport)
            {
                /* Yes, I get it! */
                if ((enID == pstSupport->enID) && (0 != (enType & pstSupport->u32Type)))
                {
                    (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
                    return pstCodecParam;
                }

                pstSupport = pstSupport->pstNext;
            }
        }
    }

    (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);

    /* Fail */
    return HI_NULL;
}

static HI_VOID CODEC_UnRegister(CODEC_PARAM_S* pstCodecParam)
{
    /* Destroy all instance of this codec */
    (HI_VOID)CODEC_LOCK(pstCodecParam->stMutex);
    CODEC_FREE_INST_LIST(pstCodecParam);
    (HI_VOID)CODEC_UNLOCK(pstCodecParam->stMutex);

    /* Delete this codec from list */
    (HI_VOID)CODEC_LOCK(s_stParam.stMutex);
    list_del(&pstCodecParam->stCodecNode);
    s_stParam.bCodecAlloc[pstCodecParam->hCodec] = HI_FALSE;
    s_stParam.u16CodecNum--;
    (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);

#if (HI_VDEC_REG_CODEC_SUPPORT == 1)
    /* Free resource */
    if (pstCodecParam->pDllModule)
    {
        dlclose(pstCodecParam->pDllModule);
    }

    if (pstCodecParam->pszLibName)
    {
        HI_FREE_CODEC(pstCodecParam->pszLibName);
    }
#endif

    (HI_VOID)pthread_mutex_destroy(&pstCodecParam->stMutex);

    /* Free parameter */
    HI_FREE_CODEC(pstCodecParam);
}

/* Init */
HI_S32 HI_CODEC_Init(HI_VOID)
{
    HI_S32 s32Ret;
    HI_S32 i;

    if (!s_stParam.bInited)
    {
        /* Init parameter */

        for (i=0; i<HI_CODEC_MAX_NUMBER; i++)
        {
            s_stParam.bCodecAlloc[i] = HI_FALSE;
        }

        /* Init list */
        INIT_LIST_HEAD(&s_stParam.stCodecHead);

        /* Init mutex */
        s32Ret = pthread_mutex_init(&s_stParam.stMutex, HI_NULL);
        if (0 != s32Ret)
        {
            HI_WARN_CODEC("CODEC mutex init err.\n");
            return HI_FAILURE;
        }


        (HI_VOID)CODEC_LOCK(s_stParam.stMutex);
        s_stParam.u16CodecNum = 0;
        (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);

        s_stParam.bInited = HI_TRUE;
    }

    HI_INFO_CODEC("HI_CODEC_Init OK\n");
    return HI_SUCCESS;
}

HI_S32 HI_CODEC_DeInit(HI_VOID)
{
#if 0
    HI_S32 s32Ret;
    struct list_head* pos;
    struct list_head* n;
    CODEC_PARAM_S* pstCodecParam;

    if (s_stParam.bInited)
    {
        CODEC_LOCK(s_stParam.stMutex);
        if (!list_empty(&s_stParam.stCodecHead))
        {
            list_for_each_safe(pos, n, &s_stParam.stCodecHead)
            {
                pstCodecParam = list_entry(pos, CODEC_PARAM_S, stCodecNode);
                CODEC_UNLOCK(s_stParam.stMutex);
                CODEC_UnRegister(pstCodecParam);
                CODEC_LOCK(s_stParam.stMutex);
            }
        }

        CODEC_UNLOCK(s_stParam.stMutex);

        /* DeInit mutex */
        s32Ret = pthread_mutex_destroy(&s_stParam.stMutex);
        if (0 != s32Ret)
        {
            HI_WARN_CODEC("CODEC mutex destroy err.\n");
            return HI_FAILURE;
        }

        /* DeInit parameter */
        s_stParam.u16CodecNum = 0;
        s_stParam.u16CodecHandle = 0;
    }

    s_stParam.bInited = HI_FALSE;
#endif
    HI_INFO_CODEC("HI_CODEC_DeInit OK\n");

    return HI_SUCCESS;
}

#if (HI_VDEC_REG_CODEC_SUPPORT == 1)
HI_S32 HI_CODEC_RegisterLib(const HI_CHAR *pszCodecDllName)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_VOID* pDllModule = HI_NULL;
    HI_CODEC_S* pstCodec = HI_NULL;
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    HI_HANDLE hCodecHandle = HI_INVALID_HANDLE;
    HI_U32 i;

    CODEC_CHECK_INIT;

    /* Check parameter */
    if (HI_NULL == pszCodecDllName)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    HI_INFO_CODEC("HI_CODEC_RegisterLib(%s) begin...\n", pszCodecDllName);

    /* Check registered */
    CODEC_FIND_CODEC_BY_LIBNAME(pszCodecDllName, pstCodecParam);
    if (HI_NULL != pstCodecParam)
    {
        HI_ERR_CODEC("Register %s fail:Had been registered.\n", pszCodecDllName);
        return HI_SUCCESS;
    }

    HI_INFO_CODEC("HI_CODEC_RegisterLib(%s) first time, go...\n", pszCodecDllName);

    /* 1. dlopen() lib; */
    // TODO: Test other open type
    pDllModule = dlopen(pszCodecDllName, RTLD_LAZY | RTLD_GLOBAL);
    if (HI_NULL == pDllModule)
    {
        HI_ERR_CODEC("Register %s fail:%s.\n", pszCodecDllName, dlerror());
        s32Ret = HI_ERR_CODEC_INVALIDPARAM;
        goto err0;
    }

    HI_INFO_CODEC("HI_CODEC_RegisterLib(%s) dlopen success, go...\n", pszCodecDllName);

    /* 2. Get entry symbol; */
    pstCodec = (HI_CODEC_S*)dlsym(pDllModule, "hi_codec_entry");
    if (HI_NULL == pstCodec)
    {
        HI_ERR_CODEC("Register %s fail:%s.\n", pszCodecDllName, dlerror());
        s32Ret = HI_ERR_CODEC_INVALIDPARAM;
        goto err1;
    }

    HI_INFO_CODEC("HI_CODEC_RegisterLib(%s) get hi_codec_entry success, go...\n", pszCodecDllName);

    /* 3. Check member methods */
    if (!CODEC_CheckMember(pstCodec))
    {
        HI_ERR_CODEC("Register %s fail: invalid method.\n", pszCodecDllName);
        s32Ret = HI_ERR_CODEC_INVALIDPARAM;
        goto err1;
    }

    HI_INFO_CODEC("HI_CODEC_RegisterLib(%s) check methods success, go...\n", pszCodecDllName);

    /* 4. Allocate resource, create CODEC_PARAM_S */
    pstCodecParam = (CODEC_PARAM_S*)HI_MALLOC_CODEC(sizeof(CODEC_PARAM_S));
    if (HI_NULL == pstCodecParam)
    {
        HI_ERR_CODEC("No memory.\n");
        s32Ret = HI_ERR_CODEC_NOENOUGHRES;
        goto err1;
    }

    /* 5. Add CODEC_PARAM_S to codec list */
    (HI_VOID)CODEC_LOCK(s_stParam.stMutex);

    /* Alloc codec handle */
    for (i=0; i<HI_CODEC_MAX_NUMBER; i++)
    {
        if (!(s_stParam.bCodecAlloc[i]))
        {
            hCodecHandle = i;
            break;
        }
    }

    if ((HI_INVALID_HANDLE == hCodecHandle) || (HI_CODEC_MAX_NUMBER == hCodecHandle))
    {
        (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
        HI_FREE_CODEC(pstCodecParam);
        HI_ERR_CODEC("Too many codecs registered.\n");
        dlclose(pDllModule);

        return HI_ERR_CODEC_NOENOUGHRES;
    }

    /* Init parameter of codec */
    pstCodecParam->pDllModule = pDllModule;
    pstCodecParam->pszLibName = (HI_CHAR*)HI_MALLOC_CODEC(strlen(pszCodecDllName)+1);
    if (HI_NULL == pstCodecParam->pszLibName)
    {
        HI_ERR_CODEC("No memory.\n");
        s32Ret = HI_ERR_CODEC_NOENOUGHRES;
        goto err2;
    }
    strncpy(pstCodecParam->pszLibName, pszCodecDllName,strlen(pszCodecDllName)+1);
    pstCodecParam->pstCodec = pstCodec;
    pstCodecParam->hCodec = hCodecHandle;
    pstCodecParam->bRegByLib = HI_TRUE;
    if (0 != pthread_mutex_init(&pstCodecParam->stMutex, HI_NULL))
    {
        (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
        s32Ret = HI_ERR_CODEC_NOENOUGHRES;
        goto err3;
    }

    INIT_LIST_HEAD(&pstCodecParam->stInstHead);

    /* Add this codec to list TAIL.(USE list_add_tail) */
    list_add_tail(&pstCodecParam->stCodecNode, &s_stParam.stCodecHead);
    s_stParam.u16CodecNum++;

    /* Set handle flag */
    s_stParam.bCodecAlloc[hCodecHandle] = HI_TRUE;

    (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);

    HI_INFO_CODEC("HI_CODEC_RegisterLib %s OK\n", pszCodecDllName);
    return HI_SUCCESS;

err3:
    HI_FREE_CODEC(pstCodecParam->pszLibName);
err2:
    HI_FREE_CODEC(pstCodecParam);
    (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
err1:
    dlclose(pDllModule);
err0:
    return s32Ret;
}

HI_S32 HI_CODEC_RegisterFunc(const HI_CODEC_S *pszCodec)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CODEC_S* pstCodec = HI_NULL;
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    HI_HANDLE hCodecHandle = HI_INVALID_HANDLE;
    HI_U32 i;

    CODEC_CHECK_INIT;

    /* Check parameter */
    if (HI_NULL == pszCodec)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    HI_INFO_CODEC("HI_CODEC_RegisterStruct(%s) begin...\n", pszCodec->pszName);

    /* Check registered */
    CODEC_FIND_CODEC_BY_LIBNAME(pszCodec->pszName, pstCodecParam);
    if (HI_NULL != pstCodecParam)
    {
        HI_ERR_CODEC("Register %s fail:Had been registered.\n", pszCodec->pszName);
        return HI_SUCCESS;
    }
    pstCodec = (HI_CODEC_S*)HI_MALLOC_CODEC(sizeof(HI_CODEC_S));
	if (HI_NULL == pstCodec)
    {
        HI_ERR_CODEC("No memory.\n");
        s32Ret = HI_ERR_CODEC_INVALIDPARAM;
        goto err0;
    }
    memcpy(pstCodec,pszCodec,sizeof(HI_CODEC_S));

    /* 3. Check member methods */
    if (!CODEC_CheckMember(pstCodec))
    {
        HI_ERR_CODEC("Register %s fail: invalid method.\n", pszCodec->pszName);
        s32Ret = HI_ERR_CODEC_INVALIDPARAM;
        goto err1;
    }

    HI_INFO_CODEC("HI_CODEC_RegisterStruct(%s) check methods success, go...\n", pszCodec->pszName);

    /* 4. Allocate resource, create CODEC_PARAM_S */
    pstCodecParam = (CODEC_PARAM_S*)HI_MALLOC_CODEC(sizeof(CODEC_PARAM_S));
    if (HI_NULL == pstCodecParam)
    {
        HI_ERR_CODEC("No memory.\n");
        s32Ret = HI_ERR_CODEC_NOENOUGHRES;
        goto err1;
    }

    /* 5. Add CODEC_PARAM_S to codec list */
    (HI_VOID)CODEC_LOCK(s_stParam.stMutex);

    /* Alloc codec handle */
    for (i=0; i<HI_CODEC_MAX_NUMBER; i++)
    {
        if (!(s_stParam.bCodecAlloc[i]))
        {
            hCodecHandle = i;
            break;
        }
    }

    if ((HI_INVALID_HANDLE == hCodecHandle) || (HI_CODEC_MAX_NUMBER == hCodecHandle))
    {
        (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
        HI_FREE_CODEC(pstCodecParam);
		HI_FREE_CODEC(pstCodec);
        HI_ERR_CODEC("Too many codecs registered.\n");
        return HI_ERR_CODEC_NOENOUGHRES;
    }

    /* Init parameter of codec */
    pstCodecParam->pszLibName = (HI_CHAR*)HI_MALLOC_CODEC(strlen(pszCodec->pszName)+1);
    if (HI_NULL == pstCodecParam->pszLibName)
    {
        HI_ERR_CODEC("No memory.\n");
	    (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
        s32Ret = HI_ERR_CODEC_NOENOUGHRES;
        goto err2;
    }
    strncpy(pstCodecParam->pszLibName, pszCodec->pszName,strlen(pszCodec->pszName)+1);
    pstCodecParam->pstCodec = pstCodec;
    pstCodecParam->hCodec = hCodecHandle;
    pstCodecParam->bRegByLib = HI_FALSE;
    pstCodecParam->pDllModule = HI_NULL;

    if (0 != pthread_mutex_init(&pstCodecParam->stMutex, HI_NULL))
    {
        (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
        s32Ret = HI_ERR_CODEC_NOENOUGHRES;
        goto err3;
    }

    INIT_LIST_HEAD(&pstCodecParam->stInstHead);

    /* Add this codec to list TAIL.(USE list_add_tail) */
    list_add_tail(&pstCodecParam->stCodecNode, &s_stParam.stCodecHead);
    s_stParam.u16CodecNum++;

    /* Set handle flag */
    s_stParam.bCodecAlloc[hCodecHandle] = HI_TRUE;

    (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);

    return HI_SUCCESS;

err3:
    HI_FREE_CODEC(pstCodecParam->pszLibName);
err2:
    HI_FREE_CODEC(pstCodecParam);
err1:
    HI_FREE_CODEC(pstCodec);
err0:
    return s32Ret;
}
HI_S32 HI_CODEC_UnRegisterLib(const HI_CHAR *pszCodecDllName)
{
    CODEC_PARAM_S* pstCodecParam = HI_NULL;

    CODEC_CHECK_INIT;

    /* Check parameter */
    if (HI_NULL == pszCodecDllName)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Find CODEC_PARAM_S* by pszCodecDllName */
    CODEC_FIND_CODEC_BY_LIBNAME(pszCodecDllName, pstCodecParam);
    if (HI_NULL == pstCodecParam)
    {
        HI_ERR_CODEC("UnRegister %s fail: can't find.\n", pszCodecDllName);
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* UnRegister */
    CODEC_UnRegister(pstCodecParam);

    return HI_SUCCESS;
}
#endif

/* Register codec using structure(HI_CODEC_S*) */
HI_S32 HI_CODEC_Register(HI_CODEC_S* pstCodec)
{
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    HI_HANDLE hCodecHandle = HI_INVALID_HANDLE;
    HI_U32 i;

    CODEC_CHECK_INIT;

    /* Check member property and method */
    if (!CODEC_CheckMember(pstCodec))
    {
        HI_ERR_CODEC("Invalid method.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Find codec */
    CODEC_FIND_CODEC_BY_STRUCT(pstCodec, pstCodecParam);
    if (HI_NULL != pstCodecParam)
    {
        HI_ERR_CODEC("Had been registered.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Allocate resource */
    pstCodecParam = (CODEC_PARAM_S*)HI_MALLOC_CODEC(sizeof(CODEC_PARAM_S));
    if (HI_NULL == pstCodecParam)
    {
        HI_ERR_CODEC("No memory.\n");
        return HI_ERR_CODEC_NOENOUGHRES;
    }

    (HI_VOID)CODEC_LOCK(s_stParam.stMutex);

    /* Alloc codec handle */
    for (i=0; i<HI_CODEC_MAX_NUMBER; i++)
    {
        if (!(s_stParam.bCodecAlloc[i]))
        {
            hCodecHandle = i;
            break;
        }
    }

    if ((HI_INVALID_HANDLE == hCodecHandle) || (HI_CODEC_MAX_NUMBER == hCodecHandle))
    {
        (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
        HI_FREE_CODEC(pstCodecParam);
        HI_ERR_CODEC("Too many codecs registered.\n");
        return HI_ERR_CODEC_NOENOUGHRES;
    }

    /* Init parameter of codec */
#if (HI_VDEC_REG_CODEC_SUPPORT == 1)
    pstCodecParam->pDllModule = HI_NULL;
    pstCodecParam->pszLibName = HI_NULL;
#endif
    pstCodecParam->pstCodec = pstCodec;
    pstCodecParam->hCodec = hCodecHandle;
    pstCodecParam->bRegByLib = HI_FALSE;
    if (0 != pthread_mutex_init(&pstCodecParam->stMutex, HI_NULL))
    {
        (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
        HI_FREE_CODEC(pstCodecParam);
        return HI_FAILURE;
    }

    INIT_LIST_HEAD(&pstCodecParam->stInstHead);

    /*
     * Add this codec to list HEAD.(USE list_add)
     * NOTE:
     *   CODEC REGISTERED BY THIS INTERFACE WILL HAVE THE HIGHEST PRIORITY.
     *   REGISTER VFMW WILL CALL THIS INTERFACE.
     */
    list_add(&pstCodecParam->stCodecNode, &s_stParam.stCodecHead);
    s_stParam.u16CodecNum++;

    /* Set handle flag */
    s_stParam.bCodecAlloc[hCodecHandle] = HI_TRUE;

    (HI_VOID)CODEC_UNLOCK(s_stParam.stMutex);
    HI_INFO_CODEC("HI_CODEC_Register OK\n");

    return HI_SUCCESS;
}

/* Unregister codec using structure(HI_CODEC_S*) */
HI_S32 HI_CODEC_UnRegister(const HI_CODEC_S* pstCodec)
{
    CODEC_PARAM_S* pstCodecParam = HI_NULL;

    CODEC_CHECK_INIT;

    if (HI_NULL == pstCodec)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Find codec */
    CODEC_FIND_CODEC_BY_STRUCT(pstCodec, pstCodecParam);
    if (HI_NULL == pstCodecParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    CODEC_UnRegister(pstCodecParam);

    HI_INFO_CODEC("HI_CODEC_UnRegister OK\n");
    return HI_SUCCESS;
}

/* Create instance */
HI_CODEC_S* HI_CODEC_Create(HI_HANDLE* phInst, const HI_CODEC_OPENPARAM_S * pstParam)
{
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    CODEC_INST_S* pstInst;
    HI_S32 s32Ret;

    if (!s_stParam.bInited)
    {
        return HI_NULL;
    }

    if ((HI_NULL == phInst) || (HI_NULL == pstParam))
    {
        return HI_NULL;
    }

    if (HI_CODEC_TYPE_BUTT <= pstParam->enType)
    {
        return HI_NULL;
    }

    /* Find usable codec by type and ID */
    pstCodecParam = CODEC_FindUsableCodec(pstParam->enType, pstParam->enID);
    if (HI_NULL == pstCodecParam)
    {
        HI_ERR_CODEC("No usable codec.\n");
        return HI_NULL;
    }

    /* Create codec instance */
    s32Ret = pstCodecParam->pstCodec->Create(phInst, pstParam);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_NULL;
    }

    /* Alloc instance resource */
    pstInst = (CODEC_INST_S*)HI_MALLOC_CODEC(sizeof(CODEC_INST_S));
    if (HI_NULL == pstInst)
    {
        s32Ret = pstCodecParam->pstCodec->Destroy(*phInst);
		if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CODEC("Destroy Codec failed:%d.\n",s32Ret);
        }
        return HI_NULL;
    }

    /* Make instance handle */
    pstInst->hInst = CODEC_MAKE_HANDLE(pstCodecParam->hCodec, *phInst);

    /* Add instance to list */
    (HI_VOID)CODEC_LOCK(pstCodecParam->stMutex);
    list_add_tail(&pstInst->stInstNode, &pstCodecParam->stInstHead);
    (HI_VOID)CODEC_UNLOCK(pstCodecParam->stMutex);

    *phInst = pstInst->hInst;

    HI_INFO_CODEC("HI_CODEC_Create OK\n");
    return pstCodecParam->pstCodec;
}

/* Destroy instance */
HI_S32 HI_CODEC_Destory(HI_HANDLE hInst)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    CODEC_INST_S* pstInst = HI_NULL;

    CODEC_CHECK_INIT;

    /* Find codec */
    CODEC_FIND_CODEC_BY_INST(hInst, pstCodecParam);
    if (HI_NULL == pstCodecParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    (HI_VOID)CODEC_LOCK(pstCodecParam->stMutex);

    /* Find instance */
    CODEC_FIND_INST(hInst, pstCodecParam, pstInst);
    if (HI_NULL == pstInst)
    {
        (HI_VOID)CODEC_UNLOCK(pstCodecParam->stMutex);
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Delete instance from list */
    list_del(&pstInst->stInstNode);
    (HI_VOID)CODEC_UNLOCK(pstCodecParam->stMutex);

    /* Stop and Destroy instance */
    if (pstCodecParam->pstCodec->Stop)
    {
        s32Ret  = pstCodecParam->pstCodec->Stop(HI_CODEC_INST_HANDLE(hInst));
    }
    s32Ret |= pstCodecParam->pstCodec->Destroy(HI_CODEC_INST_HANDLE(hInst));

    /* Free resource */
    HI_FREE_CODEC(pstInst);

    HI_INFO_CODEC("HI_CODEC_Destory OK\n");
    return s32Ret;
}

HI_BOOL HI_CODEC_SupportDecode(const HI_CODEC_S* pstCodec, HI_CODEC_ID_E enID)
{
    HI_CODEC_CAP_S stCap;
    HI_CODEC_SUPPORT_S* pstSupport;

    if (!s_stParam.bInited)
    {
        return HI_FALSE;
    }

    if ((HI_NULL == pstCodec) || (enID >= HI_CODEC_ID_BUTT))
    {
        return HI_FALSE;
    }

    if (HI_SUCCESS != pstCodec->GetCap(&stCap))
    {
        return HI_FALSE;
    }

    pstSupport = stCap.pstSupport;
    while (HI_NULL != pstSupport)
    {
        /* Yes, I get it! */
        if ((enID == pstSupport->enID) && (0 != (HI_CODEC_TYPE_DEC & pstSupport->u32Type)))
        {
            return HI_TRUE;
        }

        pstSupport = pstSupport->pstNext;
    }

    return HI_FALSE;
}

/* Get codec name */
const HI_CHAR* HI_CODEC_GetName(HI_HANDLE hInst)
{
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    CODEC_INST_S* pstInst = HI_NULL;

    if (!s_stParam.bInited)
    {
        return HI_NULL;
    }

    /* Find codec */
    CODEC_FIND_CODEC_BY_INST(hInst, pstCodecParam);
    if (pstCodecParam)
    {
        CODEC_FIND_INST(hInst, pstCodecParam, pstInst);
        if (pstInst)
        {
            HI_INFO_CODEC("HI_CODEC_GetName OK:%s.\n", pstCodecParam->pstCodec->pszName);
            return pstCodecParam->pstCodec->pszName;
        }
    }

    return HI_NULL;
}

/* Get codec version */
HI_CODEC_VERSION_U HI_CODEC_GetVersion(HI_HANDLE hInst)
{
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    CODEC_INST_S* pstInst = HI_NULL;
    HI_CODEC_VERSION_U unInvalidVer = {.stVersion = {0, 0, 0, 0}};

    if (!s_stParam.bInited)
    {
        return unInvalidVer;
    }

    /* Find codec */
    CODEC_FIND_CODEC_BY_INST(hInst, pstCodecParam);
    if (pstCodecParam)
    {
        CODEC_FIND_INST(hInst, pstCodecParam, pstInst);
        if (pstInst)
        {
            HI_INFO_CODEC("HI_CODEC_GetVersion OK:%x.\n", pstCodecParam->pstCodec->unVersion.u32Version);
            return pstCodecParam->pstCodec->unVersion;
        }
    }

    return unInvalidVer;
}

/* Codec has frame buffer? */
HI_BOOL HI_CODEC_NeedFrameBuf(HI_HANDLE hInst)
{
    CODEC_PARAM_S* pstCodecParam = HI_NULL;
    CODEC_INST_S* pstInst = HI_NULL;
    HI_CODEC_CAP_S stCap;

    if (!s_stParam.bInited)
    {
        return HI_FALSE;
    }

    /* Find codec */
    CODEC_FIND_CODEC_BY_INST(hInst, pstCodecParam);
    if (pstCodecParam)
    {
        CODEC_FIND_INST(hInst, pstCodecParam, pstInst);
        if (pstInst)
        {
            if (HI_SUCCESS == pstCodecParam->pstCodec->GetCap(&stCap))
            {
                HI_INFO_CODEC("HI_CODEC_NeedFrameBuf OK.\n");

                /* Don't need frame buffer if HI_CODEC_CAP_DRIVENSELF */
                return (HI_BOOL)(HI_CODEC_CAP_DRIVENSELF != (stCap.u32CapNumber & HI_CODEC_CAP_DRIVENSELF));
            }
        }
    }

    /* Default return false */
    return HI_FALSE;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
