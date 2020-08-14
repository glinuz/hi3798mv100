
#include "vdp_hal_ip_acc.h"
#include "vdp_hal_comm.h"


extern S_VDP_REGS_TYPE *pVdpReg;

HI_VOID VDP_VID_SetAccAccEn(HI_U32 u32Data, HI_U32 acc_en)
{
	U_VHDACCTHD1 VHDACCTHD1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccAccEn Select Wrong Layer ID\n");
	}
	
	VHDACCTHD1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET));
	VHDACCTHD1.bits.acc_en = acc_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET),VHDACCTHD1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccAccMode(HI_U32 u32Data, HI_U32 acc_mode)
{
	U_VHDACCTHD1 VHDACCTHD1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccAccMode Select Wrong Layer ID\n");
	}
	
	VHDACCTHD1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET));
	VHDACCTHD1.bits.acc_mode = acc_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET),VHDACCTHD1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccThdMedLow(HI_U32 u32Data, HI_U32 thd_med_low)
{
	U_VHDACCTHD1 VHDACCTHD1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccThdMedLow Select Wrong Layer ID\n");
	}
	
	VHDACCTHD1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET));
	VHDACCTHD1.bits.thd_med_low = thd_med_low;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET),VHDACCTHD1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccThdHigh(HI_U32 u32Data, HI_U32 thd_high)
{
	U_VHDACCTHD1 VHDACCTHD1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccThdHigh Select Wrong Layer ID\n");
	}
	
	VHDACCTHD1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET));
	VHDACCTHD1.bits.thd_high = thd_high;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET),VHDACCTHD1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccThdLow(HI_U32 u32Data, HI_U32 thd_low)
{
	U_VHDACCTHD1 VHDACCTHD1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccThdLow Select Wrong Layer ID\n");
	}
	
	VHDACCTHD1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET));
	VHDACCTHD1.bits.thd_low = thd_low;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD1.u32) + u32Data * VID_OFFSET),VHDACCTHD1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccAccRst(HI_U32 u32Data, HI_U32 acc_rst)
{
	U_VHDACCTHD2 VHDACCTHD2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccAccRst Select Wrong Layer ID\n");
	}
	
	VHDACCTHD2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET));
	VHDACCTHD2.bits.acc_rst = acc_rst;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET),VHDACCTHD2.u32);
	
	return ;
}

HI_VOID VDP_VID_SetAccDbgMode(HI_U32 u32Data, HI_U32 acc_dbg_mode)
{
	U_VHDACCTHD2 VHDACCTHD2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccAccRst Select Wrong Layer ID\n");
	}
	
	VHDACCTHD2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET));
	VHDACCTHD2.bits.acc_dbg_mode =acc_dbg_mode ;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET),VHDACCTHD2.u32);
	
	return ;
}

HI_VOID VDP_VID_SetAccDbgEn(HI_U32 u32Data, HI_U32 acc_dbg_en)
{
	U_VHDACCTHD2 VHDACCTHD2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccAccRst Select Wrong Layer ID\n");
	}
	
	VHDACCTHD2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET));
	VHDACCTHD2.bits.acc_dbg_en =acc_dbg_en ;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET),VHDACCTHD2.u32);
	
	return ;
}
HI_VOID VDP_VID_SetAccAccMultiple(HI_U32 u32Data, HI_U32 acc_multiple)
{
	U_VHDACCTHD2 VHDACCTHD2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccAccMultiple Select Wrong Layer ID\n");
	}
	
	VHDACCTHD2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET));
	VHDACCTHD2.bits.acc_multiple = acc_multiple;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET),VHDACCTHD2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccThdMedHigh(HI_U32 u32Data, HI_U32 thd_med_high)
{
	U_VHDACCTHD2 VHDACCTHD2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccThdMedHigh Select Wrong Layer ID\n");
	}
	
	VHDACCTHD2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET));
	VHDACCTHD2.bits.thd_med_high = thd_med_high;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTHD2.u32) + u32Data * VID_OFFSET),VHDACCTHD2.u32);
	
	return ;
}
HI_VOID VDP_VID_SetAccTab(HI_U32 u32Data, HI_U32 *upTable)
{
    U_VHDACCLOWN  VHDACCLOW[3];
    U_VHDACCMEDN  VHDACCMED[3];
    U_VHDACCHIGHN VHDACCHIGH[3];
    U_VHDACCMLN     VHDACCML[3];
    U_VHDACCMHN    VHDACCMH[3];

    HI_U32 ii = 0;

    switch(u32Data)   
    {
        case 0:
        {
            for (ii = 0; ii < 3; ii++)
            {
                VHDACCLOW[ii].bits.table_data1n = upTable[0 + ii*3];
                VHDACCLOW[ii].bits.table_data2n = upTable[1 + ii*3];
                VHDACCLOW[ii].bits.table_data3n = upTable[2 + ii*3];
                VDP_RegWrite((HI_ULONG)&(pVdpReg->VHDACCLOWN[ii].u32), VHDACCLOW[ii].u32);
            }
                
            for (ii = 0; ii < 3; ii++)
            {
                VHDACCMED[ii].bits.table_data1n = upTable[9 + 0 + ii*3];
                VHDACCMED[ii].bits.table_data2n = upTable[9 + 1 + ii*3];
                VHDACCMED[ii].bits.table_data3n = upTable[9 + 2 + ii*3];
                VDP_RegWrite((HI_ULONG)&(pVdpReg->VHDACCMEDN[ii].u32), VHDACCMED[ii].u32);
            }

            for (ii = 0; ii < 3; ii++)
            {
                VHDACCHIGH[ii].bits.table_data1n = upTable[18 + 0 + ii*3];
                VHDACCHIGH[ii].bits.table_data2n = upTable[18 + 1 + ii*3];
                VHDACCHIGH[ii].bits.table_data3n = upTable[18 + 2 + ii*3];
                VDP_RegWrite((HI_ULONG)&(pVdpReg->VHDACCHIGHN[ii].u32), VHDACCHIGH[ii].u32);
            }

            for (ii = 0; ii < 3; ii++)
            {
                VHDACCML[ii].bits.table_data1n = upTable[27 + 0 + ii*3];
                VHDACCML[ii].bits.table_data2n = upTable[27 + 1 + ii*3];
                VHDACCML[ii].bits.table_data3n = upTable[27 + 2 + ii*3];
                VDP_RegWrite((HI_ULONG)&(pVdpReg->VHDACCMLN[ii].u32), VHDACCML[ii].u32);
            }

            for (ii = 0; ii < 3; ii++)
            {
                VHDACCMH[ii].bits.table_data1n = upTable[36 + 0 + ii*3];
                VHDACCMH[ii].bits.table_data2n = upTable[36 + 1 + ii*3];
                VHDACCMH[ii].bits.table_data3n = upTable[36 + 2 + ii*3];
                VDP_RegWrite((HI_ULONG)&(pVdpReg->VHDACCMHN[ii].u32), VHDACCMH[ii].u32);
            }

            break;
        }
        default:
        {
            VDP_PRINT("Error! Vou_SetAccTab() Select Wrong Layer ID!\n");
            break;
        }
    }
}



