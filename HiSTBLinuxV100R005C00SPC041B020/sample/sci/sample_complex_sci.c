#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "hi_unf_sci.h"

HI_U8 rcv_buf[512];

#define TestNegotiation 0
#define TestSetETU 0
#define TestOther 0
#define T0_transfer

#if 1
//lint -save -e785
//lint -save -e14

/******************t0_irdeto_zeta*************************************/

HI_U8 t0_irdeto_zeta_send[][22] = {
    {0xd2, 0x42, 0x00, 0x00, 0x01, 0x1d},
    {0xd2, 0xfe, 0x00, 0x00,0x19},
    {0xd2, 0x40, 0x00, 0x00, 0x11,
     0x03, 0x06, 0x14, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x1d},
    {0xd2, 0xfe, 0x00, 0x00,0x09},
    {0xd2, 0x3c, 0x00, 0x00, 0x01, 0x22},
    {0xd2, 0xfe, 0x00, 0x00,0x15},
    {0xd2, 0x3e, 0x00, 0x00, 0x01, 0x23},
    {0xd2, 0xfe, 0x00, 0x00,0x0b},
    {0xd2, 0x00, 0x00, 0x00, 0x01, 0x3c},
    {0xd2, 0xfe, 0x00, 0x00,0x1d},
};
HI_U8 t0_irdeto_zeta_rcv[][32] = {
    {0x90, 0x19},
    {0x01, 0x02, 0x00, 0x00,
     0x21, 0x00, 0x00, 0x10, 0x03,
     0x06, 0x14, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x1c, 0x90, 0x00},
    {0x90, 0x09},
    {0x01, 0x02, 0x00, 0x00,
     0x20, 0x00, 0x00, 0x00, 0x1c,
     0x90, 0x00},
    {0x90, 0x15},
    {0x01, 0x02, 0x00, 0x00,
     0x1e, 0x00, 0x00, 0x0c, 0x53,
     0x36, 0x01, 0x0e, 0x00, 0x08,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x4c, 0x90, 0x00},
    {0x90, 0x0b},
    {0x01, 0x02, 0x00, 0x00,
     0x1f, 0x00, 0x00, 0x02, 0x00,
     0x02, 0x23, 0x90, 0x00},
    {0x90, 0x1d},
    {0x01, 0x02, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x14, 0x34,
     0x30, 0x32, 0x30, 0x33, 0x32,
     0x35, 0x31, 0x35, 0x32, 0x54,
     0x36, 0x33, 0x38, 0x35, 0x30,
     0x30, 0x30, 0x41, 0x00, 0x01,
     0x90, 0x00},
};


HI_U32 t0_irdeto_zeta_send_len[10] = {6, 5, 22, 5, 6, 5, 6, 5, 6, 5,};
HI_U32 t0_irdeto_zeta_rcv_len[10] = { 2, 27, 2, 11, 2, 23, 2, 13, 2, 31};

#endif

#if 0

/******************t0_irdeto_ICE*************************************/

HI_U8 t0_irdeto_zeta_send[][17] = {
    {0xd2, 0x42, 0x00, 0x00, 0x01, 0x1d},
    {0xd2, 0xfe, 0x00, 0x00,0x19},
    {0xd2, 0x3c, 0x00, 0x00, 0x01, 0x22},
    {0xd2, 0xfe, 0x00, 0x00,0x15},
};

HI_U8 t0_irdeto_zeta_rcv[][32] = {
    { 0x90, 0x19},
    { 0x01, 0x02, 0x00, 0x00,
      0x21, 0x00, 0x00, 0x10, 0x03,
      0x06, 0x14, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
      0x1c, 0x90, 0x00},
    { 0x90, 0x15},
    { 0x01, 0x02, 0x00, 0x00,
      0x1e, 0x00, 0x00, 0x0c, 0x57,
      0x12, 0x01, 0x93, 0x00, 0x0d,
      0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0xf4, 0x90, 0x00},
};

HI_U32 t0_irdeto_zeta_send_len[4] = { 6, 5, 6, 5};
HI_U32 t0_irdeto_zeta_rcv_len[4] = { 2, 27, 2, 23};

#endif

#if 0

/**************************T1 suantong******************************************/
HI_U8 atr_suantong_t1[18] = {0x3b, 0xe9, 0x00, 0x00, 0x81,
                            0x31, 0xc3, 0x45, 0x99, 0x63,
                            0x74, 0x69, 0x19, 0x99, 0x12,
                            0x56, 0x10, 0xec};
HI_U32 atr_suantong_t1_cnt = 18;

