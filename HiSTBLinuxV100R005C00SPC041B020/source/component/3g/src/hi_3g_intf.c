/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_3g_intf.c
Version       : Initial Draft
Author        : Hisilicon OSDRV group
Created       : 2013-08-18
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "3g_intf.h"
#include "hi_3g_intf.h"

static HI_U32 s32CardInit    = 0;
static HI_U32 s32CardScanned = 0;

static g3_card *g3_card_info = NULL;

/******************************************************************************/
HI_S32 HI_3G_ScanCard(HI_3G_CARD_S *pstCard, HI_S32 s32MaxNum, HI_S32 *pCardNum)
{
    HI_S32 s32Ret;
    g3_card *pstBuf;
    HI_S32 s32ix;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (pCardNum == NULL) {
        HI_ERR_3G("pCardNum is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32MaxNum == 0) {
        HI_ERR_3G("max card number is 0!\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32MaxNum > MAX_CARD_NUM) {
        HI_ERR_3G("max card number too big(%d), "
              "set max card number to %d!\n",
              s32MaxNum, MAX_CARD_NUM);
        s32MaxNum = MAX_CARD_NUM;
    }

    if (!g3_card_info) {
        g3_card_info = malloc(sizeof(g3_card) * MAX_CARD_NUM);
        if (!g3_card_info) {
            HI_ERR_3G("malloc failed\n");
            return HI_3G_CARD_ERR_NOMEMORY;
        }
    }
    memset(g3_card_info, 0, sizeof(g3_card) * MAX_CARD_NUM);
    g3_card_info->etype = pstCard->etype;

    pstBuf = g3_card_info;
    s32Ret = g3_card_scan(g3_card_info, s32MaxNum, pCardNum, pstCard->aport);
    if (s32Ret != 0) {
        HI_ERR_3G("HI_3G_ScanCard(): scan card failed, ret = %d!\n", s32Ret);
        return s32Ret;
    }

    for (s32ix = 0; s32ix < *pCardNum; s32ix++, pstBuf++, pstCard++) {
        snprintf(pstCard->avid,
            sizeof(pstCard->avid),
            "%s", pstBuf->vendor_id);
        snprintf(pstCard->apid,
            sizeof(pstCard->apid),
            "%s", pstBuf->product_id);
        snprintf(pstCard->adevice,
            sizeof(pstCard->adevice),
            "%s", g3_card_info->device);
        snprintf(pstCard->apcui,
            sizeof(pstCard->apcui),
            "%s", g3_card_info->pcui);
    }

    s32CardScanned = 1;
    HI_ERR_3G("scan card success, set s32CardScanned=1 !\n");
    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_InitCard(HI_3G_CARD_S *pstCard)
{

    HI_S32 s32Ret;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 1) {
        HI_ERR_3G("card is already initiated!\n");
        return HI_3G_CARD_SUCCESS;
    }

    if (s32CardScanned == 0) {
        HI_ERR_3G("card has not been scanned! "
              "Please scan card first!\n");
        return HI_3G_CARD_ERR_ENOCARD;
    }

    s32Ret = g3_card_init(g3_card_info);
    if (s32Ret != 0) {
        HI_ERR_3G("init pcui failed\n");
        return s32Ret;
    }
    snprintf(pstCard->amanufacturer,
        sizeof(pstCard->amanufacturer),
        "%s", g3_card_info->manufacturer);
    snprintf(pstCard->aproductname,
        sizeof(pstCard->aproductname),
        "%s", g3_card_info->product_name);
    snprintf(pstCard->adevice,
        sizeof(pstCard->adevice),
        "%s", g3_card_info->device);
    snprintf(pstCard->apcui,
        sizeof(pstCard->apcui),
        "%s", g3_card_info->pcui);

    s32CardInit = 1;

    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_DeInitCard(HI_3G_CARD_S *pstCard)
{
    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("WARNING: "
              "3g card has not init yet!\n");
        return HI_3G_CARD_SUCCESS;
    }

    g3_card_deinit(g3_card_info);
    free(g3_card_info);
    g3_card_info = NULL;
    s32CardInit = 0;
    s32CardScanned = 0;
    HI_ERR_3G("HI_3G_DeInitCard() set s32CardScanned = 0\n");
    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/
HI_S32 HI_3G_GetCardStatus(HI_3G_CARD_S *pstCard,
               HI_3G_CARD_STATE_E *pstStatus)
{
    HI_S32 s32Ret;
    g3_card_status_e stStatus;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (pstStatus == NULL) {
        HI_ERR_3G("pstStatus is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    s32Ret = g3_card_get_sim_status(g3_card_info, &stStatus);
    if (s32Ret != 0) {
        HI_ERR_3G("get sim status failed\n");
        return s32Ret;
    }

    s32Ret = g3_card_get_conn_status(g3_card_info, &stStatus);
    if (s32Ret != 0) {
        HI_ERR_3G("get connection status failed\n");
        return s32Ret;
    }

    *pstStatus = (HI_3G_CARD_STATE_E)stStatus;
    pstCard->status = (HI_3G_CARD_STATE_E)stStatus;

    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_GetAllOperators (HI_3G_CARD_S *pstCard,
                  HI_3G_OPERATOR_S *pstOperators,
                  HI_S32 s32MaxOptnum,
                  HI_S32 *pOptnum)
{
    HI_S32 s32Ret;
    HI_S32 s32ix;
    g3_operator *pstOperator;
    g3_operator *pstOperatorOrg;
    HI_3G_OPERATOR_S *pstTmp;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32MaxOptnum == 0) {
        HI_ERR_3G("max operators number is 0\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32MaxOptnum > MAX_OPERATOR_NUM) {
        HI_ERR_3G("max operators number too big(%d), "
              "set max operators number to %d!\n",
              s32MaxOptnum, MAX_OPERATOR_NUM);
        s32MaxOptnum =  MAX_OPERATOR_NUM;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    pstOperator = (g3_operator *)(malloc(sizeof(g3_operator) * s32MaxOptnum));
    if (pstOperator == NULL) {
        HI_ERR_3G("malloc failed\n");
        return HI_3G_CARD_ERR_NOMEMORY;
    }
    pstOperatorOrg = pstOperator;
    s32Ret = g3_card_get_all_operators(g3_card_info,
                       pstOperator,
                       s32MaxOptnum,
                       pOptnum);
    if (s32Ret != 0) {
        free(pstOperator);
        pstOperator = NULL;
        return s32Ret;
    }

    for (s32ix = 0, pstTmp = pstOperators; s32ix < *pOptnum;
         s32ix++, pstOperator++, pstTmp++) {
        snprintf(pstTmp->anetmode,
            sizeof(pstTmp->anetmode),
            "%s", pstOperator->netstat);
        snprintf(pstTmp->alongoperator,
            sizeof(pstTmp->alongoperator),
            "%s", pstOperator->long_operator);
        snprintf(pstTmp->ashortoperator,
            sizeof(pstTmp->ashortoperator),
            "%s", pstOperator->short_operator);
        snprintf(pstTmp->anumericoperator,
            sizeof(pstTmp->anumericoperator),
            "%s", pstOperator->numeric_operator);
        snprintf(pstTmp->arat,
            sizeof(pstTmp->arat),
            "%s", pstOperator->rat);
    }

    free(pstOperatorOrg);
    pstOperatorOrg = NULL;
    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_SearchOperatorModeAcqorder(HI_3G_CARD_S *pstCard,
                    HI_3G_CARD_MODE_E stMode,
                    HI_3G_CARD_ACQORDER_E stOpFormat)
{
    HI_S32 s32Ret;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if ((stMode >= HI_3G_CARD_MODE_BOTT)
       || ((stMode < HI_3G_CARD_MODE_GSM_ONLY)
       && (stMode != HI_3G_CARD_MODE_AUTO))) {
        HI_ERR_3G("operator mode(%d) invalid!\n", stMode);
        return HI_3G_CARD_ERR_INVAL;
    }

    if (stOpFormat >= HI_3G_CARD_ACQORDER_BOTT) {
        HI_ERR_3G("operator format(%d) invalid!\n", stOpFormat);
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    s32Ret = g3_card_set_mode_acqorder(g3_card_info,
                      (g3_card_mode_e)stMode,
                      (g3_card_acqorder_e)stOpFormat);
    if (s32Ret != HI_3G_CARD_SUCCESS)
        return s32Ret;

    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_RegisterOperator(HI_3G_CARD_S *pstCard,
                  HI_3G_CARD_COPS_MODE_E stMode,
                  HI_3G_CARD_COPS_OP_FORMAT_E stOpFormat,
                  HI_CHAR *oper)
{
    HI_S32 s32Ret;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (oper == NULL) {
        HI_ERR_3G("operator is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (stMode >= HI_3G_CARD_COPS_MODE_BOTT) {
        HI_ERR_3G("operator mode(%d) invalid!\n", stMode);
        return HI_3G_CARD_ERR_INVAL;
    }

    if (stMode != HI_3G_CARD_COPS_MODE_AUTO) {
        if (stOpFormat >= HI_3G_CARD_COPS_OP_FORMAT_BOTT) {
            HI_ERR_3G("operator format(%d) invalid!\n", stOpFormat);
            return HI_3G_CARD_ERR_INVAL;
        }
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    s32Ret = g3_card_register_operator(g3_card_info,
                      (g3_card_cops_mode_e)stMode,
                      (g3_card_cops_op_format_e)stOpFormat,
                      oper);

    if (s32Ret != HI_3G_CARD_SUCCESS)
        return s32Ret;

    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_GetApn(HI_3G_CARD_S *pstCard, HI_3G_PDP_S *stPdp)
{
    HI_S32 s32Ret;
    g3_pdp *pdp;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (stPdp == NULL) {
        HI_ERR_3G("stPdp is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    pdp = (g3_pdp *)malloc(sizeof(g3_pdp));
    if (pdp == NULL) {
        HI_ERR_3G("malloc failed\n");
        return HI_3G_CARD_ERR_NOMEMORY;
    }

    s32Ret = g3_card_get_current_pdp(g3_card_info, pdp);
    if (s32Ret != HI_3G_CARD_SUCCESS) {
        free(pdp);
        pdp = NULL;
        return s32Ret;
    }

    snprintf(stPdp->acid,
        sizeof(stPdp->acid),
        "%s", pdp->cid);
    snprintf(stPdp->apdptype,
        sizeof(stPdp->apdptype),
        "%s", pdp->pdp_type);
    snprintf(stPdp->aapn,
        sizeof(stPdp->aapn),
        "%s", pdp->apn);
    snprintf(stPdp->apdpipaddr,
        sizeof(stPdp->apdpipaddr),
        "%s", pdp->pdp_ipaddr);
    snprintf(stPdp->adcomp,
        sizeof(stPdp->adcomp),
        "%s", pdp->d_comp);
    snprintf(stPdp->ahcomp,
        sizeof(stPdp->ahcomp),
        "%s", pdp->h_comp);

    free(pdp);
    pdp = NULL;
    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_SetApn(HI_3G_CARD_S *pstCard, HI_CHAR *apn)
{
    HI_S32 s32Ret;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (apn == NULL) {
        HI_ERR_3G("apn is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    s32Ret = g3_card_set_pdp(g3_card_info, apn);
    if (s32Ret != HI_3G_CARD_SUCCESS)
        return s32Ret;

    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_GetCurrentOperator(HI_3G_CARD_S *pstCard,
                HI_3G_CARD_OPERATOR_S *pstOperators)
{
    HI_S32 s32Ret;
    g3_current_operator *pstCurrentOperator;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    pstCurrentOperator =
        (g3_current_operator *)malloc(sizeof(g3_current_operator));
    if (pstCurrentOperator == NULL) {
        HI_ERR_3G("malloc failed\n");
        return HI_3G_CARD_ERR_NOMEMORY;
    }

    s32Ret = g3_card_get_current_operator(g3_card_info, pstCurrentOperator);
    if (s32Ret != 0) {
        HI_ERR_3G("get current operator info failed\n");
        free(pstCurrentOperator);
        pstCurrentOperator = NULL;
        return s32Ret;
    }

    snprintf(pstOperators->anetmode,
        sizeof(pstOperators->anetmode),
        "%s", pstCurrentOperator->netmode);
    snprintf(pstOperators->aformatoperator,
        sizeof(pstOperators->aformatoperator),
        "%s", pstCurrentOperator->operatorformat);
    snprintf(pstOperators->aoperatorinfo,
        sizeof(pstOperators->aoperatorinfo),
        "%s", pstCurrentOperator->operatorinfo);
    snprintf(pstOperators->arat,
        sizeof(pstOperators->arat),
        "%s", pstCurrentOperator->rat);

    free(pstCurrentOperator);
    pstCurrentOperator = NULL;
    return HI_3G_CARD_SUCCESS;

}
/******************************************************************************/

HI_S32 HI_3G_GetDataFlow (HI_3G_CARD_S *pstCard,
              HI_3G_CARD_FLOW_S *pstDataFlow)
{
    HI_S32 s32Ret;
    g3_card_flow *pstFlow;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    pstFlow = (g3_card_flow *)malloc(sizeof(g3_card_flow));
    if (pstFlow == NULL) {
        HI_ERR_3G("malloc failed\n");
        return HI_3G_CARD_ERR_NOMEMORY;
    }

    s32Ret = g3_card_get_dataflow(g3_card_info, pstFlow);
    if (s32Ret != 0) {
        HI_ERR_3G("get data flow failed\n");
        goto out;
    }
    pstDataFlow->lastdstime = pstFlow->lastdstime;
    pstDataFlow->lasttxflow = pstFlow->lasttxflow;
    pstDataFlow->lastrxflow = pstFlow->lastrxflow;
    pstDataFlow->totaldstime = pstFlow->totaldstime;
    pstDataFlow->totaltxflow = pstFlow->totaltxflow;
    pstDataFlow->totalrxflow = pstFlow->totalrxflow;

    return HI_3G_CARD_SUCCESS;
out:
    free(pstFlow);
    pstFlow = NULL;

    return s32Ret;
}

/******************************************************************************/

HI_S32 HI_3G_GetQuality (HI_3G_CARD_S *pstCard, HI_S32 *pRssi, HI_S32 *pBer)
{
    HI_S32 s32Ret;
    HI_S32 s32Rssi = 0;
    HI_S32 s32ber = 0;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    s32Ret = g3_card_get_quality(g3_card_info, &s32Rssi, &s32ber);
    if (s32Ret != 0) {
        HI_ERR_3G("get data flow failed\n");
        return s32Ret;
    }
    *pRssi = s32Rssi;
    *pBer = s32ber;
    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_ConnectCard(HI_3G_CARD_S *pstCard,
               HI_CHAR *pUserName,
               HI_CHAR *pPassword,
               HI_CHAR *pTeleNum,
               HI_S32 s32ArgNum,
               HI_CHAR *pArgv[])
{
    HI_S32 s32Ret;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    s32Ret = g3_card_connect(g3_card_info, pUserName, pPassword,
                pTeleNum, s32ArgNum, pArgv);
    if (s32Ret != 0) {
        HI_ERR_3G("card connect failed\n");
        return s32Ret;
    }
    snprintf(pstCard->aunit,
        sizeof(pstCard->aunit),
        "%s", g3_card_info->inf_unit);

    return HI_3G_CARD_SUCCESS;
}

/******************************************************************************/

HI_S32 HI_3G_DisConnectCard(HI_3G_CARD_S *pstCard)
{
    HI_S32 s32Ret;

    if (pstCard == NULL) {
        HI_ERR_3G("pstCard is NULL\n");
        return HI_3G_CARD_ERR_INVAL;
    }

    if (s32CardInit == 0) {
        HI_ERR_3G("HI_3G_DisConnectCard: card has not been initialized, "
              "please init card first!\n");
        return HI_3G_CARD_ERR_NOINIT;
    }

    s32Ret = g3_card_disconnect(g3_card_info);
    if (s32Ret != 0) {
        HI_ERR_3G("stop card failed\n");
        return s32Ret;
    }

    return HI_3G_CARD_SUCCESS;
}
