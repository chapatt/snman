#include <gtk/gtk.h>

#include "snman-application.h"
#include "snman-window.h"
#include "snman-log-text-view.h"
#include "snman-hosts-toolbar.h"
#include "snman-hosts-tree-view.h"
#include "snman-hosts-icon-view.h"

struct _SnmanWindow {
	GtkApplicationWindow parent;

	GtkWidget *header_bar;
	GtkWidget *log_scroll;
	GtkWidget *hosts_box;
	GtkWidget *hosts_notebook;
	GtkWidget *hosts_icon_scroll;
	GtkWidget *hosts_tree_scroll;

	GtkWidget *menu_bar;
	GtkWidget *log_text_view;
	GtkWidget *hosts_toolbar;
	GtkWidget *hosts_icon_view;
	GtkWidget *hosts_tree_view;
};

G_DEFINE_TYPE(SnmanWindow, snman_window, GTK_TYPE_APPLICATION_WINDOW)

static void snman_window_class_init(SnmanWindowClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
		"/nl/openjr/snman/snman-window.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SnmanWindow, header_bar);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SnmanWindow, log_scroll);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SnmanWindow, hosts_box);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SnmanWindow, hosts_notebook);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SnmanWindow, hosts_icon_scroll);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SnmanWindow, hosts_tree_scroll);
}

static void snman_window_init(SnmanWindow *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));

	self->menu_bar = gtk_menu_bar_new();
	GtkWidget *file_menu = gtk_menu_item_new_with_label("File");
	GtkWidget *edit_menu = gtk_menu_item_new_with_label("Edit");
	GtkWidget *view_menu = gtk_menu_item_new_with_label("View");
	GtkWidget *help_menu = gtk_menu_item_new_with_label("Help");
	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu_bar), file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu_bar), edit_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu_bar), view_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(self->menu_bar), help_menu);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(self->header_bar),
				  GTK_WIDGET(self->menu_bar));

	self->log_text_view = GTK_WIDGET(snman_log_text_view_new(self));
	gtk_container_add(GTK_CONTAINER(self->log_scroll), self->log_text_view);
	snman_log_text_view_log(SNMAN_LOG_TEXT_VIEW(self->log_text_view),
		"link: %q\n"
		"int in decimal with field width of 5 padded with zeroes: %.*d\n"
		"int in hexadecimal: %x\n"
		"long long integer in decimal: %lld\n"
		"float: %f\n"
		"long double float: %llf\n"
		"float in scientific form: %e\n"
		"float with field width 10 padded with zeroes and precision 1: %10.1f\n"
		"char as ASCII: %c\n"
		"string: %s\n"
		"just a percent sign: %%\n",

		"this is a link",
		5, 24,
		24,
		999999999999,
		3.14,
		9999999.999999l,
		3.14,
		3.14,
		'c',
		"hiya!"
	);
	snman_log_text_view_log(SNMAN_LOG_TEXT_VIEW(self->log_text_view), "Hello, world\nhello\nhello\nhello\nhello\nfoo\n");

	self->hosts_toolbar = GTK_WIDGET(snman_hosts_toolbar_new(self));
	gtk_box_pack_start(GTK_BOX(self->hosts_box), self->hosts_toolbar, FALSE, FALSE, 0);
	gtk_box_reorder_child(GTK_BOX(self->hosts_box), self->hosts_toolbar, 0);
	g_signal_connect(self->hosts_notebook,
			 "switch-page",
			 G_CALLBACK(snman_hosts_toolbar_switch_page),
			 self->hosts_toolbar);


	self->hosts_icon_view = GTK_WIDGET(snman_hosts_icon_view_new());
	gtk_container_add(GTK_CONTAINER(self->hosts_icon_scroll), self->hosts_icon_view);

	self->hosts_tree_view = GTK_WIDGET(snman_hosts_tree_view_new());
	gtk_container_add(GTK_CONTAINER(self->hosts_tree_scroll), self->hosts_tree_view);

	gtk_widget_show_all(GTK_WIDGET(self));
}

SnmanWindow *snman_window_new(SnmanApplication *app)
{
	return g_object_new(SNMAN_TYPE_WINDOW, "application", app, NULL);
}

void snman_hosts_notebook_show_icon(GtkToolButton *button, SnmanWindow *window)
{
	gtk_notebook_set_current_page(GTK_NOTEBOOK(window->hosts_notebook), 0);
}

void snman_hosts_notebook_show_tree(GtkToolButton *button, SnmanWindow *window)
{
	gtk_notebook_set_current_page(GTK_NOTEBOOK(window->hosts_notebook), 1);
}

void snman_window_cmd_selected(GtkWidget *widget,
			       struct cmd_selected_data *data)
{
/*
	gchararray get_path = g_strdup(data->get_path);
	gtk_icon_view_selected_foreach(
		GTK_ICON_VIEW(data->window->hosts_icon_view),
		snman_hosts_icon_view_host_cmd,
		get_path);
*/
	snman_log_text_view_log(SNMAN_LOG_TEXT_VIEW(data->window->log_text_view), "This was printed during runtime");
}