HI_U8 t1_suantong_send[][18] = {
    {0x00, 0x00, 0x05, 0x00, 0x25, 0xa0, 0x21, 0x20, 0x81},
    {0x00, 0x00, 0x05, 0x80, 0xb5, 0x30, 0x01, 0x20, 0x21},
    {0x00, 0x00, 0x05, 0x81, 0xdd, 0x00, 0x10, 0x04, 0x4d},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x05, 0x54},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x0b, 0x5a},
    {0x00, 0x00, 0x05, 0x81, 0xd0, 0x00, 0x01, 0x08, 0x5d},
    {0x00, 0x00, 0x05, 0x81, 0xc0, 0x00, 0x01, 0x0a, 0x4f},
    {0x00, 0x00, 0x05, 0x00, 0x31, 0x05, 0x00, 0x08, 0x39},
    {0x00, 0x00, 0x0a, 0x33, 0x2e, 0x30, 0x34, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x83},
    {0x00, 0x00, 0x05, 0x81, 0xd1, 0x00, 0x01, 0x10, 0x44},
    {0x00, 0x00, 0x05, 0x81, 0xd2, 0x00, 0x01, 0x10, 0x47},
    {0x00, 0x00, 0x05, 0x81, 0xa3, 0x00, 0x00, 0x05, 0x22},
    {0x00, 0x00, 0x05, 0x81, 0xa3, 0x00, 0x01, 0x05, 0x23},
    {0x00, 0x00, 0x05, 0x00, 0x25, 0x00, 0x02, 0x01, 0x23},
    {0x00, 0x00, 0x0e, 0x02, 0x68, 0x00, 0x00, 0x09, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x02, 0x50, 0x03, 0xbf},
};
HI_U8 t1_suantong_rcv[][39] = {
    {0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x01, 0x90, 0x00, 0x9f},
    {0x00, 0x00, 0x23, 0xeb, 0x70, 0x11, 0x27, 0xc7, 0x72, 0xfc,
     0x84, 0xa4, 0x53, 0x39, 0x66, 0xe3, 0xc5, 0x5d, 0xa2, 0x34,
     0x27, 0x2b, 0xac, 0x1b, 0xf6, 0xf6, 0x5f, 0x49, 0xa7, 0x51,
     0x5e, 0x08, 0x00, 0xe5, 0xba, 0x0c, 0x90, 0x00, 0xc8},
    {0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x01, 0x90, 0x00,0x96},
    {0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x90, 0x00,
     0x96},
    {0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x01, 0x86, 0xf1, 0x90, 0x00, 0xeb},
    {0x00, 0x00, 0x0a, 0x28, 0x4b, 0x3c, 0x2a, 0x04, 0x99, 0x71,
     0xcf, 0x90, 0x00, 0xcc},
    {0x00, 0x00, 0x04, 0x30, 0x00, 0x90, 0x00,0xa4},
    {0x00, 0x00, 0x0a, 0x33, 0x2e, 0x30, 0x34, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x83},
    {0x00, 0x00, 0x12, 0x00, 0x1c, 0xdc, 0x33, 0x3e, 0x9f, 0xc8,
     0x0a, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x90,
     0x00, 0x12},
    {0x00, 0x00, 0x12, 0x50, 0x61, 0x79, 0x5f, 0x54, 0x56, 0x5f,
     0x43, 0x61, 0x72, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90,
     0x00, 0xfc},
    {0x00, 0x00, 0x07, 0x1c, 0x21, 0x0e, 0x15, 0x00, 0x90, 0x00,
     0xb1},
    {0x00, 0x00, 0x07, 0x2a, 0x87, 0x0e, 0x15, 0x00, 0x90, 0x00,
     0x21},
    {0x00, 0x00, 0x08, 0x00, 0x25, 0x00, 0x02, 0x01, 0x09, 0x90,
     0x00, 0xb7},
    {0x00, 0x00, 0x20, 0x50, 0x09, 0x1b, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x10, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x90, 0x00, 0xc2},
};
HI_U32 t1_suantong_send_len[15] = {9, 9, 9, 9, 9, 9, 9, 9, 14, 9, 9, 9, 9, 9, 18};
HI_U32 t1_suantong_rcv_len[15] = {18, 39, 10, 11, 17, 14, 8, 14, 22, 22, 11, 11, 12, 36};

#else

/***************************T1 suantong*******************************************/
HI_U8 atr_suantong_t1[18] = {0x3b, 0xe9, 0x00, 0x00, 0x81,
                            0x31, 0xc3, 0x45, 0x99, 0x63,
                            0x74, 0x69, 0x19, 0x99, 0x12,
                            0x56, 0x10, 0xec};
