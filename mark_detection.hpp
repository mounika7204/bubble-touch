#pragma once

#include <opencv2/core.hpp>

#include <optional>

class MarkDetection final {
public:
  MarkDetection(cv::Scalar min = cv::Scalar(0), cv::Scalar max = cv::Scalar(255));

  std::optional<cv::RotatedRect> findMark(const cv::Mat& orig) noexcept;

private:
  cv::Scalar mMin;
  cv::Scalar mMax;
};
