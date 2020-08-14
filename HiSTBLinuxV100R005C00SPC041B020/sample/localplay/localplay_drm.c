/**
 \file localplay_drm.c
 \brief 
 \author HiSilicon Technologies Co., Ltd.
 \version 1.0
 \author z00187490
 \date 2012-11-6
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <hi_drm_client.h>
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define LOGI(fmt, arg...) printf(fmt"\n", ##arg)
#define EXPECT_EQ(exp1, exp2) \
    if (exp1 != exp2) {\
        printf("expect eq (%s) failed at [%s,%d]\n", #exp1 "==" #exp2, __FUNCTION__, __LINE__);\
    }
#define EXPECT_TRUE(exp) \
    if (!exp) {\
        printf("expect true (%s) failed at [%s,%d]\n", #exp, __FUNCTION__, __LINE__);\
    }
#define CHECK(cond, action, msg, ...) \
    if (cond) {\
        printf("Check " #cond " true at [%s,%d], we will:"#action" msg:"#msg"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        action;\
    }

static HI_DRM_CLIENT_PPTR mClient;
static HI_S32 bInitialize = 0;
static const HI_CHAR* s_file = NULL;
static const HI_CHAR* s_mimeType = NULL;
#define dict_num (32)
static struct Dict {
    HI_CHAR *key;
    HI_CHAR *val;
} dict[dict_num];

/**
 * fromat:key1=val1,key2=val2
 * Note: input a writable string
 * @return the number of dict
 */
static HI_S32 parse_json(HI_CHAR* request)
{

    HI_S32 i = 0;
    HI_CHAR* pos = request;
    pos = strtok(request, "=,");
    while (pos)
    {
        if (i % 2 == 0)
        {
            dict[i / 2].key = pos;
        } else {
            dict[i / 2].val = pos;
        }

        if (++i >= dict_num * 2)
            break;
        pos = strtok(NULL, "=,");
    }

    return i / 2;
}

static void printSupportInfo(HI_DRM_SUPPORT_INFO_PPTR info)
{
    HI_ITERATOR_S iter;
    const HI_CHAR* pszVal = NULL;
    LOGI("DRM engine:");
    pszVal = HI_DRM_SUPPORT_INFO_GetDescription(info);
    CHECK(!pszVal, return, "HI_DRM_SUPPORT_INFO_GetDescription fail");
    LOGI("\tDescription:%s", pszVal);
    LOGI("\tDRM engine MimeType:");
    iter = HI_DRM_SUPPORT_INFO_GetMimeTypeIterator(info);
    while (iter.pfnHasNext(&iter))
    {
        pszVal = iter.pfnNext(&iter);
        CHECK(!pszVal, return, "Get iterator next fail");
        LOGI("\t\t%s", pszVal);
    }

    LOGI("\tDRM supported fileSuffix:");
    iter = HI_DRM_SUPPORT_INFO_GetFileSuffixIterator(info);
    while (iter.pfnHasNext(&iter))
    {
        pszVal = iter.pfnNext(&iter);
        CHECK(!pszVal, return, "Get iterator next fail");
        LOGI("\t\t%s", pszVal);
    }
}

static  HI_S32 DEVCERT_BUFFER_PREPARE_ONE(HI_CHAR* file, HI_U8** buf, HI_S32* len)
{
    HI_S32 s32Ret = HI_SUCCESS;
    FILE *fp_DevCert = NULL;
    HI_U32 u32Len;
    HI_U8 *pu8Tmp = NULL;

    fp_DevCert = fopen(file,"rb");
    if(NULL == fp_DevCert)
    {
        printf("open file '%s' error!\n", file);
        return HI_FAILURE;
    }

    fseek(fp_DevCert,0,SEEK_END);
    *len = (HI_S32)ftell(fp_DevCert);
    rewind(fp_DevCert);

    pu8Tmp = (HI_U8 *)malloc(*len);
    if(NULL == pu8Tmp)
    {
        printf("malloc size 0x%x error!\n", *len);
        s32Ret = HI_FAILURE;
        goto __Quit;
    }
    u32Len = fread(pu8Tmp, 1, *len, fp_DevCert);
    if(u32Len != *len)
    {
        printf("Read '%s' failed. real size(0x%x), should be size(0x%08x).\n", file, u32Len, *len);
        s32Ret = HI_FAILURE;
        goto __Quit;
    }

    *buf = pu8Tmp;

__Quit:
    if ( NULL != fp_DevCert)
    {
        fclose(fp_DevCert);
        fp_DevCert = NULL;
    }

    if ( HI_FAILURE == s32Ret)
    {
        if ( NULL != pu8Tmp)
        {
            free(pu8Tmp);
            pu8Tmp = NULL;
        }
    }

    return s32Ret;

}

