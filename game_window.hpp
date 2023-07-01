#pragma once

#include <memory>

#include <gtkmm.h>
#include <opencv2/core.hpp>

#include "game.hpp"
#include "game_widget.hpp"
#include "player.hpp"
#include "player_score_widget.hpp"

class GameWindow : public Gtk::Window {
public:
  GameWindow(cv::RNG& rng);

  virtual ~GameWindow() { }

  sigc::signal<void, Player> gameOverSignal() const noexcept;

private:
  void setRemainingTimeLabelText(int) noexcept;

  void gameOver() noexcept;

  Gtk::Box mBox;
  Gtk::Box mInformationBox;
  PlayerScoreWidget mPlayerOneScore;
  PlayerScoreWidget mPlayerTwoScore;
  Gtk::Label mRemainingTimeLabel;
  GameWidget mGameWidget;

  sigc::signal<void, Player> mGameOverSignal;
};
