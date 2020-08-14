/******************************************************************************

                  版权所有 (C), 2001-2020, 华为技术有限公司

 ******************************************************************************
  文 件 名   : gstsmeptscalibration.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2016年7月3日
  最近修改   :
  功能描述   : sme pts 校准库头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年7月3日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

#ifndef __GST_SME_PTS_CALIBRATION_H__
#define __GST_SME_PTS_CALIBRATION_H__

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>
#include "gstsme.h"

G_BEGIN_DECLS

typedef gpointer GstSmePTSCalibrateHDL;

typedef enum _EmEntryID
{
  E_ENTRY_TYPE_AUDIO = 0,
  E_ENTRY_TYPE_VIDEO,
  E_ENTRY_TYPE_TEXT,
  E_ENTRY_TYPE_PCR,
  E_ENTRY_TYPE_BOTTOM
}EmEntryID;

/*
 * gst_sme_pts_calibration_create:
 * @pszowner_name:name of calibration's owner.
 * create a pts calibration
 *
 *
 * Returns: handle of calibration, NULL if failed.
 */
GstSmePTSCalibrateHDL
gst_sme_pts_calibration_create(IN const gchar* pszowner_name);

/*
 * gst_sme_pts_calibration_destroy:
 * @p2this:a pointer of GstSmePTSCalibrateHDL.
 *
 * destroy a pts calibration handle, this function will set *p2this = NULL
 * if (p2this != NULL)
 *
 *
 * Returns: handle of calibration, NULL if failed.
 */
void
gst_sme_pts_calibration_destroy(INOUT GstSmePTSCalibrateHDL* p2this);

/*
 * gst_sme_pts_calibration_clear:
 * @pthis:GstSmePTSCalibrateHDL.
 * clear the base info of this hdl.
 *
 * MT safe.
 *
 * Returns: void
 */
void gst_sme_pts_calibration_clear(INOUT GstSmePTSCalibrateHDL pthis);

/*
 * gst_sme_pts_calibration_entry_update_segment:
 * @pthis:GstSmePTSCalibrateHDL.
 * @eentry_id:entryid.
 * @segment:sement of this owner.
 * update segment this owner.
 *
 *
 * Returns: entry id
 */
void
gst_sme_pts_calibration_entry_update_segment(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id, IN GstSegment segment);

/*
 * gst_sme_pts_calibration_entry_update_presync_info:
 * @pthis:GstSmePTSCalibrateHDL.
 * @eentry_id:entryid.
 * @presyncinfo:presync info of this owner.
 * update presync info this owner.
 *
 *
 * Returns: entry id
 */
void
gst_sme_pts_calibration_entry_update_presync_info(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id, IN GstSlowPreSyncInfo presyncinfo);

/*
 * gst_sme_pts_calibration_entry_create:
 * @pthis:GstSmePTSCalibrateHDL.
 * @eentry_id:entryid.
 * @pszowner_name:name of calibration entry's owner.
 * @owner:who own this hdl, we will ref it. and unref it when hdl destroy.
 *        in most case is sink.
 * create a pts calibration
 *
 *
 * Returns: entry id
 */
gboolean
gst_sme_pts_calibration_entry_init(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id, IN const gchar* pszowner_name, GstElement* ele_owner);

/*
 * gst_sme_pts_calibration_entry_destroy:
 * @pthis:GstSmePTSCalibrateHDL.
 * @eentry_id:entry id.
 *
 * destroy a entry.
 *
 *
 * Returns: void
 */
void
gst_sme_pts_calibration_entry_deinit(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id);

/*
 * gst_sme_pts_calibration_entry_clear:
 * @pthis:GstSmePTSCalibrateHDL.
 * @eentry_id:entry id.
 * reset a pts calibration handle, this function will clear just all time info
 * record before except cumulation pts hop.
 *
 * MT safe.
 *
 * Returns: void
 */
void gst_sme_pts_calibration_entry_clear(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id);

/*
 * gst_sme_pts_calibration_entry_execute:
 * @pthis:GstSmePTSCalibrateHDL.
 * @eentry_id:entry id
 * @in_pts:pts to check and Calibrate. unit is ns.
 * @in_duration:duration to check and Calibrate. unit is ns.
 * @pout_pts:pointer of the result of pts calibration. unit is ns.
 * @pout_duration:pointer of the result of duration calibration,  unit is ns.
 *                currently is reserved.
 *
 * check and calibration if pts hop.
 *
 * MT unsafe.
 *
 * Returns: TRUE if pts calibrated, others FALSE.
 */
gboolean gst_sme_pts_calibration_entry_execute(INOUT GstSmePTSCalibrateHDL pthis,
    IN EmEntryID eentry_id,
    IN GstClockTime in_pts, IN GstClockTime in_duration,
    IN GstClockTime* pout_pts, IN GstClockTime* pout_duration);

G_END_DECLS



#endif /*__GST_SME_PTS_CALIBRATION_H__*/
