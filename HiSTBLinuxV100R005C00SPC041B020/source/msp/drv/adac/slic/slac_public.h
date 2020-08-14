/***************************************************************************
*   slac_public.h
*****************************************************************************/
#ifndef _INC_SLAC_PUBLIC   
#define _INC_SLAC_PUBLIC

#include "vp_api_types.h"
#include "vp_api_common.h"


#define OSDRV_MODULE_VERSION_STRING "WISH-SLAC @Hi3511v110 2010-02-02 20:53:20"
/* define device name */
#define SLAC_DEVICE_NAME "le89116"

#define INIT_OK         	1
#define DAT_WAKEUP     		2
/* UVB specific values for device ID depending on the MPI Mode */
#define MPI_3WIRE 0x30
#define MPI_4WIRE 0x10

/* Current configuration of Line Module */
#define MPI_MODE MPI_4WIRE

/* Application specific #define to improve code readability */
#define NUM_DEVS      2     /* Total number of devices on LM */
#define DEV_LE89316   0     /* First device - Le89316 with 1 FXS and 1 FXO */
#define DEV_LE89116   1     /* Second device - Le89116 with 1 FXS */

#define NUM_LINES     3     /* Total number of lines on LM */
#define FXS_LINE_CONN 0     /* First FXS line, used for FXS-FXO voice and event
                             * passthrough. */
#define FXO_LINE      1     /* FXO line used for FXS-FXO voice and event
                             * passthrough. */
#define FXS_LINE_DAT  2     /* Second FXS line, used for Dial-A-Tone */

#define NORMAL  FALSE
#define POLREV  TRUE

typedef enum
{
   	IOCTL_INITLINE = 0,
	IOCTL_CONFIG_LINE,
	IOCTL_CAL_CODEC,
	IOCTL_INIT_RING,
	IOCTL_INIT_CID,
	IOCTL_INIT_METER,
	IOCTL_INIT_PROFILE,

	IOCTL_SET_LINE_STATE,
	IOCTL_SET_LINE_TONE,
	IOCTL_SET_RELAY_STATE,
	IOCTL_SEND_SIGNAL,
	IOCTL_SEND_CID,
	IOCTL_START_METER,
	IOCTL_SET_OPTION,   	
	IOCTL_DEVICE_IO_ACCESS,
	
	IOCTL_GET_EVENT,
	IOCTL_GET_LINE_STATUS,
	IOCTL_GET_LINE_STATE,
	IOCTL_FLUSH_EVENTS,
	IOCTL_GET_RESULTS,
	IOCTL_CLEAR_RESULTS,
	IOCTL_LOW_LEVEL_CMD,
	IOCTL_CONTINUE_CID,
	IOCTL_MAX_INDEX
} IOCTL_CMD_INDEX;
#define SLAC_INITIAL        201
#define SLAC_REG_READ       202
#define SLAC_REG_WRITE      203
#define SIO0_REG_DUMP       204
#define LE88221_MAX_CMD_LENGTH  16
typedef struct{   
unsigned char    regWR;   
unsigned char    regLen;   
unsigned char    regVal[LE88221_MAX_CMD_LENGTH];
} SLIC_REGISTER_VALUE;
/* Enum type defining possible FXS line states */
typedef enum {
    FXS_OHT,        /**< Line is neither off-hook, nor Ringing */
    FXS_TALK,       /**< Line is detecting offhook and should be set to Talk */
    FXS_RINGING,    /**< Line is in a Ringing state */
    FXS_DISCONNECT, /**< Line is in disconnect state */
    FXS_LINES_ENUM_SIZE /* Portability Req. */
} FXSLineStates;

/* Enum type defining possible FXO line states */
typedef enum {
    FXO_LOOP_OPEN,  /**< FXO is providing a loop open with OHT */
    FXO_LOOP_CLOSE, /**< FXO is providing a loop close */
    FXO_LINES_ENUM_SIZE /* Portability Req. */
} FXOLineStates;
typedef struct{
	VpEventType pSlacEvent;
    VpProfilePtrType *ptonePtrs;
    VpProfilePtrType *pcadencePtrs;    
}READ_SLAC_TYPE;
#define VP890_MAX_MPI_DATA      15
typedef struct{
    uint8 mpireg;
    uint8 mpiData[VP890_MAX_MPI_DATA];
    uint8 mpiLen;
    VpStatusType status;
}SLAC_REG_TYPE;    
#define TONE_PTRS_LEN       6
#define CADENCE_PTRS_LEN    6

#define VP_EVENT_MASK_ALL       0xFFFF
/* typedefs. */
typedef int (*FN_IOCTL) (unsigned long arg);
#endif   /* SLAC_PUBLIC_H */
