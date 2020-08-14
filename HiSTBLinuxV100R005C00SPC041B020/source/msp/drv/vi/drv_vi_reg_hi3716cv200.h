#ifndef __DRV_VI_REG_HI3716CV200_H__
#define __DRV_VI_REG_HI3716CV200_H__

#include "hi_type.h"

/* Define the union U_WK_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int power_mode            : 1; /* [0]  */
        unsigned int Reserved_1            : 15; /* [15..1]  */
        unsigned int pt0_ppc_sel           : 1; /* [16]  */
        unsigned int Reserved_0            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_WK_MODE;

/* Define the union U_AXI_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int outstanding           : 4; /* [3..0]  */
        unsigned int Reserved_2            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_AXI_CFG;

/* Define the union U_APB_TIMEOUT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int timeout               : 16; /* [15..0]  */
        unsigned int Reserved_4            : 15; /* [30..16]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_APB_TIMEOUT;

/* Define the union U_VICAP_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int int_ch0               : 1; /* [0]  */
        unsigned int Reserved_7            : 15; /* [15..1]  */
        unsigned int int_pt0               : 1; /* [16]  */
        unsigned int Reserved_6            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_VICAP_INT;

/* Define the union U_VICAP_INT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int int_ch0_en            : 1; /* [0]  */
        unsigned int Reserved_10           : 15; /* [15..1]  */
        unsigned int int_pt0_en            : 1; /* [16]  */
        unsigned int Reserved_9            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_VICAP_INT_MASK;

/* Define the union U_PT_INTF_MOD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int mode                  : 1; /* [0]  */
        unsigned int Reserved_12           : 30; /* [30..1]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_MOD;

/* Define the union U_PT_OFFSET0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int offset                : 6; /* [5..0]  */
        unsigned int Reserved_14           : 9; /* [14..6]  */
        unsigned int rev                   : 1; /* [15]  */
        unsigned int mask                  : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_OFFSET0;

/* Define the union U_PT_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int offset                : 6; /* [5..0]  */
        unsigned int Reserved_16           : 9; /* [14..6]  */
        unsigned int rev                   : 1; /* [15]  */
        unsigned int mask                  : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_OFFSET1;

/* Define the union U_PT_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int offset                : 6; /* [5..0]  */
        unsigned int Reserved_17           : 9; /* [14..6]  */
        unsigned int rev                   : 1; /* [15]  */
        unsigned int mask                  : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_OFFSET2;

/* Define the union U_PT_BT656_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int mode                  : 4; /* [3..0]  */
        unsigned int Reserved_19           : 4; /* [7..4]  */
        unsigned int hsync_inv             : 1; /* [8]  */
        unsigned int vsync_inv             : 1; /* [9]  */
        unsigned int field_inv             : 1; /* [10]  */
        unsigned int Reserved_18           : 20; /* [30..11]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_BT656_CFG;

/* Define the union U_PT_UNIFY_TIMING_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int de_sel                : 2; /* [1..0]  */
        unsigned int de_inv                : 1; /* [2]  */
        unsigned int Reserved_24           : 5; /* [7..3]  */
        unsigned int hsync_sel             : 2; /* [9..8]  */
        unsigned int hsync_inv             : 1; /* [10]  */
        unsigned int hsync_and             : 2; /* [12..11]  */
        unsigned int hsync_mode            : 2; /* [14..13]  */
        unsigned int Reserved_23           : 1; /* [15]  */
        unsigned int vsync_sel             : 2; /* [17..16]  */
        unsigned int vsync_inv             : 1; /* [18]  */
        unsigned int vsync_mode            : 2; /* [20..19]  */
        unsigned int Reserved_22           : 3; /* [23..21]  */
        unsigned int field_sel             : 2; /* [25..24]  */
        unsigned int field_inv             : 1; /* [26]  */
        unsigned int Reserved_21           : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_UNIFY_TIMING_CFG;

