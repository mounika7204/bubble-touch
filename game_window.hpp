#pragma once

#include <gtkmm.h>
#include <memory>

#include "game.hpp"
#include "game_widget.hpp"
#include "player_score_widget.hpp"

class GameWindow : public Gtk::Window {
public:
  GameWindow();

  [[nodiscard]] static std::shared_ptr<GameWindow> create() noexcept;

  virtual ~GameWindow() { }

private:
  [[nodiscard]] bool decreaseRemainingTime() noexcept;

  void setRemainingTimeLabelText() noexcept;

  Gtk::Box mBox;
  Gtk::Box mInformationBox;
  PlayerScoreWidget mPlayerOneScore;
  PlayerScoreWidget mPlayerTwoScore;
  Gtk::Label mRemainingTimeLabel;
  GameWidget mGameWidget;

  std::unique_ptr<Game> mGame;
};
