#include <ncurses.h>
#include "game.hpp"

#include <cctype>

namespace {
struct Coordinates {
  const int x;
  const int y;
};

enum Inputs {
  Quit = 'q',
  Start = 's'
};

void init_tui();
void print_basic_info(const Coordinates& win_dimensions);
[[nodiscard]] auto get_window_dimensions(const WINDOW *const win) -> Coordinates;
} // namespace

auto main() -> int {
  init_tui();

  const Coordinates win_dimensions = get_window_dimensions(stdscr);
  print_basic_info(win_dimensions);
  Game game;

  char input;
  while (true) {
    const char raw_input = getch();
    input = std::tolower(raw_input);

    switch (input) {
      case Inputs::Quit:
        goto Quit;
      case Inputs::Start:
        game.start();
        break;
    }
  }

Quit:
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

void print_basic_info(const Coordinates& win_dimensions) {
  mvprintw(win_dimensions.y / 4, win_dimensions.x / 2, "tuitris");
  mvprintw(win_dimensions.y * 19 / 20, win_dimensions.x / 8, "Quit (q)");
  refresh();
}

auto get_window_dimensions(const WINDOW *const win) -> Coordinates {
  int max_y, max_x;
  getmaxyx(win, max_y, max_x);

  return { .x = max_x, .y = max_y }; 
}
} // namespace
