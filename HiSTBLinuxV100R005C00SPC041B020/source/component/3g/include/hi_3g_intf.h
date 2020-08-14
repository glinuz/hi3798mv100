/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_3g_intf.h
Version       : Initial Draft
Author        : Hisilicon OSDRV group
Created       : 2013-08-18
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

/**
 * \file
 * \brief describle the information about 3g card component. CNcomment:提供3g card组件相关接口、数据结构信息。 CNend
 */

#ifndef __HI_3G_INF_H__
#define __HI_3G_INF_H__
#include "hi_type.h"

#if defined (ANDROID)
#define LOG_TAG "3g_intf"
#include <cutils/log.h>

#define HI_FATAL_3G(fmt...)      SLOGI(fmt)
#define HI_ERR_3G(fmt...)        SLOGI(fmt)//HI_ERR_PRINT(HI_ID_3G, fmt)
#define HI_WARN_3G(fmt...)       SLOGI(fmt)//HI_WARN_PRINT(HI_ID_3G, fmt)
#define HI_INFO_3G(fmt...)       SLOGI(fmt)//HI_INFO_PRINT(HI_ID_3G, fmt)
#define HI_DBG_3G(fmt...)        SLOGI(fmt)
#else
#define HI_FATAL_3G(fmt...)      //SLOGI(fmt)
#define HI_ERR_3G(fmt...)        //SLOGI(fmt)//HI_ERR_PRINT(HI_ID_3G, fmt)
#define HI_WARN_3G(fmt...)       //SLOGI(fmt)//HI_WARN_PRINT(HI_ID_3G, fmt)
#define HI_INFO_3G(fmt...)       //SLOGI(fmt)//HI_INFO_PRINT(HI_ID_3G, fmt)
#define HI_DBG_3G(fmt...)        //SLOGI(fmt)
#endif
/*******************************Structure declaration *****************************/
/** @addtogroup     Hi3G */
/** @{ */ /** <!--  [Hi3G] */

/** Max 3G Card number*/
#define MAX_CARD_NUM                      2
/** Max Operator number*/
#define MAX_OPERATOR_NUM                  20

/** 3g card connection status */
typedef enum HI_3G_CARD_STATE_E
{
    HI_3G_CARD_STATUS_UNAVAILABLE,             /**< 3g card unavalible */
    HI_3G_CARD_STATUS_SIM_PIN,                 /**< require for SIM PIN code */
    HI_3G_CARD_STATUS_SIM_PUK,                 /**< require for SIM PUK code */
    HI_3G_CARD_STATUS_SIM_PIN2,                /**< require for SIM PIN2 code */
    HI_3G_CARD_STATUS_SIM_PUK2,                /**< require for SIM PUK2 code */
    HI_3G_CARD_STATUS_DISCONNECTED,            /**< 3g card ready, but disconnected to 3G net */
    HI_3G_CARD_STATUS_CONNECTED,               /**< 3g card connected to 3G net */
    HI_3G_CARD_STATUS_BOTT,
} HI_3G_CARD_STATE_E;

/** 3g card search and register operators mode, huawei private */
typedef enum HI_3G_CARD_MODE_E {
    HI_3G_CARD_MODE_AUTO = 2,                 /**< auto search and register */
    HI_3G_CARD_MODE_GSM_ONLY = 13,            /**< only search and register to GSM */
    HI_3G_CARD_MODE_UTRAN_ONLY = 14,          /**< only search and register to UTRAN */
    HI_3G_CARD_MODE_TDSCDMA_ONLY = 15,        /**< only search and register to TD-SCDMA */
    HI_3G_CARD_MODE_NO_CHANGE = 16,           /**< not change search and register mode */
    HI_3G_CARD_MODE_BOTT = 17,
} HI_3G_CARD_MODE_E;

/** 3g card register operators priority , huawei private*/
typedef enum HI_3G_CARD_ACQORDER_E {
    HI_3G_CARD_ACQORDER_AUTO = 0,             /**< auto register */
    HI_3G_CARD_ACQORDER_GSM_PRIOR_UTRAN,      /**< register GSM prior to UTRAN */
    HI_3G_CARD_ACQORDER_UTRAN_PRIOR_GSM,      /**< register UTRAN prior to GSM */
    HI_3G_CARD_ACQORDER_NO_CHANGE,            /**< not change register priority */
    HI_3G_CARD_ACQORDER_BOTT,
} HI_3G_CARD_ACQORDER_E;

/** 3g card search and register operators mode */
typedef enum HI_3G_CARD_COPS_MODE_E {
    HI_3G_CARD_COPS_MODE_AUTO,               /**< auto search and register */
    HI_3G_CARD_COPS_MODE_MANUAL,             /**< maual search and register */
    HI_3G_CARD_COPS_MODE_DEREGISTER,         /**< de-register, this function may not be useful for some cards */
    HI_3G_CARD_COPS_MODE_SET_OP_FORMAT,      /**< only set operator format for COPS? */
    HI_3G_CARD_COPS_MODE_MANUAL_AUTO,        /**< if manual mode fail, then change to auto mode */
    HI_3G_CARD_COPS_MODE_BOTT,
} HI_3G_CARD_COPS_MODE_E;

