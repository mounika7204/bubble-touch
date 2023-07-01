#include <opencv2/imgproc.hpp>

#include "config.hpp"
#include "game.hpp"

Game::Game(cv::RNG& rng, int nrows, int ncols)
    : mRng { rng }
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
    Player& player, cv::RotatedRect playerMark, sigc::signal<void, int> playerScoredSignal
) noexcept
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
