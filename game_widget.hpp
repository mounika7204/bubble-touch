#pragma once

#include <memory>

#include <gtkmm.h>
#include <opencv2/core.hpp>

#include "game.hpp"
#include "mark_detection.hpp"
#include "videocamera.hpp"

class GameWidget : public Gtk::Box {
public:
  GameWidget(cv::RNG&);

  virtual ~GameWidget() { }

  [[nodiscard]] const Game& game() const noexcept;

private:
  [[nodiscard]] bool onFrame(double dt) noexcept;

  void update(double dt) noexcept;

  void draw() noexcept;

  void drawRotatedRect(cv::Mat& frame, const cv::Scalar& color, cv::RotatedRect& rect) noexcept;

  void drawBubbles(cv::Mat& frame) noexcept;

  void draw_particles(cv::Mat& canvas) noexcept;

  void draw_player_label(cv::Mat&, const cv::Rect&, const std::string&) noexcept;

  VideoCamera mCamera;

  std::optional<cv::RotatedRect> mPlayerOneMark;
  std::optional<cv::RotatedRect> mPlayerTwoMark;

  MarkDetection mPlayerOneMarkDetection;
  MarkDetection mPlayerTwoMarkDetection;

  Gtk::Image mImage;
  Gtk::Box mImageBox;

  std::unique_ptr<Game> mGame;

  double m_last_frame_time = 0;
};
