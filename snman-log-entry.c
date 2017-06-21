#include <glib-2.0/glib-object.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "snman-log-entry.h"
#include "snman-log-link.h"

struct _SnmanLogEntry {
	GObject parent;

	gchar *message;
	GSList *links;
};

G_DEFINE_TYPE(SnmanLogEntry, snman_log_entry, G_TYPE_OBJECT);

static void snman_log_entry_class_init(SnmanLogEntryClass *class)
{
}

static void snman_log_entry_init(SnmanLogEntry *self)
{
	self->message = NULL;
	self->links = NULL;
}

SnmanLogEntry *snman_log_entry_new(void)
{
	return g_object_new(SNMAN_TYPE_LOG_ENTRY, NULL);
}

void snman_log_entry_set_message(SnmanLogEntry *self, gchar *message)
{
	g_free(self->message);
	self->message = message;
}

gchar *snman_log_entry_get_message(SnmanLogEntry *self)
{
	return self->message;
}

GSList *snman_log_entry_get_links(SnmanLogEntry *self)
{
	return self->links;
}

void snman_log_entry_add_link(SnmanLogEntry *self, SnmanLogLink *link)
{
	self->links = g_slist_append(self->links, link);
}

SnmanLogLink *snman_log_entry_get_link_at_iter(SnmanLogEntry *self, GtkTextIter *iter)
{
}

/* return malloc'd string of cat'd strings s and t */
static gchar *strappendstr(gchar *s, gchar *t)
{
        gchar *final;
        size_t s_z; /* length of s */
        size_t t_z; /* length of t */
        size_t final_z; /* length of final */

        if (s == NULL)
                s = "";
        if (t == NULL)
                t = "";

        s_z = strlen(s);
        t_z = strlen(t);
        final_z = s_z + t_z;

        final = malloc((final_z + 1) * sizeof(char));
        if (final == NULL)
                return NULL;

        strcpy(final, s);
        strcat(final, t);

        return final;
}

/* return malloc'd string of cat'd string s and char c */
static gchar *strappendchar(gchar *s, gchar c)
{
        gchar *final;
        size_t s_z; /* length of s */

        if (s == NULL)
                s = "";

        s_z = strlen(s);

        final = malloc((s_z + 2) * sizeof(char));
        if (final == NULL)
                return NULL;

        strcpy(final, s);
        final[s_z] = c;
        final[s_z + 1] = '\0';

        return final;
}

enum {
	FLAG_STAR1 =	0001,
	FLAG_STAR2 =	0002,
	FLAG_LONG =	0010,
	FLAG_LLONG =	0020,
	FLAG_SIZE =	0040,
	FLAG_INTMAX =	0100,
	FLAG_PTRDIFF =	0200
};

/*
 * parse_log_format: a wrapper around asprintf (adding some conversion specifiers)
 * that takes a format string and arguments (like printf and friends), and returns
 * a pointer to a SnmanLogEntry.
 */
SnmanLogEntry *snman_log_entry_new_with_format(const gchar *format, va_list ap)
{
}

/*
 * parse_log_format: a wrapper around asprintf (adding some conversion specifiers)
 * that takes a format string and va_list (like vprintf and friends), and returns
 * a pointer to a SnmanLogEntry.
 */
