#include <ncurses.h>

#include <cctype>

namespace {
struct Coordinates {
  int x;
  int y;
};

enum Inputs {
  Quit = 'q'
};

void init_tui();
[[nodiscard]] auto get_window_dimensions(const WINDOW *const win) -> Coordinates;
} // namespace

auto main() -> int {
  init_tui();

  const Coordinates win_dimensions = get_window_dimensions(stdscr);

  mvprintw(win_dimensions.y / 4, win_dimensions.x / 2, "tuitris");
  refresh();

  char input;
  do {
    const char raw_input = getch();
    input = std::tolower(raw_input);
  } while (input != Quit);

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

auto get_window_dimensions(const WINDOW *const win) -> Coordinates {
  int max_y, max_x;
  getmaxyx(win, max_y, max_x);

  return { .x = max_x, .y = max_y }; 
}
} // namespace
