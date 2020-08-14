/******************************************************************************

    Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sha.c
  Version       : Initial Draft
  Author        : y00106256
  Created       : 2011/11/29
  Last Modified :
  Description   : gloabl.c
  Function List :
  History       :
  1.Date        : 2011/11/29
    Author      : y00106256
    Modification: Created file

******************************************************************************/
#include "base.h"
#include "sha1.h"
#include <intrins.h>

#define RAM_STORE_ADDR   (DATA_BASE_ADDR+0x850)

/*
 *  以下是为 SHA1 向左环形移位宏 之定义
 */
#if 0 
#define SHA1CircularShift(word,bits) \
                (((word) << (bits)) | ((word) >> (32-(bits))))
#else
#define SHA1CircularShift _lrol_
#endif

extern xdata SHA1Context g_stSha1Info;
extern data HI_U32 g_u32CheckAddr;
extern data HI_U32 g_u32CheckSize;
extern void printf_char(HI_U8 ch);

data HI_U32  input[4]; 
data HI_U32 A,BB,C,D,E,F;
data HI_U8 tmpoff = 0;

extern void read_regVal_big(void);
extern void read_regVal(void);
extern void write_regVal(void);

void SHA1Reset()
{
    g_stSha1Info.u32Count[0] = 0;
    g_stSha1Info.u32Count[1] = 0;
    g_stSha1Info.Message_Block_Index = 0;
		
    g_stSha1Info.Intermediate_Hash[0] = 0x67452301;
    g_stSha1Info.Intermediate_Hash[1] = 0xEFCDAB89;
    g_stSha1Info.Intermediate_Hash[2] = 0x98BADCFE;
    g_stSha1Info.Intermediate_Hash[3] = 0x10325476;
    g_stSha1Info.Intermediate_Hash[4] = 0xC3D2E1F0;    
}

void SHA1PreProcess()
{
    idata HI_U32 tmp;
    idata HI_U8 t;
	
    A = g_stSha1Info.Intermediate_Hash[0];
    BB = g_stSha1Info.Intermediate_Hash[1];
    C = g_stSha1Info.Intermediate_Hash[2];
    D = g_stSha1Info.Intermediate_Hash[3];
    E = g_stSha1Info.Intermediate_Hash[4];

    for (t = 0; t < 16; t++)
    {
    	regData.val8[0] = g_stSha1Info.Message_Block[4 * t + 0] ;
    	regData.val8[1] = g_stSha1Info.Message_Block[4 * t + 1] ;
    	regData.val8[2] = g_stSha1Info.Message_Block[4 * t + 2] ;
    	regData.val8[3] = g_stSha1Info.Message_Block[4 * t + 3] ;

		
		regAddr.val32 = RAM_STORE_ADDR + t * 4 ;
			
    	*((unsigned char volatile xdata *)(RAM_STORE_ADDR + t * 4)) =  g_stSha1Info.Message_Block[4 * t + 3] ;
    	*((unsigned char volatile xdata *)(RAM_STORE_ADDR + t * 4+1)) =  g_stSha1Info.Message_Block[4 * t + 2] ;	
    	*((unsigned char volatile xdata *)(RAM_STORE_ADDR + t * 4+2)) =  g_stSha1Info.Message_Block[4 * t + 1] ;
    	*((unsigned char volatile xdata *)(RAM_STORE_ADDR + t * 4 +3)) =  g_stSha1Info.Message_Block[4 * t + 0] ;
		
    }
	
    for (t = 16; t < 80; t++)
    {
        regAddr.val32 = RAM_STORE_ADDR + (t - 3) * 4;	
        read_regVal(); 
        input[0] =  regData.val32;

        regAddr.val32 = RAM_STORE_ADDR + (t - 8) * 4;    
        read_regVal(); 
        input[1] =  regData.val32;

        regAddr.val32 = RAM_STORE_ADDR + (t - 14) * 4;    
        read_regVal(); 
        input[2] =  regData.val32;	

        regAddr.val32 = RAM_STORE_ADDR + (t - 16) * 4;    
        read_regVal(); 
        input[3] =  regData.val32;	

        tmp = input[0] ^ input[1];
        input[0] = tmp ^ input[2];
        tmp = input[0] ^ input[3];

        tmp = SHA1CircularShift(tmp,1);

        regAddr.val32 = RAM_STORE_ADDR + t * 4;  
        regData.val32 = tmp ;	   

        *((unsigned char volatile xdata *)(RAM_STORE_ADDR + t * 4)) =  regData.val8[3];
        *((unsigned char volatile xdata *)(RAM_STORE_ADDR + t * 4+1)) =  regData.val8[2];	
        *((unsigned char volatile xdata *)(RAM_STORE_ADDR + t * 4+2)) =   regData.val8[1];
        *((unsigned char volatile xdata *)(RAM_STORE_ADDR + t * 4 +3)) =   regData.val8[0];	
		
    }
}

