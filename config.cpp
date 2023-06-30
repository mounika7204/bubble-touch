#include <cstdlib>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "config.hpp"

PlayerConfig::PlayerConfig(cv::Scalar color, cv::Scalar minHsv, cv::Scalar maxHsv)
    : mColor { color }
    , mMinHsv { minHsv }
    , mMaxHsv { maxHsv }
{
}

cv::Scalar PlayerConfig::color() const noexcept
{
  return mColor;
}

cv::Scalar PlayerConfig::minHsv() const noexcept
{
  return mMinHsv;
}

cv::Scalar PlayerConfig::maxHsv() const noexcept
{
  return mMaxHsv;
}

Config* Config::instance = nullptr;

Config::Config()
    : mPlayerOneConfig { cv::Scalar(255, 0, 0), cv::Scalar(90, 50, 70), cv::Scalar(128, 255, 255) }
    , mPlayerTwoConfig { cv::Scalar(0, 255, 0), cv::Scalar(36, 50, 70), cv::Scalar(89, 255, 255) }
{
  auto configFile = findConfigFile();
  if (!configFile) {
    return;
  }

  auto root = YAML::LoadFile(configFile.value());
  if (!root) {
    return;
  }

  if (root["playerOne"]) {
    mPlayerOneConfig = parsePlayerConfig(root["playerOne"]);
  }

  if (root["playerTwo"]) {
    mPlayerTwoConfig = parsePlayerConfig(root["playerTwo"]);
  }
}

std::optional<std::string> Config::findConfigFile() noexcept
{
  // TODO: Resolve this.
  return "config.yml";
}

PlayerConfig Config::parsePlayerConfig(const YAML::Node& node) noexcept
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

  return PlayerConfig {
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

const PlayerConfig& Config::playerOne() const noexcept
{
  return mPlayerOneConfig;
}

const PlayerConfig& Config::playerTwo() const noexcept
{
  return mPlayerTwoConfig;
}