/* Define the union U_PT_GEN_TIMING_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int Reserved_27           : 1; /* [0]  */
        unsigned int hsync_mode            : 1; /* [1]  */
        unsigned int vsync_mode            : 1; /* [2]  */
        unsigned int Reserved_26           : 28; /* [30..3]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_GEN_TIMING_CFG;

/* Define the union U_PT_UNIFY_DATA_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int comp_num              : 2; /* [1..0]  */
        unsigned int yc_seq                : 1; /* [2]  */
        unsigned int uv_seq                : 1; /* [3]  */
        unsigned int Reserved_28           : 27; /* [30..4]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_UNIFY_DATA_CFG;

/* Define the union U_PT_GEN_DATA_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int de_move               : 1; /* [0]  */
        unsigned int hsync_move            : 1; /* [1]  */
        unsigned int vsync_move            : 1; /* [2]  */
        unsigned int hsync_reset           : 1; /* [3]  */
        unsigned int vsync_reset           : 1; /* [4]  */
        unsigned int data2_move            : 1; /* [5]  */
        unsigned int data1_move            : 1; /* [6]  */
        unsigned int data0_move            : 1; /* [7]  */
        unsigned int Reserved_30           : 23; /* [30..8]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_GEN_DATA_CFG;

/* Define the union U_PT_GEN_DATA_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int step_space            : 8; /* [7..0]  */
        unsigned int inc_space             : 8; /* [15..8]  */
        unsigned int step_frame            : 8; /* [23..16]  */
        unsigned int inc_frame             : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_GEN_DATA_COEF;

/* Define the union U_PT_GEN_DATA_INIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int data0                 : 8; /* [7..0]  */
        unsigned int data1                 : 8; /* [15..8]  */
        unsigned int data2                 : 8; /* [23..16]  */
        unsigned int Reserved_31           : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_GEN_DATA_INIT;

/* Define the union U_PT_YUV444_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int Reserved_32           : 31; /* [30..0]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_YUV444_CFG;

/* Define the union U_PT_FSTART_DLY */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fstart_dly              : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_FSTART_DLY;

/* Define the union U_PT_INTF_HFB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int hfb                   : 16; /* [15..0]  */
        unsigned int Reserved_34           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_HFB;

/* Define the union U_PT_INTF_HACT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hact                    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_HACT;

/* Define the union U_PT_INTF_HBB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int hbb                   : 16; /* [15..0]  */
        unsigned int Reserved_36           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_HBB;

/* Define the union U_PT_INTF_VFB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int vfb                   : 16; /* [15..0]  */
        unsigned int Reserved_37           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_VFB;

/* Define the union U_PT_INTF_VACT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int vact                  : 16; /* [15..0]  */
        unsigned int Reserved_38           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_VACT;

/* Define the union U_PT_INTF_VBB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int vbb                   : 16; /* [15..0]  */
        unsigned int Reserved_39           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_VBB;

/* Define the union U_PT_INTF_VBFB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int vbfb                  : 16; /* [15..0]  */
        unsigned int Reserved_40           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_VBFB;

/* Define the union U_PT_INTF_VBACT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int vbact                 : 16; /* [15..0]  */
        unsigned int Reserved_41           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_VBACT;

/* Define the union U_PT_INTF_VBBB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int vbbb                  : 16; /* [15..0]  */
        unsigned int Reserved_42           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INTF_VBBB;

/* Define the union U_PT_STATUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int de                    : 1; /* [0]  */
        unsigned int hsync                 : 1; /* [1]  */
        unsigned int vysnc                 : 1; /* [2]  */
        unsigned int field                 : 1; /* [3]  */
        unsigned int Reserved_43           : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_STATUS;

/* Define the union U_PT_BT656_STATUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int Reserved_46           : 8; /* [7..0]  */
        unsigned int seav                  : 8; /* [15..8]  */
        unsigned int Reserved_45           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_BT656_STATUS;

/* Define the union U_PT_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 16; /* [15..0]  */
        unsigned int height                : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_SIZE;

/* Define the union U_PT_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int fstart                : 1; /* [0]  */
        unsigned int width_err             : 1; /* [1]  */
        unsigned int height_err            : 1; /* [2]  */
        unsigned int Reserved_48           : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INT;

/* Define the union U_PT_INT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int fstart_en             : 1; /* [0]  */
        unsigned int width_err_en          : 1; /* [1]  */
        unsigned int height_err_en         : 1; /* [2]  */
        unsigned int Reserved_49           : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_PT_INT_MASK;

/* Define the union U_CH_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int Reserved_51           : 31; /* [30..0]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CTRL;

/* Define the union U_CH_REG_NEWER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int reg_newer             : 1; /* [0]  */
        unsigned int Reserved_53           : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_REG_NEWER;

/* Define the union U_CH_ADAPTER_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int hsync_mode            : 1; /* [0]  */
        unsigned int vsync_mode            : 1; /* [1]  */
        unsigned int Reserved_54           : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_ADAPTER_CFG;

