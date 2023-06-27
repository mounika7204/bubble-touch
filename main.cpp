#include <gtkmm.h>

#include "bubbletouch_application.hpp"

auto main(int argc, char** argv) -> int
{
  auto app = BubbleTouchApplication::create();
  return app->run(argc, argv);
}
