#pragma once

#include <gtkmm.h>
#include <memory>

#include "game.hpp"

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
  Gtk::Label mPlayer1PtsLabel;
  Gtk::Label mPlayer2PtsLabel;
  Gtk::Label mRemainingTimeLabel;

  std::unique_ptr<Game> mGame;
};