/* Define the union U_CH_PACK_Y_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int bitw                  : 8; /* [7..0]  */
        unsigned int Reserved_56           : 22; /* [29..8]  */
        unsigned int mode                  : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_PACK_Y_CFG;

/* Define the union U_CH_PACK_Y_WIDTH */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int width                   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_PACK_Y_WIDTH;

/* Define the union U_CH_PACK_C_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int bitw                  : 8; /* [7..0]  */
        unsigned int Reserved_58           : 22; /* [29..8]  */
        unsigned int mode                  : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_PACK_C_CFG;

/* Define the union U_CH_PACK_C_WIDTH */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int width                   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_PACK_C_WIDTH;

/* Define the union U_CH_DES_Y_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int mirror                : 1; /* [0]  */
        unsigned int flip                  : 1; /* [1]  */
        unsigned int Reserved_60           : 6; /* [7..2]  */
        unsigned int dis                   : 8; /* [15..8]  */
        unsigned int buf_len               : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_Y_CFG;

/* Define the union U_CH_DES_Y_FADDR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int faddr                   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_Y_FADDR0;

/* Define the union U_CH_DES_Y_FADDR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int faddr                   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_Y_FADDR1;

/* Define the union U_CH_DES_Y_HEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int height                : 16; /* [15..0]  */
        unsigned int Reserved_63           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_Y_HEIGHT;

/* Define the union U_CH_DES_Y_WIDTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 28; /* [27..0]  */
        unsigned int Reserved_64           : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_Y_WIDTH;

/* Define the union U_CH_DES_Y_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int stride                : 16; /* [15..0]  */
        unsigned int Reserved_65           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_Y_STRIDE;

/* Define the union U_CH_DES_C_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int mirror                : 1; /* [0]  */
        unsigned int flip                  : 1; /* [1]  */
        unsigned int Reserved_66           : 6; /* [7..2]  */
        unsigned int dis                   : 8; /* [15..8]  */
        unsigned int buf_len               : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_C_CFG;

/* Define the union U_CH_DES_C_FADDR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int faddr                   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_C_FADDR0;

/* Define the union U_CH_DES_C_FADDR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int faddr                   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_C_FADDR1;

/* Define the union U_CH_DES_C_HEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int height                : 16; /* [15..0]  */
        unsigned int Reserved_69           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_C_HEIGHT;

/* Define the union U_CH_DES_C_WIDTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 28; /* [27..0]  */
        unsigned int Reserved_70           : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_C_WIDTH;

/* Define the union U_CH_DES_C_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int stride                : 16; /* [15..0]  */
        unsigned int Reserved_71           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DES_C_STRIDE;

/* Define the union U_CH_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int fstart                : 1; /* [0]  */
        unsigned int cc_int                : 1; /* [1]  */
        unsigned int buf_ovf               : 1; /* [2]  */
        unsigned int field_throw           : 1; /* [3]  */
        unsigned int update_cfg            : 1; /* [4]  */
        unsigned int bus_err_c             : 1; /* [5]  */
        unsigned int bus_err_y             : 1; /* [6]  */
        unsigned int Reserved_73           : 1; /* [7]  */
        unsigned int cc_int_3d             : 1; /* [8]  */
        unsigned int buf_ovf_3d            : 1; /* [9]  */
        unsigned int bus_err_c_3d          : 1; /* [10]  */
        unsigned int bus_err_y_3d          : 1; /* [11]  */
        unsigned int Reserved_72           : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_INT;

/* Define the union U_CH_INT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int fstart_en             : 1; /* [0]  */
        unsigned int cc_int_en             : 1; /* [1]  */
        unsigned int buf_ovf_en            : 1; /* [2]  */
        unsigned int field_throw_en        : 1; /* [3]  */
        unsigned int update_cfg            : 1; /* [4]  */
        unsigned int bus_err_c_en          : 1; /* [5]  */
        unsigned int bus_err_y_en          : 1; /* [6]  */
        unsigned int Reserved_76           : 1; /* [7]  */
        unsigned int cc_int_en_3d          : 1; /* [8]  */
        unsigned int buf_ovf_en_3d         : 1; /* [9]  */
        unsigned int bus_err_c_en_3d       : 1; /* [10]  */
        unsigned int bus_err_y_en_3d       : 1; /* [11]  */
        unsigned int Reserved_75           : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_INT_MASK;

