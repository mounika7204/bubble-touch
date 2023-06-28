#pragma once

#include <gtkmm.h>

#include "mark_detection.hpp"
#include "videocamera.hpp"

class GameWidget : public Gtk::Box {
public:
  GameWidget();

  virtual ~GameWidget() { }

private:
  [[nodiscard]] bool updateVideoFrame() noexcept;

  VideoCamera mCamera;
  MarkDetection mBlueMarkDetection;
  MarkDetection mGreenMarkDetection;
  Gtk::Image mImage;
  Gtk::Box mImageBox;
};
