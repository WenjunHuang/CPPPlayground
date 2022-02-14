//
// Created by rick on 2022/2/11.
//
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/application.h>
#include <iostream>

class HelloWorld: public Gtk::Window {
public:
    HelloWorld():_button{"Hello World"} {
      _button.set_margin(10);
      _button.signal_clicked().connect(sigc::mem_fun(*this,
                                                     &HelloWorld::onButtonClicked));
      set_child(_button);
    }

private:
    void onButtonClicked(){
      std::cout << "Hello World" << std::endl;

    }
    Gtk::Button _button;
};

int main(int argc,char *argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.example");
  return app->make_window_and_run<HelloWorld>(argc,argv);

}

