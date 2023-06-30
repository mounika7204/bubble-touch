#pragma once

#include <optional>

#include <opencv2/core.hpp>
#include <yaml-cpp/yaml.h>

class PlayerConfig {
public:
  PlayerConfig(cv::Scalar color, cv::Scalar minHsv, cv::Scalar maxHsv);

  [[nodiscard]] cv::Scalar color() const noexcept;

  [[nodiscard]] cv::Scalar minHsv() const noexcept;

  [[nodiscard]] cv::Scalar maxHsv() const noexcept;

private:
  cv::Scalar mColor;
  cv::Scalar mMinHsv;
  cv::Scalar mMaxHsv;
};

class Config final {
public:
  static Config& the();

  [[nodiscard]] int windowHeight() const noexcept;

  [[nodiscard]] int windowWidth() const noexcept;

  [[nodiscard]] std::string startScreenImageFile() const noexcept;

  [[nodiscard]] const PlayerConfig& playerOne() const noexcept;

  [[nodiscard]] const PlayerConfig& playerTwo() const noexcept;

private:
  Config();

  std::optional<std::string> findConfigFile() noexcept;

  PlayerConfig parsePlayerConfig(const YAML::Node& node) noexcept;

  static Config* instance;

  int mWindowHeight;
  int mWindowWidth;

  PlayerConfig mPlayerOneConfig;
  PlayerConfig mPlayerTwoConfig;
};
