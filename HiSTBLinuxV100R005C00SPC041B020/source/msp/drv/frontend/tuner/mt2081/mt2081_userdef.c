/*****************************************************************************
**
**  Name: mt2081_userdef.c
**
**  Description:    User-defined MicroTuner software interface
**
**  Functions
**  Requiring
**  Implementation: MT_WriteRead
**                  MT_Sleep
**
**  References:     None
**
**  Exports:        None
**
**  CVS ID:         $Id: mt_userdef.c,v 1.1 2010/09/06 09:12:57 software Exp $
**  CVS Source:     $Source: /home/cvs/software/tuners/MT2081/mt_userdef.c,v $
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   03-25-2004    DAD    Original
**   N/A   11-03-2009    TP     New generation of functions and types.
**
*****************************************************************************/
#define PLL_STEP 62500
#include "mt2081_userdef.h"
#include "drv_i2c_ext.h"

static HI_U32 s_u32Mt2081TunerPort = 0;
static HI_BOOL s_bInitFlag = HI_FALSE;

/*****************************************************************************
**
**  Name: MT2081_WriteSub
**
**  Description:    Write values to device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  addr       - device serial bus address  (value passed
**                               as parameter to MTxxxx_Open)
**                  subAddress - serial bus sub-address (Register Address)
**                  pData      - pointer to the Data to be written to the 
**                               device 
**                  cnt        - number of bytes/registers to be written
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_COMM_ERR      - Serial bus communications error
**                      user-defined
**
**  Notes:          This is a callback function that is called from the
**                  the tuning algorithm.  You MUST provide code for this
**                  function to write data using the tuner's 2-wire serial 
**                  bus.
**
**                  The hUserData parameter is a user-specific argument.
**                  If additional arguments are needed for the user's
**                  serial bus read/write functions, this argument can be
**                  used to supply the necessary information.
**                  The hUserData parameter is initialized in the tuner's Open
**                  function.
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   03-25-2004    DAD    Original
**
*****************************************************************************/
mt_uint32_t MT2081_WriteSub(mt_user_t   hUserData,
                            mt_uint32_t addr,
                            mt_uint8_t *pData_w,
                            mt_uint32_t cnt_w,
                            mt_uint8_t *pData_r,
                            mt_uint32_t cnt_r)
{
    
        TUNER_I2C_DATA_S stI2cDataStr;
    	HI_U32 u32Ret = HI_SUCCESS;
  
    	if(cnt_w > 1024 ) 
    	{
    		HI_ERR_TUNER("entr_user_2wb_wr : cnt >1024 \n");
    		return MT_COMM_ERR;	
    	}
    		
    	qam_config_i2c_out(s_u32Mt2081TunerPort, 1);/*bypass*/

    	stI2cDataStr.pu8ReceiveBuf = 0;
    	stI2cDataStr.u32ReceiveLength = 0;
    	stI2cDataStr.pu8SendBuf = pData_w;
    	stI2cDataStr.u32SendLength = cnt_w;
    	
		u32Ret = tuner_i2c_send_data(g_stTunerOps[s_u32Mt2081TunerPort].enI2cChannel, \
			                     g_stTunerOps[s_u32Mt2081TunerPort].u32TunerAddress, &stI2cDataStr);

    	if (HI_SUCCESS == u32Ret)
    	{
      
        	return MT_OK;
    	}
    	else
    	{
    		HI_ERR_TUNER("LINE:%d, status:0x%x, devaddr:0x%x, len:%d\n",__LINE__,u32Ret,addr,cnt_w);
        	return MT_COMM_ERR;
    	}
}

