/*******************************************************************
 * File Name:		ttv_mode.h
 * Authors:			
 * Data:			2012.11.20
 * Copyright:		tongtuic technology
 * Description:		structure definition
 *
 *******************************************************************/
#ifndef _TTV_MODE_H_
#define _TTV_MODE_H_
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#include "drv_disp_osal.h"
//--------------TEST PRINTF--------//
#define SUM_TEST_PRINTF  0
#define FRM_TEST_PRINTF  1
//--------------TEST----------------//
#define FUNC_TEST  0
#define ERROR_REPORT    0
//--------------PROBE----------------//
#define ENC_DIF_PROBE   0
#define ENC_BP_PROBE    0
#define ENC_BS_PROBE    0
#define ENC_HEAD_PROBE  0

#define DEC_SIGN_PROBE      0
#define DEC_DIF_PROBE       0
#define DEC_BP_PROBE        0
#define DEC_BS_PROBE		0
#define DEC_HEAD_PROBE      0
#define DEC_BP_CODE_PROBE   0
//--------------PROBE----------------//
#define BLOCK_HEIGHT 1
#define SEGMENT     64
//#define ALIGN_BIT   256
#define ALIGN_BIT   128

#define BP_ALL_ZERO		0
#define BP_ALL_ONE		1
#define BP_ONE_ONE		2
#define BP_ONE_ZERO		3


typedef unsigned char 	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;
typedef signed   char 	int8;
typedef signed   short 	int16;

#define TT_TRUE		1
#define TT_FALSE		0	

#define ABS(x) (((x) >= 0) ? (x) : -(x))

//--------------PROBE----------------//
#if DEC_HEAD_PROBE
extern FILE *dec_head_pb;
#endif
#if DEC_BP_PROBE
extern FILE *dec_bp_pb;
#endif
#if DEC_DIF_PROBE
extern FILE *dec_dif_pb;
#endif
#if DEC_BS_PROBE
extern FILE *dec_bs_pb;
#endif
#if DEC_BP_CODE_PROBE
extern FILE *dec_bp_code_pb;
#endif
#if DEC_SIGN_PROBE
extern FILE *dec_sign_pb;
#endif
//--------------PROBE----------------//

typedef struct 
{
	int org_frm_width;
	int org_frm_height;
	int frm_width;
	int frm_height;
	int raw;
	int bit_depth;
	int is_simple_vlc;
	int mb_num_x;
	int mb_num_y;
	int mb_x;
	int mb_y;
	int code_mode;
	int bpye_num;
	int is_segment_raw;
	int *raw_segment_list;

	uint16 blkwork_buf[3][2][64];
	int16 diff[3][2][64];
	int16 diff_sad[3][2][64];
	int16 sign[3][2][64];
	int16 code_diff[128];
	int16 code_ori[128];
	int16 code_sign[128];
	uint16 code_buf[128];
	int16  pred_mode[8];
	uint16 ori_pixel[6];
	int16 nzb[8];


//**********************************************************************************//

//-------------------func coverage para---------------------------------------------//
	
	//-------------------bitplane code func-------------------------//
	int max_nzb;
	uint32 mbp_all0_cnt[8];
	uint32 mbp_only1_cnt[8];
	uint32 mbp_norm_cnt[8];
	int mbp_all0[8];
	int mbp_only1[8];
	int mbp_norm[8];
	//-------------------pred mode func----------------------------//
	int oddline_leftpred;
	int oddline_uppred;
	int evenline_leftpred;
	int evenline_uppred;
	uint32 oddline_leftpred_cnt;
	uint32 oddline_uppred_cnt;
	uint32 evenline_leftpred_cnt;
	uint32 evenline_uppred_cnt;

	int oddline_1bk_leftpred;
	uint32  oddline_1bk_leftpred_cnt;
	//------------------raw func-------------------------------------//
	int raw_edge;
	int raw_enc;
	int raw_cfg;

	uint32 raw_edge_cnt;
	uint32 raw_enc_cnt;
	uint32 raw_cfg_cnt;
//**********************************************************************************//
	int error;
}TTV_COMP_T;

typedef struct 
{
	int org_frm_width;	//图像宽度(Y和UV配置相同)
	int org_frm_height;	//图像高度(Y和UV配置相同)
	int raw;			//该帧是否使用压缩，0：是，1：否
	int bit_depth;		//图像位深度
	int is_simple_vlc;	//固定配1
	int dec_sum_bit;	
	int is_segment_raw;	//该segment是否使用压缩，0：是，1：否
	int *raw_segment_list;	//该帧的raw列表，指示各个seg是否压缩，0：是，1：否
	
}OTT_CFG;

typedef struct
{
	uint8 * bs_buf_ptr;			//point to start of the bit stream buffer
	uint8 * cur_buf_ptr;				//point to current position of bit stream buffer
	int		left_bits;			//left bits in one byte, which can be filled bits
	int		total_bits;			//total bits encoded for current picture
	int     total_head_bits;
	int     cur_mb_bit;
	int		buf_size;			//buffer size for bs buffer

}BIT_STREAM_ENC;


typedef struct
{
	uint8 *p_start;
    uint8 *p;
	int		total_bits;
	int     cur_mb_bit;
    int     i_left;    /* i_count number of available bits */
    int     i_bits_encoded; /* RD only */

}BIT_STREAM_DEC;

extern OTT_CFG    * ott_cfg;
extern BIT_STREAM_ENC	* bit_stream_enc;
extern BIT_STREAM_DEC	* bit_stream_dec;

int BitPlaneCoding (TTV_COMP_T * ttv_comp_ptr,int writebs,int sample);
void BitstreamInitEnc (BIT_STREAM_ENC * bs_ptr, uint8 * bs_buf_ptr);
void BitstreamWrite (BIT_STREAM_ENC * bs_ptr, uint32 code_word, int code_len);
int CompressOneFrame (OTT_CFG * ott_cfg, int sample,uint16* frame0, uint16 *frame1,uint8 * bs_buf_ptr,  uint16 * bs_head_ptr);
void BitStreamInitDec (BIT_STREAM_DEC * bs_ptr, uint8 * p_data, int i_data);
uint32 BitStreamShowBits (int n_bits);
void BitStreamFlushBits (int n_bits);
uint32 BitStreamReadBits (int n_bits);
uint32 BitStreamReadHeadBits (int n_bits);
int BitPlaneDecoding (TTV_COMP_T * ttv_dec_ptr,int sample);
int DecoderOneFrame (OTT_CFG * ott_cfg, int sample,uint8 * bs_buf_ptr, uint16 * bs_head_ptr,uint16 * frame0, uint16* frame1);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif