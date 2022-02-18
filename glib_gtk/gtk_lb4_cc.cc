//
// Created by rick on 2022/2/14.
//

#include <gtkmm.h>

class BoxButton : public Gtk::Window {
public:
    BoxButton() {
      set_title("lb4");
      set_default_size(400, 300);

      _box.set_orientation(Gtk::Orientation::VERTICAL);
      _box.set_spacing(5);
      _box.set_homogeneous();
      _box.append(_button1);
      _box.append(_button2);
      _button1.set_label("Hello.");
      _button1.signal_clicked().connect(sigc::mem_fun(*this,&BoxButton::onButton1Clicked));

      _button2.set_label("Quit");
      _button2.signal_clicked().connect(sigc::mem_fun(*this, &BoxButton::onButton2Clicked));

      set_child(_box);
    }

private:
    void onButton1Clicked() {
      auto title = _button1.get_label();
      if (title == "Hello.") {
        _button1.set_label("Good-bye");
      } else
        _button1.set_label("Hello.");
    }

    void onButton2Clicked() {
      destroy_();
    }

    Gtk::Box _box;
    Gtk::Button _button1;
    Gtk::Button _button2;
};

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("com.example");

  return app->make_window_and_run<BoxButton>(argc, argv);

}