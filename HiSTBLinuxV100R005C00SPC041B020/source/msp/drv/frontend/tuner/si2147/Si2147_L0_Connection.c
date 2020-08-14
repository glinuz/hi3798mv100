/*************************************************************************************************************/
/*                                  Silicon Laboratories                                                     */
/*                                  Broadcast Video Group                                                    */
/*                     Layer 0      Communication Functions                                                  */
/*-----------------------------------------------------------------------------------------------------------*/
/*   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF                           */
/*     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING                                */
/*     TO THIS SOURCE FILE.                                                                                  */
/*  IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,                             */
/*    PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.                                                */
/*-----------------------------------------------------------------------------------------------------------*/
/*   This source code contains all i2c functions to communicate with i2c components                          */
/*     All functions are declared in Si2147_L0_API.h                                                         */
/*************************************************************************************************************/
/* Change log:

 As from 3.3.9:
  In strcmp_nocase_n: using "%s" in sprintf (greater Linux compatibility)

 As from 3.3.8:
  Declaring parameter strings as 'const char*' instead of 'char*' to limit -Wwrite-strings warnings

  As from 3.3.7:
  Adding L0_SlowI2C and L0_FastI2C for easier control of the i2c speed from the above layers

 As from 3.3.6:
  Improved porting comments in L0_ReadBytes

 As from 3.3.5:
  casting calls to strlen as (int) for greater compatibility with VisualStudio

 As from 3.3.4:
  Added help for 'resume' / "suspend'

 As from 3.3.3:
  Adding 'suspend' / 'resume' in traces to keep control of the traces destination at application level.
  This is required to avoid having the traces routed to file after each FW download with 'command mode' parts.

 As from 3.3.2:
 Correction on last character when changing traces file name
 Correction in traces file name management
 *************************************************************************************************************/
 
#include <linux/file.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/param.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/time.h>
//#include <common_mem.h>
#include <asm/io.h> 

#include <linux/delay.h>
//#include <linux/semaphore.h>
#include "linux/kernel.h"
#include <hi_debug.h>
#include "Si2147_L0_API.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "drv_i2c_ext.h"
#include "hi_debug.h"
#include "hi_type.h"


L0_Context  rawI2C_context;
static L0_Context *rawI2C;

#define  SIMULATOR_SCOPE 0xffff
static int      Byte[SIMULATOR_SCOPE];

static HI_U32 s_u32Si2147TunerPort = 0 ;
static HI_U32 s_u32Si2147I2cChannel = 5;
//static HI_U32 s_u32Si2147Init = 0;



//static unsigned char  registerDataBuffer[8];
static unsigned char *pucAddressBuffer;
static unsigned char *pucBuffer;
static unsigned char  ucAddressBuffer[4];
static unsigned char  ucBuffer[1000];
//static unsigned char  cypress_checks_done    = 0;
static char           L0_error_buffer_0[ERROR_MESSAGE_MAX_LENGH];
static char           L0_error_buffer_1[ERROR_MESSAGE_MAX_LENGH];
static char          *stored_error_message;
char          *L0_error_message;

