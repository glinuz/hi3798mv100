/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAZ8tsS4we+PBWC6a/UNlzCWIaw+Ujkv9NAY+as0
fg7WZKaXakDBaVTnAvSfI2CBXozxoY1yeb+VZjTqkoVeLL4TG/k8oE5qR0l0APhsC0nvTy4L
nP7PwcVS509AWPKzIA4/ch7uM0HGSyu93Htvad/swhBb0ywLF12nD5KbA3sKFg==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
#ifndef _BITPLANE_STB_V200_HEADER_
#define _BITPLANE_STB_V200_HEADER_
#ifdef __cplusplus
extern "C" {
#endif

#include "vfmw_osal.h"
#include "public.h"
#include "vc1.h"
//#define Bpd_REG_SIZE       128  //寄存器信息长度，单位WORD

#define WAIT_NO_ISR_MAX 400

/************************************************************************/
/* BPD寄存器                                                            */
/************************************************************************/
//control registers
#define    REG_BPD_START                      0x000
#define    REG_BPD_CFG0                     0x004
#define    REG_BPD_CFG1                     0x008
#define    REG_BPD_CFG2                       0x00c
#define    REG_BPD_CFG3                       0x010
#define    REG_BPD_CFG4                       0x014
#define    REG_BPD_CFG5                       0x018
#define    REG_BPD_CFG6                       0x01c
#define    REG_BPD_CFG7                       0x020
#define    REG_BPD_CFG8                       0x024
#define    REG_BPD_CFG9                       0x028
#define    REG_BPD_CFG10                       0x02c
#define    REG_BPD_CFG11                       0x030
//state registers
#define    REG_BPD_INT_MASK                   0x034
#define    REG_BPD_STATE                      0x040
#define    REG_BPD_INT_STATE                 0x044
#define    REG_BPD_VCTRL_STATE               0x048
//emar & timeout registers
#define    REG_BPD_EMAR_ID                    0x030
//output registers
#define    REG_BPD_OUT0                       0x050
#define    REG_BPD_OUT1                       0x054

//寄存器组
typedef struct  {
    USIGN bpd_start:                            1;
    USIGN reserved:                             31;
} BPD_START;

typedef struct  {
    USIGN bit_offset:                           8;
    USIGN pic_mbwidth_mod3:                    2;
    USIGN pic_mbheigt_mod3:                    2;
    USIGN mvmode_en:                            1;
    USIGN overflag_en:                          1;
    USIGN pic_coding_type:                      2;
    USIGN pic_structure:                        2;
    USIGN profile:                               2;
    USIGN safe_flag:                            1;
    USIGN reserved:                             11;
} BPD_CFG0;

typedef struct  {
    USIGN bs_staddr:                           32;
} BPD_CFG1;

typedef struct  {
    USIGN pic_mbwidth_m1:                          16;
    USIGN pic_mbheight_m1:                         16;
} BPD_CFG2;

typedef struct  {
    USIGN bp_sride:                             12;
    USIGN reserved:                             20;
} BPD_CFG3;

typedef struct  {
    USIGN bp0_staddr:                        32;
} BPD_CFG4;

typedef struct  {
    USIGN bp1_staddr:                        32;
} BPD_CFG5;

typedef struct  {
    USIGN bp2_staddr:                        32;
} BPD_CFG6;

typedef struct  {
    USIGN bp3_staddr:                        32;
} BPD_CFG7;

typedef struct  {
    USIGN bp4_staddr:                        32;
} BPD_CFG8;

typedef struct  {
    USIGN bp5_staddr:                        32;
} BPD_CFG9;

typedef struct  {
    USIGN bp6_staddr:                        32;
} BPD_CFG10;

typedef struct  {
    USIGN axi_id:                                 4;
    USIGN axi_rd_outstanding:                   4;
    USIGN axi_wr_outstanding:                   4;
    USIGN reserved:                              20;
} BPD_CFG11;

typedef struct  {
    USIGN intm_bpd_dec_over:                    1;
    USIGN intm_bpd_dec_err:                     1;
    USIGN reserved:                              30;
} BPD_INT_MASK;

typedef struct  {
    USIGN intrs_bpd_dec_over:                    1;
    USIGN intrs_bpd_dec_err:                     1;
    USIGN bpd_rst_busy:                          1;
    USIGN reserved:                               29;
} BPD_STATE;

typedef struct  {
    USIGN intrms_bpd_dec_over:                    1;
    USIGN intrms_bpd_dec_err:                     1;
    USIGN reserved:                                30;
} BPD_INT_STATE;

typedef struct  {
    USIGN bpd_ctrl_state:                         32;
} BPD_CTRL_STATE;

typedef struct  {
    USIGN eaten_lenth:                          32;
} BPD_OUT0;

typedef struct  {
    USIGN MVTYPEMBMode:                         4;
    USIGN SKIPMBMode:                           4;
    USIGN DIRECTMBMode:                         4;
    USIGN ACPREDMode:                           4;
    USIGN OVERFLAGSMode:                        4;
    USIGN FIELDTXMode:                          4;
    USIGN FORWARDMBMode:                        4;
    USIGN CONDOVER:                              2;
    USIGN reserved:                              2;
} BPD_OUT1;

SINT32 BPD_Drv(BS *pBs, VC1_VFMWCODECINF_S *pCodecInfo, VC1_VFMWSYNTAX_S *pSyntax, SINT32 ChanID );

#ifdef __cplusplus
}
#endif

#endif

