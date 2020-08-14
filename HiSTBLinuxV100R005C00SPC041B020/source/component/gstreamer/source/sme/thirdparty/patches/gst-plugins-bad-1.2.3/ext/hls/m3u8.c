/* GStreamer
 * Copyright (C) 2010 Marc-Andre Lureau <marcandre.lureau@gmail.com>
 *
 * m3u8.c:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/*lint -e652*/  //already defined macro GST_CAT_DEFAULT
/*lint -e717*/  //do ... while (0)
/*lint -e736*/  //64bit to 32bit
/*lint -e747*/  //float to double
/*lint -e774*/  //condition always TRUE
/*lint -e775*/  //condition always larger than zero
/*lint -e794*/  //null pointer
/*lint -e801*/  //goto

#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <glib.h>
#include <string.h>

#include <gst/glib-compat-private.h>
#include "gstfragmented.h"
#include "m3u8.h"

#define GST_CAT_DEFAULT fragmented_debug

/* BEGIN: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
typedef enum {
  SYS_WINDOWS,
  SYS_UNIX,
  SYS_MAC
} Platform;
/* END: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
static GstM3U8 *gst_m3u8_new (void);
static void gst_m3u8_free (GstM3U8 * m3u8);
static gboolean gst_m3u8_update (GstM3U8 * m3u8, gchar * data,
    gboolean * updated);
static GstM3U8MediaFile *gst_m3u8_media_file_new (gchar * uri,
    gchar * title, GstClockTime duration, gint sequence);
static void gst_m3u8_media_file_free (GstM3U8MediaFile * self);
gchar *uri_join (const gchar * uri, const gchar * path);

static GstM3U8 *
gst_m3u8_new (void)
{
  GstM3U8 *m3u8;

  m3u8 = g_new0 (GstM3U8, 1);

  return m3u8;
}

static void
gst_m3u8_set_uri (GstM3U8 * self, gchar * uri, gchar * base_uri, gchar * name)
{
  g_return_if_fail (self != NULL);

  if (self->uri)
    g_free (self->uri);
  self->uri = uri;

  if (self->base_uri)
    g_free (self->base_uri);
  self->base_uri = base_uri;

  if (self->name)
    g_free (self->name);
  self->name = name;
}

/* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
static GstM3U8RenditionGroup *
gst_m3u8_rendition_group_new (void)
{
  GstM3U8RenditionGroup * group;

  group = g_new0(GstM3U8RenditionGroup, 1);

  return group;
}
/* END:   Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

static void
gst_m3u8_free (GstM3U8 * self)
{
  g_return_if_fail (self != NULL);

  g_free (self->uri);
  g_free (self->base_uri);
  g_free (self->name);
  g_free (self->allowcache);
  g_free (self->codecs);

  g_list_foreach (self->files, (GFunc) gst_m3u8_media_file_free, NULL);
  g_list_free (self->files);

  g_free (self->last_data);
  g_list_foreach (self->lists, (GFunc) gst_m3u8_free, NULL);
  g_list_free (self->lists);
  g_list_foreach (self->iframe_lists, (GFunc) gst_m3u8_free, NULL);
  g_list_free (self->iframe_lists);

  /* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
  if (self->group_audio) {
    g_free (self->group_audio->type);
    g_free (self->group_audio->group_id);
    g_list_foreach(self->group_audio->lists, (GFunc) gst_m3u8_free, NULL);
    g_free (self->group_audio);
  }

  g_free (self->audio_group_id);

  g_free (self->default_audio_uri);

  g_free (self->type);
  g_free (self->group_id);
  g_free (self->language);
  g_free (self->language_name);

  g_free (self->playtype);
  /* END:   Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

  /* BEGIN: Modified for DTS2015081807240 by wanghua(w00347020), 2015/8/19 */
  g_free (self->program_date_time);
  /* END: Modified for DTS2015081807240 by wanghua(w00347020), 2015/8/19 */
  g_free (self);
}

static GstM3U8MediaFile *
gst_m3u8_media_file_new (gchar * uri, gchar * title, GstClockTime duration,
    gint sequence)
{
  GstM3U8MediaFile *file;

  file = g_new0 (GstM3U8MediaFile, 1);
  file->uri = uri;
  file->title = title;
  file->duration = duration;
  file->sequence = sequence;

  return file;
}

static void
gst_m3u8_media_file_free (GstM3U8MediaFile * self)
{
  g_return_if_fail (self != NULL);

  g_free (self->title);
  g_free (self->uri);
  g_free (self->key);
  g_free (self->method);
  g_free (self->print_iv);
  g_free (self);
}

