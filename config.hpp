#pragma once

#include <opencv2/core.hpp>

class Config final {
public:
  static Config& the();

  [[nodiscard]] int windowHeight() const noexcept;

  [[nodiscard]] int windowWidth() const noexcept;

  [[nodiscard]] std::string startScreenImageFile() const noexcept;

  [[nodiscard]] cv::Scalar playerOneMinHsv() const noexcept;

  [[nodiscard]] cv::Scalar playerOneMaxHsv() const noexcept;

  [[nodiscard]] cv::Scalar playerTwoMinHsv() const noexcept;

  [[nodiscard]] cv::Scalar playerTwoMaxHsv() const noexcept;

private:
  Config() = default;

  static Config* instance;
};
