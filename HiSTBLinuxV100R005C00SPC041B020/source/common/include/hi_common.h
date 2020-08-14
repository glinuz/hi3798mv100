/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_common.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   : Common apis for hisilicon system.
  History       :
  1.Date        : 2010/01/25
    Author      : jianglei
    Modification: Created file

*******************************************************************************/

#ifndef __HI_COMMON_H__
#define __HI_COMMON_H__

#include "hi_type.h"
#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*******************************Structure declaration *****************************/
/** @addtogroup     COMMON */
/** @{ */ /** <!--  [COMMON] */

/** Global config structure */
typedef struct hiSYS_CONF_S
{
    HI_U32 u32Reverse;  /**<Not used, reserved for extension*/ /**<CNcomment: 暂时没有使用，留待扩展*/

}HI_SYS_CONF_S;

/** Define the chip type. */
typedef enum hiCHIP_TYPE_E
{
    HI_CHIP_TYPE_HI3716M,
    HI_CHIP_TYPE_HI3716H,
    HI_CHIP_TYPE_HI3716C,
    HI_CHIP_TYPE_HI3716D,

    HI_CHIP_TYPE_HI3720,
    HI_CHIP_TYPE_HI3712,
    HI_CHIP_TYPE_HI3715,

    HI_CHIP_TYPE_HI3718M,
    HI_CHIP_TYPE_HI3718C,
    HI_CHIP_TYPE_HI3719M,
    HI_CHIP_TYPE_HI3719C,
    HI_CHIP_TYPE_HI3719M_A,

    HI_CHIP_TYPE_HI3796C    = 0x20,
    HI_CHIP_TYPE_HI3798C,
    HI_CHIP_TYPE_HI3796M,
    HI_CHIP_TYPE_HI3798M,

    HI_CHIP_TYPE_HI3796C_A  = 0x40,
    HI_CHIP_TYPE_HI3798C_A,
    HI_CHIP_TYPE_HI3798C_B,

    HI_CHIP_TYPE_HI3798M_A,

    HI_CHIP_TYPE_BUTT
}HI_CHIP_TYPE_E;

/** Define the chip version. */
typedef enum hiCHIP_VERSION_E
{
    HI_CHIP_VERSION_V100 = 0x100,
    HI_CHIP_VERSION_V101 = 0x101,
    HI_CHIP_VERSION_V200 = 0x200,
    HI_CHIP_VERSION_V300 = 0x300,
    HI_CHIP_VERSION_V310 = 0x310,
    HI_CHIP_VERSION_V400 = 0x400,
    HI_CHIP_VERSION_V410 = 0x410,
    HI_CHIP_VERSION_V420 = 0x420,
    HI_CHIP_VERSION_BUTT
}HI_CHIP_VERSION_E;

typedef enum
{
    HI_CHIP_PACKAGE_TYPE_BGA_15_15 = 0,
    HI_CHIP_PACKAGE_TYPE_BGA_16_16,
    HI_CHIP_PACKAGE_TYPE_BGA_19_19,
    HI_CHIP_PACKAGE_TYPE_BGA_23_23,
    HI_CHIP_PACKAGE_TYPE_BGA_31_31,
    HI_CHIP_PACKAGE_TYPE_QFP_216,
    HI_CHIP_PACKAGE_TYPE_BGA_21_21,
    HI_CHIP_PACKAGE_TYPE_BGA_14_14,
    HI_CHIP_PACKAGE_TYPE_BUTT
} HI_CHIP_PACKAGE_TYPE_E;

/** Define the chip support attrs */
typedef enum hiCHIP_CAP_E
{
    HI_CHIP_CAP_DOLBY,
    HI_CHIP_CAP_DTS,
    HI_CHIP_CAP_ADVCA,
    HI_CHIP_CAP_MACROVISION,
    HI_CHIP_CAP_HDR10,
    HI_CHIP_CAP_DOLBYVISION,
    HI_CHIP_CAP_BUTT 
} HI_CHIP_CAP_E;