void SHA1ProcessMessageWord(int t)
{
    idata HI_U32 temp;
	
    regAddr.val32 = RAM_STORE_ADDR + t * 4 ; 
	
    read_regVal(); 
    input[3] = regData.val32;		
	
    if (t >= 0 && t < 20)
    {
        temp = SHA1CircularShift(A, 5) + ((BB & C) | ((~BB) & D)) + E + input[3] + 0x5A827999;
        E = D;
        D = C;
        C = SHA1CircularShift(BB, 30);
        BB = A;
        A = temp;
    }

    if (t >= 20 && t < 40)
    {
        temp = SHA1CircularShift(A, 5) + (BB ^ C ^ D) + E + input[3] + 0x6ED9EBA1;
        E = D;
        D = C;
        C = SHA1CircularShift(BB, 30);
        BB = A;
        A = temp;
    }

    if (t >= 40 && t < 60)
    {
        temp = SHA1CircularShift(A, 5) + ((BB & C) | (BB & D) | (C & D)) + E + input[3] + 0x8F1BBCDC;
        E = D;
        D = C;
        C = SHA1CircularShift(BB, 30);
        BB = A;
        A = temp;
    }

    if (t >= 60 && t < 80)
    {
        temp = SHA1CircularShift(A, 5) + (BB ^ C ^ D) + E + input[3] + 0xCA62C1D6;
        E = D;
        D = C;
        C = SHA1CircularShift(BB, 30);
        BB = A;
        A = temp;
    }
}

void SHA1ProcessMessageBlock()
{
     idata HI_U8 t = 0;

     SHA1PreProcess();

     for(t = 0; t < 80; t++)
     {
        SHA1ProcessMessageWord(t);
     }

    /*  
     *  以下为迭代算法第80步（最后一步）描述
     */
    g_stSha1Info.Intermediate_Hash[0] += A;
    g_stSha1Info.Intermediate_Hash[1] += BB;
    g_stSha1Info.Intermediate_Hash[2] += C;
    g_stSha1Info.Intermediate_Hash[3] += D;
    g_stSha1Info.Intermediate_Hash[4] += E;

    g_stSha1Info.Message_Block_Index = 0;
}

void SHA1DoBody()
{   
    idata HI_U32 i;
    HI_U32 temp;
	
    temp = g_u32CheckSize / 4;
    while (temp--)
    {	
    	CHECK_PMOC_STATUS();//check if need change to ddr wakeup check
    	regAddr.val32 = g_u32CheckAddr;
    	read_regVal_big();

    	for (i = 0; i < 4; i++)
        {        
            g_stSha1Info.Message_Block[g_stSha1Info.Message_Block_Index++] = regData.val8[i];

            g_stSha1Info.u32Count[0] += 8;
            if (g_stSha1Info.u32Count[0] == 0)
            {
                g_stSha1Info.u32Count[1]++;
            }
    	}
    	
    	g_u32CheckAddr += 4;	
    	
    	if (g_stSha1Info.Message_Block_Index == 64)
        {
             SHA1ProcessMessageBlock();
        }

        CHECK_PMOC_STATUS();//check if need change to ddr wakeup check
    }

    temp = g_u32CheckSize % 4;
    if (temp == 0)
    {
    	return;
    }

    regAddr.val32 = g_u32CheckAddr;
    read_regVal_big();

    for (i = 0; i < temp; i++)
    {        
        g_stSha1Info.Message_Block[g_stSha1Info.Message_Block_Index++] = regData.val8[i];
    	
    	g_stSha1Info.u32Count[0] += 8;
    	if (g_stSha1Info.u32Count[0] == 0)
    	{
            g_stSha1Info.u32Count[1]++;
    	}
    }

    if (g_stSha1Info.Message_Block_Index == 64)
    {
        SHA1ProcessMessageBlock();
    }	
}

void SHA1DoTail()
{   
    idata HI_U8 i = 0;

    if (g_stSha1Info.Message_Block_Index > 55)
    {
    	 g_stSha1Info.Message_Block[g_stSha1Info.Message_Block_Index++] = 0x80;
    	 while (g_stSha1Info.Message_Block_Index < 64)
    	 {
            g_stSha1Info.Message_Block[g_stSha1Info.Message_Block_Index++] = 0;
    	 }

    	 SHA1ProcessMessageBlock();

    	 while (g_stSha1Info.Message_Block_Index < 56)
    	 {
            g_stSha1Info.Message_Block[g_stSha1Info.Message_Block_Index++] = 0;
    	 }
     }
     else
     {
    	 g_stSha1Info.Message_Block[g_stSha1Info.Message_Block_Index++] = 0x80;
    	 while (g_stSha1Info.Message_Block_Index < 56)
    	 {
            g_stSha1Info.Message_Block[g_stSha1Info.Message_Block_Index++] = 0;
    	 }
     }

    /*
     *  把最后64bit保存为数据长度
     */
    regData.val32 = g_stSha1Info.u32Count[1];
    g_stSha1Info.Message_Block[56] = regData.val8[0];
    g_stSha1Info.Message_Block[57] = regData.val8[1];
    g_stSha1Info.Message_Block[58] = regData.val8[2];
    g_stSha1Info.Message_Block[59] = regData.val8[3];

    regData.val32 = g_stSha1Info.u32Count[0];
    g_stSha1Info.Message_Block[60] = regData.val8[0];
    g_stSha1Info.Message_Block[61] = regData.val8[1];
    g_stSha1Info.Message_Block[62] = regData.val8[2];
    g_stSha1Info.Message_Block[63] = regData.val8[3];

    SHA1ProcessMessageBlock();
}

