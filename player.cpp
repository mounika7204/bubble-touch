#include <cassert>

#include "player.hpp"

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
