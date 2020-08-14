/**
 * @file sre_exc.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：异常模块的对外头文件。 \n
 */

/**@defgroup SRE_exc 异常接管
  *@ingroup SRE_inspect
*/

#ifndef _SRE_EXC_H
#define _SRE_EXC_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/**
 *@ingroup SRE_exc
 *异常错误码: 异常模块初始化缓冲区申请失败
 *
 *值：0x02001c01
 *
 *解决方案：确保有足够的动态内存
 */
#define OS_ERRNO_EXC_INIT_BUF_NOT_ENOUGH                        SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x1)

/**
 *@ingroup SRE_exc
 *异常错误码: 获取异常缓冲区信息接口入参为空指针
 *
 *值：0x02001c02
 *
 *解决方案：检查入参指针有效性
 */
#define OS_ERRNO_EXC_PARA_INVLAID                               SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x2)

/**
 *@ingroup SRE_exc
 *异常错误码: 异常缓冲区为空
 *
 *值：0x02001c03
 *
 *解决方案：请确保异常功能裁剪开关已打开
 */
#define OS_ERRNO_EXC_BUFFER_INVLAID                             SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x3)

/**
 *@ingroup SRE_exc
 *异常错误码: 临终遗言注册的钩子函数为空
 *
 *值：0x02001c04
 *
 *解决方案：注册非空的有效钩子函数
 */
#define OS_ERRNO_LASTWORDS_REGSHTCB_CBFUNC_ISNULL               SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x4)

/**
 *@ingroup SRE_exc
 *异常错误码: 临终遗言注册的钩子函数已经存在
 *
 *值：0x02001c05
 *
 *解决方案：更换其他未注册过的钩子函数进行注册
 */
#define OS_ERRNO_LASTWORDS_REGSHTCB_CBFUNC_REPEAT               SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x5)

/**
 *@ingroup SRE_exc
 *异常错误码: 取消的钩子函数为空
 *
 *值：0x02001c06
 *
 *解决方案：传入非空的有效钩子函数
 */
#define OS_ERRNO_LASTWORDS_CANCEL_CBFUNC_ISNULL                 SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x6)

/**
 *@ingroup SRE_exc
 *异常错误码: 取消的钩子函数不存在
 *
 *值：0x02001c07
 *
 *解决方案：确保要取消的钩子函数是存在的
 */
#define OS_ERRNO_LASTWORDS_CANCEL_CBFUNC_NOEXIST                SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x7)

/**
 *@ingroup SRE_exc
 *异常错误码: 注册临终遗言时，以达到临终遗言最大钩子数。
 *
 *值：0x02001c08
 *
 *解决方案：释放掉多余的临终遗言钩子
 */
#define OS_ERRNO_EXC_BUFFER_FULL                                SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x8)

/**
 *@ingroup SRE_exc
 *异常错误码: 异常模块注册钩子函数为空
 *
 *值：0x02001c09
 *
 *解决方案：查看注册异常钩子函数是否为空
 */
#define OS_ERRNO_EXC_REG_HOOK_PTR_NULL                          SRE_ERRNO_OS_ERROR(OS_MID_EXC,0x9)

/**
 *@ingroup SRE_exc
 *异常错误码: 异常注册函数中,double异常栈空间设置太小
 *
 *值：0x02001c0a
 *
 *解决方案：增大double异常栈空间的长度，使之大于异常寄存器信息（EXC_REGS_S）。
 */
#define OS_ERRNO_EXC_REGISTER_DOUBLE_EXC_STACK_TOOSMALL              SRE_ERRNO_OS_ERROR(OS_MID_EXC,0xa)

/**
 *@ingroup SRE_exc
 *异常错误码: 中断优先级掩码被踩，导致系统异常重启
 *
 *值：0x02001c0b
 *
 *解决方案：
 *产品可以通过&g_auwIntMask找到掩码存放地址，判断是否是以下的值
 *SD6108/SD6181 DSP:0xC00010C0 0x307F8D20 0x0F80201E 0x00000201
 *SD6108/SD6181 CPU:0x000000C0 0xF07F9B20 0x0F80201E 0x00000401
 *SD6182:0x2BFFB8C1 0xD400043C 0x00000302
 *如果不是，说明掩码被踩，可在中断优先级掩码所在的内存中打数据断点，定位为何会出现中断优先级掩码被踩的错误。
 */
