#include <iostream>

#include "config.hpp"
#include "game_window.hpp"

GameWindow::GameWindow()
    : mBox { Gtk::ORIENTATION_VERTICAL }
    , mInformationBox { Gtk::ORIENTATION_HORIZONTAL }
    , mPlayerOneScore { "Player 1" }
    , mPlayerTwoScore { "Player 2" }
    , mGame { std::make_unique<Game>() }
{
  set_title("BubbleTouch Game");
  set_default_size(Config::the().windowWidth(), Config::the().windowHeight());

  setRemainingTimeLabelText();
  mGame->timeDecreasedSlot().connect([&]() { setRemainingTimeLabelText(); });

  mInformationBox.pack_end(mPlayerTwoScore);
  mInformationBox.pack_end(mRemainingTimeLabel);
  mInformationBox.pack_end(mPlayerOneScore);

  mBox.pack_end(mGameWidget);
  mBox.pack_end(mInformationBox);

  add(mBox);

  show_all();
}

std::shared_ptr<GameWindow> GameWindow::create() noexcept
{
  return std::make_shared<GameWindow>();
}

void GameWindow::setRemainingTimeLabelText() noexcept
{
  auto text = std::to_string(mGame->remainingTimeInSeconds()) + "s";
  mRemainingTimeLabel.set_text(text);
}
