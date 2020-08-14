/*
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


//$Revision: 581 $ 
//$Date: 2008-10-23 13:31:49 -0400 (Thu, 23 Oct 2008) $
// 
#include "II2CRepeater.h"
#include "IBSP.h"
#include "II2C.h"

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_SendOP(AVL_puchar pBuff, AVL_uchar ucSize, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	const AVL_uint16 uiTimeDelay = 10;
	const AVL_uint16 uiMaxRetries = 30;
	AVL_uint32 i;

	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semI2CRepeater));

	i = 0;
	while (AVL_DVBSx_EC_OK != AVL_DVBSx_II2CRepeater_GetOPStatus(pAVLChip))   //Maximum waiting time is 300mS.
	{
		if (uiMaxRetries < i++)
		{
			r |= AVL_DVBSx_EC_Running;
			break;
		}
		//AVL_DVBSx_IBSP_Delay(uiTimeDelay);
        tuner_mdelay(uiTimeDelay);
	}

	if ( AVL_DVBSx_EC_OK == r )
	{
		r = AVL_DVBSx_II2C_Write(pAVLChip, pBuff, ucSize);   
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_Initialize( AVL_uint16 I2CBusClock_kHz, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_uchar pBuff[5];
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Write16(pAVLChip, rc_i2cm_speed_kHz_addr, I2CBusClock_kHz);
	ChunkAddr(i2cm_cmd_addr+I2CM_CMD_LENGTH-2, pBuff);
	pBuff[3] = 0x01;
	pBuff[4] = OP_I2CM_INIT;
	r |= AVL_DVBSx_II2CRepeater_SendOP(pBuff, 5, pAVLChip);
	return r;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_GetOPStatus( const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[2];
	r = AVL_DVBSx_II2C_Read(pAVLChip, i2cm_cmd_addr+I2CM_CMD_LENGTH-2, pBuff, 2);
	if ( AVL_DVBSx_EC_OK == r )
	{
		if ( pBuff[1] != 0 )
		{
			r = AVL_DVBSx_EC_Running;
		}
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_ReadData( AVL_uchar ucSlaveAddr, AVL_puchar pucBuff, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[I2CM_RSP_LENGTH];
	AVL_uint16 uiTimeOut;
	const AVL_uint16 uiTimeOutTh = 10;
	const AVL_uint32 uiTimeDelay = 100;  //100 ms 

	if ( uiSize > I2CM_RSP_LENGTH )
	{
		return(AVL_DVBSx_EC_GeneralFail);
	}
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semI2CRepeater_r));

	ChunkAddr(i2cm_cmd_addr+I2CM_CMD_LENGTH-4, pBuff);
	pBuff[3] = 0x0;
	pBuff[4] = (AVL_uchar)uiSize;
	pBuff[5] = ucSlaveAddr;
	pBuff[6] = OP_I2CM_READ;
	r |= AVL_DVBSx_II2CRepeater_SendOP(pBuff, 7, pAVLChip);
	if ( AVL_DVBSx_EC_OK == r )
	{
		uiTimeOut = 0;
		while ( AVL_DVBSx_EC_OK != AVL_DVBSx_II2CRepeater_GetOPStatus(pAVLChip) )
		{
			if((++uiTimeOut) >= uiTimeOutTh)
			{
				r |= AVL_DVBSx_EC_TimeOut;
				r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater_r));
				return(r);
			}
			//r |= AVL_DVBSx_IBSP_Delay(uiTimeDelay);
            tuner_mdelay(uiTimeDelay);
		}
		r |= (AVL_DVBSx_II2C_Read(pAVLChip, i2cm_rsp_addr, pucBuff, uiSize));
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater_r));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_ReadData_Multi(  AVL_uchar ucSlaveAddr, AVL_puchar pucBuff, AVL_uchar ucRegAddr, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip )
{
   	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[I2CM_RSP_LENGTH];
	AVL_uint16 uiTimeOut;
	const AVL_uint16 uiTimeOutTh = 10;
	const AVL_uint32 uiTimeDelay = 100;  //100 ms 

	if ( uiSize > I2CM_RSP_LENGTH )
	{
		return(AVL_DVBSx_EC_GeneralFail);
	}

	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semI2CRepeater_r));

	ChunkAddr(i2cm_cmd_addr+I2CM_CMD_LENGTH-6, pBuff);
	pBuff[3] = 0;
	pBuff[4] = ucRegAddr;
	pBuff[5] = 0x1;
	pBuff[6] = (AVL_uchar)uiSize;
	pBuff[7] = ucSlaveAddr;
	pBuff[8] = OP_I2CM_READ;
	r |= AVL_DVBSx_II2CRepeater_SendOP(pBuff, 9, pAVLChip);
	if ( AVL_DVBSx_EC_OK == r )
	{
		uiTimeOut = 0;
		while ( AVL_DVBSx_EC_OK != AVL_DVBSx_II2CRepeater_GetOPStatus(pAVLChip) )
		{
			if((++uiTimeOut) >= uiTimeOutTh)
			{
				r |= AVL_DVBSx_EC_TimeOut;
				r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater_r));
				return(r);
			}
			r |= AVL_DVBSx_IBSP_Delay(uiTimeDelay);
		}
		r |= (AVL_DVBSx_II2C_Read(pAVLChip, i2cm_rsp_addr, pucBuff, uiSize));
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater_r));

	return(r);
}


AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_SendData(  AVL_uchar ucSlaveAddr, const AVL_puchar pucBuff, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_uchar pBuff[I2CM_CMD_LENGTH+3];
	AVL_uint16 i1, i2;
	AVL_uint16 cmdSize;

	if ( uiSize>I2CM_CMD_LENGTH-3 )
	{
		return(AVL_DVBSx_EC_GeneralFail);
	}

	cmdSize = ((3+uiSize)%2)+3+uiSize;	  /* How many bytes need send to Availink device through i2c interface */
	ChunkAddr(i2cm_cmd_addr+I2CM_CMD_LENGTH-cmdSize, pBuff);

	i1 = 3+((3+uiSize)%2);	  /* skip one byte if the uisize+3 is odd*/

	for ( i2=0; i2<uiSize; i2++ )
	{
		pBuff[i1++] = pucBuff[i2];
	}
	pBuff[i1++] = (AVL_uchar)uiSize;
	pBuff[i1++] = ucSlaveAddr;
	pBuff[i1++] = OP_I2CM_WRITE;

	return(AVL_DVBSx_II2CRepeater_SendOP(pBuff, (AVL_uchar)(cmdSize+3), pAVLChip));
}

