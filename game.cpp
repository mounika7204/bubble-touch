#include "game.hpp"

Game::Game()
{
  auto timeoutSlot   = sigc::mem_fun(*this, &Game::decreaseRemainingTime);
  mTimeoutConnection = Glib::signal_timeout().connect(timeoutSlot, 1000);
}

Game::~Game()
{
  mTimeoutConnection.disconnect();
}

sigc::signal<void> Game::timeDecreasedSlot() noexcept
{
  return mTimeDecreasedSignal;
}

int Game::remainingTimeInSeconds() const noexcept
{
  return mRemainingTimeInSeconds;
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
