#pragma once

#include <gtkmm.h>
#include <memory>

#include "initial_window.hpp"

class BubbleTouchApplication : public Gtk::Application {
  public:
  BubbleTouchApplication();

  static std::shared_ptr<BubbleTouchApplication> create();

  protected:
  void on_activate() override;

  private:
  InitialWindow mInitialWindow;
};
