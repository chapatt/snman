#ifndef SNMAN_LOG_LINK_H
#define SNMAN_LOG_LINK_H

#include <glib-2.0/glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SNMAN_TYPE_LOG_LINK (snman_log_link_get_type())
G_DECLARE_FINAL_TYPE(SnmanLogLink, snman_log_link, SNMAN, LOG_LINK, GObject)

SnmanLogLink *snman_log_link_new(gchar *path, gint start, gint end);
gchar *snman_log_link_get_path(SnmanLogLink *self);
gint snman_log_link_get_start(SnmanLogLink *self);
gint snman_log_link_get_end(SnmanLogLink *self);

G_END_DECLS

#endif /* SNMAN_LOG_LINK_H */
