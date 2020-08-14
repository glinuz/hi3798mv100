#include "base.h"

/*************************************************************************
                        3.0  ir module
*************************************************************************/

#define IR_STD_NUM 4

typedef struct IrRegSetting
{
    HI_U32 u32IrCfg;
    HI_U32 u32Leads;
    HI_U32 u32Leade;
    HI_U32 u32SLeade;
    HI_U32 u32B0;
    HI_U32 u32B1;       
}IRREGSETTING;

code IRREGSETTING irRegSetting[IR_STD_NUM] = { {0x00001f17, 0x033c03cc, 0x019e01e6, 0x00b4010e, 0x002d0043, 0x008700cb}, //nec simple
                                       {0x00005f17, 0x019e01e6, 0x019e01e6, 0x00000000, 0x002d0043, 0x008700cb},//tc9012
                                       {0x00009f17, 0x012a01b5, 0x009300d8, 0x00000000, 0x0014003c, 0x006400b4},//nec full
                                       {0x0000cb17, 0x00c00120, 0x00300048, 0x00000000, 0x00300048, 0x00600090}};//sony 12bit


/* valiable*/
HI_U8 ir_type;
HI_U8 ir_pmocnum;
idata DATA_U32_S irPmocHval[IR_PMOC_NUM];
idata DATA_U32_S irPmocLval[IR_PMOC_NUM];

/*************************************************************************
                        3.1  raw module
*************************************************************************/
typedef struct necIrFmt
{
    HI_U16 lead_s_up;
    HI_U16 lead_s_low;
    HI_U16 lead_e_up;
    HI_U16 lead_e_low;
    HI_U16 lead2_s_up;
    HI_U16 lead2_s_low;
    HI_U16 lead2_e_up;
    HI_U16 lead2_e_low;
    HI_U8  bit0_s_up;
    HI_U8  bit0_s_low;
    HI_U8  bit0_e_up;
    HI_U8  bit0_e_low;
    HI_U8  bit1_s_up;
    HI_U8  bit1_s_low;
    HI_U8  bit1_e_up;
    HI_U8  bit1_e_low;
    HI_U8  bits_num;
}NECIRFMT;

typedef struct rc6IrFmt
{
    HI_U16 lead_s_up;      /* max mark time of the lead pulse   */
    HI_U16 lead_s_low;     /* min mark time of the lead pulse   */
    HI_U16 lead_e_up;      /* max space time of the lead pulse  */
    HI_U16 lead_e_low;     /* min space time of the lead pulse  */
    HI_U8  bit_up;         /* max mark/space time of normal bit */
    HI_U8  bit_low;        /* min mark/space time of normal bit */
    HI_U8  bit_wth;        /* mark/space time of normal bit     */
    HI_U8  tr_up;          /* max mark/space time of trailer bit*/
    HI_U8  tr_low;         /* min mark/space time of trailer bit*/
    HI_U8  tr_wth;         /* mark/space time of trailer bit    */
    HI_U8  bits_num;       /* need valid bits number            */
    HI_U8  bits_rvs;       /* trailer bit position              */
}RC6IRFMT;

typedef struct rc5IrFmt
{
    HI_U8 bit_up;          /* max mark/space time of bit */
    HI_U8 bit_low;         /* min mark/space time of bit */
    HI_U8 bit_wth;         /* mark/space time of bit     */
    HI_U8 bits_num;        /* need valid bits number     */
    HI_U8 bits_rvs;        /* toggle bit                 */
}RC5IRFMT;

#define IR_TYPE_NEC 0x0
#define IR_TYPE_RC6 0x1
#define IR_TYPE_RC5 0x2

#define PARSE_LAST_SYMBOL 0x3
#define PARSE_CUR_SYMBOL 0x2
#define PARSE_NEXT_SYMBOL 0x1

/* symbol parsed  */
HI_U8 rsvstat = PARSE_CUR_SYMBOL;

/* raw ir protocol type */
HI_U8 irfmt = IR_TYPE_NEC;

/* sub protocol index */
HI_U8 irnum = 0; 

/* received bit number */
HI_U8 irrcvnum = 0;

/* calculated symbol value */
HI_U8 rcxval0  = 0;
HI_U8 rcxval1  = 0;

/* current bit val: 0 or 1 */
HI_U8 bitval = 0;

