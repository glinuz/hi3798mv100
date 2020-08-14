
#ifndef __VPSS_REG_98M_H__
#define __VPSS_REG_98M__H__


//CV200
#define VPSS_BASE_ADDR  0xf8cb0000
// Define the union U_VPSS_CTRL
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    str_en                : 1   ; /* [0]  */
        unsigned int    vhd_en                : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    vsd_mux               : 1   ; /* [4]  */
        unsigned int    uv_invert             : 1   ; /* [5]  */
        unsigned int    img_pro_mode          : 2   ; /* [7..6]  */
        unsigned int    str_det_en            : 1   ; /* [8]  */
        unsigned int    dei_en                : 1   ; /* [9]  */
        unsigned int    reserved_1            : 1   ; /* [10]  */
        unsigned int    dr_en                 : 1   ; /* [11]  */
        unsigned int    prot                  : 2   ; /* [13..12]  */
        unsigned int    str_lba_en            : 1   ; /* [14]  */
        unsigned int    vhd_lba_en            : 1   ; /* [15]  */
        unsigned int    reserved_2            : 1   ; /* [16]  */
        unsigned int    str_mute_en           : 1   ; /* [17]  */
        unsigned int    vhd_mute_en           : 1   ; /* [18]  */
        unsigned int    reserved_3            : 1   ; /* [19]  */
        unsigned int    vc1_en                : 1   ; /* [20]  */
        unsigned int    rotate_angle          : 1   ; /* [21]  */
        unsigned int    rotate_en             : 1   ; /* [22]  */
        unsigned int    pre_vfir_mode         : 2   ; /* [24..23]  */
        unsigned int    pre_vfir_en           : 1   ; /* [25]  */
        unsigned int    pre_hfir_mode         : 2   ; /* [27..26]  */
        unsigned int    pre_hfir_en           : 1   ; /* [28]  */
        unsigned int    bfield_first          : 1   ; /* [29]  */
        unsigned int    bfield_mode           : 1   ; /* [30]  */
        unsigned int    bfield                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CTRL;

/* Define the union U_VPSS_CTRL2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    str_format            : 4   ; /* [3..0]  */
        unsigned int    vhd_format            : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    in_format             : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CTRL2;

/* Define the union U_VPSS_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    imgwidth              : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    imgheight             : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_IMGSIZE;

/* Define the union U_VPSS_CUR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    cur_tile_format       : 1   ; /* [4]  */
        unsigned int    cur_hor_offset        : 13  ; /* [17..5]  */
        unsigned int    cur_ver_offset        : 13  ; /* [30..18]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CUR_CTRL;

/* Define the union U_VPSS_CURYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int cury_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CURYADDR;
/* Define the union U_VPSS_CURCADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int curc_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CURCADDR;
/* Define the union U_VPSS_CURCRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int curcr_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CURCRADDR;
/* Define the union U_VPSS_CURSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cury_stride           : 16  ; /* [15..0]  */
        unsigned int    curc_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CURSTRIDE;

/* Define the union U_VPSS_REF_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    ref_tile_format       : 1   ; /* [4]  */
        unsigned int    ref_hor_offset        : 13  ; /* [17..5]  */
        unsigned int    ref_ver_offset        : 13  ; /* [30..18]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_REF_CTRL;

/* Define the union U_VPSS_REFYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int refy_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REFYADDR;
/* Define the union U_VPSS_REFCADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int refc_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REFCADDR;
/* Define the union U_VPSS_REFCRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int refcr_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REFCRADDR;
/* Define the union U_VPSS_REFSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    refy_stride           : 16  ; /* [15..0]  */
        unsigned int    refc_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_REFSTRIDE;

/* Define the union U_VPSS_NXT1_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    nxt1_tile_format      : 1   ; /* [4]  */
        unsigned int    nxt1_hor_offset       : 13  ; /* [17..5]  */
        unsigned int    nxt1_ver_offset       : 13  ; /* [30..18]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT1_CTRL;

/* Define the union U_VPSS_NXT1YADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt1y_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT1YADDR;
/* Define the union U_VPSS_NXT1CADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt1c_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT1CADDR;
/* Define the union U_VPSS_NXT1CRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt1cr_addr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT1CRADDR;
/* Define the union U_VPSS_NXT1STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nxt1y_stride          : 16  ; /* [15..0]  */
        unsigned int    nxt1c_stride          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT1STRIDE;

/* Define the union U_VPSS_NXT2_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    nxt2_tile_format      : 1   ; /* [4]  */
        unsigned int    nxt2_hor_offset       : 13  ; /* [17..5]  */
        unsigned int    nxt2_ver_offset       : 13  ; /* [30..18]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT2_CTRL;

/* Define the union U_VPSS_NXT2YADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt2y_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT2YADDR;
/* Define the union U_VPSS_NXT2CADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt2c_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT2CADDR;
/* Define the union U_VPSS_NXT2CRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt2cr_addr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT2CRADDR;
/* Define the union U_VPSS_NXT2STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nxt2y_stride          : 16  ; /* [15..0]  */
        unsigned int    nxt2c_stride          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT2STRIDE;

/* Define the union U_VPSS_STRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int st_raddr               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STRADDR;
/* Define the union U_VPSS_STWADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int st_waddr               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STWADDR;
/* Define the union U_VPSS_STSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    st_stride             : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STSTRIDE;

/* Define the union U_VPSS_VHDCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    vhd_mirror            : 1   ; /* [4]  */
        unsigned int    vhd_flip              : 1   ; /* [5]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    vhd_uv_invert         : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDCTRL;

/* Define the union U_VPSS_VHDSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd_width             : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vhd_height            : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDSIZE;

/* Define the union U_VPSS_VHDYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhdy_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHDYADDR;
/* Define the union U_VPSS_VHDCADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhdc_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHDCADDR;
/* Define the union U_VPSS_VHDSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhdy_stride           : 16  ; /* [15..0]  */
        unsigned int    vhdc_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDSTRIDE;

/* Define the union U_VPSS_STRCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    str_mirror            : 1   ; /* [4]  */
        unsigned int    str_flip              : 1   ; /* [5]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    str_uv_invert         : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRCTRL;

/* Define the union U_VPSS_STRSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    str_width             : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    str_height            : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRSIZE;

/* Define the union U_VPSS_STRYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int stry_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STRYADDR;
/* Define the union U_VPSS_STRCADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int strc_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STRCADDR;
/* Define the union U_VPSS_STRSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    stry_stride           : 16  ; /* [15..0]  */
        unsigned int    strc_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRSTRIDE;

/* Define the union U_VPSS_TUNL_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd_tunl_finish_line  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 16  ; /* [27..12]  */
        unsigned int    vhd_tunl_en           : 1   ; /* [28]  */
        unsigned int    reserved_1            : 1   ; /* [29]  */
        unsigned int    str_tunl_en           : 1   ; /* [30]  */
        unsigned int    cur_tunl_en           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TUNL_CTRL0;

/* Define the union U_VPSS_TUNL_CTRL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    str_tunl_finish_line  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    cur_tunl_rd_interval  : 8   ; /* [23..16]  */
        unsigned int    vhd_tunl_mode         : 2   ; /* [25..24]  */
        unsigned int    reserved_1            : 2   ; /* [27..26]  */
        unsigned int    str_tunl_mode         : 2   ; /* [29..28]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TUNL_CTRL1;

/* Define the union U_VPSS_VHD_TUNL_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd_tunl_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD_TUNL_ADDR;
/* Define the union U_VPSS_STR_TUNL_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_tunl_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_TUNL_ADDR;
/* Define the union U_VPSS_CUR_TUNL_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int cur_tunl_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CUR_TUNL_ADDR;
/* Define the union U_VPSS_INCROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    in_crop_x             : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    in_crop_y             : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INCROP_POS;

/* Define the union U_VPSS_INCROP_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    in_crop_width         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    in_crop_height        : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 2   ; /* [29..28]  */
        unsigned int    in_crop_mode          : 1   ; /* [30]  */
        unsigned int    in_crop_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INCROP_SIZE;

/* Define the union U_VPSS_VHDCROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd_crop_x            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vhd_crop_y            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDCROP_POS;

/* Define the union U_VPSS_VHDCROP_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd_crop_width        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vhd_crop_height       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 3   ; /* [30..28]  */
        unsigned int    vhd_crop_en           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDCROP_SIZE;

/* Define the union U_VPSS_STRCROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    str_crop_x            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    str_crop_y            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRCROP_POS;

/* Define the union U_VPSS_STRCROP_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    str_crop_width        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    str_crop_height       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 3   ; /* [30..28]  */
        unsigned int    str_crop_en           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRCROP_SIZE;

/* Define the union U_VPSS_VHD_HSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hratio                : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 1   ; /* [24]  */
        unsigned int    hchfir_en             : 1   ; /* [25]  */
        unsigned int    hlfir_en              : 1   ; /* [26]  */
        unsigned int    reserved_1            : 1   ; /* [27]  */
        unsigned int    hchmid_en             : 1   ; /* [28]  */
        unsigned int    hlmid_en              : 1   ; /* [29]  */
        unsigned int    hchmsc_en             : 1   ; /* [30]  */
        unsigned int    hlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_HSP;

/* Define the union U_VPSS_VHD_HLOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor_loffset           : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_HLOFFSET;

/* Define the union U_VPSS_VHD_HCOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor_coffset           : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_HCOFFSET;

/* Define the union U_VPSS_VHD_VSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 19  ; /* [18..0]  */
        unsigned int    zme_in_fmt            : 2   ; /* [20..19]  */
        unsigned int    zme_out_fmt           : 2   ; /* [22..21]  */
        unsigned int    vchfir_en             : 1   ; /* [23]  */
        unsigned int    vlfir_en              : 1   ; /* [24]  */
        unsigned int    reserved_1            : 1   ; /* [25]  */
        unsigned int    vsc_chroma_tap        : 1   ; /* [26]  */
        unsigned int    reserved_2            : 1   ; /* [27]  */
        unsigned int    vchmid_en             : 1   ; /* [28]  */
        unsigned int    vlmid_en              : 1   ; /* [29]  */
        unsigned int    vchmsc_en             : 1   ; /* [30]  */
        unsigned int    vlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_VSP;

/* Define the union U_VPSS_VHD_VSR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vratio                : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_VSR;

/* Define the union U_VPSS_VHD_VOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vchroma_offset        : 16  ; /* [15..0]  */
        unsigned int    vluma_offset          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_VOFFSET;

/* Define the union U_VPSS_VHD_ZMEORESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ow                    : 12  ; /* [11..0]  */
        unsigned int    oh                    : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_ZMEORESO;

/* Define the union U_VPSS_VHD_ZMEIRESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    iw                    : 12  ; /* [11..0]  */
        unsigned int    ih                    : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_ZMEIRESO;