/**System version, that is, the version of the software developer's kit (SDK)*/
typedef struct hiSYS_VERSION_S
{
    HI_CHIP_TYPE_E  enChipTypeSoft;      /**<Chip type corresponding to the SDK*/ /**<CNcomment:  SDK软件对应的芯片类型 */
    HI_CHIP_TYPE_E  enChipTypeHardWare;  /**<Chip type that is detected when the SDK is running*/ /**<CNcomment:  SDK运行时检测到的芯片类型 */
    HI_CHIP_VERSION_E enChipVersion;     /**<Chip version that is detected when the SDK is running*/ /**<CNcomment: SDK运行时检测到芯片版本号 */
    HI_CHAR         aVersion[80];        /**<Version string of the SDK*/ /**<CNcomment:  SDK软件版本号字符串 */
    HI_CHAR         BootVersion[80];     /**<Version string of the Boot*/ /**<CNcomment:  Boot版本号字符串 */
}HI_SYS_VERSION_S;


/** Define the chip attributes */
typedef struct hiSYS_CHIP_ATTR_S
{
    HI_BOOL bDolbySupport;         /**<Whether support dolby or not*//**<CNcomment:芯片是否支持杜比*/
    HI_BOOL bDTSSupport;           /**<Whether support DTS or not*//**<CNcomment:芯片是否支持DTS */
    HI_BOOL bADVCASupport;         /**<Whether support ADVCA or not*//**<CNcomment:芯片是否支持高安*/
    HI_BOOL bMacrovisionSupport;   /**<Whether support Macrovision or not*//**<CNcomment:芯片是否支持Macrovision */
    HI_BOOL bHDR10;                /**<Whether support HDR10 or not*//**<CNcomment:芯片是否支持HDR10 */
    HI_BOOL bDolbyvisionSupport;   /**<Whether support DolbyVision or not*//**<CNcomment:芯片是否支持Dolbyvision */
    HI_U64  u64ChipID;             /**<the unique 64 bits chipid*//**<CNcomment:64位芯片唯一ID */
    HI_U32  u32ChipID;             /**<the unique 32 bits chipid since 98mv200*//**<CNcomment:32位芯片唯一ID，98mv200及以后的芯片开始支持*/
}HI_SYS_CHIP_ATTR_S;

/** Maximum bytes of a buffer name */
#define MAX_BUFFER_NAME_SIZE 16

/**Structure of an MMZ buffer*/
typedef struct hiMMZ_BUF_S
{
    HI_CHAR bufname[MAX_BUFFER_NAME_SIZE];  /**<Strings of an MMZ buffer name*/ /**<CNcomment:  MMZ buffer名字字符串 */
    HI_U32  phyaddr;                /**<Physical address of an MMZ buffer*/ /**<CNcomment:  MMZ buffer物理地址 */
    HI_U8  *kernel_viraddr;         /**<Kernel-state virtual address of an MMZ buffer*/ /**<CNcomment:  MMZ buffer内核态虚拟地址 */
    HI_U8  *user_viraddr;           /**<User-state virtual address of an MMZ buffer*/ /**<CNcomment:  MMZ buffer用户态虚拟地址 */
    HI_U32  bufsize;                /**<Size of an MMZ buffer*/ /**<CNcomment:  MMZ buffer大小 */
    HI_U32  overflow_threshold;     /**<Overflow threshold of an MMZ buffer, in percentage. For example, the value 100 indicates 100%.*/ /**<CNcomment:  MMZ buffer上溢水线，按百分比设置，例如: 100 indicates 100%.*/
    HI_U32  underflow_threshold;    /**<Underflow threshold of an MMZ buffer, in percentage. For example, the value 0 indicates 0%.*/ /**<CNcomment:  MMZ buffer下溢水线，按百分比设置，例如: 0 indicates 0%.*/
}HI_MMZ_BUF_S;

typedef struct hiRECT_S
{
    HI_S32 s32X;
    HI_S32 s32Y;
    HI_S32 s32Width;
    HI_S32 s32Height;
} HI_RECT_S;

