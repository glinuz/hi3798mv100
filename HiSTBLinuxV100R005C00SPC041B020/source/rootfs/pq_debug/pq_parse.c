/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************/

/*-----------------------------------------------------------------------------*/
/* Include Header Files                                                        */
/*-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include "hi_debug.h"
#include "hi_common.h"
#include "hi_mpi_mem.h"
#include "pq_comm.h"
#include "pq_comm_def.h"


extern HI_BOOL g_bPQInitFlag;
extern HI_S32  g_s32PqFd;
/*-----------------------------------------------------------------------------*/
/* Local Constant Definitions                                                  */
/*-----------------------------------------------------------------------------*/
#define HI_PQ_VIRTUAL_REG_HEAD           0xFFFF0000

/*-----------------------------------------------------------------------------*/
/* Local Types Declarations                                                    */
/*-----------------------------------------------------------------------------*/
static HI_S32 PQ_DBG_ReadVirtualReg(HI_U32 u32RegAddr, HI_U32* pu32Data, HI_U32 u32Len);
static HI_S32 PQ_DBG_WriteVirtualReg(HI_U32 u32RegAddr, HI_U32* u32Data, HI_U32 u32Len);
static HI_S32 HI_PQ_ReadReg(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_WriteReg(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_ReadRegs(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_WriteRegs(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_ReadDCIHistgram(HI_S32 client_sockfd, HI_U8* pu8Buffer);
//static HI_S32 HI_PQ_ReadDCI(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_WriteACM(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_SetTNRLumaPixMean2Ratio(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_GetTNRLumaPixMean2Ratio(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_SetTNRChromPixMean2Ratio(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_GetTNRChromPixMean2Ratio(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_SetTNRLumaMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_GetTNRLumaMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_SetTNRChromMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_GetTNRChromMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_SetTNRLumaFinalMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_GetTNRLumaFinalMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_SetTNRChromFinalMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_GetTNRChromFinalMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_SetTNRFMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);
static HI_S32 HI_PQ_GetTNRFMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer);


/*-----------------------------------------------------------------------------*/
/* Extern Global Variables                                                     */
/*-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------*/
/* Interface Functions                                                         */
/*-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*/
/* Local Function Protype                                                      */
/*-----------------------------------------------------------------------------*/

HI_S32 PQ_Parse_SendErrorFlag(HI_S32 client_sockfd, HI_U8 error)
{
    HI_S32 s32Ret;
    HI_U8 u8ACK = 0;
    u8ACK = error;
    s32Ret = send (client_sockfd, (void*)&u8ACK, 1, 0);
    while (errno == EINTR || errno == EAGAIN)
    {
        usleep(10);
        s32Ret = send (client_sockfd, (void*)&u8ACK, 1, 0);
    }
    if (s32Ret < 0 && (s32Ret == EINTR || s32Ret == EWOULDBLOCK || s32Ret == EAGAIN))
    {
        HI_ERR_PQ("send error msg failed!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 PQ_Parse_RecvData(HI_S32 client_sockfd, HI_U8* pu8Buf, HI_U32 u32Count, HI_S32* ps32ReadSize)
{
    HI_S32 u32Temp = -1;
    fd_set read_fds;
    struct timeval timeout;
    HI_S32 s32Ret = 0;
    HI_U32 u32Len = 0;
    HI_U32 isRecvLen = 0;

    if (NULL == pu8Buf)
    {
        HI_ERR_PQ("pu8Buf is null!\n");
        return HI_FAILURE;
    }

    FD_ZERO(&read_fds);
    FD_SET(client_sockfd, &read_fds);

    timeout.tv_sec	= 5;
    timeout.tv_usec = 0;
    s32Ret = select(client_sockfd + 1, &read_fds, NULL, NULL, &timeout);
    if (s32Ret < 0)
    {
        HI_ERR_PQ("socketfd select failed!\n");
        return HI_FAILURE;
    }
    if (s32Ret == 0)
    {
        HI_ERR_PQ("time out!\n");
        return HI_FAILURE;
    }

    if (FD_ISSET(client_sockfd, &read_fds))//测试sock是否可读，即网络上是否有数据
    {
        s32Ret = recv(client_sockfd, pu8Buf, 4, MSG_WAITALL); //前四个字节代表包长
        while (errno == EINTR || errno == EAGAIN  )
        {
            usleep(10);
            s32Ret = recv(client_sockfd, pu8Buf, 4, MSG_WAITALL);
            if (s32Ret >= 0)
            {
                break;
            }
        }

        u32Len = GET_VALUE(pu8Buf, HI_MDBG_MSG_S, u32Datalen);

        if (0 == u32Len)
        {
            close(client_sockfd);
            HI_INFO_PQ("connection close!\n");
            pthread_exit(NULL);
        }

        if (u32Len >= u32Count)
        {
            HI_ERR_PQ("recv data is null the len is too large!\n", u32Len);
            return HI_FAILURE;
        }


        while (u32Len > isRecvLen)
        {
            u32Temp =  recv(client_sockfd, pu8Buf + 4, u32Len, MSG_WAITALL);
            if ((u32Temp < 0))
            {
                if (errno == EINTR || errno == EAGAIN)
                {
                    usleep(10);
                    continue;
                }
                else
                {

                    HI_ERR_PQ("recv data is null!\n");
                    return HI_FAILURE;
                }
            }

            isRecvLen += u32Temp;
        }
    }
    *ps32ReadSize = u32Len + 4;

    return HI_SUCCESS;
}




/*读虚拟寄存器*/
HI_S32 PQ_DBG_ReadVirtualReg(HI_U32 u32RegAddr, HI_U32* pu32Data, HI_U32 u32Len)
{
    HI_INFO_PQ("pqtools:ReadVirtualReg = %x,len = %d\n", u32RegAddr, u32Len);
    return PQ_DRV_ReadFromPqDriver(u32RegAddr, pu32Data, u32Len);
}

/*写虚拟寄存器*/
HI_S32 PQ_DBG_WriteVirtualReg(HI_U32 u32RegAddr, HI_U32* u32Data, HI_U32 u32Len)
{
    HI_INFO_PQ("pqtools:WriteVirtualReg = %x,len = %d\n", u32RegAddr, u32Len);
    return PQ_DRV_WriteToPqDriver(u32RegAddr, u32Data, u32Len);
}

HI_S32 WriteData(HI_S32 client_sockfd, HI_U8* pu8Buf, HI_S32 s32Count)
{
    HI_S32 s32RetVal = 0;

    if (NULL ==  pu8Buf)
    {
        HI_ERR_PQ("send data is null!\n");
        return HI_FAILURE;
    }
    s32RetVal = send (client_sockfd, pu8Buf, s32Count, 0);

    while (errno == EINTR || errno == EAGAIN)
    {
        usleep(10);
        s32RetVal = send(client_sockfd, pu8Buf, s32Count, 0);
    }
    if ( s32RetVal == -1 && (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
    {
        HI_ERR_PQ("send data failed!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HI_CalCheckSum(HI_U8* pu8Data, HI_U32 u32Conunt, HI_U32* pu32CheckSum)
{
    HI_U32 i;

    if (NULL ==  pu32CheckSum)
    {
        HI_ERR_PQ("pu32CheckSum is null!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < u32Conunt; i++)
    {
        *pu32CheckSum += pu8Data[i];
    }
    return HI_SUCCESS;
}


HI_S32 HI_SendData(HI_S32 client_sockfd, HI_U8* pu8Data, HI_U32 u32Conunt)
{
    HI_U32 u32CheckSum = 0;
    HI_U32 u32CountSum = 0;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8SendData  = HI_NULL;
    HI_U32 u32SendLen   = 0;

    if (NULL == pu8Data)
    {
        HI_ERR_PQ("pu8Data is null!\n");
        return HI_FAILURE;
    }
    HI_U8 u8Bye = ECHO_BYE;

    HI_CalCheckSum(pu8Data, u32Conunt, &u32CheckSum);
    u32CountSum = u32Conunt + sizeof(u32CheckSum);
    u32SendLen = u32Conunt + 9;
    pu8SendData = (HI_U8*)HI_MALLOC(HI_ID_PQ, u32SendLen);
    if (NULL == pu8SendData)
    {
        HI_ERR_PQ("MALLOC error\n");
        return HI_FAILURE;
    }
    memset(pu8SendData, 0, u32SendLen);
    memcpy(pu8SendData, &u8Bye, 1);
    memcpy(pu8SendData + 1, (HI_U8* )&u32CountSum, 4);
    memcpy(pu8SendData + 5, pu8Data, u32Conunt);

    memcpy(pu8SendData + 5 + u32Conunt, (HI_U8* )&u32CheckSum, sizeof(u32CheckSum));
    s32Ret = WriteData(client_sockfd, pu8SendData, u32SendLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send checksum error!\n");
        HI_FREE(HI_ID_PQ, pu8SendData);
        pu8SendData = NULL;
        return HI_FAILURE;
    }

    HI_FREE(HI_ID_PQ, pu8SendData);
    pu8SendData = NULL;

    return HI_SUCCESS;
}

HI_S32 HI_SendEnd(HI_S32 client_sockfd, HI_U8 cn)
{
    HI_S32 s32Ret;

    s32Ret = send (client_sockfd, (void*)&cn, 1, 0);
    while (errno == EINTR || errno == EAGAIN)
    {
        usleep(10);
        s32Ret = send (client_sockfd, (void*)&cn, 1, 0);
    }

    if (HI_SUCCESS != s32Ret && (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
    {
        HI_ERR_PQ("send end failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*判断是否虚拟寄存器*/
HI_BOOL HI_PQ_DBG_CheckIsVirtualReg(HI_U32 u32RegAddr)
{
    HI_BOOL bRet = HI_FALSE;

    if ((u32RegAddr & HI_PQ_VIRTUAL_REG_HEAD) == HI_PQ_VIRTUAL_REG_HEAD)
    {
        bRet = HI_TRUE;
    }
    else
    {
        bRet = HI_FALSE;
    }

    return bRet;
}

/*PQ CMD对应实现函数*/
static HI_S32 HI_PQ_ReadReg(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Value = 0;
    HI_U32 u32RegAddr = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Addr);
    HI_U32 u32AddrType = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32RegType);
    HI_U8* pu8ParaRegion = HI_NULL;
    PQ_DBG_BIT_RANGE_S stBitRange;

    if (PQ_PARSE_ISVIRREG == u32AddrType) //判断是否是虚拟寄存器
    {
        //read virtual register
        if (HI_FALSE == HI_PQ_DBG_CheckIsVirtualReg(u32RegAddr))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_INVALID_ADDRESS_TYPE);
            HI_ERR_PQ("Reg address[0x%x] type error\n", u32RegAddr);
            return HI_FAILURE;
        }

        if (HI_SUCCESS != PQ_DBG_ReadVirtualReg(u32RegAddr, &u32Value, sizeof(HI_U32)))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_BOARD_PROCESS_ERROR);
            HI_ERR_PQ("Read virtual Reg [0x%x] error\n", u32RegAddr);
            return HI_FAILURE;
        }

    }
    else
    {
        pu8ParaRegion = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, au8Para);
        stBitRange.u32OrgBit = GET_VALUE(pu8ParaRegion, PQ_DBG_BIT_RANGE_S, u32OrgBit);
        stBitRange.u32EndBit = GET_VALUE(pu8ParaRegion, PQ_DBG_BIT_RANGE_S, u32EndBit);
        stBitRange.u32Signed = GET_VALUE(pu8ParaRegion, PQ_DBG_BIT_RANGE_S, u32Signed);

        if (HI_SUCCESS != PQ_DRV_ReadReg(u32RegAddr, &u32Value, &stBitRange))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_BOARD_PROCESS_ERROR);
            HI_ERR_PQ("Read Reg [0x%x] error\n", u32RegAddr);
            return HI_FAILURE;
        }
    }

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&u32Value, sizeof(HI_U32));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*PQ CMD对应实现函数*/
static HI_S32 HI_PQ_ReadRegs(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret;
    HI_U32* pu32Value = NULL;
    HI_U32 u32RegAddr = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Addr);
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_U32 u32AddrType = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32RegType);

    pu32Value = (HI_U32*)HI_MALLOC(HI_ID_PQ, u32MsgLen);
    if (NULL == pu32Value)
    {
        HI_ERR_PQ("MALLOC failed\n");
        return HI_FAILURE;
    }
    memset(pu32Value, 0, u32MsgLen);


    if (PQ_PARSE_ISVIRREG == u32AddrType) //判断是否是虚拟寄存器
    {
        //read virtual register
        if (HI_FALSE == HI_PQ_DBG_CheckIsVirtualReg(u32RegAddr))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_INVALID_ADDRESS_TYPE);
            HI_ERR_PQ("Reg address[0x%x] type error\n", u32RegAddr);
            HI_FREE(HI_ID_PQ, pu32Value);
            pu32Value = NULL;
            return HI_FAILURE;
        }

        if (HI_SUCCESS != PQ_DBG_ReadVirtualReg(u32RegAddr, pu32Value, u32MsgLen))
        {
            HI_FREE(HI_ID_PQ, pu32Value);
            pu32Value = NULL;
            return HI_FAILURE;
        }
    }
    else
    {
        HI_U32* pRead = pu32Value;
        HI_U32 u32Offset = 0;
        while (u32Offset < u32MsgLen)
        {
            if (HI_SUCCESS != HI_SYS_ReadRegister(u32RegAddr, pRead))
            {
                HI_FREE(HI_ID_PQ, pu32Value);
                return HI_FAILURE;
            }
            u32RegAddr += sizeof(HI_U32);
            u32Offset += sizeof(HI_U32);
            pRead++;
        }
    }

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)pu32Value, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        HI_FREE(HI_ID_PQ, pu32Value);
        pu32Value = NULL;
        return HI_FAILURE;
    }

    HI_FREE(HI_ID_PQ, pu32Value);
    pu32Value = NULL;

    return HI_SUCCESS;
}

/*PQ CMD对应实现函数*/
static HI_S32 HI_PQ_WriteReg(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Value = 0;
    HI_U32 u32RegAddr = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Addr);
    HI_U32 u32AddrType = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32RegType);
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_U8* pu8ParaRegion = HI_NULL;
    PQ_DBG_BIT_RANGE_S stBitRange;

    memcpy(&u32Value, pu8ValidData, sizeof(HI_U32));

    if (PQ_PARSE_ISVIRREG == u32AddrType) //判断是否是虚拟寄存器
    {
        //read virtual register
        if (HI_FALSE == HI_PQ_DBG_CheckIsVirtualReg(u32RegAddr))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_INVALID_ADDRESS_TYPE);
            HI_ERR_PQ("Reg address[0x%x] type error\n", u32RegAddr);
            return HI_FAILURE;
        }

        if (HI_SUCCESS != PQ_DBG_WriteVirtualReg(u32RegAddr, &u32Value, sizeof(HI_U32)))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_BOARD_PROCESS_ERROR);
            HI_ERR_PQ("Write virtual Reg [0x%x] error\n", u32RegAddr);
            return HI_FAILURE;
        }

        HI_INFO_PQ("pqtools:Write Virtual Reg [0x%x],Value = 0x%x\n", u32RegAddr, u32Value);
    }
    else
    {
        pu8ParaRegion = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, au8Para);
        stBitRange.u32OrgBit = GET_VALUE(pu8ParaRegion, PQ_DBG_BIT_RANGE_S, u32OrgBit);
        stBitRange.u32EndBit = GET_VALUE(pu8ParaRegion, PQ_DBG_BIT_RANGE_S, u32EndBit);

        if (HI_SUCCESS != PQ_DRV_WriteReg(u32RegAddr, u32Value, &stBitRange))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_BOARD_PROCESS_ERROR);
            HI_ERR_PQ("Write Reg [0x%x] error\n", u32RegAddr);
            return HI_FAILURE;
        }

        HI_INFO_PQ("pqtools:Write Reg [0x%x],Value = 0x%x\n", u32RegAddr, u32Value);
    }


    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return HI_SUCCESS;
}

