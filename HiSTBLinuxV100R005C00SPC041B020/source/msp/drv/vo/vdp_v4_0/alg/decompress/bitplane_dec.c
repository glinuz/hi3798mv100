//-----------

#include "ttv_mode.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


int BitPlaneVld ( TTV_COMP_T * ttv_comp_ptr,  uint16 * bit_plane_ptr)
{
	int code_len = 0;
	uint32 code_word;
	uint16 bit_plane = 0;
	int is_all_zero = 0;
	int is_one_one = 0;
	int one_pos = 0;
	

	code_word = BitStreamShowBits (32);

	//code_word parsing
	if(ttv_comp_ptr->is_simple_vlc==0)
	{
		if ((code_word & 1) == 0)
		{
			code_len = 17;
			bit_plane = (code_word >> 1) & 0xffff;

#if ERROR_REPORT
	if((bit_plane==0x0001)&&(bit_plane==0x0002)&&(bit_plane==0x0004)&&(bit_plane==0x0008))
		ttv_comp_ptr->error = 3;
	if((bit_plane==0x0010)&&(bit_plane==0x0020)&&(bit_plane==0x0040)&&(bit_plane==0x0080))
		ttv_comp_ptr->error = 4;
	if((bit_plane==0x0100)&&(bit_plane==0x0200)&&(bit_plane==0x0400)&&(bit_plane==0x0800))
		ttv_comp_ptr->error = 5;
	if((bit_plane==0x1000)&&(bit_plane==0x2000)&&(bit_plane==0x4000)&&(bit_plane==0x8000))
		ttv_comp_ptr->error = 6;
	if(bit_plane==0x0)
		ttv_comp_ptr->error = 7;

#endif
		}
		else if ((code_word & 3) == 3)
		{
			code_len = 2;
			is_all_zero = 1;
		}
		else if ((code_word & 7) == 1)
		{
			code_len = 7;
			is_one_one = 1;
			one_pos = (code_word >> 3) & 0xf;
		}
	}
	else
	{
		if ((code_word & 1) == 0)
		{
			code_len = 17;
			bit_plane = (code_word >> 1) & 0xffff;
		}
		else if ((code_word & 1) == 1)
		{
			code_len = 1;
			is_all_zero = 1;
		}
	}



	//bit plane reconstruction
	if (is_all_zero)
	{
		bit_plane = 0;
	}
	else if (is_one_one)
	{
		bit_plane = 1 << one_pos;
	}

#if DEC_BP_PROBE
	fprintf(dec_bp_pb,"%04x",bit_plane);
//	printf("%04x",bit_plane);
#endif
#if DEC_BP_CODE_PROBE
	fprintf(dec_bp_code_pb,"%04x\n",bit_plane);
#endif
	code_word = code_word & ((1 << code_len) - 1);   //only for trace

	BitStreamFlushBits (code_len);

	*bit_plane_ptr = bit_plane;

	return code_len;
}

void InsertBitPlane (TTV_COMP_T * ttv_comp_ptr, 
					 uint16 abs_coeff[16], 
					 uint16 bit_plane, 
					 int bp_idx
					 )
{
	int coeff_idx;
	int bit_value;
	int coeff_num;

	coeff_num=16;

	for (coeff_idx = 0; coeff_idx < coeff_num; coeff_idx++)
	{
		bit_value = (bit_plane >> coeff_idx) & 1;

		abs_coeff[coeff_idx] = abs_coeff[coeff_idx] | (bit_value << bp_idx);

	}	

}

int BitPlaneHFDecode (
						 TTV_COMP_T * ttv_comp_ptr, 
						 uint16 abs_coeff[16],
						 int bp_idx
						 )
{
	int code_len;
	uint16 bit_plane;
	

	code_len = BitPlaneVld (ttv_comp_ptr, &bit_plane);	

	if(ttv_comp_ptr->error==0)
	{
		InsertBitPlane (ttv_comp_ptr,abs_coeff, bit_plane, bp_idx);	
		return code_len;
	}
	else
	{
		return 0;
	}
}

int GetNnzNum (TTV_COMP_T * ttv_dec_ptr, uint16 coeff_ptr[16])
{
	int coeff_idx;
	int nnz = 0;
	int coeff_num;

	coeff_num=16;

	for (coeff_idx = 0; coeff_idx < coeff_num; coeff_idx++)
	{
		//printf("%d ",coeff_ptr[coeff_idx]);
		if (coeff_ptr[coeff_idx] != 0)
		{
			nnz++;
		}
	}
	//printf("\n");

	return nnz;
}



