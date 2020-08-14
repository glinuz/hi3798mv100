/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.4
 *
******************************************************************************/
#ifndef __DT_BINDINGS_CLOCK_HI3798CV200_H
#define __DT_BINDINGS_CLOCK_HI3798CV200_H

#define PERI_CRG0_APLL_1                        0x0000
#define PERI_CRG1_APLL_2                        0x0004
#define PERI_CRG2_BPLL_1                        0x0008
#define PERI_CRG3_BPLL_2                        0x000C
#define PERI_CRG4_DPLL_1                        0x0010
#define PERI_CRG5_DPLL_2                        0x0014
#define PERI_CRG8_VPLL_1                        0x0020
#define PERI_CRG9_VPLL_2                        0x0024
#define PERI_CRG10_HPLL_1                       0x0028
#define PERI_CRG11_HPLL_2                       0x002C
#define PERI_CRG12_EPLL_1                       0x0030
#define PERI_CRG13_EPLL_2                       0x0034
#define PERI_CRG16_QPLL_1                       0x0040
#define PERI_CRG17_QPLL_2                       0x0044
#define PERI_CRG18_CPU_LP                       0x0048
#define PERI_CRG20_CPU_1                        0x0050
#define PERI_CRG21_CPU_2                        0x0054
#define PERI_CRG22_SYS                          0x0058
#define PERI_CRG23_SFC                          0x005C
#define PERI_CRG24_NANDC                        0x0060
#define PERI_CRG25_DDR                          0x0064
#define PERI_CRG26_UART                         0x0068
#define PERI_CRG27_I2C                          0x006C
#define PERI_CRG28_SSP                          0x0070
#define PERI_CRG29_SCI                          0x0074
#define PERI_CRG30_VDH                          0x0078
#define PERI_CRG31_JPGD                         0x007C
#define PERI_CRG33_PGD                          0x0084
#define PERI_CRG34_BPD                          0x0088
#define PERI_CRG35_VENC                         0x008C
#define PERI_CRG36_JPGE                         0x0090
#define PERI_CRG37_TDE                          0x0094
#define PERI_CRG39_SDIO0                        0x009C
#define PERI_CRG40_SDIO1                        0x00A0
#define PERI_CRG41_DMA                          0x00A4
#define PERI_CRG42_SATA3CTRL                    0x00A8
#define PERI_CRG43_SATA3PHY                     0x00AC
#define PERI_CRG44_USB3CTRL                     0x00b0
#define PERI_CRG46_USB2CTRL                     0x00B8
#define PERI_CRG47_USB2PHY                      0x00BC
#define PERI_CRG48_CA                           0x00C0
#define PERI_CRG49_SHA                          0x00C4
#define PERI_CRG50_PMC                          0x00C8
#define PERI_CRG51_GSF                          0x00CC
#define PERI_CRG52_DSP                          0x00D0
#define PERI_CRG53_GPU                          0x00D4
#define PERI_CRG54_VDP                          0x00D8
#define PERI_CRG55_VICAP                        0x00DC
#define PERI_CRG56_QTC                          0x00E0
#define PERI_CRG57_QAM                          0x00E4
#define PERI_CRG58_QAMADC                       0x00E8
#define PERI_CRG59_SCD                          0x00EC
#define PERI_CRG60_VPSS                         0x00F0
#define PERI_CRG61_HWC                          0x00F4
#define PERI_CRG62_BOOTMEM                      0x00F8
#define PERI_CRG63_PVR_1                        0x00FC
#define PERI_CRG64_PVR_2                        0x0100
#define PERI_CRG67_HDMITX_CTRL                  0x010C
#define PERI_CRG68_HDMITX_PHY                   0x0110
#define PERI_CRG69_ADAC                         0x0114
#define PERI_CRG70_AIAO                         0x0118
#define PERI_CRG71_VDAC                         0x011C
#define PERI_CRG72_FEPHY                        0x0120
#define PERI_CRG73_GPU_LP                       0x0124
#define PERI_CRG74_DDR_LP                       0x0128
#define PERI_CRG75_APLL                         0x012C
#define PERI_CRG76_BPLL                         0x0130
#define PERI_CRG77_DPLL                         0x0134
#define PERI_CRG79_VPLL                         0x013C
#define PERI_CRG80_HPLL                         0x0140
#define PERI_CRG81_EPLL                         0x0144
#define PERI_CRG83_QPLL                         0x014C
#define PERI_CRG84_PLL_LOCK                     0x0150
#define PERI_CRG85_SW_READBACK                  0x0154
#define PERI_CRG86_CPU                          0x0158
#define PERI_CRG87_CPU                          0x015C
#define PERI_CRG88_DDR                          0x0160
#define PERI_CRG89_DDR                          0x0164
#define PERI_CRG90_OUTPUT_RST                   0x0168
#define PERI_CRG91_USB_FREECLK_DEC              0x016C
#define PERI_CRG93_VDH_RST_READBACK             0x0174
#define PERI_CRG94_WDG                          0x0178
#define PERI_CRG95_PLL_TEST                     0x017C
#define PERI_CRG96_A9_RAM                       0x0180
#define PERI_CRG97_GPU_RAM                      0x0184
#define PERI_CRG98_COMBPHY                      0x0188
#define PERI_CRG99_PCIECTRL                     0x018c

#define HIGMAC_MAC0_CLK                         0x019C
#define HIGMAC_MAC1_CLK                         0x01A0
#define HIGMAC_MAC_IF0_CLK                      0x01A4
#define HIGMAC_MAC_IF1_CLK                      0x01A8
#define HII2C_I2C0_CLK                          0x01AC
#define HII2C_I2C1_CLK                          0x01B0
#define HII2C_I2C2_CLK                          0x01B4
#define HII2C_I2C3_CLK                          0x01B8
#define HII2C_I2C4_CLK                          0x01BC
#define PERI_CRG163_SDIO2                       0x028C
#define PERI_CRG224_FMC                         0x0380
#define HIIR_CLK                                0x0384
#define CLK_MAX                                 0x0390

/* higmac reset bit */
#define HIGMAC_PORT0_RST_BIT                    8
#define HIGMAC_PORT1_RST_BIT                    9
#define HIGMAC_MACIF0_RST_BIT                   10
#define HIGMAC_MACIF1_RST_BIT                   11
#define HIGMAC_PHY0_RST_BIT                     12
#define HIGMAC_PHY1_RST_BIT                     13

#endif /* __DT_BINDINGS_CLOCK_HI3798CV200_H */
