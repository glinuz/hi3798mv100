#include <uboot.h>
#include "hi_db.h"
#include "hi_drv_pdm.h"
#include "hi_common.h"
#include "hi_flash.h"


static PDM_DataDecryptCallBack s_Pdm_DataDecryptCallBack = HI_NULL;


#define PDM_TAG_MAXLEN 512
#define PDM_TAG_VERSION "1.0.0.3"

#define PDM_BASEPARAM_BUFNAME "baseparam"
#define PDM_PLAYPARAM_BUFNAME "playparam"
#define PDM_PLAYDATA_BUFNAME "playdata"

#ifdef HI_MINIBOOT_SUPPORT
extern char *getenv(const char *key);
extern void set_param_data(const char *name, const char *buf, int buflen);
#endif

static HI_PDM_SRCTYPE_E g_enSrcType = HI_PDM_SRCTYPE_FLASH;
static HI_PDM_MEMINFO_S g_stMemInfo = {0};

#ifdef HI_LOADER_SUPPORT
static HI_UNF_DISP_TIMING_S g_stDispTiming =
{
    .VFB           =                               27,
    .VBB           =                               23,
    .VACT          =                              768,
    .HFB           =                              210,
    .HBB           =                               46,
    .HACT          =                             1366,
    .VPW           =                                4,
    .HPW           =                               24,
    .IDV           =                                0,
    .IHS           =                                0,
    .IVS           =                                0,
    .ClockReversal = HI_FALSE,
    .DataWidth     = HI_UNF_DISP_INTF_DATA_WIDTH24,
    .ItfFormat     = HI_UNF_DISP_INTF_DATA_FMT_RGB888,
    .DitherEnable  = HI_FALSE,
    .ClkPara0      =                       0x912ccccc,
    .ClkPara1      =                       0x006d8157,

    //.InRectWidth = 1366,
    //.InRectHeight = 768,
};
#endif

// TODO: Delete
static HI_MCE_MTD_INFO_S g_stMtdInfo;

static PDM_BUFFER_S g_stResBuf[PDM_MAX_RESBUF_NUM];
static HI_U32 g_u32ResBufNum = 0;
static HI_BOOL g_bUseBackupLogo = HI_FALSE;
static HI_BOOL g_bUseBackupFastplay = HI_FALSE;

HI_VOID PDM_GetDefDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam)
{
    HI_S32 i;

    for (i = 0; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
    {
        pstDispParam->stIntf[i].enIntfType = HI_UNF_DISP_INTF_TYPE_BUTT;
    }

    return;
}

#ifdef HI_LOADER_SUPPORT
static HI_VOID LoaderGetDefDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam)
{
    HI_S32 i;

    pstDispParam->enFormat   = HI_UNF_ENC_FMT_720P_50;
    pstDispParam->u32HuePlus = 50;
    pstDispParam->u32Saturation = 50;
    pstDispParam->u32Contrast   = 50;
    pstDispParam->u32Brightness = 50;

    pstDispParam->stDispTiming = g_stDispTiming;

    pstDispParam->enPixelFormat    = HIGO_PF_8888;
    pstDispParam->u32VirtScreenWidth  = 1280;
    pstDispParam->u32VirtScreenHeight = 720;

    pstDispParam->stBgColor.u8Red   = 0x00;
    pstDispParam->stBgColor.u8Green = 0x00;
    pstDispParam->stBgColor.u8Blue  = 0xFF;

    pstDispParam->enSrcDisp = HI_TRUE;
    pstDispParam->bGammaEnable = HI_FALSE;

    pstDispParam->stAspectRatio.enDispAspectRatio = 2;
    pstDispParam->stAspectRatio.u32UserAspectHeight = 0;
    pstDispParam->stAspectRatio.u32UserAspectWidth = 0;

#if defined(HI_DAC_YPBPR_Y) && defined(HI_DAC_YPBPR_PB) && defined(HI_DAC_YPBPR_PR)
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].enIntfType = HI_UNF_DISP_INTF_TYPE_YPBPR;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacY  = HI_DAC_YPBPR_Y;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacPb = HI_DAC_YPBPR_PB;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacPr = HI_DAC_YPBPR_PR;
#endif

    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].enIntfType= HI_UNF_DISP_INTF_TYPE_HDMI;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].unIntf.enHdmi= 0;

#ifdef HI_DAC_CVBS
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS].enIntfType = HI_UNF_DISP_INTF_TYPE_CVBS;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS].unIntf.stCVBS.u8Dac = HI_DAC_CVBS;
#endif

    memset(&(pstDispParam->stOffsetInfo), 0, sizeof(pstDispParam->stOffsetInfo));

    for (i = 0; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
    {
        if ((i != HI_UNF_DISP_INTF_TYPE_YPBPR) && (i !=
        HI_UNF_DISP_INTF_TYPE_CVBS)  && (i != HI_UNF_DISP_INTF_TYPE_HDMI))
        {
            pstDispParam->stIntf[i].enIntfType = HI_UNF_DISP_INTF_TYPE_BUTT;
        }
    }
}
#endif

static HI_S32 PDM_GetMtdInfo(const HI_CHAR *pstrDataName, const HI_CHAR *pBootargs,
                             HI_MTD_INFO_S *pstDataInfo)
{
    HI_CHAR *pstr  = HI_NULL;
    HI_CHAR *pstr2 = HI_NULL;
    HI_CHAR buf[12];

    if ((HI_NULL == pstrDataName) || (HI_NULL == pBootargs) || (HI_NULL == pstDataInfo))
    {
        return HI_FAILURE;
    }

    pstr = strstr(pBootargs, pstrDataName);

    if ((HI_NULL == pstr) || (strlen(pstr) <= strlen(pstrDataName)))
    {
        return HI_FAILURE;
    }

    if (pstr[strlen(pstrDataName)] == '=')
    {
        /*get partition name*/
        pstr += strlen(pstrDataName) + 1; //point to the char after '='
        pstr2 = strstr(pstr, ",");

        if (HI_NULL == pstr2)
        {
            return HI_FAILURE;
        }

        if ((HI_U32)(pstr2 - pstr) >= sizeof(pstDataInfo->MtdName))
        {
            return HI_FAILURE;
        }

        memset(pstDataInfo->MtdName, 0, HI_MTD_NAME_LEN);
        memcpy(pstDataInfo->MtdName, pstr, pstr2 - pstr);

        /*get offset*/
        pstr  = pstr2 + 1;
        pstr2 = strstr(pstr, ",");

        if (HI_NULL == pstr2)
        {
            return HI_FAILURE;
        }

        if ((HI_U32)(pstr2 - pstr) >= sizeof(buf))
        {
            return HI_FAILURE;
        }

        memset(buf, 0, sizeof(buf));
        memcpy(buf, pstr, pstr2 - pstr);
        pstDataInfo->u32Offset = simple_strtoul_2(buf, HI_NULL, 16);

        /*get size*/
        pstr  = pstr2 + 1;
        pstr2 = strstr(pstr, " ");

        if (HI_NULL == pstr2)
        {
            return HI_FAILURE;
        }

        if ((HI_U32)(pstr2 - pstr) >= sizeof(buf))
        {
            return HI_FAILURE;
        }

        memset(buf, 0, sizeof(buf));
        memcpy(buf, pstr, pstr2 - pstr);

        //pstDataInfo->u32Size = simple_strtoul_2(buf, HI_NULL, 16);
    }

    return HI_SUCCESS;
}

