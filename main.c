#include <gtk/gtk.h>

#include "snman-application.h"

int main(int argc, char **argv)
{
	return g_application_run(G_APPLICATION(snman_application_new()),
					argc, argv);
}
