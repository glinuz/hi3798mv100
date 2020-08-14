#ifndef __HI3138_H__
#define __HI3138_H__

#include "hi3138_info.h"
#include "hi3138_ts.h"
#include "hi3138_dvbc.h"
#include "hi3138_dvbs2.h"
#include "hi3138_dvbt2.h"
#include "hi3138_isdbt.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */


#define HI3138_I2C_CHN_SEL	0x00	//I2C_BASE--I2C通道选择寄存器
#define HI3138_CHIP_ID0		0x02	//I2C_BASE--芯片ID寄存器
#define HI3138_CHIP_ID1		0x03	//I2C_BASE--芯片ID寄存器
#define HI3138_TEST_SEL		0x05	//I2C_BASE--测试选择寄存器
#define HI3138_COOL_RSTN		0x0F	//I2C_BASE--冷复位控制寄存器

#define HI3138_CHN_ON			0x10 	//IPIO--通道模式控制寄存器
#define HI3138_CRG_CTRL0		0x11	//IPIO--SDRAM时钟相位寄存器
#define HI3138_CRG_CTRL1		0x12	//IPIO--通道0工作时钟配置寄存器
#define HI3138_CRG_CTRL2		0x13	//IPIO--通道1工作时钟配置寄存器
#define HI3138_ADI2C_SEL		0x14	//IPIO--I2C转发通道配置寄存器
#define HI3138_ADC0_CTRL0		0x15	//IPIO--ADC0控制寄存器
#define HI3138_ADC0_CTRL1		0x16	//IPIO--ADC0控制寄存器
#define HI3138_ADC1_CTRL0		0x18	//IPIO--ADC1控制寄存器
#define HI3138_ADC1_CTRL1		0x19	//IPIO--ADC1控制寄存器
#define HI3138_ADC1_CTRL2		0x1B	//IPIO--ADC0/ADC1控制寄存器
#define HI3138_PLL0_CTRL0		0x1C	//IPIO--PLL0小数分频系数低位寄存器?
#define HI3138_PLL0_CTRL1		0x1D	//IPIO--PLL0控制寄存器
#define HI3138_PLL0_CTRL2		0x1E	//IPIO--PLL0输出分频系数寄存器
#define HI3138_PLL0_CTRL3		0x1F	//IPIO--PLL0反馈分频系数寄存器
#define HI3138_PLL0_CTRL4		0x20	//IPIO--PLL0参考分频系数寄存器
#define HI3138_PLL0_CTRL5		0x21	//IPIO--PLL0复位寄存器
#define HI3138_PLL1_CTRL0		0x23	//IPIO--PLL1小数分频系数低位寄存器
#define HI3138_PLL1_CTRL1		0x24	//IPIO--PLL1控制寄存器
#define HI3138_PLL1_CTRL2		0x25	//IPIO--PLL1输出分频系数寄存器
#define HI3138_PLL1_CTRL3		0x26	//IPIO--PLL1反馈分频系数寄存器
#define HI3138_PLL1_CTRL4		0x27	//IPIO--PLL1参考分频系数寄存器
#define HI3138_PLL1_CTRL5		0x28	//IPIO--PLL1复位寄存器
#define HI3138_IO_CTRL0		0x29	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL1		0x2A	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL2		0x2B	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL3		0x2C	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL4		0x2D	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL5		0x2E	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL6		0x2F	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL7		0x30	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL8		0x31	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL9		0x32	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL10		0x33	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL11		0x34	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL12		0x35	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL13		0x36	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL14		0x37	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL15		0x38	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL16		0x39	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL17		0x3A	//IPIO--IO 配置寄存器
#define HI3138_IO_CTRL18		0x3B	//IPIO--IO 配置寄存器
#define HI3138_SDR_BIST_CTRL0		0x3C	//IPIO--SDRAM BIST配置寄存器
#define HI3138_SDR_BIST_CTRL1		0x3D	//IPIO--SDRAM BIST起始地址低位寄存器
#define HI3138_SDR_BIST_CTRL2		0x3E	//IPIO--SDRAM BIST起始地址中位寄存器
#define HI3138_SDR_BIST_CTRL3		0x3F	//IPIO--SDRAM BIST起始地址高位寄存器
#define HI3138_SDR_BIST_CTRL4		0x40	//IPIO--SDRAM BIST长度低位寄存器
#define HI3138_SDR_BIST_CTRL5		0x41	//IPIO--SDRAM BIST长度中位寄存器
#define HI3138_SDR_BIST_CTRL6		0x42	//IPIO--SDRAM BIST长度高位寄存器
#define HI3138_MTS_CTRL0		0x43	//IPIO--MTS配置寄存器
#define HI3138_MTS_OUT_SEL10		0x44	//IPIO--MTS管脚映射寄存器
#define HI3138_MTS_OUT_SEL32		0x45	//IPIO--MTS管脚映射寄存器
#define HI3138_MTS_OUT_SEL54		0x46	//IPIO--MTS管脚映射寄存器
#define HI3138_MTS_OUT_SEL76		0x47	//IPIO--MTS管脚映射寄存器
#define HI3138_MTS_OUT_SEL98		0x48	//IPIO--MTS管脚映射寄存器
#define HI3138_MTS_OUT_SELA		0x49	//IPIO--MTS管脚映射寄存器
#define HI3138_MTS_CTRL1		0x4A	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL2		0x4B	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL3		0x4C	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL4		0x4D	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL5		0x4E	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL6		0x4F	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL7		0x50	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL8		0x51	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL9		0x52	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL10		0x53	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL11		0x54	//IPIO--MTS配置寄存器
#define HI3138_MTS_CTRL12		0x55	//IPIO--MTS配置寄存器
#define HI3138_TS_CLK_CTRL		0x56	//IPIO--TS_CLK配置寄存器
#define HI3138_ADC0_STATUS		0x57	//IPIO--ADC0状态寄存器
#define HI3138_ADC1_STATUS		0x58	//IPIO--ADC1状态寄存器
#define HI3138_PLL_STATUS		0x5C	//IPIO--PLL状态寄存器
#define HI3138_IO_CTRL19		0x5D	//IPIO--IO 配置寄存器

