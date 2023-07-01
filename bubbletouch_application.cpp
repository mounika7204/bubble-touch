#include <cstdlib>
#include <iostream>

#include "bubbletouch_application.hpp"

BubbleTouchApplication::BubbleTouchApplication()
    : Gtk::Application("io.github.aloussase.bubbletouch")
    , mRng(0xFFFFFFFF) // TODO: I might want this to be injected.
{
  try {
    auto cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_path("./styles.css");
    Gtk::StyleContext::add_provider_for_screen(
        Gdk::Screen::get_default(), cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
  } catch (const Gtk::CssProviderError&) {
    std::cerr << "error: failed to create css provider"
              << ", make sure there is a 'styles.css' in the project root\n";
    std::exit(1);
  }
}

std::shared_ptr<BubbleTouchApplication> BubbleTouchApplication::create()
{
  return std::make_shared<BubbleTouchApplication>();
}

void BubbleTouchApplication::on_activate()
{
  mInitialWindow.startSignal().connect([&]() { startGame(); });
  mInitialWindow.exitSignal().connect([&]() { quit(); });

  add_window(mInitialWindow);
  mInitialWindow.present();
}

void BubbleTouchApplication::startGame() noexcept
{
  if (mGameWindow) {
    return;
  }

  mGameWindow = new GameWindow(mRng);
  mGameWindow->signal_hide().connect([&]() { onHideGameWindow(); });
  add_window(*mGameWindow);
  mGameWindow->present();
}

void BubbleTouchApplication::onHideGameWindow() noexcept
{
  if (mGameWindow) {
    delete mGameWindow;
    mGameWindow = nullptr;
  }
}
