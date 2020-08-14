#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <dirent.h>
#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_go.h"
#include "hi_go_decoder.h"
#include "hi_go_bliter.h"
#include "hi_go_text.h"
#include "hi_adp_mpi.h"
#include "fv.h"
#include "fv_dbg.h"
#include "fv_face.h"
#include  "fv_item_descriptor_720.h"

#define  VO_INVALID_HANDLE 0
 
HI_HANDLE g_hFont = HIGO_INVALID_HANDLE;
HIGO_LAYER_INFO_S g_stLayerInfo = {0};
HI_HANDLE g_hLayer = HIGO_INVALID_HANDLE;
HI_HANDLE g_hLayerSurface;
HI_HANDLE vdWin = VO_INVALID_HANDLE;
HI_RECT   current_format_rect[60] = {};

int fv_face_on(void)
{	
	HI_S32 ret;	
	HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;
	HI_RECT lable_rc;
	enum fv_item_list item_type;

	ret = HI_GO_Init();
        if (HI_SUCCESS != ret)
        {
           fv_error("call fv_system_init failed.Ret = %#x\n",ret);
	   HI_GO_Deinit();
           return -1;
        }

	HI_GO_GetLayerDefaultParam(eLayerID, &g_stLayerInfo); 	
	g_stLayerInfo.PixelFormat = HIGO_PF_8888;
	g_stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;

	ret = HI_GO_CreateLayer(&g_stLayerInfo, &g_hLayer);
	if (HI_SUCCESS != ret)
	{
		fv_error("failed to create the layer sd 0, ret = 0x%x !\n", ret);
		fv_face_off();
		return -1;
	}

	ret =  HI_GO_GetLayerSurface(g_hLayer, &g_hLayerSurface);
	if  (HI_SUCCESS != ret)
	{
		fv_debug("failed to get layer surface! ret = 0x%x \n", ret);
		fv_face_off();
		return -1;      
	}

	ret = HI_GO_FillRect(g_hLayerSurface, NULL, 0x000000ff, HIGO_COMPOPT_NONE);
	if (HI_SUCCESS != ret) 
	{
		fv_error("failed to fill rect! ret = 0x%x \n", ret);
		fv_face_off();
		return -1;   	    
	}

	ret = HI_GO_CreateText(FV_FONT_PATH_720, NULL, &g_hFont);
	if (HI_SUCCESS != ret) 
	{
		fv_error("failed to create the font:DroidSansFallbackLegacy.ttf ! ret = %#x\n",ret); 
		fv_face_off();
		return -1; 	    
	}

	item_type = FV_ITEM_FV_NAME_TXT;
	while(item_descriptor_720[item_type].ITEM_TYPE != FV_ITEM_BUTT )
	{
		lable_rc.x  = item_descriptor_720[item_type].rc.x;
		lable_rc.y  = item_descriptor_720[item_type].rc.y;	
		lable_rc.w  = item_descriptor_720[item_type].rc.w;	
		lable_rc.h   = item_descriptor_720[item_type].rc.h;	
		
		if(item_descriptor_720[item_type].isLable == 1 )		
		{	
			ret = HI_GO_SetTextColor(g_hFont, 0xFFFFFFFF);
			if(ret != HI_SUCCESS) 
			{
				printf("HI_GO_SetTextColor failed!,ret = %#x,line = %d\n",ret,__LINE__);	
				fv_face_off();
				return -1;  
			}
	 	}
	 	else 
	 	{
	 		ret = HI_GO_SetTextColor(g_hFont, 0xff00ff00);
			if(ret != HI_SUCCESS) 
			{
				fv_error("HI_GO_SetTextColor failed!,ret = %#x,line = %d,item_type = %d\n",ret,__LINE__,item_type);	
				fv_face_off();
				return -1;  
			}
	 	}	 	
	 		
	 	ret = HI_GO_DrawRect(g_hLayerSurface, &lable_rc, 0x00000000);
	       	if(ret != HI_SUCCESS)
		{
			fv_error("call HI_GO_DrawRect  failed!,ret = %#x,LINE = %d,item_type = %d\n",ret,__LINE__,item_type);			
			fv_face_off();
			return -1; 
		}
       	
	        ret = HI_GO_SetTextStyle(g_hFont, HIGO_TEXT_STYLE_NORMAL);
	        if(ret != HI_SUCCESS)
		{
			fv_error("call HI_GO_SetTextStyle  failed! ret = %#x\n",ret);	
			fv_face_off();
			return -1; 
		}

	 	ret = HI_GO_TextOutEx(g_hFont,g_hLayerSurface, item_descriptor_720[item_type].str,  &lable_rc,  HIGO_LAYOUT_LEFT);
	        if(ret != HI_SUCCESS)
		{
			fv_error("call HI_GO_TextOutEx  failed! ret = %#x\n",ret);	
			fv_face_off();
			return -1; 
		}
			
	        ret = HI_GO_RefreshLayer(g_hLayer, NULL);
	        if(ret != HI_SUCCESS)
		{
			fv_error("call HI_GO_RefreshLayer  failed! ret = %#x\n",ret);	
			fv_face_off();
			return -1; 
		}		
		item_type++;	

	}		

	return ret;
}