HI_S32 PDM_ReadBase(HI_U32 *pAddr)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_S32 ReadLen = 0;
    static HI_BOOL bRead = HI_FALSE;
    HI_CHAR             *pTmp = HI_NULL;
    HI_CHAR Bootarg[BOOT_ARGS_SIZE] = {0};
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32ReadLenth = 0;

    if (HI_TRUE == bRead)
    {
        *pAddr = g_stMemInfo.u32BaseAddr;
        return HI_SUCCESS;
    }

    if (HI_PDM_SRCTYPE_DDR == g_enSrcType)
    {
        Ret = HI_DRV_PDM_AllocReserveMem(PDM_BASEPARAM_BUFNAME, MCE_DEF_BASEPARAM_SIZE, &u32StartPhyAddr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_PDM("Alloc Reserve Mem err!, Ret = %#x\n", Ret);
            return HI_FAILURE;
        }

        memcpy((HI_U32 *)u32StartPhyAddr, (HI_U32 *)g_stMemInfo.u32BaseAddr, MCE_DEF_BASEPARAM_SIZE);

        *pAddr = u32StartPhyAddr;
        bRead = HI_TRUE;

        return HI_SUCCESS;
    }

    memset(g_stMtdInfo.stBase.MtdName, 0, sizeof(g_stMtdInfo.stBase.MtdName));
    strncpy(g_stMtdInfo.stBase.MtdName, PDM_BASE_DEF_NAME, sizeof(g_stMtdInfo.stBase.MtdName) - 1);

    hFlash = HI_Flash_OpenByName(g_stMtdInfo.stBase.MtdName);
    if (HI_INVALID_HANDLE == hFlash)
    {
        pTmp = getenv("bootargs");

        if (HI_NULL == pTmp)
        {
            Ret = HI_FAILURE;
            goto RET;
        }

        memset(Bootarg, 0, BOOT_ARGS_SIZE);
        strncpy(Bootarg, pTmp, BOOT_ARGS_SIZE - 1);

        Ret = PDM_GetMtdInfo(PDM_BASE_DEF_NAME, Bootarg, &g_stMtdInfo.stBase);
        if (HI_SUCCESS == Ret)
        {
            hFlash = HI_Flash_OpenByName(g_stMtdInfo.stBase.MtdName);
            if (HI_INVALID_HANDLE == hFlash)
            {
                HI_INFO_PDM("ERR: Can not find baseparam flash data!");
                Ret = HI_FAILURE;
                goto RET;
            }

            (HI_VOID)HI_Flash_GetInfo(hFlash, &stFlashInfo);
        }
        else
        {
            HI_INFO_PDM("ERR: Can not find baseparam flash data!");
            Ret = HI_FAILURE;
            goto RET;
        }
    }
    else
    {
        g_stMtdInfo.stBase.u32Offset = 0;
        (HI_VOID)HI_Flash_GetInfo(hFlash, &stFlashInfo);
        g_stMtdInfo.stBase.u32Size = stFlashInfo.PartSize;
    }

    u32ReadLenth = stFlashInfo.PageSize > MCE_DEF_BASEPARAM_SIZE ? stFlashInfo.PageSize : MCE_DEF_BASEPARAM_SIZE;

    Ret = HI_DRV_PDM_AllocReserveMem(PDM_BASEPARAM_BUFNAME, u32ReadLenth, &u32StartPhyAddr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PDM("Alloc Reserve Mem err!, Ret = %#x\n", Ret);
        goto FLASH_CLOSE;
    }

    ReadLen = HI_Flash_Read(hFlash, (HI_U64)g_stMtdInfo.stBase.u32Offset, (HI_U8 *)u32StartPhyAddr, u32ReadLenth,
                            HI_FLASH_RW_FLAG_RAW);
    if (ReadLen < 0)
    {
        HI_INFO_PDM("ERR: HI_Flash_Read!");
        Ret = HI_FAILURE;
        goto FLASH_CLOSE;
    }

    if (s_Pdm_DataDecryptCallBack != HI_NULL)
    {
        s_Pdm_DataDecryptCallBack((HI_U8 *)u32StartPhyAddr, u32ReadLenth, (HI_U8 *)u32StartPhyAddr);
    }

    g_stMemInfo.u32BaseAddr = u32StartPhyAddr;

    *pAddr = g_stMemInfo.u32BaseAddr;
    bRead = HI_TRUE;

FLASH_CLOSE:
    HI_Flash_Close(hFlash);
RET:
    return Ret;
}

static HI_S32 PDM_UpdateCustomTiming(HI_DB_TABLE_S *pstTable, HI_UNF_DISP_TIMING_S *pstTiming)
{
    HI_S32 Ret;
    HI_DB_KEY_S stKey;

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VFB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->VFB), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VBB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->VBB), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VACT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->VACT), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_HFB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->HFB), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_HBB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->HBB), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_HACT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->HACT), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VPW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->VPW), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_HPW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->HPW), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_IDV, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->IDV), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_IHS, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->IHS), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_IVS, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->IVS), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_CLKR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->ClockReversal), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_DW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->DataWidth), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_ITFMT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->ItfFormat), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_DITE, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->DitherEnable), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_CLK0, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->ClkPara0), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_CLK1, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->ClkPara1), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_INTERL, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->bInterlace), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_FIXF, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->PixFreq), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VERTF, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->VertFreq), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_AW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->AspectRatioW), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_AH, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->AspectRatioH), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_USEGAMMA, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->bUseGamma), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_RSV0, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->Reserve0), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_RSV1, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstTiming->Reserve1), stKey.u32ValueSize);
    }

    return HI_SUCCESS;
}

HI_S32 PDM_ReadLogoParam(const HI_CHAR *pstrDataName, HI_U32 *pAddr)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_S32 ReadLen = 0;
    HI_CHAR             *pTmp = HI_NULL;
    HI_CHAR Bootarg[BOOT_ARGS_SIZE] = {0};
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U32 u32DataAddr;
    HI_U32 u32ReadLenth = 0;

    if (HI_PDM_SRCTYPE_DDR == g_enSrcType)
    {
        *pAddr = g_stMemInfo.u32LogoAddr;
        return HI_SUCCESS;
    }

    memset(g_stMtdInfo.stLogo.MtdName, 0, sizeof(g_stMtdInfo.stLogo.MtdName));
    strncpy(g_stMtdInfo.stLogo.MtdName, pstrDataName, sizeof(g_stMtdInfo.stLogo.MtdName));
    g_stMtdInfo.stLogo.MtdName[sizeof(g_stMtdInfo.stLogo.MtdName) - 1] = '\0';

    hFlash = HI_Flash_OpenByName(g_stMtdInfo.stLogo.MtdName);
    if (HI_INVALID_HANDLE == hFlash)
    {
        pTmp = getenv("bootargs");
        if (HI_NULL == pTmp)
        {
            Ret = HI_FAILURE;
            goto RET;
        }

        memset(Bootarg, 0, BOOT_ARGS_SIZE);
        strncpy(Bootarg, pTmp, BOOT_ARGS_SIZE - 1);

        Ret = PDM_GetMtdInfo(pstrDataName, Bootarg, &g_stMtdInfo.stLogo);
        if (HI_SUCCESS == Ret)
        {
            hFlash = HI_Flash_OpenByName(g_stMtdInfo.stLogo.MtdName);
            if (HI_INVALID_HANDLE == hFlash)
            {
                HI_ERR_PDM("ERR: Can not find logo flash data!");
                Ret = HI_FAILURE;
                goto RET;
            }

            (HI_VOID)HI_Flash_GetInfo(hFlash, &stFlashInfo);
        }
        else
        {
            HI_ERR_PDM("ERR: Can not find logo flash data!");
            Ret = HI_FAILURE;
            goto RET;
        }
    }
    else
    {
        g_stMtdInfo.stLogo.u32Offset = 0;
        (HI_VOID)HI_Flash_GetInfo(hFlash, &stFlashInfo);
        g_stMtdInfo.stLogo.u32Size = stFlashInfo.PartSize;
    }

    /*read logo param*/
    u32ReadLenth = stFlashInfo.PageSize > MCE_DEF_LOGOPARAM_SIZE ? stFlashInfo.PageSize : MCE_DEF_LOGOPARAM_SIZE;
    u32DataAddr = (HI_U32)malloc(u32ReadLenth);

    ReadLen = HI_Flash_Read(hFlash, (HI_U64)g_stMtdInfo.stLogo.u32Offset, (HI_U8 *)u32DataAddr, u32ReadLenth,
                            HI_FLASH_RW_FLAG_RAW);
    if (ReadLen < 0)
    {
        HI_INFO_PDM("ERR: HI_Flash_Read!, ReadLen = %#x", ReadLen);

        Ret = HI_FAILURE;
        goto FLASH_CLOSE;
    }

    if (s_Pdm_DataDecryptCallBack != HI_NULL)
    {
        s_Pdm_DataDecryptCallBack((HI_U8 *)u32DataAddr, u32ReadLenth, (HI_U8 *)u32DataAddr);
    }

    g_stMemInfo.u32LogoAddr = u32DataAddr;
    *pAddr = u32DataAddr;

