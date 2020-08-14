
#include "drv_disp_alg_gamma.h"

static GMM_COEF_BITARRAY_S stArray; //static type avoid stack overflow

static HI_U16 stGamRgbArray[4][3][257] = {{{0}}};
static HI_U16 stGamYuvArray[4][3][257] = {{{0}}};
static HI_U16 u16TmpRGB[960*4] = {0};//0~959;960~1919;1920~2879;2880~3839       //Too large,	堆栈溢出stack overflow, so make it static 全局变量

//now it is just one mode
static HI_U32 GmmTransCoefAlign(const HI_U16* pu16GmmCoef, GMM_COEF_BITARRAY_S* pBitCoef)
{
    HI_U32  i, j, u32Cnt, u32Tmp, u32Mode, u32TransNum, u32OriNum;
    HI_U16* pu16GmmCoef0;
    HI_U16* pu16GmmCoef1;
    HI_U16* pu16GmmCoef2;
	HI_U32 k = 0;	
	u32Cnt = 39;
	u32Mode = 4;
	u32TransNum = 960;
	u32OriNum = 257;


    //TransToOneArray
    for (j = 0; j < u32Mode; j++)
    {
        //Data combination
		pu16GmmCoef0 = (HI_U16*)(pu16GmmCoef + u32OriNum*(j*3));
		pu16GmmCoef1 = (HI_U16*)(pu16GmmCoef + u32OriNum*(j*3+1));
		pu16GmmCoef2 = (HI_U16*)(pu16GmmCoef + u32OriNum*(j*3+2));
//debug
#if 0
		if(j==2)
		{
			HI_PRINT("--------input R[0]=%d,G[0]=%d,B[0]=%d R[1]=%d,G[1]=%d,B[1]=%d  R[2]=%d,G[2]=%d,B[2]=%d\r\n",
					pu16GmmCoef0[0],pu16GmmCoef1[0],pu16GmmCoef2[0],
					pu16GmmCoef0[1],pu16GmmCoef1[1],pu16GmmCoef2[1],
					pu16GmmCoef0[2],pu16GmmCoef1[2],pu16GmmCoef2[2]);
		}
		else if(j==3)
		{
			HI_PRINT("--------input RR[0]=%d,GG[0]=%d,BB[0]=%d RR[1]=%d,GG[1]=%d,BB[1]=%d  RR[2]=%d,GG[2]=%d,BB[2]=%d\r\n",
					pu16GmmCoef0[0],pu16GmmCoef1[0],pu16GmmCoef2[0],
					pu16GmmCoef0[1],pu16GmmCoef1[1],pu16GmmCoef2[1],
					pu16GmmCoef0[2],pu16GmmCoef1[2],pu16GmmCoef2[2]);

		}
#endif

		for (i = 0; i < 256; i = i + 4) //(256/4)*15 = 960
		{
#if 0
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef0[i + 4]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef0[i + 3]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef0[i + 2]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef0[i + 1]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef0[i];     k++;
			
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef1[i + 4]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef1[i + 3]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef1[i + 2]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef1[i + 1]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef1[i];     k++;

			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef2[i + 4]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef2[i + 3]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef2[i + 2]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef2[i + 1]; k++;
			u16TmpRGB[u32TransNum*j + k] = pu16GmmCoef2[i];     k++;
#endif
			u16TmpRGB[k] = pu16GmmCoef0[i + 4]; k++;
			u16TmpRGB[k] = pu16GmmCoef0[i + 3]; k++;
			u16TmpRGB[k] = pu16GmmCoef0[i + 2]; k++;
			u16TmpRGB[k] = pu16GmmCoef0[i + 1]; k++;
			u16TmpRGB[k] = pu16GmmCoef0[i];     k++;
			
			u16TmpRGB[k] = pu16GmmCoef1[i + 4]; k++;
			u16TmpRGB[k] = pu16GmmCoef1[i + 3]; k++;
			u16TmpRGB[k] = pu16GmmCoef1[i + 2]; k++;
			u16TmpRGB[k] = pu16GmmCoef1[i + 1]; k++;
			u16TmpRGB[k] = pu16GmmCoef1[i];     k++;

			u16TmpRGB[k] = pu16GmmCoef2[i + 4]; k++;
			u16TmpRGB[k] = pu16GmmCoef2[i + 3]; k++;
			u16TmpRGB[k] = pu16GmmCoef2[i + 2]; k++;
			u16TmpRGB[k] = pu16GmmCoef2[i + 1]; k++;
			u16TmpRGB[k] = pu16GmmCoef2[i];     k++;

		}
//		HI_PRINT("--------the last KKKKKK=%d\r\n",k);
    }
//	HI_PRINT("--------the last KKKKKK=%d\r\n",k);
    k = 0;
    for (j = 0; j < u32Mode; j++)
    {
		for (i = 0; i < u32Cnt; i++) // 77/2=38.5 (0,1)...(74,75)  76 remanent
		{   // the last number is 38
			if (i == u32Cnt - 1) //950~959  the last 10 numbers
			{
				//even
				pBitCoef->stBit[u32Cnt*j+i].E_bits_13 = 0;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_12 = u16TmpRGB[k]; k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_11 = u16TmpRGB[k]; k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[u32Cnt*j+i].E_bits_101 = u32Tmp;       //low
				pBitCoef->stBit[u32Cnt*j+i].E_bits_109 =(u32Tmp >>1); k++; //high

				pBitCoef->stBit[u32Cnt*j+i].E_bits_9 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_8 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_7 = u16TmpRGB[k];  k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[u32Cnt*j+i].E_bits_69 = u32Tmp;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_61 =(u32Tmp >> 9); k++;

				pBitCoef->stBit[u32Cnt*j+i].E_bits_5 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_4 = u16TmpRGB[k];  k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[u32Cnt*j+i].E_bits_37 = u32Tmp;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_33 =(u32Tmp >> 7); k++;
			}
			else
			{
				//even
				pBitCoef->stBit[u32Cnt*j+i].E_bits_13 = 0;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_12 = u16TmpRGB[k]; k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_11 = u16TmpRGB[k]; k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[i].E_bits_101 = u32Tmp;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_109 =(u32Tmp >> 1);k++;

				pBitCoef->stBit[u32Cnt*j+i].E_bits_9 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_8 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_7 = u16TmpRGB[k];  k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[u32Cnt*j+i].E_bits_69 = u32Tmp;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_61 = (u32Tmp >> 9);k++;

				pBitCoef->stBit[u32Cnt*j+i].E_bits_5 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_4 = u16TmpRGB[k];  k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[u32Cnt*j+i].E_bits_37 = u32Tmp;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_33 = (u32Tmp >> 7);k++;

				pBitCoef->stBit[u32Cnt*j+i].E_bits_2 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_1 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].E_bits_0T = (u16TmpRGB[k]) >> 5;

				//odd
				pBitCoef->stBit[u32Cnt*j+i].O_bits_13 = 0;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_12B = u16TmpRGB[k];k++;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_11 = u16TmpRGB[k]; k++;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_10 = u16TmpRGB[k]; k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[u32Cnt*j+i].O_bits_96 = u32Tmp;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_94 = (u32Tmp >> 6);k++;

				pBitCoef->stBit[u32Cnt*j+i].O_bits_8 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_7 = u16TmpRGB[k];  k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[u32Cnt*j+i].O_bits_64 = u32Tmp;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_66 = (u32Tmp >> 4);k++;

				pBitCoef->stBit[u32Cnt*j+i].O_bits_5 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_4 = u16TmpRGB[k];  k++;

				u32Tmp = u16TmpRGB[k];
				pBitCoef->stBit[u32Cnt*j+i].O_bits_32 = u32Tmp;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_38 = (u32Tmp >> 2);k++;

				pBitCoef->stBit[u32Cnt*j+i].O_bits_2 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_1 = u16TmpRGB[k];  k++;
				pBitCoef->stBit[u32Cnt*j+i].O_bits_0 = u16TmpRGB[k];  k++;
			}
//		HI_PRINT("--------the last KKKKKKKKKKKKKKKKKKKKKKKK=%d\r\n",k);
		}
    }
    return HI_SUCCESS;
}


