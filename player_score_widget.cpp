#include "player_score_widget.hpp"

PlayerScoreWidget::PlayerScoreWidget(const std::string& playerLabel)
    : Gtk::Box { Gtk::ORIENTATION_VERTICAL, 10 }
    , mPlayerLabel { playerLabel }
{
  setPlayerScore(0);
  pack_start(mPlayerLabel);
  pack_end(mPlayerScoreLabel);
}

void PlayerScoreWidget::setPlayerScore(int playerScore) noexcept
{
  mPlayerScore = playerScore;
  auto text    = std::string("Score: ") + std::to_string(mPlayerScore);
  mPlayerScoreLabel.set_text(text);
}
