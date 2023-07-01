#include <cassert>

#include "player.hpp"

Player::Player(const std::string& name)
    : mName { name }
{
}

void Player::increaseScore() noexcept
{
  mScore += 1;
}

void Player::decreaseScore() noexcept
{
  assert(mScore > 0 && "cannot decrease score below 0!");
  mScore -= 1;
}

int Player::score() const noexcept
{
  return mScore;
}

const std::string& Player::name() const noexcept
{
  return mName;
}
