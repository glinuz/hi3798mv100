//-----

#include "ttv_mode.h"
#if DEC_HEAD_PROBE
FILE *dec_head_pb;
#endif
#if DEC_BP_PROBE
FILE *dec_bp_pb;
#endif
#if DEC_DIF_PROBE
FILE *dec_dif_pb;
#endif
#if DEC_BS_PROBE
FILE *dec_bs_pb;
#endif
#if DEC_BP_CODE_PROBE
FILE *dec_bp_code_pb;
#endif
#if DEC_SIGN_PROBE
FILE *dec_sign_pb;
#endif

OTT_CFG    * ott_cfg;
BIT_STREAM_ENC	* bit_stream_enc;
BIT_STREAM_DEC	* bit_stream_dec;

void CmpInit (TTV_COMP_T * ttv_dec_ptr, OTT_CFG   *ott_cfg)
{
	ttv_dec_ptr->raw  = ott_cfg->raw;
	ttv_dec_ptr->bit_depth  = ott_cfg->bit_depth;
	ttv_dec_ptr->is_simple_vlc  = ott_cfg->is_simple_vlc;
	ttv_dec_ptr->org_frm_width  = ott_cfg->org_frm_width;
	ttv_dec_ptr->org_frm_height = ott_cfg->org_frm_height;
	ttv_dec_ptr->mb_num_x  = ((ttv_dec_ptr->org_frm_width + 63) >> 6);	
	ttv_dec_ptr->frm_width  = ttv_dec_ptr->mb_num_x * SEGMENT;

	ttv_dec_ptr->mb_num_y  = ttv_dec_ptr->org_frm_height;
	ttv_dec_ptr->frm_height = ttv_dec_ptr->mb_num_y * BLOCK_HEIGHT;

}

void CmpClose (TTV_COMP_T * ttv_dec_ptr)
{
	DISP_FREE(ttv_dec_ptr);

	DISP_FREE(bit_stream_dec);

}


void PutRecMB (TTV_COMP_T * ttv_comp_ptr, int mb_y, int mb_x,int sample,int segment,uint16 * frame0, uint16 * frame1)
{
	int i;
	int offset;
	int frm_width;
	uint16 * frame_ptr;
	int odd;
	

	if(sample==1)
		segment=segment/2;


	if(mb_y%2==0)
		odd=0;
	else
		odd=1;

	frm_width = ttv_comp_ptr->org_frm_width;

	offset = frm_width * mb_y+ mb_x * SEGMENT;

	//put block Y

	
	if(sample==0)
	{
		frame_ptr = frame0 + offset;

		for (i = 0; i < segment; i++)
		{
			frame_ptr[i] = ttv_comp_ptr->blkwork_buf[sample][odd][i];
		//	printf("%x ",frame_ptr[i]);
		}
	//	printf("\n");

	}

	if(sample== 1)
	{


		frame_ptr = frame0 + offset/2;

		for (i = 0; i < segment; i++)
		{
			frame_ptr[i] = ttv_comp_ptr->blkwork_buf[1][odd][i];
		}

		
		frame_ptr =frame1 + offset/2;

		for (i = 0; i < segment; i++)
		{
			frame_ptr[i] = ttv_comp_ptr->blkwork_buf[2][odd][i];
		}
	}


}


