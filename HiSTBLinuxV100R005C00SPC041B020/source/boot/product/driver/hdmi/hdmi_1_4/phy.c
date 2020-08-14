#include "phy.h"
#include "defstx.h"
#include "hdmitx.h"
#include "siitxapidefs.h"
#include "hi_unf_hdmi.h"

// some reg
// for 3716cv200EC Version bit15~bit8 ori:0x40,ec:0x41
#define EC_3716CV200_VER      0xF8a9007c


HI_BOOL Is3716Cv200ECVer(void)
{
    HI_U32 u32Reg;
    u32Reg = HDMI_REG_READ(EC_3716CV200_VER);

    u32Reg = (u32Reg & 0xff00) >> 8;
    HDMIPrint("3716cv200 EC Version tag:0x%02x \n",u32Reg);

    if(u32Reg == 0x41)
    {
        HDMIPrint("3716cv200 EC Version \n");
        return HI_TRUE;
    }
    else if(u32Reg == 0x40)
    {
        HDMIPrint("3716cv200 original Version \n");
        return HI_FALSE;
    }
    else
    {
        HDMIPrint("unKnown 3716cv200 version,force to 3716cv200 default \n");
        return HI_FALSE;
    }
}


HI_S32 TX_PHY_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value)
{
    HI_U32 u32Ret = HI_SUCCESS;

    HDMIPrint("Writing Phy[0x%02x] :val[0x%02x] \n",u32RegAddr,u32Value);
    HDMI_REG_WRITE((HI_U32)(HDMI_TX_PHY_ADDR + u32RegAddr * 4),u32Value);

    return u32Ret;
}

HI_S32 TX_PHY_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    HI_U32 u32Ret = HI_SUCCESS;

    *pu32Value = HDMI_REG_READ((HI_U32)(HDMI_TX_PHY_ADDR + u32RegAddr * 4));

    return u32Ret;
}

HI_S32 TX_PHY_HighBandwidth(HI_BOOL bTermEn)
{
    HI_U32 u32Ret = HI_SUCCESS;
    

#if defined(BOARD_TYPE_S40V2_fpga)
    HI_U32 u32phyreg = 0;
    if(HI_TRUE == bTermEn)
    {
        HDMIPrint("From Jing:TMDS_CTL2[4]:term_en must be set to 1 for HDMI Eye-Diagram test\n");
        //set TMDS CNTL2 [4] to 1
        TX_PHY_ReadRegister(0x1,&u32phyreg);
        u32phyreg |= 0x10;
        HDMIPrint("writing phy 0x%x HighBandwidth 1\n",u32phyreg);
        TX_PHY_WriteRegister(0x1, u32phyreg);
        TX_PHY_ReadRegister(0x1, &u32phyreg);
        HDMIPrint("High Bandwith 0x10171804:0x%x\n", u32phyreg);    //0x10171804
    }
    else
    {
        TX_PHY_ReadRegister(0x1,&u32phyreg);
        u32phyreg &= 0xef;
        HDMIPrint("writing phy 0x%x HighBandwidth 0\n",u32phyreg);
        TX_PHY_WriteRegister(0x1, u32phyreg);
        TX_PHY_ReadRegister(0x1,&u32phyreg);
        HDMIPrint("low Bandwith 0x10171804:0x%x\n", u32phyreg);
    }
#elif  defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)
    HI_U32 u32phyreg = 0;
    //新phy
    if(HI_TRUE == bTermEn)
    {
        HDMIPrint("From Jing:TMDS_CTL2[4]:term_en must be set to 1 for HDMI Eye-Diagram test\n");
        //set TMDS CNTL2 [4] to 1
        TX_PHY_ReadRegister(0x0e,&u32phyreg);
        u32phyreg |= 0x01;
        HDMIPrint("writing phy 0x%x term_en 1\n",u32phyreg);
        TX_PHY_WriteRegister(0x0e, u32phyreg);
        TX_PHY_ReadRegister(0x0e, &u32phyreg);
        HDMIPrint("term_en high 0x%x\n", u32phyreg);    //0x10171804
    }
    else
    {
        TX_PHY_ReadRegister(0x0e,&u32phyreg);
        u32phyreg &= ~0x01;
        HDMIPrint("writing phy 0x%x term_en 0\n",u32phyreg);
        TX_PHY_WriteRegister(0x0e, u32phyreg);
        TX_PHY_ReadRegister(0x0e,&u32phyreg);
        HDMIPrint("term_en low 0x%x\n", u32phyreg);
    }

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

    //HDMIPrint("No source termination ctrl 0x%x\n", u32phyreg);
#endif
    return u32Ret;
}

