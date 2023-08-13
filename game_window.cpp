#include <iostream>

#include "config.hpp"
#include "game_window.hpp"

// TODO: Merge this whole class into GameWidget.

GameWindow::GameWindow(cv::RNG& rng)
    : mBox { Gtk::ORIENTATION_VERTICAL }
    , mInformationBox { Gtk::ORIENTATION_HORIZONTAL }
    , mPlayerOneScore { "Player 1" }
    , mPlayerTwoScore { "Player 2" }
    , mGameWidget { rng }
{
  set_title("BubbleTouch Game");
  set_default_size(Config::the().windowWidth(), Config::the().windowHeight());
  set_resizable(false);

  mGameWidget.game().timeDecreasedSignal().connect(
      sigc::mem_fun(*this, &GameWindow::setRemainingTimeLabelText));
  setRemainingTimeLabelText(mGameWidget.game().remainingTimeInSeconds());

  mGameWidget.game().playerOneScoredSignal().connect(
      sigc::mem_fun(mPlayerOneScore, &PlayerScoreWidget::setPlayerScore));

  mGameWidget.game().playerTwoScoredSignal().connect(
      sigc::mem_fun(mPlayerTwoScore, &PlayerScoreWidget::setPlayerScore));

  mGameWidget.game().gameOverSignal().connect(sigc::mem_fun(*this, &GameWindow::gameOver));

  mRemainingTimeLabel.set_name("remaining-time");

  mInformationBox.pack_end(mPlayerTwoScore);
  mInformationBox.pack_end(mRemainingTimeLabel);
  mInformationBox.pack_end(mPlayerOneScore);
  mInformationBox.set_name("game-window-info-box");

  mBox.pack_end(mGameWidget);
  mBox.pack_end(mInformationBox);

  add(mBox);

  show_all();
}

sigc::signal<void, Player> GameWindow::gameOverSignal() const noexcept
{
  return mGameOverSignal;
}

void GameWindow::setRemainingTimeLabelText(int remainingTimeInSeconds) noexcept
{
  auto text = "Time\n " + std::to_string(remainingTimeInSeconds) + "s";
  mRemainingTimeLabel.set_text(text);
}

void GameWindow::gameOver() noexcept
{
  mGameOverSignal.emit(mGameWidget.game().winner());
  hide();
}
