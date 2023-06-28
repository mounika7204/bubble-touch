#include "config.hpp"

Config* Config::instance = nullptr;

Config& Config::the()
{
  if (instance == nullptr) {
    instance = new Config;
  }
  return *instance;
}

int Config::windowHeight() const noexcept
{
  return 500;
}

int Config::windowWidth() const noexcept
{
  return 800;
}

std::string Config::startScreenImageFile() const noexcept
{
  return "assets/startscreen.png";
}

cv::Scalar Config::playerOneMinHsv() const noexcept
{
  return cv::Scalar(90, 50, 70);
}

cv::Scalar Config::playerOneMaxHsv() const noexcept
{
  return cv::Scalar(128, 255, 255);
}

cv::Scalar Config::playerTwoMinHsv() const noexcept
{
  return cv::Scalar(36, 50, 70);
}

cv::Scalar Config::playerTwoMaxHsv() const noexcept
{
  return cv::Scalar(89, 255, 255);
}
