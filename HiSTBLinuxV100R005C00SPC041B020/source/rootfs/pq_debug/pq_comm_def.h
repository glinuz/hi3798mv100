/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : Hi_comm_def.h
  Version       : Initial Draft
  Author        : z00232460
  Created       : 2013/10/17
  Description   :comm struct definations
  History       :
  1.Date        : 2013/10/17
    Author      : z00232460
    Modification: This file is created.
******************************************************************************/

#ifndef __HI_COMM_DEF_H__
#define __HI_COMM_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "hi_type.h"

#include <unistd.h>
#include <errno.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "hi_common.h"
#include "hi_debug.h"
#include "hi_mpi_pq.h"


#pragma pack(1)

#define BACKLOG 20
#define CMDBUF_SIZE (0x2FFFF + 0x100)
#define OFFSET_OF(s, m) (size_t)&(((s *)0)->m)
#define GET_VALUE(buf, s, m) ((*(s *)buf).m)
#define CMD_OFFSET 4
#define VIRTUAL_REG_HEAD 0xFF000000
#define ECHO_BYE 0xB0
#define PQ_CFG_FILE "hipq.cfg"
#define PQ_PARSE_ISVIRREG 1
#define PQ_PARSE_ISREG 0
#define MAXLEN 32


/*校验指针*/
#define CHECK_PTR_REINT(p) \
    do {\
        if (NULL == p) return HI_FAILURE;\
    } while (0)

/*校验返回值*/
#define CHECK_RETURN(ret) \
    do {\
        if (HI_FAILURE == ret) return HI_FAILURE;\
    } while (0)

typedef struct
{
    HI_U32	u32GammCoef[1024];
} HI_DBG_CSC_GAMM_PARA_S;


/*消息数据包*/
typedef struct hiMDBG_MSG
{
    HI_U32 u32Datalen;                       /*整个数据包长度*/
    HI_U8  au8MagicNum[4];                   /*魔数”HIPQ”*/
    HI_U8  au32ChipVersion[16];              /*芯片型号3518*/
    HI_U8  au32PQVersion[16];                /*PQ版本V1B01*/
    HI_U32 u32CMD;                           /*指令码*/
    HI_U32 u32RegType;                       /*地址类型（0x00=物理 0x01=虚拟）*/
    HI_U32 u32Addr;                          /*地址值*/
    HI_U32 u32Len;                           /*数据长*/
    HI_U8  au8Para[32];                      /*参数区*/
    HI_U8* pu8Data;                          /*有效数据*/
    HI_U32 u32DataCheckSum;                  /*数据包校验和*/
} HI_MDBG_MSG_S;

/* ECHO应答错误码 */
typedef enum hiDBG_ECHO
{
    HI_MAGIC_ERROR = 0xE0,                   /*魔术校验错误*/
    HI_CHECKSUM_ERROR = 0xE1,                /*数据校验错误*/
    HI_CHIP_ERROR = 0xE2,                    /*芯片版本不匹配*/
    HI_VERSION_ERROR = 0xE3,                 /*版本不匹配*/
    HI_INVALID_CMD = 0xE4,                   /*命令错误*/
    HI_INVALID_ADDRESS_TYPE = 0xE5,          /*无效地址类型*/
    HI_LEN_ERROR = 0xE6,                     /*数据长度校验失败*/
    HI_RUN_ERROR = 0xE7,                     /*板端处理失败，无法完成请求*/
    HI_NOT_SUPPORTED = 0xE8,              	 /*板端不支持此操作*/
    HI_BOARD_PROCESS_ERROR = 0xEE,	         /*板端处理失败，无法完成请求*/
    HI_MDBG_ECHO_BUTT
} HI_DBG_ECHO_E;