#define HI3138_RSTN_CTRL		0x2E

#define HI3138_T_MAN_RST_CTRL0		0x20	//T--复位控制寄存器
#define HI3138_T_MAN_RST_CTRL1		0x21	//T--复位控制使能寄存器
#define HI3138_T_STATE_WAITS		0x22	//T--超时复位寄存器
#define HI3138_T_CLK_DEMO_L		0x23	//T--解调时钟低位寄存器
#define HI3138_T_CLK_DEMO_M		0x24	//T--解调时钟中位寄存器
#define HI3138_T_CLK_FEC_L		0x26	//T--FEC时钟低位寄存器
#define HI3138_T_CLK_FEC_M		0x27	//T--FEC时钟中位寄存器
#define HI3138_T_CLK_SDC_L		0x29	//T--SDC时钟低位寄存器
#define HI3138_T_CLK_SDC_M		0x2A	//T--SDC时钟中位寄存器
#define HI3138_T_SDC_CTRL		0x2B	//T--SDC控制寄存器
#define HI3138_T_LOCK_FLAG		0x2C	//T--锁定指示寄存器
#define HI3138_T_TUNER_SEL		0x2D	//T--tuner控制寄存器
#define HI3138_T_RSTN_CTRL		0x2E	//T--逻辑复位寄存器
#define HI3138_T_ILA_SEL			0x2F	//T--测试向量选择寄存器
#define HI3138_T_AGC_SPEED_BOUND		0x30	//T--agc步长寄存器
#define HI3138_T_AGC_GOAL		0x31	//T--agc功率寄存器
#define HI3138_T_AGCOK_WAIT		0x32	//T--agc等待寄存器
#define HI3138_T_AGC_CTRL		0x33	//T--agc控制寄存器
#define HI3138_T_AGC_DC_I		0x34	//T--I路直流寄存器
#define HI3138_T_AGC_DC_Q		0x35	//T--Q路直流寄存器
#define HI3138_T_DAGC_CTRL		0x36	//T--数字agc控制寄存器
#define HI3138_T_AGC_CTRL_L		0x37	//T--agc功率低位寄存器
#define HI3138_T_AGC_CTRL_H		0x38	//T--agc功率高位寄存器
#define HI3138_T_AMP_ERR_IIR		0x39	//T--幅度误差寄存器
#define HI3138_T_PDM_CTRL_L		0x3A	//T--手动agc控制字低位寄存器
#define HI3138_T_PDM_CTRL_H		0x3B	//T--手动agc控制字高位寄存器
#define HI3138_T_USE_PWM			0x3C	//T--AGC输出波形选择寄存器
#define HI3138_T_DAGC_INNER_CTRL		0x40	//T--数字AGC控制寄存器
#define HI3138_T_SF_RMV			0x41	//T--窄带干扰抑制控制寄存器
#define HI3138_T_DAGC_REF		0x42	//T--dagc幅度参考值寄存器
#define HI3138_T_DAGC_SPEED		0x43	//T--dagc步长选择寄存器
#define HI3138_T_IF_FREQ_L		0x4A	//T--输入信号中频频率低位寄存器
#define HI3138_T_IF_FREQ_H		0x4B	//T--输入信号中频频率高位寄存器
#define HI3138_T_BAND_WIDTH		0x4E	//T--输入信号带宽寄存器
#define HI3138_T_SYN_CTRL0		0x50	//T--同步控制寄存器
#define HI3138_T_CORR_HIGH_TH		0x51	//T--P2相关检测高门限寄存器
#define HI3138_T_CORR_LOW_TH		0x52	//T--P2相关检测低门限寄存器
#define HI3138_T_P2_POS_MOD		0x53	//T--P2同步位置检测调整寄存器
#define HI3138_T_P1_THRES		0x54	//T--P1同步控制寄存器
#define HI3138_T_CHN_FFT_GI		0x55	//T--同步检测参数寄存器
#define HI3138_T_P1_SIGNAL		0x56	//T--P1检测信令寄存器
#define HI3138_T_TIM_OFFSET		0x57	//T--定时误差寄存器
#define HI3138_T_CAR_OFFSET_L		0x58	//T--载波误差低位寄存器
#define HI3138_T_CAR_OFFSET_H		0x59	//T--载波误差高位寄存器
#define HI3138_T_T2_CHK_CTRL		0x5D	//T--DVB-T2检测配置寄存器
#define HI3138_T_SOAC_TH			0x5E	//T--P1信令检测门限寄存器
#define HI3138_T_OUTP_RAND		0x5F	//T--加扰设置寄存器
#define HI3138_T_LOOP_BW			0x60	//T--环路带宽选择寄存器
#define HI3138_T_FD_GRP			0x61	//T--时域插值控制寄存器
#define HI3138_T_CPE			0x62
#define HI3138_T_NP_IIR_SFT		0x64	//T--CSI计算控制寄存器
#define HI3138_T_ECHO_ISI			0x65
#define HI3138_T_ECHO_THRES		0x67	//T--多径检测门限寄存器
#define HI3138_T_ECHO_THL         0x68
#define HI3138_T_MIN_THRES		0x69	//T--多径检测门限最小值寄存器
#define HI3138_T_NP_GRP			0x6A	//T--时域插值控制寄存器
#define HI3138_T_TS_A9_SEL		0x6B	//T--ts输出控制寄存器
#define HI3138_T_TS_87_SEL		0x6C	//T--ts输出控制寄存器
#define HI3138_T_TS_65_SEL		0x6D	//T--ts输出控制寄存器
#define HI3138_T_TS_43_SEL		0x6E	//T--ts输出控制寄存器
#define HI3138_T_TS_21_SEL		0x6F	//T--ts输出控制寄存器
#define HI3138_T_TIM_LOOP_L		0x70	//T--定时误差低位寄存器
#define HI3138_T_TIM_LOOP_H		0x71	//T--定时误差高位寄存器
#define HI3138_T_MAX_TH			0x74
#define HI3138_T_TS_0_SEL		0x75	//T--ts输出控制寄存器
#define HI3138_T_CIR_DIST_0		0x76	//T--多径分布寄存器
#define HI3138_T_CIR_DIST_1		0x77	//T--多径分布寄存器
#define HI3138_T_CIR_DIST_2		0x78	//T--多径分布寄存器
#define HI3138_T_CIR_DIST_3		0x79	//T--多径分布寄存器
#define HI3138_T_SNR_L			0x7A	//T--信噪比指示低位寄存器
#define HI3138_T_SNR_H			0x7B	//T--信噪比指示高位寄存器
#define HI3138_T_DOPPLER		0x7C	//T--多普勒指示寄存器
#define HI3138_T_CEQ_ADDR_1D    0x7D	//T--TS输出高阻选择寄存器(bit6)
#define HI3138_T_CW_FREQ_L		0x80	//T--单频干扰频点低位寄存器
#define HI3138_T_CW_FREQ_H		0x81	//T--单频干扰频点高位寄存器
#define HI3138_T_CLK_ADC_L		0x85	//T--ADC时钟低位寄存器
#define HI3138_T_CLK_ADC_M		0x86	//T--ADC时钟中位寄存器
#define HI3138_T_CLK_ADC_H		0x87	//T--ADC时钟高位寄存器
#define HI3138_T_ATV_STATE		0x88	//T--同频干扰标志寄存器
#define HI3138_T_SMP_CTRL		0x89	//T--射频码流采集寄存器
#define HI3138_T_ITER_CTRL		0x91	//T--迭代控制寄存器
#define HI3138_T_BER_CTRL		0x92	//T--ber控制寄存器
#define HI3138_T_AUTO_DLY		0x93	//T--迭代切换寄存器
#define HI3138_T_ITER_NUM		0x94	//T--PRE信令迭代次数寄存器
#define HI3138_T_ITER_NUM_POST		0x95	//T--POST信令迭代次数寄存器
#define HI3138_T_FEC_BER_L		0x96	//T--ber低位寄存器
#define HI3138_T_FEC_BER_H		0x97	//T--ber高位寄存器
#define HI3138_T_FEC_FER_L		0x98	//T--fer低位寄存器
#define HI3138_T_FEC_FER_H		0x99	//T--fer高位寄存器
#define HI3138_T_SWITCH_DLY		0x9C	//T--信令码字切换时延寄存器
#define HI3138_T_T2_SUCCESS		0x9E	//T--T2译码成功寄存器
#define HI3138_T_OUTP_ISSY		0xA0	//T--issy控制寄存器
#define HI3138_T_OUTP_DCAP_SET		0xA1	//T--DATA PLP容量设置寄存器
#define HI3138_T_OUTP_CCAP_SET		0xA2	//T--COMMON PLP容量设置寄存器
#define HI3138_T_OUTP_PLL0		0xA3	//T--PLL控制寄存器
#define HI3138_T_OUTP_PLL1		0xA4	//T--PLL控制寄存器
#define HI3138_T_OUTP_PLL2		0xA5	//T--PLL控制寄存器
#define HI3138_T_OUTP_PLL3		0xA6	//T--PLL控制寄存器
#define HI3138_T_OUTP_PLL4		0xA7	//T--PLL控制寄存器
#define HI3138_T_OUTP_CLK_SET		0xA8	//T--输出时钟设置寄存器
#define HI3138_T_OUTP_CLK_SETH		0xA9	//T--I2C时钟设置寄存器
#define HI3138_T_OUTP_CLK_SETL		0xAA	//T--PLL初始值设置寄存器
#define HI3138_T_OUTP_MODE_SET		0xAB	//T--输出模式设置寄存器
#define HI3138_T_OUTP_TS_MODE		0xAC	//T--TS输出模式设置寄存器
#define HI3138_T_OUTP_PKT_STA		0xAE	//T--TS统计包数设置寄存器
#define HI3138_T_OUTP_LIMIT_EN		0xAF	//T--限幅和使能寄存器
#define HI3138_T_PLP_CTRL		0xB0	//T--PLP控制寄存器
#define HI3138_T_PLP_ID0			0xB1	//T--Dataplp序号寄存器
#define HI3138_T_PLP_ID1			0xB2	//T--Commonplp序号寄存器
#define HI3138_T_TPS			0xB3	//T--信号参数寄存器
#define HI3138_T_STREAM_TYPE		0xB4	//T--传输数据流类型指示寄存器
#define HI3138_T_CODE_RATE_DVBT		0xB4	//T--DVB-T内码码率寄存器
#define HI3138_T_TPS_DVBT		0xB5	//T--DVB-T信号参数寄存器
#define HI3138_T_PAPR_L1MOD		0xB5	//T--DVB-T2信号参数寄存器
#define HI3138_T_PP_VERSION		0xB8	//T--DVB-T2 PRE信令寄存器
#define HI3138_T_NUM_T2_FRM		0xB9	//T--DVB-T2 PRE信令寄存器
#define HI3138_T_LDATA_L			0xBA	//T--DVB-T2 PRE信令寄存器
#define HI3138_T_LDATA_H			0xBB	//T--DVB-T2 PRE信令寄存器
#define HI3138_T_NUM_PLP			0xBF	//T--DVB-T2 PRE信令寄存器
#define HI3138_T_TMCC1			0xC0	//T--ISDB-T TMCC寄存器
#define HI3138_T_TMCC2			0xC1	//T--ISDB-T TMCC寄存器
#define HI3138_T_TMCC3			0xC2	//T--ISDB-T TMCC寄存器
#define HI3138_T_TMCC4			0xC3	//T--ISDB-T TMCC寄存器
#define HI3138_T_TMCC5			0xC4	//T--ISDB-T TMCC寄存器
#define HI3138_T_TMCC6			0xC5	//T--ISDB-T TMCC寄存器
#define HI3138_T_TMCC7			0xC6	//T--ISDB-T TMCC寄存器
#define HI3138_T_PLP_ID			0xC6	//T--DVB-T2 POST信令寄存器
#define HI3138_T_PLP_TYPE		0xC7	//T--DVB-T2 POST信令寄存器
#define HI3138_T_PLP_GRP_ID		0xC8	//T--DVB-T2 POST信令寄存器
#define HI3138_T_PLP_PARAM		0xC9	//T--DVB-T2 POST信令寄存器