HI_U32 atr_suantong_t1_cnt = 18;
HI_U8 t1_suantong_send[][23] = { //[][6]
    {0x00, 0x00, 0x06, 0x00, 0x32, 0x10, 0x01, 0x01, 0x01, 0x25},

    //{0x00, 0x00, 0x05, 0x00, 0x84, 0x00, 0x00, 0x10, 0x91},
    {0x00, 0x00, 0x05, 0x00, 0x25, 0xa0, 0x21, 0x20,0x81},
    {0x00, 0x00, 0x05, 0x81, 0xdd, 0x00, 0x10, 0x04,0x4d},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x05,0x54},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x0b,0x5a},
    {0x00, 0x00, 0x05, 0x81, 0xd0, 0x00, 0x01, 0x08,0x5d},
    {0x00, 0x00, 0x05, 0x81, 0xc0, 0x00, 0x01, 0x0a,0x4f},
    {0x00, 0x00, 0x05, 0x00, 0x31, 0x05, 0x00, 0x08,0x39},
    {0x00, 0x00, 0x05, 0x81, 0xd1, 0x00, 0x01, 0x10,0x44},
    {0x00, 0x00, 0x05, 0x81, 0xd2, 0x00, 0x01, 0x10,0x47},
    {0x00, 0x00, 0x05, 0x81, 0xa3, 0x00, 0x00, 0x05,0x22},
    {0x00, 0x00, 0x05, 0x81, 0xa3, 0x00, 0x01, 0x05,0x23},
    {0x00, 0x00, 0x05, 0x00, 0x25, 0x00, 0x02, 0x01,0x23},
    {0x00, 0x00, 0x0e, 0x02, 0x68, 0x00, 0x00, 0x09, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x02, 0x50, 0x03, 0xbf},
    {0x00, 0x00, 0x13, 0x02, 0x68, 0x00, 0x00, 0x0e, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x07, 0x90, 0x03, 0x00, 0x00, 0x10,
     0xa7, 0x10, 0xc7},
    {0x00, 0x00, 0x13, 0x02, 0x68, 0x00, 0x00, 0x0e, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x07, 0x90, 0x03, 0x00, 0x00, 0x20,
     0xa7, 0x10, 0xf7},
    {0x00, 0x00, 0x13, 0x02, 0x68, 0x00, 0x00, 0x0e, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x07, 0x90, 0x03, 0x00, 0x00, 0x30,
     0xa7, 0x10, 0xe7},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x0b,0x5a},
    {0x00, 0x00, 0x05, 0x00, 0x25, 0x80, 0x14, 0x01,0xb5},
};
HI_U8 t1_suantong_rcv[][36] = { //[][13]
    {0x00, 0x00, 0x02, 0x90, 0x00, 0x92},

    //{0x00, 0x00, 0x12, 0xd0, 0x9d, 0x75, 0xb4, 0xcf, 0x06, 0xd4, 0xa9, 0x5c, 0x02, 0x5f, 0x56, 0xb1, 0xa9,
    //0xf1, 0xf3, 0x90, 0x00, 0xf7},
    {0x00, 0x00, 0x02, 0x6a, 0x86, 0xee},
    {0x00, 0x00, 0x06, 0x00, 0x01,0x00,0x01,0x90,0x00,0x96},
    {0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x90, 0x00,
     0x96},
    {0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x01, 0x86, 0xf1, 0x90, 0x00, 0xeb},
    {0x00, 0x00, 0x0a, 0x33, 0xc5, 0xcf, 0xc0, 0x02, 0xed, 0x02,
     0x7b, 0x90, 0x00, 0xf5},
    {0x00, 0x00, 0x04, 0x30, 0x00,0x90,0x00,0xa4},
    {0x00, 0x00, 0x0a, 0x32, 0x2e, 0x35, 0x31, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x82},
    {0x00, 0x00, 0x12, 0x00, 0x1c, 0x6b, 0xf6, 0x14, 0x9f, 0x39,
     0xe9, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x90,
     0x00, 0x58},
    {0x00, 0x00, 0x12, 0x43, 0x54, 0x49, 0x5f, 0xd3, 0xc3, 0xbb,
     0xa7, 0xbf, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90,
     0x00, 0x98},
    {0x00, 0x00, 0x07, 0x1c, 0x29, 0x0b, 0x20, 0x00, 0x90, 0x00,
     0x89},
    {0x00, 0x00, 0x07, 0x20, 0xa9, 0x0b, 0x20, 0x00, 0x90, 0x00,
     0x35},
    {0x00, 0x00, 0x08, 0x00, 0x25, 0x00, 0x02, 0x01, 0x12, 0x90,
     0x00, 0xac},
    {0x00, 0x00, 0x20, 0x50, 0x09, 0x1b, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x10, 0x00, 0x00, 0x20, 0x00, 0x00, 0x30, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x90, 0x00, 0xf2},
    {0x00, 0x00, 0x14, 0xa7, 0x10, 0x53, 0x54, 0x42, 0x20, 0x54,
     0x65, 0x61, 0x6d, 0x28, 0x43, 0x41, 0x29, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x68},
    {0x00, 0x00, 0x14, 0xa7, 0x10, 0x32, 0xb1, 0xb1, 0xbe, 0xa9,
     0xb6, 0xfe, 0xcc, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x3a},
    {0x00, 0x00, 0x14, 0xa7, 0x10, 0x33, 0xb1, 0xb1, 0xbe, 0xa9,
     0xc8, 0xfd, 0xcc, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x46},
    {0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x01, 0x86, 0xf1, 0x90, 0x00, 0xeb},
    {0x00, 0x00, 0x02, 0x6a, 0x86, 0xee},
};
HI_U32 t1_suantong_send_len[19] = {10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 18, 23, 23, 23, 9, 9}; //{6,5,5,5};
HI_U32 t1_suantong_rcv_len[19] = {6, 6, 10, 11, 17, 14, 8, 14, 22, 22, 11, 11, 12, 36, 24, 24, 24, 17, 6};

#endif

/******************t14_irdeto_access*************************************/
HI_U8 atr_irdeto_access_t14[20] = {0x3b, 0x9f, 0x21,  0xe, 0x49, 0x52, 0x44, 0x45,
                                  0x54, 0x4f, 0x20, 0x41, 0x43, 0x53, 0x20, 0x56,
                                  0x32, 0x2e, 0x32, 0x98};
HI_U32 atr_irdeto_access_t14_cnt = 20;
HI_U8 t14_irdeto_access_send[][7] = {
    {0x1, 0x2, 0x0, 0x0, 0x0, 0x0, 0x3c},
    {0x1, 0x2, 0x1, 0x0, 0x0, 0x0, 0x3d},
    {0x1, 0x2, 0x2, 0x0, 0x0, 0x0, 0x3e},
    {0x1, 0x2, 0x8, 0x0, 0x0, 0x0, 0x34},
    {0x1, 0x2, 0x3, 0x0, 0x0, 0x0, 0x3f},
};
HI_U8 t14_irdeto_access_rcv[][73] = {
    {0x1, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x14,
     0x34, 0x30, 0x30, 0x32, 0x33, 0x33, 0x30, 0x34,
     0x39, 0x34, 0x54, 0x33, 0x32, 0x34, 0x30, 0x31,
     0x41, 0x0, 0x0, 0x0, 0x6},
    {0x1, 0x2, 0x0, 0x0, 0x1, 0x0, 0x0, 0x10,
     0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x4, 0x7, 0x37, 0x94, 0xe6, 0x18,
     0x8c},
    {0x1, 0x2, 0x0, 0x0, 0x2, 0x0, 0x0, 0x10,
     0x4, 0x6, 0x12, 0x6, 0x23, 0x6, 0x24, 0x6,
     0x25, 0x6, 0x26, 0x0, 0x0, 0x43, 0x48, 0x4e,
     0x7f},
    {0x1, 0x2, 0x0, 0x0, 0x8, 0x0, 0x0, 0x40,
     0x0, 0x1, 0x8, 0x20, 0x0, 0x15, 0x60, 0x2,
     0x60, 0x0, 0x0, 0x3, 0x81, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0xc8},
    {0x1, 0x2, 0x0, 0x0, 0x3, 0x0, 0x0, 0x18,
     0x0, 0x3, 0xe9, 0x97, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x2, 0x11, 0x1, 0x1, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x49},
};
HI_U32 t14_irdeto_access_send_len[5] = {7, 7, 7, 7, 7};
HI_U32 t14_irdeto_access_rcv_len[5] = {29, 25, 25, 73, 33};