void MBHeaderDec_BP (TTV_COMP_T * ttv_dec_ptr,int sample,uint16 * head_ptr_tmp)
{	
	uint16 pred_mode;


#if DEC_HEAD_PROBE
	fprintf(dec_head_pb,"%04x\n",(* head_ptr_tmp));
#endif

	ttv_dec_ptr->code_mode = (*head_ptr_tmp)&0x1;
	if(ttv_dec_ptr->bit_depth==8)
		ttv_dec_ptr->bpye_num = (((*head_ptr_tmp)>>1)&0x7);
	else
		ttv_dec_ptr->bpye_num = (((*head_ptr_tmp)>>1)&0xf);
	pred_mode=(((*head_ptr_tmp)>>8)&0xff);


	if(ttv_dec_ptr->code_mode )
	{
		ttv_dec_ptr->pred_mode[7]=(pred_mode&0x80)>>7;
		ttv_dec_ptr->pred_mode[6]=(pred_mode&0x40)>>6;
		ttv_dec_ptr->pred_mode[5]=(pred_mode&0x20)>>5;
		ttv_dec_ptr->pred_mode[4]=(pred_mode&0x10)>>4;
		ttv_dec_ptr->pred_mode[3]=(pred_mode&0x08)>>3;
		ttv_dec_ptr->pred_mode[2]=(pred_mode&0x04)>>2;
		ttv_dec_ptr->pred_mode[1]=(pred_mode&0x02)>>1;
		ttv_dec_ptr->pred_mode[0]=pred_mode&0x01;
	}

#if ERROR_REPORT
	if(ttv_dec_ptr->bit_depth==8)
	{
		blank_pos = (((*head_ptr_tmp)>>4)&0xf);
		if(blank_pos!=0)
			ttv_dec_ptr->error = 1;
	}
	else
	{
		blank_pos = (((*head_ptr_tmp)>>5)&0x7);
		if(blank_pos!=0)
			ttv_dec_ptr->error = 1;
	}
	if((ttv_dec_ptr->code_mode==0) && ( pred_mode!=0 ))
		ttv_dec_ptr->error = 2;
#endif

}

void MBInit (TTV_COMP_T * ttv_dec_ptr)
{

	memset (ttv_dec_ptr->blkwork_buf, 0, 3*2*64*sizeof(uint16));
	memset (ttv_dec_ptr->diff       , 0, 3*2*64*sizeof(int16));
	memset (ttv_dec_ptr->diff_sad,    0, 3*2*64*sizeof(int16));
	memset (ttv_dec_ptr->sign,        0, 3*2*64*sizeof(int16));

	memset (ttv_dec_ptr->code_diff, 0, 128*sizeof(int16));
	memset (ttv_dec_ptr->code_ori,  0, 128*sizeof(int16));
	memset (ttv_dec_ptr->code_sign, 0, 128*sizeof(int16));


	memset (ttv_dec_ptr->code_buf, 0, 128*sizeof(uint16));
	memset (ttv_dec_ptr->ori_pixel, 0,6*sizeof(uint16));
	memset (ttv_dec_ptr->nzb, 0,8*sizeof(int16));
}


void Pred_BitPlane_Dec_Y (TTV_COMP_T * ttv_comp_ptr)
{
	int i,j;
	int segment;

		segment=64;

		ttv_comp_ptr->blkwork_buf[0][0][0]=ttv_comp_ptr->ori_pixel[0];

		for(i=1;i<segment;i++)
		{
			ttv_comp_ptr->blkwork_buf[0][0][i] = ttv_comp_ptr->diff[0][0][i] + ttv_comp_ptr->blkwork_buf[0][0][i-1] ; 	
		}


		for(j=0;j<(segment/8);j++)
		{

			if(ttv_comp_ptr->pred_mode[j]==0)
			{			
				for(i=(j*8);i<((j+1)*8);i++)
				{
					if((i==0)&&(j==0))
						ttv_comp_ptr->blkwork_buf[0][1][i] = ttv_comp_ptr->diff[0][1][i] + ttv_comp_ptr->blkwork_buf[0][0][i] ;
					else
						ttv_comp_ptr->blkwork_buf[0][1][i] = ttv_comp_ptr->diff[0][1][i] + ttv_comp_ptr->blkwork_buf[0][1][i-1] ;	

				}
			}
			else
			{
				for(i=(j*8);i<((j+1)*8);i++)
				{
					ttv_comp_ptr->blkwork_buf[0][1][i] = ttv_comp_ptr->diff[0][1][i] + ttv_comp_ptr->blkwork_buf[0][0][i] ;
				}
			}
		}
}