#ifdef    __cplusplus
extern "C" {
#endif /* __cplusplus */








  

/************************************************************************************************************************
  system_wait function
  Use:        current system wait function
              Used to wait for time_ms milliseconds while doing nothing
  Parameter:  time_ms the wait duration in milliseconds
  Returns:    The current system time in milliseconds
  Porting:    Needs to use the final system call for time retrieval
************************************************************************************************************************/
int     system_wait          (int time_ms) {
  int ticks1, ticks2;
  ticks1=system_time();
  ticks2=ticks1;
  /* <porting> Replace 'clock' by whatever is necessary to return the system time in milliseconds */
  while((ticks2-ticks1)<time_ms) {ticks2=system_time();}
  return (int)ticks2;
}
/************************************************************************************************************************
  system_time function
  Use:        current system time retrieval function
              Used to retrieve the current system time in milliseconds
  Returns:    The current system time in milliseconds
  Porting:    Needs to use the final system call
************************************************************************************************************************/
int     system_time          (void)        {
  /* <porting> Replace 'clock' by whatever is necessary to return the system time in milliseconds */
  //return (int)clock()*1000/CLOCKS_PER_SEC;
   	 	struct timeval tv1;
   	 	do_gettimeofday(&tv1);
		return (int)(tv1.tv_usec/1000 +tv1.tv_sec*1000 );
}
/************************************************************************************************************************
  L0_Init function
  Use:        layer 0 initialization function
              Used to set the layer 0 context parameters to startup values.
              It must be called first and once per Layer 1 instance (i.e. once for the tuner and once for the demodulator).
              It is automatically called by the Layer 1 init function.
  Parameters: mustReadWithoutStop has been added to manage the case when some components do not allow a stop in a 'read'.
              i2c usually allows 'write 0xc8 0x01 0x02' followed by 'read 0xc9 1' to read the byte at index 0x0102.
              This should return the same data as 'read 0xc8 0x01 0x02 1'.
              If this is not allowed, set mustReadWithoutStop at 1.
              NB: at the date of writing, this behavior has only been detected in the RDA5812 satellite tuner.
  Returns:    void
  Porting:    If some members of the L0_Context structure are removed, they need to be removed from here too
************************************************************************************************************************/
void    L0_Init              (L0_Context *i2c) {
  int i;
  #ifdef    USB_Capability
  char rettxtBuffer[200];
  char *rettxt;
  double dval;
  rettxt = rettxtBuffer;
  #endif /* USB_Capability */

  pucAddressBuffer = (unsigned char *)&ucAddressBuffer;
  pucBuffer        = (unsigned char *)&ucBuffer;
  i2c->address             = 0;
  i2c->indexSize           = 0;
  i2c->connectionType      = CUSTOMER;
  i2c->trackWrite          = 0;
  i2c->trackRead           = 0;
  i2c->mustReadWithoutStop = 0;
  for (i=0; i<SIMULATOR_SCOPE; i++) {Byte[i]=0x00;}
  rawI2C = &rawI2C_context;
  rawI2C->indexSize = 0;
  stored_error_message = L0_error_buffer_0;
  L0_error_message     = L0_error_buffer_1;
  #if 0
  sprintf(stored_error_message, "%s", "");
  #ifdef    USB_Capability
  if (!cypress_checks_done) {
    L0_Connect (i2c, USB);
    L0_Cypress_Process("dll_version", "", 0, &dval, &rettxt);
    SiTRACE("dll_version %s (recommended minimal version 10.23)\n", rettxt);
    L0_Cypress_Process("fw_version" , "", 0, &dval, &rettxt);
    SiTRACE("fw_version  %s (recommended minimal version 10.20)\n", rettxt);
    cypress_checks_done = 1;
  }
  #endif /* USB_Capability */
#ifdef    SiTRACES
  if (!trace_init_done) {
    CUSTOM_PRINTF("\n********** SiTRACES activated *********\nComment the '#define SiTRACES' line\nin Si2147_L0_API.h to de-activate all traces.\n\n");
    SiTraceDefaultConfiguration();
  }
#endif /* SiTRACES */
#endif
};
/************************************************************************************************************************
  L0_InterfaceType function
  Use:        function to return the communication mode string
              Used to display a clear text related to the communication mode.
  Returns:    the string correspopnding to the current connection mode
************************************************************************************************************************/
char*   L0_InterfaceType     (L0_Context* i2c) {
  switch (i2c->connectionType) {
  #ifdef USB_Capability
    case USB:
      return (char *)"USB";
      break;
  #endif /* USB_Capability */
    case SIMU:
      return (char *)"SIMU";
      break;
    case CUSTOMER:
      return (char *)"CUST";
      break;
    default:
      return (char *)"?";
      break;
   };
}
/************************************************************************************************************************
  L0_SetAddress function
  Use:        function to set the device address
              Used to set the I2C address of the component.
              It must be called only once at startup per Layer 1 instance, as the addresses are not expected to change over time.
  Returns:    1 if OK, 0 otherwise
************************************************************************************************************************/
int     L0_SetAddress        (L0_Context* i2c, unsigned int add, int addSize) {
  i2c->address   = add;
  i2c->indexSize = addSize;
  switch (i2c->connectionType) {
  #ifdef USB_Capability
    case USB:
      L0_Connect (i2c, i2c->connectionType);
      return 1;
      break;
  #endif /* USB_Capability */
    case SIMU:
      return 1;
      break;
    default:
      break;
   };
  return 0;
}
/************************************************************************************************************************
  L0_Connect function
  Use:        Core layer 0 connection function
              Used to switch between various connection modes
              If a single connection  mode is used, this function can be removed, and the corresponding calls from the Layer 1 code can be removed as well.
              It can be useful during SW development, to easily switch between the simulator and the actual HW.
  Returns:    1 if connected to actual hw
  Porting:    Depending on the i2c layer, this function could be removed if a single connection mode is allowed
************************************************************************************************************************/
int     L0_Connect(L0_Context *i2c, CONNECTION_TYPE connType) {
  #ifdef    USB_Capability
  if (i2c->connectionType == USB) {
    Cypress_USB_Close();
  }
  #endif /* USB_Capability */
  switch (connType) {
    #ifdef    USB_Capability
    case USB:
      Cypress_USB_Open();
      i2c->connectionType =  USB;
      return 1;
      break;
    #endif /* USB_Capability */
    case CUSTOMER:
      i2c->connectionType =  CUSTOMER;
      return 1;
      break;
    default:
      i2c->connectionType =  SIMU;
      return 0;
    break;
  };
  return 0;
}
HI_U32  si2147_tda_write(HI_U8 uAddress, HI_U32 uNbData, HI_U8 * pDataBuff)
{
	HI_U8  i;
	HI_S32 s32Ret;
	TUNER_I2C_DATA_S stI2cDataStr = { 0 };
	HI_U8 au8SendData[256] = { 0 };
	HI_U8 *pu8Tmp = NULL;


	if (NULL == pDataBuff)
	{
		HI_ERR_TUNER( "pointer is null\n");
		return HI_FAILURE;
	}

	qam_config_i2c_out (s_u32Si2147TunerPort, 1);

	pu8Tmp = pDataBuff;
	//au8SendData[0] = 0x00;
	for (i = 0; i < uNbData; i++)
	{
		au8SendData[i] = *pu8Tmp;
		pu8Tmp++;
	}

	stI2cDataStr.pu8ReceiveBuf = 0;
	stI2cDataStr.u32ReceiveLength = 0;
	stI2cDataStr.pu8SendBuf = au8SendData;
	stI2cDataStr.u32SendLength = uNbData;	//subaddress + data
	//s32Ret = tuner_i2c_send_data (g_stTunerOps[s_u32Si2147TunerPort].enI2cChannel, uAddress, &stI2cDataStr);
	
	s32Ret = tuner_i2c_send_data (s_u32Si2147I2cChannel, uAddress, &stI2cDataStr);
	if (HI_SUCCESS != s32Ret)
	{
		HI_ERR_TUNER("[si2147 error]%s %d IIC write err!status:0x%x\n", __FUNCTION__, __LINE__, s32Ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;

}

 HI_U32  si2147_tda_read(HI_U8 uAddress,HI_U32 uNbData, HI_U8 * pDataBuff)
{
	HI_S32 i;
	HI_S32 s32Ret;
	TUNER_I2C_DATA_S stI2cDataStr = { 0 };
	HI_U8 au8RcvData[256] = { 0 };

       HI_U8 uSubAddress = 0x00;
   //HI_ERR_TUNER("=========si2147_tda_read==================si2147_tda_read==================\n");
	qam_config_i2c_out (s_u32Si2147TunerPort, 1);
	memset ((void *) au8RcvData, 0, sizeof (au8RcvData));

	stI2cDataStr.pu8ReceiveBuf = au8RcvData;
	stI2cDataStr.pu8SendBuf = &uSubAddress;
	stI2cDataStr.u32ReceiveLength = uNbData;
	stI2cDataStr.u32SendLength = 0;

	s32Ret = tuner_i2c_receive_data_si(g_stTunerOps[s_u32Si2147TunerPort].enI2cChannel, uAddress /*|0x01 */ ,&stI2cDataStr);
	//s32Ret = tuner_i2c_receive_data(s_u32Si2147I2cChannel, uAddress /*|0x01 */ ,&stI2cDataStr);
	if (HI_SUCCESS != s32Ret)
	{
		HI_ERR_TUNER("[si2147 error]%s %d IIC read err! status:0x%x\n", __FUNCTION__, __LINE__, s32Ret);
		return HI_FAILURE;
	}	
	for (i = 0; i < uNbData; i++)
	{
		pDataBuff[i] = au8RcvData[i];
	}
	return HI_SUCCESS;
}

/************************************************************************************************************************
  L0_ReadBytes function
  Use:        lowest layer read function
              Used to read a given number of bytes from the Layer 1 instance.
  Parameters: i2c, a pointer to the Layer 0 context.
              iI2CIndex, the index of the first byte to read.
              iNbBytes, the number of bytes to read.
              *pbtDataBuffer, a pointer to a buffer used to store the bytes.
  Returns:    the number of bytes read.
  Porting:    If a single connection mode is allowed, the entire switch can be replaced by a call to the final i2c read function
************************************************************************************************************************/
int     L0_ReadBytes         (L0_Context* i2c, unsigned int iI2CIndex, int iNbBytes, unsigned char *pucDataBuffer) {
  int r,i,nbReadBytes = 0;
  //HI_S32 s32Ret = 0;
  HI_U32 u32RegAddr = 0;
  //HI_U32 u32DataLen =0;
  
  for (i=0;i<i2c->indexSize;i++) {
    r = 8*(i2c->indexSize -1 -i);
    pucAddressBuffer[i] = (unsigned char)((iI2CIndex & (0xff<<r) ) >> r);
  }
  u32RegAddr = iI2CIndex;
  switch (i2c->connectionType) {
    case CUSTOMER:
	 nbReadBytes = iNbBytes;
             si2147_tda_read ((HI_U8)g_stTunerOps[s_u32Si2147TunerPort].u32TunerAddress, iNbBytes, pucDataBuffer);
      /* <porting> Insert here whatever is needed to
      read iNbBytes bytes
      from the chip whose i2c address is i2c->address,
      starting at index iI2CIndex,
      with an index on i2c->indexSize bytes
      the index bytes being stored in pucAddressBuffer.

      Make it such that on success nbReadBytes = iNbBytes
      and on failure nbReadBytes = 0.
      data bytes will be stored in pucDataBuffer.
      */
      break;
	  default:
      break;
  }
#if 0
  trace_skip_info = 1;
    if (i2c->trackRead==1) {
    sprintf(trace_L0_message, "reading 0x%02x ",i2c->address + 1);
    if (i2c->indexSize) {
      sprintf(trace_L0_message, "%sat", trace_L0_message);
      for (i=0;i<i2c->indexSize;i++) {
        r = 8*(i2c->indexSize -1 -i);
        sprintf(trace_L0_message, "%s 0x%02x", trace_L0_message, ((iI2CIndex & (0xff<<r))>> r)&0xff );
      }
    }
    sprintf(trace_L0_message, "%s <%s", trace_L0_message, L0_InterfaceType(i2c));
    if (nbReadBytes != iNbBytes) {sprintf(trace_L0_message, "%s READ  ERROR!", trace_L0_message);}
    for ( i=0 ; i < nbReadBytes ; i++ ) {  sprintf(trace_L0_message, "%s 0x%02x" ,trace_L0_message , (pucDataBuffer)[i]); }
    SiTRACE("%s\n", trace_L0_message);
    }
    trace_skip_info = 0;
#endif /* SiTRACES */
    return nbReadBytes;
}
/************************************************************************************************************************
  L0_WriteBytes function
  Use:        lowest layer write function
              Used to write a given number of bytes from the Layer 1 instance.
  Parameters: i2c, a pointer to the Layer 0 context.
              iI2CIndex, the index of the first byte to write.
              iNbBytes, the number of bytes to write.
              *pbtDataBuffer, a pointer to a buffer containing the bytes to write.
  Returns:    the number of bytes read.
  Porting:    If a single connection mode is allowed, the entire switch can be replaced by a call to the final i2c write function
************************************************************************************************************************/
int     L0_WriteBytes        (L0_Context* i2c, unsigned int iI2CIndex, int iNbBytes, unsigned char *pucDataBuffer) {
  int r, i, nbWrittenBytes = 0, write_error = 0;
  HI_U32 u32RegAddr = 0;
  //HI_S32 s32Ret = 0;
  for (i=0; i <i2c->indexSize;i++) 
  {
    r = 8*(i2c->indexSize -1 -i);
    pucBuffer[i] = (unsigned char)((iI2CIndex & (0xff<<r) ) >> r);
    pucAddressBuffer[i] = pucBuffer[i];
  }
  for (i=0; i < iNbBytes ;    i++) {
    pucBuffer[i+i2c->indexSize] = pucDataBuffer[i];
  }
  switch (i2c->connectionType) {
  case CUSTOMER:
	  u32RegAddr = iI2CIndex;
	  nbWrittenBytes = iNbBytes;

         si2147_tda_write ((HI_U8)g_stTunerOps[s_u32Si2147TunerPort].u32TunerAddress,  iNbBytes, pucDataBuffer);
   
        /* <porting> Insert here whatever is needed to
        (option 1)
        write iNbBytes bytes
        to the chip whose i2c address is i2c->address,
        starting at index iI2CIndex,
        with an index on i2c->indexSize bytes
        the data bytes being stored in pucDataBuffer.

        (option 2)
        Another option is to
        write iNbBytes + i2c->indexSize bytes
        to the chip whose i2c address is i2c->address,
        the index bytes and data bytes all being stored in pucBuffer.

        Make it such that on success nbWrittenBytes = iNbBytes + i2c->indexSize
        and on failure write_error is incremented.
        */
        break;
     default:
      break;
  }
#if 0
  trace_skip_info = 1;
  if (i2c->trackWrite     == 1) {
    sprintf(trace_L0_message, "writing 0x%02x ",i2c->address);
    if (i2c->indexSize) {
      sprintf(trace_L0_message, "%sat", trace_L0_message);
      for (i=0;i<i2c->indexSize;i++) {
        r = 8*(i2c->indexSize -1 -i);
        if (i2c->indexSize) sprintf(trace_L0_message, "%s 0x%02x", trace_L0_message, ((iI2CIndex & (0xff<<r))>> r)&0xff );
      }
    }
    sprintf(trace_L0_message, "%s %s>", trace_L0_message, L0_InterfaceType(i2c));
    if (write_error) { sprintf(trace_L0_message, "%s WRITE ERROR! (", trace_L0_message); }
      for ( i=0 ; i < iNbBytes ; i++ ) {
      sprintf(trace_L0_message, "%s 0x%02x", trace_L0_message, pucDataBuffer[i]);
      }
    if (write_error) { sprintf(trace_L0_message, "%s )", trace_L0_message); }
    SiTRACE("%s\n", trace_L0_message);
  }
  trace_skip_info = 0;
#endif /* SiTRACES */
  if (write_error) return 0;
  return nbWrittenBytes - i2c->indexSize;
}
/************************************************************************************************************************
  L0_ReadCommandBytes function
  Use:        'command mode' bytes reading function
              Used to read a given number of bytes from the Layer 1 instance in 'command mode'.
  Comment:    The 'command mode' is a specific mode where the indexSize is always 0 and the index is always 0x00
  Parameters: i2c, a pointer to the Layer 0 context.
              iNbBytes, the number of bytes to read.
              *pucDataBuffer, a pointer to a buffer used to store the bytes.
  Returns:    the number of bytes read.
************************************************************************************************************************/
int     L0_ReadCommandBytes  (L0_Context* i2c, int iNbBytes, unsigned char *pucDataBuffer) {
  return L0_ReadBytes (i2c, 0x00, iNbBytes, pucDataBuffer);}
/************************************************************************************************************************
  L0_WriteCommandBytes function
  Use:        'command mode' bytes writing function
              Used to write a given number of bytes to the Layer 1 instance in 'command mode'.
  Comment:    The 'command mode' is a specific mode where the indexSize is always 0 and the index is always 0x00
  Parameters: i2c, a pointer to the Layer 0 context.
              iNbBytes, the number of bytes to write.
              *pucDataBuffer, a pointer to a buffer containing the bytes.
  Returns:    the number of bytes written.
************************************************************************************************************************/
int     L0_WriteCommandBytes (L0_Context* i2c, int iNbBytes, unsigned char *pucDataBuffer) {
  return L0_WriteBytes(i2c, 0x00, iNbBytes, pucDataBuffer);}
  

