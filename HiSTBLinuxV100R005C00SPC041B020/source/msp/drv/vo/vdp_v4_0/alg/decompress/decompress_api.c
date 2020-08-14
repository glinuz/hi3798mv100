#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "decompress_api.h"
#include "drv_disp_debug.h"

#define DEF_ALG_DECOMPRESS_DBG 0

const HI_U8 fill_zero_256byte[256]={0};
HI_S32 ALG_Decompress_ArrangeOriData(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_U8 *pDstBufAddr)
{
	HI_U8* pu8SrcYAddr;
	HI_U8* pu8SrcCAddr;
	SMMU_BUFFER_S stSrcBuf;
	HI_U32 u32FrmHgt;
	HI_U32 u32FrmWidth;
	HI_U32 u32FrmSlotHgt;
	HI_U32 u32FrmSlotWidth;
	HI_U32 u32FrmStride;
	HI_U32 u32FrmHeadStride;
	HI_U32 u32FrmHeadSize;
	HI_U8* pu8CurWtAddr;
	HI_U32 u32CurWtSize;

	stSrcBuf.u32StartSmmuAddr = pstFrame->stBufAddr[0].u32PhyAddr_YHead;
	stSrcBuf.u32Size = (pstFrame->stBufAddr[0].u32PhyAddr_CHead - pstFrame->stBufAddr[0].u32PhyAddr_YHead) * 3 / 2;
	if (HI_SUCCESS != HI_DRV_SMMU_Map(&stSrcBuf))
	{
		WIN_ERROR("Map Src Buffer Failed\n PhyAddr=%#x Size=%d\n",
				stSrcBuf.u32StartSmmuAddr,
				stSrcBuf.u32Size);
		return HI_FAILURE;
	}

	pu8SrcYAddr = stSrcBuf.pu8StartVirAddr;
	pu8SrcCAddr = pu8SrcYAddr + (pstFrame->stBufAddr[0].u32PhyAddr_CHead - pstFrame->stBufAddr[0].u32PhyAddr_YHead);

	u32FrmHgt = pstFrame->u32Height;
	u32FrmWidth = pstFrame->u32Width;
	u32FrmSlotHgt = pstFrame->u32Height;
	u32FrmSlotWidth =  pstFrame->stBufAddr[0].u32Stride_Y;
	u32FrmStride = pstFrame->stBufAddr[0].u32Stride_Y * 16;
	u32FrmHeadStride = pstFrame->stBufAddr[0].u32Head_Stride;
	u32FrmHeadSize = pstFrame->stBufAddr[0].u32Head_Size;

	pu8CurWtAddr = pDstBufAddr;

	*(HI_U16 *)(pu8CurWtAddr + 0) = u32FrmWidth;
	*(HI_U16 *)(pu8CurWtAddr + 2) = u32FrmHgt;
	*(HI_U16 *)(pu8CurWtAddr + 4) = u32FrmSlotWidth;
	*(HI_U16 *)(pu8CurWtAddr + 6) = u32FrmSlotHgt;
	*(HI_U32 *)(pu8CurWtAddr + 8) = u32FrmStride;
	*(HI_U8 *)(pu8CurWtAddr + 12) = 0;
	*(HI_U8 *)(pu8CurWtAddr + 13) = 1;
	*(HI_U8 *)(pu8CurWtAddr + 14) = 8;
	*(HI_U8 *)(pu8CurWtAddr + 15) = 8;
	*(HI_U8 *)(pu8CurWtAddr + 16) = 1;
	*(HI_U32 *)(pu8CurWtAddr + 20) = u32FrmHeadStride;
	*(HI_U32 *)(pu8CurWtAddr + 24) = u32FrmHeadSize;

	pu8CurWtAddr = pDstBufAddr + 32;
	u32CurWtSize = u32FrmHeadSize + (u32FrmStride / 16) * (((u32FrmHgt + 63) / 64) * 64);
	memcpy(pu8CurWtAddr,pu8SrcYAddr,u32CurWtSize);

	pu8CurWtAddr = pu8CurWtAddr + u32CurWtSize;
	u32CurWtSize = u32FrmHeadSize / 2 + (u32FrmStride / 16) * (((u32FrmHgt + 63) / 64) * 64) / 2;
	memcpy(pu8CurWtAddr,pu8SrcCAddr,u32CurWtSize);

	HI_DRV_SMMU_Unmap(&stSrcBuf);

	return HI_SUCCESS;    
}