HI_S32 TX_PHY_GetOutPutEnable(void)
{
    HI_U32 u32Value = 0;
    HI_BOOL bOutput = HI_TRUE;

#if defined(BOARD_TYPE_S40V2_fpga)
    TX_PHY_ReadRegister(0x1,&u32Value);
    if ((u32Value & 0x20) != 0x20)
    {
        bOutput = HI_FALSE;
    }
    //bOutput = HI_TRUE;
#elif  defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    //silicon phy
    TX_PHY_ReadRegister(0x05,&u32Value);
    if ((u32Value & 0x20) != 0x20)
    {
        bOutput = HI_FALSE;
    }
    //bOutput = HI_TRUE;


#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

    TX_PHY_ReadRegister(PHY_OE_ADDR,&u32Value);
    if ((u32Value & RG_TX_RSTB) != RG_TX_RSTB)
    {
        bOutput = HI_FALSE;
    }

#endif
    return bOutput;
}

HI_S32 TX_PHY_DisableHdmiOutput(void)
{
    HI_U32 u32Reg = 0;

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    //silicon phy
    /* disable HDMI PHY Output:TMDS CNTL2 Register:oe */
    TX_PHY_ReadRegister(0x05,&u32Reg);
    u32Reg &= ~0x20;
    TX_PHY_WriteRegister(0x05, u32Reg);

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)
    /* disable HDMI PHY Output:reg 0/bit 0 */
    TX_PHY_ReadRegister(PHY_OE_ADDR,&u32Reg);
    u32Reg &= ~RG_TX_RSTB;
    TX_PHY_WriteRegister(PHY_OE_ADDR, u32Reg);
#endif

    return HI_SUCCESS;
}

HI_S32 TX_PHY_EnableHdmiOutput(void)
{
    HI_U32 u32Reg = 0;

    /* enable HDMI PHY Output:TMDS CNTL2 Register:oe */
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    //新phy
    TX_PHY_ReadRegister(0x05,&u32Reg);
    HDMIPrint("writing phy 0x%x EnableHdmiOutput\n",u32Reg);
    u32Reg |= 0x20;
    HDMIPrint("writing phy 0x%x EnableHdmiOutput\n",u32Reg);
    TX_PHY_WriteRegister(0x05, u32Reg);

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

    /* disable HDMI PHY Output:reg 0/bit 0 */
    TX_PHY_ReadRegister(PHY_OE_ADDR,&u32Reg);
    u32Reg |= RG_TX_RSTB;
    TX_PHY_WriteRegister(PHY_OE_ADDR, u32Reg);

#endif

    return HI_SUCCESS;
}

void TX_PHY_INIT(void)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    // unknown DM_TX_CTRL2
    TX_PHY_WriteRegister(0x06,0x89);
    // term_en && cap_ctl  // term_en 先关掉
    TX_PHY_WriteRegister(0x0e,0x00);
    // unknown DM_TX_CTRL3
    TX_PHY_WriteRegister(0x07,0x81);

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)
    if (Is3716Cv200ECVer())
    {
        //In Ec Version,phy has change to the 19m phy
        TX_PHY_WriteRegister(0x09,0x00);
    }
    else
    {
        //only need config with in S40v2 & 3716Cv200 series
        //there are some change with 3716cv200 series and 3719mv100 series phy.
        //so,we need not config 0x09 reg in 3719mv100 or the later phy
        // unknown DM_TX_CTRL5
        TX_PHY_WriteRegister(0x09,0x1a);
    }
#endif
    // unknown BIAS_GEN_CTRL1
    TX_PHY_WriteRegister(0x0a,0x07);
    // unknown BIAS_GEN_CTRL2
    TX_PHY_WriteRegister(0x0b,0x51);
    // pll ctrl -deep color
    TX_PHY_WriteRegister(0x02,0x24);
    // oe && pwr_down
    TX_PHY_WriteRegister(0x05,0x12);
    //unknown DM_TX_CTRL4
    TX_PHY_WriteRegister(0x08,0x40);
    // enc_bypass == nobypass
    TX_PHY_WriteRegister(0x0d,0x00);

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv310)
    // oe
    TX_PHY_WriteRegister(PHY_OE_ADDR,0x00);

    // audio clk
    TX_PHY_WriteRegister(PHY_AUD_ADDR,0x02);

    // swing & bw ctrl
    TX_PHY_WriteRegister(PHY_PLL1_ADDR,0x02);

    // deep color & pixel repeat ctrl
    TX_PHY_WriteRegister(PHY_PLL2_ADDR,0x09);

    // driver setting , slew rate ctrl
    TX_PHY_WriteRegister(PHY_DRV_ADDR,0x01);

    // clk invert
    TX_PHY_WriteRegister(PHY_CLK_ADDR,0x00);

    // I think power up at last will imporve stability of pll
    TX_PHY_WriteRegister(PHY_PWD_ADDR,0x01);
