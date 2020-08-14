#include "si_phy.h"
#include "si_defstx.h"
#include "si_hdmitx.h"
#include "si_delay.h"
#include "si_txapidefs.h"
#include "drv_reg_proc.h"
#include "drv_compatibility.h"
#include <linux/kernel.h>

extern HI_U32 u32PixelClk[];

HI_S32 SI_TX_PHY_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value)
{
    HI_U32 u32Ret = HI_SUCCESS;

    COM_INFO("writing phy u32RegAddr 0x%x,u32Value 0x%x \n",u32RegAddr,u32Value);
    u32Ret = DRV_HDMI_WriteRegister((HI_U32)(HDMI_TX_PHY_ADDR + u32RegAddr * 4),u32Value);

    return u32Ret;
}

HI_S32 SI_TX_PHY_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    HI_U32 u32Ret = HI_SUCCESS;

    u32Ret = DRV_HDMI_ReadRegister((HI_U32)(HDMI_TX_PHY_ADDR + u32RegAddr * 4),pu32Value);

    return u32Ret;
}

HI_S32 SI_TX_PHY_HighBandwidth(HI_BOOL bTermEn)
{
    HI_U32 u32Ret = HI_SUCCESS;
    HI_U32 u32phyreg  __attribute__((unused)) = 0;

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    //新phy
    if(HI_TRUE == bTermEn)
    {
        COM_INFO("From Jing:TMDS_CTL2[4]:term_en must be set to 1 for HDMI Eye-Diagram test\n");
        //set TMDS CNTL2 [4] to 1
        SI_TX_PHY_ReadRegister(0x0e,&u32phyreg);
        u32phyreg |= 0x01;
        COM_INFO("writing phy 0x%x term_en 1\n",u32phyreg);
        SI_TX_PHY_WriteRegister(0x0e, u32phyreg);
        SI_TX_PHY_ReadRegister(0x0e, &u32phyreg);
        COM_INFO("term_en high 0x%x\n", u32phyreg);    //0x10171804
    }
    else
    {
        SI_TX_PHY_ReadRegister(0x0e,&u32phyreg);
        u32phyreg &= ~0x01;
        COM_INFO("writing phy 0x%x term_en 0\n",u32phyreg);
        SI_TX_PHY_WriteRegister(0x0e, u32phyreg);
        SI_TX_PHY_ReadRegister(0x0e,&u32phyreg);
        COM_INFO("term_en low 0x%x\n", u32phyreg);
    }

#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    COM_INFO("No source termination ctrl 0x%x\n", u32phyreg);
#endif
    return u32Ret;
}


HI_S32 SI_TX_PHY_GetOutPutEnable(void)
{
    HI_U32 u32Value = 0;
    HI_BOOL bOutput = HI_TRUE;

    //bOutput = HI_TRUE;
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    //silicon phy
    SI_TX_PHY_ReadRegister(0x05,&u32Value);
    if ((u32Value & 0x20) != 0x20)
    {
        bOutput = HI_FALSE;
    }
    //bOutput = HI_TRUE;


#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    SI_TX_PHY_ReadRegister(PHY_OE_ADDR, &u32Value);
    if ((u32Value & RG_TX_RSTB) != RG_TX_RSTB)
    {
        bOutput = HI_FALSE;
    }

#endif
    return bOutput;
}

HI_S32 SI_TX_PHY_DisableHdmiOutput(void)
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
    SI_TX_PHY_ReadRegister(0x05,&u32Reg);
    u32Reg &= ~0x20;
    SI_TX_PHY_WriteRegister(0x05, u32Reg);

#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)
    /* disable HDMI PHY Output:reg 0/bit 0 */
    SI_TX_PHY_ReadRegister(PHY_OE_ADDR,&u32Reg);
    u32Reg &= ~RG_TX_RSTB;
    SI_TX_PHY_WriteRegister(PHY_OE_ADDR, u32Reg);
#endif

    return HI_SUCCESS;
}

HI_S32 SI_TX_PHY_EnableHdmiOutput(void)
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
    SI_TX_PHY_ReadRegister(0x05,&u32Reg);
    COM_INFO("writing phy 0x%x EnableHdmiOutput\n",u32Reg);
    u32Reg |= 0x20;
    COM_INFO("writing phy 0x%x EnableHdmiOutput\n",u32Reg);
    SI_TX_PHY_WriteRegister(0x05, u32Reg);
    