#define OS_ERRNO_EXC_INTERRUPT_LEVEL_MASK_ERROT                      SRE_ERRNO_OS_FATAL(OS_MID_EXC,0xb)

/**
 *@ingroup SRE_exc
 *异常错误码: Hac超时处理钩子未注册
 *
 *值：0x02001c12
 *
 *解决方案：进入此分支有两种情况，一种是Hac超时导致soc中的interrupt寄存器被清，导致中断调度到此异常分支，
 *一种是中断优先级掩码被踩，导致进入这个分支，然而由于产品没有注册此类异常的钩子，因此进入致命错误处理。
 *产品可以通过&g_auwIntMask找到掩码存放地址，判断是否是以下的值
 *SD6108/SD6181 DSP:0xC00010C0 0x307F8D20 0x0F80201E 0x00000201
 *SD6108/SD6181 CPU:0x000000C0 0xF07F9B20 0x0F80201E 0x00000401
 *SD6182:0x2BFFB8C1 0xD400043C 0x00000302
 *如果掩码都正确，则是hac出现超时自清了。
 */
#define OS_ERRNO_EXC_HAC_EXCEPTION_HOOK_NOREG                      SRE_ERRNO_OS_FATAL(OS_MID_EXC, 0x12)


/**
 * 调用栈信息结构体
 */
typedef struct tagCstkStackInfo {
	UINT32  uwStackAddr;                /**< 函数堆栈框地址 */
	UINT32  uwInstrAddr;                /**< 函数返回地址   */
} CSTK_FRAME_INFO_S;

#if ((OS_HARDWARE_PLATFORM == OS_RISC170) || (OS_HARDWARE_PLATFORM == OS_DSP170))

/**
 *@ingroup SRE_exc
 * 寄存器信息结构
 *
 * 描述:SD6181/SD6108平台下的异常触发时保存的寄存器信息
 * SD6182平台该结构体信息见sre_exc.h
 *
 * 注意:以下寄存器名称去掉类型前缀‘uw’，即对应芯片手册中的寄存器名称
 */
typedef struct tagTensilicaEccRegInfo {
	UINT32 uwMESR;          /**< ECC状态寄存器                          */
	UINT32 uwMECR;          /**< ECC校验寄存器                          */
	UINT32 uwMEVADDR;       /**< ECC地址寄存器                          */
	UINT32 uwMEPC;          /**< ECC异常时PC                            */
	UINT32 uwMEPS;          /**< ECC异常时PS                            */
	UINT32 uwMESAVE;        /**< ECC异常处理数据缓冲寄存器              */
} ECC_REGS_S;

/**
 *@ingroup SRE_exc
 * 寄存器信息结构
 *
 * 描述:SD6181&SD6108平台下的异常触发时保存的寄存器信息
 * SD6182平台该结构体信息见sre_exc.h
 *
 * 注意:以下寄存器名称去掉类型前缀‘uw’，即对应芯片手册中的寄存器名称
 * 可接管的异常类型:除了包括系统自带的64种异常外，还可以接管以下异常类型
 * #define OS_EXCCAUSE_GHWSEMERROR          65   //全局硬件信号量异常
 * #define OS_EXCCAUSE_LHWSEMERROR          66   //局部硬件信号量异常
 * #define OS_EXCCAUSE_WRITEERROR           67   //write error异常
 * #define OS_EXCCAUSE_ECCERROR             68   //L1 ECC异常
 * #define OS_EXCCAUSE_NMIERROR             69   //NMI中断
 * #define OS_EXCCAUSE_RESET_REENTRY        70   //复位向量重入异常
 * #define OS_EXCCAUSE_DOUBLEERROR          71   //double异常
 * #define OS_EXCCAUSE_FATALERROR           72   //致命错误异常
 * #define OS_EXCCAUSE_STACKOVERFLOW        73   //栈越界异常
 */
