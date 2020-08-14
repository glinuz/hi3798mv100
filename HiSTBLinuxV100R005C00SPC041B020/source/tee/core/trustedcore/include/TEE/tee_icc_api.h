#ifndef __TEE_ICC_API_H
#define __TEE_ICC_API_H
#include "tee_internal_api.h"
#include "tee_ext_api.h"
#include "tee_log.h"

#ifndef UNUSED
#define UNUSED(a) (a=a)
#endif

#define BSP_ERR_ICC_BASE              (int)(0x80000000 | 0x10180000)
#define BSP_ERR_ICC_CCORE_RESETTING   (BSP_ERR_ICC_BASE + 0x12)      /* modem发生复位 */
#define ICC_INVALID_NO_FIFO_SPACE     (BSP_ERR_ICC_BASE + 0x13)      /* 通道满 */
#define MODEM_CALL_IS_STUB            (BSP_ERR_ICC_BASE + 0xff)      /* 为桩接口，本平台不支持安全通道 */
#define ICC_CHANNEL_IS_OPEN           (0x80000000 + 0x0b)            /* 该通道已经被打开 */
/**
 * 调用安全ICC函数支持的命令ID
 */
typedef enum FUNC_MODEM_CMD_ID {
    /*0x600-0x800:TA-->驱动*/
    FUNC_ICC_CA_CFG_GET = 0x600,       /* 获取CA session初始化配置参数 */
    FUNC_MDRV_ICC_OPEN,                /* 打开一个安全ICC通道*/
    FUNC_MDRV_ICC_CLOSE,               /* 关闭一个安全ICC通道 */
    FUNC_MDRV_ICC_WRITE,               /* 通过安全ICC通道发送数据 */
    FUNC_MDRV_ICC_READ,                /* 通过安全ICC通道阻塞读取数据 */
    FUNC_MDRV_ICC_TRYREAD,             /* 通过安全ICC通道尝试读取数据 */

}FUNC_CMD_ID;

/* 通道id分配 */
enum ICC_S_CHN_ID
{
    ICC_CHN_SEC_IFC  = 17,        /* 共享IPC无任务直接回调处理函数，安全OS与ccore共享物理通道 */
    ICC_CHN_SEC_VSIM = 18,        /* 私有IPC无任务直接回调处理函数，天际通安全OS与ccore物理通道 */

    ICC_CHN_ID_MAX
};

/* 说明: 接收回调函数ID,子通道必须放置在, "通道名称_xxx=0和通道名称_RECV_FUNC_ID_MAX之间 */
enum ICC_S_RECV_FUNC_ID{
    IFC_RECV_FUNC_MODULE_VERIFY = 0,

    /* 若要在物理通道上定义子通道,请在该注释行之前定义 */
    IFC_RECV_FUNC_ID_MAX,

    VSIM_RECV_FUNC_SUB0 = 0,

    /* 若要在物理通道上定义子通道,请在该注释行之前定义 */
    VSIM_RECV_FUNC_ID_MAX  

};

typedef struct tag_CA_ICC_ATTR_S {
    TEE_UUID     svc_uuid;        /* TA的UUID */
    uint8_t      svc_name[32];    /* TA的名称 */
    uint32_t     session_cmd_id;  /* CMD_ID */
    uint32_t     task_priority;   /* 在AP起session挂起任务的优先级 */
 }CA_ICC_ATTR_S;

/*****************************************************************************
* 函 数 名  : mdrv_icc_open
* 功能描述  : 打开icc通道接口
* 输入参数  : unsigned int channel_id  channel_id = 通道id << 16 | function_id, 使用约束:
                1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID
                2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
              *pCaAttr如果赋NULL,表明用户自己在CA起session和读线程，
                      如果有赋值驱动将在CA起session和读线程，由于安全OS线程机制问题，该功能暂不打开
* 输出参数  : 无
* 返 回 值  : 正确: 0;
              错误: 错误码;
              特殊返回值:MODEM_CALL_IS_STUB:open为桩接口，代表本平台不支持安全通道
              ICC_CHANNEL_IS_OPEN错误码表示重复open
* 说    明  : 
*****************************************************************************/
static inline int mdrv_icc_open(unsigned int channel_id, CA_ICC_ATTR_S *pCaAttr)
{
    UNUSED(pCaAttr);
    return __bsp_modem_call(FUNC_MDRV_ICC_OPEN, channel_id, NULL, 0);
}

