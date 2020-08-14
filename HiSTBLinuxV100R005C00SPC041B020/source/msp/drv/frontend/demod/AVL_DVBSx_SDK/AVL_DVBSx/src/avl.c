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


//$Revision: 255 $ 
//$Date: 2008-02-06 10:37:01 -0500 (Wed, 06 Feb 2008) $
// 
#include "avl.h"

#ifdef AVL_CPLUSPLUS
extern "C" {
#endif

	void ChunkAddr(AVL_uint32 uiaddr, AVL_puchar pBuff)
	{
		pBuff[0] =(AVL_uchar)(uiaddr>>16);
		pBuff[1] =(AVL_uchar)(uiaddr>>8);
		pBuff[2] =(AVL_uchar)(uiaddr);
		return ;
	}

	void Chunk16(AVL_uint16 uidata, AVL_puchar pBuff)
	{
		pBuff[0] = (AVL_uchar)(uidata>>8);
		pBuff[1] = (AVL_uchar)(uidata & 0xff);
		return ;
	}

	AVL_uint16 DeChunk16(const AVL_puchar pBuff)
	{
		AVL_uint16 uiData;
		uiData = pBuff[0];
		uiData = (AVL_uint16)(uiData << 8) + pBuff[1];
		return uiData;
	}

	void Chunk32(AVL_uint32 uidata, AVL_puchar pBuff)
	{
		pBuff[0] = (AVL_uchar)(uidata>>24);
		pBuff[1] = (AVL_uchar)(uidata>>16);
		pBuff[2] = (AVL_uchar)(uidata>>8);
		pBuff[3] = (AVL_uchar)(uidata);
		return ;
	}

	AVL_uint32 DeChunk32(const AVL_puchar pBuff)
	{
		AVL_uint32 uiData;
		uiData = pBuff[0];
		uiData = (uiData << 8) + pBuff[1];
		uiData = (uiData << 8) + pBuff[2];
		uiData = (uiData << 8) + pBuff[3];
		return uiData;
	}

	void Add32To64(struct AVL_uint64 *pSum, AVL_uint32 uiAddend)
	{
		AVL_uint32 uiTemp;

		uiTemp = pSum->m_LowWord;
		pSum->m_LowWord += uiAddend;
		pSum->m_LowWord &= 0xFFFFFFFF;

		if (pSum->m_LowWord < uiTemp)
		{
			pSum->m_HighWord++;
		}
	}

	AVL_uint32 Divide64(struct AVL_uint64 divisor, struct AVL_uint64 dividend)
	{
		AVL_uint32 uFlag = 0x0;
		AVL_uint32 uQuto = 0x0;
		AVL_uint32 i = 0;
		AVL_uint32 dividend_H = dividend.m_HighWord;
		AVL_uint32 dividend_L = dividend.m_LowWord;
		AVL_uint32 divisor_H = divisor.m_HighWord; 
		AVL_uint32 divisor_L = divisor.m_LowWord; 

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

				uFlag = (AVL_int32)dividend_H >> 31;

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

	void Multiply32(struct AVL_uint64 *pDst, AVL_uint32 m1, AVL_uint32 m2)
	{
		pDst->m_LowWord = (m1 & 0xFFFF) * (m2 & 0xFFFF);
		pDst->m_HighWord = 0;

		AddScaled32To64(pDst, (m1 >> 16) * (m2 & 0xFFFF));
		AddScaled32To64(pDst, (m2 >> 16) * (m1 & 0xFFFF));

		pDst->m_HighWord += (m1 >> 16) * (m2 >> 16);
	}

	void AddScaled32To64(struct AVL_uint64 *pDst, AVL_uint32 a)
	{
		AVL_uint32 saved;

		saved = pDst->m_LowWord;
		pDst->m_LowWord += (a << 16);

		pDst->m_LowWord &= 0xFFFFFFFF;
		pDst->m_HighWord += ((pDst->m_LowWord < saved) ? 1 : 0) + (a >> 16);
	}


#ifdef AVL_CPLUSPLUS
}
#endif

