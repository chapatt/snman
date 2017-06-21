#ifndef SNMAN_LOG_ENTRY_H
#define SNMAN_LOG_ENTRY_H

#include <glib-2.0/glib-object.h>
#include <gtk/gtk.h>

#include "snman-log-link.h"

G_BEGIN_DECLS

#define SNMAN_TYPE_LOG_ENTRY (snman_log_entry_get_type())
G_DECLARE_FINAL_TYPE(SnmanLogEntry, snman_log_entry, SNMAN, LOG_ENTRY, GObject)

SnmanLogEntry *snman_log_entry_new(void);
SnmanLogEntry *snman_log_entry_new_with_format(const gchar *format, va_list ap);
SnmanLogEntry *snman_log_entry_new_with_vformat(const gchar *format, va_list ap);
void snman_log_entry_set_message(SnmanLogEntry *self, gchar *message);
gchar *snman_log_entry_get_message(SnmanLogEntry *self);
GSList *snman_log_entry_get_links(SnmanLogEntry *self);
void snman_log_entry_add_link(SnmanLogEntry *self, SnmanLogLink *link);
SnmanLogLink *snman_log_entry_get_link_at_iter(SnmanLogEntry *self, GtkTextIter *iter);

G_END_DECLS

#endif /* SNMAN_LOG_ENTRY_H */
