#pragma once

#include <optional>
#include <thread>

#include <opencv2/core.hpp>
#include <yaml-cpp/yaml.h>

#include "file_watcher.hpp"

class Player_Config {
public:
  Player_Config(cv::Scalar color, cv::Scalar minHsv, cv::Scalar maxHsv);

  [[nodiscard]] cv::Scalar color() const noexcept;

  [[nodiscard]] cv::Scalar min_hsv() const noexcept;

  [[nodiscard]] cv::Scalar max_hsv() const noexcept;

private:
  cv::Scalar m_color;
  cv::Scalar m_min_hsv;
  cv::Scalar m_max_hsv;
};

class Config final {
public:
  static Config& the();

  ~Config();

  [[nodiscard]] int window_height() const noexcept;

  [[nodiscard]] int window_width() const noexcept;

  [[nodiscard]] std::string start_screen_image_file() const noexcept;

  [[nodiscard]] const Player_Config& player_one() const noexcept;

  [[nodiscard]] const Player_Config& player_two() const noexcept;

  [[nodiscard]] bool fake_background() const noexcept;

  [[nodiscard]] bool show_markers() const noexcept;

private:
  Config();

  /// Resolve the path to the configuration file.
  std::optional<std::string> find_config_file() noexcept;

  /// Parse and return a Player_Config.
  Player_Config parse_player_config(const YAML::Node& node) noexcept;

  /// Reload the configuration.
  void reload() noexcept;

  static Config* instance;

  int m_window_height;
  int m_window_width;

  Player_Config m_player_one_config;
  Player_Config m_player_two_config;

  bool m_fake_background = false;
  bool m_show_markers    = false;

  std::mutex m_condomn          = {};
  File_Watcher m_config_watcher = {};
};