/* Define the union U_VPSS_VHD_LTI_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lhpass_coef4          : 8   ; /* [7..0]  */
        unsigned int    lmixing_ratio         : 8   ; /* [15..8]  */
        unsigned int    lgain_ratio           : 12  ; /* [27..16]  */
        unsigned int    reserved_0            : 2   ; /* [29..28]  */
        unsigned int    lti_demo_en           : 1   ; /* [30]  */
        unsigned int    lti_en                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_LTI_CTRL;

/* Define the union U_VPSS_VHD_LHPASS_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lhpass_coef0          : 8   ; /* [7..0]  */
        unsigned int    lhpass_coef1          : 8   ; /* [15..8]  */
        unsigned int    lhpass_coef2          : 8   ; /* [23..16]  */
        unsigned int    lhpass_coef3          : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_LHPASS_COEF;

/* Define the union U_VPSS_VHD_LTI_THD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lover_swing           : 10  ; /* [9..0]  */
        unsigned int    lunder_swing          : 10  ; /* [19..10]  */
        unsigned int    lcoring_thd           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_LTI_THD;

/* Define the union U_VPSS_VHD_LHFREQ_THD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lhfreq_thd0           : 16  ; /* [15..0]  */
        unsigned int    lhfreq_thd1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_LHFREQ_THD;

/* Define the union U_VPSS_VHD_LGAIN_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lgain_coef0           : 8   ; /* [7..0]  */
        unsigned int    lgain_coef1           : 8   ; /* [15..8]  */
        unsigned int    lgain_coef2           : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_LGAIN_COEF;

/* Define the union U_VPSS_VHD_CTI_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    cmixing_ratio         : 8   ; /* [15..8]  */
        unsigned int    cgain_ratio           : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 2   ; /* [29..28]  */
        unsigned int    cti_demo_en           : 1   ; /* [30]  */
        unsigned int    cti_en                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_CTI_CTRL;

/* Define the union U_VPSS_VHD_CHPASS_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chpass_coef0          : 8   ; /* [7..0]  */
        unsigned int    chpass_coef1          : 8   ; /* [15..8]  */
        unsigned int    chpass_coef2          : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_CHPASS_COEF;

/* Define the union U_VPSS_VHD_CTI_THD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cover_swing           : 10  ; /* [9..0]  */
        unsigned int    cunder_swing          : 10  ; /* [19..10]  */
        unsigned int    ccoring_thd           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD_CTI_THD;

/* Define the union U_VPSS_STR_HSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hratio                : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 1   ; /* [24]  */
        unsigned int    hchfir_en             : 1   ; /* [25]  */
        unsigned int    hlfir_en              : 1   ; /* [26]  */
        unsigned int    reserved_1            : 1   ; /* [27]  */
        unsigned int    hchmid_en             : 1   ; /* [28]  */
        unsigned int    hlmid_en              : 1   ; /* [29]  */
        unsigned int    hchmsc_en             : 1   ; /* [30]  */
        unsigned int    hlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STR_HSP;

/* Define the union U_VPSS_STR_HLOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor_loffset           : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STR_HLOFFSET;

/* Define the union U_VPSS_STR_HCOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor_coffset           : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STR_HCOFFSET;

/* Define the union U_VPSS_STR_VSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vratio                : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 5   ; /* [20..16]  */
        unsigned int    zme_out_fmt           : 2   ; /* [22..21]  */
        unsigned int    vchfir_en             : 1   ; /* [23]  */
        unsigned int    vlfir_en              : 1   ; /* [24]  */
        unsigned int    reserved_1            : 3   ; /* [27..25]  */
        unsigned int    vchmid_en             : 1   ; /* [28]  */
        unsigned int    vlmid_en              : 1   ; /* [29]  */
        unsigned int    vchmsc_en             : 1   ; /* [30]  */
        unsigned int    vlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STR_VSP;

/* Define the union U_VPSS_STR_VOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vchroma_offset        : 16  ; /* [15..0]  */
        unsigned int    vluma_offset          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STR_VOFFSET;

/* Define the union U_VPSS_STR_ZMEORESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ow                    : 12  ; /* [11..0]  */
        unsigned int    oh                    : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STR_ZMEORESO;

/* Define the union U_VPSS_DR_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    drthrmaxsimilarpixdiff : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 3   ; /* [23..21]  */
        unsigned int    drthrflat3x3zone      : 5   ; /* [28..24]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_CFG0;

/* Define the union U_VPSS_DR_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dralphascale          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    drbetascale           : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 3   ; /* [23..21]  */
        unsigned int    dr_demo_en            : 1   ; /* [24]  */
        unsigned int    reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_CFG1;

/* Define the union U_VPSS_DNR_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    thrintlcolcnt         : 4   ; /* [3..0]  */
        unsigned int    thrintlcnt            : 4   ; /* [7..4]  */
        unsigned int    thdmaxgrad            : 8   ; /* [15..8]  */
        unsigned int    thdcntrst8            : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DNR_INFO;

/* Define the union U_VPSS_NODEID */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nodeid                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NODEID;
/* Define the union U_VPSS_AXIID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 20  ; /* [19..0]  */
        unsigned int    awid_cfg0             : 4   ; /* [23..20]  */
        unsigned int    reserved_1            : 4   ; /* [27..24]  */
        unsigned int    arid_cfg0             : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_AXIID;

/* Define the union U_VPSS_INTMASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    eof_mask              : 1   ; /* [0]  */
        unsigned int    timeout_mask          : 1   ; /* [1]  */
        unsigned int    bus_w_err_mask        : 1   ; /* [2]  */
        unsigned int    eof_end_mask          : 1   ; /* [3]  */
        unsigned int    vhd_tunl_mask         : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    str_tunl_mask         : 1   ; /* [6]  */
        unsigned int    dcmp_err_mask         : 1   ; /* [7]  */
        unsigned int    reserved_1            : 2   ; /* [9..8]  */
        unsigned int    bus_r_err_mask        : 1   ; /* [10]  */
        unsigned int    reserved_2            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INTMASK;

/* Define the union U_VPSS_VHD_ZME_LHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd_scl_lh             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD_ZME_LHADDR;
/* Define the union U_VPSS_VHD_ZME_LVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd_scl_lv             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD_ZME_LVADDR;
/* Define the union U_VPSS_VHD_ZME_CHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd_scl_ch             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD_ZME_CHADDR;
/* Define the union U_VPSS_VHD_ZME_CVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd_scl_cv             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD_ZME_CVADDR;
/* Define the union U_VPSS_STR_ZME_LHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_scl_lh             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_ZME_LHADDR;
/* Define the union U_VPSS_STR_ZME_LVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_scl_lv             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_ZME_LVADDR;
/* Define the union U_VPSS_STR_ZME_CHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_scl_ch             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_ZME_CHADDR;
/* Define the union U_VPSS_STR_ZME_CVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_scl_cv             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_ZME_CVADDR;



// Define the union U_VPSS_VC1_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vc1_profile           : 2   ; // [1..0] 
        unsigned int    vc1_rangedfrm         : 1   ; // [2] 
        unsigned int    vc1_mapyflg           : 1   ; // [3] 
        unsigned int    vc1_mapcflg           : 1   ; // [4] 
        unsigned int    vc1_bmapyflg          : 1   ; // [5] 
        unsigned int    vc1_bmapcflg          : 1   ; // [6] 
        unsigned int    Reserved_81           : 1   ; // [7] 
        unsigned int    vc1_mapy              : 3   ; // [10..8] 
        unsigned int    Reserved_80           : 1   ; // [11] 
        unsigned int    vc1_mapc              : 3   ; // [14..12] 
        unsigned int    Reserved_79           : 1   ; // [15] 
        unsigned int    vc1_bmapy             : 3   ; // [18..16] 
        unsigned int    Reserved_78           : 1   ; // [19] 
        unsigned int    vc1_bmapc             : 3   ; // [22..20] 
        unsigned int    Reserved_77           : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPSS_VC1_CTRL;


/* Define the union U_VPSS_VC1_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fr0_vc1_profile       : 2   ; /* [1..0]  */
        unsigned int    fr0_vc1_rangedfrm     : 1   ; /* [2]  */
        unsigned int    fr0_vc1_mapyflg       : 1   ; /* [3]  */
        unsigned int    fr0_vc1_mapcflg       : 1   ; /* [4]  */
        unsigned int    fr0_vc1_bmapyflg      : 1   ; /* [5]  */
        unsigned int    fr0_vc1_bmapcflg      : 1   ; /* [6]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    fr0_vc1_mapy          : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    fr0_vc1_mapc          : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 1   ; /* [15]  */
        unsigned int    fr0_vc1_bmapy         : 3   ; /* [18..16]  */
        unsigned int    reserved_3            : 1   ; /* [19]  */
        unsigned int    fr0_vc1_bmapc         : 3   ; /* [22..20]  */
        unsigned int    reserved_4            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VC1_CTRL0;

/* Define the union U_VPSS_VC1_CTRL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fr1_vc1_profile       : 2   ; /* [1..0]  */
        unsigned int    fr1_vc1_rangedfrm     : 1   ; /* [2]  */
        unsigned int    fr1_vc1_mapyflg       : 1   ; /* [3]  */
        unsigned int    fr1_vc1_mapcflg       : 1   ; /* [4]  */
        unsigned int    fr1_vc1_bmapyflg      : 1   ; /* [5]  */
        unsigned int    fr1_vc1_bmapcflg      : 1   ; /* [6]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    fr1_vc1_mapy          : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    fr1_vc1_mapc          : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 1   ; /* [15]  */
        unsigned int    fr1_vc1_bmapy         : 3   ; /* [18..16]  */
        unsigned int    reserved_3            : 1   ; /* [19]  */
        unsigned int    fr1_vc1_bmapc         : 3   ; /* [22..20]  */
        unsigned int    reserved_4            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VC1_CTRL1;

/* Define the union U_VPSS_VC1_CTRL2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fr2_vc1_profile       : 2   ; /* [1..0]  */
        unsigned int    fr2_vc1_rangedfrm     : 1   ; /* [2]  */
        unsigned int    fr2_vc1_mapyflg       : 1   ; /* [3]  */
        unsigned int    fr2_vc1_mapcflg       : 1   ; /* [4]  */
        unsigned int    fr2_vc1_bmapyflg      : 1   ; /* [5]  */
        unsigned int    fr2_vc1_bmapcflg      : 1   ; /* [6]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    fr2_vc1_mapy          : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    fr2_vc1_mapc          : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 1   ; /* [15]  */
        unsigned int    fr2_vc1_bmapy         : 3   ; /* [18..16]  */
        unsigned int    reserved_3            : 1   ; /* [19]  */
        unsigned int    fr2_vc1_bmapc         : 3   ; /* [22..20]  */
        unsigned int    reserved_4            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VC1_CTRL2;