/** operators format */
typedef enum HI_3G_CARD_COPS_OP_FORMAT_E {
    HI_3G_CARD_COPS_OP_FORMAT_LONG_STRING,   /**< operator format:long string */
    HI_3G_CARD_COPS_OP_FORMAT_SHORT_STRING,  /**< operator format:short string */
    HI_3G_CARD_COPS_OP_FORMAT_NUMERIC_NUMBER,/**< operator format:numeric string */
    HI_3G_CARD_COPS_OP_FORMAT_BOTT,
} HI_3G_CARD_COPS_OP_FORMAT_E;

/** 3g card port type */
typedef enum HI_3G_CARD_PORT_TYPE_E
{
    HI_3G_CARD_PORT_TYPE_USB,                 /**< usb interface */
    HI_3G_CARD_PORT_TYPE_UART,                /**< uart interface */
    HI_3G_CARD_PORT_TYPE__BOTT,
} HI_3G_CARD_PORT_TYPE_E;

/** 3g card information */
typedef struct HI_3G_CARD_S
{
    HI_CHAR aunit[4];                        /**< unit number for ppp, if connected, netif name ppp[*] */
    HI_CHAR amanufacturer[2048];             /**< 3g card manufacturer */
    HI_CHAR aproductname[2048];              /**< 3g card product */
    HI_CHAR avid[8];                         /**< 3g card vendor id */
    HI_CHAR apid[8];                         /**< 3g card product id */
    HI_CHAR adevice[16];                     /**< 3g card dialer port, such as /dev/ttyUSB0 */
    HI_CHAR apcui[16];                       /**< 3g card pcui port */
    HI_CHAR aport[16];                       /**< 3g card uart port */
    HI_3G_CARD_PORT_TYPE_E etype;            /**< 3g card port type */
    HI_3G_CARD_STATE_E status;               /**< 3g card status */
} HI_3G_CARD_S;

/** operator information */
typedef struct HI_3G_OPERATOR_S
{
    HI_CHAR anetmode[2];                     /**< operator status: unknown/available/registerd/unavailable */
    HI_CHAR alongoperator[32];               /**< operator in long string format */
    HI_CHAR ashortoperator[16];              /**< operator in short string format */
    HI_CHAR anumericoperator[16];            /**< operator in numeric sting format */
    HI_CHAR arat[2];                         /**< operator rat: GSM/GSM compact(not support)/UTRAN */
} HI_3G_OPERATOR_S;

/** current registerd operator information */
typedef struct HI_3G_CARD_OPERATOR_S
{
    HI_CHAR anetmode[2];                     /**< operator register mode */
    HI_CHAR aformatoperator[2];              /**< operator format */
    HI_CHAR aoperatorinfo[32];               /**< operator info */
    HI_CHAR arat[2];                         /**< operator rat */
} HI_3G_CARD_OPERATOR_S;

/** current registerd operator information */
typedef struct HI_3G_PDP_S {
    HI_CHAR acid[4];                         /**< pdp cid(1-?) */
    HI_CHAR apdptype[8];                     /**< pdp type: IP/PPP/IPV6/IPV4V6*/
    HI_CHAR aapn[32];                        /**< pdp apn */
    HI_CHAR apdpipaddr[32];                  /**< pdp ipaddr */
    HI_CHAR adcomp[2];                       /**< data compression */
    HI_CHAR ahcomp[2];                       /**< header compression */
}HI_3G_PDP_S;

/** 3g card dataflow */
typedef struct HI_3G_CARD_FLOW_S
{
    HI_S32 lastdstime;                      /**< last dataservice time in seconds */
    HI_S32 totaldstime;                     /**< total dataservice time in seconds */
    HI_U64 lasttxflow;                      /**< last tx dataflow in bytes */
    HI_U64 totaltxflow;                     /**< total tx dataflow in bytes */
    HI_U64 lastrxflow;                      /**< last rx dataflow in bytes */
    HI_U64 totalrxflow;                     /**< total tx dataflow in bytes */
} HI_3G_CARD_FLOW_S;

