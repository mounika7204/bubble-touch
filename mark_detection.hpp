#pragma once

#include <opencv2/core.hpp>

#include <optional>

class MarkDetection final {
public:
  MarkDetection(cv::Scalar min, cv::Scalar max);

  std::optional<cv::Rect> findMark(cv::Mat orig) noexcept;

private:
  cv::Scalar mMin;
  cv::Scalar mMax;
};