#define HI3138_S_MAN_RST_CTRL0		0x20	//S--复位控制寄存器
#define HI3138_S_MAN_RST_CTRL1		0x21	//S--复位控制使能寄存器
#define HI3138_S_STATE_WAITS		0x22	//S--超时复位寄存器
#define HI3138_S_CLK_DEMO_L		0x23	//S--解调时钟低位寄存器
#define HI3138_S_CLK_DEMO_M		0x24	//S--解调时钟中位寄存器
#define HI3138_S_CLK_DEMO_H		0x25	//S--解调时钟高位寄存器
#define HI3138_S_CLK_FEC_L		0x26	//S--解码时钟低位寄存器
#define HI3138_S_CLK_FEC_M		0x27	//S--解码时钟中位寄存器
#define HI3138_S_CLK_FEC_H		0x28	//S--解码时钟高位寄存器
#define HI3138_S_LOCK_TIME_L		0x29	//S--锁定时间低位寄存器
#define HI3138_S_LOCK_TIME_M		0x2A	//S--锁定时间中位寄存器
#define HI3138_S_LOCK_TIME_H		0x2B	//S--锁定时间高位寄存器
//#define HI3138_S_LOCK_FLAG		0x2C	//S--锁定指示寄存器
//#define HI3138_S_TUNER_SEL		0x2D	//S--tuner控制寄存器
//#define HI3138_S_RSTN_CTRL		0x2E	//S--逻辑复位寄存器
#define HI3138_S_ILA_SEL			0x2F	//S--测试向量选择寄存器
#define HI3138_S_AGC_SPEED_BOUND		0x30	//S--agc步长寄存器
#define HI3138_S_AGC_GOAL		0x31	//S--agc功率寄存器
#define HI3138_S_AGCOK_WAIT		0x32	//S--agc等待寄存器
//#define HI3138_S_AGC_CTRL		0x33	//S--agc控制寄存器
#define HI3138_S_AGC_DC_I		0x34	//S--I路直流寄存器
#define HI3138_S_AGC_DC_Q		0x35	//S--Q路直流寄存器
#define HI3138_S_DAGC_CTRL		0x36	//S--数字agc控制寄存器
//#define HI3138_S_AGC_CTRL_L		0x37	//S--agc功率低位寄存器
//#define HI3138_S_AGC_CTRL_H		0x38	//S--agc功率高位寄存器
#define HI3138_S_AMP_ERR_IIR		0x39	//S--功率误差寄存器
#define HI3138_S_PDM_CTRL_L		0x3A	//S--手动agc控制字低位寄存器
#define HI3138_S_PDM_CTRL_H		0x3B	//S--手动agc控制字高位寄存器
#define HI3138_S_TR_CTRL1		0x40	//S--tr控制寄存器
#define HI3138_S_DAGC_STD		0x41	//S--dagc控制寄存器
#define HI3138_S_TR_MONITOR		0x43	//S--tr监控寄存器
#define HI3138_S_CNT_THRESH		0x44	//S--tr等待时间寄存器
//#define HI3138_S_FS_L			0x46	//S--符号率低位寄存器
//#define HI3138_S_FS_H			0x47	//S--符号率高位寄存器
//#define HI3138_S_CENT_FREQ_L		0x48	//S--载波频率低位寄存器
//#define HI3138_S_CENT_FREQ_H		0x49	//S--载波频率高位寄存器
//#define HI3138_S_FS_OFFSET_FC_L		0x4C	//S--符号率偏差低位寄存器
//#define HI3138_S_FS_OFFSET_FC_H		0x4D	//S--符号率偏差高位寄存器
//#define HI3138_S_FREQ_OFFSET_FC_L	0x4E	//S--频率偏差低位寄存器
//#define HI3138_S_FREQ_OFFSET_FC_H	0x4F	//S--频率偏差高位寄存器
#define HI3138_S_PLH_SYNC_1		0x50	//S--帧同步控制寄存器
#define HI3138_S_PLH_SYNC_2		0x51	//S--帧同步控制寄存器
#define HI3138_S_CR_CTRL_SW		0x52	//S--cr控制寄存器
#define HI3138_S_SCAN_STEP_L		0x53	//S--扫频速度寄存器
#define HI3138_S_SCAN_STEP_FB		0x54	//S--扫频控制寄存器
#define HI3138_S_SCAN_ADJUST		0x55	//S--自动扫频速度寄存器
#define HI3138_S_CR_ZUNI_WAIT		0x56	//S--载波控制寄存器
#define HI3138_S_CR_BW_ADJUST		0x57	//S--载波带宽控制寄存器
#define HI3138_S_CR_BW_MAX		0x58	//S--载波最大带宽控制寄存器
#define HI3138_S_CR_BW_SET		0x59	//S--载波带宽控制寄存器
#define HI3138_S_CR_CN			0x5A	//S--载波CN值寄存器
#define HI3138_S_CR_STATE		0x5B	//S--载波状态寄存器
#define HI3138_S_PLS_CODE		0x5C	//S--pls_code寄存器
#define HI3138_S_FREQ_INV		0x5D	//S--频谱反转寄存器
#define HI3138_S_CR_ZUNI_BW_L		0x5E	//S--载波带宽寄存器
#define HI3138_S_CR_ZUNI_BW_H		0x5F	//S--载波带宽寄存器
#define HI3138_S_SYNC_FREQ_L		0x60	//S--帧同步频率寄存器
#define HI3138_S_SYNC_FREQ_H		0x61	//S--帧同步频率寄存器
#define HI3138_S_SCAN_FREQ_L		0x62	//S--扫频频率寄存器
#define HI3138_S_SCAN_FREQ_H		0x63	//S--扫频频率寄存器
#define HI3138_S_FREQ_ACC_L		0x64	//S--载波频差慢反馈寄存器
#define HI3138_S_FREQ_ACC_H		0x65	//S--载波频差慢反馈寄存器
#define HI3138_S_TR_FREQ_FB_L		0x66	//S--载波频差tr反馈寄存器
#define HI3138_S_TR_FREQ_FB_H		0x67	//S--载波频差tr反馈寄存器
#define HI3138_S_CR_LOOP_DC_L		0x68	//S--载波PLL控制字寄存器
#define HI3138_S_CR_LOOP_DC_H		0x69	//S--载波PLL控制字寄存器
#define HI3138_S_EQU_CTRL		0x70	//S--均衡控制寄存器
#define HI3138_S_LMS_STEP		0x71	//S--均衡步长控制寄存器
#define HI3138_S_CN_CTRL			0x75	//S--噪声功率统计控制寄存器
#define HI3138_S_EQU_TAP_REAL		0x76	//S--均衡抽头实部寄存器
#define HI3138_S_EQU_TAP_IMAG		0x77	//S--均衡抽头虚部寄存器
#define HI3138_S_EQU_TAP_SEL		0x78	//S--抽头选择寄存器
#define HI3138_S_XREG_INIT_LOW		0x7A	//S--均衡控制寄存器
#define HI3138_S_XREG_INIT_MID		0x7B	//S--均衡控制寄存器
#define HI3138_S_XREG_INIT_HI		0x7C	//S--均衡控制寄存器
#define HI3138_S_RD_WR_TAP		0x7D	//S--抽头控制寄存器
//#define HI3138_S_NOISE_POW_L		0x7E	//S--噪声功率低位寄存器
//#define HI3138_S_NOISE_POW_H		0x7F	//S--噪声功率高位寄存器
#define HI3138_S_TS_PARALL_CTRL		0x80	//S--ts输出选择寄存器
#define HI3138_S_TS_10_SEL		0x81	//S--ts输出控制寄存器
#define HI3138_S_TS_32_SEL		0x82	//S--ts输出控制寄存器
#define HI3138_S_TS_54_SEL		0x83	//S--ts输出控制寄存器
#define HI3138_S_TS_76_SEL		0x84	//S--ts输出控制寄存器
#define HI3138_S_TS_98_SEL		0x85	//S--ts输出控制寄存器
#define HI3138_S_TS_CTRL0		0x86	//S--ts控制寄存器
#define HI3138_S_TS_CTRL3		0x87	//S--ts控制寄存器
#define HI3138_S_TS_CTRL4		0x88	//S--ts控制寄存器
#define HI3138_S_TS_CLK_DIV_F_L		0x89	//S--ts时钟低位寄存器
#define HI3138_S_TS_CLK_DIV_F_H		0x8A	//S--ts时钟低位寄存器
#define HI3138_S_ISI_SEL			0x8B	//S--isi选择寄存器
#define HI3138_S_MATTYPE			0x8C	//S--流id寄存器
#define HI3138_S_ISI_NUM		0x8D	//S--滚降寄存器,原名ROLL_OFF
#define HI3138_S_CRC_ERR			0x8E	//S--crc校验寄存器
#define HI3138_S_RST_WAIT		0x8F	//S--复位等待寄存器
#define HI3138_S_BER_CTRL		0x92	//S--ber控制寄存器
#define HI3138_S_ITER_CLK_CTRL		0x94	//S--LDPC迭代时钟门控寄存器
#define HI3138_S_FEC_BER_L		0x96	//S--ber低位寄存器
#define HI3138_S_FEC_BER_H		0x97	//S--ber高位寄存器
#define HI3138_S_FEC_FER_L		0x98	//S--fer低位寄存器
#define HI3138_S_FEC_FER_H		0x99	//S--fer高位寄存器
#define HI3138_S_FEC_STATUS		0x9E	//S--FEC状态寄存器
#define HI3138_S_FS_SPAN			0xA1	//S--盲扫符号率范围寄存器
#define HI3138_S_AMP_MIN_FS		0xA7	//S--盲扫最低符号率寄存器
#define HI3138_S_CBS_CTRL_RDADDR		0xA8	//S--盲扫控制寄存器
//#define HI3138_S_CBS_FS_L		0xA9	//S--盲扫符号率低位寄存器
//#define HI3138_S_CBS_FS_H		0xAA	//S--盲扫符号率高位寄存器
//#define HI3138_S_CBS_FC_L		0xAB	//S--盲扫频偏低位寄存器
//#define HI3138_S_CBS_FC_H		0xAC	//S--盲扫频偏高位寄存器
//#define HI3138_S_CBS_FINISH		0xAD	//S--盲扫结束寄存器
//#define HI3138_S_CBS_RELIABILITY1	0xAE	//S--盲扫可靠度寄存器
//#define HI3138_S_CBS_R2_NUM		0xAF	//S--盲扫信号个数寄存器
//#define HI3138_S_DSEC_ADDR		0xB0	//S--dsec地址寄存器
//#define HI3138_S_DSEC_DATA		0xB1	//S--dsec数据寄存器
//#define HI3138_S_DSEC_RATIO_L		0xB2	//S--dsec低位频率寄存器
//#define HI3138_S_DSEC_RATIO_H		0xB3	//S--dsec高位频率寄存器
//#define HI3138_S_TX_CTRL1		0xB4	//S--dsec发送控制寄存器
#define HI3138_S_RX_CTRL1		0xB5	//S--dsec接收控制寄存器
//#define HI3138_S_DSEC_EN			0xB7	//S--dsec使能寄存器
//#define HI3138_S_RX_STATE		0xB8	//S--dsec接收状态寄存器
//#define HI3138_S_INT_STATE		0xB9	//S--dsec状态寄存器
#define HI3138_S_DPUNC_CTRL0		0xE0	//S--解删余控制寄存器
#define HI3138_S_DPUNC_CTRL1		0xE1	//S--解删余控制寄存器
#define HI3138_S_PKT_SYNC_CTRL		0xE2	//S--包同步控制寄存器
#define HI3138_S_SEAR_RESULT		0xE3	//S--码率搜索结果寄存器
#define HI3138_S_ITER_CTRL		0xE6	//S--迭代控制寄存器

