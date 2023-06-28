#pragma once

#include <gtkmm.h>

class PlayerScoreWidget : public Gtk::Box {
public:
  PlayerScoreWidget(const std::string& playerLabel);

  virtual ~PlayerScoreWidget() { }

  void setPlayerScore(int playerScore) noexcept;

private:
  Gtk::Label mPlayerLabel;
  Gtk::Label mPlayerScoreLabel;

  int mPlayerScore;
};