FLASH_CLOSE:
    HI_Flash_Close(hFlash);
RET:
    return Ret;
}

HI_S32 PDM_ReadLogoData(HI_CHAR *pstrDataName, HI_U32 u32Size, HI_U32 *pAddr)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_S32 ReadLen = 0;
    HI_CHAR             *pTmp = HI_NULL;
    HI_CHAR Bootarg[BOOT_ARGS_SIZE] = {0};
    HI_MTD_INFO_S stLogoMtdInfo;
    HI_U32 u32DataAddr;

    if (HI_PDM_SRCTYPE_DDR == g_enSrcType)
    {
        *pAddr = g_stMemInfo.u32LogoAddr + MCE_DEF_LOGOPARAM_SIZE;
        return HI_SUCCESS;
    }

    memset(stLogoMtdInfo.MtdName, 0, sizeof(stLogoMtdInfo.MtdName));
    strncpy(stLogoMtdInfo.MtdName, pstrDataName, sizeof(stLogoMtdInfo.MtdName));
    stLogoMtdInfo.MtdName[sizeof(stLogoMtdInfo.MtdName) - 1] = '\0';

    stLogoMtdInfo.u32Offset = 0;
    stLogoMtdInfo.u32Size = u32Size + MCE_DEF_LOGOPARAM_SIZE;

#if !defined(HI_ADVCA_TYPE_NAGRA) && !defined(HI_ADVCA_TYPE_VERIMATRIX)
#ifdef  HI_ADVCA_FUNCTION_RELEASE
    /*The process of 16 Byte alignment is just for decryption. affect no-advca use*/
    if (0 != (stLogoMtdInfo.u32Size % 16))
    {
        stLogoMtdInfo.u32Size += 16 - (stLogoMtdInfo.u32Size % 16);
    }
#endif
#endif

    u32DataAddr = (HI_U32)malloc(stLogoMtdInfo.u32Size);

    hFlash = HI_Flash_OpenByName(pstrDataName);
    if (HI_INVALID_HANDLE == hFlash)
    {
        pTmp = getenv("bootargs");
        if (HI_NULL == pTmp)
        {
            Ret = HI_FAILURE;
            goto RET;
        }

        memset(Bootarg, 0, BOOT_ARGS_SIZE);
        strncpy(Bootarg, pTmp, BOOT_ARGS_SIZE - 1);

        Ret = PDM_GetMtdInfo(pstrDataName, Bootarg, &stLogoMtdInfo);
        if (HI_SUCCESS == Ret)
        {
            hFlash = HI_Flash_OpenByName(stLogoMtdInfo.MtdName);
            if (HI_INVALID_HANDLE == hFlash)
            {
                HI_ERR_PDM("ERR: Can not find logo flash data!");
                Ret = HI_FAILURE;
                goto RET;
            }

            //stLogoMtdInfo.u32Size = u32Size+MCE_DEF_LOGOPARAM_SIZE;
        }
        else
        {
            HI_ERR_PDM("ERR: Can not find logo flash data!");
            Ret = HI_FAILURE;
            goto RET;
        }
    }

    /*read logo data*/
    ReadLen = HI_Flash_Read(hFlash, (HI_U64)stLogoMtdInfo.u32Offset, (HI_U8 *)u32DataAddr, stLogoMtdInfo.u32Size,
                            HI_FLASH_RW_FLAG_RAW);
    if (ReadLen < 0)
    {
        HI_INFO_PDM("ERR: HI_Flash_Read!\n");
        Ret = HI_FAILURE;
        goto FLASH_CLOSE;
    }

    if (s_Pdm_DataDecryptCallBack != HI_NULL)
    {
        s_Pdm_DataDecryptCallBack((HI_U8 *)u32DataAddr, stLogoMtdInfo.u32Size, (HI_U8 *)u32DataAddr);
    }

    *pAddr = u32DataAddr + MCE_DEF_LOGOPARAM_SIZE;

FLASH_CLOSE:
    HI_Flash_Close(hFlash);
RET:
    return Ret;
}

HI_S32 PDM_ReadPlayParam(HI_CHAR *pstrDataName, HI_U32 *pAddr)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_S32 ReadLen = 0;
    HI_CHAR             *pTmp = HI_NULL;
    HI_CHAR Bootarg[BOOT_ARGS_SIZE] = {0};
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32ReadLenth = 0;

    if(HI_PDM_SRCTYPE_DDR == g_enSrcType)
    {
        Ret = HI_DRV_PDM_AllocReserveMem(PDM_PLAYPARAM_BUFNAME, MCE_DEF_PLAYPARAM_SIZE, &u32StartPhyAddr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_PDM("Alloc Reserve Mem err!, Ret = %#x\n", Ret);
            return HI_FAILURE;
        }

        memcpy((HI_U32 *)u32StartPhyAddr, (HI_U32 *)g_stMemInfo.u32PlayAddr, MCE_DEF_PLAYPARAM_SIZE);

        *pAddr = u32StartPhyAddr;
        return HI_SUCCESS;
    }

    memset(g_stMtdInfo.stPlay.MtdName, 0, sizeof(g_stMtdInfo.stPlay.MtdName));
    strncpy(g_stMtdInfo.stPlay.MtdName, pstrDataName, sizeof(g_stMtdInfo.stPlay.MtdName) - 1);

    hFlash = HI_Flash_OpenByName(pstrDataName);
    if (HI_INVALID_HANDLE == hFlash)
    {
        pTmp = getenv("bootargs");
        if (HI_NULL == pTmp)
        {
            Ret = HI_FAILURE;
            goto RET;
        }

        memset(Bootarg, 0, BOOT_ARGS_SIZE);
        strncpy(Bootarg, pTmp, BOOT_ARGS_SIZE - 1);

        Ret = PDM_GetMtdInfo(pstrDataName, Bootarg, &g_stMtdInfo.stPlay);
        if (HI_SUCCESS == Ret)
        {
            hFlash = HI_Flash_OpenByName(g_stMtdInfo.stPlay.MtdName);
            if (HI_INVALID_HANDLE == hFlash)
            {
                HI_ERR_PDM("ERR: Can not find logo flash data!");
                Ret = HI_FAILURE;
                goto RET;
            }

            (HI_VOID)HI_Flash_GetInfo(hFlash, &stFlashInfo);
        }
        else
        {
            HI_ERR_PDM("ERR: Can not find logo flash data!");
            Ret = HI_FAILURE;
            goto RET;
        }
    }
    else
    {
        g_stMtdInfo.stPlay.u32Offset = 0;
        (HI_VOID)HI_Flash_GetInfo(hFlash, &stFlashInfo);
        g_stMtdInfo.stPlay.u32Size = stFlashInfo.PartSize;
    }
    /*read play param*/
    u32ReadLenth = stFlashInfo.PageSize > MCE_DEF_PLAYPARAM_SIZE ? stFlashInfo.PageSize : MCE_DEF_PLAYPARAM_SIZE;

    if (0 == g_stMemInfo.u32PlayAddr)
    {
        Ret = HI_DRV_PDM_AllocReserveMem(PDM_PLAYPARAM_BUFNAME, u32ReadLenth, &u32StartPhyAddr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_PDM("Alloc Reserve Mem err!, Ret = %#x\n", Ret);
            goto FLASH_CLOSE;
        }
    }
    else
    {
        u32StartPhyAddr = g_stMemInfo.u32PlayAddr;
    }

    ReadLen = HI_Flash_Read(hFlash, (HI_U64)g_stMtdInfo.stPlay.u32Offset, (HI_U8 *)u32StartPhyAddr, u32ReadLenth,
                            HI_FLASH_RW_FLAG_RAW);
    if (ReadLen < 0)
    {
        HI_INFO_PDM("ERR: HI_Flash_Read!\n");
        Ret = HI_FAILURE;
        goto FLASH_CLOSE;
    }

    if (s_Pdm_DataDecryptCallBack != HI_NULL)
    {
        s_Pdm_DataDecryptCallBack((HI_U8 *)u32StartPhyAddr, u32ReadLenth, (HI_U8 *)u32StartPhyAddr);
    }

    g_stMemInfo.u32PlayAddr = u32StartPhyAddr;
    *pAddr = u32StartPhyAddr;

FLASH_CLOSE:
    HI_Flash_Close(hFlash);
RET:
    return Ret;
}