/** success */
#undef  HI_3G_CARD_SUCCESS
#define HI_3G_CARD_SUCCESS             0
/** error code for sysbrk, in this situation, check global error for details */
#undef  HI_3G_CARD_ERR_SYS
#define HI_3G_CARD_ERR_SYS             -1
/** error code for cmd reply error */
#undef  HI_3G_CARD_ERR_REPLY
#define HI_3G_CARD_ERR_REPLY           -2
/** error code for cmd reply timeout */
#undef  HI_3G_CARD_ERR_TIMEOUT
#define HI_3G_CARD_ERR_TIMEOUT         -3
/** error code for no support card when scan */
#undef  HI_3G_CARD_ERR_ENOCARD
#define HI_3G_CARD_ERR_ENOCARD         -4
/** error code for invalid args */
#undef  HI_3G_CARD_ERR_INVAL
#define HI_3G_CARD_ERR_INVAL           -5
/** error code for no memory */
#undef  HI_3G_CARD_ERR_NOMEMORY
#define HI_3G_CARD_ERR_NOMEMORY        -6
/** error code for card not init */
#undef  HI_3G_CARD_ERR_NOINIT
#define HI_3G_CARD_ERR_NOINIT          -7

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      Hi3G */
/** @{ */  /** <!-- [Hi3G] */
/**
\brief: scan for 3g cards
\attention \n
\param[out] pstcard         3g cards scanned
\param[in]  maxnum          max 3g card number can scan
\param[out] pstcardnum      3g card number scanned CNcomment:实际扫描到的3g card数目 CNend
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_ScanCard(HI_3G_CARD_S *pstcard, HI_S32 maxnum, HI_S32 *pstcardnum);
/**
\brief: init 3g card
\attention \n
\param[in] pstcard          3g card scanned
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_InitCard(HI_3G_CARD_S *pstCard);
/**
\brief: init 3g card
\attention \n
\param[in] pstcard          3g card scanned
\retval    error code       error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_DeInitCard(HI_3G_CARD_S *pstCard);
/**
\brief: deinit 3g card
\attention \n
\param[in] pstcard         3g cards scanned
\param[out] pststatus      3g card status
\retval    error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_GetCardStatus(HI_3G_CARD_S *pstcard,
               HI_3G_CARD_STATE_E *pststatus);
/**
\brief: get all operators information
\attention \n
\param[in] pstcard          3g cards scanned
\param[out]  pstOperators   operators number scanned
\param[in]  s32MaxOptnum    max operators number can scan
\param[out] pOptnum         operators number scanned CNcomment:实际扫描到的运营商数目 CNend
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_GetAllOperators (HI_3G_CARD_S *pstCard,
                  HI_3G_OPERATOR_S *pstOperators,
                  HI_S32 s32MaxOptnum,
                  HI_S32 *pOptnum);
/**
\brief: register operator
\attention \n
\param[in]  pstcard         3g cards scanned
\param[in]  stMode          search and register operators mode
\param[in]  stOpFormat      operator format
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/

HI_S32 HI_3G_SearchOperatorModeAcqorder(HI_3G_CARD_S *pstCard,
                    HI_3G_CARD_MODE_E stMode,
                    HI_3G_CARD_ACQORDER_E stOpFormat);
/**
\brief: register operator
\attention \n
\param[in]  pstcard         3g cards scanned
\param[in]  stMode          search and register operators mode
\param[in]  stOpFormat      operator format
\param[in]  oper            operator info
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/

HI_S32 HI_3G_RegisterOperator(HI_3G_CARD_S *pstCard,
                  HI_3G_CARD_COPS_MODE_E stMode,
                  HI_3G_CARD_COPS_OP_FORMAT_E stOpFormat,
                  HI_CHAR *oper);
/**
\brief: get pdp
\attention \n
\param[in]  pstcard         3g cards scanned
\param[out] stPdp           pdp information
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_GetApn(HI_3G_CARD_S *pstCard, HI_3G_PDP_S *stPdp);
/**
\brief: set pdp
\attention \n
\param[in]  pstcard         3g cards scanned
\param[in]  apn             apn information
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_SetApn(HI_3G_CARD_S *pstCard, HI_CHAR *apn);
/**
\brief: get current operator information
\attention \n
\param[in] pstcard          3g cards scanned
\param[out]  pstOperators   current operator registerd
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_GetCurrentOperator(HI_3G_CARD_S *pstCard,
                HI_3G_CARD_OPERATOR_S *pstOperators);
/**
\brief: get 3g card dataflow
\attention \n
\param[in] pstcard          3g cards scanned
\param[out]  pstDataFlow    3g card dataflow
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_GetDataFlow (HI_3G_CARD_S *pstCard,
              HI_3G_CARD_FLOW_S *pstDataFlow);
/**
\brief: get 3g card quality
\attention \n
\param[in] pstcard          3g cards scanned
\param[out]  Rssi           3g card rssi
\param[out]  Ber            3g card ber
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_GetQuality (HI_3G_CARD_S *pstCard, HI_S32 *Rssi, HI_S32 *Ber);
/**
\brief: connect 3g card
\attention \n
\param[in] pstcard          3g cards scanned
\param[in]  username        3g card username for diailer
\param[in]  password        3g card password for diailer
\param[in]  telenum         3g card telephone number for diailer
\param[in]  argnum          3g card optional arg number CNcomment:pppd使用的可选的参数个数 CNend
\param[in]  argv            3g card optional argv CNcomment:pppd使用的可选参数列表 CNend
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_ConnectCard(HI_3G_CARD_S *pstcard,
             HI_CHAR *username,
             HI_CHAR *password,
             HI_CHAR *telenum,
             HI_S32 argnum,
             HI_CHAR *argv[]);
/**
\brief: disconnect 3g card
\attention \n
\param[in] pstcard          3g cards scanned
\retval     error code      error code, when success, return HI_SUCCESS
\see \n
*/
HI_S32 HI_3G_DisConnectCard(HI_3G_CARD_S *pstcard);

/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* __HI_3G_INF_H__ */
