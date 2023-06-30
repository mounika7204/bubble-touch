#include <cassert>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "config.hpp"
#include "game_widget.hpp"
#include "mark_detection.hpp"

GameWidget::GameWidget(cv::RNG& rng)
    : Gtk::Box {}
    , mPlayerOneMarkDetection(
          Config::the().playerOne().minHsv(), Config::the().playerOne().maxHsv()
      )
    , mPlayerTwoMarkDetection(
          Config::the().playerTwo().minHsv(), Config::the().playerTwo().maxHsv()
      )
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
  cv::Mat canvas(frame.rows, frame.cols, frame.type(), cv::Scalar(255, 255, 255));

  // TODO: Use rectangles to check for collisions in game.

  if (auto playerOneMark = mPlayerOneMarkDetection.findMark(frame.clone()); playerOneMark) {
    cv::rectangle(
        canvas, playerOneMark->tl(), playerOneMark->br(), Config::the().playerOne().color(), -1
    );
  }

  if (auto playerTwoMark = mPlayerTwoMarkDetection.findMark(frame.clone()); playerTwoMark) {
    cv::rectangle(
        canvas, playerTwoMark->tl(), playerTwoMark->br(), Config::the().playerTwo().color(), -1
    );
  }

  drawBubbles(canvas);

  cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);

  auto pixbuf = Gdk::Pixbuf::create_from_data(
      canvas.data, Gdk::COLORSPACE_RGB, false, 8, canvas.cols, canvas.rows, canvas.step
  );
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