/* Define the union U_VPSS_DEI_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int dei_cfg_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEI_ADDR;
/* Define the union U_VPSS_HSCL_LADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hscl_laddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_HSCL_LADDR;
/* Define the union U_VPSS_HSCL_CADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hscl_caddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_HSCL_CADDR;
/* Define the union U_VPSS_VHDLBA_DFPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    disp_xfpos            : 12  ; /* [11..0]  */
        unsigned int    disp_yfpos            : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDLBA_DFPOS;

/* Define the union U_VPSS_VHDLBA_DSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    disp_width            : 12  ; /* [11..0]  */
        unsigned int    disp_height           : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDLBA_DSIZE;

/* Define the union U_VPSS_VHDLBA_VFPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    video_xfpos           : 12  ; /* [11..0]  */
        unsigned int    video_yfpos           : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDLBA_VFPOS;

/* Define the union U_VPSS_VHDLBA_VSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    video_width           : 12  ; /* [11..0]  */
        unsigned int    video_height          : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDLBA_VSIZE;

/* Define the union U_VPSS_VHDLBA_BK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vbk_cr                : 8   ; /* [7..0]  */
        unsigned int    vbk_cb                : 8   ; /* [15..8]  */
        unsigned int    vbk_y                 : 8   ; /* [23..16]  */
        unsigned int    vbk_alpha             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHDLBA_BK;

/* Define the union U_VPSS_STRLBA_DFPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    disp_xfpos            : 12  ; /* [11..0]  */
        unsigned int    disp_yfpos            : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRLBA_DFPOS;

/* Define the union U_VPSS_STRLBA_DSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    disp_width            : 12  ; /* [11..0]  */
        unsigned int    disp_height           : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRLBA_DSIZE;

/* Define the union U_VPSS_STRLBA_VFPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    video_xfpos           : 12  ; /* [11..0]  */
        unsigned int    video_yfpos           : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRLBA_VFPOS;

/* Define the union U_VPSS_STRLBA_VSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    video_width           : 12  ; /* [11..0]  */
        unsigned int    video_height          : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRLBA_VSIZE;

/* Define the union U_VPSS_STRLBA_BK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vbk_cr                : 8   ; /* [7..0]  */
        unsigned int    vbk_cb                : 8   ; /* [15..8]  */
        unsigned int    vbk_y                 : 8   ; /* [23..16]  */
        unsigned int    vbk_alpha             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STRLBA_BK;

/* Define the union U_STR_DET_VIDCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 30  ; /* [29..0]  */
        unsigned int    vid_mode              : 1   ; /* [30]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDCTRL;

/* Define the union U_STR_DET_VIDBLKPOS0_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk0_xlpos            : 8   ; /* [7..0]  */
        unsigned int    blk0_ylpos            : 8   ; /* [15..8]  */
        unsigned int    blk1_xlpos            : 8   ; /* [23..16]  */
        unsigned int    blk1_ylpos            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLKPOS0_1;

/* Define the union U_STR_DET_VIDBLKPOS2_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk2_xlpos            : 8   ; /* [7..0]  */
        unsigned int    blk2_ylpos            : 8   ; /* [15..8]  */
        unsigned int    blk3_xlpos            : 8   ; /* [23..16]  */
        unsigned int    blk3_ylpos            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLKPOS2_3;

/* Define the union U_STR_DET_VIDBLKPOS4_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk4_xlpos            : 8   ; /* [7..0]  */
        unsigned int    blk4_ylpos            : 8   ; /* [15..8]  */
        unsigned int    blk5_xlpos            : 8   ; /* [23..16]  */
        unsigned int    blk5_ylpos            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLKPOS4_5;

/* Define the union U_VPSS_HSCL_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hratio                : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 1   ; /* [24]  */
        unsigned int    hchfir_en             : 1   ; /* [25]  */
        unsigned int    hlfir_en              : 1   ; /* [26]  */
        unsigned int    reserved_1            : 1   ; /* [27]  */
        unsigned int    hchmid_en             : 1   ; /* [28]  */
        unsigned int    hlmid_en              : 1   ; /* [29]  */
        unsigned int    hchmsc_en             : 1   ; /* [30]  */
        unsigned int    hlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSCL_CTRL;

/* Define the union U_VPSS_HSCL_ORESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hscl_ow               : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSCL_ORESO;

/* Define the union U_VPSS_HSCL_HLOFF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hscl_loffset          : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSCL_HLOFF;

/* Define the union U_VPSS_HSCL_HCOFF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hscl_coffset          : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSCL_HCOFF;

/* Define the union U_VPSS_PNEXT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int p_next                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PNEXT;
/* Define the union U_VPSS_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    start                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_START;

/* Define the union U_VPSS_INTSTATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    eof_state             : 1   ; /* [0]  */
        unsigned int    timeout_state         : 1   ; /* [1]  */
        unsigned int    bus_w_err             : 1   ; /* [2]  */
        unsigned int    eof_end_state         : 1   ; /* [3]  */
        unsigned int    vhd_tunl_state        : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    str_tunl_state        : 1   ; /* [6]  */
        unsigned int    dcmp_err              : 1   ; /* [7]  */
        unsigned int    reserved_1            : 2   ; /* [9..8]  */
        unsigned int    bus_r_err             : 1   ; /* [10]  */
        unsigned int    reserved_2            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INTSTATE;

/* Define the union U_VPSS_INTCLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    eof_clr               : 1   ; /* [0]  */
        unsigned int    timeout_clr           : 1   ; /* [1]  */
        unsigned int    bus_w_err_clr         : 1   ; /* [2]  */
        unsigned int    eof_end_clr           : 1   ; /* [3]  */
        unsigned int    vhd_tunl_clr          : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    str_tunl_clr          : 1   ; /* [6]  */
        unsigned int    dcmp_err_clr          : 1   ; /* [7]  */
        unsigned int    reserved_1            : 2   ; /* [9..8]  */
        unsigned int    bus_r_err_clr         : 1   ; /* [10]  */
        unsigned int    reserved_2            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INTCLR;

/* Define the union U_VPSS_RAWINT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    raw_eof               : 1   ; /* [0]  */
        unsigned int    raw_timeout           : 1   ; /* [1]  */
        unsigned int    raw_w_bus_err         : 1   ; /* [2]  */
        unsigned int    raw_eof_end           : 1   ; /* [3]  */
        unsigned int    raw_vhd_state         : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    raw_str_state         : 1   ; /* [6]  */
        unsigned int    raw_dcmp_err          : 1   ; /* [7]  */
        unsigned int    reserved_1            : 2   ; /* [9..8]  */
        unsigned int    raw_r_bus_err         : 1   ; /* [10]  */
        unsigned int    reserved_2            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RAWINT;

/* Define the union U_VPSS_PFCNT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pfcnt                  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PFCNT;
/* Define the union U_VPSS_MISCELLANEOUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    routstanding          : 4   ; /* [3..0]  */
        unsigned int    woutstanding          : 4   ; /* [7..4]  */
        unsigned int    init_timer            : 16  ; /* [23..8]  */
        unsigned int    ck_gt_en              : 1   ; /* [24]  */
        unsigned int    ck_gt_en_calc         : 1   ; /* [25]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MISCELLANEOUS;

/* Define the union U_VPSS_MACCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mac_ch_prio           : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MACCFG;

/* Define the union U_VPSS_TIMEOUT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int timeout                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_TIMEOUT;
/* Define the union U_VPSS_EOFCNT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int eof_cnt                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_EOFCNT;
/* Define the union U_VPSS_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_version           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VERSION;
/* Define the union U_VPSS_DIECTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    st_rst_value          : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 1   ; /* [16]  */
        unsigned int    die_rst               : 1   ; /* [17]  */
        unsigned int    reserved_1            : 6   ; /* [23..18]  */
        unsigned int    die_chmmode           : 2   ; /* [25..24]  */
        unsigned int    die_lmmode            : 2   ; /* [27..26]  */
        unsigned int    die_out_sel_c         : 1   ; /* [28]  */
        unsigned int    die_out_sel_l         : 1   ; /* [29]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECTRL;

/* Define the union U_VPSS_DIELMA2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    recmode_frmfld_blend_mode : 1   ; /* [0]  */
        unsigned int    frame_motion_smooth_en : 1   ; /* [1]  */
        unsigned int    luma_scesdf_max       : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    motion_iir_en         : 1   ; /* [4]  */
        unsigned int    luma_mf_max           : 1   ; /* [5]  */
        unsigned int    chroma_mf_max         : 1   ; /* [6]  */
        unsigned int    rec_mode_en           : 1   ; /* [7]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIELMA2;

/* Define the union U_VPSS_DIEINTEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dir_inten_ver         : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    ver_min_inten         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEINTEN;

/* Define the union U_VPSS_DIESCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    range_scale           : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIESCALE;

/* Define the union U_VPSS_DIECHECK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ck1_max_range         : 8   ; /* [7..0]  */
        unsigned int    ck1_range_gain        : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    ck1_gain              : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECHECK1;

/* Define the union U_VPSS_DIECHECK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ck2_max_range         : 8   ; /* [7..0]  */
        unsigned int    ck2_range_gain        : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    ck2_gain              : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECHECK2;

/* Define the union U_VPSS_DIEDIR0_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir0_mult             : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir1_mult             : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir2_mult             : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir3_mult             : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIR0_3;

/* Define the union U_VPSS_DIEDIR4_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir4_mult             : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir5_mult             : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir6_mult             : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir7_mult             : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIR4_7;

/* Define the union U_VPSS_DIEDIR8_11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir8_mult             : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir9_mult             : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir10_mult            : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir11_mult            : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIR8_11;

/* Define the union U_VPSS_DIEDIR12_14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir12_mult            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir13_mult            : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir14_mult            : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIR12_14;

/* Define the union U_VPSS_DIESTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_state             : 4   ; /* [3..0]  */
        unsigned int    cur_cstate            : 4   ; /* [7..4]  */
        unsigned int    l_height_cnt          : 12  ; /* [19..8]  */
        unsigned int    c_height_cnt          : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIESTA;

/* Define the union U_VPSS_DIESTPPREADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int die_ppre_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIESTPPREADDR;
/* Define the union U_VPSS_DIESTPREADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int die_pre_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIESTPREADDR;
/* Define the union U_VPSS_DIEINTPSCL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intp_scale_ratio_1    : 4   ; /* [3..0]  */
        unsigned int    intp_scale_ratio_2    : 4   ; /* [7..4]  */
        unsigned int    intp_scale_ratio_3    : 4   ; /* [11..8]  */
        unsigned int    intp_scale_ratio_4    : 4   ; /* [15..12]  */
        unsigned int    intp_scale_ratio_5    : 4   ; /* [19..16]  */
        unsigned int    intp_scale_ratio_6    : 4   ; /* [23..20]  */
        unsigned int    intp_scale_ratio_7    : 4   ; /* [27..24]  */
        unsigned int    intp_scale_ratio_8    : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEINTPSCL0;

