#ifndef __DRV_HDMI_TEE_H__
#define __DRV_HDMI_TEE_H__

#include "drv_hdmi_common.h"
#include "teek_client_api.h"    // tee

#define HDMI_TEE_DATA_SIZE 1024
typedef enum {
    HI_HDMI_HDCP14_INIT,
    HI_HDMI_HDCP14_BKSV_SET,
    HI_HDMI_HDCP14_RPT_ENABLE,
    HI_HDMI_HDCP14_R0_VERIFY,
    HI_HDMI_HDCP14_KSVLIST_SET,
    HI_HDMI_HDCP14_VI_VERIFY,
    HI_HDMI_HDCP14_ENC_ENABLE,
    HI_HDMI_HDCP14_EVENT,
    HI_HDMI_HDCP22_INIT,
    HI_HDMI_TEE_STATUS_VIEW,
    HI_HDMI_HDCP_CAPS,
    HI_HDMI_STOP,
    HI_HDMI_TEST_SET_HDCP_STRATEGY,
    HI_HDMI_COMM_BUTT,
} HI_HDMI_COMMID_E;

enum TEE_Result_Value{
    TEE_SUCCESS = 0x0,                          /**< 成功  */
    TEE_ERROR_INVALID_CMD,                      /**< 非法命令 */
    TEE_ERROR_SERVICE_NOT_EXIST,                /**< 服务不存在 */
    TEE_ERROR_SESSION_NOT_EXIST,                /**< 连接不存在 */
    TEE_ERROR_SESSION_MAXIMUM,                  /**< 连接数已满 */
    TEE_ERROR_REGISTER_EXIST_SERVICE,           /**< 注册已经存在的服务 */
    TEE_ERROR_TARGET_DEAD_FATAL,                 /**< Global Task 崩溃  */
    TEE_ERROR_READ_DATA,                        /**< 读取文件错误  */
    TEE_ERROR_WRITE_DATA,                       /**< 写入文件错误  */
    TEE_ERROR_TRUNCATE_OBJECT,                  /**< 截断文件错误  */
    TEE_ERROR_SEEK_DATA,                        /**< 查找文件错误  */
    TEE_ERROR_SYNC_DATA,                        /**< 同步文件错误  */
    TEE_ERROR_RENAME_OBJECT,                    /**< 重命名文件错误  */
    TEE_ERROR_TRUSTED_APP_LOAD_ERROR,           /**< 打开会话时，加载可信应用程序失败*/
#ifdef GP_V1_1_SUPPORTED
    TEE_ERROR_CORRUPT_OBJECT=0xF0100001,/**If the persistent object is corrupt. The object handle is closed.*/
    TEE_ERROR_CORRUPT_OBJECT_2=0xF0100002,/**If the persistent object is stored in a storage area which is currently inaccessible.*/
    TEE_ERROR_STORAGE_NOT_AVAILABLE=0xF0100003,
    TEE_ERROR_STORAGE_NOT_AVAILABLE_2=0xF0100004,
#endif
    TEE_ERROR_GENERIC = 0xFFFF0000,             /**< 通用错误  */
    TEE_ERROR_ACCESS_DENIED = 0xFFFF0001 ,      /**< 权限校验失败  */
    TEE_ERROR_CANCEL = 0xFFFF0002 ,             /**< 操作已取消  */
    TEE_ERROR_ACCESS_CONFLICT = 0xFFFF0003 ,    /**< 并发访问导致冲突  */
    TEE_ERROR_EXCESS_DATA = 0xFFFF0004 ,        /**< 操作包含的数据太多  */
    TEE_ERROR_BAD_FORMAT = 0xFFFF0005 ,         /**< 数据格式不正确  */
    TEE_ERROR_BAD_PARAMETERS = 0xFFFF0006 ,     /**< 参数无效  */
    TEE_ERROR_BAD_STATE = 0xFFFF0007,           /**< 当前状态下的操作无效  */
    TEE_ERROR_ITEM_NOT_FOUND = 0xFFFF0008,      /**< 请求的数据未找到  */
    TEE_ERROR_NOT_IMPLEMENTED = 0xFFFF0009,     /**< 请求的操作存在但暂未实现  */
    TEE_ERROR_NOT_SUPPORTED = 0xFFFF000A,       /**< 请求的操作有效但未支持  */
    TEE_ERROR_NO_DATA = 0xFFFF000B,             /**< 数据错误  */
    TEE_ERROR_OUT_OF_MEMORY = 0xFFFF000C,       /**< 系统没有可用资源  */
    TEE_ERROR_BUSY = 0xFFFF000D,                /**< 系统繁忙  */
    TEE_ERROR_COMMUNICATION = 0xFFFF000E,       /**< 与第三方通信失败  */
    TEE_ERROR_SECURITY = 0xFFFF000F,            /**< 检测到安全错误  */
    TEE_ERROR_SHORT_BUFFER = 0xFFFF0010,        /**< 内存输入长度小于输出长度  */
    TEE_PENDING = 0xFFFF2000,                   /**< 可信服务处于等待状态(异步调用) */
    TEE_PENDING2 = 0xFFFF2001,                  /**< 可信服务处于等待状态2(命令未完成) */
    TEE_PENDING3 = 0xFFFF2002,
    TEE_ERROR_TIMEOUT = 0xFFFF3001,             /**< 请求超时 */
    TEE_ERROR_OVERFLOW = 0xFFFF300f,            /**< 计算溢出 */
    TEE_ERROR_TARGET_DEAD = 0xFFFF3024,          /**< Trusted Application崩溃  */
    TEE_ERROR_STORAGE_NO_SPACE = 0xFFFF3041,    /**< 没有足够的Flash空间来存储文件 */
    TEE_ERROR_FILE_CORRUPT = 0xFFFF3042,        /**< 存储的文件被损坏，完整性校验失败 */
    TEE_ERROR_MAC_INVALID = 0xFFFF3071,         /**< MAC值校验错误  */
    TEE_ERROR_SIGNATURE_INVALID = 0xFFFF3072,   /**< 校验失败 */
    TEE_ERROR_TIME_NOT_SET = 0xFFFF5000,        /**< 时间未设置 */
    TEE_ERROR_TIME_NEEDS_RESET = 0xFFFF5001,    /**< 时间需要重置 */
    TEE_FAIL = 0xFFFF5002,    /**< 时间需要重置 */
    TEE_ERROR_TIMER = 0xFFFF6000,
    TEE_ERROR_TIMER_CREATE_FAILED,
    TEE_ERROR_TIMER_DESTORY_FAILED,
    TEE_ERROR_TIMER_NOT_FOUND,
    TEE_ERROR_RPMB_BASE = 0xFFFF7000,    /**< RPMB安全存储错误码基址 */
    TEE_ERROR_RPMB_GENERIC = 0xFFFF7001,    /**< RPMB控制器通用错误 */
    TEE_ERROR_RPMB_MAC_FAIL,    /**< RPMB控制器MAC校验错误 */
    TEE_ERROR_RPMB_COUNTER_FAIL,    /**< RPMB控制器计数器校验错误 */
    TEE_ERROR_RPMB_ADDR_FAIL,    /**< RPMB控制器地址校验错误 */
    TEE_ERROR_RPMB_WRITE_FAIL,    /**< RPMB控制器写错误 */
    TEE_ERROR_RPMB_READ_FAIL,    /**< RPMB控制器读错误 */
    TEE_ERROR_RPMB_KEY_NOT_PROGRAM,    /**< RPMB Key未写入 */
    TEE_ERROR_RPMB_RESP_UNEXPECT_MSGTYPE = 0xFFFF7100,    /**< RPMB应答数据的消息类型校验错误 */
    TEE_ERROR_RPMB_RESP_UNEXPECT_BLKCNT,    /**< RPMB应答数据的数据块校验错误 */
    TEE_ERROR_RPMB_RESP_UNEXPECT_BLKIDX,    /**< RPMB应答数据的数据地址校验错误 */
    TEE_ERROR_RPMB_RESP_UNEXPECT_WRCNT,    /**< RPMB应答数据的计数器校验错误 */
    TEE_ERROR_RPMB_RESP_UNEXPECT_NONCE,    /**< RPMB应答数据的随机数校验错误 */
    TEE_ERROR_RPMB_RESP_UNEXPECT_MAC,    /**< RPMB应答数据的MAC校验错误 */
    TEE_ERROR_RPMB_FILE_NOT_FOUND,    /**< RPMB安全存储文件不存在 */
    TEE_ERROR_RPMB_NOSPC,    /**< RPMB安全存储磁盘空间不足 */
    TEE_ERROR_TUI_IN_USE = 0xFFFF7110,
    TEE_ERROR_TUI_SWITCH_CHANNAL,
    TEE_ERROR_TUI_CFG_DRIVER,
    TEE_ERROR_TUI_INVALID_EVENT,
    TEE_ERROR_TUI_POLL_EVENT,
    TEE_ERROR_TUI_CANCELED,
    TEE_FAIL2
};

