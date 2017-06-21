#ifndef SNMAN_APPLICATION_H
#define SNMAN_APPLICATION_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SNMAN_APPLICATION_TYPE (snman_application_get_type())
G_DECLARE_FINAL_TYPE(SnmanApplication, snman_application, SNMAN, APPLICATION, GtkApplication)

SnmanApplication *snman_application_new(void);

G_END_DECLS

#endif /* SNMAN_APPLICATION_H */