/* Define the union U_VPSS_DIEINTPSCL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intp_scale_ratio_9    : 4   ; /* [3..0]  */
        unsigned int    intp_scale_ratio_10   : 4   ; /* [7..4]  */
        unsigned int    intp_scale_ratio_11   : 4   ; /* [11..8]  */
        unsigned int    intp_scale_ratio_12   : 4   ; /* [15..12]  */
        unsigned int    intp_scale_ratio_13   : 4   ; /* [19..16]  */
        unsigned int    intp_scale_ratio_14   : 4   ; /* [23..20]  */
        unsigned int    intp_scale_ratio_15   : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEINTPSCL1;

/* Define the union U_VPSS_DIEDIRTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bc_gain               : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    dir_thd               : 4   ; /* [11..8]  */
        unsigned int    edge_mode             : 1   ; /* [12]  */
        unsigned int    hor_edge_en           : 1   ; /* [13]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    strength_thd          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIRTHD;

/* Define the union U_VPSS_DIEJITMTN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    jitter_gain           : 4   ; /* [3..0]  */
        unsigned int    jitter_coring         : 8   ; /* [11..4]  */
        unsigned int    fld_motion_coring     : 8   ; /* [19..12]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEJITMTN;

/* Define the union U_VPSS_DIEFLDMTN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fld_motion_thd_low    : 8   ; /* [7..0]  */
        unsigned int    fld_motion_thd_high   : 8   ; /* [15..8]  */
        unsigned int    fld_motion_curve_slope : 6   ; /* [21..16]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    fld_motion_gain       : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFLDMTN;

/* Define the union U_VPSS_DIEMTNDIFFTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_diff_thd_0     : 8   ; /* [7..0]  */
        unsigned int    motion_diff_thd_1     : 8   ; /* [15..8]  */
        unsigned int    motion_diff_thd_2     : 8   ; /* [23..16]  */
        unsigned int    motion_diff_thd_3     : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNDIFFTHD0;

/* Define the union U_VPSS_DIEMTNDIFFTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_diff_thd_4     : 8   ; /* [7..0]  */
        unsigned int    motion_diff_thd_5     : 8   ; /* [15..8]  */
        unsigned int    min_motion_iir_ratio  : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    max_motion_iir_ratio  : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNDIFFTHD1;

/* Define the union U_VPSS_DIEMTNIIRSLP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_iir_curve_slope_0 : 6   ; /* [5..0]  */
        unsigned int    motion_iir_curve_slope_1 : 6   ; /* [11..6]  */
        unsigned int    motion_iir_curve_slope_2 : 6   ; /* [17..12]  */
        unsigned int    motion_iir_curve_slope_3 : 6   ; /* [23..18]  */
        unsigned int    motion_iir_curve_ratio_0 : 7   ; /* [30..24]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNIIRSLP;

/* Define the union U_VPSS_DIEMTNIIRRAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_iir_curve_ratio_1 : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    motion_iir_curve_ratio_2 : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    motion_iir_curve_ratio_3 : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    motion_iir_curve_ratio_4 : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNIIRRAT;

/* Define the union U_VPSS_DIEHISMODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rec_mode_fld_motion_step_0 : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    rec_mode_fld_motion_step_1 : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    rec_mode_frm_motion_step_0 : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    rec_mode_frm_motion_step_1 : 2   ; /* [13..12]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    ppre_info_en          : 1   ; /* [16]  */
        unsigned int    pre_info_en           : 1   ; /* [17]  */
        unsigned int    his_motion_en         : 1   ; /* [18]  */
        unsigned int    his_motion_using_mode : 1   ; /* [19]  */
        unsigned int    his_motion_write_mode : 1   ; /* [20]  */
        unsigned int    his_motion_info_write_mode : 1   ; /* [21]  */
        unsigned int    reserved_4            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEHISMODE;

/* Define the union U_VPSS_DIEMORFLT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mor_flt_thd           : 8   ; /* [7..0]  */
        unsigned int    mor_flt_size          : 2   ; /* [9..8]  */
        unsigned int    reserved_0            : 14  ; /* [23..10]  */
        unsigned int    mor_flt_en            : 1   ; /* [24]  */
        unsigned int    reserved_1            : 1   ; /* [25]  */
        unsigned int    med_blend_en          : 1   ; /* [26]  */
        unsigned int    reserved_2            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMORFLT;

/* Define the union U_VPSS_DIECOMBCHK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    comb_chk_min_hthd     : 8   ; /* [7..0]  */
        unsigned int    comb_chk_min_vthd     : 8   ; /* [15..8]  */
        unsigned int    comb_chk_lower_limit  : 8   ; /* [23..16]  */
        unsigned int    comb_chk_upper_limit  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECOMBCHK0;

/* Define the union U_VPSS_DIECOMBCHK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    comb_chk_edge_thd     : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    comb_chk_md_thd       : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    comb_chk_en           : 1   ; /* [16]  */
        unsigned int    reserved_2            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECOMBCHK1;

/* Define the union U_VPSS_DIEFRMMTNSMTHTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_motion_smooth_thd0 : 8   ; /* [7..0]  */
        unsigned int    frame_motion_smooth_thd1 : 8   ; /* [15..8]  */
        unsigned int    frame_motion_smooth_thd2 : 8   ; /* [23..16]  */
        unsigned int    frame_motion_smooth_thd3 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMMTNSMTHTHD0;

/* Define the union U_VPSS_DIEFRMMTNSMTHTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_motion_smooth_thd4 : 8   ; /* [7..0]  */
        unsigned int    frame_motion_smooth_thd5 : 8   ; /* [15..8]  */
        unsigned int    frame_motion_smooth_ratio_min : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    frame_motion_smooth_ratio_max : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMMTNSMTHTHD1;

/* Define the union U_VPSS_DIEFRMMTNSMTHSLP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_motion_smooth_slope0 : 6   ; /* [5..0]  */
        unsigned int    frame_motion_smooth_slope1 : 6   ; /* [11..6]  */
        unsigned int    frame_motion_smooth_slope2 : 6   ; /* [17..12]  */
        unsigned int    frame_motion_smooth_slope3 : 6   ; /* [23..18]  */
        unsigned int    frame_motion_smooth_ratio0 : 7   ; /* [30..24]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMMTNSMTHSLP;

/* Define the union U_VPSS_DIEFRMMTNSMTHRAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_motion_smooth_ratio1 : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    frame_motion_smooth_ratio2 : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    frame_motion_smooth_ratio3 : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    frame_motion_smooth_ratio4 : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMMTNSMTHRAT;

/* Define the union U_VPSS_DIEFRMFLDBLENDTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_field_blend_thd0 : 8   ; /* [7..0]  */
        unsigned int    frame_field_blend_thd1 : 8   ; /* [15..8]  */
        unsigned int    frame_field_blend_thd2 : 8   ; /* [23..16]  */
        unsigned int    frame_field_blend_thd3 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMFLDBLENDTHD0;

/* Define the union U_VPSS_DIEFRMFLDBLENDTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_field_blend_thd4 : 8   ; /* [7..0]  */
        unsigned int    frame_field_blend_thd5 : 8   ; /* [15..8]  */
        unsigned int    frame_field_blend_ratio_min : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    frame_field_blend_ratio_max : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMFLDBLENDTHD1;

/* Define the union U_VPSS_DIEFRMFLDBLENDSLP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_field_blend_slope0 : 6   ; /* [5..0]  */
        unsigned int    frame_field_blend_slope1 : 6   ; /* [11..6]  */
        unsigned int    frame_field_blend_slope2 : 6   ; /* [17..12]  */
        unsigned int    frame_field_blend_slope3 : 6   ; /* [23..18]  */
        unsigned int    frame_field_blend_ratio0 : 7   ; /* [30..24]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMFLDBLENDSLP;

/* Define the union U_VPSS_DIEFRMFLDBLENDRAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_field_blend_ratio1 : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    frame_field_blend_ratio2 : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    frame_field_blend_ratio3 : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    frame_field_blend_ratio4 : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMFLDBLENDRAT;

/* Define the union U_VPSS_DIEMTNADJ */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_adjust_gain    : 8   ; /* [7..0]  */
        unsigned int    motion_adjust_coring  : 6   ; /* [13..8]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    motion_adjust_gain_chr : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNADJ;

/* Define the union U_VPSS_DIEGLBMTNTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_thd0           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    motion_thd1           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEGLBMTNTHD0;

/* Define the union U_VPSS_DIEGLBMTNTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_thd2           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    motion_thd3           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEGLBMTNTHD1;

/* Define the union U_VPSS_CCRSCLR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    no_ccr_detect_thd     : 10  ; /* [9..0]  */
        unsigned int    no_ccr_detect_max     : 10  ; /* [19..10]  */
        unsigned int    chroma_ma_offset      : 10  ; /* [29..20]  */
        unsigned int    chroma_ccr_en         : 1   ; /* [30]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CCRSCLR0;

/* Define the union U_VPSS_CCRSCLR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    max_xchroma           : 10  ; /* [9..0]  */
        unsigned int    no_ccr_detect_blend   : 4   ; /* [13..10]  */
        unsigned int    similar_thd           : 9   ; /* [22..14]  */
        unsigned int    similar_max           : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CCRSCLR1;

/* Define the union U_VPSS_DIEGLBMTNBIN */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glb_mtn_bin            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEGLBMTNBIN;
/* Define the union U_VPSS_DIEMTNNUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_num0              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNNUM0;
/* Define the union U_VPSS_DIEMTNSUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_sum0              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNSUM0;
/* Define the union U_VPSS_DIEMTNNUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_num1              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNNUM1;
/* Define the union U_VPSS_DIEMTNSUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_sum1              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNSUM1;
/* Define the union U_VPSS_DIEMTNNUM2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_num2              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNNUM2;
/* Define the union U_VPSS_DIEMTNSUM2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_sum2              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNSUM2;
/* Define the union U_VPSS_DIEMTNNUM3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_num3              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNNUM3;
/* Define the union U_VPSS_DIEMTNSUM3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_sum3              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNSUM3;
/* Define the union U_VPSS_DIEMTNNUM4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_num4              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNNUM4;
/* Define the union U_VPSS_DIEMTNSUM4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int glbm_sum4              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIEMTNSUM4;
/* Define the union U_VPSS_PDPHISTTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hist_thd0             : 8   ; /* [7..0]  */
        unsigned int    hist_thd1             : 8   ; /* [15..8]  */
        unsigned int    hist_thd2             : 8   ; /* [23..16]  */
        unsigned int    hist_thd3             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPHISTTHD1;

