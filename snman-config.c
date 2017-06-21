#include "snman-config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include "libjson2c/json2c.h"
#include "jsmn/jsmn.h"
#include "jsmin/jsmin.h"

static inline void *realloc_it(void *ptrmem, size_t size);

#define CONF_FILE_PATH "snmanrc"

GtkListStore *http_get_liststore;
GtkListStore *hosts_liststore;

GdkPixbuf *host_icon;

static const jsmntok_t *parse_http_get(const char *js, const jsmntok_t *t)
{
	size_t children = t->size;
	char *icon, *name, *path;
	const jsmntok_t *tmp_tok;
	http_get_liststore = gtk_list_store_new(HTTP_GET_COLS,
						GDK_TYPE_PIXBUF,
						G_TYPE_STRING,
						G_TYPE_STRING);

	++t;
	while (children > 0) {
		struct conf_schema schema = ARR_ARRAY_SIZE((
			(struct conf_element []) {
				{"icon", REQUIRED | USE_LAST, LEAF_STRING,
					&icon, NULL},
				{"name", REQUIRED | USE_LAST, LEAF_STRING,
					&name, NULL},
				{"path", REQUIRED | USE_LAST, LEAF_STRING,
					&path, NULL}
			}
		));

		if ((tmp_tok = parse_tokens(js, t, &schema)) == NULL)
		{
			return NULL;
		}
		t = tmp_tok;
		--children;

		GtkTreeIter iter;

		gtk_list_store_append(http_get_liststore, &iter);

		GdkPixbuf *pixbuf;
		pixbuf = gdk_pixbuf_new_from_file_at_size(icon, 16, 16, NULL);

		gtk_list_store_set(http_get_liststore, &iter,
			HTTP_GET_IMAGE, pixbuf,
			HTTP_GET_LABEL, name,
			HTTP_GET_PATH, path,
			-1);
	}

	return t;
}

static const jsmntok_t *parse_hosts(const char *js, const jsmntok_t *t)
{
	GtkTreeIter iter;
	size_t children = t->size;
	char *name, *hostname;
	const jsmntok_t *tmp_tok;

	hosts_liststore = gtk_list_store_new(HOSTS_COLS,
					     GDK_TYPE_PIXBUF,
					     G_TYPE_STRING,
					     G_TYPE_STRING);

	++t;
	while (children > 0) {
		struct conf_schema schema = ARR_ARRAY_SIZE((
			(struct conf_element []) {
				{"name", REQUIRED | USE_LAST, LEAF_STRING,
					&name, NULL},
				{"hostname", REQUIRED | USE_LAST, LEAF_STRING,
					&hostname, NULL}
			}
		));

		if ((tmp_tok = parse_tokens(js, t, &schema)) == NULL)
			return NULL;
		t = tmp_tok;
		--children;

		gtk_list_store_append(hosts_liststore, &iter);
		gtk_list_store_set(hosts_liststore, &iter,
			HOSTS_IMAGE, host_icon,
			HOSTS_LABEL, name,
			HOSTS_HOSTNAME, hostname,
			-1);
	}

	return t;
}


struct conf_schema conf_schema_commands = ARR_ARRAY_SIZE((
	(struct conf_element []) {
		{"http_get", 0, NODE_FN, NULL, &parse_http_get}
	}
));

struct conf_schema conf_schema_root = ARR_ARRAY_SIZE((
	(struct conf_element []) {
		{"commands", 0, NODE_SCHEMA, &conf_schema_commands, NULL},
		{"hosts", 0, NODE_FN, NULL, &parse_hosts}
	}
));

int snman_config_parse(void) {
	host_icon = gdk_pixbuf_new_from_file_at_size("./slot.png", 75, 75, NULL);

	int parse_err;
	bool done_parsing = false;
	FILE *conf_filep = NULL;
	char *js = NULL;
	size_t jslen = 0;
	struct device **devs;

	jsmn_parser json_parser;
	jsmntok_t *tok;
	size_t tokcount = 2;


	if ((conf_filep = fopen(CONF_FILE_PATH, "r")) == NULL) {
		fprintf(stderr, "\"%s\": %s\n",
			CONF_FILE_PATH, strerror(errno));
		exit(1);
	}

	js = jsmin(conf_filep);
	jslen = strlen(js);

	/* Prepare parser */
	jsmn_init(&json_parser);

	/* Allocate some tokens as a start */
	tok = malloc(sizeof(*tok) * tokcount);
	if (tok == NULL) {
		fprintf(stderr, "malloc(): errno=%d\n", errno);
		exit(1);
	}

	while (!done_parsing) {
		parse_err = jsmn_parse(&json_parser, js, jslen, tok, tokcount);
		if (parse_err < 0) {
			if (parse_err == JSMN_ERROR_NOMEM) {
				tokcount = tokcount * 2;
				tok = realloc_it(tok, sizeof(*tok) * tokcount);
				if (tok == NULL) {
					exit(1);
				}
			}
		} else {
			done_parsing = true;
		}
	}

	if (parse_tokens(js, tok, &conf_schema_root) == NULL) {
		printf("json2cerrno: %d\n", json2cerrno);
		exit(1);
	}

	return EXIT_SUCCESS;
}

/* Function realloc_it() is a wrapper function for standart realloc()
 * with one difference - it frees old memory pointer in case of realloc
 * failure. Thus, DO NOT use old data pointer in anyway after call to
 * realloc_it(). If your code has some kind of fallback algorithm if
 * memory can't be re-allocated - use standart realloc() instead.
 */
static inline void *realloc_it(void *ptrmem, size_t size)
{
	void *p = realloc(ptrmem, size);
	if (!p)  {
		free (ptrmem);
		fprintf(stderr, "realloc(): errno=%d\n", errno);
	}
	return p;
}