#define HI3138_C_MAN_RST_CTRL0		0x20	//C--复位控制寄存器
#define HI3138_C_MAN_RST_CTRL1		0x21	//C--复位控制寄存器
#define HI3138_C_DEMO_FREQ_L		0x23	//C--demo时钟低位寄存器
#define HI3138_C_DEMO_FREQ_M		0x24	//C--demo时钟中位寄存器
#define HI3138_C_LOCK_TIME		0x25	//C--系统锁定时间寄存器
#define HI3138_C_FEC_FREQ_L		0x26	//C--fec时钟低位寄存器
#define HI3138_C_FEC_FREQ_M		0x27	//C--fec时钟中位寄存器
#define HI3138_C_AUTO_RST_CNT		0x28	//C--自复位统计寄存器
#define HI3138_C_CATCH_CTRL		0x2A	//C--cacth控制寄存器
#define HI3138_C_TOUT_CTRL		0x2B	//C--超时控制寄存器
#define HI3138_C_LOCK_FLAG		0x2C	//C--指示寄存器
#define HI3138_C_TUNER_SEL		0x2D	//C--TUNER_I2C转发配置寄存器
#define HI3138_C_RSTN_CTRL		0x2E	//C--复位控制寄存器
#define HI3138_C_ILA_SEL			0x2F	//C--测试向量选择寄存器
#define HI3138_C_AGC_SPEED_BOUND		0x30	//C--测试向量选择寄存器
#define HI3138_C_AGC_GOAL		0x31	//C--agc功率寄存器
#define HI3138_C_AGCOK_WAIT		0x32	//C--agc等待寄存器
#define HI3138_C_AGC_CTRL		0x33	//C--agc控制寄存器
#define HI3138_C_AGC_DC_I		0x34	//C--I路直流寄存器
#define HI3138_C_AGC_DC_Q		0x35	//C--Q路直流寄存器
#define HI3138_C_DAGC_CTRL		0x36	//C--数字agc控制寄存器
#define HI3138_C_AGC_CTRL_H		0x37	//C--agc功率高位寄存器
#define HI3138_C_AGC_CTRL_L		0x38	//C--agc功率低位寄存器
#define HI3138_C_AMP_ERR_IIR		0x39	//C--功率误差寄存器
#define HI3138_C_PDM_CTRL_H		0x3A	//C--手动agc控制字高位寄存器
#define HI3138_C_PDM_CTRL_L		0x3B	//C--手动agc控制字低位寄存器
#define HI3138_C_USE_PWM			0x3C	//C--pwm控制寄存器
#define HI3138_C_DAGC_JIT_THRESH		0x40	//C--幅度抖动检测门限寄存器
#define HI3138_C_SF_RMV_CTRL		0x41	//C--单频消除控制寄存器
#define HI3138_C_DAGC_REF		0x42	//C--数字AGC参考幅度寄存器
#define HI3138_C_SCAN_WAIT		0x43	//C--符号率盲扫控制寄存器
#define HI3138_C_FREQ_DEV_L		0x44	//C--载波中心频率_载波偏差低位寄存器
#define HI3138_C_FREQ_DEV_H		0x45	//C--载波中心频率_载波偏差高位寄存器
#define HI3138_C_FS_L			0x46	//C--符号率低位寄存器
#define HI3138_C_FS_H			0x47	//C--符号率高位寄存器
#define HI3138_C_TR_CR_CTRL		0x48	//C--定时载波控制寄存器
#define HI3138_C_TR_SCAN_CTRL		0x4A	//C--符号率盲扫控制寄存器
#define HI3138_C_FREQ_ADC_L		0x4B	//C--ADC时钟低位寄存器
#define HI3138_C_FREQ_ADC_M		0x4C	//C--ADC时钟中位寄存器
#define HI3138_C_TR_LOOP_CTRL		0x4D	//C--定时环路控制寄存器
#define HI3138_C_SF_SCAN_CTRL		0x4E	//C--单频扫描控制寄存器
#define HI3138_C_EQU_CTRL		0x63	//C--均衡器控制寄存器
#define HI3138_C_EQU_STEP_WAIT		0x64	//C--均衡器步长与等待时间寄存器
#define HI3138_C_EQU_STATUS		0x66	//C--均衡器状态寄存器
#define HI3138_C_NOISE_POW_L		0x6C	//C--噪声功率低位寄存器
#define HI3138_C_NOISE_POW_H		0x6D	//C--噪声功率高位寄存器
#define HI3138_C_PT_CTRL			0x6E	//C--相位跟踪控制寄存器
#define HI3138_C_PT_CTRL1 			0x6F	//C--相位跟踪控制寄存器
#define HI3138_C_PT2_CR_CTRL		0x70	//C--载波环路控制寄存器
#define HI3138_C_PLS_CTRL1  		0x77	//C--脉冲噪声控制寄存器
#define HI3138_C_PLS_CTRL2  		0x78	//C--脉冲噪声控制寄存器
#define HI3138_C_PLS_CTRL3  		0x79	//C--脉冲噪声控制寄存器
#define HI3138_C_QLLR_CTRL		0x90	//C--象限LLR控制寄存器
#define HI3138_C_DEMAP_LOW_CTRL		0x91	//C--边界星座LLR控制寄存器
#define HI3138_C_SYNC_CTRL		0x92	//C--包同步控制寄存器
#define HI3138_C_ITLV_CW			0x94	//C--J.83B交织模式寄存器
#define HI3138_C_ITLV_AUTO		0x9C	//C--J.83B交织模式配置寄存器
#define HI3138_C_OUTP_PLL_INI		0xB0	//C--TS流初始频率设置寄存器
#define HI3138_C_OUTP_CAP_SET		0xB1	//C--OUTP容量设置寄存器
#define HI3138_C_OUTP_PLLUP_FRE		0xB2	//C--TS流环路更新频率设置寄存器
#define HI3138_C_OUTP_PLL0		0xB3	//C--PLL控制寄存器
#define HI3138_C_OUTP_PLL1		0xB4	//C--PLL控制寄存器
#define HI3138_C_OUTP_PLL2		0xB5	//C--PLL控制寄存器
#define HI3138_C_OUTP_PLL3		0xB6	//C--PLL控制寄存器
#define HI3138_C_OUTP_PLL4		0xB7	//C--PLL控制寄存器
#define HI3138_C_OUTP_I2C_CLK0		0xB8	//C--I2C时钟设置寄存器
#define HI3138_C_OUTP_I2C_CLK1		0xB9	//C--I2C时钟设置寄存器
#define HI3138_C_OUTP_I2C_CLK2		0xBA	//C--I2C时钟设置寄存器
#define HI3138_C_OUTP_RAM_THRE		0xBB	//C--容量阈值寄存器
#define HI3138_C_OUTP_TS_MODE		0xBC	//C--输出模式控制寄存器
#define HI3138_C_OUTP_SYNC_BYTE		0xBD	//C--同步字设置寄存器
#define HI3138_C_OUTP_PKT_SET		0xBE	//C--统计包数设置寄存器
#define HI3138_C_OUTP_RAND		0xBF	//C--TS输出加扰寄存器
#define HI3138_C_OUTP_TS_10_SEL		0xC0	//C--TS端口选择寄存器
#define HI3138_C_OUTP_TS_32_SEL		0xC1	//C--TS端口选择寄存器
#define HI3138_C_OUTP_TS_54_SEL		0xC2	//C--TS端口选择寄存器
#define HI3138_C_OUTP_TS_76_SEL		0xC3	//C--TS端口选择寄存器
#define HI3138_C_OUTP_TS_98_SEL		0xC4	//C--TS端口选择寄存器
#define HI3138_C_OUTP_BER		0xC5	//C--误比特率统计寄存器
#define HI3138_C_OUTP_PKT_MODE		0xC6	//C--TS输出包模式寄存器
#define HI3138_C_OUTP_NEAR_FUL		0xC7	//C--OUTP容量接近满配置寄存器
#define HI3138_C_OUTP_SYS_CLK0		0xC8	//C--系统时钟配置寄存器
#define HI3138_C_OUTP_SYS_CLK1		0xC9	//C--系统时钟配置寄存器
#define HI3138_C_OUTP_PLL_CTRL		0xCA	//C--PLL控制字设置寄存器
#define HI3138_C_OUTP_BER_RES0		0xCB	//C--误比特率统计结果寄存器
#define HI3138_C_OUTP_BER_RES1		0xCC	//C--误比特率统计结果寄存器
#define HI3138_C_ERR_LMT_SET		0xCD	//C--误包统计控制寄存器
#define HI3138_C_ERR_PKT_L		0xCE	//C--误包数低位寄存器
#define HI3138_C_ERR_PKT_H		0xCF	//C--误包数高位寄存器