/* Define the union U_VPSS_PDPCCMOV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mov_coring_blk        : 8   ; /* [7..0]  */
        unsigned int    mov_coring_tkr        : 8   ; /* [15..8]  */
        unsigned int    mov_coring_norm       : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPCCMOV;

/* Define the union U_VPSS_PDICHD */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int ichd                   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDICHD;
/* Define the union U_VPSS_PDCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitsmov2r             : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 16  ; /* [18..3]  */
        unsigned int    lasi_mode             : 1   ; /* [19]  */
        unsigned int    edge_smooth_ratio     : 8   ; /* [27..20]  */
        unsigned int    reserved_1            : 1   ; /* [28]  */
        unsigned int    dir_mch_c             : 1   ; /* [29]  */
        unsigned int    edge_smooth_en        : 1   ; /* [30]  */
        unsigned int    dir_mch_l             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDCTRL;

/* Define the union U_VPSS_PDBLKTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    stillblk_thd          : 8   ; /* [7..0]  */
        unsigned int    diff_movblk_thd       : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDBLKTHD;

/* Define the union U_VPSS_PDHISTTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mon_tkr_thr           : 8   ; /* [7..0]  */
        unsigned int    mon_start_idx         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDHISTTHD;

/* Define the union U_VPSS_PDUMTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    um_thd0               : 8   ; /* [7..0]  */
        unsigned int    um_thd1               : 8   ; /* [15..8]  */
        unsigned int    um_thd2               : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDUMTHD;

/* Define the union U_VPSS_PDPCCCORING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    coring_tkr            : 8   ; /* [7..0]  */
        unsigned int    coring_norm           : 8   ; /* [15..8]  */
        unsigned int    coring_blk            : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPCCCORING;

/* Define the union U_VPSS_PDPCCHTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pcc_hthd              : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPCCHTHD;

/* Define the union U_VPSS_PDPCCVTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pcc_vthd0             : 8   ; /* [7..0]  */
        unsigned int    pcc_vthd1             : 8   ; /* [15..8]  */
        unsigned int    pcc_vthd2             : 8   ; /* [23..16]  */
        unsigned int    pcc_vthd3             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPCCVTHD;

/* Define the union U_VPSS_PDITDIFFVTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    itdiff_vthd0          : 8   ; /* [7..0]  */
        unsigned int    itdiff_vthd1          : 8   ; /* [15..8]  */
        unsigned int    itdiff_vthd2          : 8   ; /* [23..16]  */
        unsigned int    itdiff_vthd3          : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDITDIFFVTHD;

/* Define the union U_VPSS_PDLASITHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lasi_coring_thd       : 8   ; /* [7..0]  */
        unsigned int    lasi_edge_thd         : 8   ; /* [15..8]  */
        unsigned int    lasi_mov_thd          : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDLASITHD;

/* Define the union U_VPSS_PDLASIMODE0COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lasi_txt_alpha        : 8   ; /* [7..0]  */
        unsigned int    lasi_txt_coring       : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDLASIMODE0COEF;

/* Define the union U_VPSS_PDLASIMODE1COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lasi_txt_thd0         : 8   ; /* [7..0]  */
        unsigned int    lasi_txt_thd1         : 8   ; /* [15..8]  */
        unsigned int    lasi_txt_thd2         : 8   ; /* [23..16]  */
        unsigned int    lasi_txt_thd3         : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDLASIMODE1COEF;

/* Define the union U_VPSS_PDREGION */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    region1_y_end         : 12  ; /* [11..0]  */
        unsigned int    region1_y_stt         : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDREGION;