typedef struct tagTensilicaExcRegInfo {
	UINT32 uwPC;            /**< pc指针地址                             */
	UINT32 uwPS;            /**< 处理器状态                             */
	UINT32 uwEXCCAUSE;      /**< 上一次异常类型                         */
	UINT32 uwEXCVADDR;      /**< 上一次读取、加载或转储异常的虚拟地址   */
	UINT32 uwLBEG;          /**< 循环起始地址                           */
	UINT32 uwLEND;          /**< 循环结束地址                           */
	UINT32 uwLCOUNT;        /**< 循环计数器寄存器                       */
	UINT32 uwSAR;           /**< 移位寄存器                             */
	UINT32 auwA[32];        /**< A组32个通用寄存器                      */
	UINT32 uwWINDOWBASE;    /**< 目前AR REG窗口位置寄存器               */
	UINT32 uwWINDOWSTART;   /**< AR REG窗口pending寄存器                */
	UINT32 uwDEPC;          /**< double 异常程序寄存器                  */
	UINT32 uwBR;            /**< br 寄存器                              */
	UINT32 uwEPC1;          /**< lv1 异常PC寄存器                       */
	UINT32 uwEPC2;          /**< lv2 异常PC寄存器                       */
	UINT32 uwEPC3;          /**< lv3 异常PC寄存器                       */
	UINT32 uwEPC4;          /**< lv4 异常PC寄存器                       */
	UINT32 uwEPC5;          /**< lv5 异常PC寄存器                       */
	UINT32 uwEPC6;          /**< lv6 异常PC寄存器                       */
	UINT32 uwEPS2;          /**< lv2 异常状态寄存器                     */
	UINT32 uwEPS3;          /**< lv3 异常状态寄存器                     */
	UINT32 uwEPS4;          /**< lv4 异常状态寄存器                     */
	UINT32 uwEPS5;          /**< lv5 异常状态寄存器                     */
	UINT32 uwEPS6;          /**< lv6 异常状态寄存器                     */
	UINT32 uwEXCSAVE1;      /**< lv1 异常保存寄存器                     */
	UINT32 uwEXCSAVE2;      /**< lv2 异常保存寄存器                     */
	UINT32 uwEXCSAVE3;      /**< lv3 异常保存寄存器                     */
	UINT32 uwEXCSAVE4;      /**< lv4 异常保存寄存器                     */
	UINT32 uwEXCSAVE5;      /**< lv5 异常保存寄存器                     */
	UINT32 uwEXCSAVE6;      /**< lv6 异常保存寄存器                     */
	UINT32 uwINTENABLE;     /**< 中断使能寄存器                         */
	UINT32 uwINTERRUPT;     /**< 中断pending寄存器                      */
	UINT32 uwSCOMPARE1;     /**< 条件存储比较寄存器                     */
	UINT32 uwCCOMPARE0;     /**< 比较寄存器0                            */
	UINT32 uwCCOMPARE1;     /**< 比较寄存器1                            */
	UINT32 uwCCOMPARE2;     /**< 比较寄存器2                            */
	UINT32 uwCCOUNT;        /**< 比较寄存器计数值                       */
	UINT32 uwCPENABLE;      /**< 协处理使能寄存器                       */
	UINT32 uwVECBASE;       /**< 动态向量表基址                         */
	UINT32 uwDBREAKA0;      /**< 数据断点地址0                          */
	UINT32 uwDBREAKA1;      /**< 数据断点地址1                          */
	UINT32 uwDBREAKC0;      /**< 数据断点控制寄存器0                    */
	UINT32 uwDBREAKC1;      /**< 数据断点控制寄存器1                    */
	UINT32 uwDEBUGCAUSE;    /**< 上一次debug异常原因                    */
	UINT32 uwIBREAK0;       /**< 指令断点地址0                          */
	UINT32 uwIBREAK1;       /**< 指令断点地址1                          */
	UINT32 uwIBREAKENABLE;  /**< 指令断点使能寄存器                     */
	UINT32 uwICOUNT;        /**< 指令计数器                             */
	UINT32 uwICOUNTLEVEL;   /**< 指令计数等级阈值寄存器                 */
	UINT32 uwHwSemErr;      /**< 硬件信号量异常类型标志位               */
	ECC_REGS_S stEccRegInfo;/**< ECC异常保存寄存器信息                  */
} EXC_REGS_S;

#else

/**
 * @ingroup SRE_exc
 * 硬件信号量错误结构体
 * 当前只有6182支持该错误结构体信息
 */