HI_VOID VDP_VID_SetAccCnt3Low(HI_U32 u32Data, HI_U32 cnt3_low)
{
	U_VHDACC3LOW VHDACC3LOW;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccCnt3Low Select Wrong Layer ID\n");
	}
	
	VHDACC3LOW.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACC3LOW.u32) + u32Data * VID_OFFSET));
	VHDACC3LOW.bits.cnt3_low = cnt3_low;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACC3LOW.u32) + u32Data * VID_OFFSET),VHDACC3LOW.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccCnt3Med(HI_U32 u32Data, HI_U32 cnt3_med)
{
	U_VHDACC3MED VHDACC3MED;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccCnt3Med Select Wrong Layer ID\n");
	}
	
	VHDACC3MED.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACC3MED.u32) + u32Data * VID_OFFSET));
	VHDACC3MED.bits.cnt3_med = cnt3_med;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACC3MED.u32) + u32Data * VID_OFFSET),VHDACC3MED.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccCnt3High(HI_U32 u32Data, HI_U32 cnt3_high)
{
	U_VHDACC3HIGH VHDACC3HIGH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccCnt3High Select Wrong Layer ID\n");
	}
	
	VHDACC3HIGH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACC3HIGH.u32) + u32Data * VID_OFFSET));
	VHDACC3HIGH.bits.cnt3_high = cnt3_high;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACC3HIGH.u32) + u32Data * VID_OFFSET),VHDACC3HIGH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccCnt8MedLow(HI_U32 u32Data, HI_U32 cnt8_med_low)
{
	U_VHDACC8MLOW VHDACC8MLOW;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccCnt8MedLow Select Wrong Layer ID\n");
	}
	
	VHDACC8MLOW.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACC8MLOW.u32) + u32Data * VID_OFFSET));
	VHDACC8MLOW.bits.cnt8_med_low = cnt8_med_low;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACC8MLOW.u32) + u32Data * VID_OFFSET),VHDACC8MLOW.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccCnt8MedHigh(HI_U32 u32Data, HI_U32 cnt8_med_high)
{
	U_VHDACC8MHIGH VHDACC8MHIGH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccCnt8MedHigh Select Wrong Layer ID\n");
	}
	
	VHDACC8MHIGH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACC8MHIGH.u32) + u32Data * VID_OFFSET));
	VHDACC8MHIGH.bits.cnt8_med_high = cnt8_med_high;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACC8MHIGH.u32) + u32Data * VID_OFFSET),VHDACC8MHIGH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAccAccPixTotal(HI_U32 u32Data, HI_U32 acc_pix_total)
{
	U_VHDACCTOTAL VHDACCTOTAL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAccAccPixTotal Select Wrong Layer ID\n");
	}
	
	VHDACCTOTAL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACCTOTAL.u32) + u32Data * VID_OFFSET));
	VHDACCTOTAL.bits.acc_pix_total = acc_pix_total;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACCTOTAL.u32) + u32Data * VID_OFFSET),VHDACCTOTAL.u32);
	
	return ;
}