SnmanLogEntry *snman_log_entry_new_with_vformat(const gchar *format, va_list ap)
{
	SnmanLogEntry *entry = snman_log_entry_new();

	int star1, star2;
	wint_t wintarg;
	long long intarg;
	long long n_printed;
	long double floatarg;
	long double *ptrarg;
	gchar *strarg1, *strarg2;
	size_t link_start, link_end;
	SnmanLogLink *link;

	int flags = 0;
	gboolean in_placeholder = FALSE;
	gchar c = '\0';
	gchar *processedsubstr = "", *substrtoprocess = "";

	size_t i;
	for (i=0; c = format[i]; ++i) {
		if (in_placeholder) {
			substrtoprocess = strappendchar(substrtoprocess, c);

			switch (c) {
			case '*':
				if (flags & FLAG_STAR1)
					flags |= FLAG_STAR2;
				else
					flags |= FLAG_STAR1;
				break;
			/*
			 * The cases below signify the length field
			 */
			case 'l':
				if (flags & FLAG_LONG) {
					flags = 0;
					flags |= FLAG_LLONG;
				} else {
					flags = 0;
					flags |= FLAG_LONG;
				}
				break;
			case 'L':
				flags = 0;
				flags |= FLAG_LLONG;
				break;
			case 'z':
				flags = 0;
				flags |= FLAG_SIZE;
				break;
			case 'j':
				flags = 0;
				flags |= FLAG_INTMAX;
				break;
			case 't':
				flags = 0;
				flags |= FLAG_PTRDIFF;
				break;
			/*
			 * The cases below signify the type field and finish a placeholder
			 */
			case 'q':	/* custom "link" type conversion specifier */
				in_placeholder = FALSE;

				if (flags & FLAG_STAR1) {
					strarg1 = va_arg(ap, char *);	/* display */
					strarg2 = va_arg(ap, char *);	/* path */
				} else {
					strarg1 = strarg2 = va_arg(ap, char *);
				}

				link_start = i - strlen(substrtoprocess) + 1;
				link_end = link_start + strlen(strarg1);
				substrtoprocess = strarg1;

				link = snman_log_link_new(strarg2, link_start, link_end);
				snman_log_entry_add_link(entry, link);

				flags = 0;
				break;
			case '%':
				in_placeholder = FALSE;
				break;
			case 'd': case 'i': case 'u': case 'o': case 'x': case 'X':
				in_placeholder = FALSE;

				if (flags & FLAG_STAR1)
					star1 = va_arg(ap, int);
				if (flags & FLAG_STAR2)
					star2 = va_arg(ap, int);

				if (flags & FLAG_LONG)
					intarg = va_arg(ap, long int);
				else if (flags & FLAG_LLONG)
					intarg = va_arg(ap, long long int);
				else if (flags & FLAG_SIZE)
					intarg = va_arg(ap, size_t);
				else if (flags & FLAG_PTRDIFF)
					intarg = va_arg(ap, ptrdiff_t);
				else
					intarg = va_arg(ap, int);

				if (flags & FLAG_STAR2) {
					asprintf(&processedsubstr,
						 substrtoprocess,
						 star1,
						 star2,
						 intarg);
				} else if (flags & FLAG_STAR1) {
					asprintf(&processedsubstr,
						 substrtoprocess,
						 star1,
						 intarg);
				} else {
					asprintf(&processedsubstr,
						 substrtoprocess,
						 intarg);
				}

				snman_log_entry_set_message(entry,
					strappendstr(snman_log_entry_get_message(entry), processedsubstr));
				flags = 0;
				substrtoprocess = "";
				break;
			case 'f': case 'F': case 'e': case 'E':
			case 'g': case 'G': case 'a': case 'A':
				in_placeholder = FALSE;

				if (flags & FLAG_STAR1)
					star1 = va_arg(ap, int);
				if (flags & FLAG_STAR2)
					star2 = va_arg(ap, int);

				if (flags & FLAG_LLONG)
					floatarg = va_arg(ap, long double);
				else
					floatarg = va_arg(ap, double);

				if (flags & FLAG_LLONG) {
					if (flags & FLAG_STAR2) {
						asprintf(&processedsubstr,
							 substrtoprocess,
							 star1,
							 star2,
							 floatarg);
					} else if (flags & FLAG_STAR1) {
						asprintf(&processedsubstr,
							 substrtoprocess,
							 star1,
							 floatarg);
					} else {
						asprintf(&processedsubstr,
							 substrtoprocess,
							 floatarg);
					}
				} else {
					if (flags & FLAG_STAR2) {
						asprintf(&processedsubstr,
							 substrtoprocess,
							 star1,
							 star2,
							 (double) floatarg);
					} else if (flags & FLAG_STAR1) {
						asprintf(&processedsubstr,
							 substrtoprocess,
							 star1,
							 (double) floatarg);
					} else {
						asprintf(&processedsubstr,
							 substrtoprocess,
							 (double) floatarg);
					}
				}

				snman_log_entry_set_message(entry,
					strappendstr(snman_log_entry_get_message(entry), processedsubstr));
				flags = 0;
				substrtoprocess = "";
				break;
			case 'c':
				in_placeholder = FALSE;

				if (flags & FLAG_STAR1)
					star1 = va_arg(ap, int);
				if (flags & FLAG_STAR2)
					star2 = va_arg(ap, int);

				if (flags & FLAG_LONG)
					intarg = va_arg(ap, wint_t);
				else
					intarg = va_arg(ap, int);

				if (flags & FLAG_STAR1) {
					asprintf(&processedsubstr,
						 substrtoprocess,
						 star1,
						 intarg);
				} else if (flags & FLAG_STAR2){
					asprintf(&processedsubstr,
						 substrtoprocess,
						 star1,
						 star2,
						 intarg);
				} else {
					asprintf(&processedsubstr,
						 substrtoprocess,
						 intarg);
				}
				snman_log_entry_set_message(entry,
					strappendstr(snman_log_entry_get_message(entry), processedsubstr));
				flags = 0;
				substrtoprocess = "";
				break;
			case 's': case 'p':
				in_placeholder = FALSE;

				if (flags & FLAG_STAR1)
					star1 = va_arg(ap, int);
				if (flags & FLAG_STAR2)
					star2 = va_arg(ap, int);
				/* FIXME! Implement stars! */

				ptrarg = va_arg(ap, void *);

				asprintf(&processedsubstr,
					 substrtoprocess,
					 ptrarg);
				snman_log_entry_set_message(entry,
					strappendstr(snman_log_entry_get_message(entry), processedsubstr));
				flags = 0;
				substrtoprocess = "";
				break;
			case 'n':
				in_placeholder = FALSE;

				n_printed = strlen(snman_log_entry_get_message(entry));

				if (flags & FLAG_LONG)
					*va_arg(ap, long int *) = (long int) n_printed;
				else if (flags & FLAG_LLONG)
					*va_arg(ap, long long int *) = (long long int) n_printed;
				else if (flags & FLAG_SIZE)
					*va_arg(ap, size_t *) = (size_t) n_printed;
				else if (flags & FLAG_PTRDIFF)
					*va_arg(ap, ptrdiff_t *) = (ptrdiff_t) n_printed;
				else
					*va_arg(ap, int *) = (int) n_printed;

				flags = 0;
				substrtoprocess = "";
				break;
			}
		} else if (c == '%') {
			/* a placeholder is starting, process the current substring and clear */
			asprintf(&processedsubstr, substrtoprocess);
			snman_log_entry_set_message(entry,
				strappendstr(snman_log_entry_get_message(entry), processedsubstr));
			substrtoprocess = "";
			in_placeholder = TRUE;
			substrtoprocess = strappendchar(substrtoprocess, c);
		} else {
			substrtoprocess = strappendchar(substrtoprocess, c);
		}
	}

	asprintf(&processedsubstr, substrtoprocess);
	snman_log_entry_set_message(entry,
		strappendstr(snman_log_entry_get_message(entry), processedsubstr));

	return entry;
}