/*****************************************************************************
* 函 数 名  : mdrv_icc_close
* 功能描述  : 关闭icc通道接口
* 输入参数  : unsigned int channel_id  channel_id = 通道id << 16 | function_id, 使用约束:
                1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID
                2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
* 输出参数  : 无
* 返 回 值  : 正确: 0;
              错误: 错误码;
* 说    明  : 
*****************************************************************************/
static inline int mdrv_icc_close(unsigned int channel_id)
{
    return __bsp_modem_call(FUNC_MDRV_ICC_CLOSE, channel_id, NULL, 0);
}

/*****************************************************************************
* 函 数 名  : mdrv_icc_write
* 功能描述  : icc异步发送数据接口
* 输入参数  : unsigned int cpuid       数据要发送的cpu的编号，使用约束: 要使用enum CPU_ID定义成员的枚举值
*             unsigned int channel_id  channel_id = 通道id << 16 | function_id, 使用约束:
                               1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID的枚举值
                               2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
*             u8 *buffer      要发送的数据buffer的指针
*             unsigned int buf_len    要发送的数据的大小, 使用约束: 最大值 < fifo_size - ICC_CHANNEL_PACKET_LEN
* 输出参数  : 无
* 返 回 值  : 正确: 实际写入fifo的大小;
              错误: 负值;
              特殊返回值: BSP_ERR_ICC_CCORE_RESETTING 表明modem正在复位，此阶段安全通道将关闭，直到modem重启恢复。
                          ICC_INVALID_NO_FIFO_SPACE 表明通道已满。
* 说    明  : 发送端必须有反压（通道数据发送缓慢）处理机制。

*****************************************************************************/
static inline int mdrv_icc_write(unsigned int channel_id, unsigned char *buffer, unsigned int buf_len)
{
    return __bsp_modem_call(FUNC_MDRV_ICC_WRITE, channel_id, buffer, buf_len);
}

/*****************************************************************************
* 函 数 名  : mdrv_icc_read
* 功能描述  : icc从fifo读取数据接口
* 输入参数  : unsigned int channel_id  channel_id = 通道id << 16 | function_id, 使用约束:
                1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID
                2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
*             unsigned char *buffer    要读取fifo的数据buffer的指针
*             unsigned int  buf_len    数据buffer的大小
* 输出参数  : 无
* 返 回 值  : 正确: 实际从通道读取数据大小;
              0:    通道内无数据；
              错误: 负值
* 说    明  : 1) 如果在open时传入了uuid、sessionid、task_priority等，mdrv_icc_read必须放在open时驱动层代为起的session中；
              2) 如果在open时传入的*pCaAttr为NULL，mdrv_icc_read必须放在使用者自己起的session中。
              由于安全OS线程机制问题，该功能暂不打开,暂使用mdrv_icc_tryread接口替代。
*****************************************************************************/
static inline int mdrv_icc_read(unsigned int channel_id, unsigned char *buffer, unsigned int buf_len)
{
    UNUSED(channel_id);
    UNUSED(buffer);
    UNUSED(buf_len);
    return 0;
    //return __bsp_modem_call(FUNC_MDRV_ICC_READ, channel_id, buffer, buf_len);
}

/*****************************************************************************
* 函 数 名  : mdrv_icc_tryread
* 功能描述  : icc从fifo读取数据接口
* 输入参数  : unsigned int channel_id  channel_id = 通道id << 16 | function_id, 使用约束:
                1) channel_id高16bit为通道标识ID，使用enum ICC_CHN_ID
                2) 低16bit为回调函数标识ID，使用ICC_RECV_FUNC_ID对应通道id的枚举值
*             unsigned char *buffer    要读取fifo的数据buffer的指针
*             unsigned int  buf_len    数据buffer的大小

* 输出参数  : 无
* 返 回 值  : 正确: 实际从通道读取数据大小;
              0:    通道内无数据；
              错误: 负值
* 说    明  :
*****************************************************************************/
static inline int mdrv_icc_tryread(unsigned int channel_id, unsigned char *buffer, unsigned int buf_len)
{
    return __bsp_modem_call(FUNC_MDRV_ICC_TRYREAD, channel_id, buffer, buf_len);
}
#endif
