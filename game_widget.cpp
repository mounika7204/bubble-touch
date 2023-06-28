#include <cassert>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "config.hpp"
#include "game_widget.hpp"
#include "mark_detection.hpp"

GameWidget::GameWidget()
    : Gtk::Box {}
    , mBlueMarkDetection(Config::the().playerOneMinHsv(), Config::the().playerOneMaxHsv())
    , mGreenMarkDetection(Config::the().playerTwoMinHsv(), Config::the().playerTwoMaxHsv())
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

  auto& frame = mCamera.frame();

  // TODO: Use rectangles to check for collisions in game.

  if (auto blueMark = mBlueMarkDetection.findMark(frame.clone()); blueMark) {
    auto color = cv::Scalar(255, 0, 0);
    cv::rectangle(frame, blueMark->tl(), blueMark->br(), color, 2);
  }

  if (auto greenMark = mGreenMarkDetection.findMark(frame.clone()); greenMark) {
    auto color = cv::Scalar(0, 255, 0);
    cv::rectangle(frame, greenMark->tl(), greenMark->br(), color, 2);
  }

  auto pixbuf = mCamera.getFrameAsPixbuf();
  auto width  = Config::the().windowWidth();
  auto height = (pixbuf->get_height() * width) / pixbuf->get_width();

  pixbuf = pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);
  mImage.set(pixbuf->flip());

  queue_resize();

  return true;
}
