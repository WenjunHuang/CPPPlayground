//
// Created by rick on 2022/2/14.
//
#include <gtkmm.h>
#include <iostream>

class MyButton : public Gtk::Button {
public:
    ~MyButton() {
      std::cout << "I am destroyed" << std::endl;
    }
};

class MyWidget : public Gtk::Widget {
public:
    void adoptChild(Gtk::Widget *child) {
      child->set_parent(*this);
    }
};

class Window : public Gtk::Window {
public:
    Window() {
      auto button = Gtk::make_managed<MyButton>();
//      auto button = new MyButton{};
      button->set_label("Clicked");
//      _widget.adoptChild(button);
//
//      set_child(_widget);
      set_child(*button);
      set_default_size(300, 300);
    }

private:
    MyWidget _widget;
};

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("com.example");
  return app->make_window_and_run<Window>(argc, argv);
}