typedef enum hiLAYER_ZORDER_E
{
    HI_LAYER_ZORDER_MOVETOP = 0,  /**<Move to the top*/ /**<CNcomment:  移到最顶部 */
    HI_LAYER_ZORDER_MOVEUP,       /**<Move up*/ /**<CNcomment:  向上移到 */
    HI_LAYER_ZORDER_MOVEBOTTOM,   /**<Move to the bottom*/ /**<CNcomment:  移到最底部 */
    HI_LAYER_ZORDER_MOVEDOWN,     /**<Move down*/ /**<CNcomment:  向下移到 */
    HI_LAYER_ZORDER_BUTT
} HI_LAYER_ZORDER_E;

/** Defines user mode proc show buffer */
/**CNcomment: 用户态PROC buffer定义 */
typedef struct hiPROC_SHOW_BUFFER_S
{
    HI_U8* pu8Buf;                  /**<Buffer address*/  /**<CNcomment: Buffer地址 */
    HI_U32 u32Size;                 /**<Buffer size*/     /**<CNcomment: Buffer大小 */
    HI_U32 u32Offset;               /**<Offset*/          /**<CNcomment: 打印偏移地址 */
}HI_PROC_SHOW_BUFFER_S;

/** Proc show function */
/**CNcomment: Proc信息显示回调函数 */
typedef HI_S32 (* HI_PROC_SHOW_FN)(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);

/** Proc command function */
/**CNcomment: Proc控制回调函数 */
typedef HI_S32 (* HI_PROC_CMD_FN)(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);

/** Defines user mode proc entry */
/**CNcomment: 用户态PROC入口定义 */
typedef struct hiPROC_ENTRY_S
{
    HI_CHAR *pszEntryName;          /**<Entry name*/            /**<CNcomment: 入口文件名 */
    HI_CHAR *pszDirectory;          /**<Directory name. If null, the entry will be added to /proc/hisi directory*/
                                    /**<CNcomment: 目录名，如果为空，将创建到/proc/hisi目录下 */
    HI_PROC_SHOW_FN pfnShowProc;    /**<Proc show function*/    /**<CNcomment: Proc信息显示回调函数 */
    HI_PROC_CMD_FN pfnCmdProc;      /**<Proc command function*/ /**<CNcomment: Proc控制回调函数 */
    HI_VOID *pPrivData;             /**<Private data*/          /**<CNcomment: Buffer地址 */
}HI_PROC_ENTRY_S;

/** Defines DDR configuration type struct */
/**CNcomment: DDR 配置类型结构定义 */
typedef struct hiSYS_MEM_CONFIG_S
{
    HI_U32 u32TotalSize;    /** Total memory size(MB) */      /**<CNcomment: 总内存大小(MB)*/
    HI_U32 u32MMZSize;      /**MMZ memory size(MB) */       /** <CNcomment: MMZ内存大小(MB)*/
}HI_SYS_MEM_CONFIG_S;

#ifndef __KERNEL__
#include <time.h>

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      COMMON */
/** @{ */  /** <!-- [COMMON] */