static gboolean
int_from_string (const gchar * ptr, gchar ** endptr, gint * val)
{
  gchar *end;
  gint64 ret;

  g_return_val_if_fail (ptr != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  errno = 0;
  ret = g_ascii_strtoll (ptr, &end, 10);
  if ((errno == ERANGE && (ret == G_MAXINT64 || ret == G_MININT64))
      || (errno != 0 && ret == 0)) {
    //GST_WARNING ("%s", g_strerror (errno));
    return FALSE;
  }

  if (ret > G_MAXINT || ret < G_MININT) {
    //GST_WARNING ("%s", g_strerror (ERANGE));
    return FALSE;
  }

  if (endptr)
    *endptr = end;

  *val = (gint) ret;

  return end != ptr;
}

static gboolean
int64_from_string (const gchar * ptr, gchar ** endptr, gint64 * val)
{
  gchar *end;
  gint64 ret;

  g_return_val_if_fail (ptr != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  errno = 0;
  ret = g_ascii_strtoll (ptr, &end, 10);
  if ((errno == ERANGE && (ret == G_MAXINT64 || ret == G_MININT64))
      || (errno != 0 && ret == 0)) {
    //GST_WARNING ("%s", g_strerror (errno));
    return FALSE;
  }

  if (endptr)
    *endptr = end;

  *val = ret;

  return end != ptr;
}

static gboolean
double_from_string (const gchar * ptr, gchar ** endptr, gdouble * val)
{
  gchar *end;
  gdouble ret;

  g_return_val_if_fail (ptr != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  errno = 0;
  ret = g_ascii_strtod (ptr, &end);
  if ((errno == ERANGE && (ret - HUGE_VAL == 0 || ret + HUGE_VAL == 0))
      || (errno != 0 && ret == 0)) {
    //GST_WARNING ("%s", g_strerror (errno));
    return FALSE;
  }

  if (!isfinite (ret)) {
    //GST_WARNING ("%s", g_strerror (ERANGE));
    return FALSE;
  }

  if (endptr)
    *endptr = end;

  *val = (gdouble) ret;

  return end != ptr;
}

static gboolean
parse_attributes (gchar ** ptr, gchar ** a, gchar ** v)
{
  gchar *end = NULL, *p;

  g_return_val_if_fail (ptr != NULL, FALSE);
  g_return_val_if_fail (*ptr != NULL, FALSE);
  g_return_val_if_fail (a != NULL, FALSE);
  g_return_val_if_fail (v != NULL, FALSE);

  /* [attribute=value,]* */

  *a = *ptr;
  end = p = g_utf8_strchr (*ptr, -1, ',');
  if (end) {
    gchar *q = g_utf8_strchr (*ptr, -1, '"');
    if (q && q < end) {
      /* special case, such as CODECS="avc1.77.30, mp4a.40.2" */
      q = g_utf8_next_char (q);
      if (q) {
        q = g_utf8_strchr (q, -1, '"');
      }
      if (q) {
        end = p = g_utf8_strchr (q, -1, ',');
      }
    }
  }
  if (end) {
    do {
      end = g_utf8_next_char (end);
    } while (end && *end == ' ');
    *p = '\0';
  }

  *v = p = g_utf8_strchr (*ptr, -1, '=');
  if (*v) {
    *v = g_utf8_next_char (*v);
    *p = '\0';
  } else {
    GST_WARNING ("missing = after attribute");
    return FALSE;
  }

  *ptr = end;
  return TRUE;
}

static gchar *
unquote_string (gchar * string)
{
  gchar *string_ret;

  string_ret = strchr (string, '"');
  if (string_ret != NULL) {
    /* found initialization quotation mark of string */
    string = string_ret + 1;
    string_ret = strchr (string, '"');
    if (string_ret != NULL) {
      /* found finalizing quotation mark of string */
      string_ret[0] = '\0';
    } else {
      GST_WARNING
          ("wrong string unqouting - cannot find finalizing quotation mark");
      return NULL;
    }
  }
  return string;
}

static gint
_m3u8_compare_uri (const GstM3U8 * a, const gchar * uri)
{
  g_return_val_if_fail (a != NULL, 0);
  g_return_val_if_fail (uri != NULL, 0);

  return g_strcmp0 (a->uri, uri);
}

static gint
_m3u8_compare_default (const GstM3U8 * a, const gpointer data)
{
  gpointer unused = data;
  g_return_val_if_fail (a != NULL, 0);

  unused = unused;

  return !(a->deft);
}

static gint
gst_m3u8_compare_playlist_by_bitrate (gconstpointer a, gconstpointer b)
{
  return ((GstM3U8 *) (a))->bandwidth - ((GstM3U8 *) (b))->bandwidth;
}

/* BEGIN: Modified for DTS2015072309865  by wanghua(w00347020), 2015/7/27 */
static void
_update_sequence (GstM3U8MediaFile * self, gint sequence)
{
  if (self) {
    self->sequence += sequence;
    if (self->sequence > MAX_SEQUENCE)
      self->sequence = (self->sequence - MAX_SEQUENCE - 1);

    if (self->sequence < 0)
      self->sequence = (self->sequence + MAX_SEQUENCE + 1);
  }
  GST_DEBUG ("self->sequence %d", self->sequence);
}
/* END: Modified for DTS2015072309865  by wanghua(w00347020), 2015/7/27 */

/*
 * @data: a m3u8 playlist text data, taking ownership
 */

/* BEGIN: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
static gboolean
_diff_platform (Platform sys_type)
{
  if (sys_type == SYS_WINDOWS || sys_type == SYS_UNIX)
    return FALSE;
  else
    return TRUE;
}
/* END: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
static void
_stream_info_print_log (GstM3U8 * self)
{
  g_return_val_if_fail (self != NULL, FALSE);

  if (self->program_id)
    GST_INFO ("m3u8 get PROGRAM-ID=%d", self->program_id);
  if (self->bandwidth)
    GST_INFO ("m3u8 get BANDWIDTH=%d", self->bandwidth);
  if (self->codecs)
    GST_INFO ("m3u8 get CODECS=%s", self->codecs);
  if (self->width && self->height)
    GST_INFO ("m3u8 get RESOLUTION=%d x %d", self->width, self->height);
  if (self->video)
    GST_INFO ("m3u8 get Multiple View Point, We don't support it currently.");
  if (self->audio_group_id)
    GST_INFO ("m3u8 get AUDIO=%s", self->audio_group_id);
  if (self->uri)
    GST_INFO ("m3u8 get STREAM-INF URI=%s", self->uri);
}

static void
_m3u8print_log (GstM3U8 * self)
{
  g_return_val_if_fail (self != NULL, FALSE);

#if !defined (SME_RLS)
  GList *list;
  GstM3U8 *m3u8;
  GstM3U8MediaFile *file;
  gboolean print_key = TRUE;

  GST_INFO ("m3u8 get #EXTM3U");

  if (self->playtype)
    GST_INFO ("m3u8 get #EXT-X-PLAYLIST-TYPE:%s", self->playtype);
  if (self->version)
    GST_INFO ("m3u8 get #EXT-X-VERSION:%d", self->version);
  if (self->targetduration)
    GST_INFO ("m3u8 get #EXT-X-TARGETDURATION:%f", (gdouble)self->targetduration/(gdouble)(GST_SECOND));
  if (self->mediaseq_flag)
    GST_INFO ("m3u8 get #EXT-X-MEDIA-SEQUENCE:%d", self->mediasequence);
  if (self->program_date_time)
    GST_INFO ("m3u8 get #EXT-X-PROGRAM-DATE-TIME %s", self->program_date_time);
  if (self->allowcache)
    GST_INFO ("m3u8 get #EXT-X-ALLOW-CACHE %s", self->allowcache);
  if (self->iframe_only)
    GST_INFO("m3u8 get #EXT-X-I-FRAMES-ONLY");
  if (self->endlist)
    GST_INFO ("m3u8 get #EXT-X-ENDLIST");

  if (self->lists)
  {
    list = self->lists;
    while(list)
    {
      GST_INFO ("m3u8 get #EXT-X-STREAM-INF");
      m3u8 = GST_M3U8 (list->data);
      _stream_info_print_log (m3u8);
      list = g_list_next (list);
    }
  }

  if (self->iframe_lists)
  {
    list = self->iframe_lists;
    while(list)
    {
      GST_INFO ("m3u8 get #EXT-X-I-FRAME-STREAM-INF");
      m3u8 = GST_M3U8 (list->data);
      _stream_info_print_log (m3u8);
      if (m3u8->uri)
        GST_INFO ("m3u8 get #EXT-X-I-FRAME-STREAM-INF:URI=%s", m3u8->uri);
      list = g_list_next (list);
    }
  }

  if (self->files)
  {
    list = g_list_last (self->files);

    while (list)
    {
      file = GST_M3U8_MEDIA_FILE (list->data);
      if (print_key)
      {
        if (file->key)
        {
          GST_INFO ("m3u8 get #EXT-X-KEY");
          GST_INFO ("m3u8 get #EXT-X-KEY:URI=%s", file->key);
        }
        if (file->method)
          GST_INFO ("m3u8 get METHOD:%s", file->method);
        if (file->print_iv)
          GST_INFO ("m3u8 get #EXT-X-KEY:IV=%s", file->print_iv);
        if (file->discont)
        GST_INFO ("m3u8 get #EXT-X-DISCONTINUITY");

        print_key = FALSE;
      }

      if (file->duration)
        GST_INFO ("m3u8 get #EXTINF: %f", (gdouble)file->duration/(gdouble)(GST_SECOND));
      if (file->title)
        GST_INFO ("m3u8 get title=%s", file->title);
      if ((-1 != file->size) && (0 != file->offset))
        GST_INFO ("m3u8 get #EXT-X-BYTERANGE:%lld@%lld", file->size, file->offset);
      if (file->uri)
        GST_INFO ("m3u8 get file uri=%s", file->uri);

      list = g_list_previous (list);
    }
  }

  if (self->group_audio)
  {
    if (self->group_audio->lists)
      list = self->group_audio->lists;
    while (list)
    {
      GST_INFO ("m3u8 get #EXT-X-MEDIA");

      m3u8 = GST_M3U8 (list->data);
      if (m3u8->type)
        GST_INFO ("m3u8 get TYPE=%s", m3u8->type);
      if (m3u8->group_id)
        GST_INFO ("m3u8 get GROUP-ID=%s", m3u8->group_id);
      if (m3u8->language_name)
        GST_INFO ("m3u8 get NAME=%s", m3u8->language_name);
      if (m3u8->autoselect)
        GST_INFO ("m3u8 get AUTOSELECT=YES");
      else
        GST_INFO ("m3u8 get AUTOSELECT=NO");
      if (m3u8->deft)
        GST_INFO ("m3u8 get DEFAULT=YES");
      else
        GST_INFO ("m3u8 get DEFAULT=NO");
      if (m3u8->language)
        GST_INFO ("m3u8 get LANGUAGE=%s", m3u8->language);
      if (m3u8->uri)
        GST_INFO ("m3u8 get URI=%s", m3u8->uri);

      list = g_list_next (list);
    }
  }
#else
  GST_INFO ("RLS Version, Prohibit Export Log Info !");
#endif
}

static gboolean
gst_m3u8_update (GstM3U8 * self, gchar * data, gboolean * updated)
{
  gint val;
  GstClockTime duration;
  gchar *title, *end, *r;
  gboolean discontinuity = FALSE;
  GstM3U8 *list;
  gchar *current_key = NULL;
  gchar *iv_log = NULL;
  gboolean have_iv = FALSE;
  guint8 iv[16] = { 0, };
  gchar *method = NULL;
  gint64 size = -1, offset = -1;
  gboolean first_segment_flag = FALSE;

  /* BEGIN: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
  Platform plat_type;
  gboolean isMAC;
  /* END: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
  /* BEGIN: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
  gboolean byterange_error = FALSE;
  /* END: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
  g_return_val_if_fail (self != NULL, FALSE);
  g_return_val_if_fail (data != NULL, FALSE);
  g_return_val_if_fail (updated != NULL, FALSE);

  *updated = TRUE;

  /* BEGIN: Modified for DTS2015072309865  by wanghua(w00347020), 2015/7/27 */
  self->filesequence = 0;
  /* END: Modified for DTS2015072309865  by wanghua(w00347020), 2015/7/27 */


  /* check if the data changed since last update */
  if (self->last_data && g_str_equal (self->last_data, data)) {
    GST_DEBUG ("Playlist is the same as previous one");
    *updated = FALSE;
    g_free (data);
    return TRUE;
  }

  if (!g_str_has_prefix (data, "#EXTM3U")) {
    GST_WARNING ("Data doesn't start with #EXTM3U");
    *updated = FALSE;
    g_free (data);
    return FALSE;
  }

  g_free (self->last_data);
  self->last_data = data;

  if (self->files) {
    g_list_foreach (self->files, (GFunc) gst_m3u8_media_file_free, NULL);
    g_list_free (self->files);
    self->files = NULL;
  }

  list = NULL;
  duration = 0;
  title = NULL;
  data += 7;
  while (TRUE) {

    end = g_utf8_strchr (data, -1, '\n');
    if (end)
      *end = '\0';

    r = g_utf8_strchr (data, -1, '\r');
    if (r)
      *r = '\0';

    /* BEGIN: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
    if (r && end)
      plat_type = SYS_WINDOWS;
    else if (r && !end)
      plat_type = SYS_MAC;
    else if (end && !r)
      plat_type = SYS_UNIX;
    else {
      if (list) {
        gst_m3u8_free (list);
        list = NULL;
        GST_INFO ("got line without line break, dropping");
      }
      break;
    }
    isMAC = _diff_platform (plat_type);
    /* END: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */

    if (data[0] != '#' && data[0] != '\0') {
      gchar *name = data;
      /* BEGIN: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
      if ((duration <= 0 && list == NULL) || (duration > 0 && byterange_error)) {
        //GST_LOG ("%s: got line without EXTINF or EXTSTREAMINF or BYTERANGE, dropping", data);
        byterange_error = FALSE;
        first_segment_flag = TRUE;
        goto next_line;
      }

      /* END: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
      data = uri_join (self->base_uri ? self->base_uri : self->uri, data);
      if (data == NULL)
        goto next_line;

      if (list != NULL) {
        if (g_list_find_custom (self->lists, data,
                (GCompareFunc) _m3u8_compare_uri)) {
          GST_DEBUG ("Already have a list with this URI");
          gst_m3u8_free (list);
          g_free (data);
        } else {
          gst_m3u8_set_uri (list, data, NULL, g_strdup (name));
          if (!list->audio_info) {
            self->lists = g_list_append (self->lists, list);
            GST_INFO ("m3u8 append list");
          } else {
            if (self->default_audio_uri)
              g_free (self->default_audio_uri);
            self->default_audio_uri = g_strdup (list->uri);

            if (self->group_audio) {
              GList *tmp_list = g_list_find_custom (
                  self->group_audio->lists, NULL, (GCompareFunc) _m3u8_compare_default);
              if (tmp_list) {
                if (GST_M3U8 (tmp_list->data)->uri)
                  g_free (GST_M3U8 (tmp_list->data)->uri);
                GST_M3U8 (tmp_list->data)->uri = g_strdup (self->default_audio_uri);
              }
            }

            gst_m3u8_free (list);
          }
        }
        list = NULL;
      } else {
        GstM3U8MediaFile *file;
        file =
            gst_m3u8_media_file_new (data, title, duration,
            self->filesequence++);

        /* set encryption params */
        file->key = current_key ? g_strdup (current_key) : NULL;
        file->method = method ? g_strdup (method) : NULL;
        file->print_iv = iv_log ? g_strdup (iv_log) : NULL;
        if (file->key) {
          if (have_iv) {
            memcpy (file->iv, iv, sizeof (iv));
          } else {
            guint8 *tmp_iv = file->iv + 12;
            GST_WRITE_UINT32_BE (tmp_iv, file->sequence);
          }
        }

        if (size != -1) {
          file->size = size;
          if (offset != -1) {
            file->offset = offset;
          } else {
            GstM3U8MediaFile *prev = self->files ? self->files->data : NULL;

            if (!prev) {
              offset = 0;
            } else {
              offset = prev->offset + prev->size;
            }
            file->offset = offset;
          }
        } else {
          file->size = -1;
          file->offset = 0;
        }

        file->discont = discontinuity;

        duration = 0;
        title = NULL;
        discontinuity = FALSE;
        size = offset = -1;
        self->files = g_list_prepend (self->files, file);
      }
      /* BEGIN: Modified for DTS2015111705873 by wanghua(w00347020), 2015/11/18 */
      if (self->iframe_only && first_segment_flag && !self->files)
        goto error;
      /* END: Modified for DTS2015111705873 by wanghua(w00347020), 2015/11/18 */
    } else if (g_str_has_prefix (data, "#EXTINF:")) {
      gdouble fval;
      if (!double_from_string (data + 8, &data, &fval)) {
        GST_WARNING ("Can't read EXTINF duration");
        goto next_line;
      }
      duration = (guint64) (fval * (gdouble) GST_SECOND);
      if (duration > self->targetduration)
        GST_WARNING ("EXTINF duration > TARGETDURATION");
      if (!data || *data == ',')
        goto next_line;
      data = g_utf8_next_char (data);
      /* BEGIN: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
      if (plat_type == SYS_WINDOWS && data == r)
        data = g_utf8_next_char (r);
      if ((!isMAC && data != end) || (isMAC && data != r)) {
        g_free (title);
        title = g_strdup (data);
      }
      /* END: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
    } else if (g_str_has_prefix (data, "#EXT-X-")) {
      gchar *data_ext_x = data + 7;

      /* All these entries start with #EXT-X- */
      if (g_str_has_prefix (data_ext_x, "ENDLIST")) {
        self->endlist = TRUE;
        /* BEGIN: Modified for DTS2015081807339  by wanghua(w00347020), 2015/8/19 */
        break;
        /* END: Modified for DTS2015081807339  by wanghua(w00347020), 2015/8/19 */
      } else if(g_str_has_prefix (data_ext_x, "PLAYLIST-TYPE:")) {
          /* BEGIN: Modified for DTS2015110206627  by wanghua(w00347020), 2015/11/16 */
          data = data + 21;
          /* END: Modified for DTS2015110206627  by wanghua(w00347020), 2015/11/16 */
          self->playtype = g_strdup (data);
      } else if (g_str_has_prefix (data_ext_x, "VERSION:")) {
        if (int_from_string (data + 15, &data, &val))
          self->version = val;
      } else if (g_str_has_prefix (data_ext_x, "STREAM-INF:") ||
          g_str_has_prefix (data_ext_x, "I-FRAME-STREAM-INF:")) {
        gchar *v, *a;
        gboolean iframe = g_str_has_prefix (data_ext_x, "I-FRAME-STREAM-INF:");
        GstM3U8 *new_list;

        new_list = gst_m3u8_new ();
        new_list->parent = self;
        new_list->iframe = iframe;
        data = data + (iframe ? 26 : 18);
        while (data && parse_attributes (&data, &a, &v)) {
          if (g_str_equal (a, "BANDWIDTH")) {
            if (!int_from_string (v, NULL, &new_list->bandwidth))
              GST_WARNING ("Error while reading BANDWIDTH");
          } else if (g_str_equal (a, "PROGRAM-ID")) {
            if (!int_from_string (v, NULL, &new_list->program_id))
              GST_WARNING ("Error while reading PROGRAM-ID");
          } else if (g_str_equal (a, "CODECS")) {
            g_free (new_list->codecs);
            new_list->codecs = g_strdup (v);
            if (!g_strstr_len(new_list->codecs, -1, ",")
                 && g_strstr_len(new_list->codecs, -1, "mp4a")) {
              //EXT-X-STREAM-INF that only contains audio
              new_list->audio_info = TRUE;
            }
          } else if (g_str_equal (a, "RESOLUTION")) {
            if (!int_from_string (v, &v, &new_list->width))
              GST_WARNING ("Error while reading RESOLUTION width");
            if (!v || *v != 'x') {
              GST_WARNING ("Missing height");
            } else {
              v = g_utf8_next_char (v);
              if (!int_from_string (v, NULL, &new_list->height))
                GST_WARNING ("Error while reading RESOLUTION height");
            }
          } else if (g_str_equal (a, "VIDEO")) {
            new_list->video = TRUE;
          } else if (g_str_equal (a, "AUDIO")) {
            g_free (new_list->audio_group_id);
            new_list->audio_group_id = g_strdup (v);
          } else if (iframe && g_str_equal (a, "URI")) {
            gchar *name;
            gchar *uri = g_strdup (v);

            uri = unquote_string (uri);
            if (uri) {
              uri = uri_join (self->base_uri ? self->base_uri : self->uri, uri);
              if (uri == NULL) {
                continue;
              }
              name = g_strdup (uri);

              gst_m3u8_set_uri (new_list, uri, NULL, name);
            } else {
              GST_WARNING
                  ("Cannot remove quotation marks from i-frame-stream URI");
            }
          }
        }

        if (iframe) {
          if (g_list_find_custom (self->iframe_lists, new_list->uri,
                  (GCompareFunc) _m3u8_compare_uri)) {
            GST_DEBUG ("Already have a list with this URI");
            gst_m3u8_free (new_list);
          } else {
            self->iframe_lists = g_list_append (self->iframe_lists, new_list);
          }
        } else if (list != NULL) {
          GST_WARNING ("Found a list without a uri..., dropping");
          gst_m3u8_free (list);
        } else {
          list = new_list;
        }
      } else if (g_str_has_prefix (data_ext_x, "TARGETDURATION:")) {
        if (int_from_string (data + 22, &data, &val)) {
          self->targetduration = (guint64) (val * (gdouble) GST_SECOND);
        }
      } else if (g_str_has_prefix (data_ext_x, "MEDIA-SEQUENCE:")) {
        gint len, negative_flag = 0;
        gchar * tem_data;
        gchar real_str[10];
        gboolean num_flag = 0;
        tem_data = data  + 22;
        if (*tem_data == '-')
          negative_flag = 1;

        if (negative_flag)
          len = strlen(tem_data) - 1;
        else
          len = strlen(tem_data);

        if (len <= 9)
          num_flag = int_from_string (data + 22, &data, &val);
        else {
          if (!negative_flag)
            strncpy (real_str, tem_data + (len - 9), 9);
          else
            strncpy (real_str, tem_data + (len - 9 + 1), 9);
          real_str[9] = '\0';
          num_flag = int_from_string (real_str, &tem_data, &val);
        }
        if (num_flag) {
          if (len > 9 && negative_flag)
            self->mediasequence = (0 - val);
          else
            self->mediasequence = val;
          self->mediaseq_flag = TRUE;
        }
        else
          return FALSE;
      } else if (g_str_has_prefix (data_ext_x, "DISCONTINUITY")) {
        discontinuity = TRUE;
      } else if (g_str_has_prefix (data_ext_x, "PROGRAM-DATE-TIME:")) {
        /* <YYYY-MM-DDThh:mm:ssZ> */
        /* BEGIN: Modified for DTS2015081807240 by wanghua(w00347020), 2015/8/19 */
        data = data + 25;
        self->program_date_time = g_strdup (data);
        /* END: Modified for DTS2015081807240 by wanghua(w00347020), 2015/8/19 */
      } else if (g_str_has_prefix (data_ext_x, "ALLOW-CACHE:")) {
        self->allowcache = g_strdup (data + 19);
      } else if (g_str_has_prefix (data_ext_x, "KEY:")) {
        gchar *v, *a;

        data = data + 11;

        /* IV and KEY are only valid until the next #EXT-X-KEY */
        have_iv = FALSE;
        g_free (current_key);
        current_key = NULL;
        g_free (method);
        method = NULL;
        g_free (iv_log);
        iv_log = NULL;
        while (data && parse_attributes (&data, &a, &v)) {
          if (g_str_equal (a, "URI")) {
            gchar *key = g_strdup (v);

            key = unquote_string (key);
            if (key) {
              current_key =
                  uri_join (self->base_uri ? self->base_uri : self->uri, key);
            } else {
              GST_WARNING
                  ("Cannot remove quotation marks from decryption key URI");
            }
          } else if (g_str_equal (a, "IV")) {
            gchar *ivp = v;
            gint i;

            iv_log = g_strdup (v);
            if (strlen (ivp) < 32 + 2 || (!g_str_has_prefix (ivp, "0x")
                    && !g_str_has_prefix (ivp, "0X"))) {
              GST_WARNING ("Can't read IV");
              continue;
            }

            ivp += 2;
            for (i = 0; i < 16; i++) {
              gint h, l;

              h = g_ascii_xdigit_value (*ivp);
              ivp++;
              l = g_ascii_xdigit_value (*ivp);
              ivp++;
              if (h == -1 || l == -1) {
                i = -1;
                break;
              }
              iv[i] = (guint8)(((guint)h << 4) | (guint)l);
            }

            if (i == -1) {
              GST_WARNING ("Can't read IV");
              continue;
            }
            have_iv = TRUE;
          } else if (g_str_equal (a, "METHOD")) {
            method = g_strdup (v);
            if (!g_str_equal (v, "AES-128")) {
              GST_WARNING ("Encryption method not supported");
              continue;
            }
          }
        }
      } else if (g_str_has_prefix (data_ext_x, "BYTERANGE:")) {
        gchar *v = data + 17;

        if (int64_from_string (v, &v, &size)) {
          /* BEGIN: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
          if (size < 0)
            goto error;
          if (*v == '@' && !int64_from_string (v + 1, &v, &offset))
            goto next_line;
        } else
          goto error;
          /* END: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
      } else if (g_str_has_prefix (data_ext_x, "MEDIA:")) {
        gchar *v, *a;
        GstM3U8 *m3u8 = NULL;
        gchar *uri;
        data = data + 13;
        m3u8 = gst_m3u8_new ();
        gint len = 0;

        while (data && parse_attributes (&data, &a, &v)) {
          if (g_str_equal (a, "TYPE")) {
            m3u8->type = g_strdup (v);
          } else if (g_str_equal (a, "GROUP-ID")) {
            m3u8->group_id = g_strdup (v);
            v = unquote_string (v);
            /* BEGIN: Modified for DTS2016062503992, 2016/6/25 */
            if (v)
              len = (gint) strlen (v);
            else
              goto error;
            /* END: Modified for DTS2016062503992, 2016/6/25 */
          } else if (g_str_equal (a, "NAME")) {
            m3u8->language_name = g_strdup (v);
          } else if (g_str_equal (a, "AUTOSELECT")) {
            m3u8->autoselect = g_ascii_strcasecmp (v, "YES") == 0;
          } else if (g_str_equal (a, "DEFAULT")) {
            m3u8->deft = g_ascii_strcasecmp (v, "YES") == 0;
          } else if(g_str_equal (a, "LANGUAGE")) {
            m3u8->language = g_strdup (v);
          } else if (g_str_equal (a, "URI")) {
            uri = g_strdup (v);
            uri = unquote_string (uri);
            uri = uri_join (self->base_uri ? self->base_uri : self->uri, uri);
            gst_m3u8_set_uri (m3u8, uri, NULL, NULL);
          } else
            GST_LOG ("Ignored line:");
        }

        /* BEGIN: Modified for DTS2015082904366  by wanghua(w00347020), 2015/9/1 */
        if (m3u8->type && g_str_equal (m3u8->type, "AUDIO") &&
           len != 0) {
          if (!self->group_audio) {
            self->group_audio = gst_m3u8_rendition_group_new ();
            self->group_audio->group_id = g_strdup(m3u8->group_id);
            self->group_audio->type = g_strdup(m3u8->type);
          }
          self->group_audio->lists = g_list_append (self->group_audio->lists, m3u8);
        } else
          gst_m3u8_free (m3u8);
        /* END: Modified for DTS2015082904366  by wanghua(w00347020), 2015/9/1 */
      } /* BEGIN: Modified for DTS2015081806976 by wanghua(w00347020), 2015/8/20 */
        else if(g_str_has_prefix (data_ext_x, "I-FRAMES-ONLY")) {
        /* BEGIN: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
        self->iframe_only = TRUE;
        /* END: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
      } /* END: Modified for DTS2015081806976 by wanghua(w00347020), 2015/8/20 */
      else {
        /* BEGIN: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
        byterange_error = TRUE;
        /* END: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
        GST_LOG ("Ignored line:");
      }
    } else {
      GST_LOG ("Ignored line:");
    }

  next_line:
    /* BEGIN: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
    if ((!isMAC && !end) || (isMAC && !r))
      break;
    if (isMAC) {
      if (r)
        data = g_utf8_next_char (r);  /* skip \n */
    }
    else {
      if (end)
        data = g_utf8_next_char (end);
    }
    /* END: Modified for DTS2015082904435 by wanghua(w00347020), 2015/8/29 */
 }

  _m3u8print_log (self);
  g_free (current_key);
  current_key = NULL;
  g_free (method);
  method = NULL;
  g_free (iv_log);
  iv_log = NULL;

  self->files = g_list_reverse (self->files);

  /* BEGIN: Modified for DTS2015072309865  by wanghua(w00347020), 2015/7/27 */
  if (self->mediasequence) {
    g_list_foreach (self->files, (GFunc) _update_sequence, (gpointer) self->mediasequence);
  }
  /* END: Modified for DTS2015072309865  by wanghua(w00347020), 2015/7/27 */

  /* reorder playlists by bitrate */
  if (self->lists) {
    gchar *top_variant_uri = NULL;
    gboolean iframe = FALSE;

    /*BEGIN: Modified for SME-107 by wanghua(w00347020), 2015/8/4 */
    self->lists =
        g_list_sort (self->lists,
        (GCompareFunc) gst_m3u8_compare_playlist_by_bitrate);

    if (!self->current_variant) {
      top_variant_uri = GST_M3U8 (self->lists->data)->uri;
    } else {
      top_variant_uri = GST_M3U8 (self->current_variant->data)->uri;
      iframe = GST_M3U8 (self->current_variant->data)->iframe;
    }
    /*END: Modified for SME-107 by wanghua(w00347020), 2015/8/4 */

    self->iframe_lists =
        g_list_sort (self->iframe_lists,
        (GCompareFunc) gst_m3u8_compare_playlist_by_bitrate);

    if (iframe)
      self->current_variant =
          g_list_find_custom (self->iframe_lists, top_variant_uri,
          (GCompareFunc) _m3u8_compare_uri);
    else
      self->current_variant = g_list_find_custom (self->lists, top_variant_uri,
          (GCompareFunc) _m3u8_compare_uri);
  }

  return TRUE;
  /* BEGIN: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
  error:
  {
    GST_WARNING ("error happened, stop update");
    g_free (current_key);
    current_key = NULL;
    g_free (method);
    method = NULL;
    g_free (iv_log);
    iv_log = NULL;
    return FALSE;
  }
  /* END: Modified for DTS2015102608347  by wanghua(w00347020), 2015/10/27 */
}

GstM3U8Client *
gst_m3u8_client_new (const gchar * uri, const gchar * base_uri)
{
  GstM3U8Client *client;

  g_return_val_if_fail (uri != NULL, NULL);

  client = g_new0 (GstM3U8Client, 1);
  client->main = gst_m3u8_new ();
  client->current = NULL;
  client->sequence = -1;
  client->update_failed_count = 0;
  client->lock = g_mutex_new ();
  gst_m3u8_set_uri (client->main, g_strdup (uri), g_strdup (base_uri), NULL);

  client->current_audio = NULL;
  client->sequence_audio = -1;

  /* BEGIN: Modified for DTS2015110206627 by lizimian(l00251625), 2015/11/2 */
  client->duration_video = GST_CLOCK_TIME_NONE;
  client->duration_audio = GST_CLOCK_TIME_NONE;
  /* END: Modified for DTS2015110206627 by lizimian(l00251625), 2015/11/2 */

  return client;
}

void
gst_m3u8_client_free (GstM3U8Client * self)
{
  g_return_if_fail (self != NULL);

  gst_m3u8_free (self->main);
  g_mutex_free (self->lock);
  g_free (self);
}

void
gst_m3u8_client_set_current (GstM3U8Client * self, GstM3U8 * m3u8, gboolean video)
{
  g_return_if_fail (self != NULL);

  GST_M3U8_CLIENT_LOCK (self);

  if (video && m3u8 != self->current) {
    self->current = m3u8;
    self->update_failed_count = 0;
  } else if (!video && m3u8 != self->current_audio) {
    self->current_audio = m3u8;
    self->update_failed_count = 0;
  }

  GST_M3U8_CLIENT_UNLOCK (self);
}

/* BEGIN: Modified for DTS2015082904308 by wanghua(w00347020), 2015/9/1 */
static void
_free_group_audio_id (GstM3U8 *list, gpointer * data)
{
  (void) data;
  if (list->audio_group_id) {
    g_free (list->audio_group_id);
    list->audio_group_id = NULL;
  }
}
/* END: Modified for DTS2015082904308 by wanghua(w00347020), 2015/9/1 */

gboolean
gst_m3u8_client_update (GstM3U8Client * self, gchar * data, gboolean video)
{
  GstM3U8 *m3u8;
  gboolean updated = FALSE;
  gboolean ret = FALSE;

  g_return_val_if_fail (self != NULL, FALSE);

  GST_M3U8_CLIENT_LOCK (self);

  /* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
  if (video)
    m3u8 = self->current ? self->current : self->main;
  else
    m3u8 = self->current_audio ? self->current_audio: self->main;
  /* END:   Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

  if (!gst_m3u8_update (m3u8, data, &updated))
    goto out;

  /* BEGIN: Modified for DTS2015082904308 by wanghua(w00347020), 2015/9/1 */
  if (m3u8 == self->main) {
    if (!m3u8->group_audio && m3u8->lists) {
      GList *tmp_lists;
      tmp_lists = m3u8->lists;
      g_list_foreach (tmp_lists, (GFunc) _free_group_audio_id, NULL);
    }
  }
  /* END: Modified for DTS2015082904308 by wanghua(w00347020), 2015/9/1 */
  if (!updated) {
    self->update_failed_count++;
    goto out;
  }

  /* select the first playlist, for now */
  if (!self->current) {
    if (self->main->lists) {
      self->current = self->main->current_variant->data;
    } else {
      self->current = self->main;
    }
  }

  /* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
  /* update current audio playlist for multi-track for version 4 */
  if (!self->current_audio) {
    self->current_audio = m3u8->group_audio ?
         g_list_first (m3u8->group_audio->lists)->data : NULL;
  }

  if (m3u8->files && video && self->sequence == -1) {
    self->sequence =
        GST_M3U8_MEDIA_FILE (g_list_first (m3u8->files)->data)->sequence;
    GST_DEBUG ("Setting first sequence at %d", self->sequence);

    /* BEGIN: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
    self->first_fragment_sequence = self->sequence;
    /* END: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
  }

  if (m3u8->files && !video && self->sequence_audio == -1) {
    self->sequence_audio =
        GST_M3U8_MEDIA_FILE (g_list_first (m3u8->files)->data)->sequence;
    GST_DEBUG ("Setting first audio sequence at %d", self->sequence_audio);
  }
  /* END:   Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

  ret = TRUE;
out:
  GST_M3U8_CLIENT_UNLOCK (self);
  return ret;
}

gboolean
_find_max_sequence (const GstM3U8MediaFile * file, gint max_sequence)
{
  g_return_val_if_fail (file != NULL, 0);

  if (file->sequence == max_sequence)
    return FALSE;
  else
    return TRUE;
}

static gboolean
_find_next (const GstM3U8MediaFile * file, const GstM3U8Client * client)
{
  GST_DEBUG ("Found fragment %d", file->sequence);
  GList * is_reversal;
  is_reversal = g_list_find_custom (client->current->files, MAX_SEQUENCE, _find_max_sequence);
  if (is_reversal) {
    if (file->sequence == client->sequence)
      return FALSE;
  }
  else if (file->sequence >= client->sequence)
    return FALSE;
  return TRUE;
}

/* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
static gboolean
_find_next_audio (const GstM3U8MediaFile * file, const GstM3U8Client * client)
{
  GST_DEBUG ("Found fragment %d", file->sequence);
  if (file->sequence >= client->sequence_audio)
    return FALSE;
  return TRUE;
}
/* END:   Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

void
gst_m3u8_client_get_current_position (const GstM3U8Client * client,
    GstClockTime * timestamp, gboolean video)
{
  GList *l;
  GList *walk;

  if (video) {
      l = g_list_find_custom (client->current->files, client,
          (GCompareFunc) _find_next);

      *timestamp = 0;
      for (walk = client->current->files; walk; walk = walk->next) {
        if (walk == l)
          break;
        *timestamp += GST_M3U8_MEDIA_FILE (walk->data)->duration;
      }
  } else {
      l = g_list_find_custom (client->current_audio->files, client,
          (GCompareFunc) _find_next_audio);

      *timestamp = 0;
      for (walk = client->current_audio->files; walk; walk = walk->next) {
        if (walk == l)
          break;
        *timestamp += GST_M3U8_MEDIA_FILE (walk->data)->duration;
      }
  }
}

gboolean
gst_m3u8_client_get_next_fragment (GstM3U8Client * client,
    gboolean * discontinuity, gchar ** uri, GstClockTime * duration,
    GstClockTime * timestamp, gint64 * range_start, gint64 * range_end,
    gchar ** key, guint8 ** iv, gboolean forward, gboolean video)
{
  GList *l;
  GstM3U8MediaFile *file;

  g_return_val_if_fail (client != NULL, FALSE);
  g_return_val_if_fail (client->current != NULL, FALSE);
  g_return_val_if_fail (discontinuity != NULL, FALSE);

  GST_M3U8_CLIENT_LOCK (client);
  GST_DEBUG ("Looking for fragment %d", client->sequence);

  if (forward)
    GST_DEBUG ("Get next fragment in forward direction");

  if (video)
    l = g_list_find_custom (client->current->files, client,
      (GCompareFunc) _find_next);
  else
    l = g_list_find_custom (client->current_audio->files, client,
      (GCompareFunc) _find_next_audio);

  if (l == NULL) {
    GST_M3U8_CLIENT_UNLOCK (client);
    return FALSE;
   }

  gst_m3u8_client_get_current_position (client, timestamp, video);

  file = GST_M3U8_MEDIA_FILE (l->data);

  /* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
  if (video) {
    *discontinuity = client->sequence != file->sequence;
    client->sequence = file->sequence + 1;
    if (file->sequence == MAX_SEQUENCE)
      client->sequence = 0;
  } else {
    *discontinuity = client->sequence_audio != file->sequence;
    client->sequence_audio = file->sequence + 1;
  }
  /* END:   Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

  *uri = file->uri;
  *duration = file->duration;
  *key = file->key;
  *iv = file->iv;

  if (range_start)
    *range_start = file->offset;
  if (range_end)
    *range_end = file->size != -1 ? file->offset + file->size - 1 : -1;

  GST_M3U8_CLIENT_UNLOCK (client);
  return TRUE;
}

static void
_sum_duration (const GstM3U8MediaFile * self, GstClockTime * duration)
{
  *duration += self->duration;
}

GstClockTime
gst_m3u8_client_get_duration (const GstM3U8Client * client, gboolean video)
{
  GstClockTime duration = GST_CLOCK_TIME_NONE;
  gboolean vod = TRUE;

  g_return_val_if_fail (client != NULL, GST_CLOCK_TIME_NONE);
  g_return_val_if_fail (client->current != NULL, GST_CLOCK_TIME_NONE);

  GST_M3U8_CLIENT_LOCK (client);

  if (client->current->playtype) {
    if (g_str_equal (client->current->playtype, "VOD"))
      vod = TRUE;
    else if (g_str_equal (client->current->playtype, "EVENT"))
      vod = FALSE;
  }
  /* We can only get the duration for on-demand streams */
  if (video) {
    /* BEGIN: Modified for DTS2015110206627  by wanghua(w00347020), 2015/11/16 */
    if ((!client->current->endlist && !client->current->playtype)
        || (client->current->playtype && !vod)) {
    /* END: Modified for DTS2015110206627  by wanghua(w00347020), 2015/11/16 */
      GST_M3U8_CLIENT_UNLOCK (client);
      return GST_CLOCK_TIME_NONE;
    }

    g_list_foreach (client->current->files, (GFunc) _sum_duration, &duration);
  } else {
      if (!client->current_audio || !client->current_audio->endlist) {
      GST_M3U8_CLIENT_UNLOCK (client);
      return GST_CLOCK_TIME_NONE;
    }

    g_list_foreach (client->current_audio->files, (GFunc) _sum_duration, &duration);
  }
  GST_M3U8_CLIENT_UNLOCK (client);

  return duration;
}

GstClockTime
gst_m3u8_client_get_target_duration (const GstM3U8Client * client, gboolean video)
{
  GstClockTime duration = 0;

  g_return_val_if_fail (client != NULL, GST_CLOCK_TIME_NONE);

  GST_M3U8_CLIENT_LOCK (client);

  if (video)
    duration = client->current->targetduration;
  else
    duration = client->current_audio->targetduration;

  GST_M3U8_CLIENT_UNLOCK (client);
  return duration;
}

const gchar *
gst_m3u8_client_get_uri (const GstM3U8Client * client)
{
  const gchar *uri;

  g_return_val_if_fail (client != NULL, NULL);

  GST_M3U8_CLIENT_LOCK (client);
  uri = client->main ? g_strdup (client->main->uri) : NULL;
  GST_M3U8_CLIENT_UNLOCK (client);
  return uri;
}

const gchar *
gst_m3u8_client_get_current_uri (const GstM3U8Client * client, gboolean video)
{
  const gchar *uri;

  g_return_val_if_fail (client != NULL, NULL);

  GST_M3U8_CLIENT_LOCK (client);
  if (video)
    uri = client->current->uri;
  else
    uri = client->current_audio->uri;
  GST_M3U8_CLIENT_UNLOCK (client);

  return uri;
}

gboolean
gst_m3u8_client_has_variant_playlist (const GstM3U8Client * client)
{
  gboolean ret;

  g_return_val_if_fail (client != NULL, FALSE);

  GST_M3U8_CLIENT_LOCK (client);
  ret = (client->main->lists != NULL);
  GST_M3U8_CLIENT_UNLOCK (client);

  return ret;
}

gboolean
gst_m3u8_client_is_live (const GstM3U8Client * client)
{
  gboolean ret;
  gboolean vod = FALSE;

  g_return_val_if_fail (client != NULL, FALSE);
  g_return_val_if_fail (client->current != NULL, FALSE);

  GST_M3U8_CLIENT_LOCK (client);

  if (client->current->playtype) {
    if (g_str_equal (client->current->playtype, "VOD"))
      vod = TRUE;
    else if (g_str_equal (client->current->playtype, "EVENT"))
      vod = FALSE;
  }

  if (client->current->endlist || (client->current->playtype && vod))
    ret = FALSE;
  else
    ret = TRUE;

  GST_M3U8_CLIENT_UNLOCK (client);

  return ret;
}

GList *
gst_m3u8_client_get_playlist_for_bitrate (const GstM3U8Client * client,
    guint64 bitrate, gboolean video)
{
  GList *list, *current_variant;

  GST_M3U8_CLIENT_LOCK (client);
  if (video) {
    current_variant = client->main->current_variant;

    /* Go to the highest possible bandwidth allowed */
    while ((guint64) (GST_M3U8 (current_variant->data)->bandwidth) < bitrate) {
      list = g_list_next (current_variant);
      if (!list)
        break;
      current_variant = list;
    }

    while ((guint64) (GST_M3U8 (current_variant->data)->bandwidth) > bitrate) {
      list = g_list_previous (current_variant);
      if (!list)
        break;
      current_variant = list;
    }
  } else {
    current_variant = g_list_first (client->main->group_audio->lists);
  }
  GST_M3U8_CLIENT_UNLOCK (client);

  return current_variant;
}

gchar *
uri_join (const gchar * uri1, const gchar * uri2)
{
  gchar *uri_copy, *tmp, *ret = NULL;

  if (!uri1 || !uri2)
    return NULL;

  if (gst_uri_is_valid (uri2))
    return g_strdup (uri2);

  uri_copy = g_strdup (uri1);
  if (uri2[0] != '/') {
    /* uri2 is a relative uri2 */
    /* look for query params */
    tmp = g_utf8_strchr (uri_copy, -1, '?');
    if (tmp) {
      /* find last / char, ignoring query params */
      tmp = g_utf8_strrchr (uri_copy, tmp - uri_copy, '/');
    } else {
      /* find last / char in URL */
      tmp = g_utf8_strrchr (uri_copy, -1, '/');
    }
    if (!tmp) {
      GST_WARNING ("Can't build a valid uri_copy");
      goto out;
    }

    *tmp = '\0';
    ret = g_strdup_printf ("%s/%s", uri_copy, uri2);
  } else {
    /* uri2 is an absolute uri2 */
    gchar *scheme, *hostname;
    gchar *uri2_copy;

    scheme = uri_copy;
    uri2_copy = g_utf8_strrchr (uri2, -1, '/');
    /* find the : in <scheme>:// */
    tmp = g_utf8_strchr (uri_copy, -1, ':');
    if (!tmp) {
      GST_WARNING ("Can't build a valid uri_copy");
      goto out;
    }

    *tmp = '\0';

    /* skip :// */
    hostname = tmp + 3;

    tmp = g_utf8_strrchr (hostname, -1, '/');
    if (tmp)
      *tmp = '\0';

    ret = g_strdup_printf ("%s://%s%s", scheme, hostname, uri2_copy);
  }

out:
  g_free (uri_copy);
  return ret;
}
