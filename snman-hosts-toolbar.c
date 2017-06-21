#include <gtk/gtk.h>

#include "snman-hosts-toolbar.h"
#include "snman-window.h"
#include "snman-config.h"
#include "snman-curl.h"

struct cmd_selected_data data;
gchararray get_path;

struct _SnmanHostsToolbar {
	GtkToolbar parent;

	SnmanWindow *window;

	GtkWidget *icon_button;
	GtkWidget *tree_button;
};

G_DEFINE_TYPE(SnmanHostsToolbar, snman_hosts_toolbar, GTK_TYPE_TOOLBAR);

enum {
	PROP_WINDOW = 1,
	N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void snman_hosts_toolbar_set_property(GObject		*object,
					     guint		property_id,
					     const GValue	*value,
					     GParamSpec		*pspec)
{
	SnmanHostsToolbar *self = SNMAN_HOSTS_TOOLBAR(object);

	switch(property_id) {
	case PROP_WINDOW:
		g_free(self->window);
		self->window = SNMAN_WINDOW(g_value_get_object(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void snman_hosts_toolbar_get_property(GObject	*object,
					     guint	property_id,
					     GValue	*value,
					     GParamSpec	*pspec)
{
	SnmanHostsToolbar *self = SNMAN_HOSTS_TOOLBAR(object);

	switch(property_id) {
	case PROP_WINDOW:
		g_value_set_object(value, self->window);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void snman_hosts_toolbar_constructed(GObject *object)
{
	SnmanHostsToolbar *self = SNMAN_HOSTS_TOOLBAR(object);

	data.window = self->window;

	g_signal_connect(self->icon_button,
			 "clicked",
			 G_CALLBACK(snman_hosts_notebook_show_icon),
			 SNMAN_WINDOW(self->window));

	g_signal_connect(self->tree_button,
			 "clicked",
			 G_CALLBACK(snman_hosts_notebook_show_tree),
			 SNMAN_WINDOW(self->window));
}

static void snman_hosts_toolbar_class_init(SnmanHostsToolbarClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS(class);

	object_class->set_property = snman_hosts_toolbar_set_property;
	object_class->get_property = snman_hosts_toolbar_get_property;
	object_class->constructed = snman_hosts_toolbar_constructed;

	obj_properties[PROP_WINDOW] =
		g_param_spec_object("window",
			"The SnmanWindow",
			"The SnmanWindow to which this toolbar belongs",
			SNMAN_TYPE_WINDOW,
			G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

void snman_hosts_toolbar_switch_page(GtkNotebook	*notebook,
				     GtkWidget		*page,
				     guint		page_num,
				     SnmanHostsToolbar	*self)
{
	switch (page_num) {
	case 0:
		gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(self->icon_button), TRUE);
		break;
	case 1:
		gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(self->tree_button), TRUE);
	}
}

/*
static void show_icon(GtkToolButton *button, SnmanHostsToolbar *self)
{
	snman_hosts_notebook_show_icon(self->window);
}

static void show_tree(GtkToolButton *button, SnmanHostsToolbar *self)
{
	snman_hosts_notebook_show_tree(self->window);
}
*/

static void snman_hosts_toolbar_init(SnmanHostsToolbar *self)
{
	GtkTreeIter http_get_iter;
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(http_get_liststore),
				      &http_get_iter);
	do {
		gchararray label;
		GdkPixbuf *icon;
		GtkWidget *image;
		GtkToolItem *toolbtn;
		gtk_tree_model_get(GTK_TREE_MODEL(http_get_liststore),
			&http_get_iter,
			HTTP_GET_IMAGE, &icon,
			HTTP_GET_LABEL, &label,
			HTTP_GET_PATH, &get_path,
			-1);
		image = gtk_image_new_from_pixbuf(icon);
		toolbtn = gtk_tool_button_new(image, label);
		gtk_toolbar_insert(GTK_TOOLBAR(self), toolbtn, -1);
		data.get_path = get_path;
		g_signal_connect(toolbtn,
				 "clicked",
				 G_CALLBACK(snman_window_cmd_selected),
				 &data);
	} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(http_get_liststore),
		&http_get_iter));


	GtkToolItem *spring;
	spring = GTK_TOOL_ITEM(gtk_separator_tool_item_new());
	gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(spring), FALSE);
	gtk_toolbar_insert(GTK_TOOLBAR(self), spring, -1);
	gtk_container_child_set(GTK_CONTAINER(self),
				GTK_WIDGET(spring),
				"expand", GTK_EXPAND,
				NULL);


	GtkToolItem *separator;
	separator = GTK_TOOL_ITEM(gtk_separator_tool_item_new());
	gtk_toolbar_insert(GTK_TOOLBAR(self), separator, -1);


	self->icon_button = GTK_WIDGET(gtk_radio_tool_button_new(NULL));
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(self->icon_button), "view-grid-symbolic");
	gtk_toolbar_insert(GTK_TOOLBAR(self), GTK_TOOL_ITEM(self->icon_button), -1);


	self->tree_button = GTK_WIDGET(gtk_radio_tool_button_new_from_widget(
					GTK_RADIO_TOOL_BUTTON(self->icon_button)));
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(self->tree_button), "view-list-symbolic");
	gtk_toolbar_insert(GTK_TOOLBAR(self), GTK_TOOL_ITEM(self->tree_button), -1);
}

SnmanHostsToolbar *snman_hosts_toolbar_new(SnmanWindow *window)
{
	return g_object_new(SNMAN_TYPE_HOSTS_TOOLBAR, "window", window, NULL);
}
