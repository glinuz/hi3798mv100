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


//$Revision: 323 $ 
//$Date: 2008-03-14 11:31:11 -0400 (Fri, 14 Mar 2008) $
// 
#include "II2C.h"
#include "IBSP.h"

/// @cond
AVL_semaphore gI2CSem;
/// @endcond

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Initialize(void)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	static AVL_uchar gI2CSem_inited = 0;
	if( 0 == gI2CSem_inited )
	{
		gI2CSem_inited = 1;
		r = AVL_DVBSx_IBSP_InitSemaphore(&gI2CSem);
	}
	return r;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiOffset, AVL_puchar pucBuff, AVL_uint16 uiSize)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pucBuffTemp[3];
	AVL_uint16 ui1, ui2;
	AVL_uint16 iSize;

	r = AVL_DVBSx_IBSP_WaitSemaphore(&(gI2CSem));
	if( AVL_DVBSx_EC_OK == r )
	{
		ChunkAddr(uiOffset, pucBuffTemp);
		ui1 = 3;
		r = AVL_DVBSx_IBSP_I2CWrite(pAVLChip, pucBuffTemp, &ui1);  
		if( AVL_DVBSx_EC_OK == r )
		{
			if( uiSize & 1 )
			{
				iSize = uiSize - 1;
			}
			else
			{
				iSize = uiSize;
			}
			ui2 = 0;
			while( iSize > MAX_II2C_READ_SIZE )
			{
				ui1 = MAX_II2C_READ_SIZE;
				r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip, pucBuff+ui2, &ui1);
				ui2 += MAX_II2C_READ_SIZE;
				iSize -= MAX_II2C_READ_SIZE;
			}

			if( 0 != iSize )
			{
				r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip, pucBuff+ui2, &iSize);
			}

			if( uiSize & 1 )
			{
				ui1 = 2;
				r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip, pucBuffTemp, &ui1);
				pucBuff[uiSize-1] = pucBuffTemp[0];
			}
		}
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(gI2CSem)); 
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_ReadDirect( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puchar pucBuff, AVL_uint16 uiSize)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pucBuffTemp[3];
	AVL_uint16 ui1, ui2;
	AVL_uint16 iSize;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(gI2CSem));
	if( AVL_DVBSx_EC_OK == r )
	{
		if( uiSize & 1 )
		{
			iSize = uiSize - 1;
		}
		else
		{
			iSize = uiSize;
		}
		ui2 = 0;
		while( iSize > MAX_II2C_READ_SIZE )
		{
			ui1 = MAX_II2C_READ_SIZE;
			r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip, pucBuff+ui2, &ui1);
			ui2 += MAX_II2C_READ_SIZE;
			iSize -= MAX_II2C_READ_SIZE;
		}

		if( 0 != iSize )
		{
			r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip, pucBuff+ui2, &iSize);
		}

		if( uiSize & 1 )
		{
			ui1 = 2;
			r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip, pucBuffTemp, &ui1);
			pucBuff[uiSize-1] = pucBuffTemp[0];
		}
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(gI2CSem)); 
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puchar pucBuff, AVL_uint16 uiSize)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pucBuffTemp[5];
	AVL_uint16 ui1, ui2, uTemp;
	AVL_uint16 iSize;
	AVL_uint32 uAddr;
	if( uiSize<3 )
	{
		return(AVL_DVBSx_EC_GeneralFail);	 //at least 3 bytes
	}

	uiSize -= 3;			//actual data size
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(gI2CSem));
	if( AVL_DVBSx_EC_OK == r )
	{
		//dump address
		uAddr = pucBuff[0];
		uAddr = uAddr<<8;
		uAddr += pucBuff[1];
		uAddr = uAddr<<8;
		uAddr += pucBuff[2];

		if( uiSize & 1 )
		{
			iSize = uiSize -1;
		}
		else
		{
			iSize = uiSize;
		}

		uTemp = (MAX_II2C_Write_SIZE-3) & 0xfffe; //how many bytes data we can transfer every time

		ui2 = 0;
		while( iSize > uTemp )
		{
			ui1 = uTemp+3;
			//save the data
			pucBuffTemp[0] = pucBuff[ui2];
			pucBuffTemp[1] = pucBuff[ui2+1];
			pucBuffTemp[2] = pucBuff[ui2+2];
			ChunkAddr(uAddr, pucBuff+ui2);
			r |= AVL_DVBSx_IBSP_I2CWrite(pAVLChip, pucBuff+ui2, &ui1);
			//restore data
			pucBuff[ui2] = pucBuffTemp[0];
			pucBuff[ui2+1] = pucBuffTemp[1];
			pucBuff[ui2+2] = pucBuffTemp[2];
			uAddr += uTemp;
			ui2 += uTemp;
			iSize -= uTemp;
		}
		ui1 = iSize+3;
		//save the data
		pucBuffTemp[0] = pucBuff[ui2];
		pucBuffTemp[1] = pucBuff[ui2+1];
		pucBuffTemp[2] = pucBuff[ui2+2];
		ChunkAddr(uAddr, pucBuff+ui2);
		r |= AVL_DVBSx_IBSP_I2CWrite(pAVLChip, pucBuff+ui2, &ui1);
		//restore data
		pucBuff[ui2] = pucBuffTemp[0];
		pucBuff[ui2+1] = pucBuffTemp[1];
		pucBuff[ui2+2] = pucBuffTemp[2];
		uAddr += iSize;
		ui2 += iSize;

		if( uiSize & 1 )
		{
			ChunkAddr(uAddr, pucBuffTemp);
			ui1 = 3;
			r |= AVL_DVBSx_IBSP_I2CWrite(pAVLChip, pucBuffTemp, &ui1);
			ui1 = 2;
			r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip, pucBuffTemp+3, &ui1);
			pucBuffTemp[3] = pucBuff[ui2+3];
			ui1 = 5;
			r |= AVL_DVBSx_IBSP_I2CWrite(pAVLChip, pucBuffTemp, &ui1);
		}
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(gI2CSem)); 

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read16( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_puint16 puiData )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[2];

	r = AVL_DVBSx_II2C_Read(pAVLChip, uiAddr, pBuff, 2);
	if( AVL_DVBSx_EC_OK == r )
	{
		*puiData = DeChunk16(pBuff);
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read32( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_puint32 puiData )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[4];

	r = AVL_DVBSx_II2C_Read(pAVLChip, uiAddr, pBuff, 4);
	if( AVL_DVBSx_EC_OK == r )
	{
		*puiData = DeChunk32(pBuff);
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write16( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_uint16 uiData )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[5];

	ChunkAddr(uiAddr, pBuff);
	Chunk16(uiData, pBuff+3);

	r = AVL_DVBSx_II2C_Write(pAVLChip, pBuff, 5);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write32( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_uint32 uiData )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[7];

	ChunkAddr(uiAddr, pBuff);
	Chunk32(uiData, pBuff+3);
	r = AVL_DVBSx_II2C_Write(pAVLChip, pBuff, 7);
	return(r);
}
