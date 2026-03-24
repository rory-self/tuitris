#include <ncurses.h>
#include "game.hpp"

#include <memory>
#include <chrono>
#include <stdexcept>

namespace {
using namespace std::chrono_literals;
constexpr std::chrono::milliseconds tick_delay_ms = 300ms;

constexpr int signed_game_height = static_cast<int>(game_height);
constexpr int signed_game_width = static_cast<int>(game_width);
constexpr int game_win_height = signed_game_height + 2;
constexpr int game_win_width = (signed_game_width + 1) * 4;

enum Inputs {
  Quit = 'q',
  Start = 's'
};

using WindowPtr = std::shared_ptr<WINDOW>;
using Clock = std::chrono::steady_clock;
using Time = std::chrono::time_point<Clock>;

void init_tui();
void print_basic_info();
[[nodiscard]] auto get_window_dimensions(const WINDOW *const win) -> Coordinates;
[[nodiscard]] auto start_game_window() -> WindowPtr;
void print_game_block(WindowPtr game_win, const Coordinates& pos);
void game_routine(Game& game, WindowPtr game_win);
auto handle_global_input(Game& game, WindowPtr game_win) -> bool;
} // namespace

auto main() -> int {
  init_tui();

  print_basic_info();
  const WindowPtr game_win = start_game_window(); 

  Game game;

  bool continue_game = true;
  Time next_tick = Clock::now();
  while (continue_game) {

    if (game.has_started() and Clock::now() >= next_tick) {
      game_routine(game, game_win);

      next_tick = Clock::now() + tick_delay_ms;
      continue;
    }

    continue_game = handle_global_input(game, game_win);
  }

  endwin();
  return 0;
}

namespace {
void init_tui() {
  setlocale(LC_ALL, ""); // utf-8 support
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
  WINDOW *const game_win = newwin(game_win_height, game_win_width, (LINES - game_win_height) / 2, (COLS - game_win_width) / 2);
  box(game_win, 0, 0);

  mvwprintw(game_win, game_win_height / 2, game_win_width / 2, "'s' to start");
  wrefresh(game_win);

  return {game_win, delwin};
}

void clear_game_window(WindowPtr game_win) {
  werase(game_win.get());
  box(game_win.get(), 0, 0);

  wrefresh(game_win.get());
}

auto handle_global_input(Game& game, WindowPtr game_win) -> bool {
  nodelay(stdscr, game.has_started());
  const char raw_input = getch();
  if (raw_input == ERR) {
    return true;
  }

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

void game_routine(Game& game, WindowPtr game_win) {
  const TileData new_data = game.tick();
  
  clear_game_window(game_win);

  int y = 0;
  for (const auto& tile_row : new_data) {
    int x = 0;
    for (const TileState state : tile_row) {
      if (state != TileState::Empty) {
        print_game_block(game_win, { .x = x, .y = y });
      }

      x++;
    }

    y++;
  }

  wrefresh(game_win.get());
}

void print_game_block(WindowPtr game_win, const Coordinates& pos) {
  if (pos.x >= signed_game_width or pos.y >= signed_game_height or pos.x < 0 or pos.y < 0) {
    throw std::out_of_range("Tile coordinate is out of range");
  }

  mvwprintw(game_win.get(), pos.y + 1, (pos.x + 1) * 2, "██");
}
} // namespace
