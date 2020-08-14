/*****************Copyright (c) 2010 Hisilicon Tech Co., Ltd.********************** 
    project: wish
    subprj:  device driver
    module:  audio codec
    file:    slac89116.c
    abstract:Control driver file of FXS interface slac chip
*******************************************************************************/
//#include <stdio.h>
//#include <stdlib.h>
#include <mach/hardware.h>
#include "vp_api.h"
#include "vp_api_common.h"
#include "sys_service.h"
#include "profiles8.h"

#include "slac_private.h"
#include "slac_public.h"

#include "hi_drv_sys.h"
#include "hi_module.h"
#include "hi_debug.h"

/*
 * Application memory space for the API required Device/Line contexts and
 * objects.  This application uses the simplest memory scheme -- global, but the
 * API will work with any memory management scheme.
 */
VpDevCtxType devCtx;
VpLineCtxType lineCtx;
Vp890DeviceObjectType devObj;
Vp890LineObjectType lineObj;

/*
 * Use this to hold the state of the line so we can toggle Standby/Ringing
 * between each On-Hook state.
 */
unsigned char fxsOnHook = FALSE;
VpEventType pSlacEvent;
/* BEGIN: Modified for PN: When SLIC initialization, sound is abnormal by f00172091, 2011/1/30 */
extern  void le89116_reg_read(unsigned char addr, unsigned char *val, unsigned char len);
/* END:   Modified for PN: When SLIC initialization, sound is abnormal by f00172091, 2011/1/30 */
extern unsigned char wake_handle_type;
/*
 * Use this to hold the state of the line so we can toggle Standby/Ringing
 * between each On-Hook state on the Dial-A-Tone line
 */
bool datRingNext = TRUE;



HI_S32 spi_init(void)
{
    HI_S32 s32Ret;
#ifndef VP890_USE_SPI
    SLIC_SCS_OUT();
    SLIC_SCS_SET();
    SLIC_SDO_OUT();
    SLIC_SDO_SET();
    SLIC_SCK_OUT();
    SLIC_SCK_SET();
    SLIC_SDI_IN();
#else

#if 0
    hi_ssp_reset(0);
    hi_ssp_clock(1);
    hi_ssp_disable();
    hi_ssp_set_frameform(0,1,1,8);//spi,SPH 1,SPO 1,bytewidth:8
    hi_ssp_set_serialclock(4,10);///TODO:USE 2Mhz CLOCK,you may need to modify it
    hi_ssp_set_bigend(true);
    hi_ssp_set_inturrupt(0);
    hi_ssp_dmac_disable();
    hi_ssp_interrupt_clear();   
#else
    s32Ret = HI_DRV_SPI_Open(SPIDEVNUM);
    if (s32Ret != HI_SUCCESS)
    {
        //printk("Open SPI device(%d)failed :%d.\n", SPIDEVNUM, s32Ret);
        return s32Ret;
    }
	//for use spi cs gpio
	//HI_DRV_SPI_SetCfg(HI_TRUE);
	//HI_DRV_SPI_SetCs_Gpio(SPIDEVNUM);
	//HI_DRV_SPI_SetCs_Level(SPIDEVNUM, 1);

   /* Motorola¡¯s standard SPI mode 0 */
    s32Ret = HI_DRV_SPI_SetFrom(SPIDEVNUM, 0, 1, 1, 8);
    if (s32Ret != HI_SUCCESS)
    {
        //printk("Set SPI from (%d)failed :%d.\n", SPIDEVNUM, s32Ret);
        return s32Ret;
    }
    /* big/little end, 0: little, 1: big */
    HI_DRV_SPI_SetBlend(SPIDEVNUM, 0);		



//just for debug spi (mclk should be 2M, bclk 1M, fs 8k) 
#if 0
	int cnt;
	int cnt2;
	unsigned char val = 0;
	unsigned char val2[2];
	val2[0] = 0xaa;
	val2[1] = 0xcc;

	const unsigned char  testarray[] =
	{
		74, 1, 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,4,
		74,1,70, 131, 
		74,1,108,127,255,
		74,1,79
	};
#if 0
	for(cnt = 0; cnt < 31; cnt++)
	{
	   	//printk("Write SPI device %d \n", testarray[cnt]);
		s32Ret = HI_DRV_SPI_WriteQuery(SPIDEVNUM, &testarray[cnt], 1);
	 	if(HI_SUCCESS  != s32Ret)
	 	{
	       	 printk("Test  Write SPI  failed :%d.\n", s32Ret);
	 	}
		udelay(4);
	}
/*
	for(cnt = 0; cnt < 2; cnt++)
	{
		s32Ret = HI_DRV_SPI_ReadQuery(SPIDEVNUM, &val, 1);
	 	if(HI_SUCCESS  != s32Ret)
	 	{
	       	 printk("Read SPI device(%d)failed :%d.\n", SPIDEVNUM, s32Ret);
	 	}
		udelay(4);
	   	 printk("Read SPI device %d \n", val);
	}
*/
    s32Ret = HI_DRV_SPI_ReadQuery(SPIDEVNUM, val2, 2);
    if(HI_SUCCESS  != s32Ret)
	{
	   	 printk("Read SPI device(%d)failed :%d.\n", SPIDEVNUM, s32Ret);
	}
	 printk("Read SPI2 device %d \n", val2[0]);
 	 printk("Read SPI2 device %d \n", val2[1]);
#else
	HI_U8 tmp = 0x73;	//read version
	s32Ret = HI_DRV_SPI_WriteQuery(SPIDEVNUM, testarray, 32);
	udelay(4);

	if(HI_SUCCESS  != s32Ret)
	{
	   	 printk("Test  Write SPI  failed :%d.\n", s32Ret);
	}
	s32Ret = HI_DRV_SPI_ReadQuery(SPIDEVNUM, val2, 2);
	if(HI_SUCCESS  != s32Ret)
	{
        printk("Read SPI device(%d)failed :%d.\n", SPIDEVNUM, s32Ret);
	}
	printk("Read SPI device %d \n", val2[0]);	//should be 02 00
 	printk("Read SPI device %d \n", val2[1]);
#endif
	return  -1; 
#endif
    return s32Ret;

#endif
#endif
}

