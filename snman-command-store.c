#include <gtk/gtk.h>

#include "snman-command-store.h"

struct _SnmanCommandStore {
        GtkListStore parent;
};

G_DEFINE_TYPE(SnmanCommandStore, snman_command_store, GTK_TYPE_LIST_STORE)

static void snman_command_store_class_init(SnmanCommandStoreClass *class)
{
}

static void snman_command_store_init(SnmanCommandStore *self)
{
}

SnmanCommandStore *snman_command_store_new(void)
{
	return g_object_new(SNMAN_TYPE_COMMAND_STORE, NULL);
}
