//
// Created by rick on 2022/2/12.
//

#include <gtk/gtk.h>

static void click_cb(GtkButton *btn, gpointer user_data) {
  GtkWindow *win = GTK_WINDOW(user_data);
  if (win)
    gtk_window_destroy(win);
}

static void app_activate(GApplication *app, gpointer *user_data) {
  GtkWindow *win = GTK_WINDOW(gtk_application_window_new(GTK_APPLICATION(app)));
  gtk_window_set_title(win, "pr4");
  gtk_window_set_default_size(win, 400, 300);

  GtkWidget *btn = gtk_button_new_with_label("Click me");
  gtk_window_set_child(win, btn);
  g_signal_connect(btn, "clicked", G_CALLBACK(click_cb), win);

  gtk_widget_show(GTK_WIDGET(win));
}

int main(int argc, char *argv[]) {
  GtkApplication *app = gtk_application_new("com.example",
                                            G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
  int stat = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return stat;
}