HI_S32 UpdateGmmCoef(ALG_GMM_MEM_S* pstGmmCoefMem, const HI_U16* pu16GmmCoef,  ALG_GMM_CS_MODE_E enCSMode)
{
    HI_U32* pu32CurAddr;
    HI_U32 u32PhyAddr;
    //ALG_ACM_COEF_ADDR_S *pstAddrTmp;

    memset(&stArray, 0, sizeof(GMM_COEF_BITARRAY_S));
	
    if (enCSMode == GMM_CS_RGB) //RGB
    {
        pu32CurAddr = (HI_U32*)pstGmmCoefMem->stMBufRGB.u32StartVirAddr; //suppositional address
        u32PhyAddr = pstGmmCoefMem->stMBufRGB.u32StartPhyAddr; //physical address
	//		pstAddrTmp = &(pstGmmCoefMem->stGmmCoefAddr);
        GmmTransCoefAlign(pu16GmmCoef, &stArray); //transform the table to struct
        stArray.u32Size = CoefCntNum * sizeof(GMM_COEF_BIT_S); //four mode
        memcpy((HI_U8*)(pu32CurAddr), stArray.stBit, stArray.u32Size); //memcpy copy to suppositional address
        
        //evaluate to stArray in function transfer
        //RGB 
        pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB0 = u32PhyAddr;
        pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB1 = u32PhyAddr + (stArray.u32Size) / 4;
        pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB2 = u32PhyAddr + (stArray.u32Size) * 2 / 4;
        pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB3 = u32PhyAddr + (stArray.u32Size) * 3 / 4;
    }
    else//YUV
    {
        pu32CurAddr = (HI_U32*)pstGmmCoefMem->stMBufYUV.u32StartVirAddr;
        u32PhyAddr = pstGmmCoefMem->stMBufYUV.u32StartPhyAddr;
        GmmTransCoefAlign(pu16GmmCoef, &stArray);
	//	GmmTransCoefAlign(&s_GmmCoef_YUV[0][0], &stArray);
        stArray.u32Size = CoefCntNum * sizeof(GMM_COEF_BIT_S); 
        memcpy((HI_U8*)(pu32CurAddr), stArray.stBit, stArray.u32Size);
        pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV0 = u32PhyAddr;
        pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV1 = u32PhyAddr + (stArray.u32Size) / 4;
        pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV2 = u32PhyAddr + (stArray.u32Size) * 2 / 4;
        pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV3 = u32PhyAddr + (stArray.u32Size) * 3 / 4;
    }

//for debug HI_PRINT address
//    HI_PRINT("\r\n-u32PhyAddr = %x-,set value = %d-,alg value = %d\r\n",(HI_U32)(pstGmmCoefMem->stMBufRGB.u32StartPhyAddr),*(HI_U32* )pu16GmmCoef,*(HI_U32*)(pu32CurAddr));
    return HI_SUCCESS;
}