#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    /* disable HDMI PHY Output:reg 0/bit 0 */
    SI_TX_PHY_ReadRegister(PHY_OE_ADDR,&u32Reg);
    u32Reg |= RG_TX_RSTB;
    SI_TX_PHY_WriteRegister(PHY_OE_ADDR, u32Reg);

#endif

    return HI_SUCCESS;
}

void SI_TX_PHY_INIT(void)
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
    SI_TX_PHY_WriteRegister(0x06,0x89);
    // term_en && cap_ctl  // term_en 先关掉
    SI_TX_PHY_WriteRegister(0x0e,0x00);
    // unknown DM_TX_CTRL3
    SI_TX_PHY_WriteRegister(0x07,0x81);

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)
    if (Is3716Cv200ECVer())
    {
        //In Ec Version,phy has change to the 19m phy
        SI_TX_PHY_WriteRegister(0x09,0x00);
    }
    else
    {
        //only need config with in S40v2 & 3716Cv200 series
        //there are some change with 3716cv200 series and 3719mv100 series phy.
        //so,we need not config 0x09 reg in 3719mv100 or the later phy
        // unknown DM_TX_CTRL5
        SI_TX_PHY_WriteRegister(0x09,0x1a);
    }
#endif
    // unknown BIAS_GEN_CTRL1
    SI_TX_PHY_WriteRegister(0x0a,0x07);
    // unknown BIAS_GEN_CTRL2
    SI_TX_PHY_WriteRegister(0x0b,0x51);
    // pll ctrl -deep color
    SI_TX_PHY_WriteRegister(0x02,0x24);
    // oe && pwr_down
    // 初始化时不打开oe
    SI_TX_PHY_WriteRegister(0x05,0x12);
    //unknown DM_TX_CTRL4
    SI_TX_PHY_WriteRegister(0x08,0x40);
    // enc_bypass == nobypass
    SI_TX_PHY_WriteRegister(0x0d,0x00);

    // after cfg phy ok,we need reset phy pll
    SI_TX_PHY_PowerDown(HI_TRUE);
    DelayMS(1);
    //SI_TX_PHY_PowerDown(HI_FALSE);

#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310)
    // oe
    SI_TX_PHY_WriteRegister(PHY_OE_ADDR,0x00);

    // audio clk
    SI_TX_PHY_WriteRegister(PHY_AUD_ADDR,0x02);

    // swing & bw ctrl
    SI_TX_PHY_WriteRegister(PHY_PLL1_ADDR,0x02);

    // deep color & pixel repeat ctrl
    SI_TX_PHY_WriteRegister(PHY_PLL2_ADDR,0x09);

    // driver setting , slew rate ctrl
    SI_TX_PHY_WriteRegister(PHY_DRV_ADDR,0x01);

    // clk invert
    SI_TX_PHY_WriteRegister(PHY_CLK_ADDR,0x00);

    // I think power up at last will imporve stability of pll
    SI_TX_PHY_WriteRegister(PHY_PWD_ADDR,0x01);
    
#elif  defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    // oe
    SI_TX_PHY_WriteRegister(PHY_OE_ADDR,0x00);

    // audio clk
    SI_TX_PHY_WriteRegister(PHY_AUD_ADDR,0x02);

    // swing & bw ctrl
    SI_TX_PHY_WriteRegister(PHY_PLL1_ADDR,0x42);

    // deep color & pixel repeat ctrl
    SI_TX_PHY_WriteRegister(PHY_PLL2_ADDR,0x09);

    // driver setting , slew rate ctrl
    SI_TX_PHY_WriteRegister(PHY_DRV_ADDR,0x40);

    // clk invert
    SI_TX_PHY_WriteRegister(PHY_CLK_ADDR,0x40);

    // I think power up at last will imporve stability of pll
    SI_TX_PHY_WriteRegister(PHY_PWD_ADDR,0x01);

#endif
}

HI_S32 SI_TX_PHY_PowerDown(HI_BOOL bPwdown)
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
    /* hdmi PHY */
    SI_TX_PHY_ReadRegister(0x05,&u32Value);
    if(bPwdown)
    {
        u32Value &= ~0x10;
    }
    else
    {
        u32Value |= 0x10;
    }
    COM_INFO("writing phy 0x%x PowerDown\n",u32Value);
    SI_TX_PHY_WriteRegister(0x05, u32Value);