/*****************************************************************************
**
**  Name: MT2081_ReadSub
**
**  Description:    Read values from device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  addr       - device serial bus address  (value passed
**                               as parameter to MTxxxx_Open)
**                  subAddress - serial bus sub-address (Register Address)
**                  pData      - pointer to the Data to be written to the 
**                               device 
**                  cnt        - number of bytes/registers to be written
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_COMM_ERR      - Serial bus communications error
**                      user-defined
**
**  Notes:          This is a callback function that is called from the
**                  the tuning algorithm.  You MUST provide code for this
**                  function to read data using the tuner's 2-wire serial 
**                  bus.
**
**                  The hUserData parameter is a user-specific argument.
**                  If additional arguments are needed for the user's
**                  serial bus read/write functions, this argument can be
**                  used to supply the necessary information.
**                  The hUserData parameter is initialized in the tuner's Open
**                  function.
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   03-25-2004    DAD    Original
**
*****************************************************************************/
mt_uint32_t MT2081_ReadSub(mt_user_t hUserData, 
    				   mt_uint32_t addr,  
    				   mt_uint8_t *pData_w,
    				   mt_uint32_t cnt_w,
    				   mt_uint8_t *pData_r,
    				   mt_uint32_t cnt_r)
{
    	HI_U32 u32Ret = HI_SUCCESS;
		HI_U32 u32RegAddr = 0;
		mt_uint8_t *u8RegAddr = pData_w;  	

    	qam_config_i2c_out(s_u32Mt2081TunerPort, 1); /*bypass*/
          
    	if (HI_NULL == pData_r)
    	{
    	    /*HI_TRACE(HI_LOG_LEVEL_DEBUG, "pointer(pData) is null\n");*/
    	    HI_ERR_TUNER("<0>pointer(pData) is null\n");
            return MT_COMM_ERR;
    	}
    	
		u32RegAddr = *u8RegAddr;
		++u8RegAddr;
		u32RegAddr = ((u32RegAddr  << 8) & 0xff00) | (*u8RegAddr);
		
    	/*u32Ret = HI_DRV_I2C_Read(g_stTunerOps[s_u32Mt2081TunerPort].enI2cChannel, \
			        g_stTunerOps[s_u32Mt2081TunerPort].u32TunerAddress, u32RegAddr, cnt_w, pData_r, cnt_r);*/
	    u32Ret = s_tuner_pI2cFunc->pfnI2cRead(g_stTunerOps[s_u32Mt2081TunerPort].enI2cChannel, \
			        g_stTunerOps[s_u32Mt2081TunerPort].u32TunerAddress, u32RegAddr, cnt_w, pData_r, cnt_r);
    
    	if (HI_SUCCESS == u32Ret)
    	{
            return MT_OK;
    	}
    	else
    	{
    		HI_ERR_TUNER("LINE:%d, status:0x%x, devaddr:0x%x, len:%d\n",__LINE__,u32Ret,addr,cnt_r);
            return MT_COMM_ERR;
    	}
}

/*****************************************************************************
**
**  Name: MT_WriteRead
**
**  Description:    Write values to device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  addr       - device serial bus address  (value passed
**                               as parameter to MTxxxx_Open)
**                  pData_w    - pointer to the Data to be written to the
**                               device
**                  cnt_w      - number of bytes to be written
**                  pData_r    - pointer to the Data to be read from the
**                               device
**                  cnt_r      - number of bytes to be read
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_COMM_ERR      - Serial bus communications error
**                      user-defined
**
**  Notes:          This is a callback function that is called from the
**                  the tuning algorithm.  You MUST provide code for this
**                  function to write data using the tuner's 2-wire serial
**                  bus.
**
**                  The hUserData parameter is a user-specific argument.
**                  If additional arguments are needed for the user's
**                  serial bus read/write functions, this argument can be
**                  used to supply the necessary information.
**                  The hUserData parameter is initialized in the tuner's Open
**                  function.
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   03-25-2004    DAD    Original
**   N/A   11-03-2009    TP     New generation of functions and types
**
*****************************************************************************/
mt_uint32_t MT_WriteRead(mt_user_t   hUserData,
                         mt_uint32_t addr,
                         mt_uint8_t *pData_w,
                         mt_uint32_t cnt_w,
                         mt_uint8_t *pData_r,
                         mt_uint32_t cnt_r)
{
    mt_uint32_t u32Ret = MT_OK;                  /* Status to be returned        */
    /*
    **  ToDo:  Add code here to implement a serial-bus write
    **         operation to the MTxxxx tuner.  If successful,
    **         return MT_OK.
    */
	if(0 == cnt_r)
	{
		u32Ret = MT2081_WriteSub(hUserData, addr, pData_w, cnt_w, pData_r, cnt_r);
	}
	else
	{
		u32Ret = MT2081_ReadSub(hUserData, addr, pData_w, cnt_w, pData_r, cnt_r);
	}
	
    return u32Ret;
}