/*PQ CMD对应实现函数*/
static HI_S32 HI_PQ_WriteRegs(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32AddrOffset = 0;

    HI_U32 u32RegAddr = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Addr);
    HI_U32 u32AddrType = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32RegType);
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_U32* pu32Value = (HI_U32*)pu8ValidData;

    HI_INFO_PQ("Write regs data addr = %x,len = %d !\n", u32RegAddr, u32MsgLen);
    if (PQ_PARSE_ISVIRREG == u32AddrType) //判断是否是虚拟寄存器
    {
        //read virtual register
        if (HI_FALSE == HI_PQ_DBG_CheckIsVirtualReg(u32RegAddr))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_INVALID_ADDRESS_TYPE);
            HI_ERR_PQ("Reg address[0x%x] type error\n", u32RegAddr);
            return HI_FAILURE;
        }

        if (HI_SUCCESS != PQ_DBG_WriteVirtualReg(u32RegAddr, pu32Value, u32MsgLen))
        {
            PQ_Parse_SendErrorFlag(client_sockfd, HI_BOARD_PROCESS_ERROR);
            HI_ERR_PQ("Write virtual Reg [0x%x] error\n", u32RegAddr);
            return HI_FAILURE;
        }

        HI_INFO_PQ("pqtools:Write Virtual Reg [0x%x],Value = 0x%x\n", u32RegAddr, *pu32Value);
    }
    else
    {
        u32AddrOffset = 0;
        while (u32AddrOffset < u32MsgLen)
        {

            if (HI_SUCCESS != HI_SYS_WriteRegister(u32RegAddr, *pu32Value))
            {
                PQ_Parse_SendErrorFlag(client_sockfd, HI_BOARD_PROCESS_ERROR);
                HI_ERR_PQ("Write Reg [0x%x] error\n", u32RegAddr);
                return HI_FAILURE;
            }
            u32RegAddr += sizeof(HI_U32);
            u32AddrOffset += sizeof(HI_U32);
            pu32Value++;
        }
    }

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return HI_SUCCESS;
}

