#ifndef __HI_UNF_CI_TYPE_H__
#define __HI_UNF_CI_TYPE_H__

#include "hi_error_mpi.h"
#include "hi_common.h"
#include <linux/string.h>


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      CI*/
/** @{ */  /** <!-- [CI] */

/** support CI Port*/
/** CNcomment:支持的CI Port */
typedef enum hiUNF_CI_PORT_E
{
    HI_UNF_CI_PORT_0,
    HI_UNF_CI_PORT_1,
    HI_UNF_CI_PORT_BUTT
} HI_UNF_CI_PORT_E;

/** support CI PCMCIA Cards(Module) */
/** CNcomment:支持的CI PCMCIA Cards(Module) */
typedef enum hiUNF_CI_PCCD_E
{
    HI_UNF_CI_PCCD_A,
    HI_UNF_CI_PCCD_B,
    HI_UNF_CI_PCCD_BUTT
} HI_UNF_CI_PCCD_E;

/** Module on-off power control */
/** CNcomment:Module开关电源控制 */
typedef enum hiUNF_CI_PCCD_CTRLPOWER_E
{
    HI_UNF_CI_PCCD_CTRLPOWER_ON,
    HI_UNF_CI_PCCD_CTRLPOWER_OFF,
    HI_UNF_CI_PCCD_CTRLPOWER_BUFF
} HI_UNF_CI_PCCD_CTRLPOWER_E;

/** Module ready state:ready/busy */
/** CNcomment:Module就绪状态:ready/busy */
typedef enum hiUNF_CI_PCCD_READY_E
{
    HI_UNF_CI_PCCD_BUSY,
    HI_UNF_CI_PCCD_READY
} HI_UNF_CI_PCCD_READY_E, *HI_UNF_CI_PCCD_READY_E_PTR;

/** Module online state:present/absent */
/** CNcomment:Module在线状态:present/absent */
typedef enum hiUNF_CI_PCCD_STATUS_E
{
    HI_UNF_CI_PCCD_STATUS_ABSENT,
    HI_UNF_CI_PCCD_STATUS_PRESENT,
    HI_UNF_CI_PCCD_STATUS_BUTT
} HI_UNF_CI_PCCD_STATUS_E, *HI_UNF_CI_PCCD_STATUS_E_PTR;

/** card call mode */
/** CNcomment:卡访问模式 */
typedef enum hiUNF_CI_PCCD_ACCESSMODE_E
{
    HI_UNF_CI_PCCD_ACCESS_ATTR,
    HI_UNF_CI_PCCD_ACCESS_IO,
    HI_UNF_CI_PCCD_ACCESS_COMMON,
    HI_UNF_CI_PCCD_ACCESS_BUTT
} HI_UNF_CI_PCCD_ACCESSMODE_E;

/** get state register mask code */
/** CNcomment:获取状态寄存器掩码 */
typedef enum hiUNF_CI_PCCD_STATUS_BIT_E
{
    HI_UNF_CI_PCCD_STATUS_BIT_ALL,  /**< get all bits */               /**< CNcomment:获取所有位 */
    HI_UNF_CI_PCCD_STATUS_BIT_DA,   /**< get Data Available bit */     /**< CNcomment:获取Data Available位 */
    HI_UNF_CI_PCCD_STATUS_BIT_FR,   /**< get Free bit */               /**< CNcomment:获取Free位 */
    HI_UNF_CI_PCCD_STATUS_BIT_RE,   /**< get Read Error bit */         /**< CNcomment:获取Read Error位 */
    HI_UNF_CI_PCCD_STATUS_BIT_WE,   /**< get Write Error bit */        /**< CNcomment:获取Write Error位 */
    HI_UNF_CI_PCCD_STATUS_BIT_BUTT
} HI_UNF_CI_PCCD_STATUS_BIT_E;

