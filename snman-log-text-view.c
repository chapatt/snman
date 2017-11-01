#include <gtk/gtk.h>

#include "snman-log-text-view.h"
#include "snman-window.h"
#include "snman-log-entry.h"

struct _SnmanLogTextView {
	GtkTextView parent;

	SnmanWindow *window;
	GtkTextBuffer *buffer;
};

G_DEFINE_TYPE(SnmanLogTextView, snman_log_text_view, GTK_TYPE_TEXT_VIEW)

enum {
	PROP_WINDOW = 1,
	N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL };

static void snman_log_text_view_set_property(GObject		*object,
					     guint		property_id,
					     const GValue	*value,
					     GParamSpec		*pspec)
{
	SnmanLogTextView *self = SNMAN_LOG_TEXT_VIEW(object);

	switch(property_id) {
	case PROP_WINDOW:
		g_free(self->window);
		self->window = SNMAN_WINDOW(g_value_get_object(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void snman_log_text_view_get_property(GObject	*object,
					     guint	property_id,
					     GValue	*value,
					     GParamSpec	*pspec)
{
	SnmanLogTextView *self = SNMAN_LOG_TEXT_VIEW(object);

	switch(property_id) {
	case PROP_WINDOW:
		g_value_set_object(value, self->window);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static gboolean snman_log_text_view_button_press(GtkWidget	*self,
						 GdkEvent	*event,
						 gpointer	data)
{
	gdouble x_win, y_win;
	gint x_buf, y_buf;
	GtkTextIter iter;
	GtkTextTag *link_tag;
	GtkTextTagTable *tag_table;
	gdk_event_get_coords(event, &x_win, &y_win);
	gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(self),
					      GTK_TEXT_WINDOW_TEXT,
					      (gint) x_win,
					      (gint) y_win,
					      &x_buf,
					      &y_buf);
	gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(self),
					   &iter,
					   x_buf,
					   y_buf);
	tag_table = gtk_text_buffer_get_tag_table(SNMAN_LOG_TEXT_VIEW(self)->buffer);
	link_tag = gtk_text_tag_table_lookup(tag_table, "link");
	if (gtk_text_iter_has_tag(&iter, link_tag))
		g_print("Link clicked!\n");

	return FALSE;
}

static gboolean snman_log_text_view_motion(GtkWidget	*self,
					   GdkEvent	*event,
					   gpointer	*data)
{
	gdouble x_win, y_win;
	gint x_buf, y_buf;
	GtkTextIter iter;
	GtkTextTag *link_tag;
	GtkTextTagTable *tag_table;
	GdkWindow *window;
	GdkCursor *pointer_cursor, *text_cursor;
	GdkDisplay *display;
	gdk_event_get_coords(event, &x_win, &y_win);

	gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(self),
					      GTK_TEXT_WINDOW_TEXT,
					      (gint) x_win,
					      (gint) y_win,
					      &x_buf,
					      &y_buf);
	gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(self),
					   &iter,
					   x_buf,
					   y_buf);
	tag_table = gtk_text_buffer_get_tag_table(SNMAN_LOG_TEXT_VIEW(self)->buffer);
	link_tag = gtk_text_tag_table_lookup (tag_table, "link");
	window = gtk_text_view_get_window(GTK_TEXT_VIEW(self), GTK_TEXT_WINDOW_TEXT);
	display = gtk_widget_get_display(self);
	if (gtk_text_iter_has_tag(&iter, link_tag)) {
		pointer_cursor = gdk_cursor_new_from_name(display, "pointer");
		gdk_window_set_cursor(window, pointer_cursor);
	} else {
		text_cursor = gdk_cursor_new_from_name(display, "text");
		gdk_window_set_cursor(window, text_cursor);
	}

	return FALSE;
}

void snman_log_text_view_scroll_to_bottom(GtkTextBuffer *buffer, SnmanLogTextView *log)
{
        GtkTextIter iter;
        GtkTextMark *mark;

        gtk_text_buffer_get_end_iter(buffer, &iter);
        mark = gtk_text_buffer_get_insert(buffer);
        gtk_text_buffer_move_mark(buffer,
                                  mark,
                                  &iter);
        gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(log),
                                     mark,
                                     0.0,
                                     TRUE, 0.0, 1.0);
}

static void snman_log_text_view_constructed(GObject *object)
{
	SnmanLogTextView *self = SNMAN_LOG_TEXT_VIEW(object);

	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(self), FALSE);

        self->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self));
	g_signal_connect(self->buffer,
			 "changed",
			 G_CALLBACK(snman_log_text_view_scroll_to_bottom),
			 self);

	GtkTextTag *link_tag = gtk_text_tag_new("link");
	g_object_set(link_tag,
		     "foreground",	"blue",
		     "underline",	PANGO_UNDERLINE_SINGLE,
		     NULL);
	GtkTextTagTable *tag_table;
	tag_table = gtk_text_buffer_get_tag_table (self->buffer);
	gtk_text_tag_table_add(tag_table, link_tag);

	gtk_widget_add_events(GTK_WIDGET(self), GDK_ENTER_NOTIFY_MASK);
	g_signal_connect(self,
			 "motion-notify-event",
			 G_CALLBACK(snman_log_text_view_motion),
			 NULL);

	gtk_widget_add_events(GTK_WIDGET(self), GDK_BUTTON_PRESS_MASK);
	g_signal_connect(self,
			 "button-press-event",
			 G_CALLBACK(snman_log_text_view_button_press),
			 NULL);
}

