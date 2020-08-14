/*************************************************************************
* si_drv_scaler.c
*************************************************************************/
//#define SII_DEBUG

//#include <math.h>

#include "si_datatypes.h"
#include "si_drv_tpg_api.h"
#include "si_lib_obj_api.h"
#include "si_lib_seq_api.h"
#include "si_drv_cra_api.h"
#include "si_lib_video_api.h"

SII_LIB_OBJ_MODULE_DEF(si_drv_vtg);

/************************************************************************/
/*      Constants Definitions                                           */
/************************************************************************/

/************************************************************************/
/*      Type Definitions                                                */
/************************************************************************/
typedef struct
{
	SiiLibVideoTiming_t    inpTiming;
	SiiLibVideoTiming_t    outTiming;
	uint16_t               hSqueeze;
	uint16_t               vSqueeze;
	bool_t                 bBypass;
	bool_t                 bForceLock;
	SiiInst_t              seqInst;

	SiiInst_t			   instTxCra;
} VtgObj_t;

static void sUpdateHandler( SiiInst_t parentInst );
//static void sUpdateTiming( void );
//static void sUpdateBypassMode( void );
static void sUpdateScaler( void );

static VtgObj_t* pObj = NULL;

/************************************************************************/
/*      Application Interface                                           */
/************************************************************************/
SiiInst_t SiiDrvVtgCreate( SiiInst_t inst )
{
	/* Enforce module to be used as singleton only */
	SII_PLATFORM_DEBUG_ASSERT(!pObj);

	/* For first instantiation only */
	if( SII_LIB_OBJ_ZEROINST() )
	{
#ifdef SII_ENV_BUILD_UFD
		SiiLibUfdRegisterCmdTable(sUfdCmdTbl);
#endif /* SII_ENV_BUILD_UFD */
	}

	/* Allocate memory for object */
	pObj = (VtgObj_t*)SII_LIB_OBJ_CREATE(NULL, sizeof(VtgObj_t));
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	pObj->instTxCra = inst;

	SiiLibVideoTimingDetailGet(SII_LIB_VIDEO_VM__480P, SII_LIB_VIDEO_MOD3D__NONE, false, &pObj->inpTiming);
	SiiLibVideoTimingDetailGet(SII_LIB_VIDEO_VM__720P60, SII_LIB_VIDEO_MOD3D__NONE, false, &pObj->outTiming);

#define SII_LIB_SEQ_PRIOR__DRV_SCALER 1
	/* Instantiate and start periodic event dispatch handler */
	pObj->seqInst = SII_LIB_SEQ_TIMER_CREATE("Scaler", sUpdateHandler, SII_LIB_OBJ_INST(pObj), SII_LIB_SEQ_PRIOR__DRV_SCALER, SII_TIMER_SCALER);
	SII_PLATFORM_DEBUG_ASSERT(pObj->seqInst);

	return SII_LIB_OBJ_INST(pObj);
}

void SiiDrvVtgDelete( void )
{
	/* Enforce module to be used as singleton only */
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	/* Delete all resources in reverse order of creation */
	SiiLibSeqTimerDelete(pObj->seqInst);

	/* De-allocate instantiation memory */
	SII_LIB_OBJ_DELETE(pObj);

	/* Do for last deleted instantiation only */
	if( SII_LIB_OBJ_ZEROINST() )
	{
#ifdef SII_ENV_BUILD_UFD
		SiiLibUfdUnRegisterCmdTable(sUfdCmdTbl);
#endif /* SII_ENV_BUILD_UFD */
	}

	pObj = NULL;
}

void SiiDrvVtgOutputVideoTimingSet( const SiiLibVideoTiming_t* pTiming )
{
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	pObj->outTiming = *pTiming;
	sUpdateScaler();
}

#if 0
void SiiDrvVtgInputVideoTimingSet( const SiiLibVideoTiming_t* pTiming )
{
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	pObj->inpTiming = *pTiming;
	sUpdateTiming();
}

void SiiDrvVtgBypassSet( bool_t isTrue )
{
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	pObj->bBypass = isTrue;
	sUpdateBypassMode();
}

