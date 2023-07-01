#pragma once

#include <string>

class Player {
public:
  Player(const std::string& name);

  void increaseScore() noexcept;

  void decreaseScore() noexcept;

  [[nodiscard]] int score() const noexcept;

  [[nodiscard]] const std::string& name() const noexcept;

private:
  int mScore = 0;
  std::string mName;
};
