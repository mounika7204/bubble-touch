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
  set_default_size(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);

  auto pixbuf = Gdk::Pixbuf::create_from_file(STARTSCREEN_IMAGE_FILE_NAME);
  auto height = APP_WINDOW_HEIGHT;
  auto width  = (pixbuf->get_width() * height) / pixbuf->get_height();
  pixbuf      = pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);
  mImage.set(pixbuf);

  mLabel.set_margin_left(10);
  mLabel.set_margin_right(10);

  mExitButton.signal_clicked().connect([&]() { mExitSignal.emit(); });
  mStartButton.signal_clicked().connect([&]() { mStartSignal.emit(); });

  mButtonBox.set_vexpand(false);
  mButtonBox.set_valign(Gtk::Align::ALIGN_CENTER);
  mButtonBox.set_margin_left(5);
  mButtonBox.set_margin_right(5);

  mButtonBox.pack_end(mExitButton);
  mButtonBox.pack_end(mStartButton);
  mButtonBox.pack_end(mLabel);

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
