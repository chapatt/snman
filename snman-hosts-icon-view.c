#include <gtk/gtk.h>

#include "snman-hosts-icon-view.h"
#include "snman-config.h"
#include "snman-log-text-view.h"
#include "snman-curl.h"

struct _SnmanHostsIconView {
	GtkIconView parent;
};

G_DEFINE_TYPE(SnmanHostsIconView, snman_hosts_icon_view, GTK_TYPE_ICON_VIEW);

static void snman_hosts_icon_view_class_init(SnmanHostsIconViewClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
		"/nl/openjr/snman/snman-hosts-icon-view.ui");
}

static void snman_hosts_icon_view_init(SnmanHostsIconView *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));

	gtk_icon_view_set_model(GTK_ICON_VIEW(self), GTK_TREE_MODEL(hosts_liststore));
	gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(self), HOSTS_IMAGE);
	gtk_icon_view_set_text_column(GTK_ICON_VIEW(self), HOSTS_LABEL);
}

SnmanHostsIconView *snman_hosts_icon_view_new(void)
{
	return g_object_new(SNMAN_TYPE_HOSTS_ICON_VIEW, NULL);
}

void snman_hosts_icon_view_host_cmd(GtkIconView *self,
        	     		    GtkTreePath *path,
		     		    gpointer get_path)
{
        GtkTreeIter iter;
        GtkTreeModel *model;
        gchararray hostname;

        model = gtk_icon_view_get_model(self);
        gtk_tree_model_get_iter(model, &iter, path);
        gtk_tree_model_get(model, &iter, HOSTS_HOSTNAME, &hostname, -1);
        curl_cmd_host((char *) hostname, (char *) get_path);
}
