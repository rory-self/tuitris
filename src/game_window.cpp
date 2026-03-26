#include "game_window.hpp"
#include "coordinates.hpp"

#include <ncurses.h>
#include <ranges>

GameWindow::GameWindow(const GameSession& game)
  : _game_window(make_game_window())
  , _score_window(make_score_window())
  , _game(game) {
  draw_border();
  refresh();
}

auto GameWindow::make_game_window() -> WindowPtr {
  WINDOW *const game_win = newwin(game_win_height, game_win_width, (LINES - game_win_height) * 1 / 4, (COLS - game_win_width) / 2);
  return {game_win, delwin};
}

auto GameWindow::make_score_window() -> WindowPtr {
  constexpr std::size_t score_win_width = 20;
  constexpr std::size_t score_win_height = 3;
  WINDOW *const score_win = newwin(score_win_height, score_win_width, (LINES - game_win_height) / 2, (COLS - game_win_width) / 2 + game_win_width);
  return {score_win, delwin};
}

void GameWindow::init_score_window() const {
  box(_score_window.get(), 0, 0);
  mvwprintw(_score_window.get(), 0, 1, "Score:");
}

void GameWindow::clear() const {
  werase(_game_window.get());
  werase(_score_window.get());
  draw_border();
  init_score_window();
}

void GameWindow::draw_border() const {
  constexpr std::size_t box_top_y = vanishing_area_height + 1;
  constexpr std::size_t box_left_x = 1;
  constexpr std::size_t box_right_x = game_win_width - 2;
  constexpr std::size_t box_bottom_y = game_win_height - 1;
  mvwvline(_game_window.get(), box_top_y, box_left_x, ACS_VLINE, play_area_height);
  mvwvline(_game_window.get(), box_top_y, box_right_x, ACS_VLINE, play_area_height);
  mvwhline(_game_window.get(), box_bottom_y, 2, ACS_HLINE, box_right_x - 2);
  mvwhline(_game_window.get(), box_top_y - 1, box_left_x + 1, '-', box_right_x - 2);

  mvwaddch(_game_window.get(), box_top_y - 1, box_left_x, ACS_ULCORNER);
  mvwaddch(_game_window.get(), box_top_y - 1, box_right_x, ACS_URCORNER);
  mvwaddch(_game_window.get(), box_bottom_y, box_left_x, ACS_LLCORNER);
  mvwaddch(_game_window.get(), box_bottom_y, box_right_x, ACS_LRCORNER);
}

void GameWindow::update() const {
  clear();
  const TileGrid& new_data = _game.get_tile_data();

  for (const auto& [y, tile_row] : std::views::enumerate(new_data)) {
    for (const auto& [x, tile] : std::views::enumerate(tile_row)) {
      std::visit([&](const auto& t) {
        if constexpr (not std::is_same_v<std::decay_t<decltype(t)>, Empty>) {
            const Colour tile_colour = t.get_colour();
            const Coordinates tile_coords = { .x = static_cast<int>(x), .y = static_cast<int>(y) };

            print_block(tile_coords, tile_colour);
        }
      }, tile);
    }
  }

  mvwprintw(_score_window.get(), 1, 1, "%d", _game.get_score()); 
  refresh();
}

void GameWindow::refresh() const {
  wrefresh(_game_window.get());
  wrefresh(_score_window.get());
}

void GameWindow::print_block(const Coordinates& pos, const Colour colour) const {
  const chtype ncurses_colour = TUIColours::colour_to_ncurses_pair(colour);

  wattron(_game_window.get(), ncurses_colour);
  mvwprintw(_game_window.get(), pos.y + 1, (pos.x + 1) * 2, "██");
  wattroff(_game_window.get(), ncurses_colour);
}

