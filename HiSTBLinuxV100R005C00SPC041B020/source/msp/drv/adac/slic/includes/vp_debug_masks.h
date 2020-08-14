/** \file vp_debug_masks.h
 * vp_debug_masks.h
 *
 * This file contains the debug masks for the VP-APi_II
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7140 $
 * $LastChangedDate: 2010-05-12 09:50:40 -0500 (Wed, 12 May 2010) $
 */

#ifndef VP_DEBUG_MASKS_H
#define VP_DEBUG_MASKS_H

/* VP-API debug message types: */
#define VP_DBG_ERROR        0x00000001 /* Any error condition */
#define VP_DBG_WARNING      0x00000002 /* Any warning condition */
#define VP_DBG_INFO         0x00000004 /* Un-categorized information */
#define VP_DBG_API_FUNC     0x00000008 /* API function entry/exit (except tick) */

#define VP_DBG_API_FUNC_INT 0x00000010 /* Internal API function entry/exit */
#define VP_DBG_HAL          0x00000020 /* HAL traffic */
#define VP_DBG_SSL          0x00000040 /* SSL function entry/exit */
#define VP_DBG_EVENT        0x00000080 /* Events/results from VpGetEvent()/VpGetResults() */

#define VP_DBG_HOOK         0x00000100 /* Hook Based Code */
#define VP_DBG_LINE_STATE   0x00000200 /* Set Line State Based Code */
#define VP_DBG_CALIBRATION  0x00000400 /* VpCalCodec(), VpCalLine(), VpCal() */
#define VP_DBG_TEST         0x00000800 /* Verbose test debug */

#define VP_DBG_TEST_FUNC    0x00001000 /* Test I/F function enter and exit */
#define VP_DBG_FXO_FUNC     0x00002000 /* FXO Detection Code */
#define VP_DBG_SEQUENCER    0x00004000 /* Sequencer Based Code */
#define VP_DBG_CID          0x00008000 /* Caller ID funcitons */

#define VP_DBG_TEST_PCM     0x00010000 /* Verbose test pcm collection */

#define VP_DBG_ALL          0xFFFFFFFF

#endif /* VP_DEBUG_MASKS_H */