HI_S32 ALG_Decompress_GetY16ByteData(HI_ORI_DATA_INFO_S *pstOriDataInfo,
                                        HI_U8 *pOriDateAddr,
                                        HI_U16 *p_decdata_y16)
{
	HI_S32 row_map_table_y[2][4][16] = 
	{{//偶数tile
		 //{0,4,8,12,  1,5,9,13},//第0列
		 {0,4,1,5,  2,6,3,7},
		 //{1,5,9,13,  8,12,0,4},//第1列
		 {6,0,7,1,  4,2,5,3},
		 //{8,12,0,4,  9,13,1,5},//第2列
		 {2,6,3,7,  0,4,1,5},
		 //{9,13,1,5,  0,4,8,12} //第3列
		 {4,2,5,3,  6,0,7,1}
	 },
	{//奇数tile
		//{1,5,9,13,  0,4,8,12},
		{4,0,5,1,  6,2,7,3},
		//{0,4,8,12,  9,13,1,5},
		{0,6,1,7,  2,4,3,5},
		//{9,13,1,5,  8,12,0,4},
		{6,2,7,3,  4,0,5,1},
		//{8,12,0,4,  1,5,9,13}
		{2,4,3,5,  0,6,1,7}
	}};

	HI_S32 width,height,stride;
	HI_S32 slot_width, slot_height;
	HI_U8 structure, chroma_idc;
	HI_U8 bit_depth, compress,yuv_bit_depth;
	HI_S32 i,j;
	HI_U8 *p_headdata_tmp,*p_compdata_tmp;
	HI_U8 *p_headdata_y,*p_compdata_y;
	HI_U32 head_inf_size,head_stride,y_plane_size;

	static OTT_CFG s_ott_cfg;
	static OTT_CFG *ps_ott_cfg;

	HI_U32 srcIdx,dstIdx;
	HI_U8 *src,*dst;
	HI_U32 segLen;
	HI_U32 segAll = 0;
	HI_U16 code,hpCnt;
	HI_U16 *pu16;
	HI_U16 u16;
	S_HEAD_INFO *pHead;
	HI_U32 tileX,tileY;

	ps_ott_cfg = &s_ott_cfg;

	width			= pstOriDataInfo->width;
	height			= pstOriDataInfo->height;
	slot_width		= pstOriDataInfo->slot_width;
	slot_height		= pstOriDataInfo->slot_height;
	stride			= pstOriDataInfo->stride;
	structure		= pstOriDataInfo->structure;
	chroma_idc		= pstOriDataInfo->chroma_idc;
	yuv_bit_depth	= pstOriDataInfo->yuv_bit_depth;
	bit_depth		= pstOriDataInfo->bit_depth;
	compress		= pstOriDataInfo->compress;

	head_stride = ((((width + 511)/512)*512)/64)*2;
	head_inf_size = (height+63)/64*32 * head_stride;
	y_plane_size =  ((stride/16)*((height+63)/64)*64);

	p_headdata_y = DISP_MALLOC(4096*4096*sizeof(HI_U8));
	if (p_headdata_y == HI_NULL )
	{
		WIN_ERROR("alloc p_headdata_y failed\n");
        return HI_FAILURE;
	}

	p_compdata_y = DISP_MALLOC(4096*4096*sizeof(HI_U8));
	if (p_compdata_y == HI_NULL )
	{
		WIN_ERROR("alloc p_compdata_y failed\n");
	    DISP_FREE(p_headdata_y);
		return HI_FAILURE;
	}

	p_headdata_tmp = DISP_MALLOC(head_inf_size);
	if (!p_headdata_tmp)
	{
		WIN_ERROR("p_headdata_tmp=0x%p\n", p_headdata_tmp);
		DISP_FREE(p_headdata_y);
		DISP_FREE(p_compdata_y);
		return HI_FAILURE;
	}
	p_compdata_tmp = DISP_MALLOC(y_plane_size);
	if (!p_compdata_tmp)
	{
		WIN_ERROR("p_compdata_tmp=0x%p\n", p_compdata_tmp);
		DISP_FREE(p_headdata_y);
		DISP_FREE(p_compdata_y);
		DISP_FREE(p_headdata_tmp);
		return HI_FAILURE;
	}

	memcpy(p_headdata_tmp,pOriDateAddr+32,head_inf_size);
	memcpy(p_compdata_tmp,pOriDateAddr+32+head_inf_size,y_plane_size);

	segAll = 0;
	for(i=0; i<height; i+=2)
	{
		for(j=0; j<width; j+=64)
		{
			//head info
			HI_U32 head_num_height = (((height+63)/64)*64)/2;
			srcIdx = i/2;
			dstIdx = (srcIdx%2)*(head_num_height/2) + (srcIdx+0)/2;
			memcpy(p_headdata_y+((i/2)*((width+63)/64)*2+(j/64)*2), p_headdata_tmp+((head_stride*dstIdx)+(j/64)*2), 2);

			code = *(HI_U16*)(p_headdata_y+((i/2)*((width+63)/64)*2+(j/64)*2));
			hpCnt = (code>>1)&0x0f;
			segLen = (hpCnt+1)*128/8;

			//comp data
			dst = p_compdata_y + segAll;
			tileX = (i/16)%2;
			tileY = (j/64)%4;
			srcIdx = (i/2)%8;
			dstIdx = row_map_table_y[tileX][tileY][srcIdx];
			src = p_compdata_tmp + stride*((i/2)/8) + ((dstIdx)*64*2 + (j/64)*64*16);

			if(((width%64)==0) || (j!=((width/64)*64)))
			{
				if((structure==0) || (structure==3))
				{
					memcpy(dst, src, segLen);
				}
				else
				{
					if(((structure==1)&&(i%4==0)) || ((structure==2)&&(i%4==2)))
					{
						memcpy(dst, src, segLen);
					}
					else
					{
						pu16 = (HI_U16*)(p_headdata_y+((i/2)*((width+63)/64)*2+(j/64)*2));
						//hpCnt = (64*2*(bit_depth>8?10:8)+127)/128-1;
						hpCnt = (64*2*8+127)/128-1;

						u16 = 0;
						pHead = (S_HEAD_INFO*)(&u16);
						pHead->cmp_flag = 0x0;	//不压缩
						pHead->num_pack = hpCnt;
						pHead->sample   = 0;
						pHead->pre_mode = 0;
						*pu16 = u16;

						//segLen = 64*2*(bit_depth>8?10:8)/8;
						segLen = 64*2*8/8;
						memcpy(dst, fill_zero_256byte, segLen);
					}
				}
			}
			else	//边界压缩bypass处理
			{
				pu16 = (HI_U16*)(p_headdata_y+((i/2)*((width+63)/64)*2+(j/64)*2));
				//hpCnt = ((width-j)*2*(bit_depth>8?10:8)+127)/128-1;
				hpCnt = ((width-j)*2*8+127)/128-1;

				u16 = 0;
				pHead = (S_HEAD_INFO*)(&u16);
				pHead->cmp_flag = 0x0;	//不压缩
				pHead->num_pack = hpCnt;
				pHead->sample   = 0;
				pHead->pre_mode = 0;
				*pu16 = u16;

				//segLen = (width-j)*(bit_depth>8?10:8)/8;
				segLen = (width-j)*8/8;
				memcpy(dst, src, segLen);
				//memcpy(dst+segLen, src+64*(bit_depth>8?10:8)/8, segLen);
				memcpy(dst+segLen, src+64*8/8, segLen);

				segLen *= 2;
				segLen = (segLen+15)/16*16;	//SEG数据长度对齐到128
			}
			segAll += segLen;
		}
	}

	ps_ott_cfg->raw = 0;
	//ott_cfg->bit_depth = bit_depth;
	ps_ott_cfg->bit_depth = 8;
	ps_ott_cfg->is_simple_vlc = 1;
	ps_ott_cfg->is_segment_raw = 1;
	ps_ott_cfg->org_frm_width = width;
	ps_ott_cfg->org_frm_height = height;
	DecoderOneFrame (ps_ott_cfg, 0, p_compdata_y, (HI_U16*)p_headdata_y, p_decdata_y16, NULL);

	DISP_FREE(p_headdata_y);
	DISP_FREE(p_compdata_y);
	DISP_FREE(p_headdata_tmp);
	DISP_FREE(p_compdata_tmp);

	return HI_SUCCESS;

}

