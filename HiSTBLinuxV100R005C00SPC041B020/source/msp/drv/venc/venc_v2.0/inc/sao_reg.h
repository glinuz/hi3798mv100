//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_sao.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module sao
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_SAO_H__
#define __C_UNION_DEFINE_SAO_H__

/* Define the union U_SAO_FSM_DFX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao2vcpi_fsm_dfx      : 8   ; /* [7..0]  */
        unsigned int    Reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_FSM_DFX;

/* Define the union U_SAO_LCU_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao2vcpi_lcu_cnt      : 16  ; /* [15..0]  */
        unsigned int    Reserved_2            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_LCU_CNT;

/* Define the union U_SAO_OFF_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao2vcpi_saooff_num_luma  : 14  ; /* [13..0]  */
        unsigned int    Reserved_4            : 2   ; /* [15..14]  */
        unsigned int    sao2vcpi_saooff_num_chroma  : 14  ; /* [29..16]  */
        unsigned int    Reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_OFF_NUM;

/* Define the union U_SAO_MSE_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao2vcpi_mse_cnt      : 12  ; /* [11..0]  */
        unsigned int    Reserved_5            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_MSE_CNT;

/* Define the union U_SAO_MSE_MAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao2vcpi_mse_max      : 28  ; /* [27..0]  */
        unsigned int    Reserved_6            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_MSE_MAX;

/* Define the union U_SAO_SSD_AREA0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area0_end_lcuy    : 8   ; /* [7..0]  */
        unsigned int    sao_area0_end_lcux    : 8   ; /* [15..8]  */
        unsigned int    sao_area0_start_lcuy  : 8   ; /* [23..16]  */
        unsigned int    sao_area0_start_lcux  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_SSD_AREA0;

/* Define the union U_SAO_SSD_AREA1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area1_end_lcuy    : 8   ; /* [7..0]  */
        unsigned int    sao_area1_end_lcux    : 8   ; /* [15..8]  */
        unsigned int    sao_area1_start_lcuy  : 8   ; /* [23..16]  */
        unsigned int    sao_area1_start_lcux  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_SSD_AREA1;

/* Define the union U_SAO_SSD_AREA2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area2_end_lcuy    : 8   ; /* [7..0]  */
        unsigned int    sao_area2_end_lcux    : 8   ; /* [15..8]  */
        unsigned int    sao_area2_start_lcuy  : 8   ; /* [23..16]  */
        unsigned int    sao_area2_start_lcux  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_SSD_AREA2;

/* Define the union U_SAO_SSD_AREA3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area3_end_lcuy    : 8   ; /* [7..0]  */
        unsigned int    sao_area3_end_lcux    : 8   ; /* [15..8]  */
        unsigned int    sao_area3_start_lcuy  : 8   ; /* [23..16]  */
        unsigned int    sao_area3_start_lcux  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_SSD_AREA3;

/* Define the union U_SAO_SSD_AREA4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area4_end_lcuy    : 8   ; /* [7..0]  */
        unsigned int    sao_area4_end_lcux    : 8   ; /* [15..8]  */
        unsigned int    sao_area4_start_lcuy  : 8   ; /* [23..16]  */
        unsigned int    sao_area4_start_lcux  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_SSD_AREA4;

/* Define the union U_SAO_SSD_AREA5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area5_end_lcuy    : 8   ; /* [7..0]  */
        unsigned int    sao_area5_end_lcux    : 8   ; /* [15..8]  */
        unsigned int    sao_area5_start_lcuy  : 8   ; /* [23..16]  */
        unsigned int    sao_area5_start_lcux  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_SSD_AREA5;

/* Define the union U_SAO_SSD_AREA6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area6_end_lcuy    : 8   ; /* [7..0]  */
        unsigned int    sao_area6_end_lcux    : 8   ; /* [15..8]  */
        unsigned int    sao_area6_start_lcuy  : 8   ; /* [23..16]  */
        unsigned int    sao_area6_start_lcux  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_SSD_AREA6;

/* Define the union U_SAO_SSD_AREA7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area7_end_lcuy    : 8   ; /* [7..0]  */
        unsigned int    sao_area7_end_lcux    : 8   ; /* [15..8]  */
        unsigned int    sao_area7_start_lcuy  : 8   ; /* [23..16]  */
        unsigned int    sao_area7_start_lcux  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SAO_SSD_AREA7;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED0[29];
    volatile U_SAO_FSM_DFX          SAO_FSM_DFX;
    volatile U_SAO_LCU_CNT          SAO_LCU_CNT;
    volatile U_SAO_OFF_NUM          SAO_OFF_NUM;
    volatile unsigned int           SAO_MSE_SUM;
    volatile U_SAO_MSE_CNT          SAO_MSE_CNT;
    volatile U_SAO_MSE_MAX          SAO_MSE_MAX;
    volatile unsigned int           SAO_SSD_AREA0_SUM;
    volatile unsigned int           SAO_SSD_AREA1_SUM;
    volatile unsigned int           SAO_SSD_AREA2_SUM;
    volatile unsigned int           SAO_SSD_AREA3_SUM;
    volatile unsigned int           SAO_SSD_AREA4_SUM;
    volatile unsigned int           SAO_SSD_AREA5_SUM;
    volatile unsigned int           SAO_SSD_AREA6_SUM;
    volatile unsigned int           SAO_SSD_AREA7_SUM;
    volatile U_SAO_SSD_AREA0        SAO_SSD_AREA0;
    volatile U_SAO_SSD_AREA1        SAO_SSD_AREA1;
    volatile U_SAO_SSD_AREA2        SAO_SSD_AREA2;
    volatile U_SAO_SSD_AREA3        SAO_SSD_AREA3;
    volatile U_SAO_SSD_AREA4        SAO_SSD_AREA4;
    volatile U_SAO_SSD_AREA5        SAO_SSD_AREA5;
    volatile U_SAO_SSD_AREA6        SAO_SSD_AREA6;
    volatile U_SAO_SSD_AREA7        SAO_SSD_AREA7;
    volatile unsigned int           RESERVED1[77];


} S_sao_REGS_TYPE;