/* PQ调试命令 */
typedef enum hiPQ_DBG_CMD
{
    PQ_CMD_READ_REG = 0,
    PQ_CMD_WRITE_REG = 1,
    PQ_CMD_READ_MATRIX = 8,
    PQ_CMD_WRITE_MATRIX = 9,
    PQ_CMD_READ_REGS = 10,
    PQ_CMD_WRITE_REGS = 11,
    PQ_CMD_READ_GAMMA = 12,
    PQ_CMD_WRITE_GAMMA = 13,
    PQ_CMD_READ_ACM = 14,
    PQ_CMD_WRITE_ACM = 15,
    PQ_CMD_READ_ACC = 16,
    PQ_CMD_WRITE_ACC = 17,
    PQ_CMD_EXPORT_BIN = 18,
    PQ_CMD_IMPORT_BIN = 19,
    PQ_CMD_FIX_BIN = 20,
    PQ_CMD_READ_YUV =  101,
    PQ_CMD_READ_RAW = 102,
    PQ_CMD_READ_DCI_LUT = 110,
    PQ_CMD_WRITE_DCI_LUT = 111,
    PQ_CMD_READ_HIST = 112,

    PQ_CMD_READ_SNR_MEAN_RATIO = 113,
    PQ_CMD_READ_SNR_EDGE_STR = 114,
    PQ_CMD_WRITE_SNR_MEAN_RATIO = 115,
    PQ_CMD_WRITE_SNR_EDGE_STR = 116,

    PQ_CMD_READ_TNR_PIXMEAN_TO_RATIO_LUMA = 117,
    PQ_CMD_READ_TNR_PIXMEAN_TO_RATIO_CHROMA = 118,
    PQ_CMD_READ_TNR_MOTION_MAPPING_LUMA = 119,
    PQ_CMD_READ_TNR_MOTION_MAPPING_CHROMA = 120,
    PQ_CMD_READ_TNR_FINAL_MOTION_MAPPING_LUMA = 121,
    PQ_CMD_READ_TNR_FINAL_MOTION_MAPPING_CHROMA = 122,
    PQ_CMD_WRITE_TNR_PIXMEAN_TO_RATIO_LUMA = 123,
    PQ_CMD_WRITE_TNR_PIXMEAN_TO_RATIO_CHROMA = 124,
    PQ_CMD_WRITE_TNR_MOTION_MAPPING_LUMA = 125,
    PQ_CMD_WRITE_TNR_MOTION_MAPPING_CHROMA = 126,
    PQ_CMD_WRITE_TNR_FINAL_MOTION_MAPPING_LUMA = 127,
    PQ_CMD_WRITE_TNR_FINAL_MOTION_MAPPING_CHROMA = 128,
    PQ_CMD_READ_FMOTION_MAPPING = 139, 
    PQ_CMD_WRITE_FMOTION_MAPPING = 140,

	PQ_CMD_READ_HDR_TM_LUT = 143, 
    PQ_CMD_WRITE_HDR_TM_LUT = 144,

    PQ_CMD_BUTT,
} PQ_DBG_CMD_E;

/* 卖场模式 */
typedef enum hiPQ_DBG_DEMO_E
{
    PQ_DBG_DEMO_SHARPNESS = 0,
    PQ_DBG_DEMO_DCI,
    PQ_DBG_DEMO_COLOR,
    PQ_DBG_DEMO_SR,
    PQ_DBG_DEMO_ALL,
    PQ_DBG_DEMO_ALL_OFF,

    PQ_DBG_DEMO_BUTT
} PQ_DBG_DEMO_E;

/*PQ Source Mode*/
typedef enum hiPQ_DBG_SOURCE_MODE_E
{
    PQ_DBG_SOURCE_MODE_NO = 0,
    PQ_DBG_SOURCE_MODE_SD,
    PQ_DBG_SOURCE_MODE_HD,
    PQ_DBG_SOURCE_MODE_UHD, 
    PQ_DBG_SOURCE_MODE_BUTT, 
} PQ_DBG_SOURCE_MODE_E;

/*PQ Output Mode*/
typedef enum hiPQ_DBG_OUTPUT_MODE_E
{
    PQ_DBG_OUTPUT_MODE_NO = 0,
    PQ_DBG_OUTPUT_MODE_SD,
    PQ_DBG_OUTPUT_MODE_HD,
    PQ_DBG_OUTPUT_MODE_UHD, 
    PQ_DBG_OUTPUT_MODE_BUTT, 
} PQ_DBG_OUTPUT_MODE_E;


/*读取寄存器的类型*/
typedef enum hiPQ_DBG_READ_TYPE_E
{
    PQ_DBG_READ_TYPE_PHY = 0,/* 物理寄存器*/
    PQ_DBG_READ_TYPE_BIN,    /* PQbin */
    PQ_DBG_READ_TYPE_BUTT, 
} PQ_DBG_READ_TYPE_E;


/* 参数比特范围*/
typedef struct tagPQ_DBG_BIT_RANGE
{
    HI_U32 u32OrgBit;
    HI_U32 u32EndBit;
    HI_U32 u32Signed;
} PQ_DBG_BIT_RANGE_S;

/*PQ 命令实现结构*/
typedef struct hiPQ_DBG_CMD_FUN_S
{
    PQ_DBG_CMD_E enDbgCmd;  /* PQ内部命令, 参考PQ_DBG_CMD_E */
    HI_S32 (*pfCmdFunction)(HI_S32 client_sockfd, HI_U8* pu8Buffer); /*PQ CMD对应的实现函数*/
} PQ_DBG_CMD_FUN_S;

/*PQ 虚拟寄存器实现结构*/
typedef struct hiPQ_DBG_REG_TYPE_S
{
    HI_U32 u32CmdRegType;    /* PQ 虚拟寄存器类型, 参考PQ_DBG_REG_TYPE_E */
    HI_S32 (*pfRegTypeFun)(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen); /*虚拟寄存器类型对应的实现函数*/
} PQ_DBG_REG_TYPE_S;

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __HI_COMM_DEF_H__ */