/* Define the union U_CH_CROP_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int n0_en                 : 1; /* [0]  */
        unsigned int n1_en                 : 1; /* [1]  */
        unsigned int Reserved_78           : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CROP_CFG;

/* Define the union U_CH_CROP_WIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 16; /* [15..0]  */
        unsigned int height                : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CROP_WIN;

/* Define the union U_CH_CROP0_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int x_start               : 16; /* [15..0]  */
        unsigned int y_start               : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CROP0_START;

/* Define the union U_CH_CROP0_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 16; /* [15..0]  */
        unsigned int height                : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CROP0_SIZE;

/* Define the union U_CH_CROP1_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int x_start               : 16; /* [15..0]  */
        unsigned int y_start               : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CROP1_START;

/* Define the union U_CH_CROP1_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 16; /* [15..0]  */
        unsigned int height                : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CROP1_SIZE;

/* Define the union U_CH_SKIP_Y_CFG */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int skip_cfg                : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_SKIP_Y_CFG;

/* Define the union U_CH_SKIP_Y_WIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 3; /* [2..0]  */
        unsigned int Reserved_84           : 13; /* [15..3]  */
        unsigned int height                : 2; /* [17..16]  */
        unsigned int Reserved_83           : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_SKIP_Y_WIN;

/* Define the union U_CH_SKIP_C_CFG */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int skip_cfg                : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_SKIP_C_CFG;

/* Define the union U_CH_SKIP_C_WIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 3; /* [2..0]  */
        unsigned int Reserved_87           : 13; /* [15..3]  */
        unsigned int height                : 2; /* [17..16]  */
        unsigned int Reserved_86           : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_SKIP_C_WIN;

/* Define the union U_CH_COEF_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int lhcoef_update         : 1; /* [0]  */
        unsigned int chcoef_update         : 1; /* [1]  */
        unsigned int Reserved_88           : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_COEF_UPDATE;

/* Define the union U_CH_COEF_RSEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int lhcoef_read_sel       : 1; /* [0]  */
        unsigned int chcoef_read_sel       : 1; /* [1]  */
        unsigned int Reserved_90           : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_COEF_RSEL;

/* Define the union U_CH_HBLANK_WIDTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int hblank_width          : 16; /* [15..0]  */
        unsigned int Reserved_91           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_HBLANK_WIDTH;

/* Define the union U_CH_LHFIR_SPH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int hlratio               : 24; /* [23..0]  */
        unsigned int Reserved_92           : 5; /* [28..24]  */
        unsigned int hlfir_en              : 1; /* [29]  */
        unsigned int hlmid_en              : 1; /* [30]  */
        unsigned int hlmsc_en              : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_LHFIR_SPH;

/* Define the union U_CH_CHFIR_SPH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int hchratio              : 24; /* [23..0]  */
        unsigned int Reserved_94           : 5; /* [28..24]  */
        unsigned int hchfir_en             : 1; /* [29]  */
        unsigned int hchmid_en             : 1; /* [30]  */
        unsigned int hchmsc_en             : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CHFIR_SPH;

/* Define the union U_CH_LHFIR_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int hluma_offset          : 28; /* [27..0]  */
        unsigned int Reserved_95           : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_LHFIR_OFFSET;

/* Define the union U_CH_CHFIR_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int hchroma_offset        : 28; /* [27..0]  */
        unsigned int Reserved_96           : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CHFIR_OFFSET;

/* Define the union U_CH_LFIR_IN_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 13; /* [12..0]  */
        unsigned int Reserved_97           : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_LFIR_IN_SIZE;

/* Define the union U_CH_CFIR_IN_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 13; /* [12..0]  */
        unsigned int Reserved_98           : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CFIR_IN_SIZE;

/* Define the union U_CH_LFIR_OUT_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 13; /* [12..0]  */
        unsigned int Reserved_99           : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_LFIR_OUT_SIZE;

/* Define the union U_CH_CFIR_OUT_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 13; /* [12..0]  */
        unsigned int Reserved_100          : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CFIR_OUT_SIZE;

/* Define the union U_CH_VCDS_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int Reserved_101          : 31; /* [30..0]  */
        unsigned int cds_en                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_VCDS_CFG;

/* Define the union U_CH_VCDS_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int coef0                 : 5; /* [4..0]  */
        unsigned int Reserved_104          : 11; /* [15..5]  */
        unsigned int coef1                 : 5; /* [20..16]  */
        unsigned int Reserved_103          : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_VCDS_COEF;

