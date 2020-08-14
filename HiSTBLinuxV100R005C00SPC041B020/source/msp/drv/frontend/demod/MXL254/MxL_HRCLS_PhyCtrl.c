/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_PhyCtrl.c
 * 
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 11/19/2011
 *
 * DESCRIPTION        : Implementation file for MxL_HRCLS PHY control functions
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include <stdarg.h>

#include "MxL_HRCLS_Common.h"
#include "MxL_HRCLS_PhyCtrl.h"

#ifdef _MNEMONIC_TRACE_ENABLED_
#include "MxL_CTRL_PhyAutoRegMapTable.h"
#endif

// Enable this flag to avoid updating registers with exactly the same value
// that they had before (see MxL_HRCLS_Ctrl_UpdateRegisterField)
// This will reduce number of i2c transactions but may produce issue
// if there is a field that require re-setting with the same value. (To be checked)
#define notMXL_HRCLS_DISCARD_UPDATE_IF_NO_CHANGE
#define not_DOWNLOAD_DEBUG_ENABLED_

// Assure MxL_HRCLS_Dev[] is zero initialized
static MXL_HRCLS_DEV_CONTEXT_T MxL_HRCLS_Dev[MXL_HRCLS_MAX_NUM_DEVICES] = {{0,},};

static MXL_STATUS_E MxL_HRCLS_Ctrl_DownloadFirmwareSegment(MXL_HRCLS_DEV_CONTEXT_T* devContextPtr, UINT8 segmentIndex, UINT8 * addressPtr, const UINT8* dataPtr, UINT32 dataLen, /*@null@*/ MXL_CALLBACK_FN_T fwCallbackFn);
static MXL_STATUS_E MxL_HRCLS_Ctrl_GetDeviceResp(MXL_HRCLS_DEV_CONTEXT_T* devContextPtr, UINT16 subAddr, PFW_RESPONSE_T responsePtr);
static MXL_STATUS_E MxL_HRCLS_Ctrl_SendDownloadCommand(MXL_HRCLS_DEV_CONTEXT_T* devContextPtr, MXL_CMD_ID_E commandId, const void* dataPtr, UINT32 dataLen, UINT16 downloadBlockCnt);

#define MXL_HRCLS_CHAN_NONE       0x00
#define MXL_HRCLS_CHAN_IF_SERDES  0x01
#define MXL_HRCLS_CHAN_IF_DEMOD   0x02
#define MXL_HRCLS_CHAN_IF_OOB     0x04
#define MXL_HRCLS_CHAN_IF_IFOUT   0x08