HI_S32 ALG_Decompress_GetUV16ByteData(HI_ORI_DATA_INFO_S *pstOriDataInfo,
                                        HI_U8 *pOriDateAddr,
                                        HI_U16 *p_decdata_uv16)
{
	//按照0,1,  4,5顺序查找索引
	HI_S32 row_map_table_uv[2][4][8] = 
	{{
		 //{0,4,  1,5},
		 {0,2,  1,3},
		 //{1,5,  0,4},
		 {2,0,  3,1},
		 //{1,5,  0,4},
		 {2,0,  3,1},
		 //{0,4,  1,5},
		 {0,2,  1,3},
	 },
	{
		//{1,5,  0,4},
		{2,0,  3,1},
		//{0,4,  1,5},
		{0,2,  1,3},
		//{0,4,  1,5},
		{0,2,  1,3},
		//{1,5,  0,4}
		{2,0,  3,1}
	}};
	HI_S32 width,height,stride;
	HI_S32 slot_width, slot_height;
	HI_U8 structure, chroma_idc;
	HI_U8 bit_depth, compress,yuv_bit_depth;
	HI_S32 i,j;
	HI_U8 *p_headdata_tmp;
	HI_U8 *p_compdata_tmp;
	HI_U8 *p_headdata_uv;
	HI_U8 *p_compdata_uv;
	HI_U32 head_inf_size,head_stride;
	HI_U32 y_plane_size;

	static OTT_CFG s_ott_cfg;
	static OTT_CFG *ps_ott_cfg;

	HI_U32 srcIdx,dstIdx;
	HI_U8 *src,*dst;
	HI_U32 segLen;
	HI_U32 segAll = 0;
	HI_U16 code,hpCnt;
	HI_U16 *pu16;
	HI_U16 u16;
	S_HEAD_INFO *pHead;
	HI_U32 tileX,tileY;

	ps_ott_cfg = &s_ott_cfg;


	width			= pstOriDataInfo->width;
	height			= pstOriDataInfo->height;
	slot_width		= pstOriDataInfo->slot_width;
	slot_height		= pstOriDataInfo->slot_height;
	stride			= pstOriDataInfo->stride;
	structure		= pstOriDataInfo->structure;
	chroma_idc		= pstOriDataInfo->chroma_idc;
	yuv_bit_depth	= pstOriDataInfo->yuv_bit_depth;
	bit_depth		= pstOriDataInfo->bit_depth;
	compress		= pstOriDataInfo->compress;

	head_stride = ((((width + 511)/512)*512)/64)*2;
	head_inf_size = (height+63)/64*32 * head_stride;

	y_plane_size =  ((stride/16)*((height+63)/64)*64);
	p_headdata_uv = DISP_MALLOC(4096*4096*sizeof(HI_U8));
	if (!p_headdata_uv)
	{
		WIN_ERROR("p_headdata_uv=0x%p\n", p_headdata_uv);
		return HI_FAILURE;
	}

	p_compdata_uv = DISP_MALLOC(4096*4096*sizeof(HI_U8)); 
	if (!p_compdata_uv)
	{
		DISP_FREE(p_headdata_uv);
		WIN_ERROR("p_compdata_uv=0x%p\n", p_compdata_uv);
		return HI_FAILURE;
	}
	
	p_headdata_tmp = DISP_MALLOC(head_inf_size);
	if (!p_headdata_tmp)
	{
		DISP_FREE(p_headdata_uv);
		DISP_FREE(p_compdata_uv);
		WIN_ERROR("p_headdata_tmp=0x%p\n", p_headdata_tmp);
		return HI_FAILURE;
	}

	p_compdata_tmp = DISP_MALLOC(y_plane_size);
	if (!p_compdata_tmp)
	{
		DISP_FREE(p_headdata_uv);
		DISP_FREE(p_compdata_uv);
		DISP_FREE(p_headdata_tmp);
		WIN_ERROR("p_compdata_tmp=0x%p\n", p_compdata_tmp);
		return HI_FAILURE;
	}

	memcpy(p_headdata_tmp,
			pOriDateAddr+head_inf_size+y_plane_size+32,
			head_inf_size/2);
	memcpy(p_compdata_tmp,
			pOriDateAddr+head_inf_size*3/2+y_plane_size+32,
			y_plane_size/2);
    
	segAll = 0;
	for(i=0; i<height/2; i+=2)
	{
		for(j=0; j<width; j+=64)
		{
			//head info
			HI_U32 head_num_height = (((height+63)/64)*64)/2/2;
			srcIdx = i/2;
			dstIdx = (srcIdx%2)*(head_num_height/2) + (srcIdx+0)/2;
			memcpy(p_headdata_uv+((i/2)*((width+63)/64)*2+(j/64)*2), p_headdata_tmp+head_stride*dstIdx+(j/64)*2, 2);

			code = *(HI_U16*)(p_headdata_uv+((i/2)*((width+63)/64)*2+(j/64)*2));
			hpCnt = (code>>1)&0x0f;
			segLen = (hpCnt+1)*128/8;

			//comp data
			dst = p_compdata_uv + segAll;
			tileX = (i/8)%2;
			tileY = (j/64)%4;
			srcIdx = (i/2)%4;
			dstIdx = row_map_table_uv[tileX][tileY][srcIdx];
			src = p_compdata_tmp + (stride/2)*((i/2)/4) + ((dstIdx)*64*2 + (j/64)*64*8);

			if(((width%64)==0) || (j!=((width/64)*64)))
			{
				if((structure==0) || (structure==3))
				{
					memcpy(dst, src, segLen);
				}
				else
				{
					//获取当前场压缩数据
					if(((structure==1)&&(i%4==0)) || ((structure==2)&&(i%4==2)))
					{
						memcpy(dst, src, segLen);
					}
					//非当期场压缩数据bypass处理
					else
					{
						pu16 = (HI_U16*)(p_headdata_uv+((i/2)*((width+63)/64)*2+(j/64)*2));
						//hpCnt = (64*2*(bit_depth>8?10:8)+127)/128-1;
						hpCnt = (64*2*8+127)/128-1;

						u16 = 0;
						pHead = (S_HEAD_INFO*)(&u16);
						pHead->cmp_flag = 0x0;	//不压缩
						pHead->num_pack = hpCnt;
						pHead->sample   = 0;
						pHead->pre_mode = 0;
						*pu16 = u16;

						//segLen = 64*2*(bit_depth>8?10:8)/8;
						segLen = 64*2*8/8;
						memcpy(dst, fill_zero_256byte, segLen);
					}
				}
			}
			else	//边界压缩bypass处理
			{
				pu16 = (HI_U16*)(p_headdata_uv+((i/2)*((width+63)/64)*2+(j/64)*2));
				//hpCnt = ((width-j)*2*(bit_depth>8?10:8)+127)/128-1;
				hpCnt = ((width-j)*2*8+127)/128-1;

				u16 = 0;
				pHead = (S_HEAD_INFO*)(&u16);
				pHead->cmp_flag = 0;	//不压缩
				pHead->num_pack = hpCnt;
				pHead->sample   = 0;
				pHead->pre_mode = 0x0;
				*pu16 = u16;

				//segLen = (width-j)*(bit_depth>8?10:8)/8;
				segLen = (width-j)*8/8;
				memcpy(dst, src, segLen);
				//memcpy(dst+segLen, src+64*(bit_depth>8?10:8)/8, segLen);
				memcpy(dst+segLen, src+64*8/8, segLen);

				segLen *= 2;
				segLen = (segLen+15)/16*16;	//SEG数据长度对齐到128
			}

			segAll += segLen;
		}
	}


	ps_ott_cfg->raw = 0;
	ps_ott_cfg->bit_depth = 8;
	ps_ott_cfg->is_simple_vlc = 1;
	ps_ott_cfg->is_segment_raw = 1;
	ps_ott_cfg->org_frm_width = width;
	ps_ott_cfg->org_frm_height = height;//配成height/2时UV都只有前半部分，与上个RM版本差异
	DecoderOneFrame (ps_ott_cfg, 1, p_compdata_uv, (uint16*)p_headdata_uv, (uint16*)(p_decdata_uv16+width*height/4), (uint16*)p_decdata_uv16);

	DISP_FREE(p_headdata_uv);
	DISP_FREE(p_compdata_uv);
	DISP_FREE(p_headdata_tmp);
	DISP_FREE(p_compdata_tmp);

	return HI_SUCCESS;
}