/* received symbol value */
HI_U8 regValLow = 0;
HI_U8 regValHigh = 0;

#define IR_NEC_NUM 0x4
#define IR_RC6_NUM 0x1
#define IR_RC5_NUM 0x1
#define key_match(max, min, val) (((max) >= (val)) && ((val) >= (min)))

DATA_U32_S irRawLKey;
DATA_U32_S irRawHKey;
DATA_U32_S irRsvLKey;
DATA_U32_S irRsvHKey;
code NECIRFMT irfmtnec[IR_NEC_NUM] = { {650, 350, 650, 350, 0, 0, 0, 0, 33, 17, 67, 35, 65, 35, 132, 70, 12},
                                       {1080, 720, 540, 360, 0, 0, 0, 0, 73, 39, 73, 39, 73, 39, 220, 118, 32}, //nec simple
                                       /*{437, 298, 216, 147, 437, 298, 216, 147, 73, 39, 73, 39, 73, 39, 146, 78, 40}, nec simple 2header*/
                                       {437, 298, 216, 147, 0, 0, 0, 0, 60, 20, 60, 20, 60, 20, 180, 100, 48}, //necfull
                                       {660, 540, 220, 180, 400, 327, 400, 327, 80, 30, 80, 30, 80, 30, 146, 78, 40}
                                     };
                                       
/* default rc6 32bit, can change it to rc6 16 bit:          */  
/*{ 320, 213, 107, 71, 61, 33, 47, 115, 62, 89, 21, 16},    */                                     
code RC6IRFMT irfmtrc6[IR_RC6_NUM] = { { 320, 213, 107, 71, 61, 33, 47, 115, 62, 89, 37, 15} };
code RC5IRFMT irfmtrc5[IR_RC5_NUM] = { { 116, 62, 89, 14, 11}};

void IR_Disable(void)
{
    regData.val32 = 0;
    regAddr.val32 = IR_BASE_ADDR + IR_EN;
    write_regVal();

    /* Reset IR and close IR clock */
    regAddr.val32 = CFG_BASE_ADDR + MCU_SRST_CTRL;
    read_regVal();
    regData.val8[3] |= 0x20;//bit 5 to 1
    regData.val8[3] &= 0xef;//bit 4 to 0
    write_regVal();

    return;
}

void IR_Start(void)
{
    regData.val32 = 0x01;
    regAddr.val32 = IR_BASE_ADDR + IR_START;
    write_regVal();

    return;
}

void ir_raw_reset(void)
{ 
    irfmt = 0xff;
    irnum = 0xff;
    irrcvnum = 0;
    irRawLKey.val32 = 0;
    irRawHKey.val32 = 0;

    regValLow = 0;
    regValHigh = 0;
    
    rcxval0 = 0;
    rcxval1 = 0;
    bitval	= 1;
    irRsvLKey.val32 = 0xff;
    irRsvHKey.val32 = 0xff;

    return;
}

void set_data_bit(HI_U8 pos)
{
    /* should use this param, announce it to u32  */
    /* can't directly use digit '1' to left shift */
    HI_U32 u32Tmp = 1;

    if (pos < 32)
	{
		irRawLKey.val32 |= (u32Tmp << pos);
	}
    else if (pos < 64)
	{
		irRawHKey.val32 |= (u32Tmp << (pos - 32));
	}

    //printf_hex(pos);
    //printf_val(irRawHKey);
    
    return;
}

#define rec_data_bit(pos, val) do \
    { \
        if (val) \
        { \
            set_data_bit(pos); \
        } \
    } while (0)