/** The mode of CI Transport Stream Interface connection */
/** CNcomment:CI TSI连接方式 */
typedef enum hiUNF_CI_TSI_MODE_E
{
    HI_UNF_CI_TSI_DAISY_CHAINED,      /**< Daisy-chained */
    HI_UNF_CI_TSI_INDEPENDENT,        /**< Independent, non-daisy-chained */
    HI_UNF_CI_TSI_BUTT
}HI_UNF_CI_TSI_MODE_E;

/** The mode of TS pass to the CI device*/
/** CNcomment:过CI设备的TS流模式 */
typedef enum hiUNF_CI_TS_MODE_E
{
    HI_UNF_CI_TS_SERIAL,         /**< serial 1*/
    HI_UNF_CI_TS_PARALLEL,       /**< Parallel TS, default */
    HI_UNF_CI_TS_USB2SERIAL,     /**< USB input, serial output */
    HI_UNF_CI_TS_USB2PARALLEL,   /**< USB input, parallel output */
    HI_UNF_CI_TS_BUTT
}HI_UNF_CI_TS_MODE_E;

/** The TSI serial number of the CI device*/
/** CNcomment:CI设备的串行TS输入端口 */
typedef enum hiUNF_CI_TSI_SERIAL_PORT_E
{
    HI_UNF_CI_TSI_SERIAL1 = 0x04,  /**< TSI serial 1*/
    HI_UNF_CI_TSI_SERIAL2,         /**< TSI serial 2*/
    HI_UNF_CI_TSI_SERIAL3,         /**< TSI serial 3*/
    HI_UNF_CI_TSI_SERIAL4,         /**< TSI serial 4*/
    HI_UNF_CI_TSI_SERIAL_BUTT
}HI_UNF_CI_TSI_SERIAL_PORT_E;

/** The command exchange channel between the host and device*/
/** CNcomment:CI设备*/
typedef enum hiUNF_CI_CMD_EXCHANGE_CHAN_E
{
    HI_UNF_CI_CMD_EXCHANGE_CHAN_USB,
    HI_UNF_CI_CMD_EXCHANGE_CHAN_SPI,
    HI_UNF_CI_EXCHANGE_CHAN_BUTT,
}HI_UNF_CI_CMD_EXCHANGE_CHAN_E;

/** CI device*/
/** CNcomment:支持的CI设备 */
typedef enum hiUNF_CI_DEV_E
{
    HI_UNF_CI_DEV_CIMAX,        /**< CIMaX, default */
    HI_UNF_CI_DEV_CIMAXPLUS,    /**< CIMaX+ */
    HI_UNF_CI_DEV_HICI,         /**< HICI */
    HI_UNF_CI_DEV_STARCI2WIN,   /**< StarCI2Win */
    HI_UNF_CI_DEV_BUTT
}HI_UNF_CI_DEV_E;

/** CIMaX device configuration */
/** CNcomment:CIMaX设备配置 */
typedef struct hiUNF_CI_CIMAX_ATTR_S
{
    HI_U32  u32SMIBitWidth;         /**< SMI Bit Width */
    HI_U32  u32SMIBaseAddr;         /**< SMI Base Address */
    HI_U32  u32I2cNum;              /**< I2C Group */
    HI_U8   u8DevAddress;           /**< I2C device address */
}HI_UNF_CI_CIMAX_ATTR_S, *HI_UNF_CI_CIMAX_ATTR_S_PTR;

/** CIMaX+ device configuration */
/** CNcomment:CIMaX+设备配置 */
typedef struct hiUNF_CI_CIMAXPLUS_ATTR_S
{
    HI_U32  u32ResetGpioNo; /**< The GPIO to reset the device */
    HI_U32  u32IntGpioNo;   /**< The GPIO to receive the device's Interrupt signal */
    HI_UNF_CI_TSI_SERIAL_PORT_E enTsiSerialPort[HI_UNF_CI_PCCD_BUTT]; /**< The TSI serial port of device*/
    HI_UNF_CI_CMD_EXCHANGE_CHAN_E  enCmdExchangeChan;     /**< The command exchange channel between the host and device*/
    HI_U32  u32SPIDevNo;           /**< The SPI device number*/
}HI_UNF_CI_CIMAXPLUS_ATTR_S, *HI_UNF_CI_CIMAXPLUS_ATTR_S_PTR;

