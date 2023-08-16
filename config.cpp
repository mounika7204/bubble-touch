#include <cstdlib>
#include <iostream>
#include <utility>

#include <yaml-cpp/yaml.h>

#include "config.hpp"
#include "file_watcher.hpp"

Player_Config::Player_Config(cv::Scalar color, cv::Scalar minHsv, cv::Scalar maxHsv)
    : m_color { std::move(color) }
    , m_min_hsv { std::move(minHsv) }
    , m_max_hsv { std::move(maxHsv) }
{
}

cv::Scalar Player_Config::color() const noexcept
{
  return m_color;
}

cv::Scalar Player_Config::min_hsv() const noexcept
{
  return m_min_hsv;
}

cv::Scalar Player_Config::max_hsv() const noexcept
{
  return m_max_hsv;
}

Config* Config::instance = nullptr;

Config::Config()
    : m_window_height { 500 }
    , m_window_width { 800 }
    , m_player_one_config { cv::Scalar(255, 0, 0),
      cv::Scalar(90, 50, 70),
      cv::Scalar(128, 255, 255) }
    , m_player_two_config {
      cv::Scalar(0, 255, 0), cv::Scalar(36, 50, 70), cv::Scalar(89, 255, 255)
    }
{
  reload();

  if (auto config_file = find_config_file(); config_file) {
    m_config_watcher.watch(config_file->c_str(), [this] { reload(); });
  }
}

Config::~Config()
{
  m_config_watcher.cancel();
}

void Config::reload() noexcept
{
  std::lock_guard lock { m_condomn };

  auto config_file = find_config_file();
  if (!config_file) {
    return;
  }

  std::cout << "Loading config file: " << *config_file << "\n";

  auto root = YAML::LoadFile(config_file.value());
  if (!root) {
    return;
  }

  if (root["window"]) {
    m_window_height = root["window"]["height"].as<int>();
    m_window_width  = root["window"]["width"].as<int>();
  }

  if (root["player_one"]) {
    m_player_one_config = parse_player_config(root["player_one"]);
  }

  if (root["player_two"]) {
    m_player_two_config = parse_player_config(root["player_two"]);
  }

  if (root["fake_background"]) {
    m_fake_background = root["fake_background"].as<bool>();
  }

  if (root["show_markers"]) {
    m_show_markers = root["show_markers"].as<bool>();
  }
}

std::optional<std::string> Config::find_config_file() noexcept
{
  // TODO: Resolve this.
  return "config.yml";
}

Player_Config Config::parse_player_config(const YAML::Node& node) noexcept
{
  auto color = node["color"];
  if (!color) {
    std::cerr << "Missing key in configuration: color\n";
    std::exit(1);
  }

  auto playerColor = cv::Scalar {
    color["blue"].as<double>(),
    color["green"].as<double>(),
    color["red"].as<double>(),
  };

  auto hue = node["hue"];
  if (!hue) {
    std::cerr << "Missing key in configuration: hue\n";
    std::exit(1);
  }

  auto playerMinHsv = cv::Scalar { hue["min"].as<double>(), 50, 70 };
  auto playerMaxHsv = cv::Scalar { hue["max"].as<double>(), 255, 255 };

  return Player_Config {
    playerColor,
    playerMinHsv,
    playerMaxHsv,
  };
}

Config& Config::the()
{
  if (instance == nullptr) {
    instance = new Config;
  }

  return *instance;
}

int Config::window_height() const noexcept
{
  return m_window_height;
}

int Config::window_width() const noexcept
{
  return m_window_width;
}

std::string Config::start_screen_image_file() const noexcept
{
  return "assets/startscreen.png";
}

const Player_Config& Config::player_one() const noexcept
{
  return m_player_one_config;
}

const Player_Config& Config::player_two() const noexcept
{
  return m_player_two_config;
}

bool Config::fake_background() const noexcept
{
  return m_fake_background;
}

bool Config::show_markers() const noexcept
{
  return m_show_markers;
}