HI_S32 PDM_ReadPlayData(HI_CHAR *pstrDataName, HI_U32 u32Size, HI_U32 *pAddr)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_S32 ReadLen = 0;
    HI_CHAR             *pTmp = HI_NULL;
    HI_CHAR Bootarg[BOOT_ARGS_SIZE] = {0};
    HI_MTD_INFO_S stPlayMtdInfo;
    HI_U32 u32StartPhyAddr;

    if(HI_PDM_SRCTYPE_DDR == g_enSrcType)
    {
        Ret = HI_DRV_PDM_AllocReserveMem(PDM_PLAYDATA_BUFNAME, u32Size, &u32StartPhyAddr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_PDM("Alloc Reserve Mem err!, Ret = %#x\n", Ret);
            return HI_FAILURE;
        }

        memcpy((HI_U32 *)u32StartPhyAddr, (HI_U32 *)(g_stMemInfo.u32PlayAddr + MCE_DEF_PLAYPARAM_SIZE), u32Size);

        *pAddr = u32StartPhyAddr;
        return HI_SUCCESS;
    }

    memset(stPlayMtdInfo.MtdName, 0, sizeof(stPlayMtdInfo.MtdName));
    strncpy(stPlayMtdInfo.MtdName, pstrDataName, sizeof(stPlayMtdInfo.MtdName) - 1);

    stPlayMtdInfo.u32Offset = 0;
    stPlayMtdInfo.u32Size = u32Size + MCE_DEF_PLAYPARAM_SIZE;

#if !defined(HI_ADVCA_TYPE_NAGRA) && !defined(HI_ADVCA_TYPE_VERIMATRIX)
#ifdef  HI_ADVCA_FUNCTION_RELEASE
    /*The process of 16 Byte alignment is just for decryption. Do not affect no-advca use */
    if (0 != (stPlayMtdInfo.u32Size % 16))
    {
        stPlayMtdInfo.u32Size += 16 - (stPlayMtdInfo.u32Size % 16);
    }
#endif
#endif

    hFlash = HI_Flash_OpenByName(pstrDataName);
    if (HI_INVALID_HANDLE == hFlash)
    {
        pTmp = getenv("bootargs");
        if (HI_NULL == pTmp)
        {
            Ret = HI_FAILURE;
            goto RET;
        }

        memset(Bootarg, 0, BOOT_ARGS_SIZE);
        strncpy(Bootarg, pTmp, BOOT_ARGS_SIZE - 1);

        Ret = PDM_GetMtdInfo(pstrDataName, Bootarg, &stPlayMtdInfo);
        if (HI_SUCCESS == Ret)
        {
            hFlash = HI_Flash_OpenByName(stPlayMtdInfo.MtdName);
            if (HI_INVALID_HANDLE == hFlash)
            {
                HI_ERR_PDM("ERR: Can not find logo flash data!");
                Ret = HI_FAILURE;
                goto RET;
            }

            //stPlayMtdInfo.u32Size = u32Size+MCE_DEF_PLAYPARAM_SIZE;
        }
        else
        {
            HI_ERR_PDM("ERR: Can not find logo flash data!");
            Ret = HI_FAILURE;
            goto RET;
        }
    }

    Ret = HI_DRV_PDM_AllocReserveMem(PDM_PLAYDATA_BUFNAME, stPlayMtdInfo.u32Size, &u32StartPhyAddr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PDM("Alloc Reserve Mem err!, Ret = %#x\n", Ret);
        goto FLASH_CLOSE;
    }

    /*read play data*/
    ReadLen = HI_Flash_Read(hFlash, (HI_U64)(stPlayMtdInfo.u32Offset), (HI_U8 *)u32StartPhyAddr, stPlayMtdInfo.u32Size,
                            HI_FLASH_RW_FLAG_RAW);
    if (ReadLen < 0)
    {
        HI_ERR_PDM("ERR: HI_Flash_Read, ReadLen:%#x.", ReadLen);
        Ret = HI_FAILURE;
        goto FLASH_CLOSE;
    }

    if (s_Pdm_DataDecryptCallBack != HI_NULL)
    {
        s_Pdm_DataDecryptCallBack((HI_U8 *)u32StartPhyAddr, stPlayMtdInfo.u32Size, (HI_U8 *)u32StartPhyAddr);
    }

    memcpy((HI_U32 *)u32StartPhyAddr, (HI_U32 *)(u32StartPhyAddr + MCE_DEF_PLAYPARAM_SIZE), u32Size);
    *pAddr = u32StartPhyAddr;

FLASH_CLOSE:
    HI_Flash_Close(hFlash);
RET:
    return Ret;
}

HI_VOID PDM_TransFomat(HI_UNF_ENC_FMT_E enSrcFmt, HI_UNF_ENC_FMT_E *penHdFmt, HI_UNF_ENC_FMT_E *penSdFmt)
{
    switch (enSrcFmt)
    {
        /* bellow are tv display formats */
    case HI_UNF_ENC_FMT_1080P_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_1080P_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_1080P_50:
    {
        *penHdFmt = HI_UNF_ENC_FMT_1080P_50;
        *penSdFmt = HI_UNF_ENC_FMT_PAL;
        break;
    }
    case HI_UNF_ENC_FMT_1080i_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_1080i_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_1080i_50:
    {
        *penHdFmt = HI_UNF_ENC_FMT_1080i_50;
        *penSdFmt = HI_UNF_ENC_FMT_PAL;
        break;
    }
    case HI_UNF_ENC_FMT_720P_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_720P_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_720P_50:
    {
        *penHdFmt = HI_UNF_ENC_FMT_720P_50;
        *penSdFmt = HI_UNF_ENC_FMT_PAL;
        break;
    }
    case HI_UNF_ENC_FMT_576P_50:
    {
        *penHdFmt = HI_UNF_ENC_FMT_576P_50;
        *penSdFmt = HI_UNF_ENC_FMT_PAL;
        break;
    }
    case HI_UNF_ENC_FMT_480P_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_480P_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_PAL:
    {
        *penHdFmt = HI_UNF_ENC_FMT_PAL;
        *penSdFmt = HI_UNF_ENC_FMT_PAL;
        break;
    }
    case HI_UNF_ENC_FMT_NTSC:
    {
        *penHdFmt = HI_UNF_ENC_FMT_NTSC;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }

        /* bellow are vga display formats */
    case HI_UNF_ENC_FMT_861D_640X480_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_861D_640X480_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_800X600_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_800X600_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1024X768_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1024X768_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1280X720_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X720_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1280X800_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X800_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1280X1024_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X1024_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1360X768_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1360X768_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1366X768_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1366X768_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1400X1050_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1400X1050_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1440X900_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1440X900_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1440X900_60_RB:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1440X900_60_RB;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1600X900_60_RB:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1600X900_60_RB;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1600X1200_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1600X1200_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1680X1050_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1680X1050_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }

    case HI_UNF_ENC_FMT_VESA_1920X1080_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1920X1080_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_1920X1200_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_1920X1200_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    case HI_UNF_ENC_FMT_VESA_2048X1152_60:
    {
        *penHdFmt = HI_UNF_ENC_FMT_VESA_2048X1152_60;
        *penSdFmt = HI_UNF_ENC_FMT_NTSC;
        break;
    }
    default:
    {
        *penHdFmt = HI_UNF_ENC_FMT_1080i_50;
        *penSdFmt = HI_UNF_ENC_FMT_PAL;
        break;
    }
    }

    return;
}

