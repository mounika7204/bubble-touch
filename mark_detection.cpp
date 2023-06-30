#include <algorithm>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "mark_detection.hpp"

MarkDetection::MarkDetection(cv::Scalar min, cv::Scalar max)
    : mMin { min }
    , mMax { max }
{
}

std::optional<cv::RotatedRect> MarkDetection::findMark(const cv::Mat& orig) noexcept
{
  // Change to the HSV color space
  cv::Mat hsv;
  cv::cvtColor(orig, hsv, cv::COLOR_BGR2HSV);

  // Create a mask that finds objects of a given hue
  cv::Mat mask;
  cv::inRange(hsv, mMin, mMax, mask);

  // Apply morphological operations to get rid of noise
  cv::Mat kernel(3, 3, CV_8U, cv::Scalar(3));
  cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

  // Find the contours
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

  if (contours.empty()) {
    return {};
  }

  std::sort(contours.begin(), contours.end(), [](auto cnt1, auto cnt2) {
    return cv::contourArea(cnt1) > cv::contourArea(cnt2);
  });

  // Find the bounding rect
  cv::RotatedRect rect = cv::minAreaRect(contours[0]);

  if (rect.size.area() < 1000) {
    return {};
  }

  return rect;
}