static HI_VOID DevCert_Buffer_Free(HI_PLAYREADY_DEVCERT_BUF_S *pOpenParam)
{
    if (!pOpenParam)
        return;
    if (pOpenParam->pu8Pri)
    {
        free(pOpenParam->pu8Pri);
    }
    if (pOpenParam->pu8Zgpriv)
    {
        free(pOpenParam->pu8Zgpriv);
    }
    if (pOpenParam->pu8Bgroupcert)
    {
        free(pOpenParam->pu8Bgroupcert);
    }
    if (pOpenParam->pu8Devcerttemplate)
    {
        free(pOpenParam->pu8Devcerttemplate);
    }
}

static  HI_S32 DevCert_Buffer_Prepare(HI_PLAYREADY_DEVCERT_BUF_S *pOpenParam, const HI_CHAR* path)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_CHAR *pf_priv = "priv.dat";
    HI_CHAR *pf_zgpriv = "zgpriv.dat";
    HI_CHAR *pf_bgroupcert = "bgroupcert.dat";
    HI_CHAR *pf_devcerttemplate = "devcerttemplate.dat";
    HI_CHAR buf[1024] = {0};
    if ( NULL == pOpenParam )
    {
        printf("Invalid param, NULL Pointer!\n");
        return HI_FAILURE;
    }

    snprintf(buf, 1024, "%s/%s", path, pf_priv);
    s32Ret = DEVCERT_BUFFER_PREPARE_ONE(buf, &pOpenParam->pu8Pri, &pOpenParam->s32PriLen);
    if ( HI_FAILURE == s32Ret)
    {
        printf("buffer prepare for %s failed!\n", pf_priv);
        goto __Quit;
    }

    snprintf(buf, 1024, "%s/%s", path, pf_zgpriv);
    s32Ret = DEVCERT_BUFFER_PREPARE_ONE(buf, &pOpenParam->pu8Zgpriv, &pOpenParam->s32ZgprivLen);
    if ( HI_FAILURE == s32Ret)
    {
        printf("buffer prepare for %s failed!\n", pf_zgpriv);
        goto __Quit;
    }

    snprintf(buf, 1024, "%s/%s", path, pf_bgroupcert);
    s32Ret = DEVCERT_BUFFER_PREPARE_ONE(buf, &pOpenParam->pu8Bgroupcert, &pOpenParam->s32BgroupcertLen);
    if ( HI_FAILURE == s32Ret)
    {
        printf("buffer prepare for %s failed!\n", pf_bgroupcert);
        goto __Quit;
    }

    snprintf(buf, 1024, "%s/%s", path, pf_devcerttemplate);
    s32Ret = DEVCERT_BUFFER_PREPARE_ONE(buf, &pOpenParam->pu8Devcerttemplate, &pOpenParam->s32DevcerttemplateLen);
    if ( HI_FAILURE == s32Ret)
    {
        printf("buffer prepare for %s failed!\n", pf_devcerttemplate);
        goto __Quit;
    }

    /* This will be called while opening done to free buffers */
    pOpenParam->pfnDestructor = DevCert_Buffer_Free;
