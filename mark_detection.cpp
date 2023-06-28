#include <algorithm>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "mark_detection.hpp"

MarkDetection::MarkDetection(cv::Scalar min, cv::Scalar max)
    : mMin { min }
    , mMax { max }
{
}

std::optional<cv::Rect> MarkDetection::findMark(cv::Mat orig) noexcept
{
  // Change to the HSV color space
  cv::Mat hsv;
  cv::cvtColor(orig, hsv, cv::COLOR_BGR2HSV);

  // Create a mask that finds objects of a given hue
  cv::Mat mask;
  cv::inRange(hsv, mMin, mMax, mask);

  // TODO: Apply morphological operations to get rid of noise

  // Find the contours
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
  std::sort(contours.begin(), contours.end(), [](auto cnt1, auto cnt2) {
    return cv::contourArea(cnt1) > cv::contourArea(cnt2);
  });

  // Find the bounding rect
  cv::Rect boundingRect = cv::boundingRect(contours[0]);

  return boundingRect;
}