#ifdef MXL_HRCLS_212_ENABLE
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap212_v2[MXL_HRCLS_IFOUT_CNT_212_V2] = 
          {MXL_HRCLS_IF3, MXL_HRCLS_IF2, MXL_HRCLS_IF1, MXL_HRCLS_IF0};

  #ifdef _MXL_HRCLS_XPT_ENABLED_
    // MXL212_V2 NOMUX_2
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap212_xpt_nomux_4[MXL_HRCLS_DEMODS_CNT_212] = 
            {
              MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap212_xpt_nomux_4[MXL_HRCLS_XPT_OUTPUTS_212] =
            { 
              MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_INVALID
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap212_xpt_nomux_4[MXL_HRCLS_MAX_DFE_CHANNELS_212_XPT_NOMUX_4] =
            {
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_IFOUT}
            };
  #endif
#endif

#ifdef MXL_HRCLS_213_ENABLE
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap213_v2[MXL_HRCLS_IFOUT_CNT_213_V2] = 
          {MXL_HRCLS_IF3, MXL_HRCLS_IF2, MXL_HRCLS_IF1, MXL_HRCLS_IF0};

  #ifdef _MXL_HRCLS_XPT_ENABLED_
    // MXL213_V2 NOMUX_4
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap213_xpt_nomux_4[MXL_HRCLS_DEMODS_CNT_213] = 
            {
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap213_xpt_nomux_4[MXL_HRCLS_XPT_OUTPUTS_213] =
            { 
              MXL_HRCLS_XPT_OUT_2, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_INVALID
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap213_xpt_nomux_4[MXL_HRCLS_MAX_DFE_CHANNELS_213_XPT_NOMUX_4] =
            {
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_IFOUT}
            };
  #endif
#endif


#ifdef MXL_HRCLS_214_ENABLE
#ifdef _MXL_HRCLS_IFOUT_ENABLED_ 
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap214_v2[MXL_HRCLS_IFOUT_CNT_214_V2] = 
          {MXL_HRCLS_IF3, MXL_HRCLS_IF2, MXL_HRCLS_IF1, MXL_HRCLS_IF0};
#endif

  #ifdef _MXL_HRCLS_XPT_ENABLED_
    // MXL214_V2 NOMUX_4
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap214_xpt_nomux_4[MXL_HRCLS_DEMODS_CNT_214] = 
            {
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap214_xpt_nomux_4[MXL_HRCLS_XPT_OUTPUTS_214] =
            { 
              MXL_HRCLS_XPT_OUT_2, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_5
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap214_xpt_nomux_4[MXL_HRCLS_MAX_DFE_CHANNELS_214_XPT_NOMUX_4] =
            {
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_IFOUT},
            };
  #endif
#endif

#ifdef MXL_HRCLS_265_ENABLE
  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap265_5400[MXL_HRCLS_MAX_DFE_CHANNELS_265_5400] = 
          {{MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN11, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN12, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN13, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN14, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN15, MXL_HRCLS_CHAN_IF_SERDES}};

  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap265_5184[MXL_HRCLS_MAX_DFE_CHANNELS_265_5184] = 
          {{MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN11, MXL_HRCLS_CHAN_IF_SERDES}}; 

#ifdef _MXL_HRCLS_DEMOD_ENABLED_
  static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap265[MXL_HRCLS_DEMODS_CNT_265] = 
          {MXL_HRCLS_DEMOD0};
#endif          
#endif

#ifdef MXL_HRCLS_267_ENABLE
  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap267_5400[MXL_HRCLS_MAX_DFE_CHANNELS_267_5400] = 
          {{MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN11, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN12, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN13, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN14, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN15, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN16, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN17, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN18, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN19, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN20, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN21, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN22, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN23, MXL_HRCLS_CHAN_IF_SERDES}}; 

  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap267_5184[MXL_HRCLS_MAX_DFE_CHANNELS_267_5184] = 
          {{MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN11, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN12, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN13, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN14, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN15, MXL_HRCLS_CHAN_IF_SERDES}}; 
  
#ifdef _MXL_HRCLS_DEMOD_ENABLED_  
  static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap267[MXL_HRCLS_DEMODS_CNT_267] = 
          {MXL_HRCLS_DEMOD0};
#endif
#endif           

#ifdef MXL_HRCLS_258_ENABLE
  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap258[MXL_HRCLS_MAX_DFE_CHANNELS_258] = 
          {{MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN11, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN12, MXL_HRCLS_CHAN_IF_DEMOD}};

  static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap258[MXL_HRCLS_DEMODS_CNT_258] = 
          {
            MXL_HRCLS_DEMOD0, MXL_HRCLS_DEMOD1, MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD3,
            MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7,
            MXL_HRCLS_DEMOD8
          };            
#ifdef _MXL_HRCLS_IFOUT_ENABLED_  
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap258_v2[MXL_HRCLS_IFOUT_CNT_258_V2] = 
          {MXL_HRCLS_IF2};
#endif
          
  #ifdef _MXL_HRCLS_XPT_ENABLED_
    // MXL258_V2 NOMUX_4
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap258_xpt_nomux_4[MXL_HRCLS_DEMODS_CNT_258_NOMUX] = 
            {
              MXL_HRCLS_DEMOD0, MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5,
              MXL_HRCLS_DEMOD6
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap258_xpt_nomux_4[MXL_HRCLS_XPT_OUTPUTS_258] =
            { 
              MXL_HRCLS_XPT_OUT_2, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_5
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap258_xpt_nomux_4[MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_NOMUX_4] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };

    // MXL258_V2 MUX_4
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap258_xpt_mux_4[MXL_HRCLS_DEMODS_CNT_258] = 
            {
              MXL_HRCLS_DEMOD0, MXL_HRCLS_DEMOD1, MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD3,
              MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7,
              MXL_HRCLS_DEMOD8
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap258_xpt_mux_4[MXL_HRCLS_XPT_OUTPUTS_258] =
            { 
              MXL_HRCLS_XPT_OUT_2, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_5
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap258_xpt_mux_4[MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_MUX_4] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };


    // MXL258_V2 MUX_2
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap258_xpt_mux_2[MXL_HRCLS_DEMODS_CNT_258] = 
            {
              MXL_HRCLS_DEMOD0, MXL_HRCLS_DEMOD1, MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD3,
              MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7,
              MXL_HRCLS_DEMOD8
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap258_xpt_mux_2[MXL_HRCLS_XPT_OUTPUTS_258] =
            { 
              MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_INVALID
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap258_xpt_mux_2[MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_MUX_2] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };

    // MXL258_V2 PAR            
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap258_xpt_par[MXL_HRCLS_DEMODS_CNT_258] = 
            {
              MXL_HRCLS_DEMOD0, MXL_HRCLS_DEMOD1, MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD3,
              MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7,
              MXL_HRCLS_DEMOD8
            };            
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap258_xpt_par[MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_PAR] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap258_xpt_par[1] =
            { 
              MXL_HRCLS_XPT_OUT_0
            };

  #endif
#endif           

#ifdef MXL_HRCLS_252_ENABLE
  
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap252_v2[MXL_HRCLS_IFOUT_CNT_252_V2] = 
          {MXL_HRCLS_IF2};

  #ifdef _MXL_HRCLS_XPT_ENABLED_
    // MXL252_V2 NOMUX_2
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap252_xpt_nomux_2[MXL_HRCLS_DEMODS_CNT_252] = 
            {
              MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD0
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap252_xpt_nomux_2[MXL_HRCLS_XPT_OUTPUTS_252] =
            { 
              MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_INVALID
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap252_xpt_nomux_2[MXL_HRCLS_MAX_DFE_CHANNELS_252_XPT_NOMUX_2] =
            {
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
            };
    
    // MXL252_V2 PAR
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap252_xpt_par[MXL_HRCLS_DEMODS_CNT_252] = 
            {
              MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD0
            };            
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap252_xpt_par[MXL_HRCLS_MAX_DFE_CHANNELS_252_XPT_PAR] =
            {
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
            };
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap252_xpt_par[1] =
            { 
              MXL_HRCLS_XPT_OUT_0
            };
  #endif
#endif           


#ifdef MXL_HRCLS_254_ENABLE
  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap254[MXL_HRCLS_MAX_DFE_CHANNELS_254] = 
          {{MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_IFOUT},
           {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_IFOUT},
           {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_IFOUT},
           {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_IFOUT}};
  
  static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap254[MXL_HRCLS_DEMODS_CNT_254] = 
          {MXL_HRCLS_DEMOD1, MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD6,
           MXL_HRCLS_DEMOD0};            

#ifdef _MXL_HRCLS_IFOUT_ENABLED_ 
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap254_v1[MXL_HRCLS_IFOUT_CNT_254_V1] = 
           {MXL_HRCLS_IF3, MXL_HRCLS_IF2, MXL_HRCLS_IF1, MXL_HRCLS_IF0};
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap254_v2[MXL_HRCLS_IFOUT_CNT_254_V2] = 
           {MXL_HRCLS_IF3, MXL_HRCLS_IF2, MXL_HRCLS_IF1, MXL_HRCLS_IF0};
#endif

  #ifdef _MXL_HRCLS_XPT_ENABLED_
    // MXL254_V2 NOMUX_4
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap254_xpt_nomux_4[MXL_HRCLS_DEMODS_CNT_254] = 
            {
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6,
              MXL_HRCLS_DEMOD0
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap254_xpt_nomux_4[MXL_HRCLS_XPT_OUTPUTS_254] =
            { 
              MXL_HRCLS_XPT_OUT_2, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_5
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap254_xpt_nomux_4[MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_NOMUX_4] =
            {
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD | MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_IFOUT},
            };

    // MXL254_V2 MUX_2
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap254_xpt_mux_2[MXL_HRCLS_DEMODS_CNT_254] = 
            {
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6,
              MXL_HRCLS_DEMOD0
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap254_xpt_mux_2[MXL_HRCLS_XPT_OUTPUTS_254] =
            { 
              MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_INVALID 
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap254_xpt_mux_2[MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_MUX_2] =
            {
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD | MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_IFOUT},
            };

    // MXL254_V2 MUX_1
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap254_xpt_mux_1[MXL_HRCLS_DEMODS_CNT_254] = 
            {
              MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7, MXL_HRCLS_DEMOD8,
              MXL_HRCLS_DEMOD0
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap254_xpt_mux_1[MXL_HRCLS_XPT_OUTPUTS_254] =
            { 
              MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_INVALID 
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap254_xpt_mux_1[MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_MUX_1] =
            {
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD | MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_IFOUT},
            };
    
    // MXL254_V2 PAR
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap254_xpt_par[MXL_HRCLS_DEMODS_CNT_254] = 
            {
              MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7, MXL_HRCLS_DEMOD8,
              MXL_HRCLS_DEMOD0
            };            
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap254_xpt_par[MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_PAR] =
            {
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD | MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_IFOUT},
              {MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_IFOUT},
            };
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap254_xpt_par[1] =
            { 
              MXL_HRCLS_XPT_OUT_0
            };
  #endif
#endif           

#ifdef MXL_HRCLS_255_ENABLE
  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap255[MXL_HRCLS_MAX_DFE_CHANNELS_255] = 
          {{MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD}};
  
  static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap255[MXL_HRCLS_DEMODS_CNT_255] = 
          {MXL_HRCLS_DEMOD1, MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD6,
           MXL_HRCLS_DEMOD0};            
  
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap255_v2[MXL_HRCLS_IFOUT_CNT_255_V2] = 
          {MXL_HRCLS_IF2};

  #ifdef _MXL_HRCLS_XPT_ENABLED_
    // MXL255_V2 NOMUX_4
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap255_xpt_nomux_4[MXL_HRCLS_DEMODS_CNT_255] = 
            {
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6,
              MXL_HRCLS_DEMOD0
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap255_xpt_nomux_4[MXL_HRCLS_XPT_OUTPUTS_255] =
            { 
              MXL_HRCLS_XPT_OUT_2, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_5
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap255_xpt_nomux_4[MXL_HRCLS_MAX_DFE_CHANNELS_255_XPT_NOMUX_4] =
            {
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };

  #endif
#endif           

#ifdef MXL_HRCLS_256_ENABLE
  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap256[MXL_HRCLS_MAX_DFE_CHANNELS_256] = 
          {{MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_DEMOD},
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_DEMOD} };
  
  static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap256[MXL_HRCLS_DEMODS_CNT_256] = 
          {MXL_HRCLS_DEMOD0, MXL_HRCLS_DEMOD1, MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD3,
           MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6};            

#ifdef _MXL_HRCLS_IFOUT_ENABLED_  
  static MXL_HRCLS_IF_ID_E mxl_hrcls_ifoutMap256_v2[MXL_HRCLS_IFOUT_CNT_256_V2] = 
          {MXL_HRCLS_IF2};
#endif

  #ifdef _MXL_HRCLS_XPT_ENABLED_
    // MXL256_V2 NOMUX_4
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap256_xpt_nomux_4[MXL_HRCLS_DEMODS_CNT_256_NOMUX] = 
            { 
              MXL_HRCLS_DEMOD0,
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6
            };
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap256_xpt_nomux_4[MXL_HRCLS_XPT_OUTPUTS_256] =
            { 
              MXL_HRCLS_XPT_OUT_2, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_5
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap256_xpt_nomux_4[MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_NOMUX_4] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };


    // MXL256_V2 MUX_4
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap256_xpt_mux_4[MXL_HRCLS_DEMODS_CNT_256] = 
            { 
              MXL_HRCLS_DEMOD0,
              MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5,
              MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap256_xpt_mux_4[MXL_HRCLS_XPT_OUTPUTS_256] =
            { 
              MXL_HRCLS_XPT_OUT_2, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_5
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap256_xpt_mux_4[MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_4] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };


    // MXL256_V2 MUX_3
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap256_xpt_mux_3[MXL_HRCLS_DEMODS_CNT_256] = 
            {
              MXL_HRCLS_DEMOD0,
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6,
              MXL_HRCLS_DEMOD7, MXL_HRCLS_DEMOD8
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap256_xpt_mux_3[MXL_HRCLS_XPT_OUTPUTS_256] =
            { 
              MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_5
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap256_xpt_mux_3[MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_3] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };


    // MXL256_V2 MUX_2
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap256_xpt_mux_2[MXL_HRCLS_DEMODS_CNT_256] = 
            {
              MXL_HRCLS_DEMOD0,
              MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5,
              MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap256_xpt_mux_2[MXL_HRCLS_XPT_OUTPUTS_256] =
            { 
              MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_INVALID
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap256_xpt_mux_2[MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_2] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };


    // MXL256_V2 MUX_2b
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap256_xpt_mux_2b[MXL_HRCLS_DEMODS_CNT_256] = 
            {
              MXL_HRCLS_DEMOD0,
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6,
              MXL_HRCLS_DEMOD7, MXL_HRCLS_DEMOD8
            };            
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap256_xpt_mux_2b[MXL_HRCLS_XPT_OUTPUTS_256] =
            { 
              MXL_HRCLS_XPT_OUT_INVALID, MXL_HRCLS_XPT_OUT_3, MXL_HRCLS_XPT_OUT_4, MXL_HRCLS_XPT_OUT_INVALID
            };
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap256_xpt_mux_2b[MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_2b] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };
    
    // MXL256_V2 PAR
    static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap256_xpt_par[MXL_HRCLS_DEMODS_CNT_256] = 
            {
              MXL_HRCLS_DEMOD0,
              MXL_HRCLS_DEMOD3, MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6,
              MXL_HRCLS_DEMOD7, MXL_HRCLS_DEMOD8
            };            
    static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap256_xpt_par[MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_PAR] =
            {
              {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_DEMOD},
              {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_IFOUT}
            };
    static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_hrcls_outputMap256_xpt_par[1] =
            { 
              MXL_HRCLS_XPT_OUT_0
            };
  #endif
#endif           

#ifdef MXL_HRCLS_269_ENABLE
  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap269_5400[MXL_HRCLS_MAX_DFE_CHANNELS_269_5400] = 
          {{MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN11, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN12, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN13, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN14, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN15, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN16, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN17, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN18, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN19, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN20, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN21, MXL_HRCLS_CHAN_IF_SERDES},  
           {MXL_HRCLS_CHAN22, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN23, MXL_HRCLS_CHAN_IF_SERDES}};
  
  static MXL_HRCLS_CHAN_T mxl_hrcls_dfeChanMap269_5184[MXL_HRCLS_MAX_DFE_CHANNELS_269_5184] = 
          {{MXL_HRCLS_CHAN0, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN1, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN2, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN3, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN4, MXL_HRCLS_CHAN_IF_OOB | MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN5, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN6, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN7, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN8, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN9, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN10, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN11, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN12, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN13, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN14, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN15, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN16, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN17, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN18, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN19, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN20, MXL_HRCLS_CHAN_IF_SERDES}, 
           {MXL_HRCLS_CHAN21, MXL_HRCLS_CHAN_IF_SERDES},  
           {MXL_HRCLS_CHAN22, MXL_HRCLS_CHAN_IF_SERDES},
           {MXL_HRCLS_CHAN23, MXL_HRCLS_CHAN_IF_SERDES}};
  
  static MXL_HRCLS_DMD_ID_E mxl_hrcls_demodMap269[MXL_HRCLS_DEMODS_CNT_269] = 
          {
            MXL_HRCLS_DEMOD0, MXL_HRCLS_DEMOD1, MXL_HRCLS_DEMOD2, MXL_HRCLS_DEMOD3,
            MXL_HRCLS_DEMOD4, MXL_HRCLS_DEMOD5, MXL_HRCLS_DEMOD6, MXL_HRCLS_DEMOD7,
            MXL_HRCLS_DEMOD8
          };            
#endif

#ifdef _MXL_HRCLS_TSENS_V2_ENABLED_        
static UINT8 MxL_HRCLS_TemperatureCodeMap[MXL_HRCLS_TEMP_CODE_MAP_LEN] = {1 , 7 , 13 , 19 , 25 , 31 , 36 , 39 , 41 , 43 , 45 , 47 , 49 , 51 , 53 , 55 , 57, 59, 61, 63, 65, 67, 69, 71, 73, 76, 81, 87, 93, 99, 105, 111};
#endif

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_InitializeDeviceParameters
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 01/30/2012
--|
--| DESCRIPTION   : This function initializes DFE channels map according to selected
--|                 device type and serdes line rate 
--|
--| INPUTS        : device context, device type 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_InitializeDeviceParameters(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  UINT8 status = MXL_SUCCESS;

  MXLENTERSTR;
  
  devContextPtr->dfeMaxChannels = MXL_HRCLS_MAX_CHANNELS_MIN;

  if (devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID)
  {
    switch (devContextPtr->chipVersion)
    {
      case 1: devContextPtr->dfeMaxChannels = MXL_HRCLS_MAX_CHANNELS_HERCULES_V1; break;
      case 2: 
      case 3: devContextPtr->dfeMaxChannels = MXL_HRCLS_MAX_CHANNELS_HERCULES_V2; break;
      default:  devContextPtr->dfeMaxChannels = MXL_HRCLS_MAX_CHANNELS_MIN; break;
    }
  }
  if (devContextPtr->chipId == MXL_HRCLS_MINOS_CHIP_ID)
  {
    devContextPtr->dfeMaxChannels = MXL_HRCLS_MAX_CHANNELS_MINOS_V1;
  }

#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
  devContextPtr->xpt.supported = MXL_FALSE;
#endif
  devContextPtr->clockOutSupported = MXL_FALSE;
  switch (devContextPtr->deviceType)
  {
#ifdef MXL_HRCLS_265_ENABLE      
    case MXL_HRCLS_DEVICE_265:
    {
      UINT8 i;
      devContextPtr->dfeChanMap = (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)?mxl_hrcls_dfeChanMap265_5400:mxl_hrcls_dfeChanMap265_5184; 
      devContextPtr->dfeChanCount = (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)?MXL_HRCLS_MAX_DFE_CHANNELS_265_5400:MXL_HRCLS_MAX_DFE_CHANNELS_265_5184; 
      devContextPtr->clockOutSupported = MXL_TRUE;
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
      devContextPtr->serDesDSLanesCnt = MXL_HRCLS_SERDES_DS_LANES_CNT_265;
      for (i = 0; i < devContextPtr->serDesDSLanesCnt; i++)
      {
        if (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)
        {
          devContextPtr->serDesSlotsPerLane[i] = (devContextPtr->serDesMode[i] == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)?MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_2700:MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_5400;
        }
        else devContextPtr->serDesSlotsPerLane[i] = MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_5184; 
      }
#endif      
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
      devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_265;
      devContextPtr->demodsMap = mxl_hrcls_demodMap265;
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_HRCLS_OOB_SUPPORT_265;
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_265;      
  #endif            
      devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_265; 
      devContextPtr->autoSpectrumInversionSupported = (devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID)?MXL_HRCLS_AUTO_SPECTRUM_INV_265_HERCULES:MXL_HRCLS_AUTO_SPECTRUM_INV_265_MINOS;
#endif        
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
      devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_265;
      devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_265; 
      devContextPtr->ifOutMap = NULL;
#endif
      break;
    }
#endif      

#ifdef MXL_HRCLS_267_ENABLE      
    case MXL_HRCLS_DEVICE_267:
    {
      UINT8 i;
      devContextPtr->dfeChanMap = (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)?mxl_hrcls_dfeChanMap267_5400:mxl_hrcls_dfeChanMap267_5184; 
      devContextPtr->dfeChanCount = (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)?MXL_HRCLS_MAX_DFE_CHANNELS_267_5400:MXL_HRCLS_MAX_DFE_CHANNELS_267_5184; 
      devContextPtr->clockOutSupported = MXL_TRUE;
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
      devContextPtr->serDesDSLanesCnt = MXL_HRCLS_SERDES_DS_LANES_CNT_267;
      for (i = 0; i < devContextPtr->serDesDSLanesCnt; i++)
      {
        if (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)
        {
          devContextPtr->serDesSlotsPerLane[i] = (devContextPtr->serDesMode[i] == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)?MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_2700:MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_5400;
        }
        else devContextPtr->serDesSlotsPerLane[i] = MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_5184; 
      }
#endif      
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
      devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_267;
      devContextPtr->demodsMap = mxl_hrcls_demodMap267;
  #ifdef _MXL_HRCLS_OOB_ENABLED_
      devContextPtr->oobSupported = MXL_HRCLS_OOB_SUPPORT_267;
      devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_267;      
  #endif            
      devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_267; 
      devContextPtr->autoSpectrumInversionSupported = (devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID)?MXL_HRCLS_AUTO_SPECTRUM_INV_267_HERCULES:MXL_HRCLS_AUTO_SPECTRUM_INV_267_MINOS;
#endif      
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
      devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_267;
      devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_267; 
      devContextPtr->ifOutMap = NULL;
#endif
      break;
    }
#endif      

#ifdef MXL_HRCLS_258_ENABLE      
    case MXL_HRCLS_DEVICE_258:
        devContextPtr->dfeChanMap = mxl_hrcls_dfeChanMap258; 
        devContextPtr->dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_258; 
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
        devContextPtr->serDesDSLanesCnt = MXL_HRCLS_SERDES_DS_LANES_CNT_258;
#endif      
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
        devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_258;
        devContextPtr->demodsMap = mxl_hrcls_demodMap258; 
        devContextPtr->autoSpectrumInversionSupported = (devContextPtr->chipVersion >= 2)?MXL_HRCLS_AUTO_SPECTRUM_INV_258_V2:MXL_HRCLS_AUTO_SPECTRUM_INV_258;
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_HRCLS_OOB_SUPPORT_258;
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_258;      
  #endif            
        devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_258; 
  #ifdef _MXL_HRCLS_XPT_ENABLED_
        if ((devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID) && (devContextPtr->chipVersion >= 2))
        {
          devContextPtr->xpt.supported = MXL_TRUE;
          devContextPtr->xpt.modesCnt = 5;
          devContextPtr->xpt.outputsCnt = MXL_HRCLS_XPT_OUTPUTS_258;
          devContextPtr->xpt.fourWireModeSupported = (MXL_BOOL_E)((devContextPtr->chipVersion >= 3)?1:0);
          devContextPtr->xpt.commonClockEnabled = MXL_FALSE;
          
          devContextPtr->xpt.currentMode = NULL;
          devContextPtr->xpt.modes[0].mode = MXL_HRCLS_XPT_MODE_NO_MUX_4; 
          devContextPtr->xpt.modes[0].demodsMap = mxl_hrcls_demodMap258_xpt_nomux_4; 
          devContextPtr->xpt.modes[0].demodsCnt = MXL_HRCLS_DEMODS_CNT_258_NOMUX; 
          devContextPtr->xpt.modes[0].outputMap = mxl_hrcls_outputMap258_xpt_nomux_4; 
          devContextPtr->xpt.modes[0].xptMap    = MXL_HRCLS_XPT_MAP_1_TO_1; 
          devContextPtr->xpt.modes[0].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_NOMUX_4; 
          devContextPtr->xpt.modes[0].dfeChanMap = mxl_hrcls_dfeChanMap258_xpt_nomux_4;
          devContextPtr->xpt.modes[0].demodScheme = MXL_HRCLS_XPT_258_SCHEME_NO_MUX_4;

          devContextPtr->xpt.modes[1].mode = MXL_HRCLS_XPT_MODE_MUX_4; 
          devContextPtr->xpt.modes[1].demodsMap = mxl_hrcls_demodMap258_xpt_mux_4; 
          devContextPtr->xpt.modes[1].demodsCnt = MXL_HRCLS_DEMODS_CNT_258; 
          devContextPtr->xpt.modes[1].outputMap = mxl_hrcls_outputMap258_xpt_mux_4; 
          devContextPtr->xpt.modes[1].xptMap    = MXL_HRCLS_XPT_MAP_2_TO_1; 
          devContextPtr->xpt.modes[1].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_MUX_4; 
          devContextPtr->xpt.modes[1].dfeChanMap = mxl_hrcls_dfeChanMap258_xpt_mux_4;
          devContextPtr->xpt.modes[1].demodScheme = MXL_HRCLS_XPT_258_SCHEME_MUX_4;

          devContextPtr->xpt.modes[2].mode = MXL_HRCLS_XPT_MODE_MUX_2; 
          devContextPtr->xpt.modes[2].demodsMap = mxl_hrcls_demodMap258_xpt_mux_2; 
          devContextPtr->xpt.modes[2].demodsCnt = MXL_HRCLS_DEMODS_CNT_258; 
          devContextPtr->xpt.modes[2].outputMap = mxl_hrcls_outputMap258_xpt_mux_2; 
          devContextPtr->xpt.modes[2].xptMap    = MXL_HRCLS_XPT_MAP_4_TO_1; 
          devContextPtr->xpt.modes[2].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_MUX_2; 
          devContextPtr->xpt.modes[2].dfeChanMap = mxl_hrcls_dfeChanMap258_xpt_mux_2;
          devContextPtr->xpt.modes[2].demodScheme = MXL_HRCLS_XPT_258_SCHEME_MUX_2;

          devContextPtr->xpt.modes[3].mode = MXL_HRCLS_XPT_MODE_PARALLEL; 
          devContextPtr->xpt.modes[3].demodsMap = mxl_hrcls_demodMap258_xpt_par; 
          devContextPtr->xpt.modes[3].demodsCnt = MXL_HRCLS_DEMODS_CNT_258; 
          devContextPtr->xpt.modes[3].outputMap = mxl_hrcls_outputMap258_xpt_par; 
          devContextPtr->xpt.modes[3].xptMap    = MXL_HRCLS_XPT_MAP_PARALLEL; 
          devContextPtr->xpt.modes[3].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_PAR; 
          devContextPtr->xpt.modes[3].dfeChanMap = mxl_hrcls_dfeChanMap258_xpt_par;
          devContextPtr->xpt.modes[3].demodScheme = MXL_HRCLS_XPT_258_SCHEME_PARALLEL;

          devContextPtr->xpt.modes[4].mode = MXL_HRCLS_XPT_MODE_CABLECARD; 
          devContextPtr->xpt.modes[4].demodsMap = mxl_hrcls_demodMap258_xpt_par; 
          devContextPtr->xpt.modes[4].demodsCnt = MXL_HRCLS_DEMODS_CNT_258; 
          devContextPtr->xpt.modes[4].outputMap = mxl_hrcls_outputMap258_xpt_par; 
          devContextPtr->xpt.modes[4].xptMap    = MXL_HRCLS_XPT_MAP_CABLECARD; 
          devContextPtr->xpt.modes[4].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_PAR; 
          devContextPtr->xpt.modes[4].dfeChanMap = mxl_hrcls_dfeChanMap258_xpt_par;
          devContextPtr->xpt.modes[4].demodScheme = MXL_HRCLS_XPT_258_SCHEME_PARALLEL;

          // Set default XPT mode
          if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, MXL_HRCLS_XPT_MODE_NO_MUX_4) == MXL_FALSE)
          {
            status = MXL_NOT_SUPPORTED;
          }
        }
  #endif
#endif      
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
      if (devContextPtr->chipVersion >= 2)
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_258_V2;
        devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_258_V2;
        devContextPtr->ifOutMap = mxl_hrcls_ifoutMap258_v2; 
      }
      else
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_258;
        devContextPtr->ifOutCnt = 0;
        devContextPtr->ifOutMap = NULL;
      }
#endif
      break;
#endif      

#ifdef MXL_HRCLS_252_ENABLE      
    case MXL_HRCLS_DEVICE_252:
        devContextPtr->dfeChanMap = NULL; 
        devContextPtr->dfeChanCount = 0; 
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
        devContextPtr->serDesDSLanesCnt = MXL_HRCLS_SERDES_DS_LANES_CNT_252;
#endif      
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
        devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_252;
        devContextPtr->demodsMap = mxl_hrcls_demodMap252_xpt_nomux_2; 
        devContextPtr->autoSpectrumInversionSupported = MXL_HRCLS_AUTO_SPECTRUM_INV_252_V2;
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_HRCLS_OOB_SUPPORT_252;
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_252;      
  #endif            
        devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_252; 
  #ifdef _MXL_HRCLS_XPT_ENABLED_
        if ((devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID) && (devContextPtr->chipVersion >= 2))
        {
          devContextPtr->xpt.supported = MXL_TRUE;
          devContextPtr->xpt.modesCnt = 3;
          devContextPtr->xpt.outputsCnt = MXL_HRCLS_XPT_OUTPUTS_252;
          devContextPtr->xpt.fourWireModeSupported = (MXL_BOOL_E)((devContextPtr->chipVersion >= 3)?1:0);
          devContextPtr->xpt.commonClockEnabled = MXL_FALSE;

          devContextPtr->xpt.currentMode = NULL;
          devContextPtr->xpt.modes[0].mode = MXL_HRCLS_XPT_MODE_NO_MUX_2;
          devContextPtr->xpt.modes[0].demodsMap = mxl_hrcls_demodMap252_xpt_nomux_2;
          devContextPtr->xpt.modes[0].demodsCnt = MXL_HRCLS_DEMODS_CNT_252;
          devContextPtr->xpt.modes[0].outputMap = mxl_hrcls_outputMap252_xpt_nomux_2; 
          devContextPtr->xpt.modes[0].xptMap    = MXL_HRCLS_XPT_MAP_1_TO_1; 
          devContextPtr->xpt.modes[0].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_252_XPT_NOMUX_2; 
          devContextPtr->xpt.modes[0].dfeChanMap = mxl_hrcls_dfeChanMap252_xpt_nomux_2;
          devContextPtr->xpt.modes[0].demodScheme = MXL_HRCLS_XPT_252_SCHEME_NO_MUX_2;
           
          devContextPtr->xpt.modes[1].mode = MXL_HRCLS_XPT_MODE_PARALLEL; 
          devContextPtr->xpt.modes[1].demodsMap = mxl_hrcls_demodMap252_xpt_par;
          devContextPtr->xpt.modes[1].demodsCnt = MXL_HRCLS_DEMODS_CNT_252;
          devContextPtr->xpt.modes[1].outputMap = mxl_hrcls_outputMap252_xpt_par;
          devContextPtr->xpt.modes[1].xptMap    = MXL_HRCLS_XPT_MAP_PARALLEL; 
          devContextPtr->xpt.modes[1].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_252_XPT_PAR; 
          devContextPtr->xpt.modes[1].dfeChanMap = mxl_hrcls_dfeChanMap252_xpt_par;
          devContextPtr->xpt.modes[1].demodScheme = MXL_HRCLS_XPT_252_SCHEME_PARALLEL;
          
          devContextPtr->xpt.modes[2].mode = MXL_HRCLS_XPT_MODE_CABLECARD; 
          devContextPtr->xpt.modes[2].demodsMap = mxl_hrcls_demodMap252_xpt_par;
          devContextPtr->xpt.modes[2].demodsCnt = MXL_HRCLS_DEMODS_CNT_252;
          devContextPtr->xpt.modes[2].outputMap = mxl_hrcls_outputMap252_xpt_par;
          devContextPtr->xpt.modes[2].xptMap    = MXL_HRCLS_XPT_MAP_CABLECARD; 
          devContextPtr->xpt.modes[2].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_252_XPT_PAR; 
          devContextPtr->xpt.modes[2].dfeChanMap = mxl_hrcls_dfeChanMap252_xpt_par;
          devContextPtr->xpt.modes[2].demodScheme = MXL_HRCLS_XPT_252_SCHEME_PARALLEL;

          // Set default XPT mode
          if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, MXL_HRCLS_XPT_MODE_NO_MUX_2) == MXL_FALSE)
          {
            status = MXL_NOT_SUPPORTED;
          }
        }
  #endif
#endif      
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
      if (devContextPtr->chipVersion >= 2)
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_252_V2;
        devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_252_V2;
        devContextPtr->ifOutMap = mxl_hrcls_ifoutMap252_v2; 
      }
      else
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_252;
        devContextPtr->ifOutCnt = 0;
        devContextPtr->ifOutMap = NULL; 
      }
#endif
      break;
#endif      
#ifdef MXL_HRCLS_254_ENABLE      
    case MXL_HRCLS_DEVICE_254:
        devContextPtr->dfeChanMap = mxl_hrcls_dfeChanMap254; 
        devContextPtr->dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_254; 
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
        devContextPtr->serDesDSLanesCnt = MXL_HRCLS_SERDES_DS_LANES_CNT_254;
#endif      
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
        devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_254;
        devContextPtr->demodsMap = mxl_hrcls_demodMap254; 
        devContextPtr->autoSpectrumInversionSupported = (devContextPtr->chipVersion >= 2)?MXL_HRCLS_AUTO_SPECTRUM_INV_254_V2:MXL_HRCLS_AUTO_SPECTRUM_INV_254;
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_HRCLS_OOB_SUPPORT_254;
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_254;      
  #endif            
        devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_254; 
  #ifdef _MXL_HRCLS_XPT_ENABLED_
        if ((devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID) && (devContextPtr->chipVersion >= 2))
        {
          devContextPtr->xpt.supported = MXL_TRUE;
          devContextPtr->xpt.modesCnt = 5;
          devContextPtr->xpt.outputsCnt = MXL_HRCLS_XPT_OUTPUTS_254;
          devContextPtr->xpt.fourWireModeSupported = (MXL_BOOL_E)((devContextPtr->chipVersion >= 3)?1:0);
          devContextPtr->xpt.commonClockEnabled = MXL_FALSE;

          devContextPtr->xpt.currentMode = NULL;
          devContextPtr->xpt.modes[0].mode = MXL_HRCLS_XPT_MODE_NO_MUX_4;
          devContextPtr->xpt.modes[0].demodsMap = mxl_hrcls_demodMap254_xpt_nomux_4;
          devContextPtr->xpt.modes[0].demodsCnt = MXL_HRCLS_DEMODS_CNT_254;
          devContextPtr->xpt.modes[0].outputMap = mxl_hrcls_outputMap254_xpt_nomux_4; 
          devContextPtr->xpt.modes[0].xptMap    = MXL_HRCLS_XPT_MAP_1_TO_1; 
          devContextPtr->xpt.modes[0].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_NOMUX_4; 
          devContextPtr->xpt.modes[0].dfeChanMap = mxl_hrcls_dfeChanMap254_xpt_nomux_4;
          devContextPtr->xpt.modes[0].demodScheme =  MXL_HRCLS_XPT_254_SCHEME_NO_MUX_4;
           
          devContextPtr->xpt.modes[1].mode = MXL_HRCLS_XPT_MODE_MUX_2; 
          devContextPtr->xpt.modes[1].demodsMap = mxl_hrcls_demodMap254_xpt_mux_2;
          devContextPtr->xpt.modes[1].demodsCnt = MXL_HRCLS_DEMODS_CNT_254;
          devContextPtr->xpt.modes[1].outputMap = mxl_hrcls_outputMap254_xpt_mux_2;
          devContextPtr->xpt.modes[1].xptMap    = MXL_HRCLS_XPT_MAP_2_TO_1; 
          devContextPtr->xpt.modes[1].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_MUX_2; 
          devContextPtr->xpt.modes[1].dfeChanMap = mxl_hrcls_dfeChanMap254_xpt_mux_2;
          devContextPtr->xpt.modes[1].demodScheme = MXL_HRCLS_XPT_254_SCHEME_MUX_2;

          devContextPtr->xpt.modes[2].mode = MXL_HRCLS_XPT_MODE_MUX_1; 
          devContextPtr->xpt.modes[2].demodsMap = mxl_hrcls_demodMap254_xpt_mux_1;
          devContextPtr->xpt.modes[2].demodsCnt = MXL_HRCLS_DEMODS_CNT_254;
          devContextPtr->xpt.modes[2].outputMap = mxl_hrcls_outputMap254_xpt_mux_1;
          devContextPtr->xpt.modes[2].xptMap    = MXL_HRCLS_XPT_MAP_4_TO_1; 
          devContextPtr->xpt.modes[2].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_MUX_1; 
          devContextPtr->xpt.modes[2].dfeChanMap = mxl_hrcls_dfeChanMap254_xpt_mux_1;
          devContextPtr->xpt.modes[2].demodScheme = MXL_HRCLS_XPT_254_SCHEME_MUX_1;

          devContextPtr->xpt.modes[3].mode = MXL_HRCLS_XPT_MODE_PARALLEL; 
          devContextPtr->xpt.modes[3].demodsMap = mxl_hrcls_demodMap254_xpt_par;
          devContextPtr->xpt.modes[3].demodsCnt = MXL_HRCLS_DEMODS_CNT_254;
          devContextPtr->xpt.modes[3].outputMap = mxl_hrcls_outputMap254_xpt_par;
          devContextPtr->xpt.modes[3].xptMap    = MXL_HRCLS_XPT_MAP_PARALLEL; 
          devContextPtr->xpt.modes[3].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_PAR; 
          devContextPtr->xpt.modes[3].dfeChanMap = mxl_hrcls_dfeChanMap254_xpt_par;
          devContextPtr->xpt.modes[3].demodScheme = MXL_HRCLS_XPT_254_SCHEME_PARALLEL;

          devContextPtr->xpt.modes[4].mode = MXL_HRCLS_XPT_MODE_CABLECARD; 
          devContextPtr->xpt.modes[4].demodsMap = mxl_hrcls_demodMap254_xpt_par;
          devContextPtr->xpt.modes[4].demodsCnt = MXL_HRCLS_DEMODS_CNT_254;
          devContextPtr->xpt.modes[4].outputMap = mxl_hrcls_outputMap254_xpt_par;
          devContextPtr->xpt.modes[4].xptMap    = MXL_HRCLS_XPT_MAP_CABLECARD; 
          devContextPtr->xpt.modes[4].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_PAR; 
          devContextPtr->xpt.modes[4].dfeChanMap = mxl_hrcls_dfeChanMap254_xpt_par;
          devContextPtr->xpt.modes[4].demodScheme = MXL_HRCLS_XPT_254_SCHEME_PARALLEL;

          // Set default XPT mode
          if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, MXL_HRCLS_XPT_MODE_NO_MUX_4) == MXL_FALSE)
          {
            status = MXL_NOT_SUPPORTED;
          }
        }
  #endif
#endif      
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
      if (devContextPtr->chipVersion >= 2)
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_254_V2;
        devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_254_V2;
        devContextPtr->ifOutMap = mxl_hrcls_ifoutMap254_v2; 
      }
      else
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_254;
        devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_254_V1;
        devContextPtr->ifOutMap = mxl_hrcls_ifoutMap254_v1; 
      }
#endif
      break;
#endif      

#ifdef MXL_HRCLS_255_ENABLE      
    case MXL_HRCLS_DEVICE_255:
        devContextPtr->dfeChanMap = mxl_hrcls_dfeChanMap255; 
        devContextPtr->dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_255; 
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
        devContextPtr->serDesDSLanesCnt = MXL_HRCLS_SERDES_DS_LANES_CNT_255;
#endif      
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
        devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_255;
        devContextPtr->demodsMap = mxl_hrcls_demodMap255; 
        devContextPtr->autoSpectrumInversionSupported = MXL_HRCLS_AUTO_SPECTRUM_INV_254_V2;
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_HRCLS_OOB_SUPPORT_255;
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_255;      
  #endif            
        devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_255; 
  #ifdef _MXL_HRCLS_XPT_ENABLED_
        if ((devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID) && (devContextPtr->chipVersion >= 2))
        {
          devContextPtr->xpt.supported = MXL_TRUE;
          devContextPtr->xpt.modesCnt = 1;
          devContextPtr->xpt.outputsCnt = MXL_HRCLS_XPT_OUTPUTS_255;
          devContextPtr->xpt.fourWireModeSupported = (MXL_BOOL_E)((devContextPtr->chipVersion >= 3)?1:0);
          devContextPtr->xpt.commonClockEnabled = MXL_FALSE;
          
          devContextPtr->xpt.currentMode = NULL;
          devContextPtr->xpt.modes[0].mode = MXL_HRCLS_XPT_MODE_NO_MUX_4;
          devContextPtr->xpt.modes[0].demodsMap = mxl_hrcls_demodMap255_xpt_nomux_4;
          devContextPtr->xpt.modes[0].demodsCnt = MXL_HRCLS_DEMODS_CNT_255;
          devContextPtr->xpt.modes[0].outputMap = mxl_hrcls_outputMap255_xpt_nomux_4; 
          devContextPtr->xpt.modes[0].xptMap    = MXL_HRCLS_XPT_MAP_1_TO_1; 
          devContextPtr->xpt.modes[0].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_255_XPT_NOMUX_4; 
          devContextPtr->xpt.modes[0].dfeChanMap = mxl_hrcls_dfeChanMap255_xpt_nomux_4;
          devContextPtr->xpt.modes[0].demodScheme = MXL_HRCLS_XPT_255_SCHEME_NO_MUX_4;

          // Set default XPT mode
          if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, MXL_HRCLS_XPT_MODE_NO_MUX_4) == MXL_FALSE)
          {
            status = MXL_NOT_SUPPORTED;
          }
        }
  #endif
#endif      
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
      if (devContextPtr->chipVersion >= 2)
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_255_V2;
        devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_255_V2;
        devContextPtr->ifOutMap = mxl_hrcls_ifoutMap255_v2; 
      }
      else
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_255;
        devContextPtr->ifOutCnt = 0;
        devContextPtr->ifOutMap = NULL; 
      }