int BitPlaneDecoding (TTV_COMP_T * ttv_dec_ptr,int sample)
{

	int nzb;
	int bp_idx;
	int x,i;
	int bit_cnt;


	if(sample==0)
	{
		if(ttv_dec_ptr->bit_depth==8)
			ttv_dec_ptr->ori_pixel[0]=BitStreamReadBits (8);
		else
			ttv_dec_ptr->ori_pixel[0]=BitStreamReadBits (10);
	}
	else
	{
		if(ttv_dec_ptr->bit_depth==8)
		{
			ttv_dec_ptr->ori_pixel[1]=BitStreamReadBits (8);
			ttv_dec_ptr->ori_pixel[2]=BitStreamReadBits (8);
		}
		else
		{
			ttv_dec_ptr->ori_pixel[1]=BitStreamReadBits (10);
			ttv_dec_ptr->ori_pixel[2]=BitStreamReadBits (10);
		}
	}

	for(i=0;i<8;i++)
	{
		if(ttv_dec_ptr->bit_depth==8)
			ttv_dec_ptr->nzb[i] = BitStreamReadBits (3);
		else
			ttv_dec_ptr->nzb[i] = BitStreamReadBits (4);
	}
	

	for(i=0;i<8;i++)
	{

		uint16 code_diff_buf[16]={0};
		uint16 code_sign_buf[16]={0};
#if DEC_BP_CODE_PROBE
			fprintf(dec_bp_code_pb,"x=%d    y=%d    %d  maxbp==%d\n",ttv_dec_ptr->mb_x,ttv_dec_ptr->mb_y,i,ttv_dec_ptr->nzb[i]);
#endif
		for(x=0;x<16;x++)
		{
			code_sign_buf[x]=BitStreamReadBits(1);
		}

#if DEC_SIGN_PROBE
		for(x=15;x>=0;x--)
		{
			fprintf(dec_sign_pb,"%d",code_sign_buf[x]);
		}
		fprintf(dec_sign_pb,"\n");
#endif
		nzb = ttv_dec_ptr->nzb[i];
	//	fprintf(rc_output,"nzb==%d\n ",nzb);

#if DEC_BP_PROBE
		
		if(nzb<=4)
		{

			//printf("\n",bit_plane);
			for (bp_idx = 4; bp_idx > nzb; bp_idx--)
			{
				bit_plane=0;
				fprintf(dec_bp_pb,"%04x",bit_plane);
			//	printf("%04x",bit_plane);
			}
		}
		else
		{
			for (bp_idx = 9; bp_idx > nzb; bp_idx--)
			{
				bit_plane=0;
				fprintf(dec_bp_pb,"%04x",bit_plane);
				//printf("%04x",bit_plane);
			}
		}
#endif
		for (bp_idx = nzb; bp_idx >= 0; bp_idx--)
		{		


			bit_cnt = BitPlaneHFDecode (ttv_dec_ptr, code_diff_buf, bp_idx);
#if DEC_BP_PROBE
		if(bp_idx==5)
		{
		fprintf(dec_bp_pb,"\n",bit_plane);
	//	printf("\n",bit_plane);
		}
		
#endif
			if(bit_cnt==0)
				return 0;

		}	

#if DEC_BP_PROBE
		fprintf(dec_bp_pb,"\n",bit_plane);
	//	printf("\n",bit_plane);
#endif

		for(x=0;x<16;x++)
		{
			if(code_sign_buf[x]==0)
				ttv_dec_ptr->code_diff[i*16+x]=(int)code_diff_buf[x];
			else
				ttv_dec_ptr->code_diff[i*16+x]=(0-(int)code_diff_buf[x]);

		}
#if DEC_BP_CODE_PROBE
		for(x=0;x<16;x++)
		{
			fprintf(dec_bp_code_pb,"%02x",ABS(ttv_dec_ptr->code_diff[i*16+x]));
		}
		fprintf(dec_bp_code_pb,"\n");
#endif


#if DEC_DIF_PROBE
		for(x=1;x<2;x++)
		{
			fprintf(dec_dif_pb,"%03x%03x%03x%03x%03x%03x%03x%03x",ABS(ttv_dec_ptr->code_diff[i*16+8*x+7]),ABS(ttv_dec_ptr->code_diff[i*16+8*x+6]),
				ABS(ttv_dec_ptr->code_diff[i*16+8*x+5]),ABS(ttv_dec_ptr->code_diff[i*16+8*x+4]),
				ABS(ttv_dec_ptr->code_diff[i*16+8*x+3]),ABS(ttv_dec_ptr->code_diff[i*16+8*x+2]),
				ABS(ttv_dec_ptr->code_diff[i*16+8*x+1]),ABS(ttv_dec_ptr->code_diff[i*16+8*x]));
		}
		for(x=0;x<1;x++)
		{
			fprintf(dec_dif_pb,"%03x%03x%03x%03x%03x%03x%03x%03x\n",ABS(ttv_dec_ptr->code_diff[i*16+8*x+7]),ABS(ttv_dec_ptr->code_diff[i*16+8*x+6]),
				ABS(ttv_dec_ptr->code_diff[i*16+8*x+5]),ABS(ttv_dec_ptr->code_diff[i*16+8*x+4]),
				ABS(ttv_dec_ptr->code_diff[i*16+8*x+3]),ABS(ttv_dec_ptr->code_diff[i*16+8*x+2]),
				ABS(ttv_dec_ptr->code_diff[i*16+8*x+1]),ABS(ttv_dec_ptr->code_diff[i*16+8*x]));
		}

#endif


	}
	return 1;

}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif