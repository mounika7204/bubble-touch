#include <cassert>
#include <iostream>

#include "config.hpp"
#include "game_widget.hpp"

GameWidget::GameWidget()
    : Gtk::Box {}
{
  mImageBox.pack_start(mImage);
  add(mImageBox);
  assert(mCamera.open() && "failed to open camera");
  add_tick_callback([&](const Glib::RefPtr<Gdk::FrameClock>&) { return updateVideoFrame(); });
}

bool GameWidget::updateVideoFrame() noexcept
{
  assert(mCamera.isOpen() && "camera is not opened");

  if (!mCamera.read()) {
    std::cerr << "error: failed to read camera frame\n";
    return false;
  }

  auto pixbuf = mCamera.getFrameAsPixbuf();
  auto width  = APP_WINDOW_WIDTH;
  auto height = (pixbuf->get_height() * width) / pixbuf->get_width();

  pixbuf = pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);
  mImage.set(pixbuf->flip());

  queue_resize();

  return true;
}