void Pred_BitPlane_Dec_UV (TTV_COMP_T * ttv_comp_ptr)
{
	int i,j;
	int segment;

	segment=32;


		ttv_comp_ptr->blkwork_buf[1][0][0]=ttv_comp_ptr->ori_pixel[1];
		ttv_comp_ptr->blkwork_buf[2][0][0]=ttv_comp_ptr->ori_pixel[2];

		for(i=1;i<segment;i++)
		{
			ttv_comp_ptr->blkwork_buf[1][0][i] = ttv_comp_ptr->diff[1][0][i] + ttv_comp_ptr->blkwork_buf[1][0][i-1] ; 	
			ttv_comp_ptr->blkwork_buf[2][0][i] = ttv_comp_ptr->diff[2][0][i] + ttv_comp_ptr->blkwork_buf[2][0][i-1] ; 	
		}



		for(j=0;j<(segment/4);j++)
		{

			if(ttv_comp_ptr->pred_mode[j]==0)
			{			
				for(i=(j*4);i<((j+1)*4);i++)
				{
					if((i==0)&&(j==0))
					{
						ttv_comp_ptr->blkwork_buf[1][1][i] = ttv_comp_ptr->diff[1][1][i] + ttv_comp_ptr->blkwork_buf[1][0][i] ;
						ttv_comp_ptr->blkwork_buf[2][1][i] = ttv_comp_ptr->diff[2][1][i] + ttv_comp_ptr->blkwork_buf[2][0][i] ;
					}
					else
					{
						ttv_comp_ptr->blkwork_buf[1][1][i] = ttv_comp_ptr->diff[1][1][i] + ttv_comp_ptr->blkwork_buf[1][1][i-1] ;	
						ttv_comp_ptr->blkwork_buf[2][1][i] = ttv_comp_ptr->diff[2][1][i] + ttv_comp_ptr->blkwork_buf[2][1][i-1] ;	
					}

				}
			}
			else
			{
				for(i=(j*4);i<((j+1)*4);i++)
				{
					ttv_comp_ptr->blkwork_buf[1][1][i] = ttv_comp_ptr->diff[1][1][i] + ttv_comp_ptr->blkwork_buf[1][0][i] ;
					ttv_comp_ptr->blkwork_buf[2][1][i] = ttv_comp_ptr->diff[2][1][i] + ttv_comp_ptr->blkwork_buf[2][0][i] ;
				}
			}
		}

}

void Diff_Trans_Dec (TTV_COMP_T * ttv_comp_ptr,int sample)
{
	int i;
	if(sample==0)
	{
		for(i=0;i<64;i++)
		{
			 ttv_comp_ptr->diff[sample][0][i] = ttv_comp_ptr->code_diff[i];

		}

		for(i=64;i<128;i++)
		{
			ttv_comp_ptr->diff[sample][1][i-64] = ttv_comp_ptr->code_diff[i];
		}
	}
	else
	{

		for(i=0;i<32;i++)
		{
			 ttv_comp_ptr->diff[1][0][i] = ttv_comp_ptr->code_diff[2*i];
			 ttv_comp_ptr->diff[2][0][i] =ttv_comp_ptr->code_diff[2*i+1];

		}

		for(i=32;i<64;i++)
		{
			 ttv_comp_ptr->diff[1][1][i-32] = ttv_comp_ptr->code_diff[2*i];
			 ttv_comp_ptr->diff[2][1][i-32] =ttv_comp_ptr->code_diff[2*i+1];
		}
	}

}


void ErrorOutput (TTV_COMP_T * ttv_comp_ptr,int sample,int segment)
{
	int i;
	int width;

	if(sample==0)
		width=segment;
	else
		width=segment/2;



	if(sample==0)
	{
		for(i=0;i<width;i++)
		{
			 ttv_comp_ptr->blkwork_buf[sample][0][i]   =128;

		}

		for(i=width;i<(2*width);i++)
		{
			 ttv_comp_ptr->blkwork_buf[sample][1][i-width]=128 ;
		}

	}
	else
	{
		for(i=0;i<width;i++)
		{
			ttv_comp_ptr->blkwork_buf[1][0][i] = 128;
			ttv_comp_ptr->blkwork_buf[2][0][i] = 128;

		}

		for(i=width;i<(2*width);i++)
		{
			ttv_comp_ptr->blkwork_buf[1][1][i-width] = 128;
			ttv_comp_ptr->blkwork_buf[2][1][i-width] = 128;

		}

	}

}

