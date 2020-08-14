#include "AVL6381_iic_rw.h"
#include "drv_i2c_ext.h"

/*--------------iic rw mutex-----------------------*/
HI_DECLARE_MUTEX(AVL6381_IIC_Mutex);

static HI_S32 AVL6381_IIC_Mutex_init(void)
{
	HI_INIT_MUTEX(&AVL6381_IIC_Mutex);
    return 0;
}

static HI_S32 AVL6381_IIC_Mutex_Lock(void)
{
    HI_S32 s32Ret;
    s32Ret = down_interruptible(&AVL6381_IIC_Mutex);
	return s32Ret;
}

static HI_S32 AVL6381_IIC_Mutex_Unlock(void)
{
    up(&AVL6381_IIC_Mutex);
	return 0;
}



void AVL6381_ChunkAddr(AVL6381_uint32 uiaddr, AVL6381_puchar pBuff)
{
	pBuff[0] =(AVL6381_uchar)(uiaddr>>16);
	pBuff[1] =(AVL6381_uchar)(uiaddr>>8);
	pBuff[2] =(AVL6381_uchar)(uiaddr);
	return ;
}

void AVL6381_Chunk16(AVL6381_uint16 uidata, AVL6381_puchar pBuff)
{       
    pBuff[0] = (AVL6381_uchar)(uidata>>8);
	pBuff[1] = (AVL6381_uchar)(uidata & 0xff);
	return ;
}

AVL6381_uint16 AVL6381_DeChunk16(const AVL6381_puchar pBuff)
{
	AVL6381_uint16 uiData;
	uiData = pBuff[0];
	uiData = (AVL6381_uint16)(uiData << 8) + pBuff[1];
	return uiData;
}

void AVL6381_Chunk32(AVL6381_uint32 uidata, AVL6381_puchar pBuff)
{
	pBuff[0] = (AVL6381_uchar)(uidata>>24);
	pBuff[1] = (AVL6381_uchar)(uidata>>16);
	pBuff[2] = (AVL6381_uchar)(uidata>>8);
	pBuff[3] = (AVL6381_uchar)(uidata);
	return ;
}

AVL6381_uint32 AVL6381_DeChunk32(const AVL6381_puchar pBuff)
{
	AVL6381_uint32 uiData;
	uiData = pBuff[0];
	uiData = (uiData << 8) + pBuff[1];
	uiData = (uiData << 8) + pBuff[2];
	uiData = (uiData << 8) + pBuff[3];
	return uiData;
}

void AVL6381_Multiply32By16(AVL6381_int64 *pY, AVL6381_int32 a, AVL6381_int16 b)
{
    AVL6381_uint16 uiLswA;
    AVL6381_int16 iMswA;
    AVL6381_int32 iMswAB;
    AVL6381_uint32 uiLswAB;
    struct AVL6381_int64 iTemp;

    // (mswA * 2^16 + lswA) * b = (mswA * b * 2^16) + (lswA * b)
    // Compute (mswA * b * 2^16):
    uiLswA = (AVL6381_uint16)(a & 0x0000FFFF);
    iMswA = (AVL6381_int16)((a >> 16) & 0x0000FFFF);
    iMswAB = (AVL6381_int32)(iMswA * b);
    iTemp.m_HighWord = iMswAB;
    iTemp.m_HighWord >>= 16;
    iTemp.m_LowWord = (AVL6381_uint32)(iMswAB << 16);
    // Compute (lswA * b):
    uiLswAB = (AVL6381_uint32)(uiLswA * b);

    // Add the results
    AVL6381_Add32To64((struct AVL6381_uint64 *)&iTemp, uiLswAB);

    pY->m_HighWord = iTemp.m_HighWord;
    pY->m_LowWord = iTemp.m_LowWord;
}

AVL6381_uint32 AVL6381_Divide64(struct AVL6381_uint64 divisor, struct AVL6381_uint64 dividend)
{
	AVL6381_uint32 uFlag = 0x0;
	AVL6381_uint32 uQuto = 0x0;
	AVL6381_uint32 i = 0;
	AVL6381_uint32 dividend_H = dividend.m_HighWord;
	AVL6381_uint32 dividend_L = dividend.m_LowWord;
	AVL6381_uint32 divisor_H = divisor.m_HighWord;
	AVL6381_uint32 divisor_L = divisor.m_LowWord;

	if(((divisor_H == 0x0) && (divisor_L == 0x0)) || (dividend_H/divisor_L))
	{
		return 0;
	}
	else if((divisor_H == 0x0)&&(dividend_H == 0x0))
	{
		return  dividend_L / divisor_L;
	}
	else
	{
		if(divisor_H != 0)
		{
			while(divisor_H)
			{
				dividend_L /= 2;
				if(dividend_H % 2)
				{
					dividend_L += 0x80000000;
				}
				dividend_H /= 2;

				divisor_L /= 2;
				if(divisor_H %2)
				{
					divisor_L += 0x80000000;
				}
				divisor_H /= 2;
			}
		}
		for   (i = 0; i <= 31; i++)
		{

			uFlag = (AVL6381_int32)dividend_H >> 31;

			dividend_H = (dividend_H << 1)|(dividend_L >> 31);
			dividend_L <<= 1;

			uQuto <<= 1;
			if((dividend_H|uFlag) >= divisor_L)
			{
				dividend_H -= divisor_L;
				uQuto++;
			}
		}
		return uQuto;
	}

}

void AVL6381_AddScaled32To64(struct AVL6381_uint64 *pDst, AVL6381_uint32 a)
{
	AVL6381_uint32 saved;

	saved = pDst->m_LowWord;
	pDst->m_LowWord += (a << 16);

	pDst->m_LowWord &= 0xFFFFFFFF;
	pDst->m_HighWord += ((pDst->m_LowWord < saved) ? 1 : 0) + (a >> 16);
}


void AVL6381_Multiply32(struct AVL6381_uint64 *pDst, AVL6381_uint32 m1, AVL6381_uint32 m2)
{
	pDst->m_LowWord = (m1 & 0xFFFF) * (m2 & 0xFFFF);
	pDst->m_HighWord = 0;

	AVL6381_AddScaled32To64(pDst, (m1 >> 16) * (m2 & 0xFFFF));
	AVL6381_AddScaled32To64(pDst, (m2 >> 16) * (m1 & 0xFFFF));

	pDst->m_HighWord += (m1 >> 16) * (m2 >> 16);
}




void AVL6381_Add32To64(struct AVL6381_uint64 *pSum, AVL6381_uint32 uiAddend)
{
	AVL6381_uint32 uiTemp;

	uiTemp = pSum->m_LowWord;
	pSum->m_LowWord += uiAddend;
	pSum->m_LowWord &= 0xFFFFFFFF;

	if (pSum->m_LowWord < uiTemp)
	{
		pSum->m_HighWord++;
	}
}


/*----------------------iic read and write----------------------------------------*/
AVL6381_IIC_ErrorCode AVL6381_I2CRead(  const struct AVL6381_Chip * pAVLChip,  AVL6381_puchar pucBuff, AVL6381_puint16 puiSize )
{           
    HI_S32  Ret = 0;           
    /* Support GPIO I2C */
    if (HI_STD_I2C_NUM >= pAVLChip->m_I2CChn)
    {       
		Ret = HI_DRV_I2C_ReadDirectly(pAVLChip->m_I2CChn, pAVLChip->m_SlaveAddr | 1, 0, 0, pucBuff, *puiSize);
    }   
    else //USE GPIO I2C
    {
    	Ret = HI_DRV_GPIOI2C_ReadExtDirectly(pAVLChip->m_I2CChn, pAVLChip->m_SlaveAddr | 1, 0, 0, pucBuff, *puiSize);
    }
    return Ret; 
} 

AVL6381_IIC_ErrorCode AVL6381_I2CWrite(  const struct AVL6381_Chip * pAVLChip,  AVL6381_puchar pucBuff,  AVL6381_puint16  puiSize )
{
    HI_S32  Ret = 0;

    /* Support GPIO I2C */
    if (HI_STD_I2C_NUM >= pAVLChip->m_I2CChn)
    {
		Ret = HI_DRV_I2C_Write(pAVLChip->m_I2CChn, pAVLChip->m_SlaveAddr | 0, 0, 0, pucBuff, *puiSize);
    }
	else
    {
		Ret = HI_DRV_GPIOI2C_WriteExt(pAVLChip->m_I2CChn, pAVLChip->m_SlaveAddr | 0, 0, 0, pucBuff, *puiSize);
    }

    return Ret;
}


AVL6381_IIC_ErrorCode AVL6381_II2C_Read( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiOffset, AVL6381_puchar pucBuff, AVL6381_uint16 uiSize)
{
    AVL6381_IIC_ErrorCode r;
    AVL6381_uchar pucBuffTemp[3];
    AVL6381_uint16 ui1, ui2;
    AVL6381_uint16 iSize;

    r = AVL6381_IIC_Mutex_Lock();
    if( AVL6381_EC_OK == r )
    {
        AVL6381_ChunkAddr(uiOffset, pucBuffTemp);
        ui1 = 3;
        r = AVL6381_I2CWrite(pAVLChip, pucBuffTemp, &ui1);
        if( AVL6381_EC_OK == r )
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
            while( iSize > AVL6381_MAX_II2C_READ_SIZE )
            {
                ui1 = AVL6381_MAX_II2C_READ_SIZE;
                r |= AVL6381_I2CRead(pAVLChip, pucBuff+ui2, &ui1);
                ui2 += AVL6381_MAX_II2C_READ_SIZE;
                iSize -= AVL6381_MAX_II2C_READ_SIZE;
            }

            if( 0 != iSize )
            {
                r |= AVL6381_I2CRead(pAVLChip, pucBuff+ui2, &iSize);
            }

            if( uiSize & 1 )
            {
                ui1 = 2;
                r |= AVL6381_I2CRead(pAVLChip, pucBuffTemp, &ui1);
                pucBuff[uiSize-1] = pucBuffTemp[0];
            }
        }
    }
    r |= AVL6381_IIC_Mutex_Unlock();
    return(r);
}

AVL6381_IIC_ErrorCode AVL6381_II2C_Write( const struct AVL6381_Chip * pAVLChip, AVL6381_puchar pucBuff, AVL6381_uint16 uiSize)
{
    AVL6381_IIC_ErrorCode r;
    AVL6381_uchar pucBuffTemp[5];
    AVL6381_uint16 ui1, ui2, uTemp;
    AVL6381_uint16 iSize;
    AVL6381_uint32 uAddr;
    if( uiSize<3 )
    {
        return(AVL6381_EC_GeneralFail);    //at least 3 bytes
    }

    uiSize -= 3;            //actual data size
    r = AVL6381_IIC_Mutex_Lock();
    if( AVL6381_EC_OK == r )
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

        uTemp = (AVL6381_MAX_II2C_Write_SIZE-3) & 0xfffe; //how many bytes data we can transfer every time
        ui2 = 0;
        while( iSize > uTemp )
        {
            ui1 = uTemp+3;
            //save the data
            pucBuffTemp[0] = pucBuff[ui2];
            pucBuffTemp[1] = pucBuff[ui2+1];
            pucBuffTemp[2] = pucBuff[ui2+2];
            AVL6381_ChunkAddr(uAddr, pucBuff+ui2);
            r |= AVL6381_I2CWrite(pAVLChip, pucBuff+ui2, &ui1);
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
        AVL6381_ChunkAddr(uAddr, pucBuff+ui2);
        r |= AVL6381_I2CWrite(pAVLChip, pucBuff+ui2, &ui1);
        //restore data
        pucBuff[ui2] = pucBuffTemp[0];
        pucBuff[ui2+1] = pucBuffTemp[1];
        pucBuff[ui2+2] = pucBuffTemp[2];
        uAddr += iSize;
        ui2 += iSize;

        if( uiSize & 1 )
        {
            AVL6381_ChunkAddr(uAddr, pucBuffTemp);
            ui1 = 3;
            r |= AVL6381_I2CWrite(pAVLChip, pucBuffTemp, &ui1);
            ui1 = 2;
            r |= AVL6381_I2CRead(pAVLChip, pucBuffTemp+3, &ui1);
            pucBuffTemp[3] = pucBuff[ui2+3];
            ui1 = 5;
            r |= AVL6381_I2CWrite(pAVLChip, pucBuffTemp, &ui1);
        }
    }
    r |= AVL6381_IIC_Mutex_Unlock();

    return(r);
}


AVL6381_ErrorCode AVL6381_II2C_Read8(const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_puchar puiData)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uchar Data = 0;

    r = AVL6381_II2C_Read(pAVLChip, uiAddr, &Data, 1);
    if( AVL6381_EC_OK == r )
    {
        *puiData = Data;
    }
    
    return r;
}



AVL6381_IIC_ErrorCode AVL6381_II2C_Read16( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_puint16 puiData )
{
    AVL6381_IIC_ErrorCode r;
    AVL6381_uchar pBuff[2];

    r = AVL6381_II2C_Read(pAVLChip, uiAddr, pBuff, 2);
    if( AVL6381_EC_OK == r )
    {
        *puiData = AVL6381_DeChunk16(pBuff);
    }
    return(r);
}

AVL6381_IIC_ErrorCode AVL6381_II2C_Read32( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_puint32 puiData )
{
    AVL6381_IIC_ErrorCode r;    
	AVL6381_uchar pBuff[4];

    r = AVL6381_II2C_Read(pAVLChip, uiAddr, pBuff, 4);
    if( AVL6381_EC_OK == r )
    {
        *puiData = AVL6381_DeChunk32(pBuff);
    }
    return(r);
}

AVL6381_IIC_ErrorCode AVL6381_II2C_Write8(const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_uchar ucData)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uchar pBuff[4];

    AVL6381_ChunkAddr(uiAddr, pBuff);
    pBuff[3] = ucData;

    r = AVL6381_II2C_Write(pAVLChip, pBuff, 4);
    
    return r;
}

AVL6381_IIC_ErrorCode AVL6381_II2C_Write16( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_uint16 uiData )
{   
	AVL6381_IIC_ErrorCode r;
    AVL6381_uchar pBuff[5]; 

    AVL6381_ChunkAddr(uiAddr, pBuff);
    AVL6381_Chunk16(uiData, pBuff+3);
    
    r = AVL6381_II2C_Write(pAVLChip, pBuff, 5);
    return(r);
} 


AVL6381_IIC_ErrorCode AVL6381_II2C_Write32( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_uint32 uiData )
{
    AVL6381_IIC_ErrorCode r;    
	AVL6381_uchar pBuff[7];
    
    AVL6381_ChunkAddr(uiAddr, pBuff);
    AVL6381_Chunk32(uiData, pBuff+3);
    r = AVL6381_II2C_Write(pAVLChip, pBuff, 7);
    return(r);
}

AVL6381_IIC_ErrorCode AVL6381_II2C_Initialize(void)
{
	AVL6381_IIC_ErrorCode r = AVL6381_EC_OK;  
	static AVL6381_uchar gII2cSem_inited = 0;
	
	if(0 == gII2cSem_inited)
	{
		gII2cSem_inited = 1;
		r = AVL6381_IIC_Mutex_init();		
	}
	return r;
}


