#ifndef SNMAN_HOSTS_TREE_VIEW_H
#define SNMAN_HOSTS_TREE_VIEW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SNMAN_TYPE_HOSTS_TREE_VIEW (snman_hosts_tree_view_get_type())
G_DECLARE_FINAL_TYPE(SnmanHostsTreeView, snman_hosts_tree_view, SNMAN, HOSTS_TREE_VIEW, GtkTreeView)

SnmanHostsTreeView *snman_hosts_tree_view_new(void);

G_END_DECLS

#endif /* SNMAN_HOSTS_TREE_VIEW_H */
