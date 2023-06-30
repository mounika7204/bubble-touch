#pragma once

#include <tuple>

#include <opencv2/core.hpp>

class Bubble {
public:
  using Bounds = std::tuple<int, int>;

  [[nodiscard]] static Bubble createRandom(cv::RNG& rng, Bounds bounds);

  [[nodiscard]] int x() const noexcept;

  [[nodiscard]] int y() const noexcept;

  [[nodiscard]] int radius() const noexcept;

  [[nodiscard]] cv::Scalar color() const noexcept;

  [[nodiscard]] cv::RotatedRect asRotatedRect() const noexcept;

private:
  Bubble(int xpos, int ypos, int radius, cv::Scalar color);

  int mXpos;
  int mYpos;
  int mRadius;
  cv::Scalar mColor;
};