#endif
      break;
#endif      

#ifdef MXL_HRCLS_256_ENABLE      
    case MXL_HRCLS_DEVICE_256:
        devContextPtr->dfeChanMap = mxl_hrcls_dfeChanMap256; 
        devContextPtr->dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_256; 
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
        devContextPtr->serDesDSLanesCnt = MXL_HRCLS_SERDES_DS_LANES_CNT_256;
#endif      
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
        devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_256;
        devContextPtr->demodsMap = mxl_hrcls_demodMap256; 
        devContextPtr->autoSpectrumInversionSupported = (devContextPtr->chipVersion >= 2)?MXL_HRCLS_AUTO_SPECTRUM_INV_256_V2:MXL_HRCLS_AUTO_SPECTRUM_INV_256;
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_HRCLS_OOB_SUPPORT_256;
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_256;      
  #endif            
        devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_256; 
  #ifdef _MXL_HRCLS_XPT_ENABLED_
        if ((devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID) && (devContextPtr->chipVersion >= 2))
        {
          devContextPtr->xpt.supported = MXL_TRUE;
          devContextPtr->xpt.modesCnt = 7;
          devContextPtr->xpt.outputsCnt = MXL_HRCLS_XPT_OUTPUTS_256;
          devContextPtr->xpt.fourWireModeSupported = (MXL_BOOL_E)((devContextPtr->chipVersion >= 3)?1:0);
          devContextPtr->xpt.commonClockEnabled = MXL_FALSE;

          devContextPtr->xpt.currentMode = NULL;
          devContextPtr->xpt.modes[0].mode = MXL_HRCLS_XPT_MODE_NO_MUX_4; 
          devContextPtr->xpt.modes[0].demodsMap = mxl_hrcls_demodMap256_xpt_nomux_4;
          devContextPtr->xpt.modes[0].demodsCnt = MXL_HRCLS_DEMODS_CNT_256_NOMUX;
          devContextPtr->xpt.modes[0].outputMap = mxl_hrcls_outputMap256_xpt_nomux_4; 
          devContextPtr->xpt.modes[0].xptMap    = MXL_HRCLS_XPT_MAP_1_TO_1; 
          devContextPtr->xpt.modes[0].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_NOMUX_4; 
          devContextPtr->xpt.modes[0].dfeChanMap = mxl_hrcls_dfeChanMap256_xpt_nomux_4;
          devContextPtr->xpt.modes[0].demodScheme = MXL_HRCLS_XPT_256_SCHEME_NO_MUX_4;

          devContextPtr->xpt.modes[1].mode = MXL_HRCLS_XPT_MODE_MUX_4; 
          devContextPtr->xpt.modes[1].demodsMap = mxl_hrcls_demodMap256_xpt_mux_4;
          devContextPtr->xpt.modes[1].demodsCnt = MXL_HRCLS_DEMODS_CNT_256;
          devContextPtr->xpt.modes[1].outputMap = mxl_hrcls_outputMap256_xpt_mux_4; 
          devContextPtr->xpt.modes[1].xptMap    = MXL_HRCLS_XPT_MAP_2_TO_1; 
          devContextPtr->xpt.modes[1].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_4; 
          devContextPtr->xpt.modes[1].dfeChanMap = mxl_hrcls_dfeChanMap256_xpt_mux_4;
          devContextPtr->xpt.modes[1].demodScheme = MXL_HRCLS_XPT_256_SCHEME_MUX_4;

          devContextPtr->xpt.modes[2].mode = MXL_HRCLS_XPT_MODE_MUX_3; 
          devContextPtr->xpt.modes[2].demodsMap = mxl_hrcls_demodMap256_xpt_mux_3;
          devContextPtr->xpt.modes[2].demodsCnt = MXL_HRCLS_DEMODS_CNT_256;
          devContextPtr->xpt.modes[2].outputMap = mxl_hrcls_outputMap256_xpt_mux_3; 
          devContextPtr->xpt.modes[2].xptMap    = MXL_HRCLS_XPT_MAP_2_TO_1; 
          devContextPtr->xpt.modes[2].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_3; 
          devContextPtr->xpt.modes[2].dfeChanMap = mxl_hrcls_dfeChanMap256_xpt_mux_3;
          devContextPtr->xpt.modes[2].demodScheme = MXL_HRCLS_XPT_256_SCHEME_MUX_3;

          devContextPtr->xpt.modes[3].mode = MXL_HRCLS_XPT_MODE_MUX_2; 
          devContextPtr->xpt.modes[3].demodsMap = mxl_hrcls_demodMap256_xpt_mux_2;
          devContextPtr->xpt.modes[3].demodsCnt = MXL_HRCLS_DEMODS_CNT_256;
          devContextPtr->xpt.modes[3].outputMap = mxl_hrcls_outputMap256_xpt_mux_2; 
          devContextPtr->xpt.modes[3].xptMap    = MXL_HRCLS_XPT_MAP_4_TO_1; 
          devContextPtr->xpt.modes[3].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_2; 
          devContextPtr->xpt.modes[3].dfeChanMap = mxl_hrcls_dfeChanMap256_xpt_mux_2;
          devContextPtr->xpt.modes[3].demodScheme = MXL_HRCLS_XPT_256_SCHEME_MUX_2;

          devContextPtr->xpt.modes[4].mode = MXL_HRCLS_XPT_MODE_MUX_2_B; 
          devContextPtr->xpt.modes[4].demodsMap = mxl_hrcls_demodMap256_xpt_mux_2b;
          devContextPtr->xpt.modes[4].demodsCnt = MXL_HRCLS_DEMODS_CNT_256;
          devContextPtr->xpt.modes[4].outputMap = mxl_hrcls_outputMap256_xpt_mux_2b; 
          devContextPtr->xpt.modes[4].xptMap    = MXL_HRCLS_XPT_MAP_4_TO_1; 
          devContextPtr->xpt.modes[4].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_2b; 
          devContextPtr->xpt.modes[4].dfeChanMap = mxl_hrcls_dfeChanMap256_xpt_mux_2b;
          devContextPtr->xpt.modes[4].demodScheme = MXL_HRCLS_XPT_256_SCHEME_MUX_2_B;

          devContextPtr->xpt.modes[5].mode = MXL_HRCLS_XPT_MODE_PARALLEL; 
          devContextPtr->xpt.modes[5].demodsMap = mxl_hrcls_demodMap256_xpt_par; 
          devContextPtr->xpt.modes[5].demodsCnt = MXL_HRCLS_DEMODS_CNT_256;
          devContextPtr->xpt.modes[5].outputMap = mxl_hrcls_outputMap256_xpt_par; 
          devContextPtr->xpt.modes[5].xptMap    = MXL_HRCLS_XPT_MAP_PARALLEL; 
          devContextPtr->xpt.modes[5].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_PAR; 
          devContextPtr->xpt.modes[5].dfeChanMap = mxl_hrcls_dfeChanMap256_xpt_par;
          devContextPtr->xpt.modes[5].demodScheme = MXL_HRCLS_XPT_256_SCHEME_PARALLEL;

          devContextPtr->xpt.modes[6].mode = MXL_HRCLS_XPT_MODE_CABLECARD; 
          devContextPtr->xpt.modes[6].demodsMap = mxl_hrcls_demodMap256_xpt_par; 
          devContextPtr->xpt.modes[6].demodsCnt = MXL_HRCLS_DEMODS_CNT_256;
          devContextPtr->xpt.modes[6].outputMap = mxl_hrcls_outputMap256_xpt_par; 
          devContextPtr->xpt.modes[6].xptMap    = MXL_HRCLS_XPT_MAP_CABLECARD; 
          devContextPtr->xpt.modes[6].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_PAR; 
          devContextPtr->xpt.modes[6].dfeChanMap = mxl_hrcls_dfeChanMap256_xpt_par;
          devContextPtr->xpt.modes[6].demodScheme = MXL_HRCLS_XPT_256_SCHEME_PARALLEL;

          // Set default XPT mode
          if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, MXL_HRCLS_XPT_MODE_NO_MUX_4) == MXL_FALSE)
          {
            status = MXL_NOT_SUPPORTED;
          }
        }
  #endif      
