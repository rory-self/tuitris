#include <ncurses.h>
#include "game.hpp"

#include <cctype>
#include <memory>

namespace {
struct Coordinates {
  const int x;
  const int y;
};

enum Inputs {
  Quit = 'q',
  Start = 's'
};

using WindowPtr = std::shared_ptr<WINDOW>;

void init_tui();
void print_basic_info();
[[nodiscard]] auto get_window_dimensions(const WINDOW *const win) -> Coordinates;
[[nodiscard]] auto start_game_window() -> WindowPtr;
void game_routine();
auto handle_global_input(Game& game, WindowPtr game_win) -> bool;
} // namespace

auto main() -> int {
  init_tui();

  print_basic_info();
  const WindowPtr game_win = start_game_window(); 

  Game game;

  bool continue_game = true;
  while (continue_game) {
    if (game.has_started()) {
      game_routine();
      wrefresh(game_win.get());
    }

    continue_game = handle_global_input(game, game_win);
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

void print_basic_info() {
  mvprintw(LINES / 4, (COLS - 7) / 2, "tuitris"); 
  mvprintw(LINES * 19 / 20, COLS / 8, "Quit (q)");
  refresh();
}

auto get_window_dimensions(const WINDOW *const win) -> Coordinates {
  int max_y, max_x;
  getmaxyx(win, max_y, max_x);

  return { .x = max_x, .y = max_y }; 
}

auto start_game_window() -> WindowPtr {
  const int height = LINES / 2;
  const int width = COLS / 4;

  WINDOW *const game_win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
  box(game_win, 0, 0);

  mvwprintw(game_win, height / 2, width / 2, "'s' to start");
  wrefresh(game_win);

  return {game_win, delwin};
}

void clear_game_window(WindowPtr game_win) {
  werase(game_win.get());
  box(game_win.get(), 0, 0);
}

auto handle_global_input(Game& game, WindowPtr game_win) -> bool {
  const char raw_input = getch();
  const char input = std::tolower(raw_input);

  bool continue_game = true;
  switch(input) {
    case Inputs::Quit:
      continue_game = false;
      break;
    case Inputs::Start:
      if (not game.has_started()) {
        game.start();
        clear_game_window(game_win);
      }

      break;
  }

  return continue_game;
}

void game_routine() {
}
} // namespace