typedef struct hiHDMI_TEE_INFO_S
{
    HI_BOOL         bTeeOpen;
    TEEC_Context    stTeecContext;
    TEEC_Operation  stTeecOperation;
    TEEC_Session    stTeecSession;
    HI_U8           au8TeeDataBuf[HDMI_TEE_DATA_SIZE];
    HI_U32          u32DataSize;
}HDMI_TEE_INFO_S;

typedef struct hiHDMI_TEE_HDCP_CAPS_S
{
    HI_BOOL         bHdcp14Support;
    HI_BOOL         bHdcp22Support;
}HDMI_TEE_HDCP_CAPS_S;

typedef struct hiHDMI_TEE_PARAM_S
{
    HI_HDMI_COMMID_E        enTeeCmd;
    HDMI_TEE_HDCP_CAPS_S    seHdcpCaps;
    HI_U32                  u32TestParam;
    HI_U32                  *pvData;
    HI_U32                  u32Size;
}HDMI_TEE_PARAM_S;

extern TEEC_UUID HdmiTaskUUID;
HI_S32 DRV_HDMI_TeeOpen(HDMI_DEVICE_ID_E enHdmiId);
HI_S32 DRV_HDMI_TeeCmdSend(HDMI_DEVICE_ID_E enHdmiId, HDMI_TEE_PARAM_S *pstParam);
HI_S32 DRV_HDMI_TeeClose(HDMI_DEVICE_ID_E enHdmiId);


#endif