#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    SI_TX_PHY_ReadRegister(PHY_PWD_ADDR,&u32Value);
    if(bPwdown)
    {
        u32Value &= ~RG_TX_EN;
    }
    else
    {
        u32Value |= RG_TX_EN;
    }
    COM_INFO("writing phy 0x%x PowerDown\n",u32Value);
    SI_TX_PHY_WriteRegister(PHY_PWD_ADDR,u32Value);

#endif

   return HI_SUCCESS;
}

HI_S32 SI_TX_PHY_SetDeepColor(HI_U8 bDeepColor)
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
    SI_TX_PHY_ReadRegister(0x02,&u32Value);

    COM_INFO("PLL_CTRL  old walue:0x%x\n", u32Value);

    if (SiI_DeepColor_30bit == bDeepColor)
    {
        u32Value =  (u32Value & ~0x03) | 0x01;
        COM_INFO("SiI_DeepColor_30bit\n");
    }
    else if (SiI_DeepColor_36bit == bDeepColor)
    {
        u32Value =  (u32Value & ~0x03) | 0x02;
        COM_INFO("SiI_DeepColor_36bit\n");
    }
    else if (SiI_DeepColor_24bit == bDeepColor)
    {
        u32Value =  (u32Value & ~0x03) | 0x00;
        COM_INFO("SiI_DeepColor_24bit(normal)\n");
    }
    else
    {
        u32Value =  (u32Value & ~0x03) | 0x00;
        COM_INFO("SiI_DeepColor_Off\n");
    }

    COM_INFO("writing phy 0x%x SetDeepColor\n",u32Value);

    SI_TX_PHY_WriteRegister(0x02,u32Value);
    u32Value = 0;
    SI_TX_PHY_ReadRegister(0x02,&u32Value);
    COM_INFO("PLL_CTRL  new walue:0x%x\n", u32Value);


#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    /* Config kudu IP for DeepColor*/
    SI_TX_PHY_ReadRegister(PHY_PLL2_ADDR,&u32Value);

    COM_INFO("PLL_CTRL  old walue:0x%x\n", u32Value);

    if (SiI_DeepColor_30bit == bDeepColor)
    {
        u32Value =  (u32Value & ~MASK_DEEPCOLOR) | 0x00;
        COM_INFO("SiI_DeepColor_30bit\n");
    }
    else if (SiI_DeepColor_36bit == bDeepColor)
    {
        u32Value =  (u32Value & ~MASK_DEEPCOLOR) | 0x02;
        COM_INFO("SiI_DeepColor_36bit\n");
    }
    else if (SiI_DeepColor_24bit == bDeepColor)
    {
        u32Value =  (u32Value & ~MASK_DEEPCOLOR) | 0x01;
        COM_INFO("SiI_DeepColor_24bit(normal)\n");
    }
    else
    {
        u32Value =  (u32Value & ~MASK_DEEPCOLOR) | 0x01;
        COM_INFO("SiI_DeepColor_Off\n");
    }

    COM_INFO("writing phy 0x%x SetDeepColor\n",u32Value);

    SI_TX_PHY_WriteRegister(PHY_PLL2_ADDR,u32Value);
    u32Value = 0;
    SI_TX_PHY_ReadRegister(PHY_PLL2_ADDR,&u32Value);
    COM_INFO("PLL_CTRL  new walue:0x%x\n", u32Value);

#endif
    return HI_SUCCESS;
}

HI_S32 SI_TX_PHY_4KRisingTime(HI_BOOL b4KFmt)
{
    HI_U32 u32Ret = HI_SUCCESS;
    HI_U32 u32phyreg  __attribute__((unused)) = 0;

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

    SI_TX_PHY_ReadRegister(0x06, &u32phyreg);
    u32phyreg &= ~0xc0;

    //新phy
    if(HI_TRUE == b4KFmt)
    {
        COM_INFO("4KFmt need set DM_TX_CTRL2 bit[6-7] 0b01 \n");

        u32phyreg |= 0x40;
        COM_INFO("writing phy 0x06:0x%x rising time\n",u32phyreg);
        SI_TX_PHY_WriteRegister(0x06, u32phyreg);
        SI_TX_PHY_ReadRegister(0x06, &u32phyreg);
        COM_INFO("rising time 0x%x\n", u32phyreg);    //0x10171804
    }
    else
    {
        COM_INFO("4KFmt need set DM_TX_CTRL2 bit[6-7] 0b10 \n");

        u32phyreg |= 0x80;
        COM_INFO("writing phy 0x06:0x%x rising time\n",u32phyreg);
        SI_TX_PHY_WriteRegister(0x06, u32phyreg);
        SI_TX_PHY_ReadRegister(0x06,&u32phyreg);
        COM_INFO("rising time 0x%x\n", u32phyreg);
    }

#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    COM_INFO("need not cfg rising time 0x%x\n", u32phyreg);

#endif
    return u32Ret;
}

