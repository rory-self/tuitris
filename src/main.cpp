#include <ncurses.h>

#include <utility>

namespace {
struct Coordinates {
  int x;
  int y;
};

// Prototypes
void init_tui();
auto get_window_dimensions(WINDOW *win) -> Coordinates;
} // namespace

auto main() -> int {
  init_tui();

  Coordinates win_dimensions = get_window_dimensions(stdscr);

  mvprintw(win_dimensions.y / 4, win_dimensions.x / 2, "tuitris");
  refresh();
  while (true) {
  }

  endwin();
  return 0;
}

namespace {
void init_tui() {
  initscr();
  curs_set(0); // hide cursor
  noecho();
  cbreak();
  keypad(stdscr, true);
}

auto get_window_dimensions(WINDOW *win) -> Coordinates {
  int max_y, max_x;
  getmaxyx(win, max_y, max_x);

  return { .x = max_x, .y = max_y }; 
}
} // namespace
