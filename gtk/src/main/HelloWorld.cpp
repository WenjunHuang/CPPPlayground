//
// Created by xxzyjy on 2018/1/8.
//

#include <iostream>
#include "HelloWorld.h"

HelloWorld::HelloWorld() {
//  _button.add_pixlabel("icon80-home.png", "cool button");
  set_title("Pixmap'd buttons!");
  set_border_width(10);

//  _button.signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));
//  add(_button);

  _checkButton.set_label("something");
  _checkButton.signal_clicked().connect([]() {
    std::cout << "check button clicked" << std::endl;
  });
  add(_checkButton);

  show_all_children();
}

HelloWorld::~HelloWorld() {

}

void HelloWorld::on_button_clicked() {
  std::cout << "Hello World" << std::endl;

}
