//
// Created by rick on 2022/2/14.
//
#include <gtk/gtk.h>

#define TFE_TYPE_TEXT_VIEW (tfe_text_view_get_type())

G_DECLARE_FINAL_TYPE(TfeTextView, tfe_text_view, TFE, TEXT_VIEW, GtkTextView)

struct _TfeTextView {
    GtkTextView parent;
    GFile *file;
};

G_DEFINE_TYPE(TfeTextView, tfe_text_view, GTK_TYPE_TEXT_VIEW)

static void
tfe_text_view_init(TfeTextView *tv) {}

static void
tfe_text_view_class_init(TfeTextViewClass *class) {}

void
tfe_text_view_set_file(TfeTextView *tv, GFile *f) {
  tv->file = f;
}

GFile *
tfe_text_view_get_file(TfeTextView *tv) {
  return tv->file;
}

GtkWidget *
tfe_text_view_new(void) {
  return GTK_WIDGET(g_object_new(TFE_TYPE_TEXT_VIEW, NULL));
}

static void
app_activate(GApplication *app, gpointer user_data) {
  g_print("You need to give filenames as arguments.\n");
}

static gboolean
before_close(GtkWindow* win,gpointer user_data){

}

static void
app_open(GApplication *app,GFile **files,gint n_files,gchar *hint,gpointer user_data) {
  GtkWindow *win = GTK_WINDOW(gtk_application_window_new(GTK_APPLICATION(app)));
  gtk_window_set_title(win,"file editor");
  gtk_window_maximize(win);

  GtkNotebook *nb = GTK_NOTEBOOK(gtk_notebook_new());
  gtk_window_set_child(win,GTK_WIDGET(nb));

  for (int i = 0;i<n_files;i++) {
    char *contents;
    gsize length;
    char *filename;
    if (g_file_load_contents(files[i],NULL,&contents,&length,NULL,NULL)){
      GtkScrolledWindow *scr = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
      TfeTextView *tv = TFE_TEXT_VIEW(tfe_text_view_new());
      GtkTextBuffer *tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
      gtk_scrolled_window_set_child(scr,GTK_WIDGET(tv));

      tfe_text_view_set_file(tv,g_file_dup(files[i]));
      gtk_text_buffer_set_text(tb,contents,length);
      g_free(contents);

      filename = g_file_get_basename(files[i]);
      GtkLabel* lab = GTK_LABEL(gtk_label_new(filename));
      gtk_notebook_append_page(nb,GTK_WIDGET(scr),GTK_WIDGET(lab));

      GtkNotebookPage* nbp = gtk_notebook_get_page(nb,GTK_WIDGET(scr));
      g_object_set(nbp,"tab-expand",TRUE,NULL);
      g_free(filename);
    } else if ((filename = g_file_get_path(files[i])) != NULL) {
      g_print("No such file: %s.\n",filename);
      g_free(filename);
    } else
      g_print("No valid file is given\n");
  }

  if (gtk_notebook_get_n_pages(nb) > 0) {
    g_signal_connect(win,"close-request",G_CALLBACK(before_close),nb);
    gtk_widget_show(GTK_WIDGET(win));
  } else
    gtk_window_destroy(win);
}

int main(int argc, char *argv[]) {
  GtkApplication *app = GTK_APPLICATION(gtk_application_new("com.example", G_APPLICATION_HANDLES_OPEN));
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
  g_signal_connect(app, "open", G_CALLBACK(app_open), NULL);

  int stat = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return stat;
}