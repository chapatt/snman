#include <gtk/gtk.h>

#include "snman-hosts-tree-view.h"

struct _SnmanHostsTreeView {
	GtkTreeView parent;
};

G_DEFINE_TYPE(SnmanHostsTreeView, snman_hosts_tree_view, GTK_TYPE_TREE_VIEW);

static void snman_hosts_tree_view_class_init(SnmanHostsTreeViewClass *class)
{
}

static void snman_hosts_tree_view_init(SnmanHostsTreeView *self)
{
}

SnmanHostsTreeView *snman_hosts_tree_view_new(void)
{
	return g_object_new(SNMAN_TYPE_HOSTS_TREE_VIEW, NULL);
}