void ir_rc_parse_symbol(HI_U8 bit_up, HI_U8 bit_low, HI_U8 bit_wth, HI_U8 bits_num)
{
    if (!key_match(bit_up, bit_low, rcxval1))
    {
        /* rcxval1 is illegal, reset all parsed parameter */
        /*CNcomment: rcxval1 值非法，所有解析参数复位     */
        if (rcxval1 < bit_up)
        {
            ir_raw_reset();
            return;
        }

        /* rcxval1 is bigger than width, use the width part and rcxval0 to build one bit */
        /* save the left value to rcxval0, wait for next cycle to handle                 */
        /*CNcomment: rcxval1值超过 width, 将其中的width和rcxval0组成一个bit，剩下的值放到rcxval0中，等待下一次处理 */
        rcxval0 = rcxval1 - bit_wth;
        rcxval1 = 0;
        rsvstat--;
    }
    else
    {
        /* rcxval1 is in the range of width, use it and rcxval0 to build one bit */
        /*CNcomment:  rcxval1 在 width范围内，和rcxval0组成了一个bit             */
        rsvstat = rsvstat - 2;
        rcxval0 = 0;
        rcxval1 = 0;
    }
    
    /* put the parsed bit to the parameter 'rawkey', from high to low bit */
    /*CNcomment:  将解析出的bit值放到rawkey中，从高位到低位存放           */
    rec_data_bit(bits_num - irrcvnum - 1, bitval);

    /* if rcxval0 is not zero, it means that rxcval1 is bigger than width,                           */
    /* so the value of next bit will overturn (from 1 to 0, or from 0 to 1)                          */
    /*CNcomment:  如果rcxval0不为0，说明rcxval1值超过 width，那么下一个bit值会反转(从1变0或者从0变1) */
    if (rcxval0)
    {
        bitval = 1 - bitval;
    }

    irrcvnum++;    
}

void  ir_rc_parsekey(void)
{
    HI_U8 ret = 0;

    /* initialize parse state   */
    /*CNcomment: 初始化解析状态 */
    if (rcxval0)
    {
        /* it comes from last symbol, should be one width    */
        /*CNcomment:上一个symbol遗留下来的，应该只有一个width*/
        rsvstat = PARSE_LAST_SYMBOL;
    }
    else
    {
        /* it starts from this symbol   */
        /*CNcomment: 从本symbol开始处理 */
        rsvstat = PARSE_CUR_SYMBOL;
    }

    while (rsvstat)
    {
        if (rcxval0)
        {
            if (rsvstat == PARSE_LAST_SYMBOL)
            {
                /* read the low part of this symbol */
                /*CNcomment: 读取本symbol的低位     */
                rcxval1 = regValLow;
            }
            else if (rsvstat == PARSE_CUR_SYMBOL)
            {
                /* The low part of this symbol is bigger than width, read the up part of it */
                /*CNcomment: 本symbol的低位宽度大于width, 读取本symbol的高位                */
                rcxval1 = regValHigh;
            }
            else if (rsvstat == PARSE_NEXT_SYMBOL)
            {
                /* The up part of this symbol is bigger than width, reduce the width part, then wait to handle next cycle */
                /*CNcomment: 本sysbol的高位宽度大于width，除去width部分等待下一轮处理                                    */
                break;
            }
        }
        else
        {
            if (rsvstat == PARSE_CUR_SYMBOL)
            {
                /* it starts from this symbol   */
                /*CNcomment: 从本symbol开始处理 */
                rcxval0 = regValLow;
                rcxval1 = regValHigh;
            }
            else if (rsvstat == PARSE_NEXT_SYMBOL)
            {
                /* The low part of this symbol has been parsed, the up part wait to handle next cycle */
                /*CNcomment: 本symbol的低位已经解析完，读取高位放到val0，等待下一轮处理               */
                rcxval0 = regValHigh;
                break;
            }
        }

        if (irfmt == IR_TYPE_RC6)
        {
            if (key_match(irfmtrc6[irnum].bit_up, irfmtrc6[irnum].bit_low, rcxval0))
            {
                 /* if it is normal bit        */
                 /*CNcomment: 如果是normal bit */
                 ir_rc_parse_symbol(irfmtrc6[irnum].bit_up, irfmtrc6[irnum].bit_low,
                    irfmtrc6[irnum].bit_wth, irfmtrc6[irnum].bits_num);
            }
            else if (key_match(irfmtrc6[irnum].tr_up, irfmtrc6[irnum].tr_low, rcxval0))
            {
                /* trailer bit, we handle it the same way as the normal bit */
                /*CNcomment:  trailer bit，处理方式和normal bit 完全相同    */
                ir_rc_parse_symbol(irfmtrc6[irnum].tr_up, irfmtrc6[irnum].tr_low,
                    irfmtrc6[irnum].tr_wth, irfmtrc6[irnum].bits_num);
            }
            else
            {
                /* rcxval0 is illegal, reset all parsed parameter */
                /*CNcomment: rcxval0 值非法，所有解析参数复位     */
                ir_raw_reset();
                return;
            }
        }
        else if (irfmt == IR_TYPE_RC5)
        {
            if (key_match(irfmtrc5[irnum].bit_up, irfmtrc5[irnum].bit_low, rcxval0))
            {
                 /* if it is normal bit        */
                 /*CNcomment: 如果是normal bit */
                 ir_rc_parse_symbol(irfmtrc5[irnum].bit_up, irfmtrc5[irnum].bit_low,
                    irfmtrc5[irnum].bit_wth, irfmtrc5[irnum].bits_num);
            }
            else
            {
                /* rcxval0 is illegal, reset all parsed parameter */
                /*CNcomment: rcxval0 值非法，所有解析参数复位     */
                ir_raw_reset();
                return;
            }               
        }
        else
        {
            /* should not enter here */
            break;
        }
    }

    return;
}

