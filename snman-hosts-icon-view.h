#ifndef SNMAN_HOSTS_ICON_VIEW_H
#define SNMAN_HOSTS_ICON_VIEW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SNMAN_TYPE_HOSTS_ICON_VIEW (snman_hosts_icon_view_get_type())
G_DECLARE_FINAL_TYPE(SnmanHostsIconView, snman_hosts_icon_view, SNMAN, HOSTS_ICON_VIEW, GtkIconView)

SnmanHostsIconView *snman_hosts_icon_view_new(void);

void snman_hosts_icon_view_host_cmd(GtkIconView *self,
				    GtkTreePath *path,
				    gpointer get_path);

G_END_DECLS

#endif /* SNMAN_HOSTS_ICON_VIEW_H */