/* Define the union U_CH_DITHER_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int Reserved_105          : 29; /* [28..0]  */
        unsigned int dither_md             : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DITHER_CFG;

/* Define the union U_CH_DITHER_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int dither_coef0          : 8; /* [7..0]  */
        unsigned int dither_coef1          : 8; /* [15..8]  */
        unsigned int dither_coef2          : 8; /* [23..16]  */
        unsigned int dither_coef3          : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DITHER_COEF0;

/* Define the union U_CH_DITHER_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int dither_coef4          : 8; /* [7..0]  */
        unsigned int dither_coef5          : 8; /* [15..8]  */
        unsigned int dither_coef6          : 8; /* [23..16]  */
        unsigned int dither_coef7          : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DITHER_COEF1;

/* Define the union U_CH_DITHER_Y_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 13; /* [12..0]  */
        unsigned int Reserved_107          : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DITHER_Y_SIZE;

/* Define the union U_CH_DITHER_C_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 13; /* [12..0]  */
        unsigned int Reserved_109          : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_DITHER_C_SIZE;

/* Define the union U_CH_3D_SPLIT_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int mode                  : 1; /* [0]  */
        unsigned int Reserved_110          : 30; /* [30..1]  */
        unsigned int enable                : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_3D_SPLIT_CFG;

/* Define the union U_CH_3D_SPLIT_Y_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 13; /* [12..0]  */
        unsigned int Reserved_113          : 3; /* [15..13]  */
        unsigned int height                : 13; /* [28..16]  */
        unsigned int Reserved_112          : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_3D_SPLIT_Y_SIZE;

/* Define the union U_CH_3D_SPLIT_C_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int width                 : 13; /* [12..0]  */
        unsigned int Reserved_116          : 3; /* [15..13]  */
        unsigned int height                : 13; /* [28..16]  */
        unsigned int Reserved_115          : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_3D_SPLIT_C_SIZE;

/* Define the union U_CH_CLIP_Y_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int min                   : 16; /* [15..0]  */
        unsigned int max                   : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CLIP_Y_CFG;

/* Define the union U_CH_CLIP_C_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int min                   : 16; /* [15..0]  */
        unsigned int max                   : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CLIP_C_CFG;

/* Define the union U_CH_LHFIR_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int coef0                 : 10; /* [9..0]  */
        unsigned int coef1                 : 10; /* [19..10]  */
        unsigned int coef2                 : 10; /* [29..20]  */
        unsigned int Reserved_118          : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_LHFIR_COEF0;

/* Define the union U_CH_LHFIR_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int coef3                 : 10; /* [9..0]  */
        unsigned int coef4                 : 10; /* [19..10]  */
        unsigned int coef5                 : 10; /* [29..20]  */
        unsigned int Reserved_120          : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_LHFIR_COEF1;

/* Define the union U_CH_LHFIR_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int coef6                 : 10; /* [9..0]  */
        unsigned int coef7                 : 10; /* [19..10]  */
        unsigned int Reserved_121          : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_LHFIR_COEF2;

/* Define the union U_CH_CHFIR_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int coef0                 : 10; /* [9..0]  */
        unsigned int coef1                 : 10; /* [19..10]  */
        unsigned int coef2                 : 10; /* [29..20]  */
        unsigned int Reserved_122          : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CHFIR_COEF0;

/* Define the union U_CH_CHFIR_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int coef3                 : 10; /* [9..0]  */
        unsigned int Reserved_124          : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CH_CHFIR_COEF1;

