#pragma once


#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/separator.h>

class GtkRadioButtons : public Gtk::Window {
public:
  GtkRadioButtons();

  virtual ~GtkRadioButtons();

protected:

  Gtk::Box _boxTop, _box1, _box2;
  Gtk::RadioButton _radioButton1, _radioButton2, _radioButton3;
  Gtk::Separator _separator;
  Gtk::Button _btnClose;

};



