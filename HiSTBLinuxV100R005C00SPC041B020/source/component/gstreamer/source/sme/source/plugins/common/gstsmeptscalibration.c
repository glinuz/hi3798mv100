/**************************************************************************//**

  Copyright (C), 2001-2020, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file          -gstsmeptscalibration.c
 * @brief         -check andr calibrate media pts
 * @author
 * @date          -2016/7/08
 * @version   -V1.00
 *
 * History:
 *
 * Number  : V1.00
 * Date      : 2016/7/08
 * Author    :
 * Desc      : Created file
 *
******************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>
#include <sys/time.h>
#include "osal_mem.h"
#include "gstsmeptscalibration.h"

//lint -e585 (可变参数...报错，为gstreamer中的日志宏，无法修改。)
//lint -e40   //va_list不识别
//lint -e10   //va_list不识别导致报变量定义缺少";"
//lint -e522  //va_list不识别导致
//lint -e717  //do{}while(0)
//lint -e838  //前一次赋值未使用，一般用于变量定义时赋空
//lint -esym(530, var_args)
//lint -e730  //函数的布尔类型参数。
//lint -e708  //union initialization

#define GST_SME_HOP_THRESHOLD (100*GST_MSECOND)//hop threshold
#define GST_CALIBRATION_THRESHOLD (300*GST_MSECOND)//PTS hop big jump calibration threshold
#define GST_REFRESH_THRESHOLD (1.5*GST_SECOND)//PTS hop big jump calibration threshold
#define GST_SME_DURATION_DFT (40*GST_MSECOND)//default duration threshold
#define GST_WAIT_PRESYNC_TIME_DEFAULT (500*GST_MSECOND)//500ms

typedef struct _tagGstSmePtsCalibrationBaseInfo
{
  GstClockTime u64base;//time base
  GstClockTimeDiff i64offset;//offset of time base
  GstClockTime u64sys_time;//system time when report this info.
  GstElement* reporter;//
}GstSmePtsCalibrationBaseInfo;

typedef enum _tagCheckHopState
{
  E_ENTRY_STATE_IDLE = 0,//IDLE状态，未init
  E_ENTRY_STATE_INIT,//初始状态
}EmCheckHopState;

typedef struct _tagGstSmeTimeInfo
{
  GstClockTime pts;
  GstClockTime duration;
  GstClockTime out_pts;
  GstClockTime sys_time;
}GstSmeTimeInfo;

typedef struct _tagEntryCtx
{
  GRecMutex rec_mutex;
  gchar* pszowner_name;//owner name
  GstSmeTimeInfo time_info;//time info of last;
  EmCheckHopState estate;//state vale currently.
  guint64 u64calibrate_num;//calibrate times
  GstElement* ele_owner;//element own this entry.
  GstClockTime u64calibratebase;//
  gint64 i64calibrateoffset;//
  GstSegment segment;
  GstSlowPreSyncInfo presyncinfo;
}EntryCtx;
//out_pts = in_pts - i64calibrateoffset + u64calibratebase;

typedef struct _tagGstSmePTSCalibrateCtx
{
  gchar* pszowner_name;//owner name
  EntryCtx entry[E_ENTRY_TYPE_BOTTOM];
  GRecMutex rec_mutex;
  GstSmePtsCalibrationBaseInfo base_info;
}GstSmePTSCalibrateCtx;

static void init_presync_info(GstSlowPreSyncInfo *info)
{
  E_SME_MEM_SECURE_RET osal_mem_err;

  osal_mem_err = VOS_Memset_S(info, sizeof(GstSlowPreSyncInfo),
    -1, sizeof(GstSlowPreSyncInfo));
  if(E_SME_MEM_OK != osal_mem_err){
    GST_WARNING("VOS_Memset_S failed(%#x)", osal_mem_err);
  }

  info->gdrate = 0.5;
  info->last_presync_pts = GST_CLOCK_TIME_NONE;
  info->slow_presync_state = 0;
  info->start_wait_presync_systime = GST_CLOCK_TIME_NONE;
  info->wait_presync_max_time = GST_WAIT_PRESYNC_TIME_DEFAULT;
  info->slow_presync_stime = GST_CLOCK_TIME_NONE;

  return;
}

//lint -e571
static gboolean
check_and_calibrate(INOUT GstSmePTSCalibrateCtx* pthis_ctx,
    INOUT EntryCtx* pthis_entry,
    IN GstClockTime in_pts, IN GstClockTime in_duration,
    IN GstClockTime* pout_pts, IN GstClockTime* pout_duration)
{
  gboolean bneed_calibrate = FALSE;
  GstClockTime last_pts = pthis_entry->time_info.pts;
  GstClockTime last_duration = pthis_entry->time_info.duration;
  GstClockTime u64hop_thd;
  GstClockTimeDiff i64diff;
  GstClockTime cur_syst = (GstClockTime)GST_USECOND * (guint64)g_get_monotonic_time();


  if(!GST_CLOCK_TIME_IS_VALID(last_duration)){
    last_duration = GST_SME_DURATION_DFT;
    GST_WARNING("%s:invalid duration, use default.",
      GST_SAFE_STRING(pthis_entry->pszowner_name));
  }
  if(GST_CLOCK_TIME_IS_VALID(last_pts)){
    if(GST_CLOCK_TIME_IS_VALID(in_pts)){
      i64diff = (GstClockTimeDiff)(in_pts - last_pts);
    }else{
      i64diff = 0LL;
      in_pts = last_pts + last_duration;
      GST_WARNING("%s: cur pts invalid, last_pts:%lld",
        GST_SAFE_STRING(pthis_entry->pszowner_name), last_pts);
    }
  }else{
    if(GST_CLOCK_TIME_IS_VALID(pthis_entry->time_info.sys_time)){
      i64diff = -1LL;
      GST_WARNING("%s: last pts invalid, in_pts:%lld",
        GST_SAFE_STRING(pthis_entry->pszowner_name), in_pts);
    }else{
      i64diff = 0LL;
    }
  }

  u64hop_thd = last_duration + GST_SME_HOP_THRESHOLD;

  if(i64diff >= 0 && i64diff <= (GstClockTimeDiff)u64hop_thd){
    ;//normal do nothing.
  }else{
    if(i64diff > (GstClockTimeDiff)u64hop_thd){
      //pts hop VS system time diff.
      GstClockTimeDiff syst_diff = (GstClockTimeDiff)(cur_syst -
                                    pthis_entry->time_info.sys_time);
      GST_WARNING("%s:syst_diff:%lld, i64diff=%lld",
        GST_SAFE_STRING(pthis_entry->pszowner_name), syst_diff, i64diff);

      if(ABS(syst_diff - i64diff) > GST_CALIBRATION_THRESHOLD){
        //hop, calibrate it
        bneed_calibrate = TRUE;
        GST_WARNING("%s:bneed_calibrate",
          GST_SAFE_STRING(pthis_entry->pszowner_name));
      }
    }else{
      if(ABS(i64diff) > GST_SME_HOP_THRESHOLD || !GST_CLOCK_TIME_IS_VALID(last_pts)){
        bneed_calibrate = TRUE;
        GST_WARNING("%s:bneed_calibrate i64diff < 0, last_pts:%lld",
          GST_SAFE_STRING(pthis_entry->pszowner_name), last_pts);
      }
    }

    if(bneed_calibrate){
      //normal donothing;
      gboolean bneed_refresh = FALSE;
      GstClock * cur_clock = gst_element_get_clock(pthis_entry->ele_owner);
      g_rec_mutex_lock(&pthis_ctx->rec_mutex);

      if(pthis_ctx->base_info.reporter == pthis_entry->ele_owner){
        //refresh
        bneed_refresh = TRUE;
      }else{
#if 1
        //for time-shift trick-mode,have audio but donot send audio when trick-mode.
        GstClockTimeDiff syst_diff = (GstClockTimeDiff)(cur_syst -
                                      pthis_ctx->base_info.u64sys_time);
        GST_WARNING("%s:syst_diff:%lld, i64diff=%lld",
          GST_SAFE_STRING(pthis_entry->pszowner_name), syst_diff, i64diff);

        if(ABS(syst_diff) > GST_REFRESH_THRESHOLD
          || NULL == pthis_ctx->base_info.reporter){
          //refresh
          bneed_refresh = TRUE;
        }else{
          //use this,do nothing;
          GST_WARNING("%s:do not need refresh",
            GST_SAFE_STRING(pthis_entry->pszowner_name));
        }
#else
        if((pthis_entry == &pthis_ctx->entry[E_ENTRY_TYPE_AUDIO])
          || ((pthis_ctx->entry[E_ENTRY_TYPE_AUDIO].time_info.sys_time == -1ULL)
          && (pthis_ctx->entry[E_ENTRY_TYPE_AUDIO].time_info.pts == -1ULL))){
          bneed_refresh = TRUE;
          //use this,do nothing;
          GST_WARNING("%s:need refresh, asink.time_info.sys_time:%lld,"
            "asink.time_info.sys_time:%lld",
            GST_SAFE_STRING(pthis_entry->pszowner_name),
            pthis_ctx->entry[E_ENTRY_TYPE_AUDIO].time_info.sys_time,
            pthis_ctx->entry[E_ENTRY_TYPE_AUDIO].time_info.pts);
        }else{
          //use this,do nothing;
          GST_WARNING("%s:do not need refresh",
            GST_SAFE_STRING(pthis_entry->pszowner_name));
        }
#endif
      }

      if(bneed_refresh){
        GstState ele_state = GST_STATE(pthis_entry->ele_owner);
        GstState pending_state = GST_STATE_PENDING(pthis_entry->ele_owner);

        pthis_ctx->base_info.i64offset = (gint64)in_pts;
        if(GST_STATE_PLAYING == ele_state && GST_STATE_VOID_PENDING == pending_state){
          if(cur_clock){
            pthis_ctx->base_info.u64base = gst_clock_get_time(cur_clock);
          }else{
            GST_WARNING("%s:invalid clock", GST_SAFE_STRING(pthis_entry->pszowner_name));
            pthis_ctx->base_info.u64base = cur_syst;
          }

          pthis_ctx->base_info.u64base -= gst_element_get_base_time(pthis_entry->ele_owner);
          if(0 == pthis_entry->presyncinfo.slow_presync_state){
            pthis_ctx->base_info.u64base =  (guint64)(pthis_ctx->base_info.u64base * pthis_entry->segment.rate);
          }else if (1 == pthis_entry->presyncinfo.slow_presync_state){
            pthis_ctx->base_info.u64base = (guint64)(pthis_ctx->base_info.u64base * pthis_entry->presyncinfo.gdrate);
            GST_WARNING("%s:refresh.baseinfo in slow-presync",
              GST_SAFE_STRING(pthis_entry->pszowner_name));
          }else if(2 == pthis_entry->presyncinfo.slow_presync_state
            && pthis_entry->presyncinfo.gdrate < 1.0
            && pthis_entry->presyncinfo.gdrate > 0.0 ){
            pthis_ctx->base_info.u64base -= (guint64)((1 - pthis_entry->presyncinfo.gdrate) * pthis_entry->presyncinfo.slow_presync_stime);
            GST_WARNING("%s:refresh.baseinfo,end of slow-presync",
              GST_SAFE_STRING(pthis_entry->pszowner_name));
          }
        }else{
          GST_WARNING("%s:not playing, ele_state:%d, pending_state:%d",
            GST_SAFE_STRING(pthis_entry->pszowner_name),
            ele_state,
            pending_state);
          pthis_ctx->base_info.u64base = GST_ELEMENT_START_TIME(pthis_entry->ele_owner);
          if(!GST_CLOCK_TIME_IS_VALID(pthis_ctx->base_info.u64base)) {
            pthis_ctx->base_info.u64base = 0ULL;
          }
        }
        pthis_ctx->base_info.u64base += pthis_entry->segment.start;
        pthis_ctx->base_info.u64sys_time = cur_syst;

        if(pthis_ctx->base_info.reporter != pthis_entry->ele_owner){
          gst_object_unref(pthis_ctx->base_info.reporter);
          pthis_ctx->base_info.reporter = gst_object_ref(pthis_entry->ele_owner);
        }
        GST_WARNING("%s:refresh.baseinfo.base:%lld,offset:%lld",
          GST_SAFE_STRING(pthis_entry->pszowner_name),
          pthis_ctx->base_info.u64base,
          pthis_ctx->base_info.i64offset);
      }

      pthis_entry->u64calibratebase = pthis_ctx->base_info.u64base;
      pthis_entry->i64calibrateoffset = pthis_ctx->base_info.i64offset;
      GST_WARNING("%s:get.baseinfo.base:%llu,offset:%lld",
        GST_SAFE_STRING(pthis_entry->pszowner_name),
        pthis_entry->u64calibratebase,
        pthis_entry->i64calibrateoffset);
      g_rec_mutex_unlock(&pthis_ctx->rec_mutex);
      if(cur_clock){
        gst_object_unref(cur_clock);
      }
    }
  }

  g_rec_mutex_lock(&pthis_ctx->rec_mutex);
  if(!bneed_calibrate){
    if(GST_CLOCK_TIME_IS_VALID(pthis_ctx->base_info.u64base)
      && ((pthis_entry->u64calibratebase != pthis_ctx->base_info.u64base)
      || (pthis_entry->i64calibrateoffset != pthis_ctx->base_info.i64offset))){
      pthis_entry->i64calibrateoffset = pthis_ctx->base_info.i64offset;
      pthis_entry->u64calibratebase = pthis_ctx->base_info.u64base;
    }
  }
  g_rec_mutex_unlock(&pthis_ctx->rec_mutex);

  *pout_pts = in_pts;
  *pout_duration = in_duration;//TODO:duration need change ?
  pthis_entry->time_info.pts = in_pts;
  pthis_entry->time_info.duration = in_duration;
  pthis_entry->time_info.sys_time = cur_syst;
  pthis_entry->time_info.out_pts = *pout_pts;

  return bneed_calibrate;
}
//lint +e571

GstSmePTSCalibrateHDL
gst_sme_pts_calibration_create(IN const gchar* pszowner_name)
{
  GstSmePTSCalibrateCtx* pret = NULL;

  GST_INFO("gst_sme_pts_calibration_create in, name:%s",
      GST_SAFE_STRING(pszowner_name));

  pret = g_new0(GstSmePTSCalibrateCtx, 1);
  do{
    if(NULL == pret){
      GST_ERROR("new GstSmePTSCalibrateCtx failed!");
      break;
    }
    g_rec_mutex_init(&pret->rec_mutex);
    pret->pszowner_name = g_strdup(GST_SAFE_STRING(pszowner_name));
    if(!pret->pszowner_name){
      GST_WARNING("dump:%s failed !", GST_SAFE_STRING(pszowner_name));
    }
    pret->base_info.i64offset = -1LL;
    pret->base_info.u64base = GST_CLOCK_TIME_NONE;
    pret->base_info.u64sys_time = 0;
    pret->base_info.reporter = NULL;
  }while(FALSE);

  GST_INFO("gst_sme_pts_calibration_create out, name:%s",
      GST_SAFE_STRING(pszowner_name));

  return pret;
}

//lint -e438
void
gst_sme_pts_calibration_destroy(INOUT GstSmePTSCalibrateHDL* p2this)
{
  GstSmePTSCalibrateCtx** p2this_ctx = (GstSmePTSCalibrateCtx**)p2this;
  GstSmePTSCalibrateCtx* pthis_ctx = NULL;
  guint idx;

  GST_INFO("gst_sme_pts_calibration_destroy in, p2this_ctx:%p", p2this_ctx);

  if(NULL != p2this_ctx){
    GST_INFO("gst_sme_pts_calibration_destroy  *p2this_ctx:%p", *p2this_ctx);
    pthis_ctx = *p2this_ctx;
  }

  if(NULL != pthis_ctx){
    g_rec_mutex_lock(&pthis_ctx->rec_mutex);
    if(pthis_ctx->pszowner_name){
      GST_INFO("gst_sme_pts_calibration_destroy name:%s", pthis_ctx->pszowner_name);
      g_free(pthis_ctx->pszowner_name);
      pthis_ctx->pszowner_name = NULL;
    }
    for(idx = 0; idx < E_ENTRY_TYPE_BOTTOM; idx++){
      if(pthis_ctx->entry[idx].estate != E_ENTRY_STATE_IDLE){
        gst_sme_pts_calibration_entry_deinit(pthis_ctx, (EmEntryID)idx);
      }
    }
    gst_sme_pts_calibration_clear(pthis_ctx);
    g_rec_mutex_unlock(&pthis_ctx->rec_mutex);
    g_rec_mutex_clear(&pthis_ctx->rec_mutex);
    g_free(pthis_ctx);
    pthis_ctx = NULL;
  }

  GST_INFO("gst_sme_pts_calibration_destroy out");

  return;
}
//lint +e438

void
gst_sme_pts_calibration_clear(INOUT GstSmePTSCalibrateHDL pthis)
{
  GstSmePTSCalibrateCtx* pthis_ctx = (GstSmePTSCalibrateCtx*)pthis;

  GST_INFO("gst_sme_pts_calibration_clear in, pthis_ctx:%p", pthis_ctx);

  if(NULL != pthis_ctx){
    GST_INFO("gst_sme_pts_calibration_clear,name:%s",
      GST_SAFE_STRING(pthis_ctx->pszowner_name));
    g_rec_mutex_lock(&pthis_ctx->rec_mutex);
    pthis_ctx->base_info.i64offset = -1LL;
    pthis_ctx->base_info.u64base = GST_CLOCK_TIME_NONE;
    pthis_ctx->base_info.u64sys_time = 0;
    if(NULL != pthis_ctx->base_info.reporter){
      gst_object_unref(pthis_ctx->base_info.reporter);
      pthis_ctx->base_info.reporter = NULL;
    }
    g_rec_mutex_unlock(&pthis_ctx->rec_mutex);
  }

  GST_INFO("gst_sme_pts_calibration_clear out");

  return;
}

gboolean
gst_sme_pts_calibration_entry_init(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id, IN const gchar* pszowner_name, GstElement* ele_owner)
{
  gboolean bret = TRUE;
  GstSmePTSCalibrateCtx* pthis_ctx = (GstSmePTSCalibrateCtx*)pthis;
  EntryCtx* pthis_entry;
  E_SME_MEM_SECURE_RET osal_mem_err;

  GST_INFO("gst_sme_pts_calibration_entry_init in, id:%d, pthis_ctx:%p",
    eentry_id, pthis_ctx);

  if(NULL != pthis_ctx && eentry_id < E_ENTRY_TYPE_BOTTOM
    && eentry_id >= E_ENTRY_TYPE_AUDIO && NULL != ele_owner){

    GST_INFO("gst_sme_pts_calibration_entry_init,name:%s",
      GST_SAFE_STRING(pszowner_name));

    pthis_entry = &pthis_ctx->entry[eentry_id];
    if(E_ENTRY_STATE_IDLE != pthis_entry->estate){
      gst_sme_pts_calibration_entry_deinit(pthis_ctx, eentry_id);
    }

    g_rec_mutex_init(&pthis_entry->rec_mutex);
    pthis_entry->estate = E_ENTRY_STATE_INIT;
    pthis_entry->u64calibrate_num = 0;
    pthis_entry->u64calibratebase = 0ULL;
    pthis_entry->i64calibrateoffset = 0LL;
    gst_segment_init(&pthis_entry->segment, GST_FORMAT_TIME);
    pthis_entry->ele_owner = gst_object_ref(ele_owner);
    osal_mem_err = VOS_Memset_S(&pthis_entry->time_info, sizeof(pthis_entry->time_info),
      -1, sizeof(pthis_entry->time_info));
    if(E_SME_MEM_OK != osal_mem_err){
      GST_WARNING("VOS_Memset_S failed(%#x)", osal_mem_err);
    }
    pthis_entry->pszowner_name = g_strdup(GST_SAFE_STRING(pszowner_name));
    if(!pthis_entry->pszowner_name){
      GST_WARNING("dump:%s failed !", GST_SAFE_STRING(pszowner_name));
    }
    init_presync_info(&pthis_entry->presyncinfo);
  }else{
    GST_ERROR("invalid args !");
    bret = FALSE;
  }

  GST_INFO("gst_sme_pts_calibration_entry_init out, id:%d, pthis_ctx:%p, ret=%d",
    eentry_id, pthis_ctx, bret);

  return bret;
}

void
gst_sme_pts_calibration_entry_deinit(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id)
{
  GstSmePTSCalibrateCtx* pthis_ctx = (GstSmePTSCalibrateCtx*)pthis;
  EntryCtx* pthis_entry;
  E_SME_MEM_SECURE_RET osal_mem_err;

  GST_INFO("gst_sme_pts_calibration_entry_deinit in, id:%d, pthis_ctx:%p",
    eentry_id, pthis_ctx);

  if(NULL != pthis_ctx && eentry_id < E_ENTRY_TYPE_BOTTOM
    && eentry_id >= E_ENTRY_TYPE_AUDIO){
    pthis_entry = &pthis_ctx->entry[eentry_id];
    if(pthis_entry->estate != E_ENTRY_STATE_IDLE){
      g_rec_mutex_clear(&pthis_entry->rec_mutex);
      pthis_entry->estate = E_ENTRY_STATE_IDLE;
      pthis_entry->u64calibrate_num = 0;
      if(NULL != pthis_entry->ele_owner){
        gst_object_unref(pthis_entry->ele_owner);
        pthis_entry->ele_owner = NULL;
      }
      osal_mem_err = VOS_Memset_S(&pthis_entry->time_info, sizeof(pthis_entry->time_info),
        0, sizeof(pthis_entry->time_info));
      if(E_SME_MEM_OK != osal_mem_err){
        GST_WARNING("VOS_Memset_S failed(%#x)", osal_mem_err);
      }
      if(pthis_entry->pszowner_name){
        g_free(pthis_entry->pszowner_name);
        pthis_entry->pszowner_name = NULL;
      }
    }
  }

  GST_INFO("gst_sme_pts_calibration_entry_deinit out, id:%d, pthis_ctx:%p",
    eentry_id, pthis_ctx);

  return;
}

void
gst_sme_pts_calibration_entry_update_segment(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id, IN GstSegment segment)
{
  GstSmePTSCalibrateCtx* pthis_ctx = (GstSmePTSCalibrateCtx*)pthis;
  EntryCtx* pthis_entry;

  GST_INFO("gst_sme_pts_calibration_entry_update_segment in, entry_id:%d, "
    "pthis_ctx:%p, segment[%lld,%lld]",
    eentry_id, pthis_ctx, segment.start, segment.stop);

  if(NULL != pthis_ctx && eentry_id < E_ENTRY_TYPE_BOTTOM
    && eentry_id >= E_ENTRY_TYPE_AUDIO){
    pthis_entry = &pthis_ctx->entry[eentry_id];

    GST_INFO("gst_sme_pts_calibration_entry_update_segment, entry_id:%d, name:%s",
      eentry_id, GST_SAFE_STRING(pthis_entry->pszowner_name));
    if(pthis_entry->estate != E_ENTRY_STATE_IDLE){
      g_rec_mutex_lock(&pthis_entry->rec_mutex);
      gst_segment_copy_into((const GstSegment *)&segment, &pthis_entry->segment);
      g_rec_mutex_unlock(&pthis_entry->rec_mutex);
    }
  }

  GST_INFO("gst_sme_pts_calibration_entry_update_segment out, entry_id:%d, pthis_ctx:%p",
    eentry_id, pthis_ctx);

  return;
}

void
gst_sme_pts_calibration_entry_clear(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id)
{
  GstSmePTSCalibrateCtx* pthis_ctx = (GstSmePTSCalibrateCtx*)pthis;
  EntryCtx* pthis_entry;
  E_SME_MEM_SECURE_RET osal_mem_err;

  GST_INFO("gst_sme_pts_calibration_entry_clear in, entry_id:%d, pthis_ctx:%p",
    eentry_id, pthis_ctx);

  if(NULL != pthis_ctx && eentry_id < E_ENTRY_TYPE_BOTTOM
    && eentry_id >= E_ENTRY_TYPE_AUDIO){
    pthis_entry = &pthis_ctx->entry[eentry_id];

    GST_INFO("gst_sme_pts_calibration_entry_clear, entry_id:%d, name:%s",
      eentry_id, GST_SAFE_STRING(pthis_ctx->pszowner_name));

    g_rec_mutex_lock(&pthis_entry->rec_mutex);
    pthis_entry->estate = E_ENTRY_STATE_INIT;
    pthis_entry->u64calibrate_num = 0ULL;
    pthis_entry->u64calibratebase = 0ULL;
    pthis_entry->i64calibrateoffset = 0LL;
    gst_segment_init(&pthis_entry->segment, GST_FORMAT_TIME);
    osal_mem_err = VOS_Memset_S(&pthis_entry->time_info, sizeof(pthis_entry->time_info),
      -1, sizeof(pthis_entry->time_info));
    if(E_SME_MEM_OK != osal_mem_err){
      GST_WARNING("VOS_Memset_S failed(%#x)", osal_mem_err);
    }
    init_presync_info(&pthis_entry->presyncinfo);
    g_rec_mutex_unlock(&pthis_entry->rec_mutex);
  }

  GST_INFO("gst_sme_pts_calibration_entry_clear out, entry_id:%d, pthis_ctx:%p",
    eentry_id, pthis_ctx);

  return;
}

gboolean
gst_sme_pts_calibration_entry_execute(INOUT GstSmePTSCalibrateHDL pthis,
    IN EmEntryID eentry_id,
    IN GstClockTime in_pts, IN GstClockTime in_duration,
    IN GstClockTime* pout_pts, IN GstClockTime* pout_duration)
{
  gboolean bcalibrated = FALSE;
  GstSmePTSCalibrateCtx* pthis_ctx = (GstSmePTSCalibrateCtx*)pthis;
  GstClockTime res_pts = in_pts;
  GstClockTime res_duration = in_duration;
  EntryCtx* pthis_entry = NULL;

  do{
    if(NULL == pthis_ctx){
      GST_ERROR("gst_sme_pts_calibration_entry_execute failed null pointer !");
      break;
    }

    if(eentry_id >= E_ENTRY_TYPE_BOTTOM
      || eentry_id < E_ENTRY_TYPE_AUDIO){
      GST_ERROR("%s:gst_sme_pts_calibration_entry_execute!entry_id:%d",
        GST_SAFE_STRING(pthis_ctx->pszowner_name), eentry_id);
      break;
    }
    pthis_entry = &pthis_ctx->entry[eentry_id];

    g_rec_mutex_lock(&pthis_entry->rec_mutex);

    bcalibrated = check_and_calibrate(pthis_ctx, pthis_entry,
      in_pts, in_duration, &res_pts, &res_duration);

    g_rec_mutex_unlock(&pthis_entry->rec_mutex);
    if(res_pts + pthis_entry->u64calibratebase >=
      (GstClockTime)(ABS(pthis_entry->i64calibrateoffset))) {
      res_pts = (res_pts - (GstClockTime)pthis_entry->i64calibrateoffset) + pthis_entry->u64calibratebase;
    }else{
      GST_WARNING("calibration_entry_execute:after res_pts:%lld, offset:%lld,"
        "base:%lld", res_pts, pthis_entry->i64calibrateoffset,
        pthis_entry->u64calibratebase);
      res_pts = GST_CLOCK_TIME_NONE;
    }
  }while(FALSE);

  if(NULL != pout_pts){
    *pout_pts = res_pts;
  }

  if(NULL != pout_duration){
    *pout_duration = res_duration;
  }

  if(NULL != pthis_entry && bcalibrated){
    pthis_entry->u64calibrate_num++;

    GST_ERROR("name:%s,pts_calibration:in[%lld,%lld],out:[%lld,%lld]"
      ", calibratebase:%lld, calibrateoffset:%lld, calibrate_num:%lld",
      GST_SAFE_STRING(pthis_entry->pszowner_name),
      in_pts, in_duration, res_pts, res_duration,
      pthis_entry->u64calibratebase,
      pthis_entry->i64calibrateoffset,
      pthis_entry->u64calibrate_num);
  }

  return bcalibrated;
}

void
gst_sme_pts_calibration_entry_update_presync_info(INOUT GstSmePTSCalibrateHDL pthis,
  IN EmEntryID eentry_id, IN GstSlowPreSyncInfo presyncinfo)
{
  GstSmePTSCalibrateCtx* pthis_ctx = (GstSmePTSCalibrateCtx*)pthis;
  EntryCtx* pthis_entry;

  GST_INFO("gst_sme_pts_calibration_entry_update_presync_info in, entry_id:%d, "
    "pthis_ctx:%p, presyncinfo state:%d,rate:%f,last_presync_pts:%lld,stime:%lld",
    eentry_id, pthis_ctx, presyncinfo.slow_presync_state, presyncinfo.gdrate,
    presyncinfo.last_presync_pts, presyncinfo.slow_presync_stime);

  if(NULL != pthis_ctx && eentry_id < E_ENTRY_TYPE_BOTTOM
    && eentry_id >= E_ENTRY_TYPE_AUDIO){
    pthis_entry = &pthis_ctx->entry[eentry_id];

    GST_INFO("gst_sme_pts_calibration_entry_update_presync_info, entry_id:%d, name:%s",
      eentry_id, GST_SAFE_STRING(pthis_entry->pszowner_name));
    if(pthis_entry->estate != E_ENTRY_STATE_IDLE){
      g_rec_mutex_lock(&pthis_entry->rec_mutex);
      pthis_entry->presyncinfo = presyncinfo;
      g_rec_mutex_unlock(&pthis_entry->rec_mutex);
    }
  }

  GST_INFO("gst_sme_pts_calibration_entry_update_presync_info out, entry_id:%d, pthis_ctx:%p",
    eentry_id, pthis_ctx);
}