/**
@brief Initializes the system. CNcomment: 系统初始化 CNend
@attention \n
You must call this API to initialize the system before using the APIs of all modules.
Though you can call other APIs successfully before calling this API, the subsequent operations may fail.\n
CNcomment: 在使用所有模块的接口之前都需要先调用此接口对系统进行初始化\n
在调用这个接口之前调用其他接口，不会返回失败，但是不保证执行的正确性 CNend
@param N/A CNcomment: 无 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_Init(HI_VOID);

/**
@brief Deinitializes the system. CNcomment: 系统去初始化 CNend
@attention \n
If all modules are not used, you need to call this API to deinitialize the system.\n
CNcomment: 所有模块都不再使用后调用此接口去初始化 CNend
@param N/A CNcomment: 无 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_DeInit(HI_VOID);


/**
@brief Obtains the compiled time of a version. CNcomment: 获取版本的编译时间 CNend
@attention \n
The compiled time is the time during which the common module is made again.
CNcomment: 时间为进行common模块重新make的时间 CNend
@param[out] pstTime Pointer to the compiled time of a version (output). CNcomment: 指针类型，输出版本编译的时间。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_GetBuildTime(struct tm * pstTime);


/**
@brief Obtains the version number. CNcomment: 获取版本号 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[out] pstVersion Pointer to the version number (output). CNcomment: 指针类型，输出版本号。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_GetVersion(HI_SYS_VERSION_S *pstVersion);


/**
@brief Obtains the package type. CNcomment: 获取封装类型 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[out] penPackageType Pointer to the package type (output). CNcomment: 指针类型，输出封装类型。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_GetChipPackageType(HI_CHIP_PACKAGE_TYPE_E *penPackageType);


/**
@brief Obtains the chip support attributes. CNcomment: 获取芯片支持的能力 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] enChipCap Indicate which chip capability. CNcomment: 输入参数，指明获取何种芯片能力 CNend
@param[out] pbSupport Pointer to the chip whether support the attributes(output). CNcomment: 输出参数，保存获取结果 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_GetChipCapability(HI_CHIP_CAP_E enChipCap, HI_BOOL *pbSupport);

/**
@brief Obtains the chip attributes. CNcomment: 获取芯片属性 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[out] pstChipAttr Pointer to the chip attributes(output). CNcomment: 指针类型，输出芯片属性 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_GetChipAttr(HI_SYS_CHIP_ATTR_S *pstChipAttr);

/**
@brief CRC32 function. CNcomment: CRC32功能 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] pu8Src Pointer to the source buffer. CNcomment: 指针类型，输入buffer CNend
@param[in] u32SrcLen length of the source buffer. CNcomment: 输入buffer长度 CNend
@param[out] pu32Dst Pointer to the out value. CNcomment: 输出数据指针 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_CRC32(HI_U8 *pu8Src, HI_U32 u32SrcLen, HI_U32 *pu32Dst);

/**
@brief Obtains the chip attributes. CNcomment: 获取内存配置信息 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[out] pstConfig Pointer to address for memory configuration(output). CNcomment: 指针类型，输出内存配置信息指针 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_GetMemConfig(HI_SYS_MEM_CONFIG_S *pstConfig);

/**
@brief Performs global system configuration. CNcomment: 设置系统的全局配置 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] pstSysConf Pointer to the address for system configuration CNcomment: 指针类型，系统配置指针地址。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_SetConf(const HI_SYS_CONF_S *pstSysConf);

/**
@brief Obtains global system configuration. CNcomment: 获取系统的全局配置 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[out] pstSysConf Pointer to the system configuration (output). CNcomment: 指针类型，输出系统配置。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_GetConf(HI_SYS_CONF_S *pstSysConf);

/**
@brief Sets the debugging information level of a module. CNcomment: 设置模块的调试信息级别 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] enModId Debugging ID of a module CNcomment: 模块的调试ID。 CNend
@param[in] enLogLevel Debugging information level of a module CNcomment: 模块的调试信息级别。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
*/
HI_S32 HI_SYS_SetLogLevel(HI_MOD_ID_E enModId,  HI_LOG_LEVEL_E enLogLevel);

/**
@brief Sets the debugging information file path for U-disk. CNcomment: 设置日志存储路径 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in]  pszLogPath Debugging information file path. CNcomment: 日志的存储路径 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
*/
HI_S32 HI_SYS_SetLogPath(const HI_CHAR* pszLogPath);

/**
@brief Sets the debugging files(may be stream, YUV data, image...) save path. CNcomment: 设置调试文件存储路径 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in]  pszPath Debugging files path. CNcomment: 调试文件的存储路径 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
*/
HI_S32 HI_SYS_SetStorePath(const HI_CHAR* pszPath);