HI_S32 spi_deinit(void)
{
    HI_S32 s32Ret;
#ifndef VP890_USE_SPI
    //TODO
#else
    s32Ret = HI_DRV_SPI_Close(SPIDEVNUM);
    if (s32Ret != HI_SUCCESS)
    {
        //printk("Close SPI device(%d)failed :%d.\n", SPIDEVNUM, s32Ret);
        return s32Ret;
    }
    return s32Ret;

#endif
}

void le89116_hardware_reset(void)
{
#ifdef GPIO_SLIC_RESET
    SLIC_RESET_OUT(); 
    SLIC_RESET_SET();
    mdelay(5);
    SLIC_RESET_CLR(); 
    mdelay(10);
    SLIC_RESET_SET();
#endif
}
void le89116_software_reset(void)
{
//    unsigned char data;
/*
*   le89116_reg_write(unsigned char addr, unsigned char * val, unsigned char len);
*/
    mdelay(50);
}  
#if 0
void irq_io_init(void)
{
    SLAC_IOINT_DISABLE();
    SLAC_IOINT_CLEAR();
    SLAC_IOINT_EDGE();
    SLAC_IOINT_SINGLE_EDGE();
    SLAC_IOINT_LOW();
    SLAC_IOINT_ENABLE();
}
#endif
#if 0
/**
 * Function:  InitQuickStartSettings()
 *
 * Description:  Function used to initialize the device and lines of the device
 * to settings not provided by default in the API.  This function should be the
 * first function called after a device init complete event occurs.
 */
