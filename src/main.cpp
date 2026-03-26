#include "colours.hpp"
#include "inputs.hpp"
#include "game_window.hpp"
#include "game/game.hpp"

#include <ncurses.h>
#include <chrono>
#include <stdexcept>
#include <csignal>
#include <variant>

namespace {
using Clock = std::chrono::steady_clock;
using Time = std::chrono::time_point<Clock>;

// Prototypes //
void init_tui();
void print_basic_info();
void game_routine(GameSession& game, const GameWindow& game_win, const Input input, Time& next_tick);
void update_game_window(GameSession& game, const GameWindow& game_win);
void print_game_block(const GameWindow& game_win, const Coordinates& pos, const Colour colour);
} // namespace

// Implementation //
auto main() -> int {
  init_tui();

  print_basic_info();
  Game game;
  const std::optional<GameWindow> game_win;

  Input input = Input::None;
  Time next_tick = Clock::now();
  while (input != Input::Quit) {
    const bool has_started = game.has_started();
    if (has_started) {
      GameSession& session = game.get_session();
      game_routine(session, game_win.value(), input, next_tick);      
    } 

     input = capture_input(has_started);
     if (input == Input::Start and not has_started) {
       game.start();
       game_win = GameWindow();
       input = None;
     } 
  }

  endwin();
  return 0;
}

namespace {
void init_tui() {
  setlocale(LC_ALL, ""); // utf-8 support
  initscr();
  curs_set(0); // hide cursor
  cbreak();
  noecho(); 
  TUIColours::init();
  keypad(stdscr, true);

  std::signal(SIGINT, [](int) {
      endwin();
      std::exit(0);
  });
}

void print_basic_info() {
  mvprintw(LINES / 4, (COLS - 7) / 2, "tuitris"); 
  mvprintw(LINES * 19 / 20, COLS / 8, "Quit (q)");
  refresh();
}

void game_routine(GameSession& game, const GameWindow& game_win, const Input input, Time& next_tick) {
  const bool moved = game.try_move(input);

  bool ticked = false;
  if (Clock::now() >= next_tick) {
    game.tick();
    ticked = true;

    using namespace std::chrono_literals;
    next_tick = Clock::now() + 300ms;
  }

  if (not ticked and not moved) {
    return;
  }

  game_win.update();
}

void print_game_block(const WindowPtr& game_win, const Coordinates& pos, const Colour colour) {
  if (pos.x >= signed_game_width or pos.y >= signed_game_height or pos.x < 0 or pos.y < 0) {
    throw std::out_of_range("Tile coordinate is out of range");
  }

  const chtype ncurses_colour = TUIColours::colour_to_ncurses_pair(colour);

  wattron(game_win.get(), ncurses_colour);
  mvwprintw(game_win.get(), pos.y + 1, (pos.x + 1) * 2, "██");
  wattroff(game_win.get(), ncurses_colour);
}
} // namespace
