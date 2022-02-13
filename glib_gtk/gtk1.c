//
// Created by rick on 2022/2/12.
//

#include <gtk/gtk.h>
static void app_activate(GApplication *app,gpointer *user_data){
g_print("GtkApplication is activated.\n");
}
int main(int argc,char *argv[]) {
  GtkApplication *app = gtk_application_new("com.example",
                                            G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app,"activate",G_CALLBACK(app_activate),NULL);
  int stat = g_application_run(G_APPLICATION(app),argc,argv);
  g_object_unref(app);

  return stat;
}