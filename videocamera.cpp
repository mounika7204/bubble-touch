#include <opencv2/imgproc.hpp>

#include "videocamera.hpp"

VideoCamera::~VideoCamera()
{
  if (mCapture.isOpened()) {
    mCapture.release();
  }
}

bool VideoCamera::open() noexcept
{
  mCapture.open(0, cv::CAP_ANY);
  return mCapture.isOpened();
}

bool VideoCamera::isOpen() const noexcept
{
  return mCapture.isOpened();
}

bool VideoCamera::read() noexcept
{
  mCapture.read(mFrame);
  return !mFrame.empty();
}

cv::Mat& VideoCamera::frame() noexcept
{
  return mFrame;
}

[[nodiscard]] Glib::RefPtr<Gdk::Pixbuf> VideoCamera::getFrameAsPixbuf() const noexcept
{
  cv::cvtColor(mFrame, mFrame, cv::COLOR_BGR2RGB);
  auto pixbuf = Gdk::Pixbuf::create_from_data(
      mFrame.data, Gdk::COLORSPACE_RGB, false, 8, mFrame.cols, mFrame.rows, mFrame.step
  );
  return pixbuf;
}
