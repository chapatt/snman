#include <gtk/gtk.h>

#include "snman-application.h"
#include "snman-window.h"
#include "snman-config.h"

struct _SnmanApplication {
	GtkApplication parent;
};

G_DEFINE_TYPE(SnmanApplication, snman_application, GTK_TYPE_APPLICATION)

static void snman_application_init(SnmanApplication *app)
{
}

static void snman_application_activate(GApplication *app)
{
	snman_config_parse();

	SnmanWindow *win = snman_window_new(SNMAN_APPLICATION(app));
	gtk_window_present(GTK_WINDOW(win));
}

static void snman_application_class_init(SnmanApplicationClass *class)
{
	G_APPLICATION_CLASS(class)->activate = snman_application_activate;
}

SnmanApplication *snman_application_new(void)
{
	return g_object_new(SNMAN_APPLICATION_TYPE,
			    "application-id",	"nl.openjr.snman",
			    "flags",		G_APPLICATION_HANDLES_OPEN,
			    NULL);
}
