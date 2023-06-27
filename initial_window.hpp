#pragma once

#include <gtkmm.h>

class InitialWindow : public Gtk::ApplicationWindow {
  public:
  InitialWindow();

  virtual ~InitialWindow() { }

  void onStartClicked();

  sigc::signal<void> startSignal();

  sigc::signal<void> exitSignal();

  private:
  Gtk::Button mButton;
  Gtk::Image mImage;
  Gtk::Box mBox;
  Gtk::Label mLabel;
  Gtk::Box mButtonBox;

  sigc::signal<void> mStartSignal;
  sigc::signal<void> mExitSignal;
};

