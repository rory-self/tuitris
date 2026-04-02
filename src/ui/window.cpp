#include "window.hpp"
#include "tui_colours.hpp"

#include <csignal>

auto Window::init_main_window() -> Window& {
  if (_main_window.has_value()) {
    return _main_window.value();
  }

  setlocale(LC_ALL, ""); // utf-8 support
  initscr();
  curs_set(0); // hide cursor
  cbreak();
  noecho(); 
  TUIColours::init();
  keypad(stdscr, true);

  std::signal(SIGINT, [](int) -> void {
      endwin();
      std::exit(0);
  });
}
