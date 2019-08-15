#pragma once
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/window.h>


class HelloWorld : public Gtk::Window{
public:
  HelloWorld();
  virtual ~HelloWorld();

protected:
  void on_button_clicked();

  Gtk::Button _button;
  Gtk::CheckButton _checkButton;

};



