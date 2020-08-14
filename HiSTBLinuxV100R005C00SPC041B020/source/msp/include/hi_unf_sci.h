/******************************************************************************

  Copyright (C), 2001-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
 File Name     : hi_unf_sci.h
Version       : Initial draft
Author        : HiSilicon multimedia software group
Created Date   : 2008-06-05
Last Modified by:
Description   : Application programming interfaces (APIs) of the external chip software (ECS)
Function List :
Change History:
******************************************************************************/
#ifndef __HI_UNF_SCI_H__
#define __HI_UNF_SCI_H__

#include "hi_common.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      SCI */
/** @{*/  /** <!-- [SCI] */

/**Output configuration of the smart card interface clock (SCICLK) pin*/
/** CNcomment:SCICLK引脚输出配置*/
typedef enum  hiUNF_SCI_MODE_E
{
    HI_UNF_SCI_MODE_CMOS = 0, /**<Complementary metal-oxide semiconductor (CMOS) output*/   /**<CNcomment:CMOS输出*/
    HI_UNF_SCI_MODE_OD, /**<Open drain (OD) output*/                                  /**<CNcomment:OD输出*/
    HI_UNF_SCI_MODE_BUTT
} HI_UNF_SCI_MODE_E;

/**SCI port*/
/** CNcomment:SCI 端口 */
typedef enum hiUNF_SCI_PORT_E
{
    HI_UNF_SCI_PORT0, /**< SCI port 0*/  /**<CNcomment:SCI端口0*/
    HI_UNF_SCI_PORT1, /**< SCI port 1*/  /**<CNcomment:SCI端口1*/
    HI_UNF_SCI_PORT_BUTT
} HI_UNF_SCI_PORT_E;

/**Status of the SCI card*/
/** CNcomment:智能卡状态 */
typedef enum hiUNF_SCI_STATUS_E
{
    HI_UNF_SCI_STATUS_UNINIT = 0, /**<The SCI card is not initialized.(Reserved status) */               /**<CNcomment: SCI未初始化。(预留状态) */
    HI_UNF_SCI_STATUS_FIRSTINIT, /**<The SCI card is being initialized.(Reserved status)*/             /**<CNcomment:SCI初始化过程中。(预留状态)*/
    HI_UNF_SCI_STATUS_NOCARD, /**<There is no SCI card.*/                          /**<CNcomment:无卡 */
    HI_UNF_SCI_STATUS_INACTIVECARD, /**<The SCI card is not activated (unavailable).*/   /**<CNcomment:卡未完成激活（卡无效） */

    //   HI_UNF_SCI_STATUS_CARDFAULT,		  /**<The SCI card is faulty.*/                        /**<CNcomment:卡故障*/
    HI_UNF_SCI_STATUS_WAITATR, /**<The SCI card is waiting for the ATR data.*/      /**<CNcomment:等待ATR*/
    HI_UNF_SCI_STATUS_READATR, /**<The SCI card is receiving the ATR data.*/        /**<CNcomment:正在接收ATR*/
    HI_UNF_SCI_STATUS_READY, /**<The SCI card is available (activated).*/         /**<CNcomment:卡可以使用（卡激活） */
    HI_UNF_SCI_STATUS_RX, /**<The SCI card is busy receiving data.*/           /**<CNcomment:卡忙（接收数据中） */
    HI_UNF_SCI_STATUS_TX /**<The SCI card is busy transmitting data.*/        /**<CNcomment:卡忙（发送数据中） */
} HI_UNF_SCI_STATUS_E;

/**SCI protocol*/
/** CNcomment:SCI 协议 */
typedef enum hiUNF_SCI_PROTOCOL_E
{
    HI_UNF_SCI_PROTOCOL_T0, /**<7816 T0 protocol*/   /**<CNcomment:7816 T0 协议 */
    HI_UNF_SCI_PROTOCOL_T1, /**<7816 T1 protocol*/   /**<CNcomment:7816 T1 协议 */
    HI_UNF_SCI_PROTOCOL_T14, /**<7816 T14 protocol*/  /**<CNcomment:7816 T14 协议 */
    HI_UNF_SCI_PROTOCOL_BUTT
} HI_UNF_SCI_PROTOCOL_E;