HI_S32 SI_TX_PHY_DVIEncoder(HI_BOOL bEnable)
{
    return HI_SUCCESS;
}

static HI_S32 SI_TX_PHY_SwingCtrl(HI_U32 u32TMDSClk)
{
#if defined(BOARD_TYPE_S40V2_fpga)

#elif  defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716cv200es) \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3719mv100_a) \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100_a) \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100_a) \
    || defined(CHIP_TYPE_hi3798cv100)

#elif defined(CHIP_TYPE_hi3798mv100_a) \
    || defined(CHIP_TYPE_hi3798mv100)  \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310)  
    
    // taiyan phy, use encoder in ctrl

    HI_U32 u32phyreg = 0;

    SI_TX_PHY_ReadRegister(PHY_PLL1_ADDR,&u32phyreg);
    u32phyreg &= ~0x03;

    if(u32TMDSClk <= 27)        // TMDS_clock <= 27M
    {
        u32phyreg |= 0x0;
    }
    else if(u32TMDSClk <= 165)  //27M < TMDS_clock <= 165M
    {
        u32phyreg |= 0x01;
    }
    else
    {
        u32phyreg |= 0x03;
    }

    COM_INFO("writing phy PHY_PLL1_ADDR(0x%2X:swing):0x%x \n", PHY_PLL1_ADDR, u32phyreg);
    SI_TX_PHY_WriteRegister(PHY_PLL1_ADDR, u32phyreg);
    
#elif  defined(CHIP_TYPE_hi3716mv410)
    // swing 
    HI_U32 u32phyreg = 0;

    if(u32TMDSClk < 165)        // TMDS_clock <165
    {
        u32phyreg = 0x03;
    }
    else                        //165M <= TMDS_clock 
    {
        u32phyreg = 0x42;
    }

    SI_TX_PHY_WriteRegister(PHY_PLL1_ADDR, u32phyreg);
    COM_INFO("writing phy PHY_PLL1_ADDR(0x%2X:swing):0x%x ", PHY_PLL1_ADDR, u32phyreg);
    SI_TX_PHY_ReadRegister(PHY_PLL1_ADDR,&u32phyreg);
    COM_INFO("== 0x%x \n", u32phyreg);
    
#elif   defined(CHIP_TYPE_hi3716mv420)
        // swing 
        HI_U32 u32phyreg = 0;
    
        if(u32TMDSClk < 165)        // TMDS_clock <165
        {
            u32phyreg = 0x40;
        }
        else                        //165M <= TMDS_clock 
        {
            u32phyreg = 0x42;
        }
    
        SI_TX_PHY_WriteRegister(PHY_PLL1_ADDR, u32phyreg);
        COM_INFO("writing phy PHY_PLL1_ADDR(0x%2X:swing):0x%x ", PHY_PLL1_ADDR, u32phyreg);
        SI_TX_PHY_ReadRegister(PHY_PLL1_ADDR,&u32phyreg);
        COM_INFO("== 0x%x \n", u32phyreg);
#endif

    return HI_SUCCESS;
}

HI_S32 SI_Adjust_SwingCtrl(HI_UNF_ENC_FMT_E fmt, HI_UNF_HDMI_DEEP_COLOR_E enDeepColorMode)    
{
    HI_U32 u32PClk = DRV_Calculate_TMDSCLK(fmt, enDeepColorMode);   
    
    u32PClk = u32PClk / 1000; 
    COM_INFO("SI_Adjust_SwingCtrl : TMDS Clock is %ud MHz\n", u32PClk);
    
    SI_TX_PHY_SwingCtrl(u32PClk);

    return HI_SUCCESS;       
}

