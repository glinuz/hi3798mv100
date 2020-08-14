/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file
*
* @brief Common library for Rx and Tx driver API
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "si_drv_common.h"
#include "si_lib_obj_api.h"
#include "si_lib_log_api.h"

/***** Register Module name **************************************************/

//SII_LIB_OBJ_MODULE_DEF(drv_common);

/***** public macro definitions **********************************************/

/***** public type definitions ***********************************************/

/***** local data objects ****************************************************/

/***** local functions *******************************************************/
static uint8_t sInfoFrameLengthGet(SiiInfoFrame_t *pInfoFrame)
{
	return pInfoFrame->b[2];
}

/***** public functions ******************************************************/
void SiiDrvInfoFrameCalculateChecksum(SiiInfoFrame_t *pInfoFrame)
{
	uint8_t  *p = pInfoFrame->b;
	uint8_t len = p[2] + 4;
	uint8_t cs  = 0;


	pInfoFrame->b[3] = 0;

	while( len-- )
	{
		cs += *p;
		p++;
	}
	pInfoFrame->b[3] = (0 - cs);
}
void SiiDrvClearInfoFrame(SiiInfoFrameId_t ifId, SiiInfoFrame_t *pInfoFrame)
{
	pInfoFrame->ifId = ifId;
	SII_MEMSET(&pInfoFrame->b[0], 0, SII_INFOFRAME_MAX_LEN);
}

void SiiDrvLogInfoFrame(SiiInfoFrame_t *pInfoFrame)
{
	uint8_t len    = sInfoFrameLengthGet(pInfoFrame)+4;
	uint8_t *pData = &(pInfoFrame->b[0]);

	SII_LIB_LOG_DEBUG1(NULL, "IF-");
	switch(pInfoFrame->ifId)
	{
	case SII_INFO_FRAME_ID__AVI    : SII_LIB_LOG_DEBUG2("AVI  :"); break;
	case SII_INFO_FRAME_ID__AUDIO  : SII_LIB_LOG_DEBUG2("AUDIO:"); break;
	case SII_INFO_FRAME_ID__VS     : SII_LIB_LOG_DEBUG2("VS   :"); break;
	case SII_INFO_FRAME_ID__SPD    : SII_LIB_LOG_DEBUG2("SPD  :"); break;
	case SII_INFO_FRAME_ID__GBD    : SII_LIB_LOG_DEBUG2("GBD  :"); break;
	case SII_INFO_FRAME_ID__MPEG   : SII_LIB_LOG_DEBUG2("MPEG :"); break;
	case SII_INFO_FRAME_ID__ISRC   : SII_LIB_LOG_DEBUG2("ISRC :"); break;
	case SII_INFO_FRAME_ID__ISRC2  : SII_LIB_LOG_DEBUG2("ISRC2:"); break;
	case SII_INFO_FRAME_ID__ACP    : SII_LIB_LOG_DEBUG2("ACP  :"); break;
#ifdef HDMI_HDR_SUPPORT
    case SII_INFO_FRAME_ID__DRM    : SII_LIB_LOG_DEBUG2("DRM  :"); break;
#endif
	default : break;
	}
	while(len--)
	{
		SII_LIB_LOG_DEBUG2(" %02X", *pData);
		pData++;
	}
	SII_LIB_LOG_DEBUG2("\n");
}

/***** end of file ***********************************************************/