typedef struct tagTensilicaHwSemErrInfo {
	UINT16  usHwSemErrId: 6 ;    /**<硬件信号量错误ID[0-63],如果没有发生全局异常，ID为局部硬件信号量ID，否则为全局硬件信号量ID */
	UINT16  usHwSemGErrFlag: 4;  /**< 硬件信号量全局异常类型标志位 */
	UINT16  usHwSemLErrFlag: 4;  /**< 硬件信号量局部异常类型标志位 */
	UINT16  usResverd: 2;
} HWSEM_ERR_S;

/**
 *@ingroup SRE_exc
 * 寄存器信息结构
 *
 * 描述:SD6182平台下的异常触发时保存的寄存器信息
 *
 * 注意:以下寄存器名称去掉类型前缀‘uw’，即对应芯片手册中的寄存器名称
 */
typedef struct tagTensilicaEccRegInfo {
	UINT32 uwMESR;          /**< ECC状态寄存器                          */
	UINT32 uwMECR;          /**< ECC校验寄存器                          */
	UINT32 uwMEVADDR;       /**< ECC地址寄存器                          */
	UINT32 uwMEPC;          /**< ECC异常时PC                            */
	UINT32 uwMEPS;          /**< ECC异常时PS                            */
	UINT32 uwMESAVE;        /**< ECC异常处理数据缓冲寄存器              */
} ECC_REGS_S;

/*
 *@ingroup SRE_exc
 * 寄存器信息结构
 *
 * 描述:SD6182平台下的异常触发时保存的MSGM异常寄存器信息
*/
typedef struct tagMsgmExcRegInfo {
	UINT16              usTraceDbgLostCnt;      /**< Filter功能消息总线阻塞丢失计数（MSGM_DBG_LOST_CNT ）           */
	UINT8               ucNorQueOvLostCnt;      /**< 总线阻塞正常消息队列溢出计数（MSGM_NOR_OVLOST_CNT）            */
	UINT8               ucPreserved;            /**< 预留字节                                                       */
	UINT8               aucNorQueOvCnt[6];      /**< 每个核第n个Qport对应的第i个队列溢出消息统计寄存器，计满保持，
                                                     软件读清 ，DSP&CPU共两个QPORT，每个QPORT有2个队列；
                                                     HACCC核共3个QPORT3，每个QPORT有2个队列； 故最多6个队列         */
	UINT16              usPreserved;            /**< 预留字节                                                       */
	UINT32              uwLsiVioBusError;       /**< LSI总线错误的类型记录,LSI异常错误id及类型寄存器MSGM_LSI_VIO    */
	UINT32              uwLsiVioBusErrorAddr;   /**< LSI总线访问命令错误地址寄存器MSGM_LSI_VIO_ADDR                 */
	UINT32              uwVioTypeExc;           /**< MSGM异常错误类型寄存器MSGM_VIO_TYPE                            */
	UINT32              uwCreditVioError;       /**< MSGM正常消息队列credit错误信息寄存器MSGM_CRED_VIO              */
} MSGM_REGS_S;

/**
 *@ingroup SRE_exc
 * 寄存器信息结构
 *
 * 描述:SD6182平台下的异常触发时保存的寄存器信息
 *
 * 注意:以下寄存器名称去掉类型前缀‘uw’，即对应芯片手册中的寄存器名称
 * 可接管的异常类型:除了包括系统自带的64种异常外，还可以接管以下异常类型
 * #define OS_EXCCAUSE_WRITEERROR           67   //write error异常
 * #define OS_EXCCAUSE_ECCERROR             68   //L1 ECC异常
 * #define OS_EXCCAUSE_NMIERROR             69   //NMI中断
 * #define OS_EXCCAUSE_RESET_REENTRY        70   //复位向量重入异常
 * #define OS_EXCCAUSE_DOUBLEERROR          71   //double异常
 * #define OS_EXCCAUSE_FATALERROR           72   //致命错误异常
 * #define OS_EXCCAUSE_STACKOVERFLOW        73   //栈越界异常
 * #define OS_EXCCAUSE_HACERROR             74   //HAC异常
 * #define OS_EXCCAUSE_HWSEMERROR           75   //硬件信号量异常
 */