/********************t0_crypto_send*******************************/
HI_U8 t0_crypto_send[][5] = {
    {0xa4, 0xc0, 0x00, 0x00, 0x11},
    {0xa4, 0xa4, 0x00, 0x00, 0x02},
    {0x3f,0x00},
    {0xa4, 0xa4, 0x00, 0x00, 0x02},
    {0x2f,0x01},
    {0xa4, 0xa2, 0x00, 0x00, 0x01},
    {0xd1},
};
HI_U32 t0_crypto_send_len[7] = {5, 5, 2, 5, 2, 5, 1};

HI_U8 t0_crypto_rcv[][20] = {
    {0xc0, 0xdf, 0x0f, 0x05, 0x04, 0x00, 0x09, 0x3f, 0x00, 0x01,
     0x00, 0xf0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x90, 0x00},
    {0xa4},
    {0x9f,0x11},
    {0xa4},
    {0x9f,0x11},
    {0xa2},
    {0x9f,0x04},
};
HI_U32 t0_crypto_rcv_len[7] = {20, 1, 2, 1, 2, 1, 2};


/********************************************************************/

static HI_U32 g_u32MaxSndLen;
static HI_U32 g_u32SndTimes;
static HI_U32 *g_pSndLen;
static HI_CHAR *g_pSndBuf;

static HI_U32 g_u32MaxRcvLen;
static HI_U32 *g_pRcvLen;
static HI_CHAR *g_pRcvBuf;

static HI_BOOL g_bDropWord;
static HI_U8 g_u8SCIPort;