#elif  defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    // oe
    TX_PHY_WriteRegister(PHY_OE_ADDR,0x00);

    // audio clk
    TX_PHY_WriteRegister(PHY_AUD_ADDR,0x02);

    // swing & bw ctrl
    TX_PHY_WriteRegister(PHY_PLL1_ADDR,0x40);

    // deep color & pixel repeat ctrl
    TX_PHY_WriteRegister(PHY_PLL2_ADDR,0x09);

    // driver setting , slew rate ctrl
    TX_PHY_WriteRegister(PHY_DRV_ADDR,0xC0);

    // clk invert
    TX_PHY_WriteRegister(PHY_CLK_ADDR,0x00);

    // I think power up at last will imporve stability of pll
    TX_PHY_WriteRegister(PHY_PWD_ADDR,0x01);

#endif
    
}

HI_S32 TX_PHY_PowerDown(HI_BOOL bPwdown)
{
    HI_U32 u32Value = 0;
#if defined(BOARD_TYPE_S40V2_fpga)
    /* hdmi PHY */
    TX_PHY_ReadRegister(0x2,&u32Value);
    if(bPwdown)
    {
        u32Value &= ~0x01;
    }
    else
    {
        u32Value |= 0x01;
    }
    HDMIPrint("writing phy 0x%x PowerDown\n",u32Value);
    TX_PHY_WriteRegister(0x2, u32Value);
#elif  defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    //新phy
    /* hdmi PHY */
    TX_PHY_ReadRegister(0x05,&u32Value);
    if(bPwdown)
    {
        u32Value &= ~0x10;
    }
    else
    {
        u32Value |= 0x10;
    }
    HDMIPrint("writing phy 0x%x PowerDown\n",u32Value);
    TX_PHY_WriteRegister(0x05, u32Value);
#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

    TX_PHY_ReadRegister(PHY_PWD_ADDR,&u32Value);
    if (bPwdown)
    {
        u32Value &= ~RG_TX_EN;
    }
    else
    {
        u32Value |= RG_TX_EN;
    }
    HDMIPrint("writing phy 0x%x PowerDown\n",u32Value);
    TX_PHY_WriteRegister(PHY_PWD_ADDR,u32Value);

#endif

   return HI_SUCCESS;
}

HI_S32 TX_PHY_SetDeepColor(HI_U8 bDeepColor)
{
    HI_U32 u32Value = 0;
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    //新phy
    /* Config kudu IP for DeepColor*/
    TX_PHY_ReadRegister(0x02,&u32Value);

    HDMIPrint("PLL_CTRL  old walue:0x%x\n", u32Value);

    if (SiI_DeepColor_30bit == bDeepColor)
    {
        u32Value =  (u32Value & ~0x03) | 0x01;
        HDMIPrint("SiI_DeepColor_30bit\n");
    }
    else if (SiI_DeepColor_36bit == bDeepColor)
    {
        u32Value =  (u32Value & ~0x03) | 0x02;
        HDMIPrint("SiI_DeepColor_36bit\n");
    }
    else if (SiI_DeepColor_24bit == bDeepColor)
    {
        u32Value =  (u32Value & ~0x03) | 0x00;
        HDMIPrint("SiI_DeepColor_24bit(normal)\n");
    }
    else
    {
        u32Value =  (u32Value & ~0x03) | 0x00;
        HDMIPrint("SiI_DeepColor_Off\n");
    }

    HDMIPrint("writing phy 0x%x SetDeepColor\n",u32Value);

    TX_PHY_WriteRegister(0x02,u32Value);
    u32Value = 0;
    TX_PHY_ReadRegister(0x02,&u32Value);
    HDMIPrint("PLL_CTRL  new walue:0x%x\n", u32Value);

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

    /* Config kudu IP for DeepColor*/
    TX_PHY_ReadRegister(PHY_PLL2_ADDR,&u32Value);

    HDMIPrint("PLL_CTRL  old walue:0x%x\n", u32Value);

    if (SiI_DeepColor_30bit == bDeepColor)
    {
        u32Value =  (u32Value & ~MASK_DEEPCOLOR) | 0x00;
        HDMIPrint("SiI_DeepColor_30bit\n");
    }
    else if (SiI_DeepColor_36bit == bDeepColor)
    {
        u32Value =  (u32Value & ~MASK_DEEPCOLOR) | 0x02;
        HDMIPrint("SiI_DeepColor_36bit\n");
    }
    else if (SiI_DeepColor_24bit == bDeepColor)
    {
        u32Value =  (u32Value & ~MASK_DEEPCOLOR) | 0x01;
        HDMIPrint("SiI_DeepColor_24bit(normal)\n");
    }
    else
    {
        u32Value =  (u32Value & ~MASK_DEEPCOLOR) | 0x01;
        HDMIPrint("SiI_DeepColor_Off\n");
    }

    HDMIPrint("writing phy 0x%x SetDeepColor\n",u32Value);

    TX_PHY_WriteRegister(PHY_PLL2_ADDR,u32Value);
    u32Value = 0;
    TX_PHY_ReadRegister(PHY_PLL2_ADDR,&u32Value);
    HDMIPrint("PLL_CTRL  new walue:0x%x\n", u32Value);

#endif
    return HI_SUCCESS;
}