void fv_face_off(void) 
{
	HI_S32 Ret;
   	if(HIGO_INVALID_HANDLE != g_hFont)
        {
        	Ret = HI_GO_DestroyText(g_hFont); 
	        if(Ret != HI_SUCCESS)
	        {
	        	fv_error("call HI_GO_DestroyText failed!,Ret = %#x\n",Ret);        	
	        }            
	}

	if (HIGO_INVALID_HANDLE != g_hLayer)
        {
        	Ret = HI_GO_DestroyLayer(g_hLayer);   
	        if(Ret != HI_SUCCESS)
	        {
	        	fv_error("call HI_GO_DestroyLayer failed!,Ret = %#x\n",Ret);        		
	        } 
   	}  
	
        Ret = HI_GO_Deinit();
        if(Ret != HI_SUCCESS)
        {
        	fv_error("call HI_GO_Deinit failed!,Ret = %#x\n",Ret);        
        }         
        
	return ;
}


int    HI_FV_DrawText(enum fv_item_list item_type , char *buf, unsigned int sz)
{
	HI_S32 Ret;
	HI_RECT text_rc;
	HI_U32 num = 0;
	
	while(item_descriptor_720[num].ITEM_TYPE != item_type )
	{	
		num++;
		if(item_descriptor_720[num].ITEM_TYPE == FV_ITEM_BUTT)
		{
			printf("the item_type you input is not exist in item_descriptor! line = %d",__LINE__);
			return -1;	
		}	
	}
		
	Ret = HI_GO_SetTextColor(g_hFont, 0xff00ff00); 
	if(Ret != HI_SUCCESS) 
	{
		fv_error("HI_GO_SetTextColor failed!line = %d\n",__LINE__);	
		return -1; 
	}

	text_rc.x  = item_descriptor_720[num].rc.x;
	text_rc.y  = item_descriptor_720[num].rc.y;	
	text_rc.w  = item_descriptor_720[num].rc.w;	
	text_rc.h   = item_descriptor_720[num].rc.h;	

	Ret= HI_GO_FillRect(g_hLayerSurface, &text_rc, 0x00000000, HIGO_COMPOPT_NONE);
        if (HI_SUCCESS != Ret)
        {
                fv_error("call HI_GO_FillRect failed. ret = %#x",Ret);
                return -1;
        }   

        Ret = HI_GO_DrawRect(g_hLayerSurface, &text_rc, 0x00000000);
       	if(Ret != HI_SUCCESS)
	{
		fv_error("call HI_GO_DrawRect  failed!\n");
		return -1; 
	} 
  
 	Ret = HI_GO_TextOutEx(g_hFont,g_hLayerSurface,  buf,  &text_rc,  HIGO_LAYOUT_LEFT);
        if(Ret != HI_SUCCESS)
	{
		fv_error("HI_GO_TextOutEx  failed!\n");	
		return -1;
	}

	Ret = HI_GO_RefreshLayer(g_hLayer, &text_rc);
        if(Ret != HI_SUCCESS)
	{
		fv_error("HI_GO_RefreshLayer  failed!\n");	
		return -1;
	}

        return 0;
	
}