__Quit:
    if ( HI_FAILURE == s32Ret)
    {
        DevCert_Buffer_Free(pOpenParam);
        memset(pOpenParam, 0 , sizeof(*pOpenParam));
    }

    return s32Ret;

}

/**
 * acquire a drm info object from drm frameworks
 * @param[in] type the request type
 * @param[in] request request description string
 *          use Json style here, ex: key1=val1&key2=val2
 * @param[in] mimeType
 */
static HI_DRM_INFO_PPTR doAcquireDrmInfo(HI_S32 type, const HI_CHAR* request, const HI_CHAR* mimeType)
{
    HI_DRM_INFO_REQUEST_PPTR mReq;
    CHECK(!bInitialize, return NULL, "drm context not opened");
    HI_S32 ret = HI_DRM_INFO_REQUEST_Create(&mReq, type, mimeType);
    CHECK(ret != HI_SUCCESS, return NULL, "Create request failed");

    if (request)
    {
        HI_CHAR* tmp_str = strdup(request);
        CHECK(!tmp_str, return NULL, "allocate string failed");
        HI_S32 n = parse_json(tmp_str);
        HI_S32 i;
        for (i = 0; i < n; i++)
        {
            ret = HI_DRM_INFO_REQUEST_Put(mReq, dict[i].key, dict[i].val);
            CHECK(ret != HI_DRM_NO_ERROR, break, "put info request failed");
            printf("put %s:%s into request\n", dict[i].key, dict[i].val);
        }
        free(tmp_str);
    }
    HI_DRM_INFO_PPTR info = HI_DRM_AcquireDrmInfo(mClient, mReq);
    HI_DRM_INFO_REQUEST_Destroy(&mReq);
    return info;
}

void doProcessDrmInfo(HI_DRM_INFO_PPTR info, const HI_CHAR* config)
{
    HI_S32 ret;
    CHECK(!bInitialize, return, "drm context not opened");
    if (config)
    {
        HI_CHAR* tmp_str = strdup(config);
        CHECK(!tmp_str, return, "dump string failed");
        HI_S32 n = parse_json(tmp_str);
        HI_S32 i;
        for (i = 0; i < n; i++)
        {
            ret = HI_DRM_INFO_Put(info, dict[i].key, dict[i].val);
            CHECK(ret != HI_DRM_NO_ERROR, break, "put info failed");
            printf("put %s:%s into info\n", dict[i].key, dict[i].val);
        }
    }

    /*TODO:Current not support event*/
//    struct OnInfoListener listener = {onInfo};
//    HI_S32 ret = HI_DRM_SetOnInfoListener(mClient, listener);
//    EXPECT_EQ(ret, HI_DRM_NO_ERROR);

    HI_DRM_INFO_STATUS_PTR infoStatus = HI_DRM_ProcessDrmInfo(mClient, info);
    CHECK(!infoStatus || infoStatus->statusCode != HI_DRM_STATUS_OK, return, "process drm info failed");
    HI_DRM_INFO_STATUS_Destroy(infoStatus);
}

void* drm_get_handle()
{
    if (bInitialize)
        return (void*)mClient;
    HI_S32 ret = HI_DRM_CreateClient(&mClient);
    if (ret == HI_SUCCESS)
    {
        bInitialize = 1;
        return (void*)mClient;
    }
    return NULL;
}

void drm_clear_handle()
{
    if (bInitialize)
    {
        HI_DRM_DestroyClient(&mClient);
        bInitialize = 0;
    }
}

void drm_install()
{
    if (bInitialize)
    {
        HI_DRM_InstallDrmEngine(mClient, "libHiplayreadyEngine.so");
    }
}
/**
 * request drm info and then process it
 * @param[in] type request type
 * @param[in] mimeType
 * @param[in] acquire string used for acquire drm info
 * @param[in] config string used for process drm info
 */
