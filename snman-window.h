#ifndef SNMAN_WINDOW_H
#define SNMAN_WINDOW_H

#include <gtk/gtk.h>
#include "snman-application.h"

G_BEGIN_DECLS

#define SNMAN_TYPE_WINDOW (snman_window_get_type())
G_DECLARE_FINAL_TYPE(SnmanWindow, snman_window, SNMAN, WINDOW, GtkApplicationWindow)

SnmanWindow *snman_window_new(SnmanApplication *app);

void snman_hosts_notebook_show_icon(GtkToolButton *button, SnmanWindow *window);
void snman_hosts_notebook_show_tree(GtkToolButton *button, SnmanWindow *window);

struct cmd_selected_data {
        SnmanWindow *window;
        gchararray get_path;
};

void snman_window_cmd_selected(GtkWidget *widget,
			       struct cmd_selected_data *data);

G_END_DECLS

#endif /* SNMAN_WINDOW_H */