HI_VOID ALG_Decompress_Get16ByteData(HI_ORI_DATA_INFO_S *pstOriDataInfo,
                                        HI_U16 *p_decdata_y16,
                                        HI_U16 *p_decdata_uv16,
                                        HI_U16 *p_decdata_16b)
{

	HI_S32 width,height,stride;
	HI_S32 slot_width, slot_height;
	HI_U8 structure, chroma_idc;
	HI_U8 bit_depth, compress,yuv_bit_depth;
	HI_U16 *pu16bY,*pu16bU,*pu16bV;
	HI_U32 i;

	width			= pstOriDataInfo->width;
	height			= pstOriDataInfo->height;
	slot_width		= pstOriDataInfo->slot_width;
	slot_height		= pstOriDataInfo->slot_height;
	stride			= pstOriDataInfo->stride;
	structure		= pstOriDataInfo->structure;
	chroma_idc		= pstOriDataInfo->chroma_idc;
	yuv_bit_depth	= pstOriDataInfo->yuv_bit_depth;
	bit_depth		= pstOriDataInfo->bit_depth;
	compress		= pstOriDataInfo->compress;

	pu16bY = p_decdata_16b;
	for(i=0; i<height; i++)
	{
		HI_S32 m = 0;
		HI_U32 srcIdx = i%4;
		if(srcIdx==0 || srcIdx==3)
		{
			m = i;
		}
		else if(srcIdx==1)
		{
			m = i+1;
		}
		else if(srcIdx==2)
		{
			//m = i-1;
			m = i;
            m = m - 1;
		}
		memcpy(pu16bY+i*width, p_decdata_y16+m*width, width*2);
	}

	pu16bU = p_decdata_16b+width*height;
	pu16bV = p_decdata_16b+width*height+width*height/4;
	for(i=0; i<height/2; i++)
	{
		HI_S32 m = 0;
		HI_U32 srcIdx = i%4;
		if(srcIdx==0 || srcIdx==3)
		{
			m = i;
		}
		else if(srcIdx==1)
		{
			m = i+1;
		}
		else if(srcIdx==2)
		{
			//m = i-1;
			m = i;
            m = m - 1;
		}
		memcpy(pu16bU+i*width/2, p_decdata_uv16+m*width/2, width/2*2);
		memcpy(pu16bV+i*width/2, p_decdata_uv16+width*height/4+m*width/2, width/2*2);
	}
	return;                
}

