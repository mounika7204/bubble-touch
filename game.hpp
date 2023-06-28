#pragma once

#include <gtkmm.h>

class Game {
public:
  Game();

  virtual ~Game();

  [[nodiscard]] int remainingTimeInSeconds() const noexcept;

  sigc::signal<void> timeDecreasedSlot() noexcept;

private:
  bool decreaseRemainingTime() noexcept;

  bool mRunning               = true;
  int mRemainingTimeInSeconds = 60;

  sigc::signal<void> mTimeDecreasedSignal;

  sigc::connection mTimeoutConnection;
};
