//
// Created by xxzyjy on 2018/1/7.
//
#include <gtkmm.h>
#include "HelloWorld.h"
#include "GtkRadioButtons.h"

int main(int argc, char *argv[]) {
  auto app =
    Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

  GtkRadioButtons buttons;
  return app->run(buttons);
}