/**SCI active level*/
/** CNcomment:SCI有效电平*/
typedef enum hiUNF_SCI_LEVEL_E
{
    HI_UNF_SCI_LEVEL_LOW, /**<Active low*/   /**<CNcomment:低电平有效 */
    HI_UNF_SCI_LEVEL_HIGH, /**<Active high*/  /**<CNcomment:高电平有效 */
    HI_UNF_SCI_LEVEL_BUTT
} HI_UNF_SCI_LEVEL_E;

/**SCI system parameters*/
/**CNcomment:SCI 系统参数 */
typedef struct hiUNF_SCI_PARAMS_S
{
    HI_UNF_SCI_PORT_E     enSciPort; /**<SCI port ID*/                                                      /**<CNcomment:SCI 端口号 */
    HI_UNF_SCI_PROTOCOL_E enProtocolType; /**<Used protocol type*/                                    /**<CNcomment:使用的协议类型 */
    HI_U32                ActalClkRate; /**<Actual clock rate conversion factor F*/                     /**<CNcomment:实际的F 值时钟转换因子 */
    HI_U32                ActalBitRate; /**<Actual bit rate conversion factor D*/                         /**<CNcomment:实际的D 值比特率转换因子 */
    HI_U32                Fi; /**<Clock factor returned by the answer to reset (ATR)*/                      /**<CNcomment:ATR 返回的时钟因子 */
    HI_U32                Di; /**<Bit rate factor returned by the ATR*/                                     /**<CNcomment:ATR 返回的比特率因子 */
    HI_U32                GuardDelay; /**<Extra guard time N*/                                        /**<CNcomment:N 值额外增加的保护时间*/
    HI_U32                CharTimeouts; /**<Character timeout of T0 or T1*/                             /**<CNcomment:T0 或T1的字符超时时间*/
    HI_U32                BlockTimeouts; /**<Block timeout of T1 */                                     /**<CNcomment:T1的块超时时间*/
    HI_U32                TxRetries; /**<Number of transmission retries*/                                   /**<CNcomment:发送重试次数*/
} HI_UNF_SCI_PARAMS_S, *HI_UNF_SCI_PARAMS_S_PTR;

/** @}*/  /** <!-- ==== Structure Definition End ====*/




/******************************* API Declaration *****************************/
/** \addtogroup      SCI*/
/** @{*/  /** <!-- [SCI] */

