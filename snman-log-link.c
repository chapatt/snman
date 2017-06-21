#include <glib-2.0/glib-object.h>
#include <gtk/gtk.h>

#include "snman-log-link.h"

struct _SnmanLogLink {
	GObject parent;

	gchar *path;
	gint start;
	gint end;
};

G_DEFINE_TYPE(SnmanLogLink, snman_log_link, G_TYPE_OBJECT);

enum {
	PROP_PATH = 1,
	PROP_START,
	PROP_END,
	N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void snman_log_link_set_property(GObject		*object,
					guint		property_id,
					const GValue	*value,
					GParamSpec	*pspec)
{
	SnmanLogLink *self = SNMAN_LOG_LINK(object);

	switch(property_id) {
	case PROP_PATH:
		g_free(self->path);
		self->path = g_value_dup_string(value);
		break;
	case PROP_START:
		self->start = g_value_get_int(value);
		break;
	case PROP_END:
		self->end = g_value_get_int(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void snman_log_link_get_property(GObject		*object,
					guint		property_id,
					const GValue	*value,
					GParamSpec	*pspec)
{
	SnmanLogLink *self = SNMAN_LOG_LINK(object);

	switch(property_id) {
	case PROP_PATH:
		g_value_set_string(value, self->path);
		break;
	case PROP_START:
		g_value_set_int(value, self->start);
		break;
	case PROP_END:
		g_value_set_int(value, self->end);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void snman_log_link_class_init(SnmanLogLinkClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS(class);

	object_class->set_property = snman_log_link_set_property;
	object_class->get_property = snman_log_link_get_property;

	obj_properties[PROP_PATH] =
		g_param_spec_string("path",
				    "The path",
				    "The path to the resource",
				    NULL,
				    G_PARAM_READWRITE);

	obj_properties[PROP_START] =
		g_param_spec_int("start",
				 "The start",
				 "The index in the SnmanLogEntry of the beginning of the link",
				 0,
				 G_MAXINT,
				 0,
				 G_PARAM_READWRITE);

	obj_properties[PROP_END] =
		g_param_spec_int("end",
				 "The end",
				 "The index in the SnmanLogEntry of the end of the link",
				 0,
				 G_MAXINT,
				 0,
				 G_PARAM_READWRITE);

	g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void snman_log_link_init(SnmanLogLink *self)
{
}

SnmanLogLink *snman_log_link_new(gchar *path, gint start, gint end)
{
	return g_object_new(SNMAN_TYPE_LOG_LINK,
			    "path",	path,
			    "start",	start,
			    "end",	end,
			    NULL);
}

gboolean *snman_log_link_has_iter(SnmanLogLink *self, GtkTextIter *iter)
{
}

gchar *snman_log_link_get_path(SnmanLogLink *self)
{
	return self->path;
}

gint snman_log_link_get_start(SnmanLogLink *self)
{
	return self->start;
}

gint snman_log_link_get_end(SnmanLogLink *self)
{
	return self->end;
}
