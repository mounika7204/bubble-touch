#include <cassert>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "config.hpp"
#include "game_widget.hpp"
#include "mark_detection.hpp"

GameWidget::GameWidget(cv::RNG& rng)
    : Gtk::Box {}
{
  assert(mCamera.open() && "failed to open camera");
  assert(mCamera.read() && "failed to read first frame");

  auto& config = Config::the();

  mPlayerOneMarkDetection = { config.playerOne().minHsv(), config.playerOne().maxHsv() };
  mPlayerTwoMarkDetection = { config.playerTwo().minHsv(), config.playerTwo().maxHsv() };

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
  auto& config = Config::the();
  auto canvas  = mCamera.frame().clone();

  if (config.fake_background()) {
    canvas = cv::Mat(canvas.rows, canvas.cols, canvas.type(), cv::Scalar(255, 255, 255));
  }

  if (mPlayerOneMark && config.show_markers()) {
    drawRotatedRect(canvas, config.playerOne().color(), mPlayerOneMark.value());
  }

  if (mPlayerTwoMark && config.show_markers()) {
    drawRotatedRect(canvas, config.playerTwo().color(), mPlayerTwoMark.value());
  }

  drawBubbles(canvas);

  // Flip around x-axis for display purposes.
  cv::flip(canvas, canvas, 1);

  if (mPlayerOneMark) {
    auto rect = mPlayerOneMark.value().boundingRect();
    draw_player_label(canvas, rect, "Player 1");
  }

  if (mPlayerTwoMark) {
    auto rect = mPlayerTwoMark.value().boundingRect();
    draw_player_label(canvas, rect, "Player 2");
  }

  cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);

  auto pixbuf = Gdk::Pixbuf::create_from_data(canvas.data, //
      Gdk::COLORSPACE_RGB,
      false,
      8,
      canvas.cols,
      canvas.rows,
      canvas.step //
  );

  auto width  = config.windowWidth();
  auto height = (pixbuf->get_height() * width) / pixbuf->get_width();

  pixbuf = pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);

  mImage.set(pixbuf);
}

void GameWidget::drawRotatedRect( //
    cv::Mat& frame,
    const cv::Scalar& color,
    cv::RotatedRect& rect) noexcept
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

void GameWidget::draw_player_label( //
    cv::Mat& canvas,                //
    const cv::Rect& r,
    const std::string& label) noexcept
{
  // This is for the original image.
  // We need to calculate the correct position in the rotated image.
  auto p    = r.tl();
  auto w    = canvas.cols;
  auto mid  = w / 2;
  auto diff = std::abs(mid - p.x);
  auto x    = p.x > mid ? mid - diff : mid + diff;
  cv::putText(canvas, label, { x, p.y }, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all(0));
}
