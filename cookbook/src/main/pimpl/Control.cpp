//
// Created by xxzyjy on 30/09/2017.
//

#include "Control.h"

#include <string>
#include <iostream>

class Control_Pimpl {
  std::string text;
  int width = 0;
  int height = 0;
  bool visible = true;

  void draw() {
    std::cout
      << "control " << std::endl
      << " visible: " << std::boolalpha << visible << std::noboolalpha << std::end;
    std::cout << " size:" << width << "," << height << std::endl;
    std::cout << " text:" << text << std::endl;
  }

public:
  void set_text(std::string_view t) {
    text = t.data();
    draw();
  }

  void resize(int const w, int const h) {
    width = w;
    height = h;
    draw();
  }

  void show() {
    visible = true;
    draw();
  }

  void hide() {
    visible = false;
    draw();
  }
};

Control::Control(): pimpl(new Control_Pimpl(),
        [](Control_Pimpl *pimpl) { delete pimpl; }) {

}

void Control::set_text(std::string_view t) {
  pimpl->set_text(t);
}

void Control::resize(int const w, int const h) {
  pimpl->resize(w,h);

}

void Control::show() {
  pimpl->show();
}

void Control::hide() {
  pimpl->hide();
}
