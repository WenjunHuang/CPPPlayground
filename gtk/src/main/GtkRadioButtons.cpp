//
// Created by xxzyjy on 2018/1/8.
//

#include "GtkRadioButtons.h"

GtkRadioButtons::GtkRadioButtons() :
  _boxTop(Gtk::ORIENTATION_VERTICAL),
  _box1(Gtk::ORIENTATION_VERTICAL, 10),
  _box2(Gtk::ORIENTATION_VERTICAL, 10),
  _radioButton1("button1"),
  _radioButton2("button2"),
  _radioButton3("button3"),
  _btnClose("close") {
  set_title("radio buttons");
  set_border_width(0);

  _radioButton2.join_group(_radioButton1);
  _radioButton3.join_group(_radioButton1);

  add(_boxTop);

  _boxTop.pack_start(_box1);
  _boxTop.pack_start(_separator);
  _boxTop.pack_start(_box2);

  _box2.set_border_width(10);
  _box1.set_border_width(10);

  _box1.pack_start(_radioButton1);
  _box1.pack_start(_radioButton2);
  _box1.pack_start(_radioButton3);

  _radioButton2.set_active();

  _box2.pack_start(_btnClose);

  _btnClose.set_can_default();
  _btnClose.grab_default();

  _btnClose.signal_clicked()
           .connect([=]() {
             this->hide();
           });
  show_all_children();
}

GtkRadioButtons::~GtkRadioButtons() {

}
