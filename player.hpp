#pragma once

class Player {
public:
  Player() = default;

  void increaseScore() noexcept;

  void decreaseScore() noexcept;

  [[nodiscard]] int score() const noexcept;

private:
  int mScore = 0;
};