static void snman_log_text_view_class_init(SnmanLogTextViewClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS(class);

	object_class->set_property = snman_log_text_view_set_property;
	object_class->get_property = snman_log_text_view_get_property;
	object_class->constructed = snman_log_text_view_constructed;

	obj_properties[PROP_WINDOW] =
		g_param_spec_object("window",
				    "The SnmanWindow",
				    "The SnmanWindow to which this log belongs",
				    SNMAN_TYPE_WINDOW,
				    G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void snman_log_text_view_init(SnmanLogTextView *self)
{
}

SnmanLogTextView *snman_log_text_view_new(SnmanWindow *window)
{
	return g_object_new(SNMAN_TYPE_LOG_TEXT_VIEW, "window", window, NULL);
}

void snman_log_text_view_log(SnmanLogTextView *self, const gchar *format, ...)
{
	SnmanLogEntry *entry;
	va_list ap;
	va_start(ap, format);
	entry = snman_log_entry_new_with_vformat(format, ap);
	va_end(ap);

	GtkTextIter entry_start_iter;
	GtkTextMark *entry_start_mark;
	GtkTextIter link_start_iter;
	GtkTextIter link_end_iter;
	GSList *link;

	gtk_text_buffer_get_end_iter(self->buffer, &entry_start_iter);
	entry_start_mark = gtk_text_buffer_create_mark(self->buffer,
						       "entry1",
						       &entry_start_iter,
						       TRUE);
	gtk_text_buffer_insert(self->buffer, &entry_start_iter, snman_log_entry_get_message(entry), -1);

	gtk_text_buffer_get_iter_at_mark(self->buffer,
					 &entry_start_iter,
					 entry_start_mark);
	for (link = snman_log_entry_get_links(entry); link != NULL; link = link->next) {
		link_start_iter = entry_start_iter;
		gtk_text_iter_forward_chars(&link_start_iter, snman_log_link_get_start(link->data));
		link_end_iter = entry_start_iter;
		gtk_text_iter_forward_chars(&link_end_iter, snman_log_link_get_end(link->data));
		gtk_text_buffer_apply_tag_by_name(self->buffer,
						  "link",
						  &link_start_iter,
						  &link_end_iter);
	}

	GtkTextIter iter;
	GdkPixbuf *success_pixbuf;
	success_pixbuf = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(),
						  "emblem-ok-symbolic",
						  16,
						  0,
						  NULL);
	GdkPixbuf *failure_pixbuf;
	failure_pixbuf = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(),
						  "window-close-symbolic",
						  16,
						  0,
						  NULL);
	gtk_text_buffer_get_end_iter(self->buffer, &iter);
	gtk_text_buffer_insert_pixbuf(self->buffer,
				      &iter,
				      success_pixbuf);
	gtk_text_buffer_get_end_iter(self->buffer, &iter);
	gtk_text_buffer_insert_pixbuf(self->buffer,
				      &iter,
				      failure_pixbuf);
}

void snman_log_text_view_add_entry(SnmanLogTextView *self, SnmanLogEntry *entry)
{
}

SnmanLogEntry *snman_log_text_view_get_log_entry_at_iter(SnmanLogTextView *self, GtkTextIter *iter)
{
}