int fv_draw_vol(unsigned int vol)
{
	HI_S32 Ret;
	HI_RECT  vol_rc,vol_val_rc;
	HI_CHAR str[] = {0};
	
	vol_rc.x  = item_descriptor_720[FV_ITEM_VOLUME_TXT].rc.x;
	vol_rc.y  = item_descriptor_720[FV_ITEM_VOLUME_TXT].rc.y;	
	vol_rc.w  = item_descriptor_720[FV_ITEM_VOLUME_TXT].rc.w;	
	vol_rc.h   = item_descriptor_720[FV_ITEM_VOLUME_TXT].rc.h;	
	
	vol_val_rc.x  = item_descriptor_720[FV_ITEM_VOLUME_VAL].rc.x;
	vol_val_rc.y  = item_descriptor_720[FV_ITEM_VOLUME_VAL].rc.y;	
	vol_val_rc.w  = item_descriptor_720[FV_ITEM_VOLUME_VAL].rc.w;	
	vol_val_rc.h   = item_descriptor_720[FV_ITEM_VOLUME_VAL].rc.h;	

        Ret= HI_GO_FillRect(g_hLayerSurface, &vol_rc, 0x00000000, HIGO_COMPOPT_NONE);
        if (HI_SUCCESS != Ret)
        {
            fv_debug("call HI_GO_FillRect failed. ret = %#x,vol = %d\n",Ret,vol);
            return -1;
        }        
        
	 vol_rc.w = vol*(VOL_SHOW_WIDTH_720/100);    
	
        if(vol > 0 && vol <101)        
       	{              
                Ret= HI_GO_FillRect(g_hLayerSurface, &vol_rc, 0xFF00ff00, HIGO_COMPOPT_NONE);
                if (HI_SUCCESS != Ret)
                {
                    fv_debug("call HI_GO_FillRect failed. ret = %#x,vol = %d\n",Ret,vol);
                    return -1;
                }                
        }
        else if(vol < 0 ||vol>100)
        {	
		fv_error("*******error!!!!!!!!the vol value is beyond the normal range (0~100)!*********");
		return -1;
        }

        Ret= HI_GO_FillRect(g_hLayerSurface, &vol_val_rc, 0xfffffff, HIGO_COMPOPT_NONE);
        if (HI_SUCCESS != Ret)
        {
                fv_debug("call HI_GO_FillRect failed. ret = %#x,vol = %d\n",Ret,vol);
                return -1;
        }
        
	sprintf(str,"%d",vol);
	Ret = HI_FV_DrawText(FV_ITEM_VOLUME_VAL, str, 4);
        if (HI_SUCCESS != Ret)
        {
               fv_debug("call HI_GO_RefreshLayer failed.\n");
               return -1;
        }
       
        Ret = HI_GO_RefreshLayer(g_hLayer, NULL);
        if (HI_SUCCESS != Ret)
        {
               fv_debug("call HI_GO_RefreshLayer failed.\n");
               return -1;
        }     

	return 0;
}


int  fv_get_win_handle(unsigned int *Win)
{
	HI_S32 Ret;
	HI_RECT      vd_rc;
	HI_RECT_S  video_rc;
	HI_U32 num = 0;	

	while(item_descriptor_720[num].ITEM_TYPE != FV_ITEM_VIDEO_TXT )
	{	
		num++;
		if(item_descriptor_720[num].ITEM_TYPE == FV_ITEM_BUTT)
		{
			printf("the item_type FV_ITEM_VIDEO_TXT is not exist in item_descriptor! \n please add this item type.line = %d",__LINE__);
			return -1;	
		}	
	}
	        
	vd_rc.x  = item_descriptor_720[num].rc.x;
	vd_rc.y  = item_descriptor_720[num].rc.y;	
	vd_rc.w  = item_descriptor_720[num].rc.w;	
	vd_rc.h   = item_descriptor_720[num].rc.h;	


        Ret =  HI_GO_FillRect(g_hLayerSurface,&vd_rc,0xFFFF0000,HIGO_COMPOPT_CLEAR);
        if (HI_SUCCESS != Ret)
        {
            fv_debug("call HI_GO_FillRect failed.\n");
            return -1;
        } 

        Ret = HI_GO_EnableSurfaceColorKey(g_hLayerSurface, HI_TRUE);
        if (HI_SUCCESS != Ret)
        {
            fv_debug("call HI_GO_EnableSurfaceColorKey failed.Ret = %#x\n",Ret);
            return -1;
        } 

        Ret = HI_GO_SetSurfaceColorKey(g_hLayerSurface, 0xFFFF0000); 
        if (HI_SUCCESS != Ret)
        {
            fv_debug("call HI_GO_SetSurfaceColorKey failed.Ret = %#x\n",Ret);
            return -1;
        } 

        Ret =  HI_GO_RefreshLayer(g_hLayer, &vd_rc);
        if (HI_SUCCESS != Ret)
        { 
            fv_debug("call HI_GO_RefreshLayer failed.line = %d,Ret = %#x\n",__LINE__,Ret);
            return -1;
        } 
        
	video_rc.s32X = vd_rc.x;
	video_rc.s32Y = vd_rc.y;
	video_rc.s32Height = vd_rc.h;
	video_rc.s32Width = vd_rc.w;

	Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
        if (HI_SUCCESS != Ret)
        {
            fv_error("call HIADP_VO_Init failed. Ret = %#x\n",Ret);
            HIADP_VO_DeInit();
            return -1;
        }

        Ret = HIADP_VO_CreatWin(&video_rc,&vdWin);
        if (HI_SUCCESS != Ret)
        {
            fv_error("call HIADP_VO_CreatWin failed. Ret = %#x\n",Ret);
            return -1;
        }
	*Win = vdWin;
	
        return 0;
        
}

void fv_destroy_win_handle(void)
{
	HI_S32 Ret;
	
	if(VO_INVALID_HANDLE != vdWin)
	{
		Ret = HI_UNF_VO_DestroyWindow(vdWin);
     		if (HI_SUCCESS != Ret)
       		{
       		     printf("call fv_destroy_win_handle failed.Ret = %#x\n",Ret);            
       		}
	}
	return;	
}