//#define TIME_CONSUMING  /*耗时打印控制*/

#define HI3138_IPIO_CINFIG(u32TunerPort) do{  \
    HI_U8 u8Temp = 0;\
    qam_read_byte(u32TunerPort, HI3138_I2C_CHN_SEL, &u8Temp);   \
    u8Temp &= ~(0x0F << 0); \
    u8Temp |= 3;    \
    qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, u8Temp);   \
    }while(0)

#define HI3138_ARRARY_SIZE(arraryName) (sizeof(arraryName)/sizeof(arraryName[0]))

#define HI3138_SELECT_ALLOCATION_PORT(u32TunerPort, arraryName, allocationIndex) \
    do { \
        for(allocationIndex = 0; allocationIndex < (HI3138_ARRARY_SIZE(arraryName)); allocationIndex++) \
        { \
        	if((u32TunerPort == arraryName[allocationIndex].u32TunerPort) \
                && (HI_UNF_TUNER_SIG_TYPE_BUTT != arraryName[i].enHi3138SigType)) \
        	{ \
        		break; \
        	} \
        } \
        if(allocationIndex >= (HI3138_ARRARY_SIZE(arraryName))) \
        { \
            HI_ERR_TUNER("allocation port is not match.\n"); \
            return HI_FAILURE; \
        } \
    } while(0)


