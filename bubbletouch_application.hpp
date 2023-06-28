#pragma once

#include <memory>

#include <gtkmm.h>
#include <opencv2/core.hpp>

#include "game_window.hpp"
#include "initial_window.hpp"

class BubbleTouchApplication : public Gtk::Application {
public:
  BubbleTouchApplication();

  static std::shared_ptr<BubbleTouchApplication> create();

protected:
  void on_activate() override;

private:
  void startGame() noexcept;

  void onHideGameWindow() noexcept;

  cv::RNG mRng;
  InitialWindow mInitialWindow;
  GameWindow* mGameWindow = nullptr;
};
