#pragma once

#include <vector>

#include <gtkmm.h>
#include <opencv2/core.hpp>

#include "bubble.hpp"
#include "player.hpp"

class Game {
public:
  Game(cv::RNG& rng, int nrows, int ncols);

  virtual ~Game();

  [[nodiscard]] int remainingTimeInSeconds() const noexcept;

  [[nodiscard]] const std::vector<Bubble>& bubbles() const noexcept;

  [[nodiscard]] const Player& playerOne() const noexcept;

  [[nodiscard]] const Player& playerTwo() const noexcept;

  void checkCollisionsWithPlayerOne(cv::RotatedRect playerOneMark) noexcept;

  void checkCollisionsWithPlayerTwo(cv::RotatedRect playerTwoMark) noexcept;

  sigc::signal<void, int> timeDecreasedSignal() const noexcept;

  sigc::signal<void, int> playerOneScoredSignal() const noexcept;

  sigc::signal<void, int> playerTwoScoredSignal() const noexcept;

private:
  [[nodiscard]] Bubble generateRandomBubble() const noexcept;

  bool decreaseRemainingTime() noexcept;

  void checkCollisionsWithPlayer(Player&, cv::RotatedRect, sigc::signal<void, int>) noexcept;

  bool mRunning                = true;
  int mRemainingTimeInSeconds  = 60;
  std::vector<Bubble> mBubbles = {};

  Player mPlayerOne;
  Player mPlayerTwo;

  int mNrows;
  int mNcols;
  cv::RNG& mRng;

  sigc::signal<void, int> mTimeDecreasedSignal;
  sigc::connection mTimeoutConnection;

  sigc::signal<void, int> mPlayerOneScoredSignal;
  sigc::signal<void, int> mPlayerTwoScoredSignal;
};