typedef enum hi3138_INNER_DEMOD_NUMBER
{
	HI3138_INNER_DEMOD0 = 0,
	HI3138_INNER_DEMOD1,
	HI3138_INNER_DEMOD_BUTT
}Hi3138_INNER_DEMOD_NUMBER;

typedef enum hi3138_LNB_POWER_SOURCE
{
    HI3138_LNB_POWER_BY_ADDR0 = 0,
    HI3138_LNB_POWER_BY_ADDR1,
    HI3138_LNB_POWER_BY_ADDR_BUTT
}Hi3138_LNB_POWER_SOURCE;

typedef struct hi3138_SIGNAL_INFO_ALLOCATION
{
    HI_U32 u32TunerPort;
    Hi3138_INNER_DEMOD_NUMBER enInnerDemodId;
    Hi3138_LNB_POWER_SOURCE enLNBSrc;
    HI_UNF_TUNER_SIG_TYPE_E enHi3138SigType;
    HI_UNF_TUNER_ADC_PORT_E enADCId;
    HI_UNF_TUNER_TS_OUTPUT_PORT_E enTSOutputId;
    HI_UNF_TUNER_IF_PATH_SEL_E enIFPath;
    HI_UNF_TUNER_I2C_TRANSMISSION_GATEWAY_E enI2CGateWaySel;
    HI_UNF_TUNER_SYNC_PIN_MUX_E               enSyncPin;
    HI_UNF_TUNER_OUPUT_MODE_E enTsType;
    HI_BOOL                     bTSCombFlag;
    HI_UNF_TUNER_TSOUT_SET_S stTSOut;
}Hi3138_SIGNAL_INFO_ALLOCATION;