/**
@brief Writes to a register or a memory. CNcomment:  写寄存器或内存 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32RegAddr Physical address of a register or a memory CNcomment: 寄存器或内存的物理地址。 CNend
@param[in] u32Value Value of a register CNcomment:  寄存器的值。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value);

/**
@brief Reads a register or a memory. CNcomment: 读寄存器或内存 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32RegAddr Physical address of a register or a memory CNcomment: 寄存器或内存的物理地址。 CNend
@param[out] pu32Value Pointer to the register value (output) CNcomment:  指针类型，输出寄存器的值。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value);

/**
@brief Map registers address. CNcomment:  映射寄存器地址 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32RegAddr The start physical address of registers. CNcomment: 寄存器的起始物理地址。 CNend
@param[in] u32Length  Length of the registers want to map CNcomment:   寄存器长度。 CNend
@param[out] pVirAddr  Virtual address CNcomment:   映射后的虚拟地址。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_MapRegister(HI_U32 u32RegAddr, HI_U32 u32Length, HI_VOID *pVirAddr);

/**
@brief Unmap registers address. CNcomment:  解除寄存器地址映射 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] pVirAddr The virtual address to be unmapped CNcomment: 要解除映射的虚拟地址。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_UnmapRegister(HI_VOID *pVirAddr);

/**
@brief Get timestamp. CNcomment: 获取时间戳。 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[out] pu32TimeMs Pointer to the timestamp value (output) CNcomment: 输出时间戳。 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_SYS_GetTimeStampMs(HI_U32 *pu32TimeMs);

/**
@brief Applies for a media memory zone (MMZ) and maps the user-state address.
CNcomment:  申请mmz内存，并映射用户态地址 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in/out] pstBuf Structure of the buffer information. Bufname and bufsize are inputs, and the physical address and user-state virtual address are outputs.
                    CNcomment: buffer信息结构，bufname和bufsize作为输入,物理地址和用户态虚拟地址作为输出 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_MMZ_Malloc(HI_MMZ_BUF_S *pstBuf);

/**
@brief Unmaps the user-state address and releases the MMZ. CNcomment: 解除用户态地址的映射，并释放mmz内存 CNend
@attention \n
Ensure that the lengths of the transferred physical address and user-state virtual address are correct.
CNcomment: 保证传入的物理地址、用户态虚拟地址和长度正确 CNend
@param[in] pstBuf Structure of the buffer information CNcomment: buffer信息结构 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_MMZ_Free(HI_MMZ_BUF_S *pstBuf);

/**
@brief pplies for an MMZ with a specified name and obtains its physical address. CNcomment: 指定mmz的名字申请mmz内存，返回物理地址 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32Size Buffer size CNcomment: buffer大小 CNend
@param[in] u32Align Alignment mode CNcomment: 对齐方式 CNend
@param[in] ps8MMZName Name of an MMZ in the buffer. If the MMZ name is set to NULL, an MMZ is anonymously applied for. CNcomment: buffer分区的名字，传入NULL匿名申请 CNend
@param[in] ps8BufName Buffer name CNcomment: buffer块的名字 CNend
@retval ::NULL The application fails. CNcomment: 申请失败 CNend
@retval Physical address CNcomment: 物理地址 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_VOID *HI_MMZ_New(HI_U32 u32Size , HI_U32 u32Align, HI_CHAR *ps8MMZName, HI_CHAR *ps8BufName);


/**
@brief Releases an MMZ based on its physical address. CNcomment: 通过物理地址释放mmz内存 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: buffer物理地址 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_MMZ_Delete(HI_U32 u32PhysAddr);

/**
@brief Maps the physical address of an MMZ applied for to a user-state virtual address. You can determine whether to cache the address.
CNcomment: 将mmz申请的物理地址映射成用户态虚拟地址，可以指定是否cached CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: buffer物理地址 CNend
@param[in] u32Cached Whether to cache the address. 0: no; 1: yes CNcomment: 是否使用cache，0不使用，1使用 CNend
@retval ::NULL The application fails. CNcomment: 申请失败 CNend
@retval User-state virtual address CNcomment: 用户态虚地址 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_VOID *HI_MMZ_Map(HI_U32 u32PhysAddr, HI_U32 u32Cached);

/**
@brief Unmaps the user-state address of an MMZ. CNcomment: 解除mmz内存用户态地址的映射 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: buffer物理地址 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_MMZ_Unmap(HI_U32 u32PhysAddr);


/**
@brief Flushes D-cache to the cached MMZ. CNcomment: 对于cached类型MMZ，刷Dcache到内存 CNend
@attention \n
If the value 0 is transferred, all D-caches are refreshed; otherwise, only the transferred memory is refreshed.
CNcomment: 如果传入0，则刷新所有的Dcache；否则只刷传入的那块内存 CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: buffer物理地址 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_MMZ_Flush(HI_U32 u32PhysAddr);


/**
@brief Maps a physical address to a user-state virtual address. CNcomment: 将物理地址映射成用户态虚拟地址 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32PhyAddr Physical address of a buffer CNcomment: buffer物理地址 CNend
@param[in] u32Size Buffer size CNcomment: buffer的大小 CNend
@retval ::NULL The application fails. CNcomment: 申请失败 CNend
@retval User-state virtual address CNcomment: 用户态虚地址 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_VOID *HI_MEM_Map(HI_U32 u32PhyAddr, HI_U32 u32Size);


/**
@brief Unmaps a user-state address. CNcomment: 解除用户态地址的映射 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] pAddrMapped User-state virtual address of a buffer. CNcomment: buffer的用户态虚地址 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_MEM_Unmap(HI_VOID *pAddrMapped);

/**
@brief Obtains the physical address and size based on the virtual address. CNcomment: 根据虚拟地址获取物理地址，以及大小 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] pVir User-state virtual address CNcomment: 用户态虚地址 CNend
@param[out] pu32Phyaddr Physical address  CNcomment: 物理地址 CNend
@param[out] pu32Size Size CNcomment: 大小 CNend
@retval ::HI_SUCCESS Success CNcomment: 成功 CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API系统调用失败 CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_MMZ_GetPhyaddr(HI_VOID * pVir, HI_U32 *pu32Phyaddr, HI_U32 *pu32Size);

/**
@brief Register one module to manager. CNcomment:模块注册，用于管理 CNend
@attention Before manager someone module, calling this interface. CNcomment:如需管理模块，用此接口先注册 CNend
@param[in] pszModuleName The module name CNcomment:模块名称 CNend
@param[in] u32ModuleID   The module ID. CNcomment:模块ID CNend
@retval ::HI_SUCCESS Success CNcomment:成功 CNend
@retval ::HI_FAILURE Failure CNcomment:失败 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_S32 HI_MODULE_Register(HI_U32 u32ModuleID, const HI_CHAR * pszModuleName);

/**
@brief Register one moudle by name. CNcomment:模块注册，ID由系统分配 CNend
@attention Before manager someone module, calling this interface. CNcomment:如需管理模块，用此接口先注册 CNend
@param[in] pszModuleName The module name CNcomment:模块名称 CNend
@param[out] pu32ModuleID The module id allocated by system. CNcomment:系统分配的模块ID CNend
@retval ::HI_SUCCESS Success CNcomment:成功 CNend
@retval ::HI_FAILURE Failure CNcomment:失败 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_S32 HI_MODULE_RegisterByName(const HI_CHAR * pszModuleName, HI_U32* pu32ModuleID);

/**
@brief UnRegister one module to trace. CNcomment:模块移除 CNend
@attention Before stopping to manage someone module, calling this interface. CNcomment:不需要管理此模块时，使用此接口移除模块 CNend
@param[in] u32ModuleID The module ID. CNcomment:模块ID CNend
@param[out] None CNcomment:无 CNend
@retval ::HI_SUCCESS Success CNcomment:成功 CNend
@retval ::HI_FAILURE Failure CNcomment:失败 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_S32 HI_MODULE_UnRegister(HI_U32 u32ModuleID);

/**
@brief User mode proc cretea directory. CNcomment:用户态proc创建目录 CNend
@attention You need register module before calling this API. Only support create one level directory. CNcomment:需要先注册模块，只支持创建一级目录 CNend
@param[in] pszName The directory name. CNcomment:目录名 CNend
@param[out] None CNcomment:无 CNend
@retval ::HI_SUCCESS Success CNcomment:成功 CNend
@retval ::HI_FAILURE Failure CNcomment:失败 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_S32 HI_PROC_AddDir(const HI_CHAR *pszName);

/**
@brief User mode proc remove directory. CNcomment:用户态proc删除目录 CNend
@attention It will return fail if there are entries in the directory. CNcomment:如果目录下还有入口文件,将会删除失败 CNend
@param[in] pszName The directory name. CNcomment:目录名 CNend
@retval ::HI_SUCCESS Success CNcomment:成功 CNend
@retval ::HI_FAILURE Failure CNcomment:失败 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_S32 HI_PROC_RemoveDir(const HI_CHAR *pszName);

/**
@brief User mode proc add entry. CNcomment:用户态proc创建入口 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32ModuleID Module ID. CNcomment:模块ID CNend
@param[in] pstEntry Parameter of entry. CNcomment:创建入口参数 CNend
@retval ::HI_SUCCESS Success CNcomment:成功 CNend
@retval ::HI_FAILURE Failure CNcomment:失败 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_S32 HI_PROC_AddEntry(HI_U32 u32ModuleID, const HI_PROC_ENTRY_S* pstEntry);

/**
@brief User mode proc remove entry. CNcomment:用户态proc删除入口 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] u32ModuleID Module ID. CNcomment:模块ID CNend
@param[in] pstEntry Parameter of entry. CNcomment:删除入口参数 CNend
@retval ::HI_SUCCESS Success CNcomment:成功 CNend
@retval ::HI_FAILURE Failure CNcomment:失败 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_S32 HI_PROC_RemoveEntry(HI_U32 u32ModuleID, const HI_PROC_ENTRY_S* pstEntry);

/**
@brief User mode proc print function. CNcomment:用户态proc打印内容的函数 CNend
@attention \n
N/A CNcomment: 无 CNend
@param[in] pstBuf Output buffer parameter. CNcomment:输出buffer参数 CNend
@param[in] pFmt   Format parameter. CNcomment:打印格式化参数 CNend
@retval ::HI_SUCCESS Success CNcomment:成功 CNend
@retval ::HI_FAILURE Failure CNcomment:失败 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_S32 HI_PROC_Printf(HI_PROC_SHOW_BUFFER_S *pstBuf, const HI_CHAR *pFmt, ...);

/**
@brief malloc the pointed size from system heap. CNcomment:从系统中分配指定大小的内存 CNend
@attention None CNcomment:无 CNend
@param[in] u32ModuleID The module ID, who need to request memory. CNcomment:模块ID CNend
@param[in] u32Size The size of requesting. CNcomment:请求分配的大小，单位是字节 CNend
@param[out] None CNcomment:无 CNend
@retval ::Valid memory address Success CNcomment:成功返回分配到的空间首地址 CNend
@retval ::NULL Failure CNcomment:失败返回NULL CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_VOID* HI_MEM_Malloc(HI_U32 u32ModuleID, HI_U32 u32Size);


/**
@brief Free the requsted memory by hi_malloc. CNcomment:释放分配的内存 CNend
@attention when stopping to use the memory, calling this interface. CNcomment:不再需要这块内存时，使用此接口进行释放 CNend
@param[in] u32ModuleID The module ID, who need to free memory. CNcomment:模块ID CNend
@param[in] pMemAddr The memory address to free CNcomment:释放空间的首地址 CNend
@param[out] None CNcomment:无 CNend
@retval ::None CNcomment:无 CNend
@see \n
N/A CNcomment:无 CNend
*/
HI_VOID HI_MEM_Free(HI_U32 u32ModuleID, HI_VOID* pMemAddr);

/**
@brief Calloc memory, with u32MemBlock blocks and u32Size size per. CNcomment:分配连续大小的内存块 CNend
@attention None CNcomment:无 CNend
@param[in] u32ModuleID The module id, who need to calloc memory. CNcomment:模块ID CNend
@param[in] u32MemBlock The requesting block number. CNcomment:分配的块数 CNend
@param[in] u32Size The requesting size per block. CNcomment:每块的大小，单位是字节 CNend
@param[out] None CNcomment:无 CNend
@retval ::Valid memory address Success CNcomment:成功则返回分配到的内存首地址 CNend
@retval ::NULL Failure CNcomment:失败返回NULL CNend
@see \n
N/A CNcomment: 无 CNend
*/
HI_VOID* HI_MEM_Calloc(HI_U32 u32ModuleID, HI_U32 u32MemBlock, HI_U32 u32Size);


/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* endif __KERNEL__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_COMMON_H__ */

