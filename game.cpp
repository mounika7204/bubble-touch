#include <opencv2/imgproc.hpp>

#include "config.hpp"
#include "game.hpp"

Game::Game(cv::RNG& rng, int nrows, int ncols)
    : mRng { rng }
    , mPlayerOne { "Player 1" }
    , mPlayerTwo { "Player 2" }
    , mNrows { nrows }
    , mNcols { ncols }
{
  auto timeoutSlot   = sigc::mem_fun(*this, &Game::decreaseRemainingTime);
  mTimeoutConnection = Glib::signal_timeout().connect(timeoutSlot, 1000);

  for (int i = 0; i < 8; i++) {
    mBubbles.push_back(generateRandomBubble());
  }
}

Game::~Game()
{
  mTimeoutConnection.disconnect();
}

Bubble Game::generateRandomBubble() const noexcept
{
  return Bubble::createRandom(mRng, std::make_tuple(mNcols, mNrows));
}

const Player& Game::playerOne() const noexcept
{
  return mPlayerOne;
}

const Player& Game::playerTwo() const noexcept
{
  return mPlayerTwo;
}

sigc::signal<void, int> Game::timeDecreasedSignal() const noexcept
{
  return mTimeDecreasedSignal;
}

sigc::signal<void, int> Game::playerOneScoredSignal() const noexcept
{
  return mPlayerOneScoredSignal;
}

sigc::signal<void, int> Game::playerTwoScoredSignal() const noexcept
{
  return mPlayerTwoScoredSignal;
}

sigc::signal<void> Game::gameOverSignal() const noexcept
{
  return mGameOverSignal;
}

int Game::remainingTimeInSeconds() const noexcept
{
  return mRemainingTimeInSeconds;
}

const std::vector<Bubble>& Game::bubbles() const noexcept
{
  return mBubbles;
}

bool Game::decreaseRemainingTime() noexcept
{
  if (mRemainingTimeInSeconds <= 0) {
    mRunning = false;
    mGameOverSignal.emit();
    return false;
  }

  mRemainingTimeInSeconds -= 1;
  mTimeDecreasedSignal.emit(mRemainingTimeInSeconds);

  return true;
}

void Game::checkCollisionsWithPlayerOne(cv::RotatedRect playerOneMark) noexcept
{
  checkCollisionsWithPlayer(mPlayerOne, playerOneMark, mPlayerOneScoredSignal);
}

void Game::checkCollisionsWithPlayerTwo(cv::RotatedRect playerTwoMark) noexcept
{
  checkCollisionsWithPlayer(mPlayerTwo, playerTwoMark, mPlayerTwoScoredSignal);
}

void Game::checkCollisionsWithPlayer(
    Player& player, cv::RotatedRect playerMark, sigc::signal<void, int> playerScoredSignal) noexcept
{
  if (!mRunning) {
    return;
  }

  std::vector<Bubble> newBubbles;

  for (const auto& bubble : mBubbles) {
    auto bubbleRect = bubble.asRotatedRect();
    cv::Mat intersectionPoints;
    auto result = cv::rotatedRectangleIntersection(bubbleRect, playerMark, intersectionPoints);

    if (result == cv::INTERSECT_PARTIAL || result == cv::INTERSECT_FULL) {
      player.increaseScore();
      playerScoredSignal.emit(player.score());
      burst_particles_at_position(bubbleRect.center, bubble.color());
      newBubbles.push_back(generateRandomBubble());
    } else {
      newBubbles.push_back(bubble);
    }
  }

  mBubbles = newBubbles;
}

void Game::floatBubbles() noexcept
{
  std::vector<Bubble> newBubbles;

  for (auto& bubble : mBubbles) {
    bubble.floatUp();
    if (bubble.y() + bubble.radius() >= 0) {
      newBubbles.push_back(bubble);
    } else {
      newBubbles.push_back(generateRandomBubble());
    }
  }

  mBubbles = newBubbles;
}

Player Game::winner() const noexcept
{
  if (mPlayerOne.score() > mPlayerTwo.score()) {
    return mPlayerOne;
  } else {
    return mPlayerTwo;
  }
}

void Game::burst_particles_at_position(cv::Point pos, cv::Scalar color)
{
  static int BURST_COUNT = 100;
  for (auto i = 0; i < BURST_COUNT; i++) {
    m_particles.emplace_back(spawn_random_particle_at_position(mRng, pos, color));
  }
}

void Game::update_particles(double dt) noexcept
{
  std::vector<Particle> new_particles = {};

  for (auto&& particle : m_particles) {
    if (particle.lt > 0) {
      particle.lt -= dt;
      particle.pos.x += particle.velocity.x;
      particle.pos.y += particle.velocity.y;
      particle.velocity *= 0.98;
      new_particles.emplace_back(particle);
    }
  }

  m_particles = new_particles;
}

[[nodiscard]] const std::vector<Particle>& Game::particles() const noexcept
{
  return m_particles;
}