#ifdef HI_HDMI_4K_SUPPORT
HI_S32 TX_PHY_4KRisingTime(HI_BOOL b4KFmt)
{
    HI_U32 u32Ret = HI_SUCCESS;
    HI_U32 u32phyreg = 0;
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    TX_PHY_ReadRegister(0x06,&u32phyreg);
    u32phyreg &= ~0xc0;

    //新phy
    if(HI_TRUE == b4KFmt)
    {
        HDMIPrint("4KFmt need set DM_TX_CTRL2 bit[6-7] 0b01 \n");

        u32phyreg |= 0x40;
        HDMIPrint("writing phy 0x06:0x%x rising time\n",u32phyreg);
        TX_PHY_WriteRegister(0x06, u32phyreg);
        TX_PHY_ReadRegister(0x06, &u32phyreg);
        HDMIPrint("rising time 0x%x\n", u32phyreg);    //0x10171804
    }
    else
    {
        HDMIPrint("4KFmt need set DM_TX_CTRL2 bit[6-7] 0b10 \n");

        u32phyreg |= 0x80;
        HDMIPrint("writing phy 0x06:0x%x rising time\n",u32phyreg);
        TX_PHY_WriteRegister(0x06, u32phyreg);
        TX_PHY_ReadRegister(0x06,&u32phyreg);
        HDMIPrint("rising time 0x%x\n", u32phyreg);
    }

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

    HDMIPrint("need not cfg rising time 0x%x\n", u32phyreg);

#endif
    return u32Ret;
}
#endif

HI_S32 TX_PHY_DVIEncoder(HI_BOOL bEnable)
{
    return HI_SUCCESS;
}

HI_S32 TX_PHY_SwingCtrl(SWING_CTL_E enSwing)
{
#if    defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv310)
    // taiyan phy, use encoder in ctrl

    HI_U32 u32phyreg = 0;

    TX_PHY_ReadRegister(PHY_PLL1_ADDR,&u32phyreg);
    u32phyreg &= ~0x03;

    if(enSwing == SWING_300M)
    {
        u32phyreg |= 0x03;
    }
    else if(enSwing == SWING_27M)
    {
        u32phyreg |= 0x00;
    }
    else
    {
        u32phyreg |= 0x01;
    }

    HDMIPrint("writing phy PHY_PLL1_ADDR(swing):0x%x \n",u32phyreg);
    TX_PHY_WriteRegister(PHY_PLL1_ADDR, u32phyreg);

#elif  defined(CHIP_TYPE_hi3716mv420)  \
    || defined(CHIP_TYPE_hi3716mv410)
    // swing 
    HI_U32 u32phyreg = 0;

    u32phyreg = 0x43;

    TX_PHY_WriteRegister(PHY_PLL1_ADDR, u32phyreg);
    HDMIPrint("writing phy PHY_PLL1_ADDR(0x%2X:swing):0x%x ", PHY_PLL1_ADDR, u32phyreg);
#endif
    return HI_SUCCESS;
}

