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


#define DPD_CAL_PASS_THRES		5
#define DPD_CAL_MAX_RETRY		5

INT32 CalcRCalibrationCode(IN PRTMP_ADAPTER pAd, IN INT32 D1, IN INT32 D2);

INT Set_TestRxIQCalibration_Proc(IN RTMP_ADAPTER * pAd, IN PSTRING arg);

VOID R_Calibration(IN PRTMP_ADAPTER pAd);

VOID RtmpKickOutHwNullFrame(IN PRTMP_ADAPTER pAd, IN BOOLEAN bPrepareContent, IN BOOLEAN bTransmit);

VOID DPD_IQ_Swap_AM_PM_Inversion(IN PRTMP_ADAPTER pAd, IN UCHAR AntIdx);

VOID DPD_AM_AM_LUT_Scaling(IN PRTMP_ADAPTER pAd, IN UCHAR AntIdx);

UCHAR DPD_Calibration(IN PRTMP_ADAPTER pAd, IN UCHAR AntIdx);

VOID DoDPDCalibration(IN PRTMP_ADAPTER pAd);

INT Set_DPDCalPassThres_Proc(IN RTMP_ADAPTER * pAd, IN PSTRING arg);

INT Set_TestDPDCalibration_Proc(IN RTMP_ADAPTER * pAd, IN PSTRING arg);

INT Set_TestDPDCalibrationTX0_Proc(IN RTMP_ADAPTER * pAd, IN PSTRING arg);

INT Set_TestDPDCalibrationTX1_Proc(IN RTMP_ADAPTER * pAd, IN PSTRING arg);

VOID LOFT_IQ_Calibration(IN RTMP_ADAPTER * pAd);

BOOLEAN BW_Filter_Calibration(IN PRTMP_ADAPTER pAd, IN BOOLEAN bTxCal);

VOID RxDCOC_Calibration(IN PRTMP_ADAPTER pAd);

VOID RXIQ_Calibration(IN PRTMP_ADAPTER pAd);

VOID RF_SELF_TXDC_CAL(IN PRTMP_ADAPTER pAd);