void OriData_Dec (TTV_COMP_T * ttv_comp_ptr,int sample,int segment)
{
	int i;
	int width;

	if(sample==0)
		width=segment;
	else
		width=segment/2;



	for(i=0;i<(2*segment);i++)
	{
		if(ttv_comp_ptr->bit_depth==8)
			ttv_comp_ptr->code_ori[i]=BitStreamReadBits (8);
		else
			ttv_comp_ptr->code_ori[i]=BitStreamReadBits (10);
	}

#if DEC_DIF_PROBE
		//pr_len=(2*segment)/4;
		//for(i=0;i<pr_len;i++)
		//{
		//	fprintf(dec_dif_pb,"%03x%03x%03x%03x%03x%03x%03x%03x\n",0,0,0,0,0,0,0,0);
		//}

#endif

	if(sample==0)
	{
		for(i=0;i<width;i++)
		{
			 ttv_comp_ptr->blkwork_buf[sample][0][i]   =ttv_comp_ptr->code_ori[i];

		}

		for(i=width;i<(2*width);i++)
		{
			 ttv_comp_ptr->blkwork_buf[sample][1][i-width]=ttv_comp_ptr->code_ori[i] ;
		}

	}
	else
	{
		for(i=0;i<width;i++)
		{
			ttv_comp_ptr->blkwork_buf[1][0][i] = ttv_comp_ptr->code_ori[2*i] ;
			ttv_comp_ptr->blkwork_buf[2][0][i] = ttv_comp_ptr->code_ori[2*i+1];

		}

		for(i=width;i<(2*width);i++)
		{
			ttv_comp_ptr->blkwork_buf[1][1][i-width] = ttv_comp_ptr->code_ori[2*i];
			ttv_comp_ptr->blkwork_buf[2][1][i-width] = ttv_comp_ptr->code_ori[2*i+1];

		}

	}

}

//sample:0表示Y分量,1表示UV分量
//frame0:Y分量或者V分量
//frame1:无用或者U分量

