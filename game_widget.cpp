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

  mPlayerOneMarkDetection = { config.player_one().min_hsv(), config.player_one().max_hsv() };
  mPlayerTwoMarkDetection = { config.player_two().min_hsv(), config.player_two().max_hsv() };

  mImageBox.pack_start(mImage);
  add(mImageBox);

  mGame = std::make_unique<Game>(rng, mCamera.frame().rows, mCamera.frame().cols);

  add_tick_callback([&](const Glib::RefPtr<Gdk::FrameClock>& clock) {
    // This shit is in microseconds.
    double current_frame_time = clock->get_frame_time();
    double dt                 = (current_frame_time - m_last_frame_time) / 1'000'000;
    auto result               = onFrame(dt);
    m_last_frame_time         = current_frame_time;
    return result;
  });
}

const Game& GameWidget::game() const noexcept
{
  return *mGame;
}

bool GameWidget::onFrame(double dt) noexcept
{
  update(dt);
  draw();
  queue_resize();
  return true;
}

void GameWidget::update(double dt) noexcept
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
  mGame->update_particles(dt);
}

void GameWidget::draw() noexcept
{
  auto& config = Config::the();
  auto canvas  = mCamera.frame().clone();

  if (config.fake_background()) {
    canvas = cv::Mat(canvas.rows, canvas.cols, canvas.type(), cv::Scalar(255, 255, 255));
  }

  if (mPlayerOneMark && config.show_markers()) {
    drawRotatedRect(canvas, config.player_one().color(), mPlayerOneMark.value());
  }

  if (mPlayerTwoMark && config.show_markers()) {
    drawRotatedRect(canvas, config.player_two().color(), mPlayerTwoMark.value());
  }

  drawBubbles(canvas);
  draw_particles(canvas);

  // Flip around x-axis for display purposes.
  cv::flip(canvas, canvas, 1);

  if (mPlayerOneMark) {
    auto rect = mPlayerOneMark.value().boundingRect();
    draw_player_label(canvas, rect, mGame->playerOne().name());
  }

  if (mPlayerTwoMark) {
    auto rect = mPlayerTwoMark.value().boundingRect();
    draw_player_label(canvas, rect, mGame->playerTwo().name());
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

  auto width  = config.window_width();
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
  // We need to calculate the correct position in the flipped image.
  auto p    = r.tl();
  auto w    = canvas.cols;
  auto mid  = w / 2;
  auto diff = std::abs(mid - p.x);
  auto x    = p.x > mid ? mid - diff : mid + diff;
  cv::putText( //
      canvas,
      label,
      { x, p.y },
      cv::FONT_HERSHEY_SIMPLEX,
      0.5,                // The font scale
      cv::Scalar::all(0), // The font color
      2                   // The line thickness
  );
}

void GameWidget::draw_particles(cv::Mat& canvas) noexcept
{
  for (const auto& particle : mGame->particles()) {
    cv::circle(canvas, particle.pos, particle.size, particle.color, -1);
  }
}
