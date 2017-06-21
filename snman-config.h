#ifndef SBKCTL_CONFIG_H
#define SBKCTL_CONFIG_H

#include <gtk/gtk.h>

enum { HTTP_GET_IMAGE, HTTP_GET_LABEL, HTTP_GET_PATH, HTTP_GET_COLS };
extern GtkListStore *http_get_liststore;

enum { HOSTS_IMAGE, HOSTS_LABEL, HOSTS_HOSTNAME, HOSTS_COLS };
extern GtkListStore *hosts_liststore;

extern GdkPixbuf *host_icon;

int snman_config_parse(void);

#endif
