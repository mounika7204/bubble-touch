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

  [[nodiscard]] int imageRows() const noexcept;

  [[nodiscard]] int imageCols() const noexcept;

  [[nodiscard]] Game& game() noexcept;

private:
  [[nodiscard]] bool updateVideoFrame() noexcept;

  void drawBubbles(cv::Mat& frame) noexcept;

  VideoCamera mCamera;
  MarkDetection mBlueMarkDetection;
  MarkDetection mGreenMarkDetection;
  Gtk::Image mImage;
  Gtk::Box mImageBox;
  int mImageRows;
  int mImageCols;
  std::unique_ptr<Game> mGame;
};