static HI_S32 HI_PQ_ReadDCIHistgram(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_DCI_HISTGRAM_S stDCIHistgram;

    memset(&stDCIHistgram, 0, sizeof(HI_PQ_DCI_HISTGRAM_S));

    HI_MPI_PQ_Init(HI_NULL);

    s32Ret = HI_MPI_PQ_GetHistgram(&stDCIHistgram);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_GetHistgram error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    HI_MPI_PQ_DeInit();

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stDCIHistgram, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 HI_PQ_WriteACM(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;

    HI_U32 u32RegAddr = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Addr);
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);

    HI_INFO_PQ("HI_PQ_WriteACM addr = %x,len = %d !\n", u32RegAddr, u32MsgLen);


    s32Ret = PQ_DRV_WriteACM(u32RegAddr, pu8ValidData, u32MsgLen);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_SetTNRLumaPixMean2Ratio(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_TNR_S* pstTnrTable = (HI_PQ_TNR_S*)pu8ValidData;

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);
    s32Ret = HI_MPI_PQ_SetTNRLumaPixMean2Ratio(pstTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_SetTNRLumaPixMean2Ratio error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetTNRLumaPixMean2Ratio(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_TNR_S stTnrTable;

    memset(&stTnrTable, 0, sizeof(HI_PQ_TNR_S));

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_MPI_PQ_GetTNRLumaPixMean2Ratio(&stTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_GetTNRLumaPixMean2Ratio error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stTnrTable, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return s32Ret;

}

static HI_S32 HI_PQ_SetTNRChromPixMean2Ratio(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_TNR_S* pstTnrTable = (HI_PQ_TNR_S*)pu8ValidData;

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);
    s32Ret = HI_MPI_PQ_SetTNRChromPixMean2Ratio(pstTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_SetTNRChromPixMean2Ratio error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetTNRChromPixMean2Ratio(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_TNR_S stTnrTable;

    memset(&stTnrTable, 0, sizeof(HI_PQ_TNR_S));

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_MPI_PQ_GetTNRChromPixMean2Ratio(&stTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_GetTNRChromPixMean2Ratio error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stTnrTable, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return s32Ret;

}

static HI_S32 HI_PQ_SetTNRLumaMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_TNR_S* pstTnrTable = (HI_PQ_TNR_S*)pu8ValidData;

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);
    s32Ret = HI_MPI_PQ_SetTNRLumaMotionMapping(pstTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_SetTNRLumaMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetTNRLumaMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_TNR_S stTnrTable;

    memset(&stTnrTable, 0, sizeof(HI_PQ_TNR_S));

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_MPI_PQ_GetTNRLumaMotionMapping(&stTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_GetTNRLumaMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stTnrTable, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return s32Ret;

}

static HI_S32 HI_PQ_SetTNRChromMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_TNR_S* pstTnrTable = (HI_PQ_TNR_S*)pu8ValidData;

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);
    s32Ret = HI_MPI_PQ_SetTNRChromMotionMapping(pstTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_SetTNRChromMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetTNRChromMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_TNR_S stTnrTable;

    memset(&stTnrTable, 0, sizeof(HI_PQ_TNR_S));

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_MPI_PQ_GetTNRChromMotionMapping(&stTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_GetTNRChromMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stTnrTable, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return s32Ret;

}

static HI_S32 HI_PQ_SetTNRLumaFinalMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_TNR_S* pstTnrTable = (HI_PQ_TNR_S*)pu8ValidData;

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);
    s32Ret = HI_MPI_PQ_SetTNRLumaFinalMotionMapping(pstTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_SetTNRLumaFinalMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetTNRLumaFinalMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_TNR_S stTnrTable;

    memset(&stTnrTable, 0, sizeof(HI_PQ_TNR_S));

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_MPI_PQ_GetTNRLumaFinalMotionMapping(&stTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_GetTNRLumaFinalMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stTnrTable, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return s32Ret;

}

static HI_S32 HI_PQ_SetTNRChromFinalMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_TNR_S* pstTnrTable = (HI_PQ_TNR_S*)pu8ValidData;

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);
    s32Ret = HI_MPI_PQ_SetTNRChromFinalMotionMapping(pstTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_SetTNRChromFinalMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetTNRChromFinalMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_TNR_S stTnrTable;

    memset(&stTnrTable, 0, sizeof(HI_PQ_TNR_S));

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_MPI_PQ_GetTNRChromFinalMotionMapping(&stTnrTable);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_GetTNRChromFinalMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stTnrTable, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return s32Ret;

}

static HI_S32 HI_PQ_SetTNRFMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_TNR_FMOTION_S* pstTnrFMotion = (HI_PQ_TNR_FMOTION_S*)pu8ValidData;

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);
    s32Ret = HI_MPI_PQ_SetTNRFMotionMapping(pstTnrFMotion);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_SetTNRFMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetTNRFMotionMapping(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_TNR_FMOTION_S stTnrFMotion;

    memset(&stTnrFMotion, 0, sizeof(HI_PQ_TNR_FMOTION_S));

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_MPI_PQ_GetTNRFMotionMapping(&stTnrFMotion);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("HI_MPI_PQ_GetTNRFMotionMapping error!\n");
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_DeInit();
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stTnrFMotion, u32MsgLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send data error!\n");
        return HI_FAILURE;
    }

    return s32Ret;

}

static HI_S32 HI_PQ_SetSnrMeanRatio(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_SNR_MEAN_RATIO_S* pstMeanRatio = (HI_PQ_SNR_MEAN_RATIO_S*)pu8ValidData;

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_SNR_MEAN_RATIO, pstMeanRatio);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetSnrMeanRatio(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_SNR_MEAN_RATIO_S stMeanRatio;

    memset(&stMeanRatio, 0, sizeof(HI_PQ_SNR_MEAN_RATIO_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_SNR_MEAN_RATIO, &stMeanRatio);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stMeanRatio, u32MsgLen);

    return s32Ret;
}

static HI_S32 HI_PQ_SetSnrEdgeStr(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U8* pu8ValidData = pu8Buffer + OFFSET_OF(HI_MDBG_MSG_S, pu8Data);
    HI_PQ_SNR_EDGE_STR_S* pstEdgeStr = (HI_PQ_SNR_EDGE_STR_S*)pu8ValidData;

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_SNR_EDGE_STR, pstEdgeStr);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendEnd(client_sockfd, ECHO_BYE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("send end error!\n");
        PQ_Parse_SendErrorFlag(client_sockfd, HI_RUN_ERROR);
    }

    return s32Ret;
}