HI_VOID ALG_Decompress_Get8ByteData(HI_ORI_DATA_INFO_S *pstOriDataInfo,
                                    HI_U16 *p_decdata_16b,
                                    HI_U8 *p_decdata_8b)
{
	HI_S32 width,height;
	HI_U32 i,j;
	HI_S32 YUVSize;
	HI_U8 *pu8TmpU;
	HI_U8 *pu8TmpV;
	HI_U8 *ptr;
#if DEF_ALG_DECOMPRESS_DBG	
    struct file *pfYUV;
#endif	    
	width			= pstOriDataInfo->width;
	height			= pstOriDataInfo->height;


	YUVSize = width*height*3/2;
	
    pu8TmpU = DISP_MALLOC(YUVSize/3/2);	
    if(!pu8TmpU)
    {
        WIN_ERROR("malloc TmpUV failed\n");
        return;
    }

    pu8TmpV = DISP_MALLOC(YUVSize/3/2);	
    if(!pu8TmpV)
    {
        DISP_FREE(pu8TmpU);
        WIN_ERROR("malloc TmpUV failed\n");
        return;
    }
    
	for(i=0; i<YUVSize; i++)
	{
		p_decdata_8b[i] = (HI_U8)(p_decdata_16b[i]);
	}
	
	memcpy(pu8TmpU,p_decdata_8b+width*height,YUVSize/3/2);
	memcpy(pu8TmpV,p_decdata_8b+width*height*5/4,YUVSize/3/2);

	ptr = p_decdata_8b+width*height;
	
	for (i=0; i<height/2; i++)
    {
        for (j=0; j<width/2; j++)
        {
            ptr[2*j] = pu8TmpV[i*width/2+j];
            ptr[2*j+1] = pu8TmpU[i*width/2+j];
        }
        ptr += width;
    }
#if DEF_ALG_DECOMPRESS_DBG	
	WIN_ERROR("8byte:%#x %#x %#x %#x %#x %#x %#x %#x\n",
	    p_decdata_8b[0],
	    p_decdata_8b[1],
	    p_decdata_8b[2],
	    p_decdata_8b[3],
	    p_decdata_8b[155],
	    p_decdata_8b[156],
	    p_decdata_8b[157],
	    p_decdata_8b[158]);
	    
    pfYUV = vdp_k_fopen("cap.yuv", O_RDWR|O_CREAT|O_APPEND, 0);

    if (pfYUV)
    {
        vdp_k_fwrite(p_decdata_8b, YUVSize, pfYUV);

        vdp_k_fclose(pfYUV);
    }
    else
    {
        WIN_ERROR("error\n");
    }
     WIN_ERROR("done\n");	    
#endif	    
    DISP_FREE(pu8TmpU);
    DISP_FREE(pu8TmpV);
	return; 
}

