//
// Created by xxzyjy on 30/09/2017.
//

#ifndef BOOSTPLAYGROUND_CONTROL_H
#define BOOSTPLAYGROUND_CONTROL_H

#include <memory>
#include <string_view>

class Control_Pimpl;

class Control {
  std::unique_ptr<Control_Pimpl, void (*)(Control_Pimpl *)> pimpl;


public:
  Control();

  void set_text(std::string_view t);

  void resize(int const w, int const h);

  void show();

  void hide();
};


#endif //BOOSTPLAYGROUND_CONTROL_H