#endif      
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
      if (devContextPtr->chipVersion >= 2)
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_256_V2;
        devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_256_V2;
        devContextPtr->ifOutMap = mxl_hrcls_ifoutMap256_v2; 
      }
      else
      {
        devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_256;
        devContextPtr->ifOutCnt = 0;
        devContextPtr->ifOutMap = NULL; 
      }
#endif
      break;
#endif      
            
#ifdef MXL_HRCLS_269_ENABLE      
    case MXL_HRCLS_DEVICE_269:
    {
      UINT8 i;
      devContextPtr->dfeChanMap = (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)?mxl_hrcls_dfeChanMap269_5400:mxl_hrcls_dfeChanMap269_5184; 
      devContextPtr->dfeChanCount = (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)?MXL_HRCLS_MAX_DFE_CHANNELS_269_5400:MXL_HRCLS_MAX_DFE_CHANNELS_269_5184; 
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
      devContextPtr->serDesDSLanesCnt = MXL_HRCLS_SERDES_DS_LANES_CNT_269;
      for (i = 0; i < devContextPtr->serDesDSLanesCnt; i++)
      {
        if (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)
        {
          devContextPtr->serDesSlotsPerLane[i] = (devContextPtr->serDesMode[i] == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)?MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_2700:MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_5400;
        }
        else devContextPtr->serDesSlotsPerLane[i] = MXL_HRCLS_SERDES_DS_SLOTS_PER_LANE_5184; 
      }
#endif      
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
      devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_269;
      devContextPtr->demodsMap = mxl_hrcls_demodMap269; 
      devContextPtr->autoSpectrumInversionSupported = MXL_HRCLS_AUTO_SPECTRUM_INV_269;
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_HRCLS_OOB_SUPPORT_269;
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_269;      
  #endif            
      devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_269; 
  #ifdef _MXL_HRCLS_XPT_ENABLED_
      devContextPtr->xpt.supported = MXL_FALSE;
  #endif    
#endif      
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
      devContextPtr->ifOutSupported = MXL_HRCLS_IFOUT_SUPPORT_269;
      devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_269;
      devContextPtr->ifOutMap = NULL;
#endif
      break;
    }
#endif      

#ifdef MXL_HRCLS_212_ENABLE
    case MXL_HRCLS_DEVICE_212:
    {
      devContextPtr->dfeChanMap = mxl_hrcls_dfeChanMap212_xpt_nomux_4;
      devContextPtr->dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_212_XPT_NOMUX_4; 
      devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_212; 
      devContextPtr->demodsMap = mxl_hrcls_demodMap212_xpt_nomux_4;
      devContextPtr->autoSpectrumInversionSupported = MXL_TRUE; 
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_FALSE; 
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_212;
  #endif      
      devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_212;
  #ifdef _MXL_HRCLS_XPT_ENABLED_
      devContextPtr->xpt.supported = MXL_TRUE;
      devContextPtr->xpt.modesCnt = 1;
      devContextPtr->xpt.outputsCnt = MXL_HRCLS_XPT_OUTPUTS_212; 
      devContextPtr->xpt.fourWireModeSupported = (MXL_BOOL_E)((devContextPtr->chipVersion >= 3)?1:0);
      devContextPtr->xpt.commonClockEnabled = MXL_FALSE;

      devContextPtr->xpt.currentMode = NULL;
      devContextPtr->xpt.modes[0].mode = MXL_HRCLS_XPT_MODE_NO_MUX_4; 
      devContextPtr->xpt.modes[0].demodsMap = mxl_hrcls_demodMap212_xpt_nomux_4;
      devContextPtr->xpt.modes[0].demodsCnt = MXL_HRCLS_DEMODS_CNT_212;
      devContextPtr->xpt.modes[0].outputMap = mxl_hrcls_outputMap212_xpt_nomux_4;
      devContextPtr->xpt.modes[0].xptMap = MXL_HRCLS_XPT_MAP_1_TO_1; 
      devContextPtr->xpt.modes[0].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_212_XPT_NOMUX_4;
      devContextPtr->xpt.modes[0].dfeChanMap = mxl_hrcls_dfeChanMap212_xpt_nomux_4;
      devContextPtr->xpt.modes[0].demodScheme = MXL_HRCLS_XPT_212_SCHEME_NO_MUX_4;

      // Set default XPT mode
      if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, devContextPtr->xpt.modes[0].mode) == MXL_FALSE)
      {
        status = MXL_NOT_SUPPORTED;
      }
  #endif    

  #ifdef _MXL_HRCLS_IFOUT_ENABLED_
      devContextPtr->ifOutSupported = MXL_TRUE; 
      devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_212_V2; 
      devContextPtr->ifOutMap = mxl_hrcls_ifoutMap212_v2; 
  #endif
      break;
    }
#endif
#ifdef MXL_HRCLS_213_ENABLE
    case MXL_HRCLS_DEVICE_213:
    {
      devContextPtr->dfeChanMap = mxl_hrcls_dfeChanMap213_xpt_nomux_4;
      devContextPtr->dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_213_XPT_NOMUX_4; 
      devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_213; 
      devContextPtr->demodsMap = mxl_hrcls_demodMap213_xpt_nomux_4;
      devContextPtr->autoSpectrumInversionSupported = MXL_TRUE; 
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_FALSE; 
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_213;
  #endif      
      devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_213;
  #ifdef _MXL_HRCLS_XPT_ENABLED_
      devContextPtr->xpt.supported = MXL_TRUE;
      devContextPtr->xpt.modesCnt = 1;
      devContextPtr->xpt.outputsCnt = MXL_HRCLS_XPT_OUTPUTS_213; 
      devContextPtr->xpt.fourWireModeSupported = (MXL_BOOL_E)((devContextPtr->chipVersion >= 3)?1:0);
      devContextPtr->xpt.commonClockEnabled = MXL_FALSE;

      devContextPtr->xpt.currentMode = NULL;
      devContextPtr->xpt.modes[0].mode = MXL_HRCLS_XPT_MODE_NO_MUX_4; 
      devContextPtr->xpt.modes[0].demodsMap = mxl_hrcls_demodMap213_xpt_nomux_4;
      devContextPtr->xpt.modes[0].demodsCnt = MXL_HRCLS_DEMODS_CNT_213;
      devContextPtr->xpt.modes[0].outputMap = mxl_hrcls_outputMap213_xpt_nomux_4;
      devContextPtr->xpt.modes[0].xptMap = MXL_HRCLS_XPT_MAP_1_TO_1; 
      devContextPtr->xpt.modes[0].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_213_XPT_NOMUX_4;
      devContextPtr->xpt.modes[0].dfeChanMap = mxl_hrcls_dfeChanMap213_xpt_nomux_4;
      devContextPtr->xpt.modes[0].demodScheme = MXL_HRCLS_XPT_213_SCHEME_NO_MUX_4;

      // Set default XPT mode
      if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, devContextPtr->xpt.modes[0].mode) == MXL_FALSE)
      {
        status = MXL_NOT_SUPPORTED;
      }
  #endif    

  #ifdef _MXL_HRCLS_IFOUT_ENABLED_
      devContextPtr->ifOutSupported = MXL_TRUE; 
      devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_213_V2; 
      devContextPtr->ifOutMap = mxl_hrcls_ifoutMap213_v2; 
  #endif
      break;
    }
#endif
#ifdef MXL_HRCLS_214_ENABLE
    case MXL_HRCLS_DEVICE_214:
    {
      devContextPtr->dfeChanMap = mxl_hrcls_dfeChanMap214_xpt_nomux_4;
      devContextPtr->dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_214_XPT_NOMUX_4; 
      devContextPtr->demodsCnt = MXL_HRCLS_DEMODS_CNT_214; 
      devContextPtr->demodsMap = mxl_hrcls_demodMap214_xpt_nomux_4;
      devContextPtr->autoSpectrumInversionSupported = MXL_TRUE; 
  #ifdef _MXL_HRCLS_OOB_ENABLED_
        devContextPtr->oobSupported = MXL_FALSE; 
        devContextPtr->oobDemod = MXL_HRCLS_OOB_DEMOD_214;
  #endif      
      devContextPtr->wakeOnWanDemod = MXL_HRLCS_WAKE_ON_WAN_DEMOD_214;
  #ifdef _MXL_HRCLS_XPT_ENABLED_
      devContextPtr->xpt.supported = MXL_TRUE;
      devContextPtr->xpt.modesCnt = 1;
      devContextPtr->xpt.outputsCnt = MXL_HRCLS_XPT_OUTPUTS_214; 
      devContextPtr->xpt.fourWireModeSupported = (MXL_BOOL_E)((devContextPtr->chipVersion >= 3)?1:0);
      devContextPtr->xpt.commonClockEnabled = MXL_FALSE;

      devContextPtr->xpt.currentMode = NULL;
      devContextPtr->xpt.modes[0].mode = MXL_HRCLS_XPT_MODE_NO_MUX_4; 
      devContextPtr->xpt.modes[0].demodsMap = mxl_hrcls_demodMap214_xpt_nomux_4;
      devContextPtr->xpt.modes[0].demodsCnt = MXL_HRCLS_DEMODS_CNT_214;
      devContextPtr->xpt.modes[0].outputMap = mxl_hrcls_outputMap214_xpt_nomux_4;
      devContextPtr->xpt.modes[0].xptMap = MXL_HRCLS_XPT_MAP_1_TO_1; 
      devContextPtr->xpt.modes[0].dfeChanCount = MXL_HRCLS_MAX_DFE_CHANNELS_214_XPT_NOMUX_4;
      devContextPtr->xpt.modes[0].dfeChanMap = mxl_hrcls_dfeChanMap214_xpt_nomux_4;
      devContextPtr->xpt.modes[0].demodScheme = MXL_HRCLS_XPT_214_SCHEME_NO_MUX_4;

      // Set default XPT mode
      if (MxLWare_HRCLS_Ctrl_ValidateXPTMode(devContextPtr, devContextPtr->xpt.modes[0].mode) == MXL_FALSE)
      {
        status = MXL_NOT_SUPPORTED;
      }
  #endif    

  #ifdef _MXL_HRCLS_IFOUT_ENABLED_
      devContextPtr->ifOutSupported = MXL_TRUE; 
      devContextPtr->ifOutCnt = MXL_HRCLS_IFOUT_CNT_214_V2; 
      devContextPtr->ifOutMap = mxl_hrcls_ifoutMap214_v2; 
  #endif
      break;
    }
#endif
    
    default:
      status = MXL_INVALID_PARAMETER;
      break; 
  }

#ifdef _MXL_HRCLS_OOB_ENABLED_
  devContextPtr->oobType = OOB_SCTE_55_1_TYPE;      
  if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
  {
    devContextPtr->oobFec_55_2_Supported = MXL_FALSE;      
    devContextPtr->oobTsOutputSupported = MXL_FALSE;      
  }
  else if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (devContextPtr->chipVersion >= 3))
  {
    devContextPtr->oobFec_55_2_Supported = MXL_TRUE;      
    devContextPtr->oobTsOutputSupported = MXL_FALSE;      
  }
  else
  {
    devContextPtr->oobFec_55_2_Supported = MXL_TRUE;      
    devContextPtr->oobTsOutputSupported = MXL_TRUE;      
  }
#endif

#ifndef _MSC_VER   
  MXLDBG1(
  if (status == MXL_SUCCESS)
  {
    MxL_HRCLS_DEBUG("---------- Device parameters information -------------\n");
    MxL_HRCLS_DEBUG("| Device type .......... %-7s                     |\n", 
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_265)?"265":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_267)?"267":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_258)?"258":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_252)?"252":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_254)?"254":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_255)?"255":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_256)?"256":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_269)?"269":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_212)?"212":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_213)?"213":
            (devContextPtr->deviceType == MXL_HRCLS_DEVICE_214)?"214":"unknown");
    MxL_HRCLS_DEBUG("| DFE channels count ... %2d                          |\n", devContextPtr->dfeChanCount);
    MxL_HRCLS_DEBUG("| Chip ID ... %8s                               |\n", (devContextPtr->chipId == MXL_HRCLS_HERCULES_CHIP_ID)?"HERCULES":"MINOS");
    MxL_HRCLS_DEBUG("| Chip version ... %d                                 |\n", devContextPtr->chipVersion);
#ifdef _MXL_HRCLS_SERDES_ENABLED_        
    if (devContextPtr->serDesDSLanesCnt)
    {
      UINT8 i;
      MxL_HRCLS_DEBUG("| Serdes mode .......... %1s                           |\n", (devContextPtr->serDesLineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ)?"A":"B");
      MxL_HRCLS_DEBUG("| Serdes lanes count ... %1d                           |\n", devContextPtr->serDesDSLanesCnt);
      for (i = 0; i < devContextPtr->serDesDSLanesCnt; i++)
      {
        UINT8 laneId = (UINT8) i;
        MXL_HRCLS_SERDES_SWAP(laneId);
        MxL_HRCLS_DEBUG("| Serdes lane%d state ... %-4s                        |\n", laneId, (devContextPtr->serDesMode[laneId] == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)?"HALF":(devContextPtr->serDesMode[laneId] == MXL_HRCLS_SERDES_MODE_ENABLED_FULLRATE)?"FULL":"OFF");
        MxL_HRCLS_DEBUG("| Serdes lane%d slots ... %2d                          |\n", laneId, devContextPtr->serDesSlotsPerLane[laneId]);
      }
    }
