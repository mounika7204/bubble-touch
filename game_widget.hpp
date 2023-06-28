#pragma once

#include <gtkmm.h>

#include "videocamera.hpp"

class GameWidget : public Gtk::Box {
public:
  GameWidget();

  virtual ~GameWidget() { }

private:
  [[nodiscard]] bool updateVideoFrame() noexcept;

  VideoCamera mCamera;
  Gtk::Image mImage;
  Gtk::Box mImageBox;
};