void
InitQuickStartSettings(
    VpEventType *pEvent)
{
    uint16 handle = 0;

    /*
     * For this demo, we want the API to detect the dial pulses on the FXS
     * line, then initialize the ringing cadence.
     */
    VpOptionPulseModeType pulseMode = VP_OPTION_PULSE_DECODE_ON;
    VpSetOption(VP_NULL, &devCtx, VP_OPTION_ID_PULSE_MODE, &pulseMode);

    VpInitRing(&lineCtx, RING_CAD_STD, VP_PTABLE_NULL);
    fxsOnHook = FALSE;
    VpSetLineState(&lineCtx, VP_LINE_STANDBY);

    /*
     * Read the event mask contents so we can unmask only the FXS signaling
     * events.
     */
//    printk("InitQuickStartSettings:VpGetOption's status = 0x%x. \n",VpGetOption(VP_NULL, &devCtx, VP_OPTION_ID_EVENT_MASK, handle));
    return;
}

/**
 * Function:  InitReadOptions()
 *
 * Description:  Function used to initialize the device event masks to only
 * allow those used by this application.
 */
void
InitReadOptions(
    VpEventType *pEvent)
{

    VpOptionEventMaskType eventMask;

    VpGetResults(pEvent, &eventMask);

    eventMask.signaling &= ~(VP_LINE_EVID_HOOK_OFF | VP_LINE_EVID_HOOK_ON);
    eventMask.signaling &= ~(VP_LINE_EVID_FLASH | VP_LINE_EVID_STARTPULSE);
    eventMask.signaling &= ~(VP_LINE_EVID_PULSE_DIG);

    VpSetOption(VP_NULL, &devCtx, VP_OPTION_ID_EVENT_MASK, &eventMask);
}
#endif
VpStatusType
PrepareLine(
	    VpLineCtxType *pLctx/*, int ChanId*/)
{
    VpStatusType status = VP_STATUS_SUCCESS;
    VpOptionTimeslotType timeSlots;
    VpOptionCodecType CodeType = VP_OPTION_LINEAR;
    VpOptionPcmTxRxCntrlType PcmMode = VP_OPTION_PCM_BOTH;
    VpOptionPulseModeType pulseMode = VP_OPTION_PULSE_DECODE_OFF;
    timeSlots.tx = 0;
    timeSlots.rx = 2;

    /* put the line into standby */
    status = VpSetLineState(pLctx, VP_LINE_STANDBY);
    if (status != VP_STATUS_SUCCESS) {
	printk("ERROR: unable to set line state to standby %i\n", status);
	return status;
    }


    /* Put the line into active and set up timeslots for linear mode. */    
    status = VpSetOption(pLctx, VP_NULL, VP_OPTION_ID_TIMESLOT, &timeSlots);
    if (status != VP_STATUS_SUCCESS) {
	printk("ERROR: unable to set time slots %i\n", status);
	return status;
    }

    /* Set the code mode for linear mode */
    status = VpSetOption(pLctx, VP_NULL, VP_OPTION_ID_CODEC, &CodeType);
    if (status != VP_STATUS_SUCCESS) {
	printk("ERROR: unable to set the codec for linear mode %i\n", status);
	return status;
    }

    /* enable PCM TXRX pathways */
    status =  VpSetOption(pLctx, VP_NULL, VP_OPTION_ID_PCM_TXRX_CNTRL, 
	    &PcmMode);
    if (status != VP_STATUS_SUCCESS) {
	printk("ERROR: unable to set option pcm txrx ctrl %i\n", status);
	return status;
    }

    /* enable PULSE DECODE */
    status = VpSetOption(pLctx, VP_NULL, VP_OPTION_ID_PULSE_MODE, &pulseMode);
    if (status != VP_STATUS_SUCCESS) {
	printk("ERROR: unable to set option pulse mode %i\n", status);
	return status;
    }
#if 0
    /* Init the ringing profile */
    //status =  VpInitRing(pLctx, VP_PTABLE_NULL, VP_PTABLE_NULL);
    status =  VpInitRing(pLctx, RING_CAD_STD, VP_PTABLE_NULL);
    if (status != VP_STATUS_SUCCESS) {
	printk("ERROR: unable to init ring %i\n", status);
	return status;
    }
#endif
    /* Init the ringing profile , Sinclair:CID */
    //status =  VpInitRing(pLctx, VP_PTABLE_NULL, VP_PTABLE_NULL);
    status =  VpInitRing(pLctx, RING_CAD_STD, CLI_TYPE1_US);
    if (status != VP_STATUS_SUCCESS) {
	printk("ERROR: unable to init ring %i\n", status);
	return status;
    }

    /* Calibrate the line */
    status = VpCalLine(pLctx);
    if (status != VP_STATUS_SUCCESS) {
	printk("ERROR: unable to cal line %i\n", status);
	return status;
    }

    return status;
}
void HandleDat(void)
{
    bool deviceEventStatus = FALSE;

    /*
     * This loop will query the FXS device for events, and when an event is
     * found (deviceEventStatus = TRUE), it will parse the event and perform
     * further operations.
     */
    VpApiTick(&devCtx, &deviceEventStatus);
    if(deviceEventStatus == TRUE) {
        while(VpGetEvent(&devCtx, &pSlacEvent)) {
            /*
             * If the event was a Response:Device Init Complete event,
             * we need to initialize the device and associated lines, so
             * send the event structure (pointer) to the Init function for
             * handling.
             */
//            printk("Vp event : Status %d, Category %d, eventId 0x%x\n",pSlacEvent.status,pSlacEvent.eventCategory,pSlacEvent.eventId); 
            if (pSlacEvent.eventCategory == VP_EVCAT_RESPONSE) 
            {
                if(pSlacEvent.eventId == VP_DEV_EVID_DEV_INIT_CMP) 
                {
                    VpStatusType status;
			    	VpOptionEventMaskType eventMask;
			    	
			    	eventMask.faults = VP_EVENT_MASK_ALL;
			    	eventMask.signaling = ~(VP_LINE_EVID_HOOK_OFF | VP_LINE_EVID_HOOK_ON );
			    	eventMask.signaling &= ~(VP_LINE_EVID_FLASH | VP_LINE_EVID_STARTPULSE);
                    eventMask.signaling &= ~(VP_LINE_EVID_PULSE_DIG);
				    eventMask.response = (VP_EVCAT_RESPONSE_UNMASK_ALL & ~VP_EVID_CAL_CMP);
				    eventMask.test = VP_EVENT_MASK_ALL;;
				    eventMask.process = VP_EVENT_MASK_ALL;
				    eventMask.fxo = VP_EVENT_MASK_ALL;
				    eventMask.packet = VP_EVENT_MASK_ALL;
              	  	status = VpSetOption(VP_NULL, &devCtx, VP_OPTION_ID_EVENT_MASK, &eventMask);
              	  	
				    if (status != VP_STATUS_SUCCESS) 
				    {
					    printk("ERROR: Unable to set event mask %i\n", status);
				    }
                	status = PrepareLine(&lineCtx);	
                }else if (pSlacEvent.eventId == VP_EVID_CAL_CMP) {
                    printk("Calibration complete\n\r");
                }else{
                    	/* Do nothing */
                }
                /* We're initializing if a response event occurred. */
                return;
            }
            wake_handle_type = DAT_WAKEUP;
//            printk("k00144455: event is present;\n");
            return;
        }
    }
    return;
}



