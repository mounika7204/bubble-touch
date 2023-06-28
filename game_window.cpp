#include <iostream>

#include "config.hpp"
#include "game_window.hpp"

GameWindow::GameWindow(cv::RNG& rng)
    : mBox { Gtk::ORIENTATION_VERTICAL }
    , mInformationBox { Gtk::ORIENTATION_HORIZONTAL }
    , mPlayerOneScore { "Player 1" }
    , mPlayerTwoScore { "Player 2" }
    , mGameWidget { rng }
{
  set_title("BubbleTouch Game");
  set_default_size(Config::the().windowWidth(), Config::the().windowHeight());

  mGameWidget.game().timeDecreasedSlot().connect([&]() { setRemainingTimeLabelText(); });
  setRemainingTimeLabelText();

  mInformationBox.pack_end(mPlayerTwoScore);
  mInformationBox.pack_end(mRemainingTimeLabel);
  mInformationBox.pack_end(mPlayerOneScore);

  mBox.pack_end(mGameWidget);
  mBox.pack_end(mInformationBox);

  add(mBox);

  show_all();
}

void GameWindow::setRemainingTimeLabelText() noexcept
{
  auto text = std::to_string(mGameWidget.game().remainingTimeInSeconds()) + "s";
  mRemainingTimeLabel.set_text(text);
}