HI_VOID ALG_Decompress_GetOriDataInfo(HI_U8 *pOriDateAddr,
                                    HI_ORI_DATA_INFO_S *pstOriDataInfo)
{
	pstOriDataInfo->width			= *(HI_U16*)(pOriDateAddr+0);
	pstOriDataInfo->height			= *(HI_U16*)(pOriDateAddr+2);
	pstOriDataInfo->slot_width		= *(HI_U16*)(pOriDateAddr+4);
	pstOriDataInfo->slot_height		= *(HI_U16*)(pOriDateAddr+6);
	pstOriDataInfo->stride			= *(HI_U32*)(pOriDateAddr+8);
	pstOriDataInfo->structure		= *(HI_U8*)(pOriDateAddr+12);
	pstOriDataInfo->chroma_idc		= *(HI_U8*)(pOriDateAddr+13);
	pstOriDataInfo->yuv_bit_depth	= *(HI_U8*)(pOriDateAddr+14);
	pstOriDataInfo->bit_depth		= *(HI_U8*)(pOriDateAddr+15);
	pstOriDataInfo->compress		= *(HI_U8*)(pOriDateAddr+16);  

	return;
}

HI_S32 ALG_Decompress_Process(HI_U8 *pOriDateAddr,HI_U8 *pDstDateAddr,HI_U32 u32DstBufSize)
{
	HI_U16 *p_decdata_y16;
	HI_U16 *p_decdata_uv16;
	HI_U16 *p_decdata_16b;
	HI_U8 *p_decdata_8b;
	HI_ORI_DATA_INFO_S stOriDataInfo;
    HI_S32 s32Ret=HI_SUCCESS;
	p_decdata_y16 = DISP_MALLOC(4096*4096*sizeof(HI_U16));
	p_decdata_uv16 = DISP_MALLOC(4096*4096*sizeof(HI_U16));
	p_decdata_16b = DISP_MALLOC(4096*4096*sizeof(HI_U16));

	if (p_decdata_y16 == HI_NULL
			|| p_decdata_uv16 == HI_NULL
			|| p_decdata_16b == HI_NULL)
	{
		WIN_ERROR("Alloc TmpDate Failed\n.");
		if (p_decdata_y16)
		{
			DISP_FREE(p_decdata_y16);
		}
		if (p_decdata_uv16)
		{
			DISP_FREE(p_decdata_uv16);
		}
		if (p_decdata_16b)
		{
			DISP_FREE(p_decdata_16b);
		}
		return HI_FAILURE;
	}

	p_decdata_8b = pDstDateAddr;
	ALG_Decompress_GetOriDataInfo(pOriDateAddr,&stOriDataInfo);

	s32Ret = ALG_Decompress_GetY16ByteData(&stOriDataInfo,
	        pOriDateAddr,
	        p_decdata_y16);
    if (s32Ret != HI_SUCCESS)
    {    
       WIN_ERROR("get y 16byte data error\n");
       goto process_error;
    }
	    
	s32Ret = ALG_Decompress_GetUV16ByteData(&stOriDataInfo,
	        pOriDateAddr,
	        p_decdata_uv16);
	if (s32Ret != HI_SUCCESS)
    {    
       WIN_ERROR("get c 16byte data error\n");
       goto process_error;
    }        

    ALG_Decompress_Get16ByteData(&stOriDataInfo,
			p_decdata_y16,
			p_decdata_uv16,
			p_decdata_16b);
    
	ALG_Decompress_Get8ByteData(&stOriDataInfo,
	        p_decdata_16b,
	        p_decdata_8b);
	
#if DEF_ALG_DECOMPRESS_DBG	
    WIN_ERROR("%#x %#x\n",pOriDateAddr,pDstDateAddr);
    WIN_ERROR("\n%d %d %d %d %d\n"
              "%d %d %d %d %d\n",
              stOriDataInfo.width,
              stOriDataInfo.height,
              stOriDataInfo.slot_width,
              stOriDataInfo.slot_height,
              stOriDataInfo.stride,
              stOriDataInfo.structure,
              stOriDataInfo.chroma_idc,
              stOriDataInfo.yuv_bit_depth,
              stOriDataInfo.bit_depth,
              stOriDataInfo.compress);
    WIN_ERROR("ori:%#x %#x %#x %#x %#x %#x %#x %#x\n",
	    pOriDateAddr[80],
	    pOriDateAddr[81],
	    pOriDateAddr[82],
	    pOriDateAddr[83],
	    pOriDateAddr[355],
	    pOriDateAddr[356],
	    pOriDateAddr[357],
	    pOriDateAddr[358]);
	 WIN_ERROR("y16:%#x %#x %#x %#x %#x %#x %#x %#x\n",
	    p_decdata_y16[0],
	    p_decdata_y16[1],
	    p_decdata_y16[2],
	    p_decdata_y16[3],
	    p_decdata_y16[155],
	    p_decdata_y16[156],
	    p_decdata_y16[157],
	    p_decdata_y16[158]); 
    WIN_ERROR("c16:%#x %#x %#x %#x %#x %#x %#x %#x\n",
	    p_decdata_uv16[0],
	    p_decdata_uv16[1],
	    p_decdata_uv16[2],
	    p_decdata_uv16[3],
	    p_decdata_uv16[155],
	    p_decdata_uv16[156],
	    p_decdata_uv16[157],
	    p_decdata_uv16[158]);	
    WIN_ERROR("16byte:%#x %#x %#x %#x %#x %#x %#x %#x\n",
	    p_decdata_16b[0],
	    p_decdata_16b[1],
	    p_decdata_16b[2],
	    p_decdata_16b[3],
	    p_decdata_16b[155],
	    p_decdata_16b[156],
	    p_decdata_16b[157],
	    p_decdata_16b[158]);	    
#endif
process_error:
	DISP_FREE(p_decdata_y16);
	DISP_FREE(p_decdata_uv16);
	DISP_FREE(p_decdata_16b);

	return s32Ret;
}