/* Define the union U_VPSS_PDFRMITDIFF */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int frm_it_diff            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDFRMITDIFF;
/* Define the union U_VPSS_PDSTLBLKSAD */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int stlblk_sad             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDSTLBLKSAD;
/* Define the union U_VPSS_PDHISTBIN */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hsit_bin               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDHISTBIN;
/* Define the union U_VPSS_PDUMMATCH0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int match_um               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDUMMATCH0;
/* Define the union U_VPSS_PDUMNOMATCH0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nomatch_um             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDUMNOMATCH0;
/* Define the union U_VPSS_PDUMMATCH1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int match_um               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDUMMATCH1;
/* Define the union U_VPSS_PDUMNOMATCH1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nomatch_um             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDUMNOMATCH1;
/* Define the union U_VPSS_PDPCCFFWD0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_ffwd_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFFWD0;
/* Define the union U_VPSS_PDPCCFWD0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_fwd_0              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFWD0;
/* Define the union U_VPSS_PDPCCBWD0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_bwd_0              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCBWD0;
/* Define the union U_VPSS_PDPCCCRSS0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_crss_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCCRSS0;
/* Define the union U_VPSS_PDPCCFWDTKR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_fwd_tkr_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFWDTKR0;
/* Define the union U_VPSS_PDPCCBWDTKR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_bwd_tkr_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCBWDTKR0;
/* Define the union U_VPSS_PDPCCFFWD1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_ffwd_1             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFFWD1;
/* Define the union U_VPSS_PDPCCFWD1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_fwd_1              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFWD1;
/* Define the union U_VPSS_PDPCCBWD1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_bwd_1              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCBWD1;
/* Define the union U_VPSS_PDPCCCRSS1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_crss_1             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCCRSS1;
/* Define the union U_VPSS_PDPCCFWDTKR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_fwd_tkr_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFWDTKR1;
/* Define the union U_VPSS_PDPCCBWDTKR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_bwd_tkr_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCBWDTKR1;
/* Define the union U_VPSS_PDLASICNT140 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_14_cnt_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT140;
/* Define the union U_VPSS_PDLASICNT320 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_32_cnt_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT320;
/* Define the union U_VPSS_PDLASICNT340 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_34_cnt_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT340;
/* Define the union U_VPSS_PDLASICNT141 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_14_cnt_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT141;
/* Define the union U_VPSS_PDLASICNT321 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_32_cnt_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT321;
/* Define the union U_VPSS_PDLASICNT341 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_34_cnt_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT341;
/* Define the union U_VPSS_DEBUG0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug0                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG0;
/* Define the union U_VPSS_DEBUG1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug1                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG1;
/* Define the union U_VPSS_DEBUG2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug2                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG2;
/* Define the union U_VPSS_DEBUG3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug3                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG3;
/* Define the union U_VPSS_DEBUG4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug4                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG4;
/* Define the union U_VPSS_DEBUG5 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug5                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG5;
/* Define the union U_VPSS_DEBUG6 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug6                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG6;
/* Define the union U_VPSS_DEBUG7 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug7                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG7;
/* Define the union U_VPSS_DEBUG8 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug8                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG8;
/* Define the union U_VPSS_DEBUG9 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug9                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG9;
/* Define the union U_VPSS_DEBUG10 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug10                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG10;
/* Define the union U_VPSS_DEBUG11 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug11                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG11;
/* Define the union U_VPSS_DEBUG12 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug12                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG12;
/* Define the union U_VPSS_DEBUG13 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug13                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG13;
/* Define the union U_VPSS_DEBUG14 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug14                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG14;
/* Define the union U_VPSS_DEBUG15 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug15                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG15;
/* Define the union U_VPSS_DEBUG16 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug16                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG16;
/* Define the union U_VPSS_DEBUG17 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug17                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG17;
/* Define the union U_VPSS_DEBUG18 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug18                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG18;
/* Define the union U_VPSS_DEBUG19 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug19                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG19;
/* Define the union U_VPSS_DEBUG20 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug20                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG20;
/* Define the union U_VPSS_DEBUG21 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug21                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG21;
/* Define the union U_VPSS_DEBUG22 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug22                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG22;
/* Define the union U_VPSS_DEBUG23 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug23                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG23;
/* Define the union U_VPSS_DEBUG24 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug24                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG24;
/* Define the union U_VPSS_DEBUG25 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug25                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG25;
/* Define the union U_VPSS_DEBUG26 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug26                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG26;
/* Define the union U_VPSS_DEBUG27 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug27                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG27;
/* Define the union U_VPSS_DEBUG28 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug28                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG28;
/* Define the union U_VPSS_DEBUG29 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug29                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG29;
/* Define the union U_VPSS_DEBUG30 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug30                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG30;
/* Define the union U_VPSS_DEBUG31 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug31                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG31;
/* Define the union U_VPSS_DEBUG32 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug32                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG32;
/* Define the union U_VPSS_DEBUG33 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug33                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG33;
/* Define the union U_VPSS_DEBUG34 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug34                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG34;
/* Define the union U_VPSS_DEBUG35 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug35                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG35;
/* Define the union U_VPSS_DEBUG36 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug36                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG36;
/* Define the union U_VPSS_DEBUG37 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug37                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG37;
/* Define the union U_VPSS_DEBUG38 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug38                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG38;
/* Define the union U_VPSS_DEBUG39 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug39                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG39;
/* Define the union U_VPSS_DEBUG40 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug40                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG40;
/* Define the union U_VPSS_DEBUG41 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug41                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG41;
/* Define the union U_VPSS_DEBUG42 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug42                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG42;
/* Define the union U_VPSS_DEBUG43 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug43                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG43;
/* Define the union U_STR_DET_VIDBLK0TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK0TOL0;

/* Define the union U_STR_DET_VIDBLK0TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK0TOL1;

/* Define the union U_STR_DET_VIDBLK1TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK1TOL0;

/* Define the union U_STR_DET_VIDBLK1TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK1TOL1;

/* Define the union U_STR_DET_VIDBLK2TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK2TOL0;

/* Define the union U_STR_DET_VIDBLK2TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK2TOL1;

/* Define the union U_STR_DET_VIDBLK3TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK3TOL0;

/* Define the union U_STR_DET_VIDBLK3TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK3TOL1;

/* Define the union U_STR_DET_VIDBLK4TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK4TOL0;

/* Define the union U_STR_DET_VIDBLK4TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK4TOL1;

/* Define the union U_STR_DET_VIDBLK5TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK5TOL0;

/* Define the union U_STR_DET_VIDBLK5TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK5TOL1;

/* Define the union U_VPSS_DEI_WST_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int dei_st_w_chk_sum       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEI_WST_CHKSUM;
/* Define the union U_VPSS_VHD_Y_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd_y_chk_sum          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD_Y_CHKSUM;
/* Define the union U_VPSS_VHD_C_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd_c_chk_sum          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD_C_CHKSUM;
/* Define the union U_VPSS_STR_Y_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_y_chk_sum          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_Y_CHKSUM;
/* Define the union U_VPSS_STR_C_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_c_chk_sum          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_C_CHKSUM;

//==============================================================================
// Define the global struct
typedef struct
{
    U_VPSS_CTRL            VPSS_CTRL                         ; /* 0x0 */
    U_VPSS_CTRL2           VPSS_CTRL2                        ; /* 0x4 */
    U_VPSS_IMGSIZE         VPSS_IMGSIZE                      ; /* 0x8 */
    U_VPSS_CUR_CTRL        VPSS_CUR_CTRL                     ; /* 0xc */
    unsigned int           reserved_0[2]                     ; /* 0x10~0x14 */
    U_VPSS_CURYADDR        VPSS_CURYADDR                     ; /* 0x18 */
    U_VPSS_CURCADDR        VPSS_CURCADDR                     ; /* 0x1c */
    U_VPSS_CURCRADDR       VPSS_CURCRADDR                    ; /* 0x20 */
    U_VPSS_CURSTRIDE       VPSS_CURSTRIDE                    ; /* 0x24 */
    U_VPSS_REF_CTRL        VPSS_REF_CTRL                     ; /* 0x28 */
    unsigned int           reserved_1[2]                     ; /* 0x2c~0x30 */
    U_VPSS_REFYADDR        VPSS_REFYADDR                     ; /* 0x34 */
    U_VPSS_REFCADDR        VPSS_REFCADDR                     ; /* 0x38 */
    U_VPSS_REFCRADDR       VPSS_REFCRADDR                    ; /* 0x3c */
    U_VPSS_REFSTRIDE       VPSS_REFSTRIDE                    ; /* 0x40 */
    U_VPSS_NXT1_CTRL       VPSS_NXT1_CTRL                    ; /* 0x44 */
    unsigned int           reserved_2[2]                     ; /* 0x48~0x4c */
    U_VPSS_NXT1YADDR       VPSS_NXT1YADDR                    ; /* 0x50 */
    U_VPSS_NXT1CADDR       VPSS_NXT1CADDR                    ; /* 0x54 */
    U_VPSS_NXT1CRADDR      VPSS_NXT1CRADDR                   ; /* 0x58 */
    U_VPSS_NXT1STRIDE      VPSS_NXT1STRIDE                   ; /* 0x5c */
    U_VPSS_NXT2_CTRL       VPSS_NXT2_CTRL                    ; /* 0x60 */
    unsigned int           reserved_3[2]                     ; /* 0x64~0x68 */
    U_VPSS_NXT2YADDR       VPSS_NXT2YADDR                    ; /* 0x6c */
    U_VPSS_NXT2CADDR       VPSS_NXT2CADDR                    ; /* 0x70 */
    U_VPSS_NXT2CRADDR      VPSS_NXT2CRADDR                   ; /* 0x74 */
    U_VPSS_NXT2STRIDE      VPSS_NXT2STRIDE                   ; /* 0x78 */
    unsigned int           reserved_4[7]                     ; /* 0x7c~0x94 */
    U_VPSS_STRADDR         VPSS_STRADDR                      ; /* 0x98 */
    U_VPSS_STWADDR         VPSS_STWADDR                      ; /* 0x9c */
    U_VPSS_STSTRIDE        VPSS_STSTRIDE                     ; /* 0xa0 */
    U_VPSS_VHDCTRL         VPSS_VHDCTRL                      ; /* 0xa4 */
    U_VPSS_VHDSIZE         VPSS_VHDSIZE                      ; /* 0xa8 */
    unsigned int           reserved_5[2]                     ; /* 0xac~0xb0 */
    U_VPSS_VHDYADDR        VPSS_VHDYADDR                     ; /* 0xb4 */
    U_VPSS_VHDCADDR        VPSS_VHDCADDR                     ; /* 0xb8 */
    U_VPSS_VHDSTRIDE       VPSS_VHDSTRIDE                    ; /* 0xbc */
    U_VPSS_STRCTRL         VPSS_STRCTRL                      ; /* 0xc0 */
    U_VPSS_STRSIZE         VPSS_STRSIZE                      ; /* 0xc4 */
    unsigned int           reserved_6[2]                     ; /* 0xc8~0xcc */
    U_VPSS_STRYADDR        VPSS_STRYADDR                     ; /* 0xd0 */
    U_VPSS_STRCADDR        VPSS_STRCADDR                     ; /* 0xd4 */
    U_VPSS_STRSTRIDE       VPSS_STRSTRIDE                    ; /* 0xd8 */
    unsigned int           reserved_7[8]                     ; /* 0xdc~0xf8 */
    U_VPSS_TUNL_CTRL0      VPSS_TUNL_CTRL0                   ; /* 0xfc */
    U_VPSS_TUNL_CTRL1      VPSS_TUNL_CTRL1                   ; /* 0x100 */
    U_VPSS_VHD_TUNL_ADDR   VPSS_VHD_TUNL_ADDR                ; /* 0x104 */
    U_VPSS_STR_TUNL_ADDR   VPSS_STR_TUNL_ADDR                ; /* 0x108 */
    unsigned int           reserved_8                     ; /* 0x10c */
    U_VPSS_CUR_TUNL_ADDR   VPSS_CUR_TUNL_ADDR                ; /* 0x110 */
    U_VPSS_INCROP_POS      VPSS_INCROP_POS                   ; /* 0x114 */
    U_VPSS_INCROP_SIZE     VPSS_INCROP_SIZE                  ; /* 0x118 */
    unsigned int           reserved_9[2]                     ; /* 0x11c~0x120 */
    U_VPSS_VHDCROP_POS     VPSS_VHDCROP_POS                  ; /* 0x124 */
    U_VPSS_VHDCROP_SIZE    VPSS_VHDCROP_SIZE                 ; /* 0x128 */
    U_VPSS_STRCROP_POS     VPSS_STRCROP_POS                  ; /* 0x12c */
    U_VPSS_STRCROP_SIZE    VPSS_STRCROP_SIZE                 ; /* 0x130 */
    unsigned int           reserved_10[11]                     ; /* 0x134~0x15c */
    U_VPSS_VHD_HSP         VPSS_VHD_HSP                      ; /* 0x160 */
    U_VPSS_VHD_HLOFFSET    VPSS_VHD_HLOFFSET                 ; /* 0x164 */
    U_VPSS_VHD_HCOFFSET    VPSS_VHD_HCOFFSET                 ; /* 0x168 */
    U_VPSS_VHD_VSP         VPSS_VHD_VSP                      ; /* 0x16c */
    U_VPSS_VHD_VSR         VPSS_VHD_VSR                      ; /* 0x170 */
    U_VPSS_VHD_VOFFSET     VPSS_VHD_VOFFSET                  ; /* 0x174 */
    U_VPSS_VHD_ZMEORESO    VPSS_VHD_ZMEORESO                 ; /* 0x178 */
    U_VPSS_VHD_ZMEIRESO    VPSS_VHD_ZMEIRESO                 ; /* 0x17c */
    U_VPSS_VHD_LTI_CTRL    VPSS_VHD_LTI_CTRL                 ; /* 0x180 */
    U_VPSS_VHD_LHPASS_COEF   VPSS_VHD_LHPASS_COEF            ; /* 0x184 */
    U_VPSS_VHD_LTI_THD     VPSS_VHD_LTI_THD                  ; /* 0x188 */
    U_VPSS_VHD_LHFREQ_THD   VPSS_VHD_LHFREQ_THD              ; /* 0x18c */
    U_VPSS_VHD_LGAIN_COEF   VPSS_VHD_LGAIN_COEF              ; /* 0x190 */
    U_VPSS_VHD_CTI_CTRL    VPSS_VHD_CTI_CTRL                 ; /* 0x194 */
    U_VPSS_VHD_CHPASS_COEF   VPSS_VHD_CHPASS_COEF            ; /* 0x198 */
    U_VPSS_VHD_CTI_THD     VPSS_VHD_CTI_THD                  ; /* 0x19c */
    U_VPSS_STR_HSP         VPSS_STR_HSP                      ; /* 0x1a0 */
    U_VPSS_STR_HLOFFSET    VPSS_STR_HLOFFSET                 ; /* 0x1a4 */
    U_VPSS_STR_HCOFFSET    VPSS_STR_HCOFFSET                 ; /* 0x1a8 */
    U_VPSS_STR_VSP         VPSS_STR_VSP                      ; /* 0x1ac */
    U_VPSS_STR_VOFFSET     VPSS_STR_VOFFSET                  ; /* 0x1b0 */
    U_VPSS_STR_ZMEORESO    VPSS_STR_ZMEORESO                 ; /* 0x1b4 */
    U_VPSS_DR_CFG0         VPSS_DR_CFG0                      ; /* 0x1b8 */
    U_VPSS_DR_CFG1         VPSS_DR_CFG1                      ; /* 0x1bc */
    unsigned int           reserved_11[3]                     ; /* 0x1c0~0x1c8 */
    U_VPSS_DNR_INFO        VPSS_DNR_INFO                     ; /* 0x1cc */
    unsigned int           reserved_12[9]                     ; /* 0x1d0~0x1f0 */
    U_VPSS_NODEID          VPSS_NODEID                       ; /* 0x1f4 */
    U_VPSS_AXIID           VPSS_AXIID                        ; /* 0x1f8 */
    U_VPSS_INTMASK         VPSS_INTMASK                      ; /* 0x1fc */
    unsigned int           reserved_13[4]                     ; /* 0x200~0x20c */
    U_VPSS_VHD_ZME_LHADDR   VPSS_VHD_ZME_LHADDR              ; /* 0x210 */
    U_VPSS_VHD_ZME_LVADDR   VPSS_VHD_ZME_LVADDR              ; /* 0x214 */
    U_VPSS_VHD_ZME_CHADDR   VPSS_VHD_ZME_CHADDR              ; /* 0x218 */
    U_VPSS_VHD_ZME_CVADDR   VPSS_VHD_ZME_CVADDR              ; /* 0x21c */
    U_VPSS_STR_ZME_LHADDR   VPSS_STR_ZME_LHADDR              ; /* 0x220 */
    U_VPSS_STR_ZME_LVADDR   VPSS_STR_ZME_LVADDR              ; /* 0x224 */
    U_VPSS_STR_ZME_CHADDR   VPSS_STR_ZME_CHADDR              ; /* 0x228 */
    U_VPSS_STR_ZME_CVADDR   VPSS_STR_ZME_CVADDR              ; /* 0x22c */
    U_VPSS_VC1_CTRL       VPSS_VC1_CTRL[3]                    ; /* 0x230 */
    //U_VPSS_VC1_CTRL1       VPSS_VC1_CTRL1                    ; /* 0x234 */
    //U_VPSS_VC1_CTRL2       VPSS_VC1_CTRL2                    ; /* 0x238 */
    unsigned int           reserved_14                     ; /* 0x23c */
    U_VPSS_DEI_ADDR        VPSS_DEI_ADDR                     ; /* 0x240 */
    U_VPSS_HSCL_LADDR      VPSS_HSCL_LADDR                   ; /* 0x244 */
    U_VPSS_HSCL_CADDR      VPSS_HSCL_CADDR                   ; /* 0x248 */
    unsigned int           reserved_15[13]                     ; /* 0x24c~0x27c */
    U_VPSS_VHDLBA_DFPOS    VPSS_VHDLBA_DFPOS                 ; /* 0x280 */
    U_VPSS_VHDLBA_DSIZE    VPSS_VHDLBA_DSIZE                 ; /* 0x284 */
    U_VPSS_VHDLBA_VFPOS    VPSS_VHDLBA_VFPOS                 ; /* 0x288 */
    U_VPSS_VHDLBA_VSIZE    VPSS_VHDLBA_VSIZE                 ; /* 0x28c */
    U_VPSS_VHDLBA_BK       VPSS_VHDLBA_BK                    ; /* 0x290 */
    unsigned int           reserved_16[3]                     ; /* 0x294~0x29c */
    U_VPSS_STRLBA_DFPOS    VPSS_STRLBA_DFPOS                 ; /* 0x2a0 */
    U_VPSS_STRLBA_DSIZE    VPSS_STRLBA_DSIZE                 ; /* 0x2a4 */
    U_VPSS_STRLBA_VFPOS    VPSS_STRLBA_VFPOS                 ; /* 0x2a8 */
    U_VPSS_STRLBA_VSIZE    VPSS_STRLBA_VSIZE                 ; /* 0x2ac */
    U_VPSS_STRLBA_BK       VPSS_STRLBA_BK                    ; /* 0x2b0 */
    unsigned int           reserved_17[3]                     ; /* 0x2b4~0x2bc */
    U_STR_DET_VIDCTRL      STR_DET_VIDCTRL                   ; /* 0x2c0 */
    U_STR_DET_VIDBLKPOS0_1   STR_DET_VIDBLKPOS0_1            ; /* 0x2c4 */
    U_STR_DET_VIDBLKPOS2_3   STR_DET_VIDBLKPOS2_3            ; /* 0x2c8 */
    U_STR_DET_VIDBLKPOS4_5   STR_DET_VIDBLKPOS4_5            ; /* 0x2cc */
    unsigned int           reserved_18[7]                     ; /* 0x2d0~0x2e8 */
    U_VPSS_HSCL_CTRL       VPSS_HSCL_CTRL                    ; /* 0x2ec */
    U_VPSS_HSCL_ORESO      VPSS_HSCL_ORESO                   ; /* 0x2f0 */
    U_VPSS_HSCL_HLOFF      VPSS_HSCL_HLOFF                   ; /* 0x2f4 */
    U_VPSS_HSCL_HCOFF      VPSS_HSCL_HCOFF                   ; /* 0x2f8 */
    U_VPSS_PNEXT           VPSS_PNEXT                        ; /* 0x2fc */
    U_VPSS_START           VPSS_START                        ; /* 0x300 */
    U_VPSS_INTSTATE        VPSS_INTSTATE                     ; /* 0x304 */
    U_VPSS_INTCLR          VPSS_INTCLR                       ; /* 0x308 */
    U_VPSS_RAWINT          VPSS_RAWINT                       ; /* 0x30c */
    U_VPSS_PFCNT           VPSS_PFCNT                        ; /* 0x310 */
    U_VPSS_MISCELLANEOUS   VPSS_MISCELLANEOUS                ; /* 0x314 */
    U_VPSS_MACCFG          VPSS_MACCFG                       ; /* 0x318 */
    U_VPSS_TIMEOUT         VPSS_TIMEOUT                      ; /* 0x31c */
    U_VPSS_EOFCNT          VPSS_EOFCNT                       ; /* 0x320 */
    unsigned int           reserved_19[3]                     ; /* 0x324~0x32c */
    U_VPSS_VERSION         VPSS_VERSION                      ; /* 0x330 */
    unsigned int           reserved_20[819]                     ; /* 0x334~0xffc */
    U_VPSS_DIECTRL         VPSS_DIECTRL                      ; /* 0x1000 */
    U_VPSS_DIELMA2         VPSS_DIELMA2                      ; /* 0x1004 */
    U_VPSS_DIEINTEN        VPSS_DIEINTEN                     ; /* 0x1008 */
    U_VPSS_DIESCALE        VPSS_DIESCALE                     ; /* 0x100c */
    U_VPSS_DIECHECK1       VPSS_DIECHECK1                    ; /* 0x1010 */
    U_VPSS_DIECHECK2       VPSS_DIECHECK2                    ; /* 0x1014 */
    U_VPSS_DIEDIR0_3       VPSS_DIEDIR0_3                    ; /* 0x1018 */
    U_VPSS_DIEDIR4_7       VPSS_DIEDIR4_7                    ; /* 0x101c */
    U_VPSS_DIEDIR8_11      VPSS_DIEDIR8_11                   ; /* 0x1020 */
    U_VPSS_DIEDIR12_14     VPSS_DIEDIR12_14                  ; /* 0x1024 */
    U_VPSS_DIESTA          VPSS_DIESTA                       ; /* 0x1028 */
    U_VPSS_DIESTPPREADDR   VPSS_DIESTPPREADDR                ; /* 0x102c */
    U_VPSS_DIESTPREADDR    VPSS_DIESTPREADDR                 ; /* 0x1030 */
    U_VPSS_DIEINTPSCL0     VPSS_DIEINTPSCL0                  ; /* 0x1034 */
    U_VPSS_DIEINTPSCL1     VPSS_DIEINTPSCL1                  ; /* 0x1038 */
    U_VPSS_DIEDIRTHD       VPSS_DIEDIRTHD                    ; /* 0x103c */
    U_VPSS_DIEJITMTN       VPSS_DIEJITMTN                    ; /* 0x1040 */
    U_VPSS_DIEFLDMTN       VPSS_DIEFLDMTN                    ; /* 0x1044 */
    U_VPSS_DIEMTNDIFFTHD0   VPSS_DIEMTNDIFFTHD0              ; /* 0x1048 */
    U_VPSS_DIEMTNDIFFTHD1   VPSS_DIEMTNDIFFTHD1              ; /* 0x104c */
    U_VPSS_DIEMTNIIRSLP    VPSS_DIEMTNIIRSLP                 ; /* 0x1050 */
    U_VPSS_DIEMTNIIRRAT    VPSS_DIEMTNIIRRAT                 ; /* 0x1054 */
    U_VPSS_DIEHISMODE      VPSS_DIEHISMODE                   ; /* 0x1058 */
    U_VPSS_DIEMORFLT       VPSS_DIEMORFLT                    ; /* 0x105c */
    U_VPSS_DIECOMBCHK0     VPSS_DIECOMBCHK0                  ; /* 0x1060 */
    U_VPSS_DIECOMBCHK1     VPSS_DIECOMBCHK1                  ; /* 0x1064 */
    U_VPSS_DIEFRMMTNSMTHTHD0   VPSS_DIEFRMMTNSMTHTHD0        ; /* 0x1068 */
    U_VPSS_DIEFRMMTNSMTHTHD1   VPSS_DIEFRMMTNSMTHTHD1        ; /* 0x106c */
    U_VPSS_DIEFRMMTNSMTHSLP   VPSS_DIEFRMMTNSMTHSLP          ; /* 0x1070 */
    U_VPSS_DIEFRMMTNSMTHRAT   VPSS_DIEFRMMTNSMTHRAT          ; /* 0x1074 */
    U_VPSS_DIEFRMFLDBLENDTHD0   VPSS_DIEFRMFLDBLENDTHD0      ; /* 0x1078 */
    U_VPSS_DIEFRMFLDBLENDTHD1   VPSS_DIEFRMFLDBLENDTHD1      ; /* 0x107c */
    U_VPSS_DIEFRMFLDBLENDSLP   VPSS_DIEFRMFLDBLENDSLP        ; /* 0x1080 */
    U_VPSS_DIEFRMFLDBLENDRAT   VPSS_DIEFRMFLDBLENDRAT        ; /* 0x1084 */
    U_VPSS_DIEMTNADJ       VPSS_DIEMTNADJ                    ; /* 0x1088 */
    U_VPSS_DIEGLBMTNTHD0   VPSS_DIEGLBMTNTHD0                ; /* 0x108c */
    U_VPSS_DIEGLBMTNTHD1   VPSS_DIEGLBMTNTHD1                ; /* 0x1090 */
    U_VPSS_CCRSCLR0        VPSS_CCRSCLR0                     ; /* 0x1094 */
    U_VPSS_CCRSCLR1        VPSS_CCRSCLR1                     ; /* 0x1098 */
    U_VPSS_DIEGLBMTNBIN    VPSS_DIEGLBMTNBIN[32]             ; /* 0x109c~0x1118 */
    U_VPSS_DIEMTNNUM0      VPSS_DIEMTNNUM0                   ; /* 0x111c */
    U_VPSS_DIEMTNSUM0      VPSS_DIEMTNSUM0                   ; /* 0x1120 */
    U_VPSS_DIEMTNNUM1      VPSS_DIEMTNNUM1                   ; /* 0x1124 */
    U_VPSS_DIEMTNSUM1      VPSS_DIEMTNSUM1                   ; /* 0x1128 */
    U_VPSS_DIEMTNNUM2      VPSS_DIEMTNNUM2                   ; /* 0x112c */
    U_VPSS_DIEMTNSUM2      VPSS_DIEMTNSUM2                   ; /* 0x1130 */
    U_VPSS_DIEMTNNUM3      VPSS_DIEMTNNUM3                   ; /* 0x1134 */
    U_VPSS_DIEMTNSUM3      VPSS_DIEMTNSUM3                   ; /* 0x1138 */
    U_VPSS_DIEMTNNUM4      VPSS_DIEMTNNUM4                   ; /* 0x113c */
    U_VPSS_DIEMTNSUM4      VPSS_DIEMTNSUM4                   ; /* 0x1140 */
    U_VPSS_PDPHISTTHD1     VPSS_PDPHISTTHD1                  ; /* 0x1144 */
    U_VPSS_PDPCCMOV        VPSS_PDPCCMOV                     ; /* 0x1148 */
    U_VPSS_PDICHD          VPSS_PDICHD                       ; /* 0x114c */
    U_VPSS_PDCTRL          VPSS_PDCTRL                       ; /* 0x1150 */
    U_VPSS_PDBLKTHD        VPSS_PDBLKTHD                     ; /* 0x1154 */
    U_VPSS_PDHISTTHD       VPSS_PDHISTTHD                    ; /* 0x1158 */
    U_VPSS_PDUMTHD         VPSS_PDUMTHD                      ; /* 0x115c */
    U_VPSS_PDPCCCORING     VPSS_PDPCCCORING                  ; /* 0x1160 */
    U_VPSS_PDPCCHTHD       VPSS_PDPCCHTHD                    ; /* 0x1164 */
    U_VPSS_PDPCCVTHD       VPSS_PDPCCVTHD                    ; /* 0x1168 */
    U_VPSS_PDITDIFFVTHD    VPSS_PDITDIFFVTHD                 ; /* 0x116c */
    U_VPSS_PDLASITHD       VPSS_PDLASITHD                    ; /* 0x1170 */
    U_VPSS_PDLASIMODE0COEF   VPSS_PDLASIMODE0COEF            ; /* 0x1174 */
    U_VPSS_PDLASIMODE1COEF   VPSS_PDLASIMODE1COEF            ; /* 0x1178 */
    U_VPSS_PDREGION        VPSS_PDREGION                     ; /* 0x117c */
    U_VPSS_PDFRMITDIFF     VPSS_PDFRMITDIFF                  ; /* 0x1180 */
    U_VPSS_PDSTLBLKSAD     VPSS_PDSTLBLKSAD[16]              ; /* 0x1184~0x11c0 */
    U_VPSS_PDHISTBIN       VPSS_PDHISTBIN[4]                 ; /* 0x11c4~0x11d0 */
    U_VPSS_PDUMMATCH0      VPSS_PDUMMATCH0                   ; /* 0x11d4 */
    U_VPSS_PDUMNOMATCH0    VPSS_PDUMNOMATCH0                 ; /* 0x11d8 */
    U_VPSS_PDUMMATCH1      VPSS_PDUMMATCH1                   ; /* 0x11dc */
    U_VPSS_PDUMNOMATCH1    VPSS_PDUMNOMATCH1                 ; /* 0x11e0 */
    U_VPSS_PDPCCFFWD0      VPSS_PDPCCFFWD0                   ; /* 0x11e4 */
    U_VPSS_PDPCCFWD0       VPSS_PDPCCFWD0                    ; /* 0x11e8 */
    U_VPSS_PDPCCBWD0       VPSS_PDPCCBWD0                    ; /* 0x11ec */
    U_VPSS_PDPCCCRSS0      VPSS_PDPCCCRSS0                   ; /* 0x11f0 */
    U_VPSS_PDPCCFWDTKR0    VPSS_PDPCCFWDTKR0                 ; /* 0x11f4 */
    U_VPSS_PDPCCBWDTKR0    VPSS_PDPCCBWDTKR0                 ; /* 0x11f8 */
    U_VPSS_PDPCCFFWD1      VPSS_PDPCCFFWD1                   ; /* 0x11fc */
    U_VPSS_PDPCCFWD1       VPSS_PDPCCFWD1                    ; /* 0x1200 */
    U_VPSS_PDPCCBWD1       VPSS_PDPCCBWD1                    ; /* 0x1204 */
    U_VPSS_PDPCCCRSS1      VPSS_PDPCCCRSS1                   ; /* 0x1208 */
    U_VPSS_PDPCCFWDTKR1    VPSS_PDPCCFWDTKR1                 ; /* 0x120c */
    U_VPSS_PDPCCBWDTKR1    VPSS_PDPCCBWDTKR1                 ; /* 0x1210 */
    U_VPSS_PDLASICNT140    VPSS_PDLASICNT140                 ; /* 0x1214 */
    U_VPSS_PDLASICNT320    VPSS_PDLASICNT320                 ; /* 0x1218 */
    U_VPSS_PDLASICNT340    VPSS_PDLASICNT340                 ; /* 0x121c */
    U_VPSS_PDLASICNT141    VPSS_PDLASICNT141                 ; /* 0x1220 */
    U_VPSS_PDLASICNT321    VPSS_PDLASICNT321                 ; /* 0x1224 */
    U_VPSS_PDLASICNT341    VPSS_PDLASICNT341                 ; /* 0x1228 */
    unsigned int           reserved_21[2933]                     ; /* 0x122c~0x3ffc */
    U_VPSS_DEBUG0          VPSS_DEBUG0                       ; /* 0x4000 */
    U_VPSS_DEBUG1          VPSS_DEBUG1                       ; /* 0x4004 */
    U_VPSS_DEBUG2          VPSS_DEBUG2                       ; /* 0x4008 */
    U_VPSS_DEBUG3          VPSS_DEBUG3                       ; /* 0x400c */
    U_VPSS_DEBUG4          VPSS_DEBUG4                       ; /* 0x4010 */
    U_VPSS_DEBUG5          VPSS_DEBUG5                       ; /* 0x4014 */
    U_VPSS_DEBUG6          VPSS_DEBUG6                       ; /* 0x4018 */
    U_VPSS_DEBUG7          VPSS_DEBUG7                       ; /* 0x401c */
    U_VPSS_DEBUG8          VPSS_DEBUG8                       ; /* 0x4020 */
    U_VPSS_DEBUG9          VPSS_DEBUG9                       ; /* 0x4024 */
    U_VPSS_DEBUG10         VPSS_DEBUG10                      ; /* 0x4028 */
    U_VPSS_DEBUG11         VPSS_DEBUG11                      ; /* 0x402c */
    U_VPSS_DEBUG12         VPSS_DEBUG12                      ; /* 0x4030 */
    U_VPSS_DEBUG13         VPSS_DEBUG13                      ; /* 0x4034 */
    U_VPSS_DEBUG14         VPSS_DEBUG14                      ; /* 0x4038 */
    U_VPSS_DEBUG15         VPSS_DEBUG15                      ; /* 0x403c */
    U_VPSS_DEBUG16         VPSS_DEBUG16                      ; /* 0x4040 */
    U_VPSS_DEBUG17         VPSS_DEBUG17                      ; /* 0x4044 */
    U_VPSS_DEBUG18         VPSS_DEBUG18                      ; /* 0x4048 */
    U_VPSS_DEBUG19         VPSS_DEBUG19                      ; /* 0x404c */
    U_VPSS_DEBUG20         VPSS_DEBUG20                      ; /* 0x4050 */
    U_VPSS_DEBUG21         VPSS_DEBUG21                      ; /* 0x4054 */
    U_VPSS_DEBUG22         VPSS_DEBUG22                      ; /* 0x4058 */
    U_VPSS_DEBUG23         VPSS_DEBUG23                      ; /* 0x405c */
    U_VPSS_DEBUG24         VPSS_DEBUG24                      ; /* 0x4060 */
    U_VPSS_DEBUG25         VPSS_DEBUG25                      ; /* 0x4064 */
    U_VPSS_DEBUG26         VPSS_DEBUG26                      ; /* 0x4068 */
    U_VPSS_DEBUG27         VPSS_DEBUG27                      ; /* 0x406c */
    U_VPSS_DEBUG28         VPSS_DEBUG28                      ; /* 0x4070 */
    U_VPSS_DEBUG29         VPSS_DEBUG29                      ; /* 0x4074 */
    U_VPSS_DEBUG30         VPSS_DEBUG30                      ; /* 0x4078 */
    U_VPSS_DEBUG31         VPSS_DEBUG31                      ; /* 0x407c */
    U_VPSS_DEBUG32         VPSS_DEBUG32                      ; /* 0x4080 */
    U_VPSS_DEBUG33         VPSS_DEBUG33                      ; /* 0x4084 */
    U_VPSS_DEBUG34         VPSS_DEBUG34                      ; /* 0x4088 */
    U_VPSS_DEBUG35         VPSS_DEBUG35                      ; /* 0x408c */
    U_VPSS_DEBUG36         VPSS_DEBUG36                      ; /* 0x4090 */
    U_VPSS_DEBUG37         VPSS_DEBUG37                      ; /* 0x4094 */
    U_VPSS_DEBUG38         VPSS_DEBUG38                      ; /* 0x4098 */
    U_VPSS_DEBUG39         VPSS_DEBUG39                      ; /* 0x409c */
    U_VPSS_DEBUG40         VPSS_DEBUG40                      ; /* 0x40a0 */
    U_VPSS_DEBUG41         VPSS_DEBUG41                      ; /* 0x40a4 */
    U_VPSS_DEBUG42         VPSS_DEBUG42                      ; /* 0x40a8 */
    U_VPSS_DEBUG43         VPSS_DEBUG43                      ; /* 0x40ac */
    unsigned int           reserved_22[980]                     ; /* 0x40b0~0x4ffc */
    U_STR_DET_VIDBLK0TOL0   STR_DET_VIDBLK0TOL0              ; /* 0x5000 */
    U_STR_DET_VIDBLK0TOL1   STR_DET_VIDBLK0TOL1              ; /* 0x5004 */
    U_STR_DET_VIDBLK1TOL0   STR_DET_VIDBLK1TOL0              ; /* 0x5008 */
    U_STR_DET_VIDBLK1TOL1   STR_DET_VIDBLK1TOL1              ; /* 0x500c */
    U_STR_DET_VIDBLK2TOL0   STR_DET_VIDBLK2TOL0              ; /* 0x5010 */
    U_STR_DET_VIDBLK2TOL1   STR_DET_VIDBLK2TOL1              ; /* 0x5014 */
    U_STR_DET_VIDBLK3TOL0   STR_DET_VIDBLK3TOL0              ; /* 0x5018 */
    U_STR_DET_VIDBLK3TOL1   STR_DET_VIDBLK3TOL1              ; /* 0x501c */
    U_STR_DET_VIDBLK4TOL0   STR_DET_VIDBLK4TOL0              ; /* 0x5020 */
    U_STR_DET_VIDBLK4TOL1   STR_DET_VIDBLK4TOL1              ; /* 0x5024 */
    U_STR_DET_VIDBLK5TOL0   STR_DET_VIDBLK5TOL0              ; /* 0x5028 */
    U_STR_DET_VIDBLK5TOL1   STR_DET_VIDBLK5TOL1              ; /* 0x502c */
    unsigned int           reserved_23[4]                     ; /* 0x5030~0x503c */
    U_VPSS_DEI_WST_CHKSUM   VPSS_DEI_WST_CHKSUM              ; /* 0x5040 */
    unsigned int           reserved_24[2]                     ; /* 0x5044~0x5048 */
    U_VPSS_VHD_Y_CHKSUM    VPSS_VHD_Y_CHKSUM                 ; /* 0x504c */
    U_VPSS_VHD_C_CHKSUM    VPSS_VHD_C_CHKSUM                 ; /* 0x5050 */
    U_VPSS_STR_Y_CHKSUM    VPSS_STR_Y_CHKSUM                 ; /* 0x5054 */
    U_VPSS_STR_C_CHKSUM    VPSS_STR_C_CHKSUM                 ; /* 0x5058 */

} VPSS_REG_S, S_CAS_REGS_TYPE;

typedef struct
{
} S_VPSSWB_REGS_TYPE;


#endif
