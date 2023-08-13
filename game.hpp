#pragma once

#include <vector>

#include <gtkmm.h>
#include <opencv2/core.hpp>

#include "bubble.hpp"
#include "particle.hpp"
#include "player.hpp"

class Game {
public:
  Game(cv::RNG& rng, int nrows, int ncols);

  virtual ~Game();

  [[nodiscard]] int remainingTimeInSeconds() const noexcept;

  [[nodiscard]] const std::vector<Bubble>& bubbles() const noexcept;

  [[nodiscard]] const Player& playerOne() const noexcept;

  [[nodiscard]] const Player& playerTwo() const noexcept;

  [[nodiscard]] const std::vector<Particle>& particles() const noexcept;

  void checkCollisionsWithPlayerOne(cv::RotatedRect playerOneMark) noexcept;

  void checkCollisionsWithPlayerTwo(cv::RotatedRect playerTwoMark) noexcept;

  void floatBubbles() noexcept;

  /// Update the game's particles.
  void update_particles(double dt) noexcept;

  [[nodiscard]] Player winner() const noexcept;

  [[nodiscard]] sigc::signal<void, int> timeDecreasedSignal() const noexcept;

  [[nodiscard]] sigc::signal<void, int> playerOneScoredSignal() const noexcept;

  [[nodiscard]] sigc::signal<void, int> playerTwoScoredSignal() const noexcept;

  [[nodiscard]] sigc::signal<void> gameOverSignal() const noexcept;

private:
  [[nodiscard]] Bubble generateRandomBubble() const noexcept;

  bool decreaseRemainingTime() noexcept;

  void checkCollisionsWithPlayer(Player&, cv::RotatedRect, sigc::signal<void, int>) noexcept;

  /// Burst particles at the specified position.
  void burst_particles_at_position(cv::Point, cv::Scalar color);

  bool mRunning                     = true;
  int mRemainingTimeInSeconds       = 30;
  std::vector<Bubble> mBubbles      = {};
  std::vector<Particle> m_particles = {};

  Player mPlayerOne;
  Player mPlayerTwo;

  int mNrows;
  int mNcols;
  cv::RNG& mRng;

  sigc::signal<void, int> mTimeDecreasedSignal;
  sigc::connection mTimeoutConnection;

  sigc::signal<void, int> mPlayerOneScoredSignal;
  sigc::signal<void, int> mPlayerTwoScoredSignal;

  sigc::signal<void> mGameOverSignal;
};