/* BEGIN: Modified for PN: When SLIC initialization, sound is abnormal by f00172091, 2011/1/30 */
VpStatusType hisi_slac_init(void)
/* END:   Modified for PN: When SLIC initialization, sound is abnormal by f00172091, 2011/1/30 */
{
    VpStatusType status;
	HI_S32 s32Ret;
#if 0
	HI_CHIP_TYPE_E enChipType;
	HI_CHIP_VERSION_E u32ChipVersion;
	SYS_GetChipVersion(&enChipType,&u32ChipVersion);
	if(HI_CHIP_TYPE_HI3716L == enChipType)
	{
        printk("%s %d, ChipType %x, ChipVersion 0x%x, not support slac.\n",__FUNCTION__,__LINE__,enChipType,u32ChipVersion);
		return VP_STATUS_FUNC_NOT_SUPPORTED;    
	}
	else if(HI_CHIP_TYPE_HI3716M == enChipType)
	{
        if((HI_CHIP_VERSION_V100 == u32ChipVersion)||(HI_CHIP_VERSION_V101 == u32ChipVersion))
        {
            printk("%s %d, ChipType %x, ChipVersion 0x%x, not support slac.\n",__FUNCTION__,__LINE__,enChipType,u32ChipVersion);
            return VP_STATUS_FUNC_NOT_SUPPORTED;    
        }
	}
#endif
    //MODULE_SET();
    //le89116_hardware_reset();
    s32Ret = spi_init();
	if(HI_SUCCESS != s32Ret)
		return s32Ret;
    
//    irq_io_init();
    
    /*
     * Simple loop to initialize the device/line contexts and objects for the
     * API as well as initialize the Device and Line.  When this loop is
     * complete, the 89316 device and its FXS line will be initialized.
     */
    VpMakeDeviceObject(VP_DEV_890_SERIES, MPI_3WIRE, &devCtx, &devObj);
    VpMakeLineObject(VP_TERM_FXS_GENERIC, 0, &lineCtx, &lineObj, &devCtx);

    /* Initialize the Device */
    status = VpInitDevice(&devCtx, DEV_PROFILE, AC_FXS_RF50_600, DC_FXS_REGION1,
        RING_20HZ_SINE, VP_PTABLE_NULL, VP_PTABLE_NULL);

    if (status != VP_STATUS_SUCCESS) {
        /* Try MPI 4-Wire Mode if MPI 3-Wire Failed */
        VpMakeDeviceObject(VP_DEV_890_SERIES, MPI_4WIRE, &devCtx, &devObj);
        VpMakeLineObject(VP_TERM_FXS_GENERIC, 0, &lineCtx, &lineObj, &devCtx);

        /* Re-attempt to initialize the Device */
        status = VpInitDevice(&devCtx, DEV_PROFILE, AC_FXS_RF50_600,
            DC_FXS_REGION1, RING_20HZ_SINE, VP_PTABLE_NULL, VP_PTABLE_NULL);

        if (status != VP_STATUS_SUCCESS) {
            printk("\n\r Le89116 Device Did Not Init Properly\t%d\n\r", status);
        } else {
            printk("\n\r Le89116 Device Initialized in MPI 4-Wire Mode\n\r");
        }
    } else 
    {
/* BEGIN: Modified for PN: When SLIC initialization, sound is abnormal by f00172091, 2011/1/30 */
/*
        uint8 data;
        le89116_reg_read(0x41, &data, 1);
        printk("SLIC 0x41 REG is 0x%x\n",data);

        le89116_reg_read(0x43, &data, 1);
        printk("SLIC 0x43 REG is 0x%x\n",data);
        
        le89116_reg_read(0x61, &data, 1);
        printk("SLIC 0x61 REG is 0x%x\n",data);

        le89116_reg_read(0x45, &data, 1);
        printk("SLIC 0x45 REG is 0x%x\n",data);

        le89116_reg_read(0x4b, &data, 1);
        printk("SLIC 0x4b REG is 0x%x\n",data);
*/        
/* END:   Modified for PN: When SLIC initialization, sound is abnormal by f00172091, 2011/1/30 */
        printk("\n\r Le89116 Device Initialized in MPI 3-Wire Mode\n\r");
    }
    /* BEGIN: Modified for PN: When SLIC initialization, sound is abnormal by f00172091, 2011/1/30 */
    return status;
    /* END:   Modified for PN: When SLIC initialization, sound is abnormal by f00172091, 2011/1/30 */
}

void hisi_slac_deinit(void)
{
	HI_S32 s32Ret;

	s32Ret = spi_deinit();
	if(HI_SUCCESS != s32Ret)
	{
        //printk("hisi_slac_deinit device(%d)failed :%d.\n", SPIDEVNUM, s32Ret);
	}
}
