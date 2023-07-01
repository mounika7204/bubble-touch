#include "game_over_window.hpp"

GameOverWindow::GameOverWindow(Player winner)
    : mWinner { winner }
    , mBox { Gtk::ORIENTATION_VERTICAL, 10 }
    , mGameOverLabel { "GAME OVER!" }
{
  set_resizable(false);
  set_default_size(300, 100);
  set_title("Game Over!");
  set_modal(true);
  set_name("game-over-window");

  mGameOverLabel.set_name("game-over-label");

  mWinnerLabel.set_text(getWinnerLabelText());

  mBox.set_name("game-over-window-box");
  mBox.pack_start(mGameOverLabel);
  mBox.pack_end(mWinnerLabel);

  add(mBox);

  show_all();
}

std::string GameOverWindow::getWinnerLabelText() const noexcept
{
  std::string text = "The winner is '" + mWinner.name();
  text += "' with " + std::to_string(mWinner.score());

  if (mWinner.score() > 1) {
    text += " points";
  } else {
    text += " point";
  }

  return text;
}