typedef struct tagTensilicaExcRegInfo {
	UINT32 uwPC;            /**< pc指针地址                             */
	UINT32 uwPS;            /**< 处理器状态                             */
	UINT32 uwEXCCAUSE;      /**< 上一次异常类型                         */
	UINT32 uwEXCVADDR;      /**< 上一次读取、加载或转储异常的虚拟地址   */
	UINT32 uwLBEG;          /**< 循环起始地址                           */
	UINT32 uwLEND;          /**< 循环结束地址                           */
	UINT32 uwLCOUNT;        /**< 循环计数器寄存器                       */
	UINT32 uwSAR;           /**< 移位寄存器                             */
	UINT32 auwA[64];        /**< A组64个通用寄存器                      */
	UINT32 uwWINDOWBASE;    /**< 目前AR REG窗口位置寄存器               */
	UINT32 uwWINDOWSTART;   /**< AR REG窗口pending寄存器                */
	UINT32 uwDEPC;          /**< double 异常程序寄存器                  */
	UINT32 uwBR;            /**< br 寄存器                              */
	UINT32 uwEPC1;          /**< lv1 异常PC寄存器                       */
	UINT32 uwEPC2;          /**< lv2 异常PC寄存器                       */
	UINT32 uwEPC3;          /**< lv3 异常PC寄存器                       */
	UINT32 uwEPC4;          /**< lv4 异常PC寄存器                       */
	UINT32 uwEPC5;          /**< lv5 异常PC寄存器                       */
	UINT32 uwEPS2;          /**< lv2 异常状态寄存器                     */
	UINT32 uwEPS3;          /**< lv3 异常状态寄存器                     */
	UINT32 uwEPS4;          /**< lv4 异常状态寄存器                     */
	UINT32 uwEPS5;          /**< lv5 异常状态寄存器                     */
	UINT32 uwEXCSAVE1;      /**< lv1 异常保存寄存器                     */
	UINT32 uwEXCSAVE2;      /**< lv2 异常保存寄存器                     */
	UINT32 uwEXCSAVE3;      /**< lv3 异常保存寄存器                     */
	UINT32 uwEXCSAVE4;      /**< lv4 异常保存寄存器                     */
	UINT32 uwEXCSAVE5;      /**< lv5 异常保存寄存器                     */
	UINT32 uwINTENABLE;     /**< 中断使能寄存器                         */
	UINT32 uwINTERRUPT;     /**< 中断pending寄存器                      */
	UINT32 uwCCOMPARE0;     /**< 比较寄存器0                            */
	UINT32 uwCCOMPARE1;     /**< 比较寄存器1                            */
	UINT32 uwCCOUNT;        /**< 比较寄存器计数值                       */
	UINT32 uwCPENABLE;      /**< 协处理使能寄存器                       */
	UINT32 uwVECBASE;       /**< 动态向量表基址                         */
	UINT32 uwDBREAKA0;      /**< 数据断点地址0                          */
	UINT32 uwDBREAKA1;      /**< 数据断点地址1                          */
	UINT32 uwDBREAKC0;      /**< 数据断点控制寄存器0                    */
	UINT32 uwDBREAKC1;      /**< 数据断点控制寄存器1                    */
	UINT32 uwDEBUGCAUSE;    /**< 上一次debug异常原因                    */
	UINT32 uwIBREAK0;       /**< 指令断点地址0                          */
	UINT32 uwIBREAK1;       /**< 指令断点地址1                          */
	UINT32 uwIBREAKENABLE;  /**< 指令断点使能寄存器                     */
	UINT32 uwICOUNT;        /**< 指令计数器                             */
	UINT32 uwICOUNTLEVEL;   /**< 指令计数等级阈值寄存器                 */
	UINT32 uwResvered0;     /**< 对齐保留                               */
	ECC_REGS_S stEccRegInfo;/**< ECC异常保存寄存器信息                  */
	HWSEM_ERR_S astHwSemErrInfo[17];/**< 硬件信号量异常保存寄存器信息   */
	UINT16 usResvered;
	MSGM_REGS_S stMsgmExcRegInfo; /**< MSGM异常寄存器信息               */
} EXC_REGS_S;

#endif