/*****************************************************************************
**
**  Name: MT_Sleep
**
**  Description:    Delay execution for "nMinDelayTime" milliseconds
**
**  Parameters:     hUserData     - User-specific I/O parameter that was
**                                  passed to tuner's Open function.
**                  nMinDelayTime - Delay time in milliseconds
**
**  Returns:        None.
**
**  Notes:          This is a callback function that is called from the
**                  the tuning algorithm.  You MUST provide code that
**                  blocks execution for the specified period of time.
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   03-25-2004    DAD    Original
**
*****************************************************************************/
void MT2081_Sleep(mt_user_t   hUserData,
              mt_uint32_t nMinDelayTime)
{
    /*
    **  ToDo:  Add code here to implement a OS blocking
    **         for a period of "nMinDelayTime" milliseconds.
    **         (also know as busywait)
    */
}

/*
** Definitions for a systems-wide multituner spuravoidance
*/
#if (defined(MT2082_CNT) && (MT2082_CNT > 0)) || (defined(MT2084_CNT) && (MT2084_CNT > 0))
static struct spuravoid_entry *head = NULL;

void spuravoid_register_entry(struct spuravoid_entry *entry) {
    entry->next = head;
    head = entry;
}

struct spuravoid_entry *spuravoid_get_list_head() {
    return head;
}
#endif

static mt_handle_t hMT2081;
HI_S32 mt2081_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF)
{
	mt_uint32_t u32FreqPll= u32PuRF * 1000;
	mt_uint32_t u32Ret;

	s_u32Mt2081TunerPort = u32TunerPort;
	
	if(!s_bInitFlag)
	{
		/* Get a handle to the MT2081 */
		u32Ret = MT2081_Open(g_stTunerOps[u32TunerPort].u32TunerAddress, &hMT2081, HI_NULL);
		if(MT_OK == u32Ret)
		{
			HI_INFO_TUNER("MT2081 is init\n");
		}
		else
		{
			HI_ERR_TUNER("ERROR init, status = %x\n",u32Ret);
			return HI_FAILURE;
		}
		
		/* Perform tuner driver overrides here */
		u32Ret |= MT2081_SetParam(hMT2081, MT2081_OUTPUT_BW, 8000000);
		
		/* Set up for cable digital reception */
		u32Ret |= MT2081_SetParam(hMT2081, MT2081_RCVR_MODE,MT2081_CABLE_QAM);

		/* Perform tuner driver overrides here: Output 36.15 MHz */
		u32Ret |= MT2081_SetParam(hMT2081, MT2081_OUTPUT_FREQ, 36150000);

            u32Ret |= MT2081_SetParam(hMT2081, MT2081_RFLT_ENABLE, 1);
		
		s_bInitFlag = HI_TRUE;
	}

	if(4 == g_stTunerOps[u32TunerPort].u32CurrQamMode)
	{
        u32FreqPll += PLL_STEP * 2;
    }
    else if(2 == g_stTunerOps[u32TunerPort].u32CurrQamMode)
    {
       	u32FreqPll += PLL_STEP * 1 / 2;
    }
    else
    {
    	u32FreqPll += PLL_STEP * 1 / 2;
    }
	
	u32Ret = MT2081_Tune(hMT2081,u32FreqPll);
	
	if(MT_OK == (u32Ret >> 30))
	{
		HI_INFO_TUNER("MT2081 is locking, freq: %dkhz\n",u32PuRF);
		return HI_SUCCESS;
	}
	else
	{
		HI_ERR_TUNER("ERROR setting the channel, status = %d\n",u32Ret);
		return HI_FAILURE;
	}
	
}

HI_S32 mt2081_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    mt_uint32_t u32Ret = MT_OK;
    
    u32Ret = MT2081_GetParam(hMT2081, MT2081_RSSI, &pu32SignalStrength[1]);
    
    if(MT_OK != u32Ret)
    {
        HI_ERR_TUNER("ERROR getting signal strength, status = %d\n",u32Ret);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_VOID mt2081_tuner_resume(HI_U32 u32TunerPort)
{
	s_bInitFlag = HI_FALSE;
	
	return;
}