static HI_S32 HI_PQ_GetSnrEdgeStr(HI_S32 client_sockfd, HI_U8* pu8Buffer)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32MsgLen = GET_VALUE(pu8Buffer, HI_MDBG_MSG_S, u32Len);
    HI_PQ_SNR_EDGE_STR_S stEdgeStr;

    memset(&stEdgeStr, 0, sizeof(HI_PQ_SNR_EDGE_STR_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_SNR_EDGE_STR, &stEdgeStr);
    CHECK_RETURN(s32Ret);

    s32Ret = HI_SendData(client_sockfd, (HI_U8*)&stEdgeStr, u32MsgLen);
    return s32Ret;
}

/*PQ CMD对应实现函数结构*/
PQ_DBG_CMD_FUN_S stPQCmdFunction[] =
{
    {PQ_CMD_READ_REG, HI_PQ_ReadReg},
    {PQ_CMD_WRITE_REG, HI_PQ_WriteReg},
    {PQ_CMD_READ_MATRIX, HI_PQ_ReadRegs},
    {PQ_CMD_WRITE_MATRIX, HI_PQ_WriteRegs},
    {PQ_CMD_READ_GAMMA, HI_PQ_ReadRegs},
    {PQ_CMD_WRITE_GAMMA, HI_PQ_WriteRegs},
    {PQ_CMD_READ_ACM, HI_PQ_ReadRegs},
    {PQ_CMD_WRITE_ACM, HI_PQ_WriteACM},
    {PQ_CMD_READ_ACC, HI_PQ_ReadRegs},
    {PQ_CMD_WRITE_ACC, HI_PQ_WriteRegs},
    {PQ_CMD_EXPORT_BIN, HI_PQ_ReadRegs},
    {PQ_CMD_IMPORT_BIN, HI_PQ_WriteRegs},
    {PQ_CMD_FIX_BIN, HI_PQ_WriteRegs},
    {PQ_CMD_READ_REGS, HI_PQ_ReadRegs},
    {PQ_CMD_WRITE_REGS, HI_PQ_WriteRegs},
    {PQ_CMD_READ_DCI_LUT, HI_PQ_ReadRegs},
    {PQ_CMD_WRITE_DCI_LUT, HI_PQ_WriteRegs},
    {PQ_CMD_READ_HIST, HI_PQ_ReadDCIHistgram},
    {PQ_CMD_READ_HDR_TM_LUT, HI_PQ_ReadRegs},
    {PQ_CMD_WRITE_HDR_TM_LUT, HI_PQ_WriteRegs},


    {PQ_CMD_READ_TNR_PIXMEAN_TO_RATIO_LUMA,        HI_PQ_GetTNRLumaPixMean2Ratio},
    {PQ_CMD_READ_TNR_PIXMEAN_TO_RATIO_CHROMA,      HI_PQ_GetTNRChromPixMean2Ratio},
    {PQ_CMD_READ_TNR_MOTION_MAPPING_LUMA,          HI_PQ_GetTNRLumaMotionMapping},
    {PQ_CMD_READ_TNR_MOTION_MAPPING_CHROMA,        HI_PQ_GetTNRChromMotionMapping},
    {PQ_CMD_READ_TNR_FINAL_MOTION_MAPPING_LUMA,    HI_PQ_GetTNRLumaFinalMotionMapping},
    {PQ_CMD_READ_TNR_FINAL_MOTION_MAPPING_CHROMA,  HI_PQ_GetTNRChromFinalMotionMapping},
    {PQ_CMD_WRITE_TNR_PIXMEAN_TO_RATIO_LUMA,       HI_PQ_SetTNRLumaPixMean2Ratio},
    {PQ_CMD_WRITE_TNR_PIXMEAN_TO_RATIO_CHROMA,     HI_PQ_SetTNRChromPixMean2Ratio},
    {PQ_CMD_WRITE_TNR_MOTION_MAPPING_LUMA,         HI_PQ_SetTNRLumaMotionMapping},
    {PQ_CMD_WRITE_TNR_MOTION_MAPPING_CHROMA,       HI_PQ_SetTNRChromMotionMapping},
    {PQ_CMD_WRITE_TNR_FINAL_MOTION_MAPPING_LUMA,   HI_PQ_SetTNRLumaFinalMotionMapping},
    {PQ_CMD_WRITE_TNR_FINAL_MOTION_MAPPING_CHROMA, HI_PQ_SetTNRChromFinalMotionMapping},
    {PQ_CMD_READ_FMOTION_MAPPING,                  HI_PQ_GetTNRFMotionMapping},
    {PQ_CMD_WRITE_FMOTION_MAPPING,                 HI_PQ_SetTNRFMotionMapping},
    {PQ_CMD_READ_SNR_MEAN_RATIO,                   HI_PQ_GetSnrMeanRatio},
    {PQ_CMD_WRITE_SNR_MEAN_RATIO,                  HI_PQ_SetSnrMeanRatio},
    {PQ_CMD_READ_SNR_EDGE_STR,                     HI_PQ_GetSnrEdgeStr},
    {PQ_CMD_WRITE_SNR_EDGE_STR,                    HI_PQ_SetSnrEdgeStr},
    //{PQ_CMD_BUTT, NULL},
};