void ir_nec_parsekey(void)
{
    if (key_match(irfmtnec[irnum].bit0_s_up, irfmtnec[irnum].bit0_s_low, regData.val8[3])
        && key_match(irfmtnec[irnum].bit0_e_up, irfmtnec[irnum].bit0_e_low, regData.val8[1]))
    {
        /* symbol matches to 0 */
        rec_data_bit(irrcvnum, 0);
        irrcvnum++;
    }
    else if (key_match(irfmtnec[irnum].bit1_s_up, irfmtnec[irnum].bit1_s_low, regData.val8[3])
             && key_match(irfmtnec[irnum].bit1_e_up, irfmtnec[irnum].bit1_e_low, regData.val8[1]))
    {
        /* symbol matches to 1 */
        rec_data_bit(irrcvnum, 1);
        irrcvnum++;
    }
    else if (key_match(irfmtnec[irnum].lead2_s_up, irfmtnec[irnum].lead2_s_low, regData.val16[1])
             && key_match(irfmtnec[irnum].lead2_e_up, irfmtnec[irnum].lead2_e_low, regData.val16[0]))
    {
        /* symbol matches to second head */
        ;
    }
    else
    {
        /* not match, reset all data */
        ir_raw_reset();
        return;
    }

    return;
}

/* check lead code to judge ir protocol type */
void ir_raw_getfmtval(void)
{
    HI_U8 i;

    for (i = 0; i < IR_NEC_NUM; i++)
    {
        if (key_match(irfmtnec[i].lead_s_up, irfmtnec[i].lead_s_low, regData.val16[1])
            && key_match(irfmtnec[i].lead_e_up, irfmtnec[i].lead_e_low, regData.val16[0]))
        {
            irrcvnum = 0;
            irfmt = IR_TYPE_NEC;
            irnum = i;
            return;
        }
    }

    for (i = 0; i < IR_RC6_NUM; i++)
    {
        if (key_match(irfmtrc6[i].lead_s_up, irfmtrc6[i].lead_s_low, regData.val16[1])
            && key_match(irfmtrc6[i].lead_e_up, irfmtrc6[i].lead_e_low, regData.val16[0]))
        {
            irrcvnum = 0;
            irfmt = IR_TYPE_RC6;
            irnum = i;
            rcxval0 = 0;
            rcxval1 = 0;
            bitval	= 1;
            return;
        }
    }

    for (i = 0; i < IR_RC5_NUM; i++)
    {
        if (key_match(irfmtrc5[i].bit_up, irfmtrc5[i].bit_low, regData.val8[3])
            && key_match(irfmtrc5[i].bit_up, irfmtrc5[i].bit_low, regData.val8[1]))
        {
            irrcvnum = 0;
            irfmt = IR_TYPE_RC5;
            irnum = i;

            /* RC5 has no lead code. The first half symbol of rc5 is space.*/
            /* And we can't recognize it, so we add it manually.           */
            rcxval0 = irfmtrc5[i].bit_wth; 
            rcxval1 = 0;
            bitval	= 1;
            ir_rc_parsekey();
            return;
        }
    }

    return;
}

