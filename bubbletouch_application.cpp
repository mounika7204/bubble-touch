#include <iostream>

#include "bubbletouch_application.hpp"

BubbleTouchApplication::BubbleTouchApplication()
    : Gtk::Application("io.github.aloussase.bubbletouch")
{
}

std::shared_ptr<BubbleTouchApplication> BubbleTouchApplication::create()
{
  return std::make_shared<BubbleTouchApplication>();
}

void BubbleTouchApplication::on_activate()
{
  mInitialWindow.startSignal().connect([]() { std::cout << "Comenzando\n"; });

  add_window(mInitialWindow);
  mInitialWindow.present();
}