static void drm_transaction(HI_S32 type, const HI_CHAR* mimeType, const HI_CHAR* acquire, const HI_CHAR* config)
{
//    CHECK(!type || !acquire || !config, return, "invalid parameter");
    printf("do transaction, trans:%d; %s; %s; %s\n", type, mimeType, acquire, config);
    HI_DRM_INFO_PPTR info = doAcquireDrmInfo(type, acquire, mimeType);
    CHECK(!info, return, "acquire drm info failed");

    doProcessDrmInfo(info, config);
    HI_DRM_INFO_Destroy(&info);
}

static void drm_openex(const HI_CHAR* path)
{
    HI_DRM_INFO_PPTR info = doAcquireDrmInfo(HI_DRM_PLAYERADY_EXTERN_CERT, NULL,
        "application/vnd.ms-playready");
    CHECK(!info, return, "doAcquireDrmInfo failed");
    HI_DRM_BUFFER_S exBuf = HI_DRM_INFO_GetData(info);
    if (exBuf.data)
    {
        DevCert_Buffer_Prepare((HI_PLAYREADY_DEVCERT_BUF_S *)exBuf.data, path);
    }
    HI_DRM_INFO_STATUS_PTR infoStatus = HI_DRM_ProcessDrmInfo(mClient, info);
    CHECK(!infoStatus || infoStatus->statusCode != HI_DRM_STATUS_OK, return, "process drm info failed");
    HI_DRM_INFO_STATUS_Destroy(infoStatus);
    HI_DRM_INFO_Destroy(&info);
}
void drm_cmd(const HI_CHAR* arg)
{
    HI_S32 ret = HI_FAILURE;
    HI_S32 i = 0;
    const HI_CHAR* pszKey = NULL;
    const HI_CHAR* pszVal = NULL;

    printf("doing drm commands:%s\n", arg);
    if (!strncmp("trans", arg, strlen("trans")))
    {
        HI_S32 type = 0;
        HI_CHAR *mimeType = NULL;
        HI_CHAR *acquire = NULL;
        HI_CHAR *config = NULL;
        sscanf(arg, "trans=%d ", &type);
        mimeType = strchr(arg, ' ');
        if (mimeType)
        {
            *mimeType++ = '\0';
            mimeType += strlen("mimetype=");
            acquire = strchr(mimeType, ' ');
        }

        if (acquire)
        {
            *acquire++ = '\0';
            config = strchr(acquire, ' ');
        }

        if (config)
        {
            *config++ = '\0';
        }

        drm_transaction(type, mimeType, config, acquire);
    }
    else if (!strncmp("GetAllSupportInfo", arg, strlen("GetAllSupportInfo")))
    {
        printf("Execute commands:%s\n", arg);
        HI_S32 len = 0;
        HI_DRM_SUPPORT_INFO_PPTR *supportInfo;
        ret = HI_DRM_GetAllSupportInfo(mClient, &len, &supportInfo);
        CHECK(ret != HI_DRM_NO_ERROR, return, "HI_DRM_GetAllSupportInfo failed");
        for (i = 0; i < len; i++)
        {
            printSupportInfo(supportInfo[i]);
        }
        free(supportInfo);
    }
    else if (!strncmp("GetConstraints", arg, strlen("GetConstraints")))
    {
        printf("Execute commands:%s\n", arg);
        HI_DRM_CONSTRAINTS_PPTR cons = HI_DRM_GetConstraints(mClient, s_file, HI_ACTION_DEFAULT);
        CHECK(!cons, return, "HI_DRM_GetConstraints failed");
        HI_ITERATOR_S it = HI_DRM_CONSTRAINTS_KeyIterator(cons);

        LOGI("Constrains:");
        while (it.pfnHasNext(&it))
        {
            pszKey = it.pfnNext(&it);
            CHECK(!pszKey, break, "Get Constrain key failed");
            pszVal = HI_DRM_CONSTRAINTS_Get(cons, pszKey);
            CHECK(!pszVal, break ,"Get constrain value failed");
            LOGI("\t%s:%s", pszKey, pszVal);
        }
        HI_DRM_CONSTRAINTS_Destroy(&cons);
    }
    else if (!strncmp("GetMetadata", arg, strlen("GetMetadata")))
    {
        printf("Execute commands:%s\n", arg);
        HI_DRM_METADATA_PPTR metadata = HI_DRM_GetMetadata(mClient, s_file);
        CHECK(!metadata, return, "HI_DRM_GetMetadata failed");
        HI_ITERATOR_S it = HI_DRM_METADATA_KeyIterator(metadata);
        LOGI("DRM Matadata:");
        while (it.pfnHasNext(&it))
        {
            pszKey = it.pfnNext(&it);
            CHECK(!pszKey, break, "Get matadata key  failed");
            pszVal = HI_DRM_METADATA_Get(metadata, pszKey);
            CHECK(!pszKey, break, "Get matadata val  failed");
            LOGI("\t%s:%s", pszKey, pszVal);
        }
        HI_DRM_METADATA_Destroy(&metadata);
    }
    else if (!strncmp("InstallDrmEngine", arg, strlen("InstallDrmEngine")))
    {
        /*Must Install DRM engine before setMedia*/
        printf("Execute commands:%s\n", arg);
        HI_CHAR* p = strstr(arg, ".so");
        if (p)
        {
            p[3] = 0;
        }
        ret = HI_DRM_InstallDrmEngine(mClient, &arg[strlen("InstallDrmEngine") + 1]);
        CHECK(ret != HI_DRM_NO_ERROR, ,"install engine failed");
    }
    else if (!strncmp("GetDrmObjectType", arg, strlen("GetDrmObjectType")))
    {
        printf("Not supported commands:%s\n", arg);
    }
    else if (!strncmp("ExternalCert", arg, strlen("ExternalCert")))
    {
        HI_CHAR* p = strstr(arg, " ");
        if (p)
        {
            *p = 0;
        }
        drm_openex((HI_CHAR*)&arg[strlen("ExternalCert") + 1]);
    }
}

