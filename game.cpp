#include "game.hpp"
#include "config.hpp"

Game::Game(cv::RNG& rng, int nrows, int ncols)
    : mRng { rng }
    , mNrows { nrows }
    , mNcols { ncols }
{
  auto timeoutSlot   = sigc::mem_fun(*this, &Game::decreaseRemainingTime);
  mTimeoutConnection = Glib::signal_timeout().connect(timeoutSlot, 1000);

  for (int i = 0; i < 7; i++) {
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

sigc::signal<void> Game::timeDecreasedSlot() noexcept
{
  return mTimeDecreasedSignal;
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
    return false;
  }

  mRemainingTimeInSeconds -= 1;
  mTimeDecreasedSignal.emit();

  return true;
}