int DecoderOneFrame (OTT_CFG * ott_cfg,int sample, uint8 * bs_buf_ptr, uint16 * bs_head_ptr,uint16 * frame0, uint16 * frame1)
{
	int mb_x;
	int mb_y;

	uint16 * head_ptr_tmp;
	int temp;

	int bit128_align;
	int code_width;
	int edge;
	int suc;
	TTV_COMP_T * ttv_dec_ptr;

	ott_cfg->dec_sum_bit=0;

#if DEC_BS_PROBE
		uint32 sum_128bit=0;
		dec_bs_pb=fopen("bs_probe_dec.log","wb");
#endif
#if DEC_HEAD_PROBE
		dec_head_pb=fopen("head_probe_dec.log","wb");
#endif
#if DEC_BP_PROBE	
		dec_bp_pb=fopen("bp_probe_dec.log","wb");
#endif
#if DEC_DIF_PROBE	
		dec_dif_pb=fopen("dif_probe_dec.log","wb");
#endif
#if DEC_SIGN_PROBE
		dec_sign_pb=fopen("dec_sign_pb.log","wb");
#endif

#if DEC_BP_CODE_PROBE
		dec_bp_code_pb=fopen("bp_code_probe_dec.log","wb");
#endif
		head_ptr_tmp=bs_head_ptr;
		
		ttv_dec_ptr = (TTV_COMP_T *)DISP_MALLOC(sizeof(TTV_COMP_T));		
        if (HI_NULL == ttv_dec_ptr)
        {
            goto TTV_DEC_PTR_NULL_ERR_PROCESS;
        }
		
		CmpInit (ttv_dec_ptr, ott_cfg);
		
		bit_stream_dec = (BIT_STREAM_DEC *)DISP_MALLOC(sizeof(BIT_STREAM_DEC));		
        if (HI_NULL == bit_stream_dec)
        {
            goto BIT_STREAM_DEC_ERR_PROCESS;
        }
		
		BitStreamInitDec (bit_stream_dec, bs_buf_ptr, 0);
		ttv_dec_ptr->error=0;

		if(sample==0)
		{
			ttv_dec_ptr->mb_num_y  = ttv_dec_ptr->org_frm_height;
			ttv_dec_ptr->mb_num_x  = ((ttv_dec_ptr->org_frm_width + 63) >> 6);	
		}
		else
		{
			ttv_dec_ptr->mb_num_y  = ttv_dec_ptr->org_frm_height/2;
			ttv_dec_ptr->mb_num_x  = ((ttv_dec_ptr->org_frm_width + 63) >> 6);	
		}

		for (mb_y = 0; mb_y <ttv_dec_ptr->mb_num_y; mb_y=mb_y+2)
		{
			for (mb_x = 0; mb_x < ttv_dec_ptr->mb_num_x ; mb_x++)
			{
				ttv_dec_ptr->mb_y = mb_y;
				ttv_dec_ptr->mb_x = mb_x;
				bit_stream_dec->cur_mb_bit = 0;

				//printf("mb_y=%d, mb_x=%d, *head_ptr_tmp=0x%x\n", mb_y, mb_x, *head_ptr_tmp);

				if(ttv_dec_ptr->org_frm_width%64!=0)
				{
					if(mb_x==(ttv_dec_ptr->mb_num_x-1))
					{
						edge=1;
						code_width=ttv_dec_ptr->org_frm_width%64;
					}
					else
					{
						edge=0;
						code_width=64;
					}
				}
				else
				{
					edge=0;
					code_width=64;
				}


				if(ttv_dec_ptr->raw )
				{
					OriData_Dec(ttv_dec_ptr,sample,code_width);	
				}
				else if(ttv_dec_ptr->error==0)
				{
			
					MBHeaderDec_BP (ttv_dec_ptr,sample,head_ptr_tmp);	
#if DEC_BS_PROBE
		sum_128bit=sum_128bit+(ttv_dec_ptr->bpye_num+1);
#endif
					MBInit (ttv_dec_ptr);
	
					if(ttv_dec_ptr->code_mode==1)
					{
						suc=BitPlaneDecoding (ttv_dec_ptr,sample);	
						if(suc==1)
						{
							if(sample==0)
							{
								Diff_Trans_Dec (ttv_dec_ptr, sample);
								Pred_BitPlane_Dec_Y (ttv_dec_ptr);
							}
							else
							{
								Diff_Trans_Dec (ttv_dec_ptr, sample);
								Pred_BitPlane_Dec_UV (ttv_dec_ptr);
							}
						}
#if ERROR_REPORT
						if(ttv_dec_ptr->error!=0)
						{
							ErrorOutput(ttv_dec_ptr,sample,code_width);	
						}
#endif
					}
					else
					{
						OriData_Dec(ttv_dec_ptr,sample,code_width);		
					}
				}
#if ERROR_REPORT
				else if(ttv_dec_ptr->error!=0)
				{
					ErrorOutput(ttv_dec_ptr,sample,code_width);	
				}
#endif
				if((bit_stream_dec->cur_mb_bit%ALIGN_BIT)!=0)
				{
					bit128_align=ALIGN_BIT-(bit_stream_dec->cur_mb_bit%ALIGN_BIT);
					temp=BitStreamReadBits (bit128_align);
				}

				ott_cfg->dec_sum_bit=((bit_stream_dec->cur_mb_bit+ALIGN_BIT-1)/ALIGN_BIT)*ALIGN_BIT + ott_cfg->dec_sum_bit;

				PutRecMB (ttv_dec_ptr, mb_y, mb_x,sample,code_width,frame0,frame1);	
				PutRecMB (ttv_dec_ptr, mb_y+1, mb_x,sample,code_width,frame0,frame1);
				head_ptr_tmp++;
			}
			
		}
#if DEC_BS_PROBE
					for(i=0;i<sum_128bit;i++)
					{
						for(j=0;j<16;j++)
						{
							fprintf(dec_bs_pb,"%03x",(*(bs_buf_ptr+i*16+(15-j))));
						}
						fprintf(dec_bs_pb,"\n");
					}

#endif		

		CmpClose(ttv_dec_ptr);

#if DEC_BS_PROBE
		fclose(dec_bs_pb);
#endif
#if DEC_HEAD_PROBE
		fclose(dec_head_pb);
#endif
#if DEC_BP_PROBE	
		fclose(dec_bp_pb);
#endif
#if DEC_DIF_PROBE	
		fclose(dec_dif_pb);
#endif
#if DEC_SIGN_PROBE
		fclose(dec_sign_pb);
#endif
		
	return HI_SUCCESS;

BIT_STREAM_DEC_ERR_PROCESS:
    DISP_FREE(ttv_dec_ptr);
    
TTV_DEC_PTR_NULL_ERR_PROCESS:
#if DEC_BS_PROBE
		fclose(dec_bs_pb);
#endif
#if DEC_HEAD_PROBE
		fclose(dec_head_pb);
#endif
#if DEC_BP_PROBE	
		fclose(dec_bp_pb);
#endif
#if DEC_DIF_PROBE	
		fclose(dec_dif_pb);
#endif
#if DEC_SIGN_PROBE
		fclose(dec_sign_pb);
#endif
    return HI_FAILURE;
}