/** HICI device configuration */
/** CNcomment:HICI设备配置 */
typedef struct hiUNF_CI_HICI_ATTR_S
{
    HI_BOOL bIsPowerCtrlGpioUsed;
    HI_U32 u32PowerCtrlGpioNo[HI_UNF_CI_PCCD_BUTT];/**< The GPIO to power on/off the device */
}HI_UNF_CI_HICI_ATTR_S, *HI_UNF_CI_HICI_ATTR_S_PTR;

/** CI device configuration */
/** CNcomment:CI设备配置 */
typedef struct hiUNF_CI_ATTR_S
{
    HI_UNF_CI_DEV_E enDevType;      					/**< CI device type */    
    HI_UNF_CI_TSI_MODE_E enTSIMode; 					/**< The mode of CI Transport Stream Interface connection */
    HI_UNF_CI_TS_MODE_E enTSMode[HI_UNF_CI_PCCD_BUTT]; 	/**< The mode of TS pass to the CI device */
    union
    {
        HI_UNF_CI_CIMAX_ATTR_S stCIMaX;
        HI_UNF_CI_CIMAXPLUS_ATTR_S stCIMaXPlus;
        HI_UNF_CI_HICI_ATTR_S stCIHICI;
    }unDevAttr;
}HI_UNF_CI_ATTR_S, *HI_UNF_CI_ATTR_S_PTR;

/** TS control command */
/** CNcomment:TS控制命令 */
typedef enum hiUNF_CI_PCCD_TSCTRL_E
{
    HI_UNF_CI_PCCD_TSCTRL_BYPASS,
    HI_UNF_CI_PCCD_TSCTRL_SETMODE,
    HI_UNF_CI_PCCD_TSCTRL_WRITETS,
    HI_UNF_CI_PCCD_TSCTRL_BUTT
}HI_UNF_CI_PCCD_TSCTRL_E;
/** USB TS Write parameter */
/** CNcomment:USB送流参数 */
typedef struct hiUNF_CI_PCCD_TSWRITE_S
{
    HI_U8* pu8Data;     /**< Write data [in]*/
    HI_U32 u32Size;     /**< Write data size [in]*/
}HI_UNF_CI_PCCD_TSWRITE_S;

/** TS control parameter */
/** CNcomment:TS控制参数 */
typedef union 
{
    struct
    {
        HI_BOOL bByPass;    /**< TURE means bypass, FALSE means pass-through */  /**< CNcomment:TRUE表示bypass, FALSE表示pass-through */
    }stByPass;

    struct
    {
        HI_UNF_CI_TS_MODE_E enTSMode;        /**< TS mode: parallel/serial/USB */
    }stMode;

    HI_UNF_CI_PCCD_TSWRITE_S stWrite;
    
    /* For extend */
}HI_UNF_CI_PCCD_TSCTRL_PARAM_U;


/** @} */  /** <!-- ==== Structure Definition end ==== */

/** @} */  /** <!-- ==== API declaration end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      CI */
/** @{ */  /** <!-- [CI]*/

/** 
\brief CI init
\brief CNcomment:CI初始化  CNend
\attention \n
N/A
\param    N/A                                         CNcomment:无 CNend
\retval ::HI_SUCCESS              success             CNcomment: 成功 CNend
\retval ::HI_FAILURE              fail                CNcomment:失败 CNend
\retval ::HI_ERR_CI_OPEN_ERR      opening file fail   CNcomment:打开文件失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_Init(HI_VOID);

/** 
\brief CI deinit
\brief CNcomment:CI去初始化  CNend
\attention \n
N/A
\param  N/A                                              CNcomment:无 CNend
\retval ::HI_SUCCESS             success                 CNcomment:成功 CNend
\retval ::HI_FAILURE             fail                    CNcomment:失败 CNend
\retval ::HI_ERR_CI_CLOSE_ERR    closing file fail       CNcomment:关闭文件失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_DeInit(HI_VOID);

/** 
\brief Sets CI device configration.
\brief CNcomment:CI设备属性设置  CNend
\attention \n
N/A
\param[in] enCIPort  CI Port number.                     CNcomment:操作的CI port CNend
\param[in] pstCIAttr The pointer to a structure of CI device. 
                                                         CNcomment:指向CI设备属性的指针。 CNend
\retval ::HI_SUCCESS            success                  CNcomment:成功 CNend
\retval ::HI_FAILURE            fail                     CNcomment:失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_SetAttr(HI_UNF_CI_PORT_E enCIPort, const HI_UNF_CI_ATTR_S *pstCIAttr);

/** 
\brief Gets CI device configration.
\brief CNcomment:CI设备属性获取  CNend
\attention \n
N/A
\param[in] enCIPort  CI Port number.                     CNcomment:操作的CI port CNend
\param[in] pstCIAttr The pointer to a structure of CI device. 
                                                         CNcomment:指向CI设备属性的指针。 CNend
\retval ::HI_SUCCESS            success                  CNcomment:成功 CNend
\retval ::HI_FAILURE            fail                     CNcomment:失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_GetAttr(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_ATTR_S *pstCIAttr);

/** 
\brief open CI Port
\brief CNcomment:打开CI Port CNend
\attention \n
N/A
\param[in] enCIPort     CI Port number                    CNcomment:操作的CI port CNend
\retval ::HI_SUCCESS    success                           CNcomment:成功 CNend
\retval ::HI_FAILURE     fail                             CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter     CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port   CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR    read register error   CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error  CNcomment:写寄存器错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_Open(HI_UNF_CI_PORT_E enCIPort);

/** 
\brief close CI Port
\brief CNcomment:关闭CI Port CNend
\attention \n
N/A
\param[in] enCIPort    CI Port number   CNcomment:操作的CI port CNend
\retval ::HI_SUCCESS       success      CNcomment:成功 CNend
\retval ::HI_FAILURE       fail         CNcomment:失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_Close(HI_UNF_CI_PORT_E enCIPort);

/** 
\brief open CAM
\brief CNcomment:打开CAM卡 CNend
\attention \n
N/A
\param[in] enCIPort    CI Port number                                       CNcomment:操作的CI port CNend
\param[in] enCardId     Card ID                                             CNcomment:操作卡 CNend
\retval ::HI_SUCCESS    success                                             CNcomment:成功 CNend
\retval ::HI_FAILURE     fail                                               CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA   invalid parameter                        CNcomment: 无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open   CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                     CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                     CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                    CNcomment:写寄存器错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_Open(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/**
 \brief close CAM
 \brief CNcomment:关闭CAM卡 CNend

 \attention \n
N/A
 \param[in] enCIPort   CI Port number   CNcomment:操作的CI port CNend
 \param[in] enCardId   Card Id          CNcomment:操作卡 CNend
 \retval ::HI_SUCCESS   success         CNcomment:成功 CNend
 \retval ::HI_FAILURE    fail           CNcomment:失败 CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_CI_PCCD_Close(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/**
 \brief CAM power switch
 \brief CNcomment:CAM卡电源开关 CNend
 \attention \n
	Some solutions don't support power down for a single card, the calling of this interface
	automatically detects a power outage card plug, if it is on ,it will keep power on
    CNcomment:有的方案不支持针对单卡断电，调用本接口断电时会自动检测是否有卡插上，如果有，将不会断电 CNend
 \param[in] enCIPort    CI Port number                                       CNcomment:操作的CI port CNend
 \param[in] enCardId    Card Id                                              CNcomment:操作卡 CNend
 \param[in] enCtrlPower power on\power off                                   CNcomment:开电、关电 CNend
 \retval ::HI_SUCCESS      success                                           CNcomment:成功 CNend
 \retval ::HI_FAILURE      fail                                              CNcomment:失败 CNend
 \retval ::HI_ERR_CI_INVALID_PARA     invalid parameter                      CNcomment:无效参数 CNend
 \retval ::HI_ERR_CI_NOT_INIT         have not been initialized or not open  CNcomment:未初始化或未打开 CNend
 \retval ::HI_ERR_CI_UNSUPPORT        unsupported CI Port                    CNcomment:不支持的CI Port CNend
 \retval ::HI_ERR_CI_REG_READ_ERR     read register error                    CNcomment:读寄存器错误 CNend
 \retval ::HI_ERR_CI_REG_WRITE_ERR    write register error                   CNcomment:写寄存器错误 CNend
 \retval ::HI_ERR_CI_CANNOT_POWEROFF  can not power off                      CNcomment:不能断电 CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_CI_PCCD_CtrlPower(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_CTRLPOWER_E enCtrlPower);

/** 
\brief CI reset
\brief CNcomment:CAM卡复位 CNend
\attention \n
N/A
\param[in] enCIPort    CI Port number                                         CNcomment:操作的CI port CNend
\param[in] enCardId    Card Id                                                CNcomment:操作卡 CNend
\retval ::HI_SUCCESS   success                                                CNcomment:成功 CNend
\retval ::HI_FAILURE   fail                                                   CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                      CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open  CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                    CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                    CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                   CNcomment:写寄存器错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_Reset(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/** 
\brief CAM ready state checking
\brief CNcomment:CAM卡就绪状态检测 CNend
\attention \n
N/A
\param[in] enCIPort       CI Port number                                     CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                            CNcomment:操作卡 CNend
\param[out] penCardReady  state value                                        CNcomment:状态值 CNend
\retval ::HI_SUCCESS      success                                            CNcomment:成功 CNend
\retval ::HI_FAILURE      fail                                               CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA   invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT       have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT      unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR   read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR  write register error                      CNcomment:写寄存器错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_IsReady(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_READY_E_PTR penCardReady);

/** 
\brief check CAM if it is on  
\brief CNcomment:CAM卡是否插上检测 CNend
\attention \n
N/A
\param[in] enCIPort    CI Port number                                         CNcomment:操作的CI port CNend
\param[in] enCardId    Card Id                                                CNcomment:操作卡 CNend
\param[out] penCardStatus  card up or card down                               CNcomment:卡插上或未插状态 CNend
\retval ::HI_SUCCESS        success                                           CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                              CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                      CNcomment:写寄存器错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_Detect(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_STATUS_E_PTR penCardStatus);

/** 
\brief CAM set access module
\brief CNcomment:CAM卡设置访问模块 CNend
\attention \n
N/A
\param[in] enCIPort    CI Port number                                         CNcomment:操作的CI port CNend
\param[in] enCardId    Card Id                                                CNcomment:操作卡 CNend
\param[in] enAccessMode mode you want to set                                  CNcomment:要设置的模式 CNend
\retval ::HI_SUCCESS        success                                           CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                              CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                      CNcomment:写寄存器错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_SetAccessMode(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                    HI_UNF_CI_PCCD_ACCESSMODE_E enAccessMode);

/** 
\brief CAM write or read state checking
\brief CNcomment:CAM卡读写状态检测 CNend
\attention \n
N/A
\param[in] enCIPort    CI Port number                                         CNcomment:操作的CI port CNend
\param[in] enCardId    Card Id                                                CNcomment:操作卡 CNend
\param[in] enStatus    status of the bit will check                           CNcomment:要检测的状态位 CNend
\param[out] pu8Value   status code returned                                   CNcomment:返回的状态码 CNend
\retval ::HI_SUCCESS        success                                           CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                              CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_IO_READ_ERR       read IO error                           CNcomment:IO读错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_GetStatus (HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                HI_UNF_CI_PCCD_STATUS_BIT_E enStatus, HI_U8 *pu8Value);

/** 
\brief CAM IO read data
\brief CNcomment:CAM卡IO读数据 CNend
\attention \n
N/A
\param[in] enCIPort       CI Port number                                      CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                             CNcomment:操作卡 CNend
\param[out] pu8Buffer     data buffer                                         CNcomment: 数据缓冲区 CNend
\param[out] pu32ReadLen   data length that read successful                    CNcomment:成功读取的长度 CNend
\retval ::HI_SUCCESS      success                                             CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                              CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                      CNcomment:写寄存器错误 CNend
\retval ::HI_ERR_CI_IO_READ_ERR     read IO error                             CNcomment:IO读错误 CNend
\see \n
    HI_UNF_CI_PCCD_IOWrite()
*/
HI_S32 HI_UNF_CI_PCCD_IORead(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_U8 *pu8Buffer, HI_U32 *pu32ReadLen);

/** 
\brief CAM IO write data
\brief CNcomment:CAM卡IO写数据 CNend
\attention \n
N/A
\param[in] enCIPort       CI Port number                                      CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                             CNcomment:操作卡 CNend
\param[in] pu8Buffer      data buffer                                         CNcomment:数据缓冲区 CNend
\param[in] u32WriteLen     byte number that have been  writen                 CNcomment:写入字节数 CNend
\param[out] pu32WriteOKLen  data length that write successful                 CNcomment:成功写入字节数 CNend
\param[in] enCIPort       operate CI port                                     CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                             CNcomment:操作卡 CNend
\param[out] pu8Buffer     data buffer                                         CNcomment: 数据缓冲区 CNend
\param[out] pu32ReadLen   data length that read successful                    CNcomment:成功读取的长度 CNend
\retval ::HI_SUCCESS      success                                             CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                              CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                      CNcomment:写寄存器错误 CNend
\retval ::HI_ERR_CI_IO_WRITE_ERR    write IO error                            CNcomment:IO写错误 CNend
\see \n
    HI_UNF_CI_PCCD_IORead()
*/
HI_S32 HI_UNF_CI_PCCD_IOWrite(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_U8 *pu8Buffer, HI_U32 u32WriteLen, HI_U32 *pu32WriteOKLen);

