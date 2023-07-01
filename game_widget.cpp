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
  assert(mCamera.open() && "failed to open camera");
  assert(mCamera.read() && "failed to read first frame");

  mImageBox.pack_start(mImage);
  add(mImageBox);

  mGame = std::make_unique<Game>(rng, mCamera.frame().rows, mCamera.frame().cols);

  add_tick_callback([&](const Glib::RefPtr<Gdk::FrameClock>&) { return onFrame(); });
}

const Game& GameWidget::game() const noexcept
{
  return *mGame;
}

bool GameWidget::onFrame() noexcept
{
  update();
  draw();
  queue_resize();
  return true;
}

void GameWidget::update() noexcept
{
  assert(mCamera.isOpen() && "camera is not opened");
  assert(mCamera.read() && "failed to read camera frame");

  mPlayerOneMark = mPlayerOneMarkDetection.findMark(mCamera.frame());
  mPlayerTwoMark = mPlayerTwoMarkDetection.findMark(mCamera.frame());

  if (mPlayerOneMark) {
    mGame->checkCollisionsWithPlayerOne(mPlayerOneMark.value());
  }

  if (mPlayerTwoMark) {
    mGame->checkCollisionsWithPlayerTwo(mPlayerTwoMark.value());
  }

  mGame->floatBubbles();
}

void GameWidget::draw() noexcept
{
  auto& frame = mCamera.frame();
  cv::Mat canvas(frame.rows, frame.cols, frame.type(), cv::Scalar(255, 255, 255));

  if (mPlayerOneMark) {
    drawRotatedRect(canvas, Config::the().playerOne().color(), mPlayerOneMark.value());
  }

  if (mPlayerTwoMark) {
    drawRotatedRect(canvas, Config::the().playerTwo().color(), mPlayerTwoMark.value());
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
}

void GameWidget::drawRotatedRect(
    cv::Mat& frame, const cv::Scalar& color, cv::RotatedRect& rect
) noexcept
{
  cv::Point2f vertices2f[4];
  rect.points(vertices2f);

  cv::Point vertices[4];
  for (int i = 0; i < 4; i++) {
    vertices[i] = vertices2f[i];
  }

  cv::fillConvexPoly(frame, vertices, 4, color);
}

void GameWidget::drawBubbles(cv::Mat& frame) noexcept
{
  auto& bubbles = mGame->bubbles();
  for (auto& bubble : bubbles) {
    cv::circle(frame, cv::Point(bubble.x(), bubble.y()), bubble.radius(), bubble.color(), -1);
  }
}