typedef enum tagHI3138_C_LOCK_FLAG_E
{
    HI3138_C_AGC_LOCK_FLAG = 0,
    HI3138_C_TDP_LOCK_FLAG,
    HI3138_C_EQU_LOCK_FLAG,
    HI3138_C_SYNC_LOCK_FLAG,
    HI3138_C_FEC_LOCK_FLAG,
    HI3138_C_RESERVED_LOCK_1,
    HI3138_C_RESERVED_LOCK_2,
    HI3138_C_CR_SCAN_LOCK_FLAG
} HI3138_C_LOCK_FLAG_E;

HI_S32 hi3138_i2c_chn_sel(HI_U32 u32TunerPort);
HI_S32 hi3138_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
HI_S32 hi3138_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr);
HI_S32 hi3138_set_ter_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr);
HI_S32 hi3138_set_cab_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_CAB_ATTR_S *pstCabTunerAttr);
HI_S32 hi3138_set_multi_mode_chip_attr(HI_U32 u32TunerPort,HI_UNF_TUNER_MULTI_MODE_CHIP_ATTR_S *enPortAttr);
HI_S32 hi3138_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel);
HI_S32 hi3138_deinit(HI_VOID);
HI_VOID hi3138_connect_timeout(HI_U32 u32ConnectTimeout);
HI_S32 hi3138_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode);
HI_S32 hi3138_standby(HI_U32 u32TunerPort, HI_U32 u32Status);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
