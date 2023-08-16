#include <gdkmm/pixbuf.h>

#include <opencv2/opencv.hpp>

#include "config.hpp"
#include "initial_window.hpp"

InitialWindow::InitialWindow()
    : mExitButton("Salir")
    , mStartButton("Comenzar")
    , mButtonBox(Gtk::ORIENTATION_VERTICAL, 5)
    , mLabel("¡Haz clic para empezar!")
{
  set_title("BubbleTouch");
  set_default_size(Config::the().window_width(), Config::the().window_height());
  set_resizable(false);

  set_name("initial-window");

  auto pixbuf = Gdk::Pixbuf::create_from_file(Config::the().start_screen_image_file());
  auto height = Config::the().window_height();
  auto width  = (pixbuf->get_width() * height) / pixbuf->get_height();
  pixbuf      = pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);
  mImage.set(pixbuf);

  mLabel.set_margin_left(10);
  mLabel.set_margin_right(10);

  mExitButton.signal_clicked().connect([&]() { mExitSignal.emit(); });
  mStartButton.signal_clicked().connect([&]() { mStartSignal.emit(); });

  mButtonBox.set_name("initial-window-button-box");

  mButtonBox.set_vexpand(false);
  mButtonBox.set_valign(Gtk::Align::ALIGN_CENTER);
  mButtonBox.set_halign(Gtk::Align::ALIGN_CENTER);

  mButtonBox.pack_end(mExitButton);
  mButtonBox.pack_end(mStartButton);
  mButtonBox.pack_end(mLabel);

  mBox.set_name("initial-window-box");
  mBox.pack_start(mImage);
  mBox.pack_start(mButtonBox);

  add(mBox);

  show_all();
}

sigc::signal<void> InitialWindow::startSignal()
{
  return mStartSignal;
}

sigc::signal<void> InitialWindow::exitSignal()
{
  return mExitSignal;
}
