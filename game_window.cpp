#include <iostream>

#include "config.hpp"
#include "game_window.hpp"

GameWindow::GameWindow()
    : mBox { Gtk::ORIENTATION_VERTICAL }
    , mInformationBox { Gtk::ORIENTATION_HORIZONTAL }
    , mPlayer1PtsLabel { "Player 1: 0" }
    , mPlayer2PtsLabel { "Player 2: 0" }
    , mGame { std::make_unique<Game>() }
{
  set_title("BubbleTouch Game");
  set_default_size(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);

  setRemainingTimeLabelText();
  mGame->timeDecreasedSlot().connect([&]() { setRemainingTimeLabelText(); });

  mInformationBox.pack_end(mPlayer2PtsLabel);
  mInformationBox.pack_end(mRemainingTimeLabel);
  mInformationBox.pack_end(mPlayer1PtsLabel);

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
