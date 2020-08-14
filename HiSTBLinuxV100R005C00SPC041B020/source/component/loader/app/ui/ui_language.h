/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_language.h
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

#ifndef __UI_LANGUAGE_H__
#define __UI_LANGUAGE_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      UI_LANGUAGE */
/** @{ */  /** <!-- [UI_LANGUAGE] */


#ifdef HI_APPLOADER_OSD_LANG_CN
//0-4
#define UI_LAN_MAN_UPGRD            "手动升级"
#define UI_LAN_PROMPT_UPGRD_PWD1    "请输入系统升级权限密码 "
#define UI_LAN_PROMPT_UPGRD_PWD2    ""
#define UI_LAN_INPUT_PWD            "输入密码"
#define UI_LAN_PROMPT_NOT_OP1       ""

//5-9
#define UI_LAN_PROMPT_NOT_OP2       "请出现此界面的用户不要进行任何操作并"
#define UI_LAN_PROMPT_NOT_OP3       "及时与运营商联系客服电话:3928888"
#define UI_LAN_BUTTON_CONFIRM       "确定  "
#define UI_LAN_BUTTON_CANCLE        "取消  "
#define UI_LAN_PROMPT_OPRTN         "操作提示:按退出键继续启动应用"

//10-14
#define UI_LAN_TITLE_FREQ           " 下载频点(MHz)"
#define UI_LAN_TITLE_SYM_RATE       " 符号率(KS/s)"
#define UI_LAN_TITLE_MODULATE       " 调制方式:"
#define UI_LAN_TITLE_PID            " 升级PID"
#define UI_LAN_TITLE_AUTO           " 主动下载"

//15-19
#define UI_LAN_BUTTON_START         "开始"
#define UI_LAN_TRANS_YES            "是"
#define UI_LAN_TRANS_NO             "否"
#define UI_LAN_TRANS_EXIT           "退出"
#define UI_LAN_PROMPT_POWER1        "升级过程中请勿断电，请稍候..."

//20-24
#define UI_LAN_PROMPT_POWER2        ""
#define UI_LAN_PROMPT_SEARCH        "正在搜索数据  "
#define UI_LAN_NT_CUR_PRGRSS        "当前进度"
#define UI_LAN_NT_TTL_PRGRSS        "总进度"
#define UI_LAN_PT_UPGRADING         "升级过程中请勿断电,请稍候..."

//25-29
#define UI_LAN_FAIL_SIG_EXPT        "信号异常"
#define UI_LAN_FAIL_FACTORY         "厂商标志不匹配"
#define UI_LAN_FAIL_HARD_VER        "硬件版本不匹配"
#define UI_LAN_FAIL_SOFT_VER        "软件版本不匹配"
#define UI_LAN_FAIL_SN_DISMATCH     "系列号不匹配"

//30-34
#define UI_LAN_FAIL_FILE_RNG        "文件范围出错"
#define UI_LAN_FAIL_CRC             "CRC出错"
#define UI_LAN_FAIL_DOWNLOAD_TIMEOUT "下载数据超时"
#define UI_LAN_FAIL_DOWNLOAD        "下载数据出错"
#define UI_LAN_FAIL_DATA            "数据无效"

//35-39
#define UI_LAN_FAIL_FREQUENCY       "频点设置错误！"
#define UI_LAN_FAIL_SYMBOL          "Symbol rate设置错误！"
#define UI_LAN_FAIL_PID             "PID设置错误！"
#define UI_LAN_FAIL_MODUL           "modulation设置错误！"
#define UI_LAN_PT_DOWNLOADING       "正在下载数据  "

//40-44
#define UI_LAN_PT_WRITING           "正在更新数据  "
#define UI_LAN_PT_SUCC              "升级成功"
#define UI_LAN_IP_NOT_FIND_FILE     "没有找到升级文件！"
#define UI_LAN_SET_IP_ERR           "配置IP失败"
#define UI_LAN_FAIL_NET_EXPT        "网络异常"

//45-49
#define UI_LAN_FAIL_NET_TIMEOUT     "网络超时"
#define UI_LAN_FAIL_NO_RESP         "连接服务器失败"
#define UI_LAN_FAIL_FILE_LEN        "下载失败:文件长度不匹配"
#define UI_LAN_DWNLD_WTING          "正在下载数据中请勿断电，请稍候..."
#define UI_LAN_FAIL_UPGRD           " 警告！"

//50-54
#define UI_LAN_DWNLD_PROGRESS       "下载进度"
#define UI_LAN_BURN_PROGRESS        "烧写进度"
#define UI_LAN_UPGRADING            "正在升级"
#define UI_LAN_NOT_FIND_USB_FILE    "没有找到USB或升级文件"
#define UI_LAN_REBOOT               "机顶盒正在重启!"

//55-
#define UI_LAN_WARNING              "警告"
#define UI_LAN_NOTICE               "提示"

#define UI_LAN_FACTORY_NOTICE       "正在恢复出厂设置, 请稍候..."
#define UI_LAN_FACTORY_SUCCESS      "恢复出厂设置成功"
#define UI_LAN_FACTORY_FAILED       "恢复出厂设置失败"

