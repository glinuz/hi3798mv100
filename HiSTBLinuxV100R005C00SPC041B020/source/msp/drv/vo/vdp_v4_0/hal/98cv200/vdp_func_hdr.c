
#include "hi_type.h"
#include "vdp_hal_vid.h"
#include "vdp_hal_comm.h"
#include "vdp_hal_comm.h"
#include "vdp_func_hdr.h"
#include "vdp_drv_comm.h"
#if VDP_CBB_FPGA
#include <linux/string.h>
#else
#include <string.h>
extern DmKsFxp_t g_pKs;
#endif


HI_S32 VDP_FUNC_GetVdmCfg(VDP_HDR_DM_CFG_E DmMode, DmKsFxp_t * pKs)
{


    if(DmMode == VDP_HDR_DM_CFG_TYP)
    {
        //read metadata=>pKs
    }
    else if(DmMode == VDP_HDR_DM_CFG_RAND_YUV2RGB)
    {
        pKs->ksIMap.m33Yuv2RgbScale2P      =   sGetRandEx(5,31);              
        pKs->ksIMap.m33Yuv2Rgb[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[2][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.v3Yuv2RgbOffInRgb[0]   =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);              
        pKs->ksIMap.v3Yuv2RgbOffInRgb[1]   =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);              
        pKs->ksIMap.v3Yuv2RgbOffInRgb[2]   =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);              
    }
    else if(DmMode == VDP_HDR_DM_CFG_RAND_RGB2LMS)
    {

        pKs->ksIMap.m33Rgb2LmsScale2P      =   sGetRandEx(0,31);              
        pKs->ksIMap.m33Rgb2Lms[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[2][2]       =   sGetRandEx(-32768,32767);              

    }
    else if(DmMode == VDP_HDR_DM_CFG_RAND_LMS2IPT)
    {

        pKs->ksIMap.m33Lms2IptScale2P      =   sGetRandEx(0,31);              
        pKs->ksIMap.m33Lms2Ipt[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[2][2]       =   sGetRandEx(-32768,32767);              

    }
    else if(DmMode == VDP_HDR_DM_CFG_RAND_IPT2LMS)
    {

        pKs->ksOMap.m33Ipt2LmsScale2P      =   sGetRandEx(1,31);              
        pKs->ksOMap.m33Ipt2Lms[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[2][2]       =   sGetRandEx(-32768,32767);              

    }
    else if(DmMode == VDP_HDR_DM_CFG_RAND_LMS2RGB)
    {

        pKs->ksOMap.m33Lms2RgbScale2P      =   sGetRandEx(0,31);              
        pKs->ksOMap.m33Lms2Rgb[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[2][2]       =   sGetRandEx(-32768,32767);              
    }
    else if(DmMode == VDP_HDR_DM_CFG_RAND_RGB2YUV)
    {

        pKs->ksOMap.m33Rgb2YuvScale2P      =   sGetRandEx(0,31);              
        pKs->ksOMap.m33Rgb2Yuv[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[2][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.v3Rgb2YuvOff[0]        =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);
        pKs->ksOMap.v3Rgb2YuvOff[1]        =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);
        pKs->ksOMap.v3Rgb2YuvOff[2]        =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);

    }
    else if(DmMode == VDP_HDR_DM_CFG_MAX)
    {
        pKs->ksIMap.m33Yuv2RgbScale2P      =          31;
        pKs->ksIMap.m33Yuv2Rgb[0][0]       =       32767;       
        pKs->ksIMap.m33Yuv2Rgb[0][1]       =       32767;       
        pKs->ksIMap.m33Yuv2Rgb[0][2]       =       32767;       
        pKs->ksIMap.m33Yuv2Rgb[1][0]       =       32767;       
        pKs->ksIMap.m33Yuv2Rgb[1][1]       =       32767;       
        pKs->ksIMap.m33Yuv2Rgb[1][2]       =       32767;       
        pKs->ksIMap.m33Yuv2Rgb[2][0]       =       32767;       
        pKs->ksIMap.m33Yuv2Rgb[2][1]       =       32767;       
        pKs->ksIMap.m33Yuv2Rgb[2][2]       =       32767;       
        pKs->ksIMap.v3Yuv2RgbOffInRgb[0]   =   0x7FFFFFFF;          
        pKs->ksIMap.v3Yuv2RgbOffInRgb[1]   =   0x7FFFFFFF;          
        pKs->ksIMap.v3Yuv2RgbOffInRgb[2]   =   0x7FFFFFFF;

        pKs->ksIMap.m33Rgb2LmsScale2P      =           31;   
        pKs->ksIMap.m33Rgb2Lms[0][0]       =        32767;           
        pKs->ksIMap.m33Rgb2Lms[0][1]       =        32767;           
        pKs->ksIMap.m33Rgb2Lms[0][2]       =        32767;           
        pKs->ksIMap.m33Rgb2Lms[1][0]       =        32767;           
        pKs->ksIMap.m33Rgb2Lms[1][1]       =        32767;           
        pKs->ksIMap.m33Rgb2Lms[1][2]       =        32767;           
        pKs->ksIMap.m33Rgb2Lms[2][0]       =        32767;           
        pKs->ksIMap.m33Rgb2Lms[2][1]       =        32767;           
        pKs->ksIMap.m33Rgb2Lms[2][2]       =        32767; 

        pKs->ksIMap.m33Lms2IptScale2P      =            31; 
        pKs->ksIMap.m33Lms2Ipt[0][0]       =         32767; 
        pKs->ksIMap.m33Lms2Ipt[0][1]       =         32767; 
        pKs->ksIMap.m33Lms2Ipt[0][2]       =         32767; 
        pKs->ksIMap.m33Lms2Ipt[1][0]       =         32767; 
        pKs->ksIMap.m33Lms2Ipt[1][1]       =         32767; 
        pKs->ksIMap.m33Lms2Ipt[1][2]       =         32767; 
        pKs->ksIMap.m33Lms2Ipt[2][0]       =         32767; 
        pKs->ksIMap.m33Lms2Ipt[2][1]       =         32767; 
        pKs->ksIMap.m33Lms2Ipt[2][2]       =         32767; 

        pKs->ksOMap.m33Ipt2LmsScale2P      =            31; 
        pKs->ksOMap.m33Ipt2Lms[0][0]       =         32767; 
        pKs->ksOMap.m33Ipt2Lms[0][1]       =         32767; 
        pKs->ksOMap.m33Ipt2Lms[0][2]       =         32767; 
        pKs->ksOMap.m33Ipt2Lms[1][0]       =         32767; 
        pKs->ksOMap.m33Ipt2Lms[1][1]       =         32767; 
        pKs->ksOMap.m33Ipt2Lms[1][2]       =         32767; 
        pKs->ksOMap.m33Ipt2Lms[2][0]       =         32767; 
        pKs->ksOMap.m33Ipt2Lms[2][1]       =         32767; 
        pKs->ksOMap.m33Ipt2Lms[2][2]       =         32767; 

        pKs->ksOMap.m33Lms2RgbScale2P      =            31; 
        pKs->ksOMap.m33Lms2Rgb[0][0]       =         32767; 
        pKs->ksOMap.m33Lms2Rgb[0][1]       =         32767; 
        pKs->ksOMap.m33Lms2Rgb[0][2]       =         32767; 
        pKs->ksOMap.m33Lms2Rgb[1][0]       =         32767; 
        pKs->ksOMap.m33Lms2Rgb[1][1]       =         32767; 
        pKs->ksOMap.m33Lms2Rgb[1][2]       =         32767; 
        pKs->ksOMap.m33Lms2Rgb[2][0]       =         32767; 
        pKs->ksOMap.m33Lms2Rgb[2][1]       =         32767; 
        pKs->ksOMap.m33Lms2Rgb[2][2]       =         32767; 

        pKs->ksOMap.m33Rgb2YuvScale2P      =            31; 
        pKs->ksOMap.m33Rgb2Yuv[0][0]       =         32767; 
        pKs->ksOMap.m33Rgb2Yuv[0][1]       =         32767; 
        pKs->ksOMap.m33Rgb2Yuv[0][2]       =         32767; 
        pKs->ksOMap.m33Rgb2Yuv[1][0]       =         32767; 
        pKs->ksOMap.m33Rgb2Yuv[1][1]       =         32767; 
        pKs->ksOMap.m33Rgb2Yuv[1][2]       =         32767; 
        pKs->ksOMap.m33Rgb2Yuv[2][0]       =         32767; 
        pKs->ksOMap.m33Rgb2Yuv[2][1]       =         32767; 
        pKs->ksOMap.m33Rgb2Yuv[2][2]       =         32767; 
        pKs->ksOMap.v3Rgb2YuvOff[0]        =     0x7FFFFFFF; 
        pKs->ksOMap.v3Rgb2YuvOff[1]        =     0x7FFFFFFF; 
        pKs->ksOMap.v3Rgb2YuvOff[2]        =     0x7FFFFFFF; 
    }
    else if(DmMode == VDP_HDR_DM_CFG_MIN)
    {
        pKs->ksIMap.m33Yuv2RgbScale2P      =            5;
        pKs->ksIMap.m33Yuv2Rgb[0][0]       =       -32768;       
        pKs->ksIMap.m33Yuv2Rgb[0][1]       =       -32768;       
        pKs->ksIMap.m33Yuv2Rgb[0][2]       =       -32768;       
        pKs->ksIMap.m33Yuv2Rgb[1][0]       =       -32768;       
        pKs->ksIMap.m33Yuv2Rgb[1][1]       =       -32768;       
        pKs->ksIMap.m33Yuv2Rgb[1][2]       =       -32768;       
        pKs->ksIMap.m33Yuv2Rgb[2][0]       =       -32768;       
        pKs->ksIMap.m33Yuv2Rgb[2][1]       =       -32768;       
        pKs->ksIMap.m33Yuv2Rgb[2][2]       =       -32768;       
        pKs->ksIMap.v3Yuv2RgbOffInRgb[0]   =     -(0x7FFFFFFF);          
        pKs->ksIMap.v3Yuv2RgbOffInRgb[1]   =     -(0x7FFFFFFF);          
        pKs->ksIMap.v3Yuv2RgbOffInRgb[2]   =     -(0x7FFFFFFF);

        pKs->ksIMap.m33Rgb2LmsScale2P      =            31; 
        pKs->ksIMap.m33Rgb2Lms[0][0]       =        -32768;         
        pKs->ksIMap.m33Rgb2Lms[0][1]       =        -32768;         
        pKs->ksIMap.m33Rgb2Lms[0][2]       =        -32768;         
        pKs->ksIMap.m33Rgb2Lms[1][0]       =        -32768;         
        pKs->ksIMap.m33Rgb2Lms[1][1]       =        -32768;         
        pKs->ksIMap.m33Rgb2Lms[1][2]       =        -32768;         
        pKs->ksIMap.m33Rgb2Lms[2][0]       =        -32768;         
        pKs->ksIMap.m33Rgb2Lms[2][1]       =        -32768;         
        pKs->ksIMap.m33Rgb2Lms[2][2]       =        -32768; 

        pKs->ksIMap.m33Lms2IptScale2P      =            31; 
        pKs->ksIMap.m33Lms2Ipt[0][0]       =        -32768; 
        pKs->ksIMap.m33Lms2Ipt[0][1]       =        -32768; 
        pKs->ksIMap.m33Lms2Ipt[0][2]       =        -32768; 
        pKs->ksIMap.m33Lms2Ipt[1][0]       =        -32768; 
        pKs->ksIMap.m33Lms2Ipt[1][1]       =        -32768; 
        pKs->ksIMap.m33Lms2Ipt[1][2]       =        -32768; 
        pKs->ksIMap.m33Lms2Ipt[2][0]       =        -32768; 
        pKs->ksIMap.m33Lms2Ipt[2][1]       =        -32768; 
        pKs->ksIMap.m33Lms2Ipt[2][2]       =        -32768; 

        pKs->ksOMap.m33Ipt2LmsScale2P      =              1;
        pKs->ksOMap.m33Ipt2Lms[0][0]       =         -32768;
        pKs->ksOMap.m33Ipt2Lms[0][1]       =         -32768;
        pKs->ksOMap.m33Ipt2Lms[0][2]       =         -32768;
        pKs->ksOMap.m33Ipt2Lms[1][0]       =         -32768;
        pKs->ksOMap.m33Ipt2Lms[1][1]       =         -32768;
        pKs->ksOMap.m33Ipt2Lms[1][2]       =         -32768;
        pKs->ksOMap.m33Ipt2Lms[2][0]       =         -32768;
        pKs->ksOMap.m33Ipt2Lms[2][1]       =         -32768;
        pKs->ksOMap.m33Ipt2Lms[2][2]       =         -32768;

        pKs->ksOMap.m33Lms2RgbScale2P      =             31;
        pKs->ksOMap.m33Lms2Rgb[0][0]       =         -32768;
        pKs->ksOMap.m33Lms2Rgb[0][1]       =         -32768;
        pKs->ksOMap.m33Lms2Rgb[0][2]       =         -32768;
        pKs->ksOMap.m33Lms2Rgb[1][0]       =         -32768;
        pKs->ksOMap.m33Lms2Rgb[1][1]       =         -32768;
        pKs->ksOMap.m33Lms2Rgb[1][2]       =         -32768;
        pKs->ksOMap.m33Lms2Rgb[2][0]       =         -32768;
        pKs->ksOMap.m33Lms2Rgb[2][1]       =         -32768;
        pKs->ksOMap.m33Lms2Rgb[2][2]       =         -32768;

        pKs->ksOMap.m33Rgb2YuvScale2P      =              0;  
        pKs->ksOMap.m33Rgb2Yuv[0][0]       =         -32768;  
        pKs->ksOMap.m33Rgb2Yuv[0][1]       =         -32768;  
        pKs->ksOMap.m33Rgb2Yuv[0][2]       =         -32768;  
        pKs->ksOMap.m33Rgb2Yuv[1][0]       =         -32768;  
        pKs->ksOMap.m33Rgb2Yuv[1][1]       =         -32768;  
        pKs->ksOMap.m33Rgb2Yuv[1][2]       =         -32768;  
        pKs->ksOMap.m33Rgb2Yuv[2][0]       =         -32768;  
        pKs->ksOMap.m33Rgb2Yuv[2][1]       =         -32768;  
        pKs->ksOMap.m33Rgb2Yuv[2][2]       =         -32768;  
        pKs->ksOMap.v3Rgb2YuvOff[0]        =       -(0x7FFFFFFF); 
        pKs->ksOMap.v3Rgb2YuvOff[1]        =       -(0x7FFFFFFF); 
        pKs->ksOMap.v3Rgb2YuvOff[2]        =       -(0x7FFFFFFF); 
    }
    else if(DmMode == VDP_HDR_DM_CFG_ZERO)
    {
        pKs->ksIMap.m33Yuv2RgbScale2P      =   5; 
        pKs->ksIMap.m33Yuv2Rgb[0][0]       =   0;    
        pKs->ksIMap.m33Yuv2Rgb[0][1]       =   0;    
        pKs->ksIMap.m33Yuv2Rgb[0][2]       =   0;    
        pKs->ksIMap.m33Yuv2Rgb[1][0]       =   0;    
        pKs->ksIMap.m33Yuv2Rgb[1][1]       =   0;    
        pKs->ksIMap.m33Yuv2Rgb[1][2]       =   0;    
        pKs->ksIMap.m33Yuv2Rgb[2][0]       =   0;    
        pKs->ksIMap.m33Yuv2Rgb[2][1]       =   0;    
        pKs->ksIMap.m33Yuv2Rgb[2][2]       =   0;    
        pKs->ksIMap.v3Yuv2RgbOffInRgb[0]   =   0;       
        pKs->ksIMap.v3Yuv2RgbOffInRgb[1]   =   0;       
        pKs->ksIMap.v3Yuv2RgbOffInRgb[2]   =   0; 

        pKs->ksIMap.m33Rgb2LmsScale2P      =   0; 
        pKs->ksIMap.m33Rgb2Lms[0][0]       =   0;       
        pKs->ksIMap.m33Rgb2Lms[0][1]       =   0;       
        pKs->ksIMap.m33Rgb2Lms[0][2]       =   0;       
        pKs->ksIMap.m33Rgb2Lms[1][0]       =   0;       
        pKs->ksIMap.m33Rgb2Lms[1][1]       =   0;       
        pKs->ksIMap.m33Rgb2Lms[1][2]       =   0;       
        pKs->ksIMap.m33Rgb2Lms[2][0]       =   0;       
        pKs->ksIMap.m33Rgb2Lms[2][1]       =   0;       
        pKs->ksIMap.m33Rgb2Lms[2][2]       =   0; 

        pKs->ksIMap.m33Lms2IptScale2P      =   0; 
        pKs->ksIMap.m33Lms2Ipt[0][0]       =   0; 
        pKs->ksIMap.m33Lms2Ipt[0][1]       =   0; 
        pKs->ksIMap.m33Lms2Ipt[0][2]       =   0; 
        pKs->ksIMap.m33Lms2Ipt[1][0]       =   0; 
        pKs->ksIMap.m33Lms2Ipt[1][1]       =   0; 
        pKs->ksIMap.m33Lms2Ipt[1][2]       =   0; 
        pKs->ksIMap.m33Lms2Ipt[2][0]       =   0; 
        pKs->ksIMap.m33Lms2Ipt[2][1]       =   0; 
        pKs->ksIMap.m33Lms2Ipt[2][2]       =   0; 

        pKs->ksOMap.m33Ipt2LmsScale2P      =   1; 
        pKs->ksOMap.m33Ipt2Lms[0][0]       =   0; 
        pKs->ksOMap.m33Ipt2Lms[0][1]       =   0; 
        pKs->ksOMap.m33Ipt2Lms[0][2]       =   0; 
        pKs->ksOMap.m33Ipt2Lms[1][0]       =   0; 
        pKs->ksOMap.m33Ipt2Lms[1][1]       =   0; 
        pKs->ksOMap.m33Ipt2Lms[1][2]       =   0; 
        pKs->ksOMap.m33Ipt2Lms[2][0]       =   0; 
        pKs->ksOMap.m33Ipt2Lms[2][1]       =   0; 
        pKs->ksOMap.m33Ipt2Lms[2][2]       =   0; 

        pKs->ksOMap.m33Lms2RgbScale2P      =   0; 
        pKs->ksOMap.m33Lms2Rgb[0][0]       =   0; 
        pKs->ksOMap.m33Lms2Rgb[0][1]       =   0; 
        pKs->ksOMap.m33Lms2Rgb[0][2]       =   0; 
        pKs->ksOMap.m33Lms2Rgb[1][0]       =   0; 
        pKs->ksOMap.m33Lms2Rgb[1][1]       =   0; 
        pKs->ksOMap.m33Lms2Rgb[1][2]       =   0; 
        pKs->ksOMap.m33Lms2Rgb[2][0]       =   0; 
        pKs->ksOMap.m33Lms2Rgb[2][1]       =   0; 
        pKs->ksOMap.m33Lms2Rgb[2][2]       =   0; 

        pKs->ksOMap.m33Rgb2YuvScale2P      =   0; 
        pKs->ksOMap.m33Rgb2Yuv[0][0]       =   0; 
        pKs->ksOMap.m33Rgb2Yuv[0][1]       =   0; 
        pKs->ksOMap.m33Rgb2Yuv[0][2]       =   0; 
        pKs->ksOMap.m33Rgb2Yuv[1][0]       =   0; 
        pKs->ksOMap.m33Rgb2Yuv[1][1]       =   0; 
        pKs->ksOMap.m33Rgb2Yuv[1][2]       =   0; 
        pKs->ksOMap.m33Rgb2Yuv[2][0]       =   0; 
        pKs->ksOMap.m33Rgb2Yuv[2][1]       =   0; 
        pKs->ksOMap.m33Rgb2Yuv[2][2]       =   0; 
        pKs->ksOMap.v3Rgb2YuvOff[0]        =   0; 
        pKs->ksOMap.v3Rgb2YuvOff[1]        =   0; 
        pKs->ksOMap.v3Rgb2YuvOff[2]        =   0; 
    }
    else if(DmMode == VDP_HDR_DM_CFG_12BIT_YUV2RGB)
    {
        pKs->ksIMap.eotfParam.bdp =12;
    }
    else if(DmMode == VDP_HDR_DM_CFG_14BIT_YUV2RGB)
    {
        pKs->ksIMap.eotfParam.bdp =14;
    }

    else if(DmMode == VDP_HDR_DM_CFG_RAND)
    {

        pKs->ksIMap.m33Yuv2RgbScale2P      =   sGetRandEx(5,31);              
        pKs->ksIMap.m33Yuv2Rgb[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Yuv2Rgb[2][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.v3Yuv2RgbOffInRgb[0]   =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);              
        pKs->ksIMap.v3Yuv2RgbOffInRgb[1]   =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);              
        pKs->ksIMap.v3Yuv2RgbOffInRgb[2]   =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);              

        pKs->ksIMap.m33Rgb2LmsScale2P      =   sGetRandEx(0,31);              
        pKs->ksIMap.m33Rgb2Lms[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Rgb2Lms[2][2]       =   sGetRandEx(-32768,32767);        

        pKs->ksIMap.m33Lms2IptScale2P      =   sGetRandEx(0,31);              
        pKs->ksIMap.m33Lms2Ipt[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksIMap.m33Lms2Ipt[2][2]       =   sGetRandEx(-32768,32767);    


        pKs->ksOMap.m33Ipt2LmsScale2P      =   sGetRandEx(1,31);              
        pKs->ksOMap.m33Ipt2Lms[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Ipt2Lms[2][2]       =   sGetRandEx(-32768,32767);    

        pKs->ksOMap.m33Lms2RgbScale2P      =   sGetRandEx(0,31);              
        pKs->ksOMap.m33Lms2Rgb[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Lms2Rgb[2][2]       =   sGetRandEx(-32768,32767);              

        pKs->ksOMap.m33Rgb2YuvScale2P      =   sGetRandEx(0,31);              
        pKs->ksOMap.m33Rgb2Yuv[0][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[0][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[0][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[1][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[1][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[1][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[2][0]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[2][1]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.m33Rgb2Yuv[2][2]       =   sGetRandEx(-32768,32767);              
        pKs->ksOMap.v3Rgb2YuvOff[0]        =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);
        pKs->ksOMap.v3Rgb2YuvOff[1]        =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);
        pKs->ksOMap.v3Rgb2YuvOff[2]        =   sGetRandEx(-(0x7FFFFFFF),0x7FFFFFFF);
    }
#if EDA_TEST
	memcpy(&g_pKs, pKs, sizeof(DmKsFxp_t));
#endif

#if 0

    cout        << " [drv] " << " pKs->rowPitchNum                 =" <<dec<< (HI_S32)pKs->rowPitchNum                 << endl;
    cout        << " [drv] " << " pKs->frmBuf0                     =" <<dec<< (HI_U32)pKs->rowPitchNum                 << endl;
    cout        << " [drv] " << " pKs->frmBuf0                     =" <<dec<< (HI_U32)pKs->rowPitchNum                 << endl;
    cout        << " [drv] " << " pKs->frmBuf0                     =" <<dec<< (HI_U32)pKs->rowPitchNum                 << endl;
    cout        << " [drv] " << " pKs->ksUds.chrmIn                =" <<dec<< (HI_S32)pKs->ksUds.chrmIn                << endl;
    cout        << " [drv] " << " pKs->ksUds.chrmOut               =" <<dec<< (HI_S32)pKs->ksUds.chrmOut               << endl;
    cout        << " [drv] " << " pKs->ksUds.minUs                 =" <<dec<< (HI_S32)pKs->ksUds.minUs                 << endl;
    cout        << " [drv] " << " pKs->ksUds.maxUs                 =" <<dec<< (HI_S32)pKs->ksUds.maxUs                 << endl;
    cout        << " [drv] " << " pKs->ksUds.minDs                 =" <<dec<< (HI_S32)pKs->ksUds.minDs                 << endl;
    cout        << " [drv] " << " pKs->ksUds.maxDs                 =" <<dec<< (HI_S32)pKs->ksUds.maxDs                 << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUsHalfSize =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUsHalfSize << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUsScale2P  =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUsScale2P  << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs0_m[0]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs0_m[0]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs0_m[1]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs0_m[1]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs0_m[2]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs0_m[2]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs0_m[3]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs0_m[3]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs0_m[4]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs0_m[4]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs0_m[5]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs0_m[5]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs0_m[6]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs0_m[6]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs0_m[7]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs0_m[7]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs1_m[0]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs1_m[0]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs1_m[1]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs1_m[1]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs1_m[2]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs1_m[2]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs1_m[3]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs1_m[3]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs1_m[4]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs1_m[4]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs1_m[5]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs1_m[5]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs1_m[6]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs1_m[6]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvRowUs1_m[7]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvRowUs1_m[7]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUsHalfSize =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUsHalfSize << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUsScale2P  =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUsScale2P  << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUs_m[0]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUs_m[0]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUs_m[1]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUs_m[1]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUs_m[2]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUs_m[2]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUs_m[3]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUs_m[3]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUs_m[4]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUs_m[4]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUs_m[5]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUs_m[5]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUs_m[6]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUs_m[6]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColUs_m[7]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColUs_m[7]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDsRadius   =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDsRadius   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDsScale2P  =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDsScale2P  << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[0]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[0]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[1]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[1]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[2]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[2]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[3]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[3]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[4]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[4]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[5]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[5]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[6]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[6]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[7]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[7]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[8]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[8]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[9]    =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[9]    << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[10]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[10]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[11]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[11]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[12]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[12]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[13]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[13]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[14]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[14]   << endl;
    cout        << " [drv] " << " pKs->ksUds.filterUvColDs_m[15]   =" <<dec<< (HI_S32)pKs->ksUds.filterUvColDs_m[15]   << endl;
    cout        << " [drv] " << " pKs->ksDmCtrl.mainIn             =" <<dec<< (HI_S32)pKs->ksDmCtrl.mainIn             << endl;
    cout        << " [drv] " << " pKs->ksDmCtrl.prf                =" <<dec<< (HI_S32)pKs->ksDmCtrl.prf                << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.rowNum            =" <<dec<< (HI_S32)pKs->ksFrmFmtI.rowNum            << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.colNum            =" <<dec<< (HI_S32)pKs->ksFrmFmtI.colNum            << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.dtp               =" <<dec<< (HI_S32)pKs->ksFrmFmtI.dtp               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.weav              =" <<dec<< (HI_S32)pKs->ksFrmFmtI.weav              << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.loc               =" <<dec<< (HI_S32)pKs->ksFrmFmtI.loc               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.bdp               =" <<dec<< (HI_S32)pKs->ksFrmFmtI.bdp               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.chrm              =" <<dec<< (HI_S32)pKs->ksFrmFmtI.chrm              << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.clr               =" <<dec<< (HI_S32)pKs->ksFrmFmtI.clr               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.rowPitch          =" <<dec<< (HI_S32)pKs->ksFrmFmtI.rowPitch          << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.colPitch          =" <<dec<< (HI_S32)pKs->ksFrmFmtI.colPitch          << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtI.rowPitchC         =" <<dec<< (HI_S32)pKs->ksFrmFmtI.rowPitchC         << endl;
    cout        << " [drv] " << " pKs->ksIMap.clr                  =" <<dec<< (HI_S32)pKs->ksIMap.clr                  << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2RgbScale2P    =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2RgbScale2P    << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[0][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[0][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[0][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[1][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[1][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[1][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[2][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[2][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Yuv2Rgb[2][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Yuv2Rgb[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.v3Yuv2RgbOffInRgb[0] =" <<dec<< (HI_S32)pKs->ksIMap.v3Yuv2RgbOffInRgb[0] << endl;
    cout        << " [drv] " << " pKs->ksIMap.v3Yuv2RgbOffInRgb[1] =" <<dec<< (HI_S32)pKs->ksIMap.v3Yuv2RgbOffInRgb[1] << endl;
    cout        << " [drv] " << " pKs->ksIMap.v3Yuv2RgbOffInRgb[2] =" <<dec<< (HI_S32)pKs->ksIMap.v3Yuv2RgbOffInRgb[2] << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.rangeMin   =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.rangeMin   << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.range      =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.range      << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.rangeInv   =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.rangeInv   << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.bdp        =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.bdp        << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.eotf       =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.eotf       << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.gamma      =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.gamma      << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.a          =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.a          << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.b          =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.b          << endl;
    cout        << " [drv] " << " pKs->ksIMap.eotfParam.g          =" <<dec<< (HI_U32)pKs->ksIMap.eotfParam.g          << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2LmsScale2P    =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2LmsScale2P    << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[0][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[0][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[0][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[1][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[1][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[1][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[2][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[2][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Rgb2Lms[2][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Rgb2Lms[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2IptScale2P    =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2IptScale2P    << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[0][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[0][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[0][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[1][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[1][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[1][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[2][0]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[2][1]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.m33Lms2Ipt[2][2]     =" <<dec<< (HI_S32)pKs->ksIMap.m33Lms2Ipt[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMap.iptScale             =" <<dec<< (HI_S32)pKs->ksIMap.iptScale             << endl;
    cout        << " [drv] " << " pKs->ksIMap.v3IptOff[0]          =" <<dec<< (HI_S32)pKs->ksIMap.v3IptOff[0]          << endl;
    cout        << " [drv] " << " pKs->ksIMap.v3IptOff[1]          =" <<dec<< (HI_S32)pKs->ksIMap.v3IptOff[1]          << endl;
    cout        << " [drv] " << " pKs->ksIMap.v3IptOff[2]          =" <<dec<< (HI_S32)pKs->ksIMap.v3IptOff[2]          << endl;
    cout        << " [drv] " << " pKs->ksTMap.tmLutISizeM1         =" <<dec<< (HI_S32)pKs->ksTMap.tmLutISizeM1         << endl;
    cout        << " [drv] " << " pKs->ksTMap.tmLutSSizeM1         =" <<dec<< (HI_S32)pKs->ksTMap.tmLutSSizeM1         << endl;
    cout        << " [drv] " << " pKs->ksTMap.smLutISizeM1         =" <<dec<< (HI_S32)pKs->ksTMap.smLutISizeM1         << endl;
    cout        << " [drv] " << " pKs->ksTMap.smLutSSizeM1         =" <<dec<< (HI_S32)pKs->ksTMap.smLutSSizeM1         << endl;

    cout        << " [drv] " << " pKs->ksFrmFmtG.rowNum            =" <<dec<< (HI_S32)pKs->ksFrmFmtG.rowNum            << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.colNum            =" <<dec<< (HI_S32)pKs->ksFrmFmtG.colNum            << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.dtp               =" <<dec<< (HI_S32)pKs->ksFrmFmtG.dtp               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.weav              =" <<dec<< (HI_S32)pKs->ksFrmFmtG.weav              << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.loc               =" <<dec<< (HI_S32)pKs->ksFrmFmtG.loc               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.bdp               =" <<dec<< (HI_S32)pKs->ksFrmFmtG.bdp               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.chrm              =" <<dec<< (HI_S32)pKs->ksFrmFmtG.chrm              << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.clr               =" <<dec<< (HI_S32)pKs->ksFrmFmtG.clr               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.rowPitch          =" <<dec<< (HI_S32)pKs->ksFrmFmtG.rowPitch          << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.colPitch          =" <<dec<< (HI_S32)pKs->ksFrmFmtG.colPitch          << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtG.rowPitchC         =" <<dec<< (HI_S32)pKs->ksFrmFmtG.rowPitchC         << endl;
    cout        << " [drv] " << " pKs->ksIMapG.clr                  =" <<dec<< (HI_S32)pKs->ksIMapG.clr                  << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2RgbScale2P    =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2RgbScale2P    << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[0][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[0][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[0][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[1][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[1][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[1][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[2][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[2][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Yuv2Rgb[2][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Yuv2Rgb[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.v3Yuv2RgbOffInRgb[0] =" <<dec<< (HI_S32)pKs->ksIMapG.v3Yuv2RgbOffInRgb[0] << endl;
    cout        << " [drv] " << " pKs->ksIMapG.v3Yuv2RgbOffInRgb[1] =" <<dec<< (HI_S32)pKs->ksIMapG.v3Yuv2RgbOffInRgb[1] << endl;
    cout        << " [drv] " << " pKs->ksIMapG.v3Yuv2RgbOffInRgb[2] =" <<dec<< (HI_S32)pKs->ksIMapG.v3Yuv2RgbOffInRgb[2] << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.rangeMin   =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.rangeMin   << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.range      =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.range      << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.rangeInv   =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.rangeInv   << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.bdp        =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.bdp        << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.eotf       =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.eotf       << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.gamma      =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.gamma      << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.a          =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.a          << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.b          =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.b          << endl;
    cout        << " [drv] " << " pKs->ksIMapG.eotfParam.g          =" <<dec<< (HI_U32)pKs->ksIMapG.eotfParam.g          << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2LmsScale2P    =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2LmsScale2P    << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[0][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[0][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[0][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[1][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[1][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[1][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[2][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[2][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Rgb2Lms[2][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Rgb2Lms[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2IptScale2P    =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2IptScale2P    << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[0][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[0][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[0][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[1][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[1][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[1][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[2][0]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[2][1]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.m33Lms2Ipt[2][2]     =" <<dec<< (HI_S32)pKs->ksIMapG.m33Lms2Ipt[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksIMapG.iptScale             =" <<dec<< (HI_S32)pKs->ksIMapG.iptScale             << endl;
    cout        << " [drv] " << " pKs->ksIMapG.v3IptOff[0]          =" <<dec<< (HI_S32)pKs->ksIMapG.v3IptOff[0]          << endl;
    cout        << " [drv] " << " pKs->ksIMapG.v3IptOff[1]          =" <<dec<< (HI_S32)pKs->ksIMapG.v3IptOff[1]          << endl;
    cout        << " [drv] " << " pKs->ksIMapG.v3IptOff[2]          =" <<dec<< (HI_S32)pKs->ksIMapG.v3IptOff[2]          << endl;
    cout        << " [drv] " << " pKs->ksTMapG.tmLutISizeM1         =" <<dec<< (HI_S32)pKs->ksTMapG.tmLutISizeM1         << endl;
    cout        << " [drv] " << " pKs->ksTMapG.tmLutSSizeM1         =" <<dec<< (HI_S32)pKs->ksTMapG.tmLutSSizeM1         << endl;
    cout        << " [drv] " << " pKs->ksTMapG.smLutISizeM1         =" <<dec<< (HI_S32)pKs->ksTMapG.smLutISizeM1         << endl;
    cout        << " [drv] " << " pKs->ksTMapG.smLutSSizeM1         =" <<dec<< (HI_S32)pKs->ksTMapG.smLutSSizeM1         << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2LmsScale2P    =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2LmsScale2P    << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[0][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[0][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[0][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[1][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[1][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[1][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[2][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[2][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Ipt2Lms[2][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Ipt2Lms[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2RgbScale2P    =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2RgbScale2P    << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[0][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[0][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[0][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[1][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[1][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[1][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[2][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[2][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Lms2Rgb[2][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Lms2Rgb[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.oetfParam.oetf       =" <<dec<< (HI_S32)pKs->ksOMap.oetfParam.oetf       << endl;
    cout        << " [drv] " << " pKs->ksOMap.oetfParam.min        =" <<dec<< (HI_S32)pKs->ksOMap.oetfParam.min        << endl;
    cout        << " [drv] " << " pKs->ksOMap.oetfParam.max        =" <<dec<< (HI_S32)pKs->ksOMap.oetfParam.max        << endl;
    cout        << " [drv] " << " pKs->ksOMap.oetfParam.bdp        =" <<dec<< (HI_S32)pKs->ksOMap.oetfParam.bdp        << endl;
    cout        << " [drv] " << " pKs->ksOMap.clr                  =" <<dec<< (HI_S32)pKs->ksOMap.clr                  << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2YuvScale2P    =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2YuvScale2P    << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[0][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[0][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[0][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[0][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[0][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[0][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[1][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[1][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[1][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[1][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[1][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[1][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[2][0]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[2][0]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[2][1]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[2][1]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.m33Rgb2Yuv[2][2]     =" <<dec<< (HI_S32)pKs->ksOMap.m33Rgb2Yuv[2][2]     << endl;
    cout        << " [drv] " << " pKs->ksOMap.v3Rgb2YuvOff[0]      =" <<dec<< (HI_S32)pKs->ksOMap.v3Rgb2YuvOff[0]      << endl;
    cout        << " [drv] " << " pKs->ksOMap.v3Rgb2YuvOff[1]      =" <<dec<< (HI_S32)pKs->ksOMap.v3Rgb2YuvOff[1]      << endl;
    cout        << " [drv] " << " pKs->ksOMap.v3Rgb2YuvOff[2]      =" <<dec<< (HI_S32)pKs->ksOMap.v3Rgb2YuvOff[2]      << endl;
    cout        << " [drv] " << " pKs->ksOMap.iptScale             =" <<dec<< (HI_S32)pKs->ksOMap.iptScale             << endl;
    cout        << " [drv] " << " pKs->ksOMap.v3IptOff[0]          =" <<dec<< (HI_S32)pKs->ksOMap.v3IptOff[0]          << endl;
    cout        << " [drv] " << " pKs->ksOMap.v3IptOff[1]          =" <<dec<< (HI_S32)pKs->ksOMap.v3IptOff[1]          << endl;
    cout        << " [drv] " << " pKs->ksOMap.v3IptOff[2]          =" <<dec<< (HI_S32)pKs->ksOMap.v3IptOff[2]          << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.rowNum            =" <<dec<< (HI_S32)pKs->ksFrmFmtO.rowNum            << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.colNum            =" <<dec<< (HI_S32)pKs->ksFrmFmtO.colNum            << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.dtp               =" <<dec<< (HI_S32)pKs->ksFrmFmtO.dtp               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.weav              =" <<dec<< (HI_S32)pKs->ksFrmFmtO.weav              << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.loc               =" <<dec<< (HI_S32)pKs->ksFrmFmtO.loc               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.bdp               =" <<dec<< (HI_S32)pKs->ksFrmFmtO.bdp               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.chrm              =" <<dec<< (HI_S32)pKs->ksFrmFmtO.chrm              << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.clr               =" <<dec<< (HI_S32)pKs->ksFrmFmtO.clr               << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.rowPitch          =" <<dec<< (HI_S32)pKs->ksFrmFmtO.rowPitch          << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.colPitch          =" <<dec<< (HI_S32)pKs->ksFrmFmtO.colPitch          << endl;
    cout        << " [drv] " << " pKs->ksFrmFmtO.rowPitchC         =" <<dec<< (HI_S32)pKs->ksFrmFmtO.rowPitchC         << endl;
    cout        << " [drv] " << " pKs->bypassShift                 =" <<dec<< (HI_S32)pKs->bypassShift                 << endl;


#endif

	return HI_SUCCESS;
}

HI_S32 VDP_DRV_SetHdrEdaCfg(VDP_HDR_CFG_S *stHdrCfg)
{
#if EDA_TEST
	if(stHdrCfg->bGfxEn)
		g_pKs.ksDmCtrl.secIn = 1;
	else
		g_pKs.ksDmCtrl.secIn = 0;

	//null for SDK
	g_pKs.ksFrmFmtI.colNum     = stHdrCfg->stBlReso.u32Wth;
	g_pKs.ksFrmFmtI.rowNum     = stHdrCfg->stBlReso.u32Hgt;
	g_pKs.ksFrmFmtI.rowPitchC  = stHdrCfg->stBlReso.u32Wth;
	g_pKs.ksFrmFmtI.rowPitch   = stHdrCfg->stBlReso.u32Wth*g_pKs.ksFrmFmtI.colPitch;

	g_pKs.ksFrmFmtO.colNum     = stHdrCfg->stBlReso.u32Wth;
	g_pKs.ksFrmFmtO.rowNum     = stHdrCfg->stBlReso.u32Hgt;
	g_pKs.ksFrmFmtO.rowPitchC  = stHdrCfg->stBlReso.u32Wth;
	g_pKs.ksFrmFmtO.rowPitch   = stHdrCfg->stBlReso.u32Wth*g_pKs.ksFrmFmtO.colPitch;

	if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_HDR10_OUT)
	{
		g_pKs.ksIMap.eotfParam.bdp       = 14;
		g_pKs.ksIMap.eotfParam.range     = (g_pKs.ksIMap.eotfParam.range)*16;
        g_pKs.ksUds.minUs                 =0  ;
        g_pKs.ksUds.maxUs                 =16383;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[0] *= 16;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[1] *= 16;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[2] *= 16;

	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_HDR10_OUT_FPGA)
	{

		//----------------------------------------------------------------------
		//for FPGA test
		//----------------------------------------------------------------------
		//g_pKs.ksOMap.m33Rgb2YuvScale2P = g_pKs.ksOMap.m33Rgb2YuvScale2P - 2;
		g_pKs.ksIMap.eotfParam.bdp       = 14;
		g_pKs.ksIMap.eotfParam.range     = (g_pKs.ksIMap.eotfParam.range)*16;
        g_pKs.ksUds.minUs                 =0  ;
        g_pKs.ksUds.maxUs                 =16383;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[0] *= 16;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[1] *= 16;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[2] *= 16;

		g_pKs.ksOMap.v3Rgb2YuvOff[0] *= 4;
		g_pKs.ksOMap.v3Rgb2YuvOff[1] *= 4;
		g_pKs.ksOMap.v3Rgb2YuvOff[2] *= 4;

		g_pKs.ksOMap.oetfParam.bdp = 12;
	}
	if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_HDR10_OUT_FPGA)
	{
		//----------------------------------------------------------------------
		//for FPGA test
		//----------------------------------------------------------------------
		g_pKs.ksOMap.v3Rgb2YuvOff[0] *= 4;
		g_pKs.ksOMap.v3Rgb2YuvOff[1] *= 4;
		g_pKs.ksOMap.v3Rgb2YuvOff[2] *= 4;

		g_pKs.ksOMap.oetfParam.bdp = 12;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_SDR_OUT || stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_SDR_OUT_CERT)
	{
		g_pKs.ksIMap.eotfParam.bdp        = 14;
		g_pKs.ksIMap.eotfParam.range      = (g_pKs.ksIMap.eotfParam.range)*16;
        g_pKs.ksUds.minUs                 = 0  ;
        g_pKs.ksUds.maxUs                 = 16383;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[0] *= 16;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[1] *= 16;
        g_pKs.ksIMap.v3Yuv2RgbOffInRgb[2] *= 16;

	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_DOVI_IPT_OUT)
	{

		g_pKs.ksIMap.m33Yuv2Rgb[0][0] =  g_pKs.ksIMap.iptScale >> 9;
		g_pKs.ksIMap.m33Yuv2Rgb[0][1] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[0][2] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[1][0] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[1][1] =  (1<<8) ;
		g_pKs.ksIMap.m33Yuv2Rgb[1][2] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][0] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][1] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][2] =  (1<<8) ;

		g_pKs.ksIMap.m33Yuv2RgbScale2P = 11;

		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[0] = 0;
		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[1] = 0;
		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[2] = 0;

		g_pKs.ksIMap.m33Lms2Ipt[0][0] =  1<<8;
		g_pKs.ksIMap.m33Lms2Ipt[0][1] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[0][2] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[1][0] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[1][1] =  g_pKs.ksIMap.iptScale >> 9;
		g_pKs.ksIMap.m33Lms2Ipt[1][2] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][0] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][1] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][2] =  g_pKs.ksIMap.iptScale >> 9;

		g_pKs.ksIMap.m33Lms2IptScale2P = 11;


		if(g_pKs.ksOMap.clr == 3)
		{
			g_pKs.ksOMap.m33Rgb2Yuv[0][0] = g_pKs.ksOMap.iptScale;
			g_pKs.ksOMap.m33Rgb2Yuv[0][1] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[0][2] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[1][0] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[1][1] = g_pKs.ksOMap.iptScale*2;
			g_pKs.ksOMap.m33Rgb2Yuv[1][2] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][0] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][1] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][2] = g_pKs.ksOMap.iptScale*2;

			g_pKs.ksOMap.m33Rgb2YuvScale2P = 16;

			g_pKs.ksOMap.v3Rgb2YuvOff[0] = g_pKs.ksOMap.v3IptOff[0];
			g_pKs.ksOMap.v3Rgb2YuvOff[1] = g_pKs.ksOMap.v3IptOff[1]-g_pKs.ksOMap.iptScale;
			g_pKs.ksOMap.v3Rgb2YuvOff[2] = g_pKs.ksOMap.v3IptOff[2]-g_pKs.ksOMap.iptScale;
		}

	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_OUT_CERT || stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_IPT_OUT)
	{
		g_pKs.ksIMap.m33Yuv2Rgb[0][0] =  g_pKs.ksIMap.iptScale >> 9;
		g_pKs.ksIMap.m33Yuv2Rgb[0][1] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[0][2] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[1][0] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[1][1] =  (1<<8) ;
		g_pKs.ksIMap.m33Yuv2Rgb[1][2] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][0] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][1] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][2] =  (1<<8);

		g_pKs.ksIMap.m33Yuv2RgbScale2P = 11;

		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[0] = 0;
		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[1] = 0;
		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[2] = 0;


		g_pKs.ksIMap.m33Lms2Ipt[0][0] =  1<<8;
		g_pKs.ksIMap.m33Lms2Ipt[0][1] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[0][2] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[1][0] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[1][1] =  g_pKs.ksIMap.iptScale >> 9;
		g_pKs.ksIMap.m33Lms2Ipt[1][2] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][0] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][1] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][2] =  g_pKs.ksIMap.iptScale >> 9;

		g_pKs.ksIMap.m33Lms2IptScale2P = 11;//dut+3


		if(g_pKs.ksOMap.clr == 3)
		{

			g_pKs.ksOMap.m33Rgb2Yuv[0][0] = g_pKs.ksOMap.iptScale;
			g_pKs.ksOMap.m33Rgb2Yuv[0][1] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[0][2] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[1][0] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[1][1] = g_pKs.ksOMap.iptScale*2;
			g_pKs.ksOMap.m33Rgb2Yuv[1][2] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][0] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][1] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][2] = g_pKs.ksOMap.iptScale*2;

			g_pKs.ksOMap.m33Rgb2YuvScale2P = 16;

			g_pKs.ksOMap.v3Rgb2YuvOff[0] = g_pKs.ksOMap.v3IptOff[0];
			g_pKs.ksOMap.v3Rgb2YuvOff[1] = g_pKs.ksOMap.v3IptOff[1]-g_pKs.ksOMap.iptScale;
			g_pKs.ksOMap.v3Rgb2YuvOff[2] = g_pKs.ksOMap.v3IptOff[2]-g_pKs.ksOMap.iptScale;
		}

	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_HDR10_OUT)
	{
		//yuv2rgb
		g_pKs.ksIMap.m33Yuv2Rgb[0][0] =  g_pKs.ksIMap.iptScale >> 9;
		g_pKs.ksIMap.m33Yuv2Rgb[0][1] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[0][2] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[1][0] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[1][1] =  (1<<8) ;
		g_pKs.ksIMap.m33Yuv2Rgb[1][2] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][0] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][1] =  0;
		g_pKs.ksIMap.m33Yuv2Rgb[2][2] =  (1<<8);

		g_pKs.ksIMap.m33Yuv2RgbScale2P = 11;

		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[0] = 0;
		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[1] = 0;
		g_pKs.ksIMap.v3Yuv2RgbOffInRgb[2] = 0;

		//lms2ipt
		g_pKs.ksIMap.m33Lms2Ipt[0][0] =  1<<8;
		g_pKs.ksIMap.m33Lms2Ipt[0][1] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[0][2] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[1][0] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[1][1] =  g_pKs.ksIMap.iptScale >> 9;
		g_pKs.ksIMap.m33Lms2Ipt[1][2] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][0] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][1] =  0;
		g_pKs.ksIMap.m33Lms2Ipt[2][2] =  g_pKs.ksIMap.iptScale >> 9;

		g_pKs.ksIMap.m33Lms2IptScale2P = 11;//dut+3

	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_OUT)
	{
		if(g_pKs.ksOMap.clr == 3)
		{

			g_pKs.ksOMap.m33Rgb2Yuv[0][0] = g_pKs.ksOMap.iptScale;
			g_pKs.ksOMap.m33Rgb2Yuv[0][1] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[0][2] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[1][0] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[1][1] = g_pKs.ksOMap.iptScale*2;
			g_pKs.ksOMap.m33Rgb2Yuv[1][2] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][0] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][1] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][2] = g_pKs.ksOMap.iptScale*2;

			g_pKs.ksOMap.m33Rgb2YuvScale2P = 16;

			g_pKs.ksOMap.v3Rgb2YuvOff[0] = g_pKs.ksOMap.v3IptOff[0];
			g_pKs.ksOMap.v3Rgb2YuvOff[1] = g_pKs.ksOMap.v3IptOff[1]-g_pKs.ksOMap.iptScale;
			g_pKs.ksOMap.v3Rgb2YuvOff[2] = g_pKs.ksOMap.v3IptOff[2]-g_pKs.ksOMap.iptScale;
		}

	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_SDR_IPT_OUT)
	{
		if(g_pKs.ksOMap.clr == 3)
		{

			g_pKs.ksOMap.m33Rgb2Yuv[0][0] = g_pKs.ksOMap.iptScale;
			g_pKs.ksOMap.m33Rgb2Yuv[0][1] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[0][2] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[1][0] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[1][1] = g_pKs.ksOMap.iptScale*2;
			g_pKs.ksOMap.m33Rgb2Yuv[1][2] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][0] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][1] = 0;
			g_pKs.ksOMap.m33Rgb2Yuv[2][2] = g_pKs.ksOMap.iptScale*2;

			g_pKs.ksOMap.m33Rgb2YuvScale2P = 16;

			g_pKs.ksOMap.v3Rgb2YuvOff[0] = g_pKs.ksOMap.v3IptOff[0];
			g_pKs.ksOMap.v3Rgb2YuvOff[1] = g_pKs.ksOMap.v3IptOff[1]-g_pKs.ksOMap.iptScale;
			g_pKs.ksOMap.v3Rgb2YuvOff[2] = g_pKs.ksOMap.v3IptOff[2]-g_pKs.ksOMap.iptScale;
		}

	}
#endif

	return HI_SUCCESS;
}

