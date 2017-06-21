#ifndef SNMAN_HOSTS_TOOLBAR_H
#define SNMAN_HOSTS_TOOLBAR_H

#include <gtk/gtk.h>
#include "snman-window.h"
#include "snman-config.h"

G_BEGIN_DECLS

#define SNMAN_TYPE_HOSTS_TOOLBAR (snman_hosts_toolbar_get_type())
G_DECLARE_FINAL_TYPE(SnmanHostsToolbar, snman_hosts_toolbar, SNMAN, HOSTS_TOOLBAR, GtkToolbar)

SnmanHostsToolbar *snman_hosts_toolbar_new(SnmanWindow *window);

void snman_hosts_toolbar_switch_page(GtkNotebook	*notebook,
				     GtkWidget		*page,
				     guint		page_num,
				     SnmanHostsToolbar	*self);

G_END_DECLS

#endif /* SNMAN_HOSTS_TOOLBAR_H */