HI_S32 ALG_GmmInit(ALG_GMM_MEM_S* pstGmmCoefMem)
{
    HI_U32 u32PhyAddr;
	HI_U16* pu16GmmCoef;
    //GMM_COEF_BITARRAY_S stArray;
    memset(&stArray, 0, sizeof( stArray));
	stArray.u32Size = CoefCntNum * sizeof(GMM_COEF_BIT_S);
    
    //RGB
    u32PhyAddr = pstGmmCoefMem->stMBufRGB.u32StartPhyAddr; //physical address
    pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB0 = u32PhyAddr;
    pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB1 = u32PhyAddr + (stArray.u32Size) / 4;
    pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB2 = u32PhyAddr + (stArray.u32Size) * 2 / 4;
    pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB3 = u32PhyAddr + (stArray.u32Size) * 3 / 4;

	pu16GmmCoef = &(stGamRgbArray[0][0][0]);    
	UpdateGmmCoef(pstGmmCoefMem, pu16GmmCoef,	0);


    //YUV
    u32PhyAddr = pstGmmCoefMem->stMBufYUV.u32StartPhyAddr; //physical address
    pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV0 = u32PhyAddr;
    pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV1 = u32PhyAddr + (stArray.u32Size) / 4;
    pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV2 = u32PhyAddr + (stArray.u32Size) * 2 / 4;
    pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV3 = u32PhyAddr + (stArray.u32Size) * 3 / 4;
//for debug HI_PRINT address
#if 0
	HI_PRINT("&&&&&&&&&&&&&&&&&&&&&&add0=0x%8x, add1=0x%8x, add2=0x%8x, add3=0x%8x,\n", 
		pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV0,
		pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV1,
		pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV2,
		pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV3);
#endif
	pu16GmmCoef = &(stGamYuvArray[0][0][0]);
	UpdateGmmCoef(pstGmmCoefMem, pu16GmmCoef,	1);

    return HI_SUCCESS;
}


