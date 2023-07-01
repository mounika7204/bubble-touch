#pragma once

#include <gtkmm.h>

#include "player.hpp"

class GameOverWindow : public Gtk::Window {
public:
  GameOverWindow(Player winner);

private:
  [[nodiscard]] std::string getWinnerLabelText() const noexcept;

  Player mWinner;
  Gtk::Box mBox;
  Gtk::Label mGameOverLabel;
  Gtk::Label mWinnerLabel;
};