/* Define the global struct */
typedef struct hiVI_REG_S
{
    volatile U_WK_MODE             WK_MODE;
    volatile unsigned int          Reserved_0[3];
    volatile U_AXI_CFG             AXI_CFG;
    volatile unsigned int          Reserved_1[51];
    volatile U_APB_TIMEOUT         APB_TIMEOUT;
    volatile unsigned int          Reserved_2[3];
    volatile U_VICAP_INT           VICAP_INT;
    volatile unsigned int          Reserved_3;
    volatile U_VICAP_INT_MASK      VICAP_INT_MASK;
    volatile unsigned int          Reserved_4;
    volatile U_PT_INTF_MOD         PT_INTF_MOD;
    volatile unsigned int          Reserved_5[3];
    volatile U_PT_OFFSET0          PT_OFFSET0;
    volatile U_PT_OFFSET1          PT_OFFSET1;
    volatile U_PT_OFFSET2          PT_OFFSET2;
    volatile unsigned int          Reserved_6;
    volatile U_PT_BT656_CFG        PT_BT656_CFG;
    volatile unsigned int          Reserved_7[3];
    volatile U_PT_UNIFY_TIMING_CFG PT_UNIFY_TIMING_CFG;
    volatile U_PT_GEN_TIMING_CFG   PT_GEN_TIMING_CFG;
    volatile unsigned int          Reserved_8[2];
    volatile U_PT_UNIFY_DATA_CFG   PT_UNIFY_DATA_CFG;
    volatile U_PT_GEN_DATA_CFG     PT_GEN_DATA_CFG;
    volatile U_PT_GEN_DATA_COEF    PT_GEN_DATA_COEF;
    volatile U_PT_GEN_DATA_INIT    PT_GEN_DATA_INIT;
    volatile U_PT_YUV444_CFG       PT_YUV444_CFG;
    volatile unsigned int          Reserved_9[3];
    volatile U_PT_FSTART_DLY       PT_FSTART_DLY;
    volatile unsigned int          Reserved_10[7];
    volatile U_PT_INTF_HFB         PT_INTF_HFB;
    volatile U_PT_INTF_HACT        PT_INTF_HACT;
    volatile U_PT_INTF_HBB         PT_INTF_HBB;
    volatile U_PT_INTF_VFB         PT_INTF_VFB;
    volatile U_PT_INTF_VACT        PT_INTF_VACT;
    volatile U_PT_INTF_VBB         PT_INTF_VBB;
    volatile U_PT_INTF_VBFB        PT_INTF_VBFB;
    volatile U_PT_INTF_VBACT       PT_INTF_VBACT;
    volatile U_PT_INTF_VBBB        PT_INTF_VBBB;
    volatile unsigned int          Reserved_11[15];
    volatile U_PT_STATUS           PT_STATUS;
    volatile U_PT_BT656_STATUS     PT_BT656_STATUS;
    volatile unsigned int          Reserved_12;
    volatile U_PT_SIZE             PT_SIZE;
    volatile U_PT_INT              PT_INT;
    volatile unsigned int          Reserved_13;
    volatile U_PT_INT_MASK         PT_INT_MASK;
    volatile unsigned int          Reserved_14[897];
    volatile U_CH_CTRL             CH_CTRL;
    volatile U_CH_REG_NEWER        CH_REG_NEWER;
    volatile unsigned int          Reserved_15[2];
    volatile U_CH_ADAPTER_CFG      CH_ADAPTER_CFG;
    volatile unsigned int          Reserved_16[27];
    volatile U_CH_PACK_Y_CFG       CH_PACK_Y_CFG;
    volatile U_CH_PACK_Y_WIDTH     CH_PACK_Y_WIDTH;
    volatile unsigned int          Reserved_17[2];
    volatile U_CH_PACK_C_CFG       CH_PACK_C_CFG;
    volatile U_CH_PACK_C_WIDTH     CH_PACK_C_WIDTH;
    volatile unsigned int          Reserved_18[6];
    volatile U_CH_DES_Y_CFG        CH_DES_Y_CFG;
    volatile unsigned int          Reserved_19;
    volatile U_CH_DES_Y_FADDR0     CH_DES_Y_FADDR0;
    volatile U_CH_DES_Y_FADDR1     CH_DES_Y_FADDR1;
    volatile U_CH_DES_Y_HEIGHT     CH_DES_Y_HEIGHT;
    volatile U_CH_DES_Y_WIDTH      CH_DES_Y_WIDTH;
    volatile U_CH_DES_Y_STRIDE     CH_DES_Y_STRIDE;
    volatile unsigned int          Reserved_20;
    volatile U_CH_DES_C_CFG        CH_DES_C_CFG;
    volatile unsigned int          Reserved_21;
    volatile U_CH_DES_C_FADDR0     CH_DES_C_FADDR0;
    volatile U_CH_DES_C_FADDR1     CH_DES_C_FADDR1;
    volatile U_CH_DES_C_HEIGHT     CH_DES_C_HEIGHT;
    volatile U_CH_DES_C_WIDTH      CH_DES_C_WIDTH;
    volatile U_CH_DES_C_STRIDE     CH_DES_C_STRIDE;
    volatile unsigned int          Reserved_22;
    volatile U_CH_INT              CH_INT;
    volatile unsigned int          Reserved_23;
    volatile U_CH_INT_MASK         CH_INT_MASK;
    volatile unsigned int          Reserved_24;
    volatile U_CH_CROP_CFG         CH_CROP_CFG;
    volatile U_CH_CROP_WIN         CH_CROP_WIN;
    volatile unsigned int          Reserved_25[2];
    volatile U_CH_CROP0_START      CH_CROP0_START;
    volatile U_CH_CROP0_SIZE       CH_CROP0_SIZE;
    volatile unsigned int          Reserved_26[2];
    volatile U_CH_CROP1_START      CH_CROP1_START;
    volatile U_CH_CROP1_SIZE       CH_CROP1_SIZE;
    volatile unsigned int          Reserved_27[54];
    volatile U_CH_SKIP_Y_CFG       CH_SKIP_Y_CFG;
    volatile U_CH_SKIP_Y_WIN       CH_SKIP_Y_WIN;
    volatile unsigned int          Reserved_28[2];
    volatile U_CH_SKIP_C_CFG       CH_SKIP_C_CFG;
    volatile U_CH_SKIP_C_WIN       CH_SKIP_C_WIN;
    volatile unsigned int          Reserved_29[58];
    volatile U_CH_COEF_UPDATE      CH_COEF_UPDATE;
    volatile U_CH_COEF_RSEL        CH_COEF_RSEL;
    volatile U_CH_HBLANK_WIDTH     CH_HBLANK_WIDTH;
    volatile unsigned int          Reserved_30;
    volatile U_CH_LHFIR_SPH        CH_LHFIR_SPH;
    volatile U_CH_CHFIR_SPH        CH_CHFIR_SPH;
    volatile U_CH_LHFIR_OFFSET     CH_LHFIR_OFFSET;
    volatile U_CH_CHFIR_OFFSET     CH_CHFIR_OFFSET;
    volatile U_CH_LFIR_IN_SIZE     CH_LFIR_IN_SIZE;
    volatile U_CH_CFIR_IN_SIZE     CH_CFIR_IN_SIZE;
    volatile U_CH_LFIR_OUT_SIZE    CH_LFIR_OUT_SIZE;
    volatile U_CH_CFIR_OUT_SIZE    CH_CFIR_OUT_SIZE;
    volatile unsigned int          Reserved_31[52];
    volatile U_CH_VCDS_CFG         CH_VCDS_CFG;
    volatile U_CH_VCDS_COEF        CH_VCDS_COEF;
    volatile unsigned int          Reserved_32[62];
    volatile U_CH_DITHER_CFG       CH_DITHER_CFG;
    volatile U_CH_DITHER_COEF0     CH_DITHER_COEF0;
    volatile U_CH_DITHER_COEF1     CH_DITHER_COEF1;
    volatile unsigned int          Reserved_33;
    volatile U_CH_DITHER_Y_SIZE    CH_DITHER_Y_SIZE;
    volatile U_CH_DITHER_C_SIZE    CH_DITHER_C_SIZE;
    volatile unsigned int          Reserved_34[58];
    volatile U_CH_3D_SPLIT_CFG     CH_3D_SPLIT_CFG;
    volatile unsigned int          Reserved_35[3];
    volatile U_CH_3D_SPLIT_Y_SIZE  CH_3D_SPLIT_Y_SIZE;
    volatile U_CH_3D_SPLIT_C_SIZE  CH_3D_SPLIT_C_SIZE;
    volatile unsigned int          Reserved_36[58];
    volatile U_CH_CLIP_Y_CFG       CH_CLIP_Y_CFG;
    volatile U_CH_CLIP_C_CFG       CH_CLIP_C_CFG;
    volatile unsigned int          Reserved_37[62];
    volatile U_CH_LHFIR_COEF0      CH_LHFIR_COEF0;
    volatile U_CH_LHFIR_COEF1      CH_LHFIR_COEF1;
    volatile U_CH_LHFIR_COEF2      CH_LHFIR_COEF2;
    volatile unsigned int          Reserved_38[125];
    volatile U_CH_CHFIR_COEF0      CH_CHFIR_COEF0;
    volatile U_CH_CHFIR_COEF1      CH_CHFIR_COEF1;
} VI_REG_S;

#endif // __DRV_VI_REG_HI3716CV200_H__
