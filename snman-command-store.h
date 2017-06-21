#ifndef SNMAN_COMMAND_STORE_H
#define SNMAN_COMMAND_STORE_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SNMAN_TYPE_COMMAND_STORE (snman_command_store_get_type())
G_DECLARE_FINAL_TYPE(SnmanCommandStore, snman_command_store, SNMAN, COMMAND_STORE, GtkListStore)

SnmanCommandStore *snman_command_store_new(void);

G_END_DECLS

#endif /* SNMAN_COMMAND_STORE_H */
