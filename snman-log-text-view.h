#ifndef SNMAN_LOG_TEXT_VIEW_H
#define SNMAN_LOG_TEXT_VIEW_H

#include <gtk/gtk.h>

#include "snman-window.h"
#include "snman-log-entry.h"

G_BEGIN_DECLS

#define SNMAN_TYPE_LOG_TEXT_VIEW (snman_log_text_view_get_type())
G_DECLARE_FINAL_TYPE(SnmanLogTextView, snman_log_text_view, SNMAN, LOG_TEXT_VIEW, GtkTextView)

SnmanLogTextView *snman_log_text_view_new(SnmanWindow *window);
void snman_log_text_view_log(SnmanLogTextView *text_view,
			     const gchar *format,
			     ...);
void snman_log_text_view_add_entry(SnmanLogTextView *self, SnmanLogEntry *entry);
SnmanLogEntry *snman_log_text_view_get_log_entry_at_iter(SnmanLogTextView *self, GtkTextIter *iter);

G_END_DECLS

#endif /* SNMAN_LOG_TEXT_VIEW_H */