#endif
#ifdef _MXL_HRCLS_DEMOD_ENABLED_        
    MxL_HRCLS_DEBUG("| Demods count ......... %1d                           |\n", devContextPtr->demodsCnt);
    MxL_HRCLS_DEBUG("| Auto spectrum inv .... %8s                    |\n", (devContextPtr->autoSpectrumInversionSupported == MXL_TRUE)?"enabled":"disabled");    
  #ifdef _MXL_HRCLS_OOB_ENABLED_
    MxL_HRCLS_DEBUG("| OOB .................. %8s (%1d)                |\n", (devContextPtr->oobSupported == MXL_TRUE)?"enabled":"disabled", devContextPtr->oobDemod);    
  #endif  
  #ifdef _MXL_HRCLS_XPT_ENABLED_
    MxL_HRCLS_DEBUG("| XPT .................. %8s                    |\n", (devContextPtr->xpt.supported == MXL_TRUE)?"enabled":"disabled");    
  #endif    
#endif
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
    MxL_HRCLS_DEBUG("| IFOUT supported ...... %s (%1d)                     |\n", devContextPtr->ifOutSupported ? "Yes" : "No ", devContextPtr->ifOutCnt);
#endif
    MxL_HRCLS_DEBUG("------------------------------------------------------\n");
  }
  )
#endif  
  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/21/2011
--|
--| DESCRIPTION   : This function converts logical DFE channel number to physical
--|
--| INPUTS        : Logical DFE channel number 
--|
--| RETURN VALUE  : Physical DFE channel number 
--|
--|-------------------------------------------------------------------------------------*/
MXL_HRCLS_CHAN_ID_E MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_CHAN_ID_E logicalChanId)
{
  MXL_HRCLS_CHAN_ID_E physicalChanId;
 
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("logicalChannelId=%d\n", logicalChanId););
  if (logicalChanId == MXL_HRCLS_CHAN_NB)
  {
    physicalChanId = MXL_HRCLS_CHAN_NB;
  }
  else
  {
    physicalChanId = devContextPtr->dfeChanMap[(UINT8) logicalChanId].phyChan;
  }
  MXLEXITSTR(physicalChanId);

  return physicalChanId;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetOOBPhysicalChannelId
--| 
--| AUTHOR        : Sudhamshu Ramachandra
--|
--| DATE CREATED  : 03/20/2013
--|
--| DESCRIPTION   : This function obtains channel id for oob. Returns MXL_HRCLS_CHAN_NB if not supported in FB Tuner.
--|
--| INPUTS        : 
--|
--| RETURN VALUE  : Physical DFE channel number 
--|
--|-------------------------------------------------------------------------------------*/

MXL_HRCLS_CHAN_ID_E MxL_HRCLS_Ctrl_GetOOBPhysicalChannelId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  MXL_HRCLS_CHAN_ID_E physicalChanId = MXL_HRCLS_CHAN_NB;
  UINT8 logicalChanId = 0;

  MXLENTERSTR;
  while (logicalChanId < devContextPtr->dfeChanCount)
  {
    if (0 != (MXL_HRCLS_CHAN_IF_OOB & devContextPtr->dfeChanMap[(UINT8) logicalChanId].ifMask))
    { 
      physicalChanId = devContextPtr->dfeChanMap[(UINT8) logicalChanId].phyChan;
    }
    logicalChanId++;
  }
  MXLEXITSTR(physicalChanId);
  return physicalChanId;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetDeveiceContext
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/1/2011
--|
--| DESCRIPTION   : Retrieve MxL_HRCLS device context
--|
--| INPUTS        : devId 
--|
--| RETURN VALUE  : device context or NULL 
--|
--|-------------------------------------------------------------------------------------*/

MXL_HRCLS_DEV_CONTEXT_T * MxL_HRCLS_Ctrl_GetDeviceContext(UINT8 devId)
{
#ifdef _MXL_DIAG_ENABLED_
  devId &= 0x03;  // Mariusz: Temporary, just fo GUI!!!!
#endif  
  return (devId < MXL_HRCLS_MAX_NUM_DEVICES)?(&MxL_HRCLS_Dev[devId]):NULL;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_WaitForFieldValue 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/1/2012
--|
--| DESCRIPTION   : Wait until field has required value (or opposite) 
--|
--| INPUTS        : devId, field data, expected value, polarization, timeoutMs
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE 
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_WaitForFieldValue(UINT8 devId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, UINT16 expectedValue, MXL_BOOL_E waitForEqual, UINT16 timeoutMs, UINT16 * returnedValue)
{
  UINT16 value = 0;
  MXL_STATUS_E status = MXL_FAILURE;
  UINT64 timeNow, timeout;

  MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
  timeout = timeNow + (timeoutMs * 1000);

  do
  {
    status = MxL_HRCLS_Ctrl_ReadRegisterField(devId, regAddr, lsbPos, fieldWidth, &value);
    if ((status == MXL_SUCCESS) && 
         (
          ((value != expectedValue) && (waitForEqual == MXL_TRUE)) ||
          ((value == expectedValue) && (waitForEqual == MXL_FALSE))))
    {
      MxLWare_HRCLS_OEM_DelayUsec(1000); // 1ms
    }
    MXLDBG1(MxL_HRCLS_DEBUG("value=0x%04x (expected%s0x%04x)\n", value, (waitForEqual==MXL_TRUE)?"==":"!=", expectedValue););
    MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
  } while ((status == MXL_SUCCESS) && (((value != expectedValue) && (waitForEqual == MXL_TRUE)) || ((value == expectedValue) && (waitForEqual == MXL_FALSE))) && (timeNow <= timeout));

  MXLDBG1(MxL_HRCLS_DEBUG("delay: %dms\n", timeoutMs - (UINT16) (((SINT32)(timeout - timeNow)) / 1000)););
  
  if (status == MXL_SUCCESS)
  {
    status = MXL_FAILURE;
    if ((waitForEqual == MXL_TRUE) && (value == expectedValue)) status = MXL_SUCCESS;
    if ((waitForEqual == MXL_FALSE) && (value != expectedValue)) status = MXL_SUCCESS;
    if (returnedValue) *returnedValue = value;
  } else status = MXL_FAILURE;

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_UpdateRegisterField
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Reads and updates field 
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_UpdateRegisterField(UINT8 devId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, UINT16 newValue)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  UINT16 newregData;

  status = MxLWare_HRCLS_OEM_ReadRegister(devId, regAddr, &regData);
  if (status == MXL_SUCCESS)
  {
    UINT16 mask;

    mask = (UINT16) MXL_MASK(lsbPos, fieldWidth);
    newregData = (regData & ~mask) | ((newValue << lsbPos) & mask);

#ifdef MXL_HRCLS_DISCARD_UPDATE_IF_NO_CHANGE    
    if (newregData != regData)
#endif      
    {
#ifdef _MNEMONIC_TRACE_ENABLED_
      Diag_ctrlFlag |= MXL_HRCLS_DIAG_CTRL_FLAG_MNEMONIC_TRACE_PRINTED;
#endif // _MNEMONIC_TRACE_ENABLED_

      status = MxLWare_HRCLS_OEM_WriteRegister(devId, regAddr, newregData);

#ifdef _MNEMONIC_TRACE_ENABLED_
      if (0 != (MXL_HRCLS_DIAG_FLAG_MNEMONIC_TRACE_ENABLED & Diag_flag))
      {
        Ctrl_PrintRegFieldsInMask(regAddr, newregData, mask, 0xFFFFFFFF/*prevVal*/);
      }
      Diag_ctrlFlag &= ~MXL_HRCLS_DIAG_CTRL_FLAG_MNEMONIC_TRACE_PRINTED;
#endif // _MNEMONIC_TRACE_ENABLED_
    }
#if 0    
    MxL_HRCLS_DEBUG("[HRCLS][%d] UpdateRegisterField reg_addr=%x, value=%x (newValue=%x), mask=%x\n", 
                  devId, regAddr, newregData, newValue, mask);
#endif    

  } else status = MXL_FAILURE;

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_CtrlReadRegisterField
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 10/17/2011
--|                  
--| DESCRIPTION   : Read register field
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadRegisterField(UINT8 devId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, UINT16 * valuePtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT16 regData = 0;
  
  status = MxLWare_HRCLS_OEM_ReadRegister(devId, regAddr, &regData);
  if (status == MXL_SUCCESS)
  {
    UINT16 mask;

    mask = (UINT16) MXL_MASK(lsbPos, fieldWidth);
    regData &= mask;
    regData >>= lsbPos;

    if (valuePtr) *valuePtr = regData;
  } else status = MXL_FAILURE;
  return (MXL_STATUS_E) status;
}  

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetBigEndian32
--| 
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24
--|
--| DESCRIPTION   : Convert INT8 buf[0..2] into 32-bit big endian
--|
--| RETURN VALUE  : 32-bit unsigned value
--|-------------------------------------------------------------------------------------*/
#ifdef _DOWNLOAD_DEBUG_ENABLED_
static UINT32 MxL_HRCLS_Ctrl_GetBigEndian32(const UINT8 buf[])
{
  return (((UINT32)buf[0])<<24) | (((UINT32)buf[1])<<16) | (((UINT32)buf[2])<<8) | buf[3];
}
#endif

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CalCheckSum
--|
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24
--|
--| DESCRIPTION   : This function calculate CheckSum of input data array.
--|
--| RETURN VALUE  : CheckSum, check sum of input data array
--|
--|-------------------------------------------------------------------------------------*/

static UINT8 MxL_HRCLS_Ctrl_CalCheckSum(UINT8 inDataLen, UINT8 *inDataPtr)
{
  UINT8 sum = 0;
  while (inDataLen--)
  {
    sum += *inDataPtr++;
  }
  return sum;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CheckIntegration
--|
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24
--|
--| DESCRIPTION   : This function check if received data is integrate or not by 
--|                 compare CheckSum.
--|
--| RETURN VALUE  : MXL_SUCCESS: data is complete, MXL_FAILURE: not complete
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_CheckIntegration(UINT8 readCheckSum, UINT8 readDataLen, UINT8 *readDataPtr)
{
  UINT16 sum = 0;
  UINT8 len;

  if (readDataLen == 0)
    return(MXL_SUCCESS);  // Do not need to check integrate 
    
  for (len = 0; len < readDataLen; len ++)
    sum += *readDataPtr++;
 
  if ((sum & 0xff) == readCheckSum) 
    return(MXL_SUCCESS);  // Data is complete 
  else 
    return(MXL_FAILURE); // Data is not complete
}

MXL_STATUS_E MxL_HRCLS_Ctrl_WriteClockRegister(UINT8 devId, UINT32 newValue)
{
  UINT8 status;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("newValue=0x%08x\n", newValue););
  status = (UINT8) MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(CLK_ENA_LO), (UINT16) (newValue & 0xffff));
  status |= (UINT8) MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(CLK_ENA_HI), (UINT16) ((newValue >> 16) & 0xff));
  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

MXL_STATUS_E MxL_HRCLS_Ctrl_ReadClockRegister(UINT8 devId, UINT32 * readValue)
{
  UINT8 status = MXL_FAILURE;
  UINT16 regHi, regLo;

  MXLENTERSTR;
  if (readValue)
  {
    status = (UINT8) MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(CLK_ENA_LO), &regLo);
    status |= (UINT8) MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(CLK_ENA_HI), &regHi);

    *readValue = (UINT32) ((UINT32) regLo) | (((UINT32) regHi) << 16);
    MXLDBG2(MxL_HRCLS_DEBUG("clock register = 0x%08x\n", *readValue););
  }
  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_DownloadFirmwareInMbin
