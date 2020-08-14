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

#ifndef __FRQCAL_H__
#define __FRQCAL_H__

/* */
/* The frequency calibration control */
/* */
typedef struct _FREQUENCY_CALIBRATION_CONTROL {
	BOOLEAN bEnableFrequencyCalibration;	/* Enable the frequency calibration algorithm */

	BOOLEAN bSkipFirstFrequencyCalibration;	/* Avoid calibrating frequency at the time the STA is just link-up */
	BOOLEAN bApproachFrequency;	/* Approach the frequency */
	CHAR AdaptiveFreqOffset;	/* Adaptive frequency offset */
	CHAR LatestFreqOffsetOverBeacon;	/* Latest frequency offset from the beacon */
	CHAR BeaconPhyMode;	/* Latest frequency offset from the beacon */

} FREQUENCY_CALIBRATION_CONTROL, *PFREQUENCY_CALIBRATION_CONTROL;

#define RTMP_FREQ_CAL_DISABLE(__pAd)									\
	__pAd->FreqCalibrationCtrl.bEnableFrequencyCalibration = FALSE

/* */
/* Invalid frequency offset */
/* */
#define INVALID_FREQUENCY_OFFSET			-128

/* */
/* The upperbound/lowerbound of the frequency offset */
/* */
#define UPPERBOUND_OF_FREQUENCY_OFFSET		127
#define LOWERBOUND_OF_FREQUENCY_OFFSET	-127

/*#ifdef RT5390 */
/* */
/* The trigger point of the high/low frequency */
/* */
#define HIGH_FREQUENCY_TRIGGER_POINT_OFDM		20
#define LOW_FREQUENCY_TRIGGER_POINT_OFDM		-20
#define HIGH_FREQUENCY_TRIGGER_POINT_CCK		4
#define LOW_FREQUENCY_TRIGGER_POINT_CCK		-4


/* */
/* The trigger point of decreasng/increasing the frequency offset */
/* */
#define DECREASE_FREQUENCY_OFFSET_OFDM			10
#define INCREASE_FREQUENCY_OFFSET_OFDM			-10
#define DECREASE_FREQUENCY_OFFSET_CCK			2
#define INCREASE_FREQUENCY_OFFSET_CCK			-2
/*#endif // RT5390 */
/* */
/* The trigger point of decreasng/increasing the frequency offset */
/* */
#define DECREASE_FREQUENCY_OFFSET			3
#define INCREASE_FREQUENCY_OFFSET			-3

/* */
/* Frequency calibration period */
/* */

#define FREQUENCY_CALIBRATION_PERIOD		100

#endif /* __FRQCAL_H__ */
