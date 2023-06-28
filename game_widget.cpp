#include <cassert>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "config.hpp"
#include "game_widget.hpp"
#include "mark_detection.hpp"

GameWidget::GameWidget(cv::RNG& rng)
    : Gtk::Box {}
    , mBlueMarkDetection(Config::the().playerOneMinHsv(), Config::the().playerOneMaxHsv())
    , mGreenMarkDetection(Config::the().playerTwoMinHsv(), Config::the().playerTwoMaxHsv())
{
  mImageBox.pack_start(mImage);
  add(mImageBox);

  assert(mCamera.open() && "failed to open camera");
  assert(mCamera.read() && "failed to read first frame");

  mImageRows = mCamera.frame().rows;
  mImageCols = mCamera.frame().cols;

  mGame = std::make_unique<Game>(rng, mImageRows, mImageCols);

  add_tick_callback([&](const Glib::RefPtr<Gdk::FrameClock>&) { return updateVideoFrame(); });
}

Game& GameWidget::game() noexcept
{
  return *mGame;
}

int GameWidget::imageRows() const noexcept
{
  return mImageRows;
}

int GameWidget::imageCols() const noexcept
{
  return mImageCols;
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

  drawBubbles(frame);

  auto pixbuf = mCamera.getFrameAsPixbuf();
  auto width  = Config::the().windowWidth();
  auto height = (pixbuf->get_height() * width) / pixbuf->get_width();

  pixbuf = pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);
  mImage.set(pixbuf->flip());

  queue_resize();

  return true;
}

void GameWidget::drawBubbles(cv::Mat& frame) noexcept
{
  auto& bubbles = mGame->bubbles();
  for (auto& bubble : bubbles) {
    cv::circle(frame, cv::Point(bubble.x(), bubble.y()), bubble.radius(), bubble.color(), -1);
  }
}