--| 
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24 
--|
--| DESCRIPTION   : Download MBIN format firmware file into device 
--|                 MBIN file format is like below:
--|                 MBIN Header (16 bytes)
--|                 Seg#0 Header (8 bytes)
--|                 Seg#0 Data (Variable length in bytes)
--|                 ...
--|                 Seg#N Header (8 bytes)
--|                 Seg#N Data (Variable length in bytes)
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_DownloadFirmwareInMbin(MXL_HRCLS_DEV_CONTEXT_T* devContextPtr, const MBIN_FILE_T* mbinPtr, UINT8 enableRun, MXL_CALLBACK_FN_T fwCallbackFn)
{
  MBIN_SEGMENT_T *segmentPtr ;
  UINT32 index;
  UINT32 segLength;
  MXL_STATUS_E status = MXL_SUCCESS;
  //UINT8 devId = devContextPtr->devId;

  MXLDBG2(MxL_HRCLS_DEBUG("Ctrl_DownloadFirmwareInMbin: Header ID = %d, Segments number = %d \n",mbinPtr->header.id, mbinPtr->header.numSegments););

  segmentPtr = (MBIN_SEGMENT_T*)(&mbinPtr->data[0]);

  /* Check for proper file format */
  if (mbinPtr->header.id == MBIN_FILE_HEADER_ID )
  {
    /* Process each segment */
    for (index = 0; ((index < mbinPtr->header.numSegments) && (status == MXL_SUCCESS)); index++)
    {
      /* Validate segment header ID */
      if (segmentPtr->header.id == MBIN_SEGMENT_HEADER_ID)
      {
        /* Get segment data length and start address */
        segLength = (((UINT32) segmentPtr->header.len24[0]) << 16) 
                    | (((UINT32)  segmentPtr->header.len24[1]) << 8) 
                    | ((UINT32) segmentPtr->header.len24[2]);
        
        /* Send segment */
        status = MxL_HRCLS_Ctrl_DownloadFirmwareSegment(devContextPtr, index, segmentPtr->header.address, segmentPtr->data, segLength, fwCallbackFn);

        if (status != MXL_SUCCESS)
        {
          MXLDBG2(MxL_HRCLS_ERROR("Ctrl_DownloadFirmwareInMbin: Invalid segment Location. ID(%d): Start Address:%x segLength:%d\n",segmentPtr->header.id, 
                  ((UINT32) segmentPtr->header.address[3] << 24) | ((UINT32) segmentPtr->header.address[2] << 16) | ((UINT32) segmentPtr->header.address[1] << 8) | ((UINT32) segmentPtr->header.address[0]),
                  segLength););
          break;
        }
        if (status == MXL_SUCCESS)
        {
          /* Next segment */
          segmentPtr = (MBIN_SEGMENT_T*)&(segmentPtr->data[((segLength + 3)/4)*4]);
        }          
      }
      else
      {
        MXLERR(MxL_HRCLS_ERROR("Ctrl_DownloadFirmwareInMbin: Invalid segment header ID (%c)\n",segmentPtr->header.id););
        status = MXL_FAILURE; // If the segment format is wrong, then exit with MXL_FAILURE 
      }
    }   

    if (((MXL_BOOL_E)enableRun == MXL_ENABLE) && (status == MXL_SUCCESS))
    { 
      // Run firmware after download
      status = MxL_HRCLS_Ctrl_SendDownloadCommand(devContextPtr, HOST_CMD_CFG_DOWNLOAD_DONE, &(mbinPtr->header.entryAddress[0]), 4, 0);
    }
  }
  else
  {
    MXLERR(MxL_HRCLS_ERROR("Ctrl_DownloadFirmwareInMbin: Invalid file header ID (%c)\n", mbinPtr->header.id););
    status = MXL_FAILURE; // If the file is not MBIN format, then exit with MXL_FAILURE 
  }
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_DownloadFirmwareSegment
--| 
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24 
--|
--| DESCRIPTION   : Download segment of MBIN format firmware into MxL_HRCLS device  
--|
--| RETURN VALUE  : none
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_DownloadFirmwareSegment(MXL_HRCLS_DEV_CONTEXT_T* devContextPtr, UINT8 segmentIndex, UINT8 * addressPtr, const UINT8* dataPtr, UINT32 dataLen, MXL_CALLBACK_FN_T fwCallbackFn)
{
  UINT32 nSend;       // Bytes that send to device
  UINT16 seqNum = 0;  // Record the firmware block sequence number that have been downloaded 
  MXL_HRCLS_FIRMWARE_DOWNLOAD_STATS_T downloadStats;
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;

  downloadStats.segmentIndex = segmentIndex;
  downloadStats.segmentSize = dataLen;
  downloadStats.segmentBytesSent = 0;

  // Send  command firstly 
  status = MxL_HRCLS_Ctrl_SendDownloadCommand(devContextPtr, HOST_CMD_CFG_SET_ADDR, addressPtr, 4, 0);
  if (status == MXL_SUCCESS)
  {
    while ((status == MXL_SUCCESS) && (dataLen>0))
    {
      // Send segment data, the block length is nSend
      if (dataLen > (MXL_HRCLS_OEM_MAX_BLOCK_WRITE_LENGTH - COMMAND_HEADER_LENGTH))
      {
        nSend = MXL_HRCLS_OEM_MAX_BLOCK_WRITE_LENGTH - COMMAND_HEADER_LENGTH;
      }
      else
      {
        nSend = dataLen;
      }

      // MxL_HRCLS_DEBUG("%s: HOST_CMD_DOWNLOAD_SEGMENT_CFG, Max Limitation = %d, send length = %d bytes\n",MxL_HRCLS_I2cMaxWriteLen, nSend);
      
      status = MxL_HRCLS_Ctrl_SendDownloadCommand(devContextPtr, HOST_CMD_CFG_DOWNLOAD_SEGMENT, (void*)dataPtr, nSend, seqNum);
      if (status == MXL_SUCCESS)
      {
        // Sending data pointer is moved to next segment 
        seqNum ++;
        dataPtr += nSend;
        dataLen -= nSend;
        downloadStats.segmentBytesSent += nSend;

        if (fwCallbackFn) status = (MXL_STATUS_E) (fwCallbackFn)(devId, MXL_HRCLS_CB_FW_DOWNLOAD_STATS, (void *) &downloadStats);
      }
      else
      {
        MXLERR(MxL_HRCLS_ERROR("%s: send HOST_CMD_DOWNLOAD_SEGMENT_CFG fail.\n", __FUNCTION__););
        status = MXL_FAILURE;
      }
    }
  }
  else
  {
    MXLERR(MxL_HRCLS_ERROR("%s: HOST_CMD_SET_ADDR_CFG failed!\n", __FUNCTION__););
    status = MXL_FAILURE;
  }

  return status;
}

#ifdef _DOWNLOAD_DEBUG_ENABLED_
/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CheckCommandResponse
--| 
--| AUTHOR        : Jun Huang
--|
--| DATE CREATED  : 04/07/2011
--|
--| DESCRIPTION   : Check respose of download commands
--|
--| RETURN VALUE  : none
--|-------------------------------------------------------------------------------------*/

void MxL_HRCLS_Ctrl_CheckCommandResponse(UINT8 devId, UINT16 cmd)
{
  UINT8 readBuffer[COMMAND_HEADER_LENGTH];
  if (0 == MxLWare_HRCLS_OEM_ReadBlock(devId, cmd, COMMAND_HEADER_LENGTH, readBuffer))
  {
    MXLDBG2(MxL_HRCLS_DEBUG("CMDRESPONSE: SYNC=0x%02X, errorCode=0x%02X, checkSum=0x%02X, dataLen=0x%02X. \n",
      readBuffer[0],readBuffer[1],readBuffer[2],readBuffer[3]););
  }
MXLERR(  
  else
  {
    MxL_HRCLS_DEBUG("Block read failed\n");
  }
  )
}
#endif // _DOWNLOAD_DEBUG_ENABLED_

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SendDownloadCommand
--| 
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24 
--|
--| DESCRIPTION   : Send download command into MxL_HRCLS device  
--|
--| RETURN VALUE  : none
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_SendDownloadCommand(MXL_HRCLS_DEV_CONTEXT_T* devContextPtr, MXL_CMD_ID_E commandId, const void* dataPtr, UINT32 dataLen, UINT16 downloadBlockCnt)
{
  HOST_COMMAND_T downloadFwSeg;
  MXL_STATUS_E status = MXL_SUCCESS;
  FW_RESPONSE_T  response;
  UINT32 i;
  UINT8 devId = devContextPtr->devId;
#ifdef _DOWNLOAD_DEBUG_ENABLED_  
  static UINT32 curAddress = 0; // curAddress is used for debugging purpose only
#endif  

  // Check if the sending data length exceeds the limitation 
  if (dataLen <= (MXL_HRCLS_OEM_MAX_BLOCK_WRITE_LENGTH - COMMAND_HEADER_LENGTH))
  {
#ifdef _DOWNLOAD_DEBUG_ENABLED_  
    switch (commandId)
    {
      case HOST_CMD_CFG_SET_ADDR:
        curAddress = MxL_HRCLS_Ctrl_GetBigEndian32((UINT8*)dataPtr);
        MxL_HRCLS_DEBUG("I2CTRC_FW_DOWNLOAD // COMMAND_SET_ADDR_CFG(0x%08x)\n", curAddress);
        break;

      case HOST_CMD_CFG_DOWNLOAD_SEGMENT:
        MxL_HRCLS_DEBUG("I2CTRC_FW_DOWNLOAD // COMMAND_DOWNLOAD_SEGMENT_CFG(0x%08x-0x%08x)\n", curAddress, curAddress+dataLen-1);
        curAddress += dataLen;
        break;

      case HOST_CMD_CFG_DOWNLOAD_DONE:
        MxL_HRCLS_DEBUG("I2CTRC_FW_DOWNLOAD // COMMAND_DOWNLOAD_DONE_CFG(0x%08x)\n", MxL_HRCLS_Ctrl_GetBigEndian32((UINT8*)dataPtr));
        break;

      default: 
        MxL_HRCLS_ERROR("Invalid command (%d)\n", commandId);
        break;
    }
#endif  

    // Copy data from dataPtr to downloadFwSeg.data
    for (i = 0; i < dataLen; i++)  // Copy data from dataPtr to downloadFwSeg.data
      downloadFwSeg.data[i] = (UINT8)*((UINT8*)dataPtr+i);
    
    downloadFwSeg.payloadLen = dataLen;
    downloadFwSeg.payloadCheckSum = 0;
    downloadFwSeg.syncChar = 0;
    downloadFwSeg.commandId = 0;

    status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &downloadFwSeg, commandId, (UINT8)downloadBlockCnt);
    MXLERR(
    if (status != MXL_SUCCESS)
    { 
       MxL_HRCLS_ERROR("Calling Ctrl_SendHostCommand error ! (cmdId:%d)\n", commandId);
    }
    )

    if ((status == MXL_SUCCESS) && (commandId == HOST_CMD_CFG_DOWNLOAD_DONE))
    {
      status = MxL_HRCLS_Ctrl_GetDeviceResp(devContextPtr, HOST_CMD_CFG_DOWNLOAD_DONE, &response);

      if (status == MXL_SUCCESS)
      {
        if ((0xFF != response.syncChar) || (0x00 != response.errorCode))
        {
          MXLERR(MxL_HRCLS_ERROR("HOST_CMD_DOWNLOAD_DONE_CFG failed: syncChar=0x%02x errorCode=0x%02x\n", 
             response.syncChar, response.errorCode);)
          status = MXL_FAILURE;
        }
      }
      MXLERR(
      else
      {
        MxL_HRCLS_ERROR("HOST_CMD_DOWNLOAD_DONE_CFG failed MxL_HRCLS_Ctrl_GetDeviceResp\n");
      }
      )
    }
#ifdef _DOWNLOAD_DEBUG_ENABLED_
    else if (commandId == HOST_CMD_CFG_DOWNLOAD_SEGMENT)
    {
      MxL_HRCLS_Ctrl_CheckCommandResponse(devId, HOST_CMD_CFG_DOWNLOAD_SEGMENT);
      // MxL_HRCLS_DEBUG("Block %d is downloaded successfully. \n", downloadBlockCnt);
    }
    else
    {
      MxL_HRCLS_Ctrl_CheckCommandResponse(devId, HOST_CMD_CFG_SET_ADDR);
    }
#endif  
  }
  else
  {
    MXLERR(MxL_HRCLS_ERROR("Invalid command length (%d)\n", dataLen););
    status = MXL_FAILURE;
  }

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReadTable16
--| 
--| AUTHOR        : Sudhamshu Ramachandra
--|
--| DATE CREATED  : 10/31/2011
--|                  
--| DESCRIPTION   : Read a table from memory based on table id. 
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadTable16(UINT8 devId, UINT16 tableId, UINT16* readBufPtr, UINT16 length, UINT16 offsetBytes)
{
  UINT8 status = MXL_SUCCESS;  
  UINT16 count16; 
  
  MXLENTERSTR
  MXLENTER(MxL_HRCLS_DEBUG("tableId=%d, length=%d, offsetBytes=%d\n", tableId, length, offsetBytes););
  status = MxLWare_HRCLS_OEM_WriteRegister(devId, MEM_I2C_TABLE_ID, tableId); // Setting table id and therefore memory address for read
  if (MXL_SUCCESS == status)
  {
    status = MxLWare_HRCLS_OEM_WriteRegister(devId, MEM_I2C_ADDR_OFFSET, offsetBytes); // Setting the offset from the beginning of the table. 
    if (status == MXL_SUCCESS)
    {
      for (count16 = 0; (count16 < length) && (status == MXL_SUCCESS); count16++)
      {
        status |= MxLWare_HRCLS_OEM_ReadRegister(devId, MEM_I2C_DATA_16_AI, readBufPtr);      
        readBufPtr++;
      }
    } else status = MXL_FAILURE;
  }else status = MXL_FAILURE;
  MXLEXITSTR(status);
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SendHostCommand
--|
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24 
--|
--| DESCRIPTION   : This function send host command to device, not read response.
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_SendHostCommand(UINT8 devId, HOST_COMMAND_T* cmdPtr, MXL_CMD_ID_E cmdId, UINT8 seqNum)
{
  MXL_STATUS_E status;
  UINT16 regAddr = (((UINT16)(MXL_HRCLS_HOST_COMMAND_SYNC_BYTE + (seqNum&7)))<< 8) + (UINT16)cmdId;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLDBG3(MXLENTER(MxL_HRCLS_DEBUG("+++++ SendHostCommand (%x)\n", cmdId);););

  if (cmdId < MXL_HRCLS_HOST_CMD_MAX)
  {
    if ((devContextPtr->firmwareStatus == FIRMWARE_STATUS_LOADED) || (cmdId <= HOST_CMD_CFG_DOWNLOAD_DONE))
    {
      MXLDBG3(
      UINT64 timeStart;
      UINT64 timeNow;

      MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
      timeStart = timeNow;
      )
      /* Calculate checksum */
      cmdPtr->payloadCheckSum = MxL_HRCLS_Ctrl_CalCheckSum(cmdPtr->payloadLen, cmdPtr->data); 
#ifdef _MXL_BOOTLOADER_TEST_ENABLED_
      if (Diag_debugDownloadErrInjCnt && HOST_CMD_CFG_DOWNLOAD_SEGMENT == cmdId)
      {
        Diag_debugDownloadErrInjCnt--;
        if (Diag_debugDownloadErrInjCnt == 0)
        {
          MxL_HRCLS_DEBUG("Corrupting checksum of %dth packet from %02x to %02x\n",
              Diag_debugDownloadErrInjCnt,
              cmdPtr->payloadCheckSum,
              cmdPtr->payloadCheckSum+1);
          // Corrupt checksum by adding one
          cmdPtr->payloadCheckSum++;
        }
      }
#endif // _MXL_BOOTLOADER_TEST_ENABLED_
      status = MxLWare_HRCLS_OEM_WriteBlock(devId, regAddr, (UINT16) cmdPtr->payloadLen + 2, &(cmdPtr->payloadCheckSum));
      MXLDBG3(
      MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
      MxL_HRCLS_DEBUG("SendHostCommand executed in %dms\n", (unsigned int) MXL_DIV_ROUND(((SINT32)(timeNow - timeStart)),1000));
      )
      // MxL_HRCLS_Diag_SpinOnRegField(devId, MAILBOX_REG_NUM_PENDING_HOST_CMDS, 0xFFFF, 0, SPIN_UNTIL_EQ, 1000000);
    }
    else
    {
#ifdef _MXL_DIAG_ENABLED_
      if (cmdId > HOST_CMD_CFG_DOWNLOAD_DONE)
      {
        status = MxL_HRCLS_Diag_SendHostCommand(devId, cmdId, cmdPtr); 
      }
      else
#endif // _MXL_DIAG_ENABLED_
      {
        status = MXL_FAILURE;
        MXLERR(MxL_HRCLS_ERROR("Firmware is not downloaded. Cannot execure commands\n"););
      }
    }
  } 
  else 
  {
    MxL_HRCLS_ERROR("%s: invalid command id %d\n", __FUNCTION__, cmdId);
    status = MXL_FAILURE;
  }
  MXLDBG3(MXLEXITSTR(status););

  return status; 
}

MXL_STATUS_E MxL_HRCLS_Ctrl_ReadBlock16(UINT8 devId, UINT16 cmdId, UINT16 offset, UINT16 ReadWords, UINT16 *BufPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT8 * bufferPointer = (UINT8 *) BufPtr;
  UINT16 bytesToBeRead = ReadWords * 2;

  while (bytesToBeRead)
  {
    UINT16 count;

    if (bytesToBeRead > MXL_HRCLS_OEM_MAX_BLOCK_READ_LENGTH) count = MXL_HRCLS_OEM_MAX_BLOCK_READ_LENGTH; else count = bytesToBeRead;
    status = MxLWare_HRCLS_OEM_ReadBlockExt(devId, cmdId, offset, count, bufferPointer);

    bytesToBeRead -= count;
    bufferPointer += count;
    offset += count;
  }
#ifdef _MXL_HRCLS_LITTLE_ENDIAN_  
  if ((status == MXL_SUCCESS) && (1)) // Little endian
  {
    UINT16 numOfWords;
    for (numOfWords = 0; numOfWords < ReadWords; numOfWords++)
    {
      BufPtr[numOfWords] = (((BufPtr[numOfWords]) & 0xFF) << 8) | (((BufPtr[numOfWords]) & 0xFF00) >> 8);
    }
  }
#endif  
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetDeviceResp
--|
--| AUTHOR        : Richard Liu
--|
--| DATE CREATED  : 2010/12/24 
--|
--| DESCRIPTION   : This function gets device response from certain Sub address.
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_GetDeviceResp(MXL_HRCLS_DEV_CONTEXT_T* devContextPtr, UINT16 subAddr, PFW_RESPONSE_T responsePtr)
{
  UINT16 bufferSize,i;
  UINT8 status = MXL_SUCCESS;
  UINT8 readBuffer[sizeof(HOST_COMMAND_T)] = {0};
  UINT8 devId = devContextPtr->devId;

  // Read response header firstly 
  status |= (UINT8)MxLWare_HRCLS_OEM_ReadBlock(devId, subAddr, COMMAND_HEADER_LENGTH, readBuffer);

  MXLDBG2(MxL_HRCLS_DEBUG("Block read back : SYNC=0x%02X, errorCode=0x%02X, checkSum=0x%02X, dataLen=0x%02X. \n",
        readBuffer[0],readBuffer[1],readBuffer[2],readBuffer[3]););

  if (status == MXL_SUCCESS)  // Read block success 
  {
    responsePtr->syncChar = readBuffer[0];
    responsePtr->errorCode = (MXL_RSP_ID_E)readBuffer[1];
    responsePtr->payloadCheckSum = readBuffer[2];
    responsePtr->payloadLen = readBuffer[3];

    if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
    {
      // Neglected for Version compatibility.
      responsePtr->errorCode = RESPONSE_ID_SUCCESS;
    }
    else
    {
      if (responsePtr->errorCode == RESPONSE_ID_SUCCESS)
      {
        // Check if need to read response data
        if (responsePtr->payloadLen > 0)
        {
          bufferSize = responsePtr->payloadLen;

          // Read Response data 
          status |= (UINT8)MxLWare_HRCLS_OEM_ReadBlock(devId, subAddr, bufferSize, readBuffer);  

          // Check if received response data is complete or not 
          status |= MxL_HRCLS_Ctrl_CheckIntegration(responsePtr->payloadCheckSum, (UINT8)bufferSize, readBuffer);

          if (status == MXL_SUCCESS) // Read response data success 
          {
            for (i = 0; i < bufferSize; i++)  // Copy data from readBuffer to responsePtr->data
              responsePtr->data[i] = readBuffer[i]; 
          }
        }
      }
      else
      {
        status = MXL_FAILURE;
        MXLERR(MxL_HRCLS_ERROR("%s wrong response error code (%d)\n", __FUNCTION__, responsePtr->errorCode););
      }
    }

  }
  return((MXL_STATUS_E)status);
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_DeviceStandby
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 06/22/2011
--|
--| DESCRIPTION   : Config device to standby
--|                 Turn off all nb channels, demod, mpegout, tuners, splitter
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_DeviceStandby(UINT8 devId)
{
  devId=devId; // anti-warning
  return MXL_SUCCESS;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractRegAddr
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 09/13/2011
--|
--| DESCRIPTION   : Extract the address out of addr, lsbloc, nbits macro
--|
--| RETURN VALUE  : Register value
--|
--|-------------------------------------------------------------------------------------*/

UINT16 MxL_HRCLS_Ctrl_ExtractRegAddr(UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth) 
{
  bitLocation = bitLocation; // anti-warning
  bitWidth = bitWidth; // anti-warning
  return(regAddr);
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractRegAddr32
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 01/26/2013
--|
--| DESCRIPTION   : Extract the 32-bit address out of addr, lsbloc, nbits macro
--|
--| RETURN VALUE  : Register value
--|
--|-------------------------------------------------------------------------------------*/

UINT32 MxL_HRCLS_Ctrl_ExtractRegAddr32(UINT32 regAddr, UINT8 bitLocation, UINT8 bitWidth) 
{
  bitLocation = bitLocation; // anti-warning
  bitWidth = bitWidth; // anti-warning
  return(regAddr);
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractRegMask
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 09/13/2011
--|
--| DESCRIPTION   : Get mask out of lsbloc and nbits
--|
--| RETURN VALUE  : Register value
--|
--|-------------------------------------------------------------------------------------*/

UINT16 MxL_HRCLS_Ctrl_ExtractRegMask(UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth) 
{
  regAddr = regAddr; // anti-warning
  return (HWD_MASK(bitLocation, bitWidth));
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractRegMask32
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 09/13/2011
--|
--| DESCRIPTION   : Get 32-bit mask out of lsbloc and nbits
--|
--| RETURN VALUE  : Register value
--|
--|-------------------------------------------------------------------------------------*/

UINT32 MxL_HRCLS_Ctrl_ExtractRegMask32(UINT32 regAddr, UINT8 bitLocation, UINT8 bitWidth) 
{
  regAddr = regAddr; // anti-warning
  return (MXL_MASK(bitLocation, bitWidth));
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractRegLsbLoc
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 09/13/2011
--|
--| DESCRIPTION   : Get mask out of lsbloc and nbits
--|
--| RETURN VALUE  : Register value
--|
--|-------------------------------------------------------------------------------------*/

UINT8 MxL_HRCLS_Ctrl_ExtractRegLsbLoc(UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
  regAddr=regAddr; //anti-warning
  bitWidth=bitWidth; //anti-warning
  return bitLocation;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractRegLsbLoc32
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 09/13/2011
--|
--| DESCRIPTION   : Extract bitLocation for a 32-bit regAddr
--|
--| RETURN VALUE  : Register value
--|
--|-------------------------------------------------------------------------------------*/

UINT8 MxL_HRCLS_Ctrl_ExtractRegLsbLoc32(UINT32 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
  regAddr=regAddr; //anti-warning
  bitWidth=bitWidth; //anti-warning
  return bitLocation;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractRegNumBits
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 09/13/2011
--|
--| DESCRIPTION   : Get bitWidth of field
--|
--| RETURN VALUE  : Bit width of field
--|
--|-------------------------------------------------------------------------------------*/

UINT8 MxL_HRCLS_Ctrl_ExtractRegNumBits(UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
  regAddr = regAddr; // anti-warning
  bitLocation = bitLocation; // anit-warning;
  return bitWidth;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractRegNumBits32
--| 
--| AUTHOR        : SidS
--|
--| DATE CREATED  : 03/01/2013
--|
--| DESCRIPTION   : Get bitWidth of field
--|
--| RETURN VALUE  : Bit width of field
--|
--|-------------------------------------------------------------------------------------*/

UINT8 MxL_HRCLS_Ctrl_ExtractRegNumBits32(UINT32 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
  regAddr = regAddr; // anti-warning
  bitLocation = bitLocation; // anit-warning;
  return bitWidth;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ExtractFieldFromRegister
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 12/29/2011
--|
--| DESCRIPTION   : Extract field value from register
--|
--| RETURN VALUE  : Field value
--|
--|-------------------------------------------------------------------------------------*/
UINT16 MxL_HRCLS_Ctrl_ExtractFieldFromRegister(UINT16 regValue, UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
  UINT16 value;

  regAddr = regAddr;  // anti-warning

  value = (regValue >> bitLocation);
  value &= (UINT16) MXL_HRCLS_LSB_MASK(bitWidth); 

  return value;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 12/29/2011
--|
--| DESCRIPTION   : Toggle field value -> 1 -> 0 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(UINT8 devId, UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
  UINT8 status;
  UINT16 regData = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("regAddr=%x, bitLoc=%d, bitWidth=%d\n", regAddr, bitLocation, bitWidth););
  bitWidth = bitWidth;  // anti-warning

  status = MxLWare_HRCLS_OEM_ReadRegister(devId, regAddr, &regData);
  if (status == MXL_SUCCESS)
  {
    regData |= (UINT16) (1 << bitLocation);
    status = MxLWare_HRCLS_OEM_WriteRegister(devId, regAddr, regData);

    regData &= (UINT16) (~(1 << bitLocation));
    status |= MxLWare_HRCLS_OEM_WriteRegister(devId, regAddr, regData);
  }
  MXLEXITSTR(status);
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 12/29/2011
--|
--| DESCRIPTION   : Toggle field value -> 1 -> 0 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne(UINT8 devId, UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
  UINT8 status;
  UINT16 regData = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("regAddr=%x, bitLoc=%d, bitWidth=%d\n", regAddr, bitLocation, bitWidth););
  bitWidth = bitWidth;  // anti-warning

  status = MxLWare_HRCLS_OEM_ReadRegister(devId, regAddr, &regData);
  if (status == MXL_SUCCESS)
  {
    regData &= (UINT16) (~(1 << bitLocation));
    status = MxLWare_HRCLS_OEM_WriteRegister(devId, regAddr, regData);
    regData |= (UINT16) (1 << bitLocation);
    status |= MxLWare_HRCLS_OEM_WriteRegister(devId, regAddr, regData);
  }
  MXLEXITSTR(status);
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_MapCodeToTempInDegrees
--| 
--| AUTHOR        : Sudhamshu Ramachandra
--|
--| DATE CREATED  : 10/24/2012
--|
--| DESCRIPTION   : Decodes code to temperature in degree C. Firmware is encoding to reduce bitwidth
--|                 Due to temperature sensor changes for V2/3, temperatue is now in degree C.
--|
--| INPUTS        : Temperature code
--|
--| RETURN VALUE  : Temperature in degree C
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_MapCodeToTempInDegrees(UINT8 code, UINT8* temperature)
{
  UINT8 status = MXL_SUCCESS;
#ifdef _MXL_HRCLS_TSENS_V2_ENABLED_        
  if (code < MXL_HRCLS_TEMP_CODE_MAP_LEN)
  {
    if (temperature)
    {
      *temperature = MxL_HRCLS_TemperatureCodeMap[code];
    }
    else
    {
      status = MXL_INVALID_PARAMETER;
    }
  }
  else
  {
    status = MXL_INVALID_PARAMETER;
  }
#else
  code = code; // anti-warning
  *temperature = MxL_HRCLS_DEFAULT_TEMPERATURE_TSENS_NOTUSED;
#endif
  return (MXL_STATUS_E) status;
}


/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReqTunerTemperature
--| 
--| AUTHOR        : Sudhamshu Ramachandra
--|
--| DATE CREATED  : 10/24/2012
--|
--| DESCRIPTION   : This function reads the temperature code and updates in device context
--|                 In case of V2/3, the code is translated to temperature.
--|                 Readback is done from firmware mailbox.
--|
--| INPUTS        : 
--|
--| RETURN VALUE  : Temperature
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_ReqTunerTemperatureCode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  MXL_STATUS_E status;
  UINT16 regData = 0;

  status = MxLWare_HRCLS_OEM_ReadRegister(devContextPtr->devId, MAILBOX_REG_TEMPERATURE_CODE, &regData);
  MXLDBG2(MxL_HRCLS_DEBUG("MailboxReg [0x%04x] -> 0x%04x\n", MAILBOX_REG_TEMPERATURE_CODE, regData););

  if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (devContextPtr->chipVersion == 1))
  {
    devContextPtr->currentTemp = (regData) & 0x07;
  }
  else
  {
    status = MxL_HRCLS_Ctrl_MapCodeToTempInDegrees(((regData) & MXL_HRCLS_TEMP_CODE_MAP_MASK), &devContextPtr->currentTemp);
  }
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReqTunerRxPwr 
--| 
--| AUTHOR        : Mariusz Murawski, Sudhamshu Ramachandra
--|
--| DATE CREATED  : 01/30/2012
--|
--| DESCRIPTION   : This function reads power level for given channel ID
--|                 Due to temperature sensor changes for V2/3, temperatue is now in degree C.
--|
--| INPUTS        : Physical DFE channel number 
--|
--| RETURN VALUE  : Uncalibrated power level, accuracy estimation, 
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_ReqTunerRxPwr(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, 
                      MXL_HRCLS_CHAN_ID_E     phyChanId,
                      UINT16*                 rxRawPwrIndBuVPtr,
                      MXL_HRCLS_RX_PWR_ACCURACY_E * accuracyPtr)
{
  MXL_STATUS_E status;
  UINT16 regData = 0;
  UINT16 rawPwr;
  UINT8 accuracy;

  status = MxLWare_HRCLS_OEM_ReadRegister(devContextPtr->devId, MAILBOX_REG_RX_PWR + (2 * phyChanId), &regData);

  MXLDBG2(MxL_HRCLS_DEBUG("MailboxReg [0x%04x] -> 0x%04x\n", MAILBOX_REG_RX_PWR + (2 * phyChanId), regData););
  rawPwr = regData & 0x3FF;
  accuracy = (regData >> 15) & 0x01;
  if (rxRawPwrIndBuVPtr) *rxRawPwrIndBuVPtr = rawPwr;

  if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (devContextPtr->chipVersion == 1))
  {
    devContextPtr->currentTemp = (regData >> 10) & 0x07;
  }
  else
  {
    status = MxL_HRCLS_Ctrl_MapCodeToTempInDegrees(((regData >> 10) & MXL_HRCLS_TEMP_CODE_MAP_MASK), &devContextPtr->currentTemp);
  }

  if (accuracyPtr)
  {
    if (rawPwr == 0)
    {
      *accuracyPtr = MXL_HRCLS_PWR_INVALID;
    }
    else
    {
      *accuracyPtr = (accuracy == 1)?MXL_HRCLS_PWR_AVERAGED:MXL_HRCLS_PWR_ROUGH;
    }
  }

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CfgTunerChanTune
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 01/30/2012
--|
--| DESCRIPTION   : This function tunes given physical channel to desired frequency
--|                 and bandwidth 
--|
--| INPUTS        : Physical DFE channel number, bandwidth in MHz and Frequency in Hz 
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE 
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_CfgTunerChanTune(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
                        MXL_HRCLS_CHAN_ID_E phyChanId,              
                        UINT8     bandWidthInMhz,    
                        UINT32    centerFrequencyInHz)
{
  MXL_STATUS_E status;
  HOST_COMMAND_T tunerChanTuneCfg;
  UINT8         chnBwMode;
  /* Form command payload */
  
  chnBwMode = (6==bandWidthInMhz)?DFE_CHN_BW_6_MODE:DFE_CHN_BW_8_MODE;           

  tunerChanTuneCfg.data[0]    = ((UINT8) phyChanId)       << 0;
  tunerChanTuneCfg.data[0]   |= (UINT8)(1                 << 5); // channel enable
  tunerChanTuneCfg.data[0]   |= (UINT8)((chnBwMode)       << 7);
  tunerChanTuneCfg.data[1]    = (UINT8)((centerFrequencyInHz >> 0)  & 0xFF);
  tunerChanTuneCfg.data[2]    = (UINT8)((centerFrequencyInHz >> 8)  & 0xFF);
  tunerChanTuneCfg.data[3]    = (UINT8)((centerFrequencyInHz >> 16) & 0xFF);
  tunerChanTuneCfg.data[4]    = (UINT8)((centerFrequencyInHz >> 24) & 0xFF);
  tunerChanTuneCfg.payloadLen = 5;
  tunerChanTuneCfg.payloadCheckSum = 0;
  tunerChanTuneCfg.syncChar = 0;
  tunerChanTuneCfg.commandId = 0;

  /* Send host command */
  status = MxL_HRCLS_Ctrl_SendHostCommand(devContextPtr->devId, &tunerChanTuneCfg, HOST_CMD_CFG_TUNER_CHAN_TUNE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SelectAFETiltMode
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 02/07/2012
--|
--| DESCRIPTION   : This function sets required tilt mode for calibration purposes.
--|                 It also waits until tilt is indicated by FW as set. 
--|
--| INPUTS        : Device context, new tilt id, timeout
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE 
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxL_HRCLS_Ctrl_SelectAFETiltMode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E newTilt, UINT16 timeoutMs)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  UINT16 regData;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("newTilt=%d, timeoutMs=%d\n", newTilt, timeoutMs);)
  regData = 0x08 | ((UINT16) newTilt & 0x07);
  status = MxLWare_HRCLS_OEM_WriteRegister(devContextPtr->devId, MAILBOX_REG_AFE_TILT_MODE, regData); 

  if (status == MXL_SUCCESS)
  {
    UINT64 timeNow;
    UINT64 timeEnd;
    MXLDBG1(
    UINT64 timeStart;
    )

    MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
    timeEnd = timeNow + (UINT64) (timeoutMs * 1000);
    MXLDBG1(
    timeStart = timeNow
    );

    regData = 0;
    do
    {
      MxLWare_HRCLS_OEM_DelayUsec(1000);
      status = MxLWare_HRCLS_OEM_ReadRegister(devContextPtr->devId, MAILBOX_REG_AFE_TILT_MODE, &regData); 
      MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
    } while ((regData & 0x08) && (status == MXL_SUCCESS) && (timeNow < timeEnd));
    status = (((regData & 0x08) == 0) && (status == MXL_SUCCESS))?MXL_SUCCESS:MXL_FAILURE;
    if (status == MXL_SUCCESS) devContextPtr->currentAfeTiltMode = newTilt;
    MXLDBG1(MxL_HRCLS_DEBUG("Tilt change to #%d in %ums %s\n", newTilt, (UINT32) (((SINT32)(timeNow - timeStart)) / 1000), (status == MXL_SUCCESS)?"OK":"FAIL"););
  }    
  MXLEXITSTR(status);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CfgTunerFFTRunMode
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 03/19/2012
--|
--| DESCRIPTION   : This function initialize FFT engine 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_READY
--|                 
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_CfgTunerFFTRunMode(
    MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, 
    MXL_HRCLS_FFT_RUN_MODE_E runMode, 
    MXL_HRCLS_FFT_SOURCE_TYPE_E sourceType, 
    UINT8 sourceId, 
    MXL_HRCLS_FFT_DEBUG_TYPE_E debugType,
    UINT32 freqStartInHz,
    UINT32 freqStepInHz,
    UINT16 stepsReq
    )
{
  MXL_STATUS_E status = MXL_SUCCESS;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("runMode=%d, sourceType=%d, sourceId=%d, debugType=%d\n", runMode, sourceType, sourceId, debugType);)
    
  {
    HOST_COMMAND_T fftHostCmd;

    fftHostCmd.data[0]    = (UINT8) runMode;
    fftHostCmd.data[1]    = (UINT8) sourceId;
    fftHostCmd.data[2]    = (UINT8) sourceType;
    fftHostCmd.data[3]    = (UINT8) debugType;
    fftHostCmd.payloadLen = 4;
    if (MXL_HRCLS_FFT_MODE_SPECTRUM_API == runMode)
    {
      fftHostCmd.data[4]    = (UINT8)((freqStartInHz>> 0)  & 0xFF);
      fftHostCmd.data[5]    = (UINT8)((freqStartInHz>> 8)  & 0xFF);
      fftHostCmd.data[6]    = (UINT8)((freqStartInHz>> 16) & 0xFF);
      fftHostCmd.data[7]    = (UINT8)((freqStartInHz>> 24) & 0xFF);
      fftHostCmd.data[8]    = (UINT8)((freqStepInHz>> 0)  & 0xFF);
      fftHostCmd.data[9]    = (UINT8)((freqStepInHz>> 8)  & 0xFF);
      fftHostCmd.data[10]    = (UINT8)((freqStepInHz>> 16) & 0xFF);
      fftHostCmd.data[11]    = (UINT8)((freqStepInHz>> 24) & 0xFF);
      fftHostCmd.data[12]    = (UINT8)((stepsReq>> 0) & 0xFF);
      fftHostCmd.data[13]    = (UINT8)((stepsReq>> 8) & 0xFF);
      fftHostCmd.payloadLen += 10;
    }

    /* Send host command */
    status = MxL_HRCLS_Ctrl_SendHostCommand(devContextPtr->devId, &fftHostCmd, HOST_CMD_CFG_FFT_STATE, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);
  }
  MXLEXITSTR(status);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetVbeCode
--| 
--| AUTHOR        : Sudhamshu Ramachandra
--|
--| DATE CREATED  : 10/11/2012
--|
--| DESCRIPTION   : Obtains VBE codes for a given temperature sensor mode.
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_SUPPORTED
--|                 
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_GetVbeCode(
   UINT8 devId,
   UINT8 tsensMode, 
   UINT16* vbeCode
   )
{
  UINT8 status = (UINT8) MXL_SUCCESS;
  UINT16 readData;

  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TEMPSEN_MODE, tsensMode);
  MxLWare_HRCLS_OEM_DelayUsec(1);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TEMPSEN_START, 1);
  
  // ANA_DIG_TEMPSEN_READY will be set within 2.44ms
#ifndef _MXL_DIAG_ENABLED_              
  status |=  MxL_HRCLS_Ctrl_WaitForFieldValue(devId, ANA_DIG_TEMPSEN_READY, 1, MXL_TRUE, 10, &readData); 
#else
  MxLWare_HRCLS_OEM_DelayUsec(10*1000);
  status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_TEMPSEN_READY, &readData);
  if (readData == 0) status = MXL_FAILURE;
#endif
  status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_TEMPSEN_OUT, vbeCode);

  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TEMPSEN_START, 0);
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetTemperatureCode
--| 
--| AUTHOR        : Sudhamshu Ramachandra
--|
--| DATE CREATED  : 10/10/2012
--|
--| DESCRIPTION   : Obtains temperature which is required for RefSx
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_SUPPORTED
--|                 
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_GetTemperatureCode(
   MXL_HRCLS_DEV_CONTEXT_T *devContextPtr,
   UINT16* tsensCode
   )
{
  UINT8 status = (UINT8) MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;
#ifdef _MXL_HRCLS_TSENS_V2_ENABLED_        
  UINT16 vbeCode;
  UINT16 nVbeCode;
  UINT16 vbeCodeAvg;
  UINT32 vbeCodeAccum = 0;
  SINT32 deltaVbeCode;
  SINT32 temperature;
  UINT8  tsensMode;
#endif
  UINT8  GrayCode[] = MxL_HRCLS_GRAY_CODE;

  if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
  {
    status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_TSENS_READBACK, tsensCode); // Read back temp sensor
  }
  else // if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
  {
#ifdef _MXL_HRCLS_TSENS_V2_ENABLED_        
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_EN_RCTUNECLK, 1);
    MxLWare_HRCLS_OEM_DelayUsec(50);

    status |= MxL_HRCLS_Ctrl_GetVbeCode(devId, 1<<0, &vbeCode); // Dummy run due to bad value first time.
    for (tsensMode = 0; tsensMode < MXL_HRCLS_TSENS_MODE_MAX; tsensMode++)
    {
      status |= MxL_HRCLS_Ctrl_GetVbeCode(devId, 1<<tsensMode, &vbeCode);
      vbeCodeAccum += vbeCode;
    } 
    vbeCodeAvg = vbeCodeAccum/MXL_HRCLS_TSENS_MODE_MAX;
    status |= MxL_HRCLS_Ctrl_GetVbeCode(devId, MXL_HRCLS_TSENS_MODE_NVBE, &nVbeCode);
 
    deltaVbeCode = nVbeCode - vbeCodeAvg;

    temperature = MXL_DIV_ROUND_S(((SINT32)(deltaVbeCode * 7237 - 2237644)),8192);
    MXLDBG2(MxL_HRCLS_DEBUG("temperature = %d\n", temperature););
    *tsensCode = GrayCode[MxL_HRCLS_TEMPERATURE_INDEX(temperature)]; 

    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_EN_RCTUNECLK, 0);

    if (MXL_SUCCESS != status)
    {
      *tsensCode = GrayCode[MxL_HRCLS_TEMPERATURE_INDEX(MxL_HRCLS_DEFAULT_TEMPERATURE)]; 
      status = MXL_SUCCESS;
      MXLERR(MxL_HRCLS_ERROR("[HRCLS] Error! MxL_HRCLS_Ctrl_GetTemperatureCode Failed\n"););
    }
#else
    *tsensCode = GrayCode[MxL_HRCLS_TEMPERATURE_INDEX(MxL_HRCLS_DEFAULT_TEMPERATURE_TSENS_NOTUSED)]; 
#endif //_MXL_HRCLS_TSENS_V2_ENABLED_        
  }
  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CfgDevXtalSetting
--| 
--| AUTHOR        : Sailesh/Jun Huang
--|
--| DATE CREATED  : 02/14/2012
--|
--| DESCRIPTION   : This function enables RefSx, and runs RCAL, and RCTUNE
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_SUPPORTED
--|                 
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_CfgDevXtalSetting(
   MXL_HRCLS_DEV_CONTEXT_T *devContextPtr,
   UINT8     xtalCap
   )
{
  UINT8 status = (UINT8) MXL_SUCCESS;
  UINT16 dataRb = 0;
  UINT16 rcTuneCode = 0;
  UINT8 devId = devContextPtr->devId;

  xtalCap = xtalCap;
  status = (UINT8) MxL_HRCLS_Ctrl_ReadRegisterField(devId, HCLK_SEL, &dataRb);
  if ((status == (UINT8) MXL_SUCCESS) && (dataRb == 1))
  {
    if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
    {
      // Unlock radio control
      status = MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne(devId, DIG_ANA_ASIC_RDY);
      status |= MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne(devId, DIG_ANA_ASIC_RDY);
      // ---------------------------------------------------------------------------------------
      // Overwrite defaults (REF: RADIO_PG_01032012 "Miscellaneous"->"Default Overwrites")
      // ---------------------------------------------------------------------------------------
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_MISC_SPARE,     0x1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_RCTUNE,    0x1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_CLKOUT_AMP,     0x6);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_REG_ANA_AMP, 0x3);
      // Removed per 0.93 Radio PG
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_REG_CLK_AMP, 0x3);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_REG_DIG_AMP, 0x3);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_BUF1_ENA,  0x1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_LNA_TILT_MODE_SOURCE, 0x1);

      // Added per 0.95 Radio PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_SPARE, 0xAB);
      // Added per 0.95 Radio PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TX_CLK_FLIP, 0x1);
      // Added per 0.95 Radio PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_NB_BUF_ENA, 0x0);
      // Added per 0.94 but removed per 0.95 Radio PG
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, Dig_ana_tx_gain,       1);
      // Changed from 7 to 6 per 0.95 Radio PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TX_SPARE,       0x06);
      // Removed per 0.95 Radio PG
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_SPARE,      0xAB);

      // Improves phase noise/spur at 3MHz to 9MHz range 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFSX_SEL_DSM_ORDER3, 0x1); 

      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_INPUTMATCH,     0x0); 

      // cap setting is causing problems. Removed
    }
    else // if ((MXL_HRCLS_HERCULES_CHIP_ID == devContextPtr->chipId) && (1 == devContextPtr->chipVersion))
    {
      // ---------------------------------------------------------------------------------------
      // Overwrite defaults (REF: RADIO_PG_01032012 "Miscellaneous"->"Default Overwrites")
      // ---------------------------------------------------------------------------------------
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_XTAL_CAP,       0x0);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_MISC_SPARE,     0x1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_RCTUNE,    0x1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_CLKOUT_AMP,     0x6);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_REG_ANA_AMP, 0x3);
      // Removed per 0.93 Radio PG
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_REG_CLK_AMP, 0x3);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_REG_DIG_AMP, 0x3);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_BUF1_ENA,  0x1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_LNA_TILT_MODE_SOURCE, 0x1);

      // Added per 0.95 Radio PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_SPARE, 0x8C);
      // Added per 0.95 Radio PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TX_CLK_FLIP, 0x1);
      // Added per 0.95 Radio PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_NB_BUF_ENA, 0x0);
      // Added per 0.94 but removed per 0.95 Radio PG
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, Dig_ana_tx_gain,       1);
      // Changed from 7 to 6 per 0.95 Radio PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TX_SPARE,       0x2A);
      // Removed per 0.95 Radio PG
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_SPARE,      0xAB);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_GAIN,             0x24); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_CLKOUT_TEST_MODE,    0x1); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_XTAL_BUF_LP,         0x1); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFSX_SEL_DSM_ORDER3, 0x1); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_SPARE,            0x10); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TX_GAIN,             0x2); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TEST0_ADDR,          0xF); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TEST0_MODE,          0xF); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_REG_FE_AMP,      0x4); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_SX_REG_AMP,    0x4); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFSX_SX_REG_AMP,     0x4); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFSX_VCO_REG_AMP,    0x4); 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_INPUTMATCH,     0x0); 

      // cap setting is causing problems. Removed
      // Unlock radio control
      status |= MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne(devId, DIG_ANA_ASIC_RDY);
      status |= MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne(devId, DIG_ANA_ASIC_RDY);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TEMPSEN_DURATION, 3); // Overwrite default.
      // Not an overwrite default. One time enabling of temperature sensor.
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_TEMPSEN_ENA, 1); 
    }

    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, ECO_1 , 0x2);           // Cosim checkpoint

    // ---------------------------------------------------------------------------------------
    // REF SYNTH (REF: VISIO_PG_20120103 - "REF SYNTH", RADIO_PG_20120103 - "REF SYNTH")
    // ---------------------------------------------------------------------------------------
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_ENA, 1);
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_SX_REG_ENA, 1);
    MxLWare_HRCLS_OEM_DelayUsec(1000); // Wait 1000us for regulators to stabilize
    status |= MxL_HRCLS_Ctrl_GetTemperatureCode(devContextPtr, &dataRb); // Read back temp sensor
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_TEMP_INFO, dataRb); // Program temp sensor info