void ir_rcx_posthandle(HI_U8 pos)
{
    HI_U32 u32Tmp = 1;
    
    irRsvLKey.val32 = irRawLKey.val32;
    irRsvHKey.val32 = irRawHKey.val32;

    if (pos < 32)
	{
		if (irRawLKey.val32 & (u32Tmp << pos))
		{
			irRsvLKey.val32 &= ~(u32Tmp << pos);
		}
		else
		{
			irRsvLKey.val32 |= (u32Tmp << pos);
		}
	}
	else if (pos < 64)
	{
		pos = pos - 32;
		if (irRawHKey.val32 & (u32Tmp << pos))
		{
			irRsvHKey.val32 &= ~(u32Tmp << pos);
		}
		else
		{
			irRsvHKey.val32 |= (u32Tmp << pos);
		}
	}

    return;
}

void ir_raw_cmpval(void)
{
    HI_U8 i;

	/* add to avoid error when irfmt is 0xff */
	if (0 == irrcvnum)
    {
        return;
    }

    if (irfmt == IR_TYPE_NEC)
    {
        if (irfmtnec[irnum].bits_num != irrcvnum)
        {
            return;
        }
    }

    if (irfmt == IR_TYPE_RC6)
    {
        if (irfmtrc6[irnum].bits_num != irrcvnum)
        {
            return;
        }

        if (irfmtrc6[irnum].bits_rvs != 0xff)
        {
            /* handle the toggle bit         */
            /*CNcomment: 处理码值中的翻转bit */
            ir_rcx_posthandle(irfmtrc6[irnum].bits_rvs);
        }
    }

    if (irfmt == IR_TYPE_RC5)
    {
        if (irfmtrc5[irnum].bits_num != irrcvnum)
        {
            return;
        }

        if (irfmtrc5[irnum].bits_rvs != 0xff)
        {
            /* mask the high 3bit: 2 start and 1 toggle bit */
			irRawLKey.val8[2] &= 0x07;
        }
    }
    
    //printf_val(irRawLKey);
    //printf_val(irRawHKey);
    //printf_val(irRsvLKey);
    
    for (i = 0; i < ir_pmocnum; i++)
    {
        if (((irPmocLval[i].val32 == irRawLKey.val32) && (irPmocHval[i].val32 == irRawHKey.val32))
            || ((irPmocLval[i].val32 == irRsvLKey.val32) && (irPmocHval[i].val32 == irRsvHKey.val32)))
        {
            /* save power key to ram */
            regAddr.val32 = DATA_WAKEUPKEYLOW;
            regData.val32 = irPmocLval[i].val32;
            write_regValEx();

            regAddr.val32 = DATA_WAKEUPKEYHIGH;
            regData.val32 = irPmocHval[i].val32;
            write_regValEx();
            
            pmocType = HI_UNF_PMOC_WKUP_IR;
            pmocflag = HI_TRUE;
            break;
        }
    }

    if (i >= ir_pmocnum)
    {
        ir_raw_reset();
    }

    return;
}

void ir_raw_isr()
{
    HI_U8 cnt;
    HI_U8 isrflg;

    regAddr.val32 = IR_BASE_ADDR + IR_INT_STATUS;
    read_regVal();
    isrflg = regData.val8[0];

    if ((isrflg & 0x07) == 0)
    {
        return;
    }

    /* get symbol count */
    regAddr.val32 = IR_BASE_ADDR + IR_DATAH;
    read_regVal();
    cnt = regData.val8[3] & 0x3f;
    
    /* overflow interrupt */
    if (isrflg & 0x04)
    {
        /* clear symbol fifo */
        regAddr.val32 = IR_BASE_ADDR + IR_DATAL;
        while (cnt > 0)
        {
            read_regVal();
            cnt--;
        }
    }
    else //symbol or timeout interrupt
    {
        /* get symbol from fifo */
        while (cnt-- > 0)
        {
            regAddr.val32 = IR_BASE_ADDR + IR_DATAL;
            read_regVal();

            /* save the symbol data to global variables */
            regValLow = regData.val8[3];           
            regValHigh = regData.val8[1]; 

            if (irfmt != 0xff)
            {
                if (irfmt == IR_TYPE_NEC)
                {
                    ir_nec_parsekey();
                }

                if ((irfmt == IR_TYPE_RC5) || (irfmt == IR_TYPE_RC6))
                {
                    ir_rc_parsekey();
                }
            }
            else
            {
                /* get the type of ir protocol */
                ir_raw_getfmtval();
                continue;
            }

            /* compare received key to wake up key */
            ir_raw_cmpval();
            if (pmocflag)
            {
                break;
            }
        }          

        /* overtime int, the last symbol. After handle it, reset all parameter */
        if(isrflg & 0x2)
        {
            ir_raw_reset();
        }           
        
    }
    
    /* clear interrupt */
    regData.val32 = 0x00070000;
    regAddr.val32 = IR_BASE_ADDR + IR_INT_CLR;
    write_regVal();
    
    return;
}