#else
//0-4
#define UI_LAN_MAN_UPGRD            "Manual Upgrade"
#define UI_LAN_PROMPT_UPGRD_PWD1    "Enter password for obtaining"
#define UI_LAN_PROMPT_UPGRD_PWD2    "system upgrade permission."
#define UI_LAN_INPUT_PWD            "Password: "
#define UI_LAN_PROMPT_NOT_OP1       "Do not perform any operation when"

//5-9
#define UI_LAN_PROMPT_NOT_OP2       "this page appears. Contact the"
#define UI_LAN_PROMPT_NOT_OP3       "operator by calling 3928888."
#define UI_LAN_BUTTON_CONFIRM       "Confirm"
#define UI_LAN_BUTTON_CANCLE        " Cancel "
#define UI_LAN_PROMPT_OPRTN         "Tip: Press BACK to restart."

//10-14
#define UI_LAN_TITLE_FREQ           "Frequency (MHz)"
#define UI_LAN_TITLE_SYM_RATE       "Symbol rate (KS/s)"
#define UI_LAN_TITLE_MODULATE       "Modulation"
#define UI_LAN_TITLE_PID            "PID"
#define UI_LAN_TITLE_AUTO           "Auto download"

//15-19
#define UI_LAN_BUTTON_START         "Start"
#define UI_LAN_TRANS_YES            "Yes"
#define UI_LAN_TRANS_NO             "No"
#define UI_LAN_TRANS_EXIT           "Exit"
#define UI_LAN_PROMPT_POWER1        "Please do not power off !"

//20-24
#define UI_LAN_PROMPT_POWER2        "during upgrade. Waiting..."
#define UI_LAN_PROMPT_SEARCH        "Searching"
#define UI_LAN_NT_CUR_PRGRSS        "Current progress"
#define UI_LAN_NT_TTL_PRGRSS        "Total progress"
#define UI_LAN_PT_UPGRADING         "Do not power off. Waiting..."

//25-29
#define UI_LAN_FAIL_SIG_EXPT        "Signal exception."
#define UI_LAN_FAIL_FACTORY         "Invalid factory ID."
#define UI_LAN_FAIL_HARD_VER        "Invalid hardware ver."
#define UI_LAN_FAIL_SOFT_VER        "Invalid software ver."
#define UI_LAN_FAIL_SN_DISMATCH     "Invalid serial num."

//30-34
#define UI_LAN_FAIL_FILE_RNG        "File is too large."
#define UI_LAN_FAIL_CRC             "CRC error."
#define UI_LAN_FAIL_DOWNLOAD_TIMEOUT "Download data timeout."
#define UI_LAN_FAIL_DOWNLOAD        "Error when downloading."
#define UI_LAN_FAIL_DATA            "Invalid data."

//35-39
#define UI_LAN_FAIL_FREQUENCY       "Invalid frequency."
#define UI_LAN_FAIL_SYMBOL          "Invalid symbol rate."
#define UI_LAN_FAIL_PID             "Invalid PID."
#define UI_LAN_FAIL_MODUL           "Invalid modulation."
#define UI_LAN_PT_DOWNLOADING       "Downloading"

//40-44
#define UI_LAN_PT_WRITING           "Writing to flash"
#define UI_LAN_PT_SUCC              "Upgrade success !"
#define UI_LAN_IP_NOT_FIND_FILE     "Can't find upgrade file on server!"
#define UI_LAN_SET_IP_ERR           "Config IP Failed."
#define UI_LAN_FAIL_NET_EXPT        "Network exception."

//45-49
#define UI_LAN_FAIL_NET_TIMEOUT     "Network timeout."
#define UI_LAN_FAIL_NO_RESP         "Connect Server failed."
#define UI_LAN_FAIL_FILE_LEN        "Invalid length of file."
#define UI_LAN_DWNLD_WTING          "Downloading data, Waiting..."
#define UI_LAN_FAIL_UPGRD           "Warning!"

//50-54
#define UI_LAN_DWNLD_PROGRESS       "Download progress"
#define UI_LAN_BURN_PROGRESS        "Burn progress"
#define UI_LAN_UPGRADING            "Upgrading"
#define UI_LAN_NOT_FIND_USB_FILE    "can not find upgrade file"
#define UI_LAN_REBOOT               "Rebooting..."

//55-
#define UI_LAN_WARNING              "Warning"
#define UI_LAN_NOTICE               "Notice"

#define UI_LAN_FACTORY_NOTICE       "Factory Reset is going on, waiting..."
#define UI_LAN_FACTORY_SUCCESS      "Factory Reset Success"
#define UI_LAN_FACTORY_FAILED       "Factory Reset Failed"

#endif


/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      UI_LANGUAGE */
/** @{ */  /** <!-- [UI_LANGUAGE] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      UI_LANGUAGE */
/** @{ */  /** <!-- [UI_LANGUAGE] */


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /*__UI_LANGUAGE_H__*/
