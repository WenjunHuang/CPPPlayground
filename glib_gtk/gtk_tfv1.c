//
// Created by rick on 2022/2/14.
//
#include <gtk/gtk.h>


static void app_active(GtkApplication *app, gpointer user_data) {
  gchar *text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  GtkWindow *win = GTK_WINDOW(gtk_application_window_new(app));
  gtk_window_set_title(win, "Taketori");
  gtk_window_set_default_size(win, 400, 300);

  GtkTextView *tv = GTK_TEXT_VIEW(gtk_text_view_new());
  GtkTextBuffer *tb = gtk_text_view_get_buffer(tv);
//  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tv), GTK_WRAP_WORD);
  gtk_text_buffer_set_text(tb, text, -1);

  GtkScrolledWindow *scr = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
  gtk_scrolled_window_set_child(scr,GTK_WIDGET(tv));

  gtk_window_set_child(win, GTK_WIDGET(scr));
  gtk_widget_show(GTK_WIDGET(win));
}

int main(int argc, char *argv[]) {
  GtkApplication *app = gtk_application_new("com.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(app_active), NULL);

  int stat = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return stat;
}