/***************************************************************************************/
void ir_std_isr()
{
    regAddr.val32 = IR_BASE_ADDR + IR_INT_STATUS;
    read_regVal();

    if ((regData.val8[1] & 0x0f) == 0)
    {
        return;
    }

    /* if receive data */
    if (regData.val8[1] & 0x01)
    {
        /* read low 32 bit of ir key */
        regAddr.val32 = IR_BASE_ADDR + IR_DATAL;
        read_regVal();
        if (regData.val32 == irPmocLval[0].val32)
        {
            /* save power key to ram */
            regAddr.val32 = DATA_WAKEUPKEYLOW;
            regData.val32 = irPmocLval[0].val32;
            write_regValEx();

            regAddr.val32 = DATA_WAKEUPKEYHIGH;
            regData.val32 = irPmocHval[0].val32;
            write_regValEx();
            
            pmocType = HI_UNF_PMOC_WKUP_IR;
            pmocflag = HI_TRUE;
        }
    }
    
    regData.val32 = 0x0f;
    regAddr.val32 = IR_BASE_ADDR + IR_INT_CLR;
    write_regVal();

    return;
}

void IR_Init(void)
{
   /* open IR clock */
    regAddr.val32 = CFG_BASE_ADDR + MCU_SRST_CTRL;
    read_regVal();
    regData.val8[3] |= 0x10;//bit 4 to 1
    regData.val8[3] &= 0xdf;//bit 5 to 0
    write_regVal();
    
    //enable IR
    regData.val32 = 0x1;
    regAddr.val32 = IR_BASE_ADDR + IR_EN;
    write_regVal();
    
    // 0  busy
    regAddr.val32 = IR_BASE_ADDR + IR_BUSY;
    do
    {
        wait_minute_1(10);
        read_regVal();
    } while (regData.val8[3]);

    // 1  cfg
    if (ir_type < IR_STD_NUM)
    {
        regData.val32 = irRegSetting[ir_type].u32IrCfg;
    }
    else 
    {
        regData.val32 = 0x3e800197;
    }
    regAddr.val32 = IR_BASE_ADDR + IR_CFG;
    write_regVal();

    /* raw type do not need set leads, leade and so on */
    if (ir_type >= IR_STD_NUM)
    {
        goto over;
    }

    // 2 leads
    regData.val32 = irRegSetting[ir_type].u32Leads;
    regAddr.val32 = IR_BASE_ADDR + IR_LEADS;
    write_regVal();

    // 3 leade
    regData.val32 = irRegSetting[ir_type].u32Leade;
    regAddr.val32 = IR_BASE_ADDR + IR_LEADE;
    write_regVal();

    // 4 sleade
    regData.val32 = irRegSetting[ir_type].u32SLeade;
    regAddr.val32 = IR_BASE_ADDR + IR_SLEADE;
    write_regVal();

    // 5 b0
    regData.val32 = irRegSetting[ir_type].u32B0;
    regAddr.val32 = IR_BASE_ADDR + IR_B0;
    write_regVal();

    // 6 b1
    regData.val32 = irRegSetting[ir_type].u32B1;
    regAddr.val32 = IR_BASE_ADDR + IR_B1;
    write_regVal();

over:
    // 7 int mask
    if (ir_type < IR_STD_NUM)
    {
        regData.val32 = 0x0007000e;
    }
    else
    {
        regData.val32 = 0x0f;
    }
    regAddr.val32 = IR_BASE_ADDR + IR_INT_MASK;
    write_regVal();

    if (ir_type >= IR_STD_NUM)
    {
        ir_raw_reset();
    }
    
    return;
}

void IR_Isr(void)
{
    if (ir_type < IR_STD_NUM)
    {
        ir_std_isr();
    }
    else
    {
        ir_raw_isr();
    }

    return;
}