/* PQ Debug 解析命令 */
HI_S32 PQ_Parse_ProcessCmd(HI_S32 client_sockfd, HI_U8* pu8Buf)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32CmdItem;
    HI_U32 u32Cmd = 0;
    HI_U32 u32CmdTotal = 0;

    CHECK_PTR_REINT(pu8Buf);

    u32Cmd = GET_VALUE(pu8Buf, HI_MDBG_MSG_S, u32CMD);

    switch (u32Cmd)
    {
        case PQ_CMD_READ_REG:
        case PQ_CMD_WRITE_REG:
        case PQ_CMD_READ_MATRIX:
        case PQ_CMD_WRITE_MATRIX:
        case PQ_CMD_READ_REGS:
        case PQ_CMD_WRITE_REGS:
        case PQ_CMD_READ_GAMMA:
        case PQ_CMD_WRITE_GAMMA:
        case PQ_CMD_EXPORT_BIN:
        case PQ_CMD_IMPORT_BIN:
        case PQ_CMD_FIX_BIN:
        case PQ_CMD_READ_YUV:
        case PQ_CMD_READ_RAW:
        case PQ_CMD_READ_ACM:
        case PQ_CMD_WRITE_ACM:
        case PQ_CMD_READ_ACC :
        case PQ_CMD_WRITE_ACC:
        case PQ_CMD_READ_DCI_LUT :
        case PQ_CMD_WRITE_DCI_LUT:
        case PQ_CMD_READ_HIST:
        case PQ_CMD_READ_TNR_PIXMEAN_TO_RATIO_LUMA:
        case PQ_CMD_READ_TNR_PIXMEAN_TO_RATIO_CHROMA:
        case PQ_CMD_READ_TNR_MOTION_MAPPING_LUMA:
        case PQ_CMD_READ_TNR_MOTION_MAPPING_CHROMA:
        case PQ_CMD_READ_TNR_FINAL_MOTION_MAPPING_LUMA:
        case PQ_CMD_READ_TNR_FINAL_MOTION_MAPPING_CHROMA:
        case PQ_CMD_WRITE_TNR_PIXMEAN_TO_RATIO_LUMA:
        case PQ_CMD_WRITE_TNR_PIXMEAN_TO_RATIO_CHROMA:
        case PQ_CMD_WRITE_TNR_MOTION_MAPPING_LUMA:
        case PQ_CMD_WRITE_TNR_MOTION_MAPPING_CHROMA:
        case PQ_CMD_WRITE_TNR_FINAL_MOTION_MAPPING_LUMA:
        case PQ_CMD_WRITE_TNR_FINAL_MOTION_MAPPING_CHROMA:
        case PQ_CMD_READ_FMOTION_MAPPING:
        case PQ_CMD_WRITE_FMOTION_MAPPING:
        case PQ_CMD_READ_SNR_MEAN_RATIO:
        case PQ_CMD_WRITE_SNR_MEAN_RATIO:
        case PQ_CMD_READ_SNR_EDGE_STR:
        case PQ_CMD_WRITE_SNR_EDGE_STR:
        case PQ_CMD_READ_HDR_TM_LUT:
        case PQ_CMD_WRITE_HDR_TM_LUT:
            HI_INFO_PQ("pqtools:receive command[%d]\n", u32Cmd);
            break;
        default:
            HI_ERR_PQ("PQ CMD failed! [%d]\n", u32Cmd);
            return HI_FAILURE;
    }

    u32CmdTotal = sizeof(stPQCmdFunction) / sizeof(PQ_DBG_CMD_FUN_S);
    for (u32CmdItem = 0; u32CmdItem < u32CmdTotal; u32CmdItem++)
    {
        if (u32Cmd != stPQCmdFunction[u32CmdItem].enDbgCmd )
        {
            continue;
        }

        if (HI_NULL == stPQCmdFunction[u32CmdItem].pfCmdFunction)
        {
            continue;
        }

        s32Ret = stPQCmdFunction[u32CmdItem].pfCmdFunction	(client_sockfd, pu8Buf);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("stPQCmdFunction failed! [%i]\n", s32Ret);
            return HI_FAILURE;
        }

        break;
    }


    return HI_SUCCESS;
}

