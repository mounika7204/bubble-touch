#pragma once

#include <vector>

#include <gtkmm.h>
#include <opencv2/core.hpp>

#include "bubble.hpp"

class Game {
public:
  Game(cv::RNG& rng, int nrows, int ncols);

  virtual ~Game();

  [[nodiscard]] int remainingTimeInSeconds() const noexcept;

  [[nodiscard]] const std::vector<Bubble>& bubbles() const noexcept;

  sigc::signal<void> timeDecreasedSlot() noexcept;

private:
  [[nodiscard]] Bubble generateRandomBubble() const noexcept;

  bool decreaseRemainingTime() noexcept;

  bool mRunning                = true;
  int mRemainingTimeInSeconds  = 60;
  std::vector<Bubble> mBubbles = {};
  int mNrows;
  int mNcols;
  cv::RNG& mRng;
  sigc::signal<void> mTimeDecreasedSignal;
  sigc::connection mTimeoutConnection;
};