HI_S32 SelectCard(HI_U32 CardNo)
{
    if (0 == CardNo)
    {
        g_pSndBuf = (HI_CHAR*)t0_irdeto_zeta_send;
        g_pSndLen = t0_irdeto_zeta_send_len;
        g_u32MaxSndLen = sizeof(t0_irdeto_zeta_send[0]);
        g_u32SndTimes = sizeof(t0_irdeto_zeta_send) / sizeof(t0_irdeto_zeta_send[0]);

        g_pRcvBuf = (HI_CHAR*)t0_irdeto_zeta_rcv;
        g_pRcvLen = t0_irdeto_zeta_rcv_len;
        g_u32MaxRcvLen = sizeof(t0_irdeto_zeta_rcv[0]);
        g_bDropWord = HI_TRUE;
    }
    else if (1 == CardNo)
    {
        g_pSndBuf = (HI_CHAR*)t1_suantong_send;
        g_pSndLen = t1_suantong_send_len;
        g_u32MaxSndLen = sizeof(t1_suantong_send[0]);
        g_u32SndTimes = sizeof(t1_suantong_send) / sizeof(t1_suantong_send[0]);

        g_pRcvBuf = (HI_CHAR*)t1_suantong_rcv;
        g_pRcvLen = t1_suantong_rcv_len;
        g_u32MaxRcvLen = sizeof(t1_suantong_rcv[0]);
        g_bDropWord = HI_FALSE;
    }
    else if (14 == CardNo)
    {
        g_pSndBuf = (HI_CHAR*)t14_irdeto_access_send;
        g_pSndLen = t14_irdeto_access_send_len;
        g_u32MaxSndLen = sizeof(t14_irdeto_access_send[0]);
        g_u32SndTimes = sizeof(t14_irdeto_access_send) / sizeof(t14_irdeto_access_send[0]);

        g_pRcvBuf = (HI_CHAR*)t14_irdeto_access_rcv;
        g_pRcvLen = t14_irdeto_access_rcv_len;
        g_u32MaxRcvLen = sizeof(t14_irdeto_access_rcv[0]);
        g_bDropWord = HI_FALSE;
    }
    else
    {
        printf("not supported in this sample now\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DealWith_ProcedureByte(HI_UNF_SCI_PORT_E enSciPort, HI_U8* ProcedureByte, HI_U8* uRespondData,
                              HI_U32* uRespondLen)
{
    HI_U32 tmpCnt = 0, u32ReceiveLen = 0;
    HI_S32 ret = HI_FAILURE;

    ret = HI_UNF_SCI_Receive(enSciPort, ProcedureByte, 1, &u32ReceiveLen, 4000);
    if ((HI_SUCCESS != ret) || (1 != u32ReceiveLen))
    {
        printf(" T0_Communication ErrorCode:%08x, receive data len:%d\n", ret, u32ReceiveLen);
        return HI_FAILURE;
    }

    //      printf("DealWith_ProcedureByte *ProcedureByte = 0x%x \n",*ProcedureByte);

    while (*ProcedureByte == 0x60)
    {
        //          printf("DealWith_ProcedureByte *ProcedureByte = 0x%x \n",*ProcedureByte);
        usleep(10 * 1000);
        ret = HI_UNF_SCI_Receive(enSciPort, ProcedureByte, 1, &u32ReceiveLen, 4000);
        tmpCnt++;
        if (HI_SUCCESS != ret)
        {
            printf(" T0_Communication ErrorCode:%08x\n", ret);
            return ret;
        }

        if (1 != u32ReceiveLen)
        {
            printf("T0_Communication cmd fail, receive len:%d ,expect: 1", u32ReceiveLen);
            return HI_FAILURE;
        }

        if (*ProcedureByte != 0x60)
        {
            printf(" receive procedure byte time = %d \n", tmpCnt);
            break;
        }
    }

    /*the first byte of status bytes is 0x6x or 0x9x,will receive next status byte*/
    if (((*ProcedureByte & 0xF0) == 0x60) || ((*ProcedureByte & 0xF0) == 0x90))
    {
        *uRespondData = *ProcedureByte;

        ret = HI_UNF_SCI_Receive(enSciPort, uRespondData + 1, 1, &u32ReceiveLen, 4000);
        if (HI_SUCCESS != ret)
        {
            printf("T0_Communication ErrorCode:%08x\n", ret);
            return ret;
        }

        if (1 != u32ReceiveLen)
        {
            printf("T0_Communication cmd fail, receive len:%d ,expect: 1", u32ReceiveLen);
            return HI_FAILURE;
        }

        //          printf("T0_Communication received SW1 SW2 :%02x %02x\n",*uRespondData ,*(uRespondData +1));
        *uRespondLen = 2;
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

HI_S32 T0_Communication(HI_UNF_SCI_PORT_E enSciPort, HI_U8* uSendData, HI_U32 uSendLength, HI_U8* uRespondData,
                        HI_U32* uRespondLen)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32Len = 0;
    HI_U32 u32ReceiveLen = 0;
    HI_U8 ProcedureByte = 0;
    HI_U8 SendINS = 0;
    HI_U8 ReceiveBuf[258]  = {0};  /* max len :  256bytes + SW1 SW2  */
    HI_U32 u32ActualLength = 0;

    /*  if cmd length <5 will not support ,but can fix it length*/
    if (uSendLength < 5)
    {
        printf(" T0_Communication cmd length invalid:%d!\n", uSendLength);
        return HI_FAILURE;
    }

    /* CLA check */
    if (*uSendData == 0xFF)
    {
        printf(" T0_Communication  0xFF is PPS cmd not support on here!\n");
        return HI_FAILURE;
    }

    SendINS = *(uSendData + 1);
    if (((SendINS & 0x1) == 1)
        || ((SendINS & 0xF0) == 0x60)
        || ((SendINS & 0xF0) == 0x90))
    {
        printf(" T0_Communication INS invalid!\n");
        return HI_FAILURE;
    }

    /*send 5 bytes command head*/
    ret = HI_UNF_SCI_Send(enSciPort, uSendData, 5, &u32ActualLength, 4000);
    if (HI_SUCCESS != ret)
    {
        printf(" T0_Communication ErrorCode:%08x\n", ret);
        return HI_FAILURE;
    }

    /* receive data command mode have no procedure byte */
    if (5 == uSendLength)
    {
        ret = DealWith_ProcedureByte( enSciPort, ReceiveBuf, uRespondData, uRespondLen);
        if (HI_SUCCESS != ret)
        {
            printf("%s %d :DealWith_ProcedureByte respond command error ! \n ", __FUNCTION__, __LINE__ );
            return ret;
        }

        //      printf("  ReceiveBuf -> ProcedureByte = 0x%x \n",ReceiveBuf[0]);

        /* if command length == 5 and received first data is't procedure byte,will receive Le data*/
        if (SendINS != (ReceiveBuf[0] & 0xFE))
        {
            printf(" T0_Communication get procedure byte:%02x when send:%02x!\n", ReceiveBuf[0], SendINS);
        }

        if (0 == *(uSendData + 4))
        {
            u32Len = 256;
        }
        else
        {
            u32Len = *(uSendData + 4);
        }

        ret = HI_UNF_SCI_Receive(enSciPort, ReceiveBuf, u32Len, &u32ReceiveLen, 4000);
        if ((HI_SUCCESS != ret) || (u32Len != u32ReceiveLen))
        {
            printf(" T0_Communication ErrorCode:%08x, receive data len:%d (expect:%d)\n", ret, u32ReceiveLen, u32Len);
            return HI_FAILURE;
        }

        ret = HI_UNF_SCI_Receive(enSciPort, ReceiveBuf + u32Len, 2, &u32ReceiveLen, 4000);
        if ((HI_SUCCESS != ret) || (2 != u32ReceiveLen))
        {
            printf(" T0_Communication ErrorCode:%08x, receive data len:%d (expect:2)\n", ret, u32ReceiveLen);
            return HI_FAILURE;
        }

        *uRespondLen = u32Len + 2;
        memcpy((void *)uRespondData, ReceiveBuf, *uRespondLen);

        //      printf(" T0_Communication receive %d bytes!\n",*uRespondLen);
    }
    else
    {
        /* common length >5 ,procedure disposal,and send remain data,send mode */

        ret = DealWith_ProcedureByte( enSciPort, &ProcedureByte, uRespondData, uRespondLen);
        if (HI_SUCCESS != ret)
        {
            printf("%s %d :DealWith_ProcedureByte respond command error ! \n ", __FUNCTION__, __LINE__ );
            return ret;
        }

        //  printf("%s,%d... ProcedureByte=0x%x SendINS=0x%x\n",__FUNCTION__,__LINE__,ProcedureByte,SendINS);

        /*if ProcedureByte xor 0xFE  == SendINS will send all remain data,else if  ProcedureByte xor 0xFE  == SendINS^0xFE
        will send next one byte, will  wait to receive new procedure byte after all */
        if ((ProcedureByte & 0xFE) == SendINS)
        {
            /*if Lc = 0 will send remain 256 data,else send remain Lc data*/
            if (*(uSendData + 4) == 0)
            {
                ret = HI_UNF_SCI_Send(enSciPort, uSendData + 5, 256, &u32ActualLength, 4000);
            }
            else
            {
                ret = HI_UNF_SCI_Send(enSciPort, uSendData + 5,
                                      *(uSendData + 4), &u32ActualLength, 4000);
            }

            if (HI_SUCCESS != ret)
            {
                printf(" T0_Communication ErrorCode:%08x\n", ret);
                return HI_FAILURE;
            }
        }
        else if ((ProcedureByte & 0xFE) == (SendINS ^ 0xFE))
        {
            ret = HI_UNF_SCI_Send(enSciPort, uSendData + 5, 1, &u32ActualLength, 4000);
            if (HI_SUCCESS != ret)
            {
                printf(" T0_Communication ErrorCode:%08x\n", ret);
                return HI_FAILURE;
            }
        }
        else
        {
            printf(" T0_Communication invalid ACK:%02x\n", ProcedureByte);
            return HI_FAILURE;
        }

        ret = DealWith_ProcedureByte( enSciPort, &ProcedureByte, uRespondData, uRespondLen);
        if (HI_SUCCESS != ret)
        {
            printf("%s %d :DealWith_ProcedureByte respond command error ! \n ", __FUNCTION__, __LINE__ );
            return ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 CardOutProcess()
{
    return HI_SUCCESS;
}

HI_S32 CardInProcess()
{
    HI_S32 s32Ret;
    HI_UNF_SCI_STATUS_E u32SCIStatus;
    HI_S32 s32ResetTime;
    HI_U8 u8ATRCount;
    HI_U8 ATRBuf[255];
    HI_U32 u32ReadLen;
    HI_U32 i, j;
    HI_U8 u8Result = 0;

#ifndef T0_transfer
    HI_U32 u32TotalReadLen, u32SendLen;
#endif

    struct timeval tv_1, tv_2;
    struct timezone tz;
    HI_S32 usd_us;

#if TestNegotiation
    HI_U8 PPSBuf[6] = {0xff, 0x10, 0x12}, RecBuf[6];
    HI_U32 PPSLen = 0, PPSDataLen = 0;
#endif

    gettimeofday(&tv_1, &tz);

    s32Ret = HI_UNF_SCI_ResetCard(g_u8SCIPort, 0);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }
    else
    {
        printf("Reset Card\n");
    }

    s32ResetTime = 0;
    while (1)
    {
        /* will exit reset when reseting out of 10S*/
        if (s32ResetTime >= 200)
        {
            printf("Reset Card Failure\n");
            return HI_FAILURE;
        }

        /*get SCI card status */
        s32Ret = HI_UNF_SCI_GetCardStatus(g_u8SCIPort, &u32SCIStatus);
        if (HI_SUCCESS != s32Ret)
        {
            printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
            return s32Ret;
        }
        else
        {
            if (u32SCIStatus >= HI_UNF_SCI_STATUS_READY)
            {
                gettimeofday(&tv_2, &tz);
                usd_us = (tv_2.tv_sec * 1000 + tv_2.tv_usec / 1000) - (tv_1.tv_sec * 1000 + tv_1.tv_usec / 1000);

                printf("Reset:%d ms \n", usd_us);

                /*reset Success*/
                printf("Reset Card Success\n");
                break;
            }
            else
            {
                printf("Reset Card Waiting...\n");
                usleep( 50000);
                s32ResetTime += 1;
            }
        }
    }

    /*get and print ATR message*/
    s32Ret = HI_UNF_SCI_GetATR(g_u8SCIPort, ATRBuf, 255, &u8ATRCount);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    printf("GetATR Count:%d\n", u8ATRCount);

    for (i = 0; i < u8ATRCount; i++)
    {
        printf("ATRBuf[%d]:%#x\n", i, ATRBuf[i]);
    }

    /****if the card support PPS Negotiation, you can test NegotiatePPS fanction *****/
#if TestNegotiation
    s32Ret = HI_UNF_SCI_NegotiatePPS( g_u8SCIPort, (HI_U8 *)&PPSBuf, PPSLen, 1000);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_SCI_GetPPSResponData( g_u8SCIPort, (HI_U8 *)RecBuf, &PPSDataLen);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    for (i = 0; i < PPSDataLen; i++)
    {
        printf("PPSData[%d]:%#x\n", i, RecBuf[i]);
    }
#endif


    /*****if the card don't support PPS Negotiation or PPS Negotiation fail . you can try  to SetEtuFactor to
    change the baudrate,  or SetGuardTime  to change extra char guardtime,but this fanction noly for tianbai card currently******/
#if TestSetETU
    s32Ret = HI_UNF_SCI_SetEtuFactor(g_u8SCIPort, 372, 2);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_SCI_SetGuardTime(g_u8SCIPort, 2);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }
#endif


    /***test other unf fanction****/
#if TestOther
    s32Ret = HI_UNF_SCI_SetCharTimeout( g_u8SCIPort, HI_UNF_SCI_PROTOCOL_T0, 10000);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    //  s32Ret = HI_UNF_SCI_SetBlockTimeout( g_u8SCIPort,  300000);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_SCI_SetTxRetries( g_u8SCIPort, 5);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }
#endif


    HI_UNF_SCI_PARAMS_S SciParam;

    s32Ret = HI_UNF_SCI_GetParams( g_u8SCIPort, &SciParam);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    printf("SciParam.Fi = %d\n", SciParam.Fi);
    printf("SciParam.Di = %d\n", SciParam.Di);
    printf("SciParam.enProtocolType = %d\n", SciParam.enProtocolType);
    printf("SciParam.GuardDelay = %d\n", SciParam.GuardDelay);
    printf("SciParam.TxRetries = %d\n", SciParam.TxRetries);
    printf("SciParam.CharTimeouts = %d\n", SciParam.CharTimeouts);
    printf("SciParam.enSciPort = %d\n", SciParam.enSciPort);

    //  printf("SciParam.BlockTimeouts = %d\n",SciParam.BlockTimeouts);
    printf("SciParam.ActalClkRate = %d\n", SciParam.ActalClkRate);
    printf("SciParam.ActalBitRate = %d\n", SciParam.ActalBitRate);

    ///////////////////////////////////

    /* start send and receive data*/
    printf("now begin send and rcv\n");
    printf("we should send %d times\n", g_u32SndTimes);
    for (j = 0; j < g_u32SndTimes; j++)
    {
        printf("this is round %d\n", j);
        printf("send sequence:");
        for (i = 0; i < g_pSndLen[j]; i++)
        {
            printf("%#x ", g_pSndBuf[j * g_u32MaxSndLen + i]);
        }

        printf("\n");

#ifdef T0_transfer
        s32Ret = T0_Communication( g_u8SCIPort, (HI_U8 *)g_pSndBuf + j * g_u32MaxSndLen, g_pSndLen[j],
                                   (HI_U8 *)&rcv_buf, &u32ReadLen);
        if (s32Ret != HI_SUCCESS)
        {
            printf("%s->%d HI_UNF_SCI_Receive return %#x ReadLen:%d\n", __func__, __LINE__, s32Ret, u32ReadLen);
            return s32Ret;
        }

#else
        s32Ret = HI_UNF_SCI_Send(g_u8SCIPort, (HI_U8 *)g_pSndBuf + j * g_u32MaxSndLen, g_pSndLen[j], &u32SendLen, 5000);
        if ((HI_SUCCESS != s32Ret) || (g_pSndLen[j] != u32SendLen))
        {
            printf("%s->%d, HI_UNF_SCI_Send return %d SendLen:%d\n", __func__, __LINE__, s32Ret, u32SendLen);
            return s32Ret;
        }

        u32TotalReadLen = 0;
        while (u32TotalReadLen < g_pRcvLen[j])
        {
            //      gettimeofday(&tv_1, &tz);
            s32Ret = HI_UNF_SCI_Receive(g_u8SCIPort, (HI_U8 *)&rcv_buf[u32TotalReadLen], 1, &u32ReadLen, 10000);
            if (s32Ret != HI_SUCCESS)
            {
                printf("%s->%d HI_UNF_SCI_Receive return %#x ReadLen:%d\n", __func__, __LINE__, s32Ret, u32ReadLen);
                return s32Ret;
            }

            if ((0x60 == rcv_buf[u32TotalReadLen]) && (HI_TRUE == g_bDropWord))
            {
                printf("drop 0x60\n");
                continue;
            }

            u32TotalReadLen++;

            //          gettimeofday(&tv_2, &tz);
            //          usd_us = (tv_2.tv_sec * 1000000 + tv_2.tv_usec ) - (tv_1.tv_sec * 1000000 + tv_1.tv_usec );
            //          printf(" [%d]  %d ", j,usd_us);
        }
#endif


        printf("expect rcv sequence:");
        for (i = 0; i < g_pRcvLen[j]; i++)
        {
            printf("%#x ", g_pRcvBuf[j * g_u32MaxRcvLen + i]);
        }

        printf("\nactual rcv sequence:");
        for (i = 0; i < g_pRcvLen[j]; i++)
        {
            if ((0x60 == rcv_buf[ i ]) && (HI_TRUE == g_bDropWord))
            {
                printf("drop 0x60\n");
                continue;
            }

            printf("%#x ", rcv_buf[i]);
            if ((0 == u8Result) && (rcv_buf[i] != g_pRcvBuf[j * g_u32MaxRcvLen + i]))
            {
                u8Result = 1;
            }
        }

        printf("\n");

        if (1 == u8Result)
        {
            printf("ERROR\n");
        }
        else
        {
            printf("OK\n");
        }

        u8Result = 0;
    }

    return HI_SUCCESS;
}

#define ACTION_CARDIN 0       /* card inset flag*/
#define ACTION_CARDOUT 1      /* card pull out flag*/
#define ACTION_NONE 2         /* card immobile */

HI_BOOL g_bCardStatus = HI_FALSE; /*g_bCardStatus = HI_TRUE indicate card in ;g_bCardStatus = HI_FALSE  indicate no card */
HI_U8 g_CardAction = ACTION_NONE;

HI_S32 CheckCa(HI_U8 *pAction)
{
    HI_S32 s32Ret;
    HI_UNF_SCI_STATUS_E enStatus;
    HI_BOOL bStatus;

    s32Ret = HI_UNF_SCI_GetCardStatus(g_u8SCIPort, &enStatus);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    if (enStatus <= HI_UNF_SCI_STATUS_NOCARD)
    {
        bStatus = HI_FALSE;  /*no card  */
    }
    else
    {
        bStatus = HI_TRUE;   /*have card*/
    }

    /* if bStatus is ture indicate the card have been pull out or push in */
    if (g_bCardStatus != bStatus)
    {
        g_bCardStatus = bStatus;
        if (HI_TRUE == bStatus)
        {
            *pAction = ACTION_CARDIN;     /*card in  */
        }
        else
        {
            *pAction = ACTION_CARDOUT;    /*card out */
        }
    }
    else
    {
        *pAction = ACTION_NONE;           /*no operation*/
    }

    return HI_SUCCESS;
}

static HI_BOOL g_RunFlag1, g_RunFlag2;
static pthread_mutex_t g_SciMutex = PTHREAD_MUTEX_INITIALIZER;

HI_VOID* CheckSci(HI_VOID* args)
{
    HI_S32 Ret;

    while (g_RunFlag1)
    {
        pthread_mutex_lock(&g_SciMutex);
        Ret = CheckCa(&g_CardAction);
        if (HI_SUCCESS != Ret)
        {
            printf("CheckCa failed\n");
        }

        pthread_mutex_unlock(&g_SciMutex);
        usleep(50 * 1000);
    }

    return (HI_VOID*)HI_SUCCESS;
}

HI_VOID* RunSci(HI_VOID *args)
{
    HI_U8 CardAction;

    while (g_RunFlag2)
    {
        pthread_mutex_lock(&g_SciMutex);
        CardAction = g_CardAction;
        pthread_mutex_unlock(&g_SciMutex);
        if (ACTION_CARDIN == CardAction)
        {
            printf("CARD IN\n");
            CardInProcess();
        }
        else if (ACTION_CARDOUT == CardAction)
        {
            printf("CARD OUT\n");
            CardOutProcess();
        }
        else
        {
            //do nothing
        }

        usleep(50 * 1000);
    }

    return (HI_VOID*)HI_SUCCESS;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Freq;
    HI_UNF_SCI_MODE_E enMode;
    HI_U32 u8ProtocolType;
    HI_UNF_SCI_PROTOCOL_E enProtocolType;
    HI_UNF_SCI_LEVEL_E enVccenLevel, u32DetectLevel;
    HI_U32 u32Value = 0;
    pthread_t task1, task2;

#define STR_LEN (10)
    HI_CHAR a[STR_LEN];

    printf("please select sci prot: 0-SCI0 1-SCI1\n");
    scanf("%s", a);
    g_u8SCIPort = (HI_U32)strtoul(a, 0, 0);
    if((0 != g_u8SCIPort) && (1 != g_u8SCIPort))
    {
        printf("input parament error!\n");
        return HI_FAILURE;
    }

    printf("please select protocol type: 0-T0 1-T1 14-T14 others-T0\n");
    scanf("%s", a);
    u8ProtocolType = (HI_U32)strtoul(a, 0, 0);
    if ((0 != u8ProtocolType) && (1 != u8ProtocolType) && (14 != u8ProtocolType))
    {
        u8ProtocolType = 0;
    }

    printf("u8ProtocolType = %d\n", u8ProtocolType);
    s32Ret = SelectCard(u8ProtocolType);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    if (14 == u8ProtocolType)
    {
        enProtocolType = HI_UNF_SCI_PROTOCOL_T14;
        u32Freq = 5000;
    }
    else
    {
        enProtocolType = u8ProtocolType;

        u32Freq = 3570;
    }

    HI_UNF_SCI_Init();

    /*open SCI device */
    s32Ret = HI_UNF_SCI_Open(g_u8SCIPort, enProtocolType, u32Freq);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        HI_UNF_SCI_DeInit();
        return s32Ret;
    }

    printf("please input clock mode(default %d): %d-CMOS, %d-OD, others-default\n", HI_SCI_CLK_MODE, HI_UNF_SCI_MODE_CMOS,
            HI_UNF_SCI_MODE_OD);

    scanf("%s", a);
    u32Value = (HI_U32)strtoul(a, 0, 0);
    if ((u32Value != HI_UNF_SCI_MODE_CMOS) && (u32Value != HI_UNF_SCI_MODE_OD))
    {
        enMode = HI_SCI_CLK_MODE;
    }
    else
    {
        enMode = u32Value;
    }

    s32Ret = HI_UNF_SCI_ConfigClkMode(g_u8SCIPort, enMode);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)

    printf("please input vccen mode (default %d): %d-CMOS, %d-OD, others-default\n", HI_SCI_VCCEN_MODE, HI_UNF_SCI_MODE_CMOS,
            HI_UNF_SCI_MODE_OD);

    scanf("%s", a);
    u32Value = (HI_U32)strtoul(a, 0, 0);

    if ((u32Value != HI_UNF_SCI_MODE_CMOS) && (u32Value != HI_UNF_SCI_MODE_OD))
    {
        enMode = HI_SCI_VCCEN_MODE;
    }
    else
    {
        enMode = u32Value;
    }

    /*set sci card detect level.it reference the hardware design*/
    s32Ret = HI_UNF_SCI_ConfigVccEnMode(g_u8SCIPort, enMode);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }

    printf("please input reset mode (default %d): %d-CMOS, %d-OD, others-default\n", HI_SCI_RESET_MODE, HI_UNF_SCI_MODE_CMOS,
            HI_UNF_SCI_MODE_OD);

    scanf("%s", a);
    u32Value = (HI_U32)strtoul(a, 0, 0);

    if ((u32Value != HI_UNF_SCI_MODE_CMOS) && (u32Value != HI_UNF_SCI_MODE_OD))
    {
        enMode = HI_SCI_RESET_MODE;
    }
    else
    {
        enMode = u32Value;
    }

    /*set sci card detect level.it reference the hardware design*/
    s32Ret = HI_UNF_SCI_ConfigResetMode(g_u8SCIPort, enMode);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }
#endif

    printf("please input vccen level(default %d): %d-Low level, %d-High level, others-default\n", HI_SCI_VCCEN_LEVEL, HI_UNF_SCI_LEVEL_LOW,
            HI_UNF_SCI_LEVEL_HIGH);

    scanf("%s", a);
    u32Value = (HI_U32)strtoul(a, 0, 0);

    if ((u32Value != HI_UNF_SCI_LEVEL_LOW) && (u32Value != HI_UNF_SCI_LEVEL_HIGH))
    {
        enVccenLevel = HI_SCI_VCCEN_LEVEL;
    }
    else
    {
        enVccenLevel = u32Value;
    }

    /*set sci card power enable level.it reference the hardware design*/
    s32Ret = HI_UNF_SCI_ConfigVccEn(g_u8SCIPort, enVccenLevel);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }

    printf("please input detect level(default %d): %d-Low level, %d-High level, others-default\n", HI_SCI_DETECT_LEVEL, HI_UNF_SCI_LEVEL_LOW,
            HI_UNF_SCI_LEVEL_HIGH);

    scanf("%s", a);
    u32Value = (HI_U32)strtoul(a, 0, 0);

    if ((u32Value != HI_UNF_SCI_LEVEL_LOW) && (u32Value != HI_UNF_SCI_LEVEL_HIGH))
    {
        u32DetectLevel = HI_SCI_DETECT_LEVEL;
    }
    else
    {
        u32DetectLevel = u32Value;
    }

    /*set sci card detect level.it reference the hardware design*/
    s32Ret = HI_UNF_SCI_ConfigDetect(g_u8SCIPort, u32DetectLevel);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR1;
    }

    printf("protocol:%d, vcc-en:%d, detect-level:%d\n",
            u8ProtocolType, enVccenLevel, u32DetectLevel);

    pthread_mutex_init(&g_SciMutex, NULL);

    /** create thread to checkca */
    g_RunFlag1 = 1;
    s32Ret = pthread_create(&task1, NULL, CheckSci, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR2;
    }

    /** Create thread to send and rcv data */
    g_RunFlag2 = 1;
    s32Ret = pthread_create(&task2, NULL, RunSci, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR3;
    }

    printf("Press any key to finish sci demo\n");
    getchar();
    getchar();

    g_RunFlag1 = 0;
    g_RunFlag2 = 0;

    pthread_join(task1, NULL);
    pthread_join(task2, NULL);

    pthread_mutex_destroy(&g_SciMutex);
    HI_UNF_SCI_DeactiveCard( g_u8SCIPort);
    HI_UNF_SCI_Close(g_u8SCIPort);

    printf("\nrun sci demo finished\n");
    return HI_SUCCESS;

ERR3:
    g_RunFlag1 = 0;
    pthread_join(task1, NULL);
ERR2:
    pthread_mutex_destroy(&g_SciMutex);
ERR1:
    HI_UNF_SCI_DeactiveCard( g_u8SCIPort);
    HI_UNF_SCI_Close(g_u8SCIPort);
    HI_UNF_SCI_DeInit();
    return HI_FAILURE;
}
//lint -restore