/* Declare the struct pointor of the module sao */
extern volatile S_sao_REGS_TYPE *gopsaoAllReg;

/* Declare the functions that set the member value */
int iSetSAO_SSD_AREA0sao_area0_end_lcuy(unsigned int usao_area0_end_lcuy);
int iSetSAO_SSD_AREA0sao_area0_end_lcux(unsigned int usao_area0_end_lcux);
int iSetSAO_SSD_AREA0sao_area0_start_lcuy(unsigned int usao_area0_start_lcuy);
int iSetSAO_SSD_AREA0sao_area0_start_lcux(unsigned int usao_area0_start_lcux);
int iSetSAO_SSD_AREA1sao_area1_end_lcuy(unsigned int usao_area1_end_lcuy);
int iSetSAO_SSD_AREA1sao_area1_end_lcux(unsigned int usao_area1_end_lcux);
int iSetSAO_SSD_AREA1sao_area1_start_lcuy(unsigned int usao_area1_start_lcuy);
int iSetSAO_SSD_AREA1sao_area1_start_lcux(unsigned int usao_area1_start_lcux);
int iSetSAO_SSD_AREA2sao_area2_end_lcuy(unsigned int usao_area2_end_lcuy);
int iSetSAO_SSD_AREA2sao_area2_end_lcux(unsigned int usao_area2_end_lcux);
int iSetSAO_SSD_AREA2sao_area2_start_lcuy(unsigned int usao_area2_start_lcuy);
int iSetSAO_SSD_AREA2sao_area2_start_lcux(unsigned int usao_area2_start_lcux);
int iSetSAO_SSD_AREA3sao_area3_end_lcuy(unsigned int usao_area3_end_lcuy);
int iSetSAO_SSD_AREA3sao_area3_end_lcux(unsigned int usao_area3_end_lcux);
int iSetSAO_SSD_AREA3sao_area3_start_lcuy(unsigned int usao_area3_start_lcuy);
int iSetSAO_SSD_AREA3sao_area3_start_lcux(unsigned int usao_area3_start_lcux);
int iSetSAO_SSD_AREA4sao_area4_end_lcuy(unsigned int usao_area4_end_lcuy);
int iSetSAO_SSD_AREA4sao_area4_end_lcux(unsigned int usao_area4_end_lcux);
int iSetSAO_SSD_AREA4sao_area4_start_lcuy(unsigned int usao_area4_start_lcuy);
int iSetSAO_SSD_AREA4sao_area4_start_lcux(unsigned int usao_area4_start_lcux);
int iSetSAO_SSD_AREA5sao_area5_end_lcuy(unsigned int usao_area5_end_lcuy);
int iSetSAO_SSD_AREA5sao_area5_end_lcux(unsigned int usao_area5_end_lcux);
int iSetSAO_SSD_AREA5sao_area5_start_lcuy(unsigned int usao_area5_start_lcuy);
int iSetSAO_SSD_AREA5sao_area5_start_lcux(unsigned int usao_area5_start_lcux);
int iSetSAO_SSD_AREA6sao_area6_end_lcuy(unsigned int usao_area6_end_lcuy);
int iSetSAO_SSD_AREA6sao_area6_end_lcux(unsigned int usao_area6_end_lcux);
int iSetSAO_SSD_AREA6sao_area6_start_lcuy(unsigned int usao_area6_start_lcuy);
int iSetSAO_SSD_AREA6sao_area6_start_lcux(unsigned int usao_area6_start_lcux);
int iSetSAO_SSD_AREA7sao_area7_end_lcuy(unsigned int usao_area7_end_lcuy);
int iSetSAO_SSD_AREA7sao_area7_end_lcux(unsigned int usao_area7_end_lcux);
int iSetSAO_SSD_AREA7sao_area7_start_lcuy(unsigned int usao_area7_start_lcuy);
int iSetSAO_SSD_AREA7sao_area7_start_lcux(unsigned int usao_area7_start_lcux);



#endif /* __C_UNION_DEFINE_SAO_H__ */
