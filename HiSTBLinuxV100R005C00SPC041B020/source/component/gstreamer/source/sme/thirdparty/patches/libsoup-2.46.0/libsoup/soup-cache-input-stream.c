/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2012 Igalia, S.L.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>
#include "soup-cache-input-stream.h"
#include "soup-message-body.h"

static void soup_cache_input_stream_pollable_init (GPollableInputStreamInterface *pollable_interface, gpointer interface_data);

G_DEFINE_TYPE_WITH_CODE (SoupCacheInputStream, soup_cache_input_stream, SOUP_TYPE_FILTER_INPUT_STREAM,
		 G_IMPLEMENT_INTERFACE (G_TYPE_POLLABLE_INPUT_STREAM,
		soup_cache_input_stream_pollable_init))

/* properties */
enum {
	PROP_0,

	PROP_OUTPUT_STREAM,

	LAST_PROP
};

enum {
	CACHING_FINISHED,

	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

struct _SoupCacheInputStreamPrivate
{
	GOutputStream *output_stream;
	GCancellable *cancellable;
	gsize bytes_written;

	gboolean read_finished;
	SoupBuffer *current_writing_buffer;
	GQueue *buffer_queue;
};

static void soup_cache_input_stream_write_next_buffer (SoupCacheInputStream *istream);

static inline void
notify_and_clear (SoupCacheInputStream *istream, GError *error)
{
	SoupCacheInputStreamPrivate *priv = istream->priv;

	g_signal_emit (istream, signals[CACHING_FINISHED], 0, priv->bytes_written, error);

	g_clear_object (&priv->cancellable);
	g_clear_object (&priv->output_stream);
	g_clear_error (&error);
}

static inline void
try_write_next_buffer (SoupCacheInputStream *istream)
{
	SoupCacheInputStreamPrivate *priv = istream->priv;

	if (priv->current_writing_buffer == NULL && priv->buffer_queue->length)
		soup_cache_input_stream_write_next_buffer (istream);
	else if (priv->read_finished)
		notify_and_clear (istream, NULL);
	else if (g_input_stream_is_closed (G_INPUT_STREAM (istream))) {
		GError *error = NULL;
		g_set_error_literal (&error, G_IO_ERROR, G_IO_ERROR_CLOSED,
		     _("Network stream unexpectedly closed"));
		notify_and_clear (istream, error);
	}
}

static void
file_replaced_cb (GObject      *source,
		  GAsyncResult *res,
		  gpointer      user_data)
{
	SoupCacheInputStream *istream = SOUP_CACHE_INPUT_STREAM (user_data);
	SoupCacheInputStreamPrivate *priv = istream->priv;
	GError *error = NULL;

	priv->output_stream = (GOutputStream *) g_file_replace_finish (G_FILE (source), res, &error);

	if (error)
		notify_and_clear (istream, error);
	else
		try_write_next_buffer (istream);

	g_object_unref (istream);
}

static void
soup_cache_input_stream_init (SoupCacheInputStream *self)
{
	SoupCacheInputStreamPrivate *priv =
		G_TYPE_INSTANCE_GET_PRIVATE (self, SOUP_TYPE_CACHE_INPUT_STREAM,
		     SoupCacheInputStreamPrivate);

	priv->buffer_queue = g_queue_new ();
	self->priv = priv;
}

static void
soup_cache_input_stream_get_property (GObject *object,
		      guint property_id, GValue *value, GParamSpec *pspec)
{
	SoupCacheInputStream *self = SOUP_CACHE_INPUT_STREAM (object);
	SoupCacheInputStreamPrivate *priv = self->priv;

	switch (property_id) {
	case PROP_OUTPUT_STREAM:
		g_value_set_object (value, priv->output_stream);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
soup_cache_input_stream_set_property (GObject *object,
		      guint property_id, const GValue *value, GParamSpec *pspec)
{
	SoupCacheInputStream *self = SOUP_CACHE_INPUT_STREAM (object);
	SoupCacheInputStreamPrivate *priv = self->priv;

	switch (property_id) {
	case PROP_OUTPUT_STREAM:
		priv->output_stream = g_value_dup_object (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
soup_cache_input_stream_finalize (GObject *object)
{
	SoupCacheInputStream *self = (SoupCacheInputStream *)object;
	SoupCacheInputStreamPrivate *priv = self->priv;

	g_clear_object (&priv->cancellable);
	g_clear_object (&priv->output_stream);
	g_clear_pointer (&priv->current_writing_buffer, soup_buffer_free);
	g_queue_free_full (priv->buffer_queue, (GDestroyNotify) soup_buffer_free);

	G_OBJECT_CLASS (soup_cache_input_stream_parent_class)->finalize (object);
}

static void
write_ready_cb (GObject *source, GAsyncResult *result, SoupCacheInputStream *istream)
{
	GOutputStream *ostream = G_OUTPUT_STREAM (source);
	SoupCacheInputStreamPrivate *priv = istream->priv;
	gssize write_size;
	gsize pending;
	GError *error = NULL;

	write_size = g_output_stream_write_finish (ostream, result, &error);
	if (error) {
		notify_and_clear (istream, error);
		g_object_unref (istream);
		return;
	}

	/* Check that we have written everything */
	pending = priv->current_writing_buffer->length - write_size;
	if (pending) {
		SoupBuffer *subbuffer = soup_buffer_new_subbuffer (priv->current_writing_buffer,
		   write_size, pending);
		g_queue_push_head (priv->buffer_queue, subbuffer);
	}

	priv->bytes_written += write_size;
	g_clear_pointer (&priv->current_writing_buffer, soup_buffer_free);

	try_write_next_buffer (istream);
	g_object_unref (istream);
}

static void
soup_cache_input_stream_write_next_buffer (SoupCacheInputStream *istream)
{
	SoupCacheInputStreamPrivate *priv = istream->priv;
	SoupBuffer *buffer = g_queue_pop_head (priv->buffer_queue);
	int priority;

	g_assert (priv->output_stream && !g_output_stream_is_closed (priv->output_stream));

	g_clear_pointer (&priv->current_writing_buffer, soup_buffer_free);
	priv->current_writing_buffer = buffer;

	if (priv->buffer_queue->length > 10)
		priority = G_PRIORITY_DEFAULT;
	else
		priority = G_PRIORITY_LOW;

	g_output_stream_write_async (priv->output_stream, buffer->data, buffer->length,
		     priority, priv->cancellable,
		     (GAsyncReadyCallback) write_ready_cb,
		     g_object_ref (istream));
}

static gssize
read_internal (GInputStream  *stream,
	       void          *buffer,
	       gsize          count,
	       gboolean       blocking,
	       GCancellable  *cancellable,
	       GError       **error)
{
	SoupCacheInputStream *istream = SOUP_CACHE_INPUT_STREAM (stream);
	SoupCacheInputStreamPrivate *priv = istream->priv;
	GInputStream *base_stream;
	gssize nread;

	base_stream = g_filter_input_stream_get_base_stream (G_FILTER_INPUT_STREAM (stream));
	nread = g_pollable_stream_read (base_stream, buffer, count, blocking,
		cancellable, error);

	if (G_UNLIKELY (nread == -1 || priv->read_finished))
		return nread;

	if (nread == 0) {
		priv->read_finished = TRUE;

		if (priv->current_writing_buffer == NULL && priv->output_stream)
		notify_and_clear (istream, NULL);
	} else {
		SoupBuffer *soup_buffer = soup_buffer_new (SOUP_MEMORY_COPY, buffer, nread);
		g_queue_push_tail (priv->buffer_queue, soup_buffer);

		if (priv->current_writing_buffer == NULL && priv->output_stream)
		soup_cache_input_stream_write_next_buffer (istream);
	}

	return nread;
}

static gssize
soup_cache_input_stream_read_fn (GInputStream  *stream,
		 void          *buffer,
		 gsize          count,
		 GCancellable  *cancellable,
		 GError       **error)
{
	return read_internal (stream, buffer, count, TRUE,
		      cancellable, error);
}

static gssize
soup_cache_input_stream_read_nonblocking (GPollableInputStream  *stream,
		  void                  *buffer,
		  gsize                  count,
		  GError               **error)
{
	return read_internal (G_INPUT_STREAM (stream), buffer, count, FALSE,
		      NULL, error);
}

static void
soup_cache_input_stream_pollable_init (GPollableInputStreamInterface *pollable_interface,
		       gpointer interface_data)
{
	pollable_interface->read_nonblocking = soup_cache_input_stream_read_nonblocking;
}

static gboolean
soup_cache_input_stream_close_fn (GInputStream  *stream,
		  GCancellable  *cancellable,
		  GError       **error)
{
	SoupCacheInputStream *istream = SOUP_CACHE_INPUT_STREAM (stream);
	SoupCacheInputStreamPrivate *priv = istream->priv;

	if (!priv->read_finished) {
		if (priv->output_stream) {
		/* Cancel any pending write operation or return an error if none. */
		if (g_output_stream_has_pending (priv->output_stream))
		g_cancellable_cancel (priv->cancellable);
		else {
		GError *error = NULL;
		g_set_error_literal (&error, G_IO_ERROR, G_IO_ERROR_PARTIAL_INPUT,
		     _("Failed to completely cache the resource"));
		notify_and_clear (istream, error);
		}
		} else if (priv->cancellable)
		/* The file_replace_async() hasn't finished yet */
		g_cancellable_cancel (priv->cancellable);
	}

	return G_INPUT_STREAM_CLASS (soup_cache_input_stream_parent_class)->close_fn (stream, cancellable, error);
}

static void
soup_cache_input_stream_class_init (SoupCacheInputStreamClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	GInputStreamClass *istream_class = G_INPUT_STREAM_CLASS (klass);

	g_type_class_add_private (klass, sizeof (SoupCacheInputStreamPrivate));

	gobject_class->get_property = soup_cache_input_stream_get_property;
	gobject_class->set_property = soup_cache_input_stream_set_property;
	gobject_class->finalize = soup_cache_input_stream_finalize;

	istream_class->read_fn = soup_cache_input_stream_read_fn;
	istream_class->close_fn = soup_cache_input_stream_close_fn;

	g_object_class_install_property (gobject_class, PROP_OUTPUT_STREAM,
		 g_param_spec_object ("output-stream", "Output stream",
		      "the output stream where to write.",
		      G_TYPE_OUTPUT_STREAM,
		      G_PARAM_READWRITE |
		      G_PARAM_CONSTRUCT_ONLY |
		      G_PARAM_STATIC_STRINGS));

	signals[CACHING_FINISHED] =
		g_signal_new ("caching-finished",
		      G_OBJECT_CLASS_TYPE (gobject_class),
		      G_SIGNAL_RUN_FIRST,
		      G_STRUCT_OFFSET (SoupCacheInputStreamClass, caching_finished),
		      NULL, NULL,
		      NULL,
		      G_TYPE_NONE, 2,
		      G_TYPE_INT, G_TYPE_ERROR);
}

GInputStream *
soup_cache_input_stream_new (GInputStream *base_stream,
		     GFile        *file)
{
	SoupCacheInputStream *istream = g_object_new (SOUP_TYPE_CACHE_INPUT_STREAM,
		      "base-stream", base_stream,
		      "close-base-stream", FALSE,
		      NULL);

	istream->priv->cancellable = g_cancellable_new ();
	g_file_replace_async (file, NULL, FALSE,
		      G_FILE_CREATE_PRIVATE | G_FILE_CREATE_REPLACE_DESTINATION,
		      G_PRIORITY_DEFAULT, istream->priv->cancellable,
		      file_replaced_cb, g_object_ref (istream));

	return (GInputStream *) istream;
}
