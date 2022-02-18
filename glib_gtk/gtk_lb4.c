//
// Created by rick on 2022/2/14.
//
#include <gtk/gtk.h>

static void click1_cb(GtkButton *button, gpointer user_data) {
  const gchar *s = gtk_button_get_label(button);
  if (g_strcmp0(s, "Hello.") == 0)
    gtk_button_set_label(button, "Good-bye.");
  else
    gtk_button_set_label(button, "Hello.");
}

static void click2_cb(GtkButton *button,gpointer user_data){
  GtkWindow *win = GTK_WINDOW(user_data);
  if (win){
    gtk_window_destroy(win);
  }
}

static void app_active(GtkApplication *app) {
  GtkWindow *win = GTK_WINDOW(gtk_application_window_new(app));
  gtk_window_set_title(win, "lb4");
  gtk_window_set_default_size(win, 400, 300);

  GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 5));
  gtk_box_set_homogeneous(box, TRUE);
  gtk_window_set_child(win, GTK_WIDGET(box));

  GtkButton *btn1 = GTK_BUTTON(gtk_button_new_with_label("Hello."));
  g_signal_connect(btn1, "clicked", G_CALLBACK(click1_cb), NULL);

  GtkButton *btn2 = GTK_BUTTON(gtk_button_new_with_label("Quit"));
  g_signal_connect(btn2, "clicked", G_CALLBACK(click2_cb), win);

  gtk_box_append(box, GTK_WIDGET(btn1));
  gtk_box_append(box, GTK_WIDGET(btn2));

  gtk_widget_show(GTK_WIDGET(win));

}

int main(int argc, char *argv[]) {
  GtkApplication *app = gtk_application_new("com.example",
                                            G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(app_active), NULL);
  int stat = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return stat;
}