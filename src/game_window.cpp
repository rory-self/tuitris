#include "game_window.hpp"

#include <ranges>

GameWindow::GameWindow(const GameSession& game) : _window(make_game_window()), _game(game) {}

auto GameWindow::make_game_window() -> WindowPtr {
  WINDOW *const game_win = newwin(game_win_height, game_win_width, (LINES - game_win_height) / 2, (COLS - game_win_width) / 2);
  box(game_win, 0, 0);
  wrefresh(game_win);

  return {game_win, delwin};
}

void GameWindow::clear() const {
  werase(_window.get());
  box(_window.get(), 0, 0);
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

  wrefresh(_window.get());
}

void GameWindow::print_block(const Coordinates& pos, const Colour colour) const {
  const chtype ncurses_colour = TUIColours::colour_to_ncurses_pair(colour);

  wattron(_window.get(), ncurses_colour);
  mvwprintw(_window.get(), pos.y + 1, (pos.x + 1) * 2, "██");
  wattroff(_window.get(), ncurses_colour);
}