/**
 \brief Initializes the SCI.   CNcomment:SCI初始化。CNend
 \attention \n
 \param  N/A                   CNcomment:无。CNend
 \retval 0 Success             CNcomment:成功。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_Init(HI_VOID);

/**
 \brief Deinitializes the SCI. CNcomment:SCI去初始化。CNend
 \attention \n
 \param  N/A                   CNcomment:无。CNend
 \retval 0 Success             CNcomment:0 成功。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_DeInit(HI_VOID);

/**
 \brief Starts an SCI device to initialize it based on the input port and protocol. If there is an SCI card, the card is also initialized.\n
CNcomment:打开SCI（Smart Card Interface）设备，针对输入的端口和协议，进行SCI接口设备的初始化；如果卡存在，则同时对卡进行初始化。CNend
\attention \n
After an SCI device is started, it is initialized based on the default configuration.\n
If you modify the configuration, you need to call HI_UNF_SCI_ResetCard for the modifications to take effect.\n
If you remove and then insert the SCI card, you need to call HI_UNF_SCI_ResetCard to reset the card.\n
You need to set frequency parameters when enabling an SCI device. The actual SCI clock provided by the chip is obtained by using the clock divider.\n
The clock divider is calculated based on the externally transferred clock frequency parameters. The fractional part of the clock divider is discarded during calculation. \n
Therefore, an enhanced error occurs when the SCI clock is obtained by dividing the system clock by the clock divider. You need to set the actual output frequency of \n
the SCI card based on the close output frequency. The SCI clock is calculated as follows: Fsci_clk = Frefclk/[(Clkicc + 1) x 2]. Where, Frefclk is the 96 MHz system clock, \n
Fsci_clk is the SCI clock required by peripherals, and clkicc is the required clock divider of registers. clkicc is calculated as follows: Clkicc = Frefclk/(2 x Fsci_clk) - 1. \n
The clkicc may be a floating point value, but only the integral part is used. Therefore, the configured frequency is different from the actual frequency.\n

CNcomment: 打开后SCI设备采用默认配置对设备进行初始化。\n 
之后如果更改了配置，需要调用HI_UNF_SCI_ResetCard来使配置生效。\n 
之后如果对卡进行拔插，需要调用HI_UNF_SCI_ResetCard来进行复位。\n 
打开设备的时候需要设置频率参数，而芯片实际给SCI 的时钟是由外部传入的 \n
时钟频率参数计算的分频因子分频得来，因为计算公式会舍弃计算的分频因子的小数部分，\n
所以通过系统时钟分频之后给卡的时钟会存在增量误差，实际配置考虑与需求最接近的值来 \n
设置和权衡，具体算法 Fsci_clk = Frefclk/[(Clkicc + 1) x 2];其中Frefclk是系统96M时钟，Fsci_clk是外部需要设置的 \n
sci时钟,实际要配入寄存器分频因子Clkicc = Frefclk/(2 x Fsci_clk) - 1;clkicc 有时候计算出来是浮点数，但只取整，\n
所以设置的频率与实际频率有偏差。CNend

 \param[in] enSciPort     ID of an SCI port. The port ID can be 0 or 1.  CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in] enSciProtocol Protocol type.                                  CNcomment:协议类型。CNend
 \param[in] u32Frequency    Frequency of the SCI card to be set, in kHz. For the T0 and T1 cards, the frequency ranges from 1 MHz to 5 MHz; for the T14 card, the frequency must be set to 6 MHz.  CNcomment:要设置的SCI卡频率。对于T0，T1卡，支持频率1MHz～5MHz；对于T14卡，只支持6MHz。单位为khz。CNend
 \retval 0 Success.                                                       CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  SCI  No SCI device is started.           CNcomment:设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.            CNcomment:参数非法。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_Open(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 u32Frequency);

/**
 \brief Stops an SCI device.
CNcomment:关闭SCI设备。CNend

 \attention \n
This API cannot be called repeatedly.
CNcomment:重复关闭会失败。CNend

 \param[in] enSciPort     ID of an SCI port. The port ID can be 0 or 1.  CNcomment:SCI端口号，取值范围为0和1。CNend
 \retval 0 Success.                                                       CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.               CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.            CNcomment:参数非法。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_Close(HI_UNF_SCI_PORT_E enSciPort);

/**
 \brief Performs a warm reset on an SCI card.
CNcomment:复位SCI卡。CNend

 \attention \n
If no SCI card is inserted, the reset fails.\n
If you modify the configuration of an SCI device, you need to call HI_S32 HI_UNF_SCI_ResetCard for the modifications to take effect.\n
In addition, if you remove and insert an SCI card after calling HI_UNF_SCI_Open, you also need to call HI_S32 HI_UNF_SCI_ResetCard to reset the card.
CNcomment:没有插入卡，复位会失败。\n
当更改了SCI设备的配置后，需要调用该接口使配置生效。\n
在调用打开SCI设备接口后，对卡进行拔插，也需要调用该接口进行复位。CNend

 \param[in] enSciPort   ID of an SCI port. The port ID can be 0 or 1                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in] bWarmResetValid   Reset mode.  HI_TRUE: warm reset; HI_FALSE: cold reset (recommended)    			 CNcomment:复位方式。HI_TRUE: 热复位; HI_FALSE: 冷复位（推荐用这种方式）。CNend
 \retval 0 Success.                                                                                              CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			 CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_ResetCard(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bWarmResetValid);

/**
 \brief Deactivates an SCI card.
CNcomment:去激活SCI卡。CNend

 \attention \n
After an SCI card is deactivated, the card cannot be read or written until it is reset.
CNcomment:去激活卡后，无法读写数据。只有重新复位卡后，才能继续读写。CNend

 \param[in] enSciPort ID of an SCI port. The port ID can be 0 or 1.         CNcomment:SCI端口号，取值范围为0和1。CNend
 \retval 0   Success.                                                        CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT No SCI device is started.                    CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.               CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                 CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_DeactiveCard(HI_UNF_SCI_PORT_E enSciPort);

/**
 \brief Obtains the ATR data of an SCI card.
CNcomment:获取SCI卡ATR数据。CNend

 \attention \n
N/A
 \param[in]  enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[out]  pu8AtrBuf   Address for storing the obtained ATR data.                                               CNcomment:获取的ATR数据的存储地址。CNend
 \param[in]  u32AtrBufSize  Length of the ATR data read from the buffer.                                          CNcomment:ATR 数据读取 buffer 长度。CNend
 \param[out]  pu8AtrRcvCount Actual number of ATR data segments.                                                   CNcomment:实际获取的ATR数据个数。CNend
 \retval 0   Success.                                                                                             CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                                                        CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_NULL_PTR The pointer is invalid.                                                      	  CNcomment: 非法指针。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                     CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                               CNcomment:无ATR数据。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			  				  CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_GetATR(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pu8AtrBuf, HI_U32 u32AtrBufSize, HI_U8 *pu8AtrRcvCount);

/**
 \brief Obtains the status of an SCI card.
CNcomment:获取SCI卡状态。CNend

 \attention \n
This API is a non-block API. You can transfer data to an SCI card by calling HI_UNF_SCI_Send or HI_UNF_SCI_Receive only when the card status is HI_UNF_SCI_STATUS_READY.\n
CNcomment:此接口是无阻塞函数，在卡的状态为HI_UNF_SCI_STATUS_READY时，才可调用HI_UNF_SCI_Send或HI_UNF_SCI_Receive接口与卡进行数据交互。CNend

 \param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[out] penSCIStatus  Status of an SCI card.                                                                CNcomment: SCI卡状态。CNend
 \retval 0 Success.                                                                                              CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_NULL_PTR The pointer is invalid.                                                      	 CNcomment:非法指针。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			 CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_GetCardStatus(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_STATUS_E *penSciStatus);

/**
 \brief Transmits data to an SCI card.
CNcomment:向SCI卡发送数据。CNend

 \attention \n
Data must be transmitted based on application protocols.\n
Data cannot be transmitted consecutively.\n
In addition, data is transmitted and received in block mode. Therefore, when a large number of data is being transmitted, the transmission may fail if the timeout is too small.\n
CNcomment:发送数据需要按照应用协议来发送 \n
不能连续两次发送而中间不接收数据 \n
发送读取都是阻塞的，因此发送大量的数据时，受到超时时间的限制，如果超时值比较短，有可能发送失败。CNend

 \param[in] enSciPort   ID of an SCI port. The port ID can be 0 or 1.                                            CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in]  pSciSendBuf Address for storing the data to be transmitted.                                          CNcomment:发送数据的存储地址。CNend
 \param[in] u32SendLen  Number of data segments (in byte) to be transmitted.                                      CNcomment:发送数据的个数,单位为 BYTE。CNend
 \param[in]  pu32ActLen Number of transmitted data segments (in byte).                                            CNcomment:实际发送数据个数,单位为 BYTE。CNend
 \param[in] u32TimeoutMs Wait timeout (in ms). 0: not blocked; 0xFFFFFFFF: infinite block.                        CNcomment:等待超时值, 单位是毫秒, 0 - 不阻塞, 0xFFFFFFFF-永久阻塞。CNend
 \retval 0 Success.                                                                                               CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  SCI  No SCI device is started.                                                   CNcomment:设备未打开。CNend
 \retval ::HI_ERR_SCI_NULL_PTR  The pointer is invalid.                                                     	 CNcomment:非法指针。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA The parameter is invalid.                                                     CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_SEND_ERR  The transmission operation fails.                                             	CNcomment: 发送操作失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_Send(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciSendBuf, HI_U32 u32SendLen, HI_U32 *pu32ActLen,
                       HI_U32 u32TimeoutMs);

/**
 \brief Receives data from an SCI card.
CNcomment:从SCI卡接收数据。CNend

 \attention \n
You must set the data length obtained by each upper-layer application based on the protocol. If the length of the obtained data is greater than that of the returned data, this API is returned after timeout occurs.\n
CNcomment:上层应用程序必须根据协议来配置所获取的长度，如果希望获取的长度超出实际能够返回的长度，则只能等到超时到期才能返回。CNend

 \param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[out]  pSciReceiveBuf   Address for storing the received data.                                              CNcomment:接收数据的存储地址。CNend
 \param[in] u32ReceiveLen  Number of data segments (in byte) to be received.                                      CNcomment:期望接收数据的个数,单位为 BYTE。CNend
 \param[out]  pu32ActLen   Number of received data segments (in byte).                                             CNcomment:实际接收数据个数,单位为 BYTE。CNend
 \param[in] u32TimeOutMs  Wait timeout (in ms). 0: not blocked; 0xFFFFFFFF: infinite block.                       CNcomment:等待超时值, 单位是毫秒, 0 - 不阻塞, 0xFFFFFFFF-永久阻塞。CNend
 \retval 0 Success.                                                                                               CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                                                       CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_NULL_PTR  The pointer is invalid.                                                     	 CNcomment: 非法指针。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			 CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_RECEIVE_ERR  The reception operation fails.                                             	CNcomment:接收操作失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_Receive(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciReceiveBuf, HI_U32 u32ReceiveLen, HI_U32 *pu32ActLen,
                          HI_U32 u32TimeOutMs);

/**
 \brief Sets the active level of the VCCEN signal.
CNcomment:配置VCCEN信号线的有效电平。CNend
 \attention \n
The active level needs to be set based on the circuits of hardware, and the low level is active by default.\n
After changing the active level, you need to call HI_UNF_SCI_ResetCard for the modification take effect.\n
CNcomment:需要根据硬件电路进行配置，默认为低电平有效，更改此项配置后需要调用HI_UNF_SCI_ResetCard才能使新的配置有效。CNend

 \param[in] enSciPort ID of an SCI port. The port ID can be 0 or 1.                                              CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in] enSciVcc  Active level of a signal. HI_FALSE: active low; HI_TRUE: active high  Active level of a signal. HI_FALSE: active low; HI_TRUE: active high.  CNcomment:信号线的有效电平。HI_FALSE：低电平有效，HI_TRUE：高电平有效。CNend
 \retval 0   Success.                                                                                             CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                                                       CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			 CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_ConfigVccEn(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciVcc);

/**
 \brief Sets the active level of the DETECT signal.
CNcomment:配置DETECT信号线的有效电平。CNend

 \attention \n
The active level needs to be set based on the circuits of hardware, and the high level is active by default.\n
After changing the active level, you need to call HI_UNF_SCI_ResetCard for the modification take effect.\n
CNcomment:需要根据硬件电路进行设置，默认为高电平有效，更改此项配置后需要调用HI_UNF_SCI_ResetCard才能使新的配置有效。CNend

 \param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in] enSciDetect Active level of a signal. HI_FALSE: active low; HI_TRUE: active high.                     CNcomment:信号线的有效电平。HI_FALSE：低电平有效，HI_TRUE：高电平有效。CNend
 \retval 0  Success.                                                                                              CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			 CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_ConfigDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciDetect);

/**
 \brief Sets the mode of a clock signal.
CNcomment:设置时钟线的模式。CNend
 \attention \n
The mode needs to be set based on the circuits of hardware, and the OD mode is selected by default.\n
After changing the mode, you need to call HI_UNF_SCI_ResetCard for the modification take effect.\n
CNcomment:需要根据硬件电路进行设置，默认为OD模式，更改此项配置后需要调用HI_UNF_SCI_ResetCard才能使新的配置有效。CNend

 \param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in] enClkMode  Mode of a clock signal.                                                                    CNcomment:时钟线的模式。CNend
 \retval 0 Success.                                                                                               CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                            			 CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_ConfigClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enClkMode);

/**
 \brief Sets the mode of a clock signal.
CNcomment:设置RESET线的模式。CNend
 \attention \n
The mode needs to be set based on the circuits of hardware, and the OD mode is selected by default.\n
After changing the mode, you need to call HI_UNF_SCI_ResetCard for the modification take effect.\n
CNcomment:需要根据硬件电路进行设置，默认为OD模式，更改此项配置后需要调用HI_UNF_SCI_ResetCard才能使新的配置有效。CNend

 \param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in] enClkMode  Mode of a reset signal.                                                                    CNcomment:时钟线的模式。CNend
 \retval 0 Success.                                                                                               CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                            			                 CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_NOTSUPPORT	Current chipset not support config RESET output type.                        CNcomment:当前芯片不支持配置RESET输出类型。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_ConfigResetMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enResetMode);

/**
 \brief Sets the mode of a clock signal.
CNcomment:设置POWEREN线的模式。CNend
 \attention \n
The mode needs to be set based on the circuits of hardware, and the OD mode is selected by default.\n
After changing the mode, you need to call HI_UNF_SCI_ResetCard for the modification take effect.\n
CNcomment:需要根据硬件电路进行设置，默认为OD模式，更改此项配置后需要调用HI_UNF_SCI_ResetCard才能使新的配置有效。CNend

 \param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in] enClkMode  Mode of a poweren signal.                                                                    CNcomment:时钟线的模式。CNend
 \retval 0 Success.                                                                                               CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                            			                 CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_NOTSUPPORT	Current chipset not support config POWEREN output type.                      CNcomment:当前芯片不支持配置POWEREN输出类型。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_ConfigVccEnMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enVccEnMode);


/**
 \brief Changes the card slot.
CNcomment:切换卡槽。CNend

 \attention \n
When you call HI_UNF_SCI_Open, the primary card is used by default, that is, port 0 is used.\n
After you switch to a new SCI device, all the preceding SCI APIs are called to operate the new SCI device.\n
In addition, after a new SCI device is switched, the device is initialized based on the default configuration.\n
If any modifications are made, you need to call HI_UNF_SCI_ResetCard for the modifications to take effect.\n
If the SCI port to be switched to is the same as the current active port, an error code is returned.\n
CNcomment:调用HI_UNF_SCI_Open时，默认打开的为主卡，即端口0。\n
切换到新的SCI设备后，上述所有SCI接口即对新的SCI设备进行操作。\n
切换到新的SCI设备采用默认配置对设备进行初始化。\n
之后如果更改了配置，需要调用HI_UNF_SCI_ResetCard来使配置生效。\n
希望切换到的SCI端口与当前有效端口相同时，会返回失败。CNend

 \param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param[in] enSciProtocol  Protocol type.                                                                         CNcomment:协议类型。CNend
 \param[in] u32Frequency Frequency of the SCI card to be set, in kHz. For the T0 and T1 cards, the frequency ranges from 1 MHz to 5 MHz; for the T14 card, the frequency must be set to 6 MHz. CNcomment:要设置的SCI卡频率。对于T0，T1卡，支持频率1MHz~5MHz；对于T14卡，只支持6MHz。单位为kHz。CNend
 \retval 0  Success.                                                                                              CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_NULL_PTR The pointer is invalid.                                                      	 CNcomment:非法指针。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			 CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_SwitchCard(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 u32Frequency);

/**
 \brief Sets the clock rate factor (in ETU) and baud rate regulator factor for special cards.
CNcomment:对特殊卡需要设置指定etu 时钟率因子，波特率调节因子。CNend

 \attention \n
This API needs to be called only for special cards.
The configured values must match the clock rate conversion factor F and bit rate regulator factor D defined in the protocol.
You can also set the factors to the values defined in the card specifications. Note that the values must be set correctly.
CNcomment:只有特需要求的卡需要设置，普通卡不需要调用此接口设置，
设置的值要与协议中规定的F时钟转率转换因子、D比特率调节因子兼容，
或者设置为卡规范中规定的值，不可随意设置。CNend

 \param [in] 	enSciPort	ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI端口号，取值范围为0和1。CNend
 \param [in] 	u32ClkFactor  Clock rate conversion factor, ranging from 372 to 2048. For details, see the factor F and card features in the protocol.	CNcomment:时钟转率因子372～2048 ，具体设置参考协议的F因子和卡特性。CNend
 \param [in] 	u32BaudFactor	Baud rate regulator factor 1, 2 x n (n = 1-16). For details, see the factor D and card features in the protocol. CNcomment:波特率校正因子1、2*n (n=1～16) ，具体设置参考协议D因子和卡特性。CNend
 \retval  0 	Success.                                                                                               CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                          CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA The parameter is invalid.                                                       CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             			   CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_SetEtuFactor(HI_UNF_SCI_PORT_E enSciPort, HI_U32 u32ClkFactor, HI_U32 u32BaudFactor);

/**
 \brief \brief Sets the extra guard time for transmitting two consecutive bytes from the terminal to an IC card. The guard time is the interval between the start edges of two consecutive bytes.
CNcomment:设置终端向IC卡发送连续的两个字节起始沿额外增加的间隔时间。CNend

 \attention \n
This setting is performed based on related features only for the cards that require extra character guard time.
In general, the default value is used or the interval guard time is automatically set based on the ART analysis result.
CNcomment:只有特需要求的卡，需要额外设置字符保护时间的才能根据卡的相关特性设置，
不可随意设置。一般使用系统默认值，或者系统根据ATR解析自动设置。CNend
 \param [in] 	enSciPort	ID of an SCI port. The port ID can be 0 or 1.      	CNcomment:SCI端口号，取值范围为0和1。CNend
 \param [in] 	u32GuardTime  Extra guard time, ranging from 0 to 254 ETU.	   	CNcomment:额外间隔保护的范围0～254  etu。CNend
 \retval 0  Success.	                                                       		CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                  		CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.               		CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                          CNcomment:不可用的选项。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_SetGuardTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 u32GuardTime);

/**
 \brief Sets the baud rate supported by the SCI card through protocol and parameters selection (PPS) negotiation or performs negotiation by using the F and D factors returned by the ATR to switch the protocol of a card.
CNcomment:通过PPS 协商可以设置卡所支持的波特率或者用ATR 传回的F、D因子进行协商，支持多协议卡的协议切换。CNend
\attention \n
PPS negotiation is available only when the SCI card supports this function.
The negotiated baud rate must be supported by the SCI card.
The command words for negotiation must comply with the specifications defined in section 9.2 "PPS request and response" in the 7816-3 protocol.
CNcomment:要进行PPS 协商首先卡必须支持该功能，
要协商的波特率内容必须是卡所规定的支持的波特率范围，
请求协商的命令字需符合7816-3 中9.2 PPS request and response。CNend

 \param [in] 	enSciPort	ID of an SCI port. The port ID is 0 or 1.                                 CNcomment:SCI端口号，取值范围为0和1。CNend
 \param [in] 	pSciSendBuf  PPS negotiation command combined based on card specifications. If the default negotiation mode is used, the command word can be left empty.   CNcomment:根据卡规范组合的PPS协商命令，若使用系统默认的协商方式，命令字的内容可为空。CNend
 \param [in]	Sendlen	   Length of the command word to be transmitted, ranging from 0 bytes to 5 bytes. The value 0 indicates that the default negotiation mode is used.   CNcomment: 发送命令字的长度(0～5),设置为0表示使用系统默认的协商命令方式。CNend
 \param [in]	RecTimeouts  PPS response timeout after commands are transmitted. The value ranges from 1 ms to 10000 ms.  CNcomment: 发送完命令之后，接收PPS 响应的超时时间(1-10000),单位是毫秒。CNend
 \retval 0 	Success.                                                                               		CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                                         		CNcomment:SCI设备未打开。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                      		CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_NULL_PTR		The pointer is null.                                          		CNcomment:空指针。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                                          	CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_RECEIVE_ERR  A reception error occurs.                                       		CNcomment:接收错误。CNend
 \retval ::HI_ERR_SCI_PPS_PTYPE_ERR 	The protocol type for PPS negotiation is incorrect.           		CNcomment:PPS协商的协议类型错误。CNend
 \retval ::HI_ERR_SCI_PPS_FACTOR_ERR 	The F factor and D factor for PPS negotiation are incorrect. 	        CNcomment:PPS协商的F、D因子错误。CNend
 \retval ::HI_ERR_SCI_PPS_NOTSUPPORT_ERR  The PPS negotiation type is not supported.               		CNcomment:不支持的PPS协商类型。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_NegotiatePPS(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciSendBuf, HI_U32 Sendlen, HI_U32 RecTimeouts);

/**
 \brief  Obtains the PPS data for the card response after PPS negotiation is successful.
CNcomment:PPS 协商成功之后,通过该接口可获得卡响应的PPS 数据。CNend
 \attention \n
This API takes effect only after PPS negotiation is successful.
CNcomment:要等PPS 协商成功之后,使用该接口才有实际意义。CNend

 \param [in] 	enSciPort	ID of an SCI port. The port ID is 0 or 1.                   CNcomment:SCI端口号，取值范围为0和1。CNend
 \param [out] 	pRecBuf   Address of the buffer for storing the data to be received.   	CNcomment:待接收的数据buffer 地址。CNend
 \param [out]	pDataLen	  Required length of the received data.                      CNcomment:期望接收数据的长度。CNend
 \retval 0  Success.	                                                                	CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                        	CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_NULL_PTR	The pointer is null.                                	CNcomment:空指针。CNend
 \retval ::HI_ERR_SCI_PPS_NOTSUPPORT_ERR  The PPS negotiation type is not supported. 	CNcomment:不支持的PPS协商类型。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_GetPPSResponData(HI_UNF_SCI_PORT_E enSciPort, HI_U8* pRecBuf, HI_U32* pDataLen);

/**
brief  Obtains the SCI parameter values including the protocol, ATR clock factor, ATR bit rate factor, actual clock factor, actual bit rate factor, number of retries, guard delay, character wait timeout, and block wait timeout.
CNcomment:获取sci 参数包括协议、ATR 时钟和比特率因子、实际设置的时钟因子和比特率因子，
发送重试次数、GuardDelay 、字符等待超时、块等待超时。CNend
 \attention \n
This API is available only after HI_UNF_SCI_Init and HI_UNF_SCI_ResetCard are called.
CNcomment:要等 到初始化复位之后,才能使用该接口。CNend

 \param [in] 	enSciPort	 ID of an SCI port. The port ID is 0 or 1.                      CNcomment:SCI端口号，取值范围为0和1。CNend
 \param [out] 	pParams   Pointer to the data type of the data structure to be received.   	CNcomment:待接收的结构体数据类型指针。CNend
 \retval 0 	Success.                                                                     	CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA The parameter is invalid.                             	CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_NULL_PTR		The pointer is null.                                    CNcomment:空指针。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                                	CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_NOT_INIT  	The SCI device is not initialized.                      CNcomment:没完成初始化。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_GetParams(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PARAMS_S_PTR pParams);

/**
 \brief  Sets the character timeout of T0 and T1 separately based on the current protocol type.
CNcomment:根据当前协议类型分别可设置T0、T1的字符超时时间。CNend

 \attention \n
The timeout can be set only after the SCI card is reset successfully. The protocol type must be consistent with the current card type. In general, the timeout is obtained by parsing the ATR. That is, the timeout needs to be set only when card communication fails.
CNcomment:要等卡复位成功之后,才能进行设置，协议类型参数必须与当前卡类型一致，一般情况都是根据ATR解析，只有特殊卡情况才需要设置。CNend

 \param [in] 	enSciPort	ID of an SCI port. The port ID is 0 or 1.   CNcomment:SCI端口号，取值范围为0和1。CNend
 \param [in] 	enSciProtocol  Protocol type.                           	CNcomment:协议类型。CNend
 \param [in]	MaxCharTime  Maximum character timeout. The value of MaxCharTime for T0 ranges from 960 to 244800, and the value of MaxCharTime for T1 ranges from 12 to 32779.	  CNcomment:设置最大的字符超时时间，设置范围T0(960～244800),T1(12～32779)。CNend
 \retval 0 	Success.                                                  	CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.         	CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.             	CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_NO_ATR  	The SCI card fails to be reset.        	CNcomment:没复位成功。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_SetCharTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 MaxCharTime);

/**
 \brief  Sets the block timeout of T1.
CNcomment:设置T1的块超时时间。CNend
\attention \n
The timeout can be set only after the SCI card is reset successfully. In general, the timeout is obtained by parsing the ATR. That is, the timeout needs to be set only in special cases
CNcomment:要等卡复位成功之后,才能进行设置，一般情况都是根据ATR解析，只有特殊卡情况才需要设置。CNend

 \param [in] 	enSciPort	 ID of an SCI port. The port ID is 0 or 1.                         	CNcomment:SCI端口号，取值范围为0和1。CNend
 \param [in] 	MaxBlockTime   Maximum block timeout. The value ranges from 971 to 491531.   	CNcomment:设置块超时的最大值，范围(971～491531)。CNend
 \retval 0 	Success.                                                                        		CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA The parameter is invalid.                                		CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                                   	CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_NO_ATR  		The SCI card fails to be reset.                            	CNcomment:没复位成功。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_SetBlockTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_U32 MaxBlockTime);

/**
 \brief  Sets the maximum number of transmission retries after a check error occurs.
CNcomment:设置校验错误后重发送最大次数。CNend
\attention \n
The number can be set only after the SCI card is reset successfully.
CNcomment:要等卡复位成功之后,才能进行设置。CNend

 \param [in] 	enSciPort	ID of an SCI port. The port ID is 0 or 1.                   CNcomment:SCI端口号，取值范围为0和1。CNend
 \param [in] 	TxRetryTimes   Number of transmission retries, ranging from 0 to 7.    	CNcomment:次数范围(0～7)。CNend
 \retval 0 	Success.                                                                  	CNcomment:成功。CNend
 \retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                         	CNcomment:参数非法。CNend
 \retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             	CNcomment:不可用的选项。CNend
 \retval ::HI_ERR_SCI_NO_ATR  	The SCI card fails to be reset.	                       	CNcomment:没复位成功。CNend

 \see \n
N/A
 */
HI_S32 HI_UNF_SCI_SetTxRetries(HI_UNF_SCI_PORT_E enSciPort, HI_U32 TxRetryTimes);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_UNF_SCI_H__ */