/** 
\brief check CAM CIS information
\brief CNcomment:检验CAM卡CIS信息 CNend
\attention \n
    reference EN50221.
    CNcomment:请参考规范 EN50221. CNend
\param[in] enCIPort       CI Port number                                         CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                                CNcomment:操作卡 CNend
\retval ::HI_SUCCESS      success                                                CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                                 CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_PCCD_DEVICE_BUSY   device busy                               CNcomment:设备忙 CNend
\retval ::HI_ERR_CI_PCCD_CIS_READ      read CIS information fail                 CNcomment:读CIS信息失败 CNend
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                          CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                         CNcomment:写寄存器错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_CheckCIS(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/** 
\brief config CAM COR information
\brief CNcomment:配置CAM卡COR信息 CNend
\attention \n
    N/A
\param[in] enCIPort       CI Port number                                         CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                                CNcomment:操作卡 CNend
\retval ::HI_SUCCESS      success                                                CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                                 CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_PCCD_DEVICE_BUSY   device busy                               CNcomment:设备忙 CNend
\retval ::HI_ERR_CI_ATTR_WRITE_ERR     config COR fail                           CNcomment:配置COR失败 CNend
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:写寄存器错误 CNend
\see \n
    N/A
*/
HI_S32 HI_UNF_CI_PCCD_WriteCOR(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/** 
\brief CAM IO reset
\brief CNcomment:CAM卡IO口复位 CNend
\attention \n
    N/A
\param[in] enCIPort       CI Port number                                         CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                                CNcomment:操作卡 CNend
\retval ::HI_SUCCESS      success                                                CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                                 CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT          not supported CI Port                     CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_PCCD_TIMEOUT       timeout                                   CNcomment:超时 CNend
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:写寄存器错误 CNend
\see \n
    HI_UNF_CI_PCCD_CheckCIS()
*/
HI_S32 HI_UNF_CI_PCCD_IOReset(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/** 
\brief CAM negotiate buffer size
\brief CNcomment:CAM卡协商buffer size CNend
\attention \n
    N/A
\param[in] enCIPort      CI Port number                                          CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                                CNcomment:操作卡 CNend
\param[in] pu16BufferSize   input  buffer size that master support               CNcomment:输入主机端支持buffer size CNend
\param[out] pu16BufferSize  output buffer size after consultation                CNcomment:输出协商后的buffer size CNend
\retval ::HI_SUCCESS      success                                                CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                                 CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_PCCD_TIMEOUT       timeout                                   CNcomment:超时 CNend
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:写寄存器错误 CNend
\retval ::HI_ERR_CI_IO_READ_ERR        read IO error                             CNcomment:IO读错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_NegBufferSize(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                    HI_U16 *pu16BufferSize);

/** 
\brief CAM TS control
CNcomment: \brief CAM卡TS控制 CNend
\attention \n
    N/A
\param[in] enCIPort       CI Port number                                         CNcomment:操作的CI port CNend
\param[in] enCardId       Card Id                                                CNcomment:操作卡 CNend
\param[in] enCMD          control command                                        CNcomment:控制命令 CNend
\param[in] pParam         parameter that matching command                        CNcomment:与命令匹配的参数 CNend
\retval ::HI_SUCCESS      success                                                CNcomment:成功 CNend
\retval ::HI_FAILURE        fail                                                 CNcomment:失败 CNend
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:无效参数 CNend
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:未初始化或未打开 CNend
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:不支持的CI Port CNend
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:读寄存器错误 CNend
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:写寄存器错误 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_TSCtrl(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                        HI_UNF_CI_PCCD_TSCTRL_E enCMD, HI_UNF_CI_PCCD_TSCTRL_PARAM_U *pParam);

/** 
\brief Get CAM CIS 
CNcomment:\brief 获取CAM的CIS数据
\attention \n
    N/A
\param[in] enCIPort			CI Port number                                       CNcomment:操作的CI port
\param[in] enCardId       	Card Id                                              CNcomment:操作卡
\param[out] pu8CisBuffer   	output CIS data                                      CNcomment:输出CAM的CIS数据
\param[out] pu32CisLen  	output CIS data length               				 CNcomment:输出CAM的CIS数据长度
\retval ::HI_SUCCESS      success                                                CNcomment:成功
\retval ::HI_FAILURE        fail                                                 CNcomment:失败
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:无效参数
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:未初始化或未打开
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:不支持的CI Port
\retval ::HI_ERR_CI_PCCD_DEVICE_BUSY   device busy                               CNcomment:设备忙
\retval ::HI_ERR_CI_PCCD_CIS_READ      read cis error                       	 CNcomment:读CIS错误
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_GetCIS(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                             HI_U8 *pu8CisBuffer, HI_U32 *pu32CisLen);

/** 
\brief config CAM COR information
CNcomment:\brief 配置CAM卡COR信息
\attention \n
    N/A
\param[in] enCIPort       CI Port number                                         CNcomment:操作的CI port
\param[in] enCardId       Card Id                                                CNcomment:操作卡
\param[in] u16Addr        COR register address                                   CNcomment:COR寄存器地址
\param[in] u8Data         COR register data                                      CNcomment:要写入COR寄存器的数据
\retval ::HI_SUCCESS      success                                                CNcomment:成功
\retval ::HI_FAILURE        fail                                                 CNcomment:失败
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:无效参数
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:未初始化或未打开
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:不支持的CI Port
\retval ::HI_ERR_CI_PCCD_DEVICE_BUSY   device busy                               CNcomment:设备忙
\retval ::HI_ERR_CI_ATTR_WRITE_ERR     config COR fail                           CNcomment:配置COR失败
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:读寄存器错误
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:写寄存器错误
\see \n
    N/A
*/
HI_S32 HI_UNF_CI_PCCD_WriteCOREx(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                  HI_U16 u16Addr, HI_U8 u8Data);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_ECS_TYPE_H__ */