/**
 *@ingroup SRE_exc
 *@brief 保存异常的处理状态函数类型定义。
 *
 *@par 描述:
 *在单核复位时钩子执行失败的返回信息通过该钩子函数进行保存。
 *@attention 无。
 *
 *@param uwState [IN] 类型#UINT32，异常处理的出错信息。
 *
 *@retval 无。
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V100R001C01
 *@see 无
 */
typedef VOID (* EXC_STATE_TRACE_HOOK)(UINT32 uwState);

/**
 *@ingroup SRE_exc
 *@brief 临终遗言钩子函数类型定义。
 *
 *@par 描述:
 *通过该函数类型定义临终遗言钩子函数。
 *@attention 无。
 *
 *@param 无。
 *
 *@retval 无。
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V100R001C01
 *@see 无
 */
typedef VOID (* SRE_LASTWORDS_FUNC)(VOID);

/**
 *@ingroup SRE_exc
 *@brief 异常处理函数类型定义。
 *
 *@par 描述:
 *通过异常处理函数类型定义异常处理函数钩子，记录异常信息。
 *@attention 无。
 *
 *@param pstExcRegs [IN] 类型#EXC_REGS_S *，异常时寄存器信息。
 *
 *@retval TRUE，系统在发生异常后(并做完相关处理后)进入死循环，等待重启。
 *@retval FALSE，系统在发生异常后(并做完相关处理后)继续运行。
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V100R001C01
 *@see 无
 */
typedef BOOL (* EXC_PROC_FUNC)(EXC_REGS_S *pstExcRegs);

/**
 *@ingroup SRE_exc
 *@brief Hac超时自清异常处理函数类型定义。
 *
 *@par 描述:
 *通过Hac超时自清异常处理函数类型定义异常处理函数钩子，产品通过查看Hac超时寄存器判断是否存在超时，如果存在超时返回TRUE(1)，如果不存在超时，说明系统出现异常，返回FALSE(0),系统进入异常接管。
 *@attention 无。
 *
 *@param VOID。
 *
 *@retval TRUE，系统检测到HAC超时，返回之后OS做中断丢弃处理。
 *@retval FALSE，系统未检测到HAC超时，返回之后OS做异常处理。
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V200R003C06
 *@see 无
 */
typedef BOOL (* HACTIMEOUT_PROC_FUNC)(VOID);

/**
 *@ingroup SRE_exc
 *模块配置信息结构体
 */
typedef struct tagExcModInfo {
	HACTIMEOUT_PROC_FUNC    pfnHacExtHook;          /**< Hac异常钩子                                */
	EXC_PROC_FUNC           pfnExcepHook;           /**< 异常时用户注册的函数钩子                    */
	UINT32                  uwStackSize;            /**< 发生异常时保存堆栈的大小                    */
	VOID                    *pDoubleExcStackStart;  /**< DoubleException堆栈起始地址                */
	UINT32                  uwDoubleExcStackSize;   /**< DoubleException堆栈大小                    */
} EXC_MOD_INFO_S;

/**
 *@ingroup SRE_exc
 *@brief 异常缓冲区首地址和长度获取函数。
 *
 *@par 描述:
 *获取异常信息缓冲区的首地址与大小。
 *@attention 异常发生时，可根据缓冲区的首地址和大小，进而获取到异常信息；无异常时，此间内存值为0。
 *
 *@param ppBufAddr [OUT] 类型#VOID **，返回缓冲区首地址。
 *@param puwBufLen [OUT] 类型#UINT32 *，缓冲区长度。
 *
 *@retval #SRE_OK 0x00000000，成功返回。
 *@retval #OS_ERRNO_EXC_PARA_INVLAID 0x02001c02，入参指针为空。
 *@retval #OS_ERRNO_EXC_BUFFER_INVLAID 0x02001c03，缓冲区为空。
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V100R001C01
 *@see 无
 */
extern UINT32 SRE_ExcGetBufInfo(VOID **ppBufAddr, UINT32 *puwBufLen);