HI_S32 ALG_GmmDeInit(ALG_GMM_MEM_S* pstGmmCoefMem)
{
    ALG_BUFFER_S* pstMBuf;

    pstMBuf = &(pstGmmCoefMem->stMBufRGB);
    //release gmm coefficient memory
    if (pstMBuf->u32StartVirAddr != 0)
    {
        //release memory
        //xxxx();
        pstMBuf->u32StartVirAddr = 0;
    }

    pstMBuf = &(pstGmmCoefMem->stMBufYUV);
    //release gmm coefficient memory
    if (pstMBuf->u32StartVirAddr != 0)
    {
        //release memory
        //xxxx();
        pstMBuf->u32StartVirAddr = 0;
    }

    return HI_SUCCESS;
}


HI_S32 ALG_GmmThdSet(ALG_GMM_MEM_S* pstGmmCoefMem, ALG_GMM_DRV_PARA_S* pstGmmDrvPara, ALG_GMM_RTL_PARA_S* pstGmmRtlPara)
{
    pstGmmRtlPara->bGmmEn = pstGmmDrvPara->bGmmEn;

    if (pstGmmDrvPara->enCSMode == GMM_CS_RGB) //RGB mode ,need modify the condition.
    {
        switch (pstGmmDrvPara->enGmmMode)
        {
            case GMM_MODE_0:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB0;
                break;
            case GMM_MODE_1:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB1;
                break;
            case GMM_MODE_2:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB2;
                break;
            case GMM_MODE_3:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB3;
                break;
            default:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrRGB0;
        }
    }
    else
    {
        switch (pstGmmDrvPara->enGmmMode)
        {
            case GMM_MODE_0:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV0;
                break;
            case GMM_MODE_1:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV1;
                break;
            case GMM_MODE_2:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV2;
                break;
            case GMM_MODE_3:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV3;
                break;
            default:
                pstGmmRtlPara->u32GmmCoefAddr = pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV0;
        }
    }

//for debug HI_PRINT address
#if 0
	HI_PRINT("addr_cur=0x%8x\n", pstGmmRtlPara->u32GmmCoefAddr );
	HI_PRINT("add0=0x%8x, add1=0x%8x, add2=0x%8x, add3=0x%8x,\n", 
		pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV0,
		pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV1,
		pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV2,
		pstGmmCoefMem->stGmmCoefAddr.u32GmmCoefAddrYUV3);
#endif

    return HI_SUCCESS;
}