HI_S32 PDM_MatchFormat(HI_UNF_ENC_FMT_E enSrcFmt, HI_UNF_ENC_FMT_E *penDstFmt)
{
    /*
    * Old version fmt,  HI_UNF_ENC_FMT_3840X2160_24 = 0x100
    * New version fmt, HI_UNF_ENC_FMT_3840X2160_24 = 0x40
    * Need to be adapted to the latest version    
    */
    
    switch ((HI_U32)enSrcFmt)
    {
        case HI_UNF_ENC_ORG_FMT_3840X2160_24:
        {
            *penDstFmt = HI_UNF_ENC_FMT_3840X2160_24;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_3840X2160_25:
        {
            *penDstFmt = HI_UNF_ENC_FMT_3840X2160_25;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_3840X2160_30:
        {
            *penDstFmt = HI_UNF_ENC_FMT_3840X2160_30;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_3840X2160_50:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_3840X2160_50;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_3840X2160_60:
        {           
            *penDstFmt = HI_UNF_ENC_FMT_3840X2160_60;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_4096X2160_24:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_4096X2160_24;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_4096X2160_25:
        {
            *penDstFmt = HI_UNF_ENC_FMT_4096X2160_25;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_4096X2160_30:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_4096X2160_30;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_4096X2160_50:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_4096X2160_50;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_4096X2160_60:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_4096X2160_60;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_3840X2160_23_976:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_3840X2160_23_976;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_3840X2160_29_97:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_3840X2160_29_97;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_720P_59_94:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_720P_59_94;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_1080P_59_94:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_1080P_59_94;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_1080P_29_97:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_1080P_29_97;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_1080P_23_976:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_1080P_23_976;
            break;
        }
        case HI_UNF_ENC_ORG_FMT_1080i_59_94:
        {            
            *penDstFmt = HI_UNF_ENC_FMT_1080i_59_94;
            break;
        }
        default:
        {
            *penDstFmt = enSrcFmt;
            break;
        }
    }

    return HI_SUCCESS;
}

HI_S32 PDM_ReadVersionInfo(HI_DB_S *pstDB, HI_U32 *Version)
{
    HI_S32        Ret;
    HI_DB_TABLE_S stTable;
    HI_DB_KEY_S   stKey;

    Ret = HI_DB_GetTableByName(pstDB, MCE_BASE_TABLENAME_VERSION, &stTable);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("Table: MCE_BASE_TABLENAME_VERSION is not found\n");
        return HI_FAILURE;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_UNF_VERSION, &stKey);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    *Version = *(HI_U32 *)stKey.pValue;

    return HI_SUCCESS;

}

HI_S32 HI_DRV_PDM_SetSource(HI_PDM_SRCTYPE_E enSrcType, HI_PDM_MEMINFO_S *pstMemInfo)
{
    if (HI_NULL == pstMemInfo)
    {
        return HI_FAILURE;
    }

    if (HI_PDM_SRCTYPE_DDR != enSrcType)
    {
        HI_ERR_PDM("ERR: only support set source type ddr!\n");
        return HI_FAILURE;
    }

    g_enSrcType = enSrcType;
    g_stMemInfo = *pstMemInfo;

    return HI_SUCCESS;
}

static HI_S32 PDM_GetCustomTiming(HI_DB_TABLE_S *pstTable, HI_UNF_DISP_TIMING_S *pstTiming)
{
    HI_S32 Ret;
    HI_DB_KEY_S stKey;

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VFB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->VFB = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VBB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->VBB = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VACT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->VACT = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_HFB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->HFB = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_HBB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->HBB = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_HACT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->HACT = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VPW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->VPW = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_HPW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->HPW = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_IDV, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->IDV = *(HI_BOOL*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_IHS, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->IHS = *(HI_BOOL*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_IVS, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->IVS = *(HI_BOOL*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_CLKR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->ClockReversal = *(HI_BOOL*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_DW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->DataWidth = *(HI_UNF_DISP_INTF_DATA_WIDTH_E*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_ITFMT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->ItfFormat = *(HI_UNF_DISP_INTF_DATA_FMT_E*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_DITE, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->DitherEnable = *(HI_BOOL*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_CLK0, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->ClkPara0 = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_CLK1, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->ClkPara1 = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_INTERL, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->bInterlace = *(HI_BOOL*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_FIXF, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->PixFreq = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_VERTF, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->VertFreq = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_AW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->AspectRatioW = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_AH, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->AspectRatioH = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_USEGAMMA, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->bUseGamma = *(HI_BOOL*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_RSV0, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->Reserve0 = *(HI_U32*)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(pstTable, MCE_BASE_KEYNAME_TIMING_RSV1, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstTiming->Reserve1 = *(HI_U32*)(stKey.pValue);
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_PDM_GetDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam)
{
    HI_S32                  Ret;
    HI_U32                  BaseDBAddr;
    HI_DB_S                 stBaseDB;
    HI_DB_TABLE_S           stTable;
    HI_DB_KEY_S             stKey;
    HI_U32                  Version;
    HI_UNF_ENC_FMT_E        enFmt;

    PDM_GetDefDispParam(enDisp, pstDispParam);

    Ret = PDM_ReadBase(&BaseDBAddr);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: PDM_ReadBase, use default baseparam!\n");
#ifdef HI_LOADER_SUPPORT
        LoaderGetDefDispParam(enDisp, pstDispParam);
        return HI_SUCCESS;
#endif
        return HI_FAILURE;
    }

    Ret = HI_DB_GetDBFromMem((HI_U8 *)BaseDBAddr, &stBaseDB);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetDBFromMem, use default baseparam!\n");
#ifdef HI_LOADER_SUPPORT
        LoaderGetDefDispParam(enDisp, pstDispParam);
        return HI_SUCCESS;
#endif
        return HI_FAILURE;
    }

    if (HI_UNF_DISPLAY0 == enDisp)
    {
        Ret = HI_DB_GetTableByName(&stBaseDB, MCE_BASE_TABLENAME_DISP0, &stTable);
    }
    else
    {
        Ret = HI_DB_GetTableByName(&stBaseDB, MCE_BASE_TABLENAME_DISP1, &stTable);
    }

    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetTableByName, use default baseparam!\n");
        return HI_SUCCESS;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_FMT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->enFormat = *(HI_UNF_ENC_FMT_E *)(stKey.pValue);

        if (HI_UNF_ENC_FMT_BUTT == pstDispParam->enFormat)
        {
            Ret = PDM_GetCustomTiming(&stTable, &(pstDispParam->stDispTiming));
        }

        if (HI_UNF_DISPLAY1 == enDisp)
        {
             /*
             * if there is not a UNF version number on the baseparam image
             * Convert format, HI_UNF_ENC_FMT_3840X2160_24...HI_UNF_ENC_FMT_1080i_59_94
             */
            if (HI_FAILURE == PDM_ReadVersionInfo(&stBaseDB, &Version))
            {
                if (HI_SUCCESS == PDM_MatchFormat(pstDispParam->enFormat, &enFmt))
                {
                    pstDispParam->enFormat = enFmt;
                }
            }
            else
            {
            }
        }
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_HDMI, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
        HI_INFO_PDM("DISP%d, HDMI:%d\n", enDisp, pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].enIntfType);
        HI_INFO_PDM("DISP%d, HDMI:%d\n", enDisp, pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].unIntf.enHdmi);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_YPBPR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
        HI_INFO_PDM("DISP%d, YPbPR:%d [%d, %d, %d]\n", enDisp,
                    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].enIntfType,
                    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacY,
                    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacPb,
                    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacPr);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_CVBS, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
        HI_INFO_PDM("DISP%d, CVBS:%d\n", enDisp, pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS].unIntf.stCVBS.u8Dac);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_RGB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_RGB] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_SVIDEO, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_SVIDEO] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_BT1120, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_BT1120] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_BT656, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_BT656] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_LCD, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_LCD] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_VGA, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_VGA] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_HULEP, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->u32HuePlus = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SATU, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->u32Saturation = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_CONTR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->u32Contrast = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_BRIG, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->u32Brightness = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_BGCOLOR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stBgColor = *(HI_UNF_DISP_BG_COLOR_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_GAMA, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->bGammaEnable = *(HI_BOOL *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_ASPECT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stAspectRatio = *(HI_UNF_DISP_ASPECT_RATIO_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SRC_DISP, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->enSrcDisp = *(HI_UNF_DISP_E *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_VIRSCW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->u32VirtScreenWidth = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_VIRSCH, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->u32VirtScreenHeight = *(HI_U32 *)(stKey.pValue);
    }

    if ((pstDispParam->u32VirtScreenWidth > 3840)
        || (pstDispParam->u32VirtScreenWidth < 480)
        || (pstDispParam->u32VirtScreenHeight > 3840)
        || (pstDispParam->u32VirtScreenHeight < 480)
    )
    {
        HI_INFO_PDM("Invalid vir screen:W %d, H %d\n",
            pstDispParam->u32VirtScreenWidth, pstDispParam->u32VirtScreenHeight);

        pstDispParam->u32VirtScreenWidth = 1280;
        pstDispParam->u32VirtScreenHeight = 720;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISP_L, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stOffsetInfo.u32Left = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISP_T, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stOffsetInfo.u32Top = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISP_R, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stOffsetInfo.u32Right = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISP_B, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->stOffsetInfo.u32Bottom = *(HI_U32 *)(stKey.pValue);
    }

    if ((pstDispParam->stOffsetInfo.u32Left > 200)
        || (pstDispParam->stOffsetInfo.u32Top > 200)
        || (pstDispParam->stOffsetInfo.u32Right > 200)
        || (pstDispParam->stOffsetInfo.u32Bottom > 200)
    )
    {
        HI_INFO_PDM("Invalid offset:T %d, B %d, L %d, R %d\n",
            pstDispParam->stOffsetInfo.u32Top,
            pstDispParam->stOffsetInfo.u32Bottom,
            pstDispParam->stOffsetInfo.u32Left,
            pstDispParam->stOffsetInfo.u32Right);

        pstDispParam->stOffsetInfo.u32Left = 0;
        pstDispParam->stOffsetInfo.u32Top = 0;
        pstDispParam->stOffsetInfo.u32Right = 0;
        pstDispParam->stOffsetInfo.u32Bottom = 0;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_PF, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->enPixelFormat = *(HIGO_PF_E *)(stKey.pValue);
        if ((pstDispParam->enPixelFormat < HIGO_PF_4444) || (pstDispParam->enPixelFormat > HIGO_PF_0888))
        {
            HI_INFO_PDM("Invalid enPixelFormat:%d\n", pstDispParam->enPixelFormat);

            pstDispParam->enPixelFormat = HIGO_PF_8888;
        }
    }
    else
    {
        pstDispParam->enPixelFormat = HIGO_PF_8888;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_HDMI_VIDEOMODE, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->enVidOutMode = *(HI_UNF_HDMI_VIDEO_MODE_E *)(stKey.pValue);
    }
    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_HDMI_DEEPCOLOR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstDispParam->enDeepColorMode = *(HI_UNF_HDMI_DEEP_COLOR_E *)(stKey.pValue);
    }
    HI_INFO_PDM("DISP%d:fmt:%d, pixelfmt:%d, offset:[%d, %d, %d, %d], stAspectRatio:[%d, %d, %d]\n",
                enDisp, pstDispParam->enFormat, pstDispParam->enPixelFormat,
                pstDispParam->stOffsetInfo.u32Left, pstDispParam->stOffsetInfo.u32Top,
                pstDispParam->stOffsetInfo.u32Right, pstDispParam->stOffsetInfo.u32Bottom,
                pstDispParam->stAspectRatio.enDispAspectRatio,
                pstDispParam->stAspectRatio.u32UserAspectWidth,
                pstDispParam->stAspectRatio.u32UserAspectHeight);
    HI_INFO_PDM("DISP%d:srcdisp:%d, virscreen:[%d, %d], BGColor:[%d, %d, %d], kinds of degree:[B:%d, C:%d, H:%d, S:%d]\n",
                enDisp, pstDispParam->enSrcDisp, pstDispParam->u32VirtScreenWidth, pstDispParam->u32VirtScreenHeight,
                pstDispParam->stBgColor.u8Red, pstDispParam->stBgColor.u8Green, pstDispParam->stBgColor.u8Blue,
                pstDispParam->u32Brightness, pstDispParam->u32Contrast, pstDispParam->u32HuePlus,
                pstDispParam->u32Saturation
    );

    HI_INFO_PDM("VideOutMode: %d, DeepColorMode: %d\n", pstDispParam->enVidOutMode, pstDispParam->enDeepColorMode);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_PDM_GetPmocParam(HI_PDM_PMOCPARAM_S *pstPmocParam)
{
    HI_S32 Ret;
    HI_U32 BaseDBAddr;
    HI_DB_S stBaseDB;
    HI_DB_TABLE_S stTable;
    HI_DB_KEY_S stKey;

    if (HI_NULL == pstPmocParam)
    {
        HI_ERR_PDM("ERR:pstPmocParam is NULL\n");
        return HI_FAILURE;
    }

    Ret = PDM_ReadBase(&BaseDBAddr);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: PDM_ReadBase, use default baseparam!\n");
        return HI_FAILURE;
    }

    Ret = HI_DB_GetDBFromMem((HI_U8 *)BaseDBAddr, &stBaseDB);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetDBFromMem, use default baseparam!\n");
        return HI_FAILURE;
    }

    Ret = HI_DB_GetTableByName(&stBaseDB, MCE_BASE_TABLENAME_PMOC, &stTable);

    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetTableByName, use default baseparam!\n");
        return HI_SUCCESS;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_PMOC_FLAG, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstPmocParam->u32Flag = *(HI_U32 *)(stKey.pValue);
        HI_INFO_PDM("Pmoc Param:%d\n", pstPmocParam->u32Flag);
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_PDM_UpdateDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam)
{
    HI_S32 Ret;
    HI_DB_S stBaseDB;
    HI_DB_TABLE_S stTable;
    HI_DB_KEY_S stKey;
    HI_U32 BaseDBAddr;

    if (enDisp >= HI_UNF_DISPLAY_BUTT)
    {
        HI_INFO_PDM("ERR: Param invalid!\n");
        return HI_FAILURE;
    }

    Ret = PDM_ReadBase(&BaseDBAddr);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: PDM_ReadBase!\n");
        return HI_FAILURE;
    }

    Ret = HI_DB_GetDBFromMem((HI_U8 *)BaseDBAddr, &stBaseDB);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PDM("ERR: HI_DB_GetDBFromMem!");
        return Ret;
    }

    if (HI_UNF_DISPLAY0 == enDisp)
    {
        Ret = HI_DB_GetTableByName(&stBaseDB, MCE_BASE_TABLENAME_DISP0, &stTable);
    }
    else
    {
        Ret = HI_DB_GetTableByName(&stBaseDB, MCE_BASE_TABLENAME_DISP1, &stTable);
    }

    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PDM("ERR: HI_DB_GetTableByName!");
        return Ret;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_FMT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->enFormat), stKey.u32ValueSize);

        if (HI_UNF_ENC_FMT_BUTT == pstDispParam->enFormat)
        {
            Ret = PDM_UpdateCustomTiming(&stTable, &(pstDispParam->stDispTiming));
        }
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_HDMI, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_YPBPR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_CVBS, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_RGB, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_RGB]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_SVIDEO, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_SVIDEO]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_BT1120, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_BT1120]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_BT656, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_BT656]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_LCD, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_LCD]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_VGA, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_VGA]), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_PF, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->enPixelFormat), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SRC_DISP, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->enSrcDisp), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_VIRSCW, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->u32VirtScreenWidth), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_VIRSCH, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->u32VirtScreenHeight), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISP_L, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stOffsetInfo.u32Left), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISP_T, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stOffsetInfo.u32Top), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISP_R, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stOffsetInfo.u32Right), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISP_B, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stOffsetInfo.u32Bottom), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_HULEP, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->u32HuePlus), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SATU, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->u32Saturation), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_CONTR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->u32Contrast), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_BRIG, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->u32Brightness), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_BGCOLOR, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stBgColor), stKey.u32ValueSize);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_ASPECT, &stKey);
    if (HI_SUCCESS == Ret)
    {
        memcpy(stKey.pValue, &(pstDispParam->stAspectRatio), stKey.u32ValueSize);
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_PDM_GetLogoParam(HI_LOGO_PARAM_S *pstLogoParam)
{
    HI_S32 Ret;
    HI_U32 LogoDBAddr;
    HI_DB_S stLogoDB;
    HI_DB_TABLE_S stTable;
    HI_DB_KEY_S stKey;

    Ret = PDM_ReadLogoParam(PDM_LOGO_DEF_NAME, &LogoDBAddr);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("there is no logo param!\n");
        pstLogoParam->u32CheckFlag = 0;
        return HI_SUCCESS;
    }

    Ret = HI_DB_GetDBFromMem((HI_U8 *)LogoDBAddr, &stLogoDB);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetDBFromMem!\n");

        Ret = PDM_ReadLogoParam(PDM_LOGOBACKUP_DEF_NAME, &LogoDBAddr);
        if (HI_SUCCESS != Ret)
        {
            HI_INFO_PDM("there is no logobak param!\n");
            pstLogoParam->u32CheckFlag = 0;
            return HI_SUCCESS;
        }

        Ret = HI_DB_GetDBFromMem((HI_U8 *)LogoDBAddr, &stLogoDB);
        if (HI_SUCCESS != Ret)
        {
            HI_INFO_PDM("ERR: HI_DB_GetDBFromMem!\n");
            return HI_FAILURE;
        }

        g_bUseBackupLogo = HI_TRUE;
    }

    Ret = HI_DB_GetTableByName(&stLogoDB, MCE_LOGO_TABLENAME, &stTable);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetTableByName!\n");
        return HI_FAILURE;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_LOGO_KEYNAME_FLAG, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstLogoParam->u32CheckFlag = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstLogoParam->u32CheckFlag = 0;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_LOGO_KEYNAME_CONTLEN, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstLogoParam->u32LogoLen = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstLogoParam->u32LogoLen = 0;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_PDM_GetLogoData(HI_U32 Len, HI_U32 *pLogoDataAddr)
{
    if (HI_NULL == pLogoDataAddr)
    {
        return HI_FAILURE;
    }

    if (0 == Len)
    {
        return HI_FAILURE;
    }

    if (!g_bUseBackupLogo)
    {
        return PDM_ReadLogoData(PDM_LOGO_DEF_NAME, Len, pLogoDataAddr);
    }
    else
    {
        return PDM_ReadLogoData(PDM_LOGOBACKUP_DEF_NAME, Len, pLogoDataAddr);
    }
}