HI_BOOL VO_ALG_CheckFrameValid(HI_DRV_VIDEO_FRAME_S *pstFrame)
{
	HI_BOOL bValid = HI_TRUE;

	if (pstFrame->enBitWidth != HI_DRV_PIXEL_BITWIDTH_8BIT)
	{
		bValid = HI_FALSE;
		WIN_ERROR("Decompress Frame is invalid,BitWidth=%d\n",pstFrame->enBitWidth);
	}

	if (pstFrame->ePixFormat != HI_DRV_PIX_FMT_NV21_TILE_CMP)
	{
		bValid = HI_FALSE;
		WIN_ERROR("Decompress Frame is invalid,PixFormat=%d\n",pstFrame->ePixFormat);
	}

	return bValid;
}

HI_S32 VO_ALG_Decompress(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_U8 *pDstBufAddr,HI_U32 u32DstBufSize)
{
	HI_S32 s32Ret;

	SMMU_BUFFER_S stArrangedData;
	HI_U32 u32ArrangedDataSize;
	HI_U32 u32FrmStride;
	HI_U32 u32FrmHgt;
	HI_U32 u32FrmHeadInfSize;

	if (!VO_ALG_CheckFrameValid(pstFrame))
	{
		WIN_ERROR("Decompress Frame is invalid\n");
		return HI_FAILURE;
	}

	u32FrmStride = pstFrame->stBufAddr[0].u32Stride_Y * 16;
	u32FrmHgt = pstFrame->u32Height;
	u32FrmHeadInfSize = pstFrame->stBufAddr[0].u32Head_Size;

	u32ArrangedDataSize = 32 
		+ u32FrmHeadInfSize 
		+ (u32FrmStride / 16) * (((u32FrmHgt + 63) / 64) * 64)
		+ u32FrmHeadInfSize / 2 
		+ (u32FrmStride / 16) * (((u32FrmHgt + 63) / 64) * 64) / 2;
	s32Ret = HI_DRV_SMMU_AllocAndMap("VO_TmpData", u32ArrangedDataSize, 16, &stArrangedData);
	if (HI_SUCCESS != s32Ret)
	{
		WIN_ERROR("Alloc Temp Buffer Failed,Ret=%#x\n",s32Ret);
		return s32Ret;
	}
#if DEF_ALG_DECOMPRESS_DBG	
    WIN_ERROR("%d %d %#x %#x %#x %#x\n",
        pstFrame->u32Width,
        pstFrame->u32Height,
        pstFrame->stBufAddr[0].u32PhyAddr_YHead,
        pstFrame->stBufAddr[0].u32PhyAddr_CHead,
        pstFrame->stBufAddr[0].u32PhyAddr_Y,
        pstFrame->stBufAddr[0].u32PhyAddr_C
        );
#endif        
	s32Ret = ALG_Decompress_ArrangeOriData(pstFrame,
			stArrangedData.pu8StartVirAddr);
	if (HI_SUCCESS != s32Ret)
	{
        HI_DRV_SMMU_UnmapAndRelease(&stArrangedData);
		WIN_ERROR("Arrange OriData Failed,Ret=%#x\n",s32Ret);
		return s32Ret;
	}

	s32Ret = ALG_Decompress_Process(stArrangedData.pu8StartVirAddr,
			pDstBufAddr,u32DstBufSize);
	if (HI_SUCCESS != s32Ret)
	{
        HI_DRV_SMMU_UnmapAndRelease(&stArrangedData);
		WIN_ERROR("Decompress ArrangedData Failed,Ret=%#x\n",s32Ret);
		return s32Ret;
	}

    HI_DRV_SMMU_UnmapAndRelease(&stArrangedData);
	return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

