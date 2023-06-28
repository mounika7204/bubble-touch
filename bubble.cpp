
#include "bubble.hpp"

Bubble::Bubble(int xpos, int ypos, int radius, cv::Scalar color)
    : mXpos { xpos }
    , mYpos { ypos }
    , mRadius { radius }
    , mColor { color }
{
}

Bubble Bubble::createRandom(cv::RNG& rng, Bounds bounds)
{
  auto radius = rng.uniform(20, 50);
  auto x      = rng.uniform(radius / 2, std::get<0>(bounds) - radius / 2);
  auto y      = rng.uniform(radius / 2, std::get<1>(bounds) - radius / 2);
  auto icolor = (unsigned)rng;
  auto color  = cv::Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
  return Bubble { x, y, radius, color };
}

int Bubble::x() const noexcept
{
  return mXpos;
}

int Bubble::y() const noexcept
{
  return mYpos;
}

int Bubble::radius() const noexcept
{
  return mRadius;
}

cv::Scalar Bubble::color() const noexcept
{
  return mColor;
}