void SiiDrvVtgForceLockSet( bool_t isTrue )
{
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	pObj->bForceLock = isTrue;
	sUpdateTiming();
}

bool_t SiiDrvVtgForceLockGet( void )
{
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	return pObj->bForceLock;
}


bool_t SiiDrvVtgBypassIs( void )
{
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	return pObj->bBypass;
}
#endif

static void sUpdateHandler( SiiInst_t parentInst )
{
	parentInst = parentInst;
	sUpdateScaler();
}

#if 0
static void sUpdateTiming( void )
{
	SiiLibSeqTimerStart(pObj->seqInst, 10, 0);
}

static void sUpdateBypassMode( void )
{
	//dBOSTONreg_setCMC__CSC1_BypassConfig(pObj->bBypass);
}
#endif

static void sUpdateScaler( void )
{
#define TRIGGER_DELTA         10
#define HORIZONTAL_FRAC_BITS  28
#define VERTICAL_FRAC_BITS    17

	//SiiLibVideoTiming_t* pIVT                  = &pObj->inpTiming;
	SiiLibVideoTiming_t* pOVT                  = &pObj->outTiming;

	uint32_t hsqueeze                          = pObj->hSqueeze;
	uint32_t vsqueeze                          = pObj->vSqueeze;

	//uint32_t input_vtg_sync_line               = 3; /* Minimum value required by 480i input format */
	//uint32_t input_vtg_trigger_delay           = (in_htotal*2+1)/4;

	uint32_t out_hfp                           = pOVT->hFrontPorch;
	uint32_t out_hs                            = pOVT->hSync;
	uint32_t out_hbp                           = pOVT->hBackPorch;
	uint32_t out_hres                          = pOVT->hActive;
	uint32_t out_htotal                        = pOVT->hTotal;

	bool_t 	 out_interlaced                    = pOVT->bInterlaced;
	uint32_t out_vfp                           = (out_interlaced) ? (pOVT->vFrontPorch) : (pOVT->vFrontPorch<<1);
	uint32_t out_vs                            = (out_interlaced) ? (pOVT->vSync)       : (pOVT->vSync<<1);
	uint32_t out_vbp                           = (out_interlaced) ? (pOVT->vBackPorch)  : (pOVT->vBackPorch<<1);
	uint32_t out_vres                          = (out_interlaced) ? (pOVT->vActive)     : (pOVT->vActive<<1);
	//uint32_t out_vtotal                        = (out_interlaced) ? (pOVT->vTotal)      : (pOVT->vTotal<<1);
	uint32_t trigger_delta                     = (out_interlaced) ? (TRIGGER_DELTA)     : (TRIGGER_DELTA<<1);

	uint32_t out_hres_adj                      = out_hres - hsqueeze;
	uint32_t out_hoff                          = (hsqueeze>>2)<<1;
	uint32_t out_vres_adj                      = out_vres - vsqueeze;
	uint32_t out_voff                          = (vsqueeze>>2)<<1;

	uint32_t out_trigger_start                 = out_vs + out_vbp - trigger_delta - 1;

	// Calculate VTG settings
	uint32_t out_horizontal_sync_end           = out_hs - 1;
	uint32_t out_horizontal_active_video_start = out_hs + out_hbp - 1;
	uint32_t out_horizontal_active_video_end   = out_hs + out_hbp + out_hres - 1;
	uint32_t out_horizontal_end_of_line        = out_hs + out_hbp + out_hres + out_hfp - 2;
	uint32_t out_halfline                      = ((out_htotal*2+1)/4) - 2;

	uint32_t out_vertical_sync_end             = out_vs - 1;
	uint32_t out_vertical_active_video_start   = out_vs + out_vbp - 1;
	uint32_t out_vertical_active_video_end     = out_vs + out_vbp + out_vres - 1;
	uint32_t out_vertical_end_of_frame         = out_vs + out_vbp + out_vres + out_vfp - 1;

	//uint32_t left_mask_start                   = out_horizontal_active_video_start - 2;
	//uint32_t left_border_start                 = out_horizontal_active_video_start - 1;
	uint32_t left_border_end                   = out_horizontal_active_video_start + out_hoff;

	//uint32_t right_border_start                = left_border_end + out_hres_adj - 2;
	//uint32_t right_border_end                  = out_horizontal_active_video_end - 1;
	//uint32_t right_mask_end                    = out_horizontal_active_video_end;

	//uint32_t top_mask_start                    = out_vertical_active_video_start - 4;
	//uint32_t top_border_start                  = out_vertical_active_video_start - 2;
	uint32_t top_border_end                    = out_vertical_active_video_start + out_voff;

	//uint32_t bottom_border_start               = top_border_end + out_vres_adj - 4;
//	uint32_t bottom_border_end                 = out_vertical_active_video_end - 2;
//	uint32_t bottom_mask_end                   = out_vertical_active_video_end;

	out_horizontal_active_video_start = left_border_end;
	out_horizontal_active_video_end   = out_horizontal_active_video_start + out_hres_adj;
	//    out_halfline                      = out_horizontal_active_video_start + ((out_hres_adj+1)>>1);

	out_vertical_active_video_start = top_border_end;
	out_vertical_active_video_end   = out_vertical_active_video_start + out_vres_adj;

	/* Check/adjust halfline */
	if( out_halfline > (out_horizontal_active_video_end - 3) )
	{
		SII_PLATFORM_DEBUG_ASSERT(!out_interlaced); /* Format not supported (halfline value too large) */
		out_halfline = out_horizontal_active_video_end - 3;
	}
	else if( out_halfline < (out_horizontal_active_video_start + 3) )
	{
		SII_PLATFORM_DEBUG_ASSERT(!out_interlaced); /* Format not supported (halfline value too small) */
		out_halfline = out_horizontal_active_video_start + 3;
	}   

	SII_PRINTF("\nout_hres: %d, out_vres: %d\n", (int)out_hres, (int)out_vres);
#if 0
	//should replace with the PLL configuration.
	// Generate 74.25Mhz using Pll
	SiiDrvCraWrReg8( 0xb8, 0x01); // source for Pll0 is RX0 ref
	SiiDrvCraWrReg8( 0xb1, 0x00); // NR-1 divider
	SiiDrvCraWrReg16( 0xb2, 0x02b); // NF -1 multiplier
	SiiDrvCraWrReg8( 0xb4, 0x0f); // OD -1 post divide
	SiiDrvCraWrReg16( 0xb6, 0x2b); // NB -1
	SiiDrvCraWrReg8( 0xb9, 0x00);  // no post divider
	SiiDrvCraWrReg8( 0xba, 0x01);  // update request
#endif

	
	//vtg settings
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0820, (uint16_t)out_horizontal_sync_end);	            // vtg end of hs pulse-1	
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0822,(uint16_t)out_horizontal_active_video_start);	// vtg horizontal active line start-1
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0824, (uint16_t)out_halfline);	                        // vtg half line-2	
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0826, (uint16_t)out_horizontal_active_video_end);	    // vtg horizontal active line end-1
					// pObj->instTxCra, 
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0828, (uint16_t)out_horizontal_end_of_line);	        // vtg end of horizontal line-2
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0830, (uint16_t)out_vertical_sync_end);	            // vtg end of vs pulse (in half lines)-1
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0832, (uint16_t)out_trigger_start);	                // vtg sc trigger point (in half lines)-1
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0834, (uint16_t)out_vertical_active_video_start);	    // vtg vertical active start (in half lines)-1
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0836, (uint16_t)out_vertical_active_video_end);	    // vtg vertical active end (in half lines)-1
	SiiDrvCraWrReg16(pObj->instTxCra, 0x0838, (uint16_t)out_vertical_end_of_frame);	        // vtg end of frame (in half lines)-1

	SiiDrvCraWrReg8(pObj->instTxCra, 0x083e, 0x01);	// vtg update request
	SiiDrvCraWrReg8(pObj->instTxCra, 0x09fc, 0x10);	// force vtg unlock mode, need to force unlock to use Pll
					

	//===== enable tpg =====
	SiiDrvCraWrReg8(pObj->instTxCra,  0x810,0x01);		//enable vidgen using VTG timing
}

/** END of File *********************************************************/