HI_S32 drm_acquire_right_progress(const HI_CHAR* mime_type)
{
    HI_S32 progress = 0;
    HI_DRM_INFO_PPTR info = doAcquireDrmInfo(HI_DRM_RIGHTS_ACQUISITION_PROGRESS_INFO, NULL, mime_type);
    CHECK(!info, return HI_FAILURE, "doAcquireDrmInfo failed");
    HI_DRM_INFO_STATUS_PTR infoStatus = HI_DRM_ProcessDrmInfo(mClient, info);
    CHECK(!infoStatus || infoStatus->statusCode != HI_DRM_STATUS_OK, return HI_FAILURE, "process drm info failed");
    progress = infoStatus->drmBuffer.length;/* FIXME: use drmBuffer.length not good */
    HI_DRM_INFO_STATUS_Destroy(infoStatus);
    HI_DRM_INFO_Destroy(&info);
    return progress;
}

static inline HI_S32 drm_acquire_right(const HI_CHAR* mime_type)
{
    drm_transaction(HI_DRM_RIGHTS_ACQUISITION_INFO, mime_type, NULL, NULL);
    return HI_SUCCESS;
}

HI_S32 drm_check_right_status(const HI_CHAR* path, const HI_CHAR* mime_type)
{
    HI_S32 ret = HI_DRM_CheckRightsStatus(mClient, path, HI_ACTION_PLAY);
    s_file = path;
    s_mimeType = mime_type;
    switch (ret) {
    case HI_DRM_RIGHTS_NOT_ACQUIRED: {
        /*acquire right now*/
        return drm_acquire_right(mime_type);
    } break;
    case HI_DRM_RIGHTS_VALID: {
        /*Every thing is OK*/
        return 0;
    } break;
    case HI_DRM_RIGHTS_INVALID:
    case HI_DRM_RIGHTS_EXPIRED: {
        printf("DRM right status error:%d", ret);
        return HI_FAILURE;
    } break;
    default: {
        printf("Unrecognized right status:%d", ret);
        return HI_FAILURE;
    } break;
    }
    return HI_FAILURE;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
