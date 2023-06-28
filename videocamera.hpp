#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include <gdkmm/pixbuf.h>

class VideoCamera {
public:
  VideoCamera() = default;

  virtual ~VideoCamera();

  [[nodiscard]] bool open() noexcept;

  [[nodiscard]] bool isOpen() const noexcept;

  [[nodiscard]] bool read() noexcept;

  [[nodiscard]] cv::Mat& frame() noexcept;

  [[nodiscard]] Glib::RefPtr<Gdk::Pixbuf> getFrameAsPixbuf() const noexcept;

private:
  cv::Mat mFrame            = {};
  cv::VideoCapture mCapture = {};
};