HI_S32 HI_DRV_PDM_GetMceParam(HI_MCE_PARAM_S *pstMceParam)
{
    HI_S32 Ret;
    HI_U32 PlayDBAddr;
    HI_DB_S stPlayDB;
    HI_DB_TABLE_S stTable;
    HI_DB_KEY_S stKey;
    HI_BOOL bTryBackup = HI_FALSE;

    Ret = PDM_ReadPlayParam(PDM_FASTPLAY_DEF_NAME, &(PlayDBAddr));
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: PDM_ReadPlayParam\n");
        return Ret;
    }

    Ret = HI_DB_GetDBFromMem((HI_U8 *)PlayDBAddr, &stPlayDB);
    if (HI_SUCCESS == Ret)
    {
        Ret = HI_DB_GetTableByName(&stPlayDB, MCE_PLAY_TABLENAME, &stTable);
        if (HI_SUCCESS != Ret)
        {
            HI_INFO_PDM("ERR: HI_DB_GetTableByName!\n");
            bTryBackup = HI_TRUE;;
        }
        else
        {
            Ret = HI_DB_GetKeyByName(&stTable, MCE_PLAY_KEYNAME_FLAG, &stKey);
            if (HI_SUCCESS == Ret)
            {
                pstMceParam->u32CheckFlag = *(HI_U32 *)(stKey.pValue);
            }
            else
            {
                pstMceParam->u32CheckFlag = 0;
            }

            if (1 != pstMceParam->u32CheckFlag)
            {
                bTryBackup = HI_TRUE;
            }
        }
    }
    else
    {
        bTryBackup = HI_TRUE;
    }

    if (HI_TRUE == bTryBackup)
    {
        Ret = PDM_ReadPlayParam(PDM_FASTPLAYBACKUP_DEF_NAME, &(PlayDBAddr));
        if(HI_SUCCESS != Ret)
        {
            HI_INFO_PDM("ERR: PDM_ReadPlayParam\n");
            return Ret;
        }

        Ret = HI_DB_GetDBFromMem((HI_U8 *)PlayDBAddr, &stPlayDB);
        if(HI_SUCCESS != Ret)
        {
            HI_INFO_PDM("ERR: HI_DB_GetDBFromMem!\n");
            return HI_FAILURE;
        }

        g_bUseBackupFastplay = HI_TRUE;
    }

    Ret = HI_DB_GetTableByName(&stPlayDB, MCE_PLAY_TABLENAME, &stTable);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetTableByName!\n");
        return HI_FAILURE;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_PLAY_KEYNAME_FLAG, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstMceParam->u32CheckFlag = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstMceParam->u32CheckFlag = 0;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_PLAY_KEYNAME_DATALEN, &stKey);
    if (HI_SUCCESS == Ret)
    {
        pstMceParam->u32PlayDataLen = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstMceParam->u32PlayDataLen = 0;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_PDM_GetMceData(HI_U32 u32Size, HI_U32 *pAddr)
{
    if (HI_NULL == pAddr)
    {
        return HI_FAILURE;
    }

    if (!g_bUseBackupFastplay)
    {
        return PDM_ReadPlayData(PDM_FASTPLAY_DEF_NAME, u32Size, pAddr);
    }
    else
    {
        return PDM_ReadPlayData(PDM_FASTPLAYBACKUP_DEF_NAME, u32Size, pAddr);
    }
}

#ifndef HI_MINIBOOT_SUPPORT
extern void *reserve_mem_alloc(unsigned int size, unsigned int *allocsize);
#endif

/* Alloc reserve memory in boot */
HI_S32 HI_DRV_PDM_AllocReserveMem(const HI_CHAR *BufName, HI_U32 u32Len, HI_U32 *pu32PhyAddr)
{
    HI_U32 AlignSize;

    //*pu32PhyAddr = (HI_U32)malloc(u32Len);
    *pu32PhyAddr = (HI_U32)reserve_mem_alloc(u32Len, &AlignSize);
    if (HI_NULL == *pu32PhyAddr)
    {
        HI_ERR_PDM("malloc err\n");
        return HI_FAILURE;
    }

    if (g_u32ResBufNum >= PDM_MAX_RESBUF_NUM)
    {
        HI_ERR_PDM("ERR: you have alloc the max num reserve mem!\n");
        return HI_FAILURE;
    }

    strncpy(g_stResBuf[g_u32ResBufNum].Name, BufName, strlen(BufName));
    g_stResBuf[g_u32ResBufNum].PhyAddr = *pu32PhyAddr;
    g_stResBuf[g_u32ResBufNum].Len = AlignSize;

    g_u32ResBufNum++;

    return HI_SUCCESS;
}

/*
set the reserve addresss to tag.

the pdm tag format is:
version=1.0.0.0  fb=0x85000000,0x10000  baseparam=0x86000000,0x2000 ……
 */
HI_VOID HI_DRV_PDM_SetTagData(HI_VOID)
{
    HI_S32 i;
    HI_CHAR PdmTag[PDM_TAG_MAXLEN];
    HI_CHAR *p = PdmTag;

    memset(PdmTag, 0x0, PDM_TAG_MAXLEN);

    snprintf(p, sizeof(PdmTag), "version=%s ", PDM_TAG_VERSION);

    p += strlen(p);

    for (i = 0; i < g_u32ResBufNum; i++)
    {
        if (!strncmp(g_stResBuf[i].Name, PDM_BASEPARAM_BUFNAME,sizeof(PDM_BASEPARAM_BUFNAME)))
        {
                snprintf(p, sizeof(PdmTag) - strlen(p), "%s=0x%x,0x%x,%s ", g_stResBuf[i].Name, g_stResBuf[i].PhyAddr,
                         g_stResBuf[i].Len, "nosnapshot");
        }
        else
        {
                snprintf(p, sizeof(PdmTag) - strlen(p), "%s=0x%x,0x%x ", g_stResBuf[i].Name, g_stResBuf[i].PhyAddr,
                         g_stResBuf[i].Len);
        }

        p += strlen(p);
    }

    *p = '\0';

    set_param_data("pdm_tag", PdmTag, strlen(PdmTag));

    HI_INFO_PDM("tag is: %s\n", PdmTag);

    return;
}

static HI_S32 PDM_VerifyDeviceInfo(HI_CHAR * pDevInfoBuf, HI_U32 u32DevInfoBufLen, HI_U32 *pu32ValidDevInfoLen)
{
    HI_U32 u32PosIndex = 0;
    HI_S32 s32Ret = HI_FAILURE;

    /* locate magic num */
    while(u32PosIndex < u32DevInfoBufLen)
    {
        HI_CHAR *pCurPos = strstr(&pDevInfoBuf[u32PosIndex], "MAGICNUM");

        if (pCurPos && '=' == *(pCurPos + strlen("MAGICNUM")))
        {
            /* +1 for skip '=' */
            if (!strncmp(pCurPos + strlen("MAGICNUM") + 1, PDM_DEVICE_INFO_DEF_NAME, sizeof(PDM_DEVICE_INFO_DEF_NAME)))
            {
                /* magic num is ok ,get device info valid length */
                u32PosIndex += strlen(&pDevInfoBuf[u32PosIndex]) + 1;   /* +1 for skip \0 */
                pCurPos = strstr(&pDevInfoBuf[u32PosIndex], "LENGTH");
                if (pCurPos && '=' == *(pCurPos + strlen("LENGTH")))
                {
                    *pu32ValidDevInfoLen = simple_strtoul_2(pCurPos + strlen("LENGTH") + 1, HI_NULL, 16); /* +1 for skip '=' */
                    s32Ret = HI_SUCCESS;
                    goto out;
                }
                else
                {
                    HI_ERR_PDM("invalid device info part, no found valid length.\n");
                    goto out;
                }
            }
            else
            {
                HI_ERR_PDM("invalid device info part, magicnum dismatched.\n");
                goto out;
            }
        }
        else
        {
            u32PosIndex += strlen(&pDevInfoBuf[u32PosIndex]) + 1;   /* +1 for skip \0 */
        }
    }

out:
    return s32Ret;
}

/*
 * raw data example(MAC+\0+SN+\0+MAGICNUM+\0+LENGTH+\0+OUI+\0+HW+\0):
 * fc48efc93536\0ab56fe98\0MAGICNUM=deviceinfo\0LENGTH=12345678\0OUI=12345678\
0HW=98765432\0
 */
static HI_S32 PDM_GetDeviceCfgItem(HI_CHAR *pstrCfgName,  HI_CHAR * pOutBuf, HI_U32 u32OutBuflen, HI_CHAR * pInBuf, HI_U32 u32InBuflen)
{
    HI_U32 u32PosIndex = 0, u32DevInfoLen = 0;
    HI_S32 s32Ret = HI_FAILURE;

    /* verify device info data */
    s32Ret = PDM_VerifyDeviceInfo(pInBuf, u32InBuflen, &u32DevInfoLen);
    if (HI_SUCCESS != s32Ret)
    {
        goto out;
    }

    /* because of compatibility reasons mac and sn do a special deal */
     if (!strncmp(pstrCfgName, "MAC", sizeof("MAC")))
    {
        strncpy(pOutBuf, &pInBuf[u32PosIndex], u32OutBuflen );
        pOutBuf[u32OutBuflen - 1] = '\0';
        s32Ret = HI_SUCCESS;
        goto out;
    }
    else  /* locate next cfg item */
    {
        u32PosIndex += strlen(&pInBuf[u32PosIndex]) + 1; /* +1 for skip \0 */
        if (u32PosIndex > u32DevInfoLen)         /* pos index overflow */
        {
            HI_ERR_PDM("pos index(%u) overflowed.\n", u32PosIndex);
            s32Ret = HI_FAILURE;
            goto out;
        }
    }

     if (!strncmp(pstrCfgName, "SN", sizeof("SN")))
     {
        strncpy(pOutBuf, &pInBuf[u32PosIndex], u32OutBuflen);
        pOutBuf[u32OutBuflen - 1] = '\0';
        s32Ret = HI_SUCCESS;
        goto out;
     }
     else /* locate next cfg item */
     {
        u32PosIndex += strlen(&pInBuf[u32PosIndex]) + 1; /* +1 for skip \0 */
        if (u32PosIndex > u32DevInfoLen) /* pos index overflow */
        {
            HI_ERR_PDM("pos index(%u) overflowed.\n", u32PosIndex);
            s32Ret = HI_FAILURE;
            goto out;
        }
     }

    /* search XXX=YYY style cfg item */
    while(u32PosIndex < u32DevInfoLen)
    {
        HI_CHAR *pCurPos = strstr(&pInBuf[u32PosIndex], pstrCfgName);

        if (pCurPos && '=' == *(pCurPos + strlen(pstrCfgName)))
        {
            strncpy(pOutBuf, pCurPos + strlen(pstrCfgName) + 1, u32OutBuflen); /* + 1 for skip '=' */
            pOutBuf[u32OutBuflen - 1] = '\0';
            s32Ret = HI_SUCCESS;
            break;
        }
        else /* locate next cfg item */
        {
            u32PosIndex += strlen(&pInBuf[u32PosIndex]) + 1;   /* +1 for skip \0 */
        }
    }

out:
    return s32Ret;

}

HI_S32 HI_DRV_PDM_GetDeviceInfo(HI_CHAR * pstrCfgName, HI_CHAR * pstrCfgValue, HI_U32 u32Size)
{
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_CHAR  *pDevInfoBuf, *pstrBootargs;
    HI_U32 u32DevInfoBufLen = 0;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_MTD_INFO_S stMtdInfo;
    HI_CHAR Bootarg[BOOT_ARGS_SIZE] ;
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pstrCfgName || HI_NULL == pstrCfgValue || 0 == u32Size)
    {
        HI_ERR_PDM("ERR: param is invalid!");
        goto out0;
    }

    hFlash = HI_Flash_OpenByName(PDM_DEVICE_INFO_DEF_NAME);
    /* shared partition contition */
    if (HI_INVALID_HANDLE == hFlash)
    {
        pstrBootargs = getenv("bootargs");
        if (HI_NULL == pstrBootargs)
        {
            HI_ERR_PDM("ERR: no bootargs env!");
            goto out0;
        }

        strncpy(Bootarg, pstrBootargs, BOOT_ARGS_SIZE);
        Bootarg[BOOT_ARGS_SIZE -1] = '\0';

        memset(&stMtdInfo, 0, sizeof(stMtdInfo));
        s32Ret = PDM_GetMtdInfo(PDM_DEVICE_INFO_DEF_NAME, Bootarg, &stMtdInfo);
        if (HI_SUCCESS == s32Ret)
        {
            hFlash = HI_Flash_OpenByName(stMtdInfo.MtdName);
            if (HI_INVALID_HANDLE == hFlash)
            {
                HI_INFO_PDM("ERR: Can not find '%s' flash data!", PDM_DEVICE_INFO_DEF_NAME);
                s32Ret = HI_FAILURE;
                goto out0;
            }
        }
        else
        {
            HI_INFO_PDM("ERR: no found '%s' partition.\n", PDM_DEVICE_INFO_DEF_NAME);
            s32Ret = HI_FAILURE;
            goto out0;
        }
    }
    else  /* none shared partition contition */
    {
        stMtdInfo.u32Offset = 0;
        strncpy(stMtdInfo.MtdName, PDM_DEVICE_INFO_DEF_NAME,  strlen(PDM_DEVICE_INFO_DEF_NAME)+1);
        stMtdInfo.MtdName[strlen(PDM_DEVICE_INFO_DEF_NAME)] = '\0';
        (HI_VOID)HI_Flash_GetInfo(hFlash, &stFlashInfo);
        stMtdInfo.u32Size = stFlashInfo.PartSize;
    }

    u32DevInfoBufLen = stMtdInfo.u32Offset + stMtdInfo.u32Size;

    s32Ret = HI_MEM_Alloc((HI_U32*)&pDevInfoBuf, u32DevInfoBufLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PDM("Failed to malloc space(%u bytes).", u32DevInfoBufLen);
        goto out;
    }

    if (0 >= HI_Flash_Read(hFlash, (HI_U64)0, (HI_U8 *)pDevInfoBuf, u32DevInfoBufLen, HI_FLASH_RW_FLAG_RAW))
    {
        HI_ERR_PDM("Failed to read block.");
        goto out;
    }

    s32Ret = PDM_GetDeviceCfgItem(pstrCfgName, pstrCfgValue, u32Size, pDevInfoBuf + stMtdInfo.u32Offset, stMtdInfo.u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PDM("parse device cfg item failed.\n");
        goto out;
    }

out:
    HI_Flash_Close(hFlash);
out0:
    return s32Ret;
}

HI_S32 HI_DRV_PDM_RegDecryptCallBack(PDM_DataDecryptCallBack pCallBackFunc)
{
   if (pCallBackFunc == HI_NULL)
   {
       HI_ERR_PDM("Parameter is NULL !");
       return HI_FAILURE;
   }

   s_Pdm_DataDecryptCallBack = pCallBackFunc;

   return HI_SUCCESS;
}
