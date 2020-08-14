/*
  * Copyright (c) 2016 MediaTek Inc.  All rights reserved.
  *
  * This software is available to you under a choice of one of two
  * licenses.  You may choose to be licensed under the terms of the GNU
  * General Public License (GPL) Version 2, available from the file
  * COPYING in the main directory of this source tree, or the
  * BSD license below:
  *
  *     Redistribution and use in source and binary forms, with or
  *     without modification, are permitted provided that the following
  *     conditions are met:
  *
  *      - Redistributions of source code must retain the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer.
  *
  *      - Redistributions in binary form must reproduce the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer in the documentation and/or other materials
  *        provided with the distribution.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */

/****************************************************************************

    Module Name:
    vr_brlt.c
 
    Abstract:
    Only for BroadLight 2348 platform.
 
    Revision History:
    Who        When          What
    ---------  ----------    ----------------------------------------------
    Sample Lin	01-12-2010    Created

***************************************************************************/

#define RTMP_MODULE_OS
#define RTMP_MODULE_OS_UTIL

#define MODULE_BDLT

/*#include "rt_config.h" */
#include "rtmp_comm.h"
#include "rt_os_util.h"


#if defined(PLATFORM_BL2348) || defined(PLATFORM_BL23570)

/* global variables */
int (*pToUpperLayerPktSent)(struct sk_buff *pSkb) = netif_rx ;




/*
========================================================================
Routine Description:
	Assign the briding function.

Arguments:
	xi_destination_ptr	- bridging function

Return Value:
	None

Note:
	The function name must be replace_upper_layer_packet_destination.
========================================================================
*/
VOID replace_upper_layer_packet_destination(VOID *pXiDestination)
{
	DBGPRINT(RT_DEBUG_TRACE, ("ralink broad light> replace_upper_layer_packet_destination\n"));
	pToUpperLayerPktSent = pXiDestination ;
} /* End of replace_upper_layer_packet_destination */


EXPORT_SYMBOL(pToUpperLayerPktSent);
EXPORT_SYMBOL(replace_upper_layer_packet_destination);

#endif /* PLATFORM_BL2348 */


/* End of vr_bdlt.c */