/**
 *@ingroup SRE_exc
 *@brief 临终遗言钩子注册函数。
 *
 *@par 描述:
 *注册临终遗言钩子到临终遗言链表中。
 *@attention
 *@li 注册数量最大支持8条；
 *
 *@param ptrHook [IN] 类型#SRE_LASTWORDS_FUNC，钩子函数。
 *
 *
 *@retval #SRE_OK 0x00000000，成功返回。
 *@retval #OS_ERRNO_EXC_BUFFER_FULL 0x02001c08，注册临终遗言时，以达到临终遗言最大钩子数。
 *@retval #OS_ERRNO_LASTWORDS_REGSHTCB_CBFUNC_ISNULL 0x02001c04，注册的钩子函数为空。
 *@retval #OS_ERRNO_LASTWORDS_REGSHTCB_CBFUNC_REPEAT 0x02001c05，注册的钩子函数已经存在。
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V100R001C02
 *@see SRE_LastWordsDisHook
 */
extern UINT32 SRE_LastWordsRegHook(SRE_LASTWORDS_FUNC ptrHook);

/**
 *@ingroup SRE_exc
 *@brief 临终遗言钩子取消函数。
 *
 *@par 描述:
 *将已注册的临终遗言钩子从链表中删除。
 *@attention 无
 *
 *@param ptrHook [IN] 类型#SRE_LASTWORDS_FUNC，钩子函数。
 *
 *
 *@retval #SRE_OK 0x00000000，成功返回。
 *@retval #OS_ERRNO_LASTWORDS_CANCEL_CBFUNC_ISNULL 0x02001c06，取消的钩子函数为空
 *@retval #OS_ERRNO_LASTWORDS_CANCEL_CBFUNC_NOEXIST 0x02001c07，取消的钩子函数不存在。
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V100R001C02
 *@see SRE_LastWordsRegHook
 */
extern UINT32 SRE_LastWordsDisHook(SRE_LASTWORDS_FUNC ptrHook);

/**
 *@ingroup SRE_exc
 *@brief 用户注册异常处理钩子。
 *
 *@par 描述:
 *注册异常处理钩子。
 *@attention 当多次注册该钩子时，最后一次注册的钩子生效。
 *
 *@param pfnHook [IN] 类型#EXC_PROC_FUNC，钩子函数。
 *
 *
 *@retval #SRE_OK 0x00000000，成功返回。
 *@retval #OS_ERRNO_EXC_REG_HOOK_PTR_NULL 0x02001c09，钩子函数为空
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V100R001C02
 *@see 无
 */
extern UINT32 SRE_ExcRegHook(EXC_PROC_FUNC pfnHook);

/**
 *@ingroup SRE_exc
 *@brief 用户注册Hac超时自清异常处理用户钩子。
 *
 *@par 描述:
 *注册Hac超时自清异常处理用户钩子。
 *@attention 当多次注册该钩子时，最后一次注册的钩子生效。
 *
 *@param pfnHook [IN] 类型#HACTIMEOUT_PROC_FUNC，钩子函数。
 *
 *
 *@retval #SRE_OK 0x00000000，成功返回。
 *@retval #OS_ERRNO_EXC_REG_HOOK_PTR_NULL 0x02001c09，钩子函数为空
 *
 *@par 依赖:
 *sre_exc.h：该接口声明所在的头文件
 *@since UniDSP V200R003C06
 *@see 无
 */
extern UINT32 SRE_HacTimeOutCheckHook(HACTIMEOUT_PROC_FUNC pfnHook);

/**
 * @ingroup  SRE_exc
 * @brief 获取RTOSck异常入口地址
 *
 * @par 描述:
 * SD6181芯片提供有一个硬件TraceBuffer功能，可以实时循环记录指令执行流，当进入异常处理时，为避免TraceBuffer中的信息被异常处理本身的指令流覆盖，
 * 应该停止记录指令流。用户可以用这个API获取异常入口地址,设置到硬件Trace提供的一个结束寄存器，当PC与结束寄存器的值相同时，TraceBuffer自动停止
 * 指令流的记录。
 * @attention 无
 *
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 函数地址                  函数地址，异常处理的函数地址。
 * @par 依赖:
 * sre_exc.h：该接口声明所在的头文件。
 * @since UniDSP V200R003C05
 * @see 无
 */
extern VOID *SRE_ExcEntryGet(VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _SRE_EXC_H */

/**
 * History:
 * 2009-02-23 q00144072: Create this file.
 * 2010-12-23 w00169463: Modify this file.
 * 2012-09-07 y00204700: Modify this file.
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