#ifndef _SEQ_COSIM_ENABLED_
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_INTMOD, 200);
#endif // _SEQ_COSIM_ENABLED_

    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_START_TUNE, 0); // Start tune, toggle from 0->1, 
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_START_TUNE, 1);

    // Wait for ana_dig_refsx_tune_done to become 1.
    // Changed from 10us to 1000ms per 0.95 Radio PG
    MxLWare_HRCLS_OEM_DelayUsec(MXL_HRCLS_REF_SX_TUNE_TO_IN_US /* changed from 10 us to 1ms */);
    status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_REFSX_TUNE_DONE, &dataRb);
    if (dataRb == 1)
    {
      MXLDBG2(MxL_HRCLS_DEBUG("ANA_DIG_REFSX_TUNE_DONE = 1\n"););
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, ECO_1 , 0x3);           // Cosim checkpoint

      // ---------------------------------------------------------------------------------------
      // Enable BB clock (337.5MHz) to ASIC (REF: RADIO_PG_20120103 - "Programing REF SX and BB Clock"
      // ---------------------------------------------------------------------------------------
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_REG_FE_ENA, 1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_BBCLK_ENA, 1);
      MxLWare_HRCLS_OEM_DelayUsec(10); // Wait 10us for regulators to stabilize
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_BBCLK_RSTB, 0);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_BBCLK_RSTB, 1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, HCLK_SEL, 0); // Switch CPU to BB clock
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, ECO_1 , 0x5); // cosim checkpoint

      // ---------------------------------------------------------------------------------------
      // Perform R-CAL (REF: RADIO_PG_20120103 - "Program R-CAL", VISIO_PG_20120103 - "RC Tune")
      // ---------------------------------------------------------------------------------------
      // Note: Resistance Calibration (or R-CAL) is required before the WB tuner starts to receive 
      // signals. It should be run after I2C slave address has been latched into the digital. 
      // Follow the steps below to perform R-CAL
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_NB_ENA, 1); // Removed per 0.91 PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_NBCLK_ENA, 1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_IFCLK_ENA, 1);

      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, PSM_DIG_ANA_DNX_FE_SEQ_BYPASS, 1);   // (QUESTION 33)
      // The following change is made per RADIO_PG 0105
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_DACIN_ENA, 1); // was PSM_DIG_ANA_DNX_FE_SEQ_BYPASS

      MxLWare_HRCLS_OEM_DelayUsec(10); // Wait 10us for regulators and clock to stabilize

      // 3. Set dig_ana_rffe_lp_ena=1 to start the R-CAL. Wait for 10us for ana_dig_rfsx_spare<4> to become 1. 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_LP_ENA, 1); // Start R-CAL

      MxLWare_HRCLS_OEM_DelayUsec(1000/*MXL_HRCLS_RCAL_TO_IN_US was 10us*/);
      status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_RFSX_SPARE, &dataRb); // Read back temp sensor
      MXLDBG3(MxL_HRCLS_DEBUG("ANA_DIG_RFSX_SPARE_BIT4 (RCAL done) = %d\n", (dataRb >> 4) & 1););

      // 4. Read back R-CAL value from ana_dig_rfsx_spare<3:0> and write it to dig_ana_adc_rtune<3:0>.
      status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_RFSX_SPARE, &dataRb); // Read back temp sensor
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_RTUNE, (dataRb & MXL_HRCLS_BITMASK_RFSX_SPARE_RCAL));

      // 5. Set dig_ana_rffe_lp_ena = 0, dig_ana_nb_dnx_fe_seq_bypass = 0, dig_ana_adc_ifclk_ena = 0, dig_ana_adc_nbclk_ena = 0, dig_ana_rffe_nb_ena = 0 to turn of the R-CAL clock. 
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_LP_ENA, 0);
      // The following change is made per RADIO_PG 0105
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_DACIN_ENA, 0); // was PSM_DIG_ANA_DNX_FE_SEQ_BYPASS
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, PSM_DIG_ANA_DNX_FE_SEQ_BYPASS, 0);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_IFCLK_ENA, 0);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_ADC_NBCLK_ENA, 0);
      // MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RFFE_NB_ENA, 0); // Removed per 0.91 PG
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, ECO_1 , 0x6); // cosim checkpoint

      // ---------------------------------------------------------------------------------------
      // Perform RCTUNE (REF: RADIO_PG_20120103 - "Program RC-TUNE", VISIO_PG_20120103 - "RC Tune")
      // ---------------------------------------------------------------------------------------
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_EN_RCTUNECLK, 1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RCTUNE_ENA, 1);
      MxLWare_HRCLS_OEM_DelayUsec(MXL_HRCLS_RCTUNE_TO_IN_US);
      status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, ANA_DIG_RCTUNE, &rcTuneCode);
      //Disable RC Tune block
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_RCTUNE_ENA, 0);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_REFSX_EN_RCTUNECLK, 0);
      //Write back the value read previously.
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_IF_RCTUNE, rcTuneCode);

      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, ECO_1 , 0x7); // cosim checkpoint

      // QUESTION 68: NB ADC and TZ also need the RCTUNE code, but they need to be set
      // after the NB reset is released. Please refer to the NB tuner section for more information.

      // ---------------------------------------------------------------------------------------
      // QUESTION 50: need programming guide for Program RF LPF cap
      // ---------------------------------------------------------------------------------------
    }
    else
    {
      status = (UINT8) MXL_FAILURE;
      MXLERR(MxL_HRCLS_ERROR("[HRCLS] Error! ANA_DIG_REFSX_TUNE_DONE = 0\n"););
    }
  } 
  else 
  {
    status = (UINT8) MXL_NOT_SUPPORTED;
    MXLERR(MxL_HRCLS_ERROR("[HRCLS] REFPLL was already enabled\n"));
  }

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReqDevCurrentAFETiltMode
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/24/2012
--|
--| DESCRIPTION   : This function returns current AFE tilt mode 
--|                 
--| RETURN VALUE  : current tilt mode or MXL_HRCLS_AFE_FLAT_LNABO_0 if failed 
--|                 
--|-------------------------------------------------------------------------------------*/

MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E MxL_HRCLS_Ctrl_ReqDevCurrentAFETiltMode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E tiltMode;
  MXL_STATUS_E status;
  UINT16 data;

  MXLENTERSTR;
  // read current tilt mode
  status = MxLWare_HRCLS_OEM_ReadRegister(devContextPtr->devId, MAILBOX_REG_AFE_TILT_MODE, &data); 
  data &= 0x07;
  if ((data >= MXL_HRCLS_AFE_FLAT_LNABO_0) && (data <= MXL_HRCLS_AFE_TILT4) && (status == MXL_SUCCESS))
  {
    tiltMode = (MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E) data;
    MXLDBG2(MxL_HRCLS_DEBUG("currentAfeTiltMode=%d\n", data););
  }
  else
  {
    tiltMode = MXL_HRCLS_AFE_FLAT_LNABO_0;  // first mode -> index=0
    MXLERR(MxL_HRCLS_ERROR("Current tilt value is incorrect. Assuming current tilt is #%u\n", MXL_HRCLS_AFE_FLAT_LNABO_0););
  }
  MXLEXITSTR((UINT8) tiltMode);

  return tiltMode;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ReqDevCurrentAFEBackOff
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/24/2012
--|
--| DESCRIPTION   : This function returns current AFE back off value
--|                 
--| RETURN VALUE  : current backoff value or 0 if failed 
--|                 
--|-------------------------------------------------------------------------------------*/

UINT8 MxL_HRCLS_Ctrl_ReqDevCurrentAFEBackOff(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  UINT16 regData = 0;
  
  MXLENTERSTR;
  if (devContextPtr->currentAfeTiltMode == MXL_HRCLS_AFE_FLAT_LNABO_NON0)
  {
    if (MxLWare_HRCLS_OEM_ReadRegister(devContextPtr->devId, MAILBOX_REG_AFE_HOST_BO_CTRL, &regData) != MXL_SUCCESS) regData = 0;
    regData &= 0x1f;
  }
  MXLDBG2(MxL_HRCLS_DEBUG("Current BO=%d\n", regData););
  MXLEXITSTR((UINT8) regData);

  return (UINT8) (regData);
}

#ifdef _MXL_HRCLS_IFOUT_ENABLED_
/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_GetPhysicalIfOutId 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 03/04/2013
--|
--| DESCRIPTION   : This function transalates logical IFOUT id to physical 
--|                 
--| RETURN VALUE  : Physical IFOUT id 
--|                 
--|-------------------------------------------------------------------------------------*/
MXL_HRCLS_IF_ID_E MxL_HRCLS_Ctrl_GetPhysicalIfOutId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_IF_ID_E logicalIfOutId)
{
  MXL_HRCLS_IF_ID_E physicalIfOutId;
  if (devContextPtr->ifOutMap)
  {
    physicalIfOutId = devContextPtr->ifOutMap[(UINT8) logicalIfOutId];
  } else physicalIfOutId = logicalIfOutId;
  return physicalIfOutId;
}
#endif

