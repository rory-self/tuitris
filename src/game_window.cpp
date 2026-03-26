#include "game_window.hpp"

GameWindow::GameWindow(const GameSession& game) : _window(make_game_window()), _game(game) {}

auto GameWindow::make_game_window() -> WindowPtr {
  WINDOW *const game_win = newwin(game_win_height, game_win_width, (LINES - game_win_height) / 2, (COLS - game_win_width) / 2);
  box(game_win, 0, 0);

  mvwprintw(game_win, game_win_height / 2, game_win_width / 2, "Start (s)");
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
  int y = 0;
  for (const auto& tile_row : new_data) {
    int x = 0;
    for (const Tile& tile : tile_row) {
      std::visit([&](const auto& t) {
        if constexpr (!std::is_same_v<std::decay_t<decltype(t)>, Empty>) {
            const Colour tile_colour = t.get_colour();
            print_block({ .x = x, .y = y }, tile_colour);
        }
      }, tile);

      x++;
    }

    y++;
  }

  wrefresh(_window.get());
}

void GameWindow::print_block(const Coordinates& pos, const Colour colour) const {
  if (pos.x >= signed_game_width or pos.y >= signed_game_height or pos.x < 0 or pos.y < 0) {
    throw std::out_of_range("Tile coordinate is out of range");
  }

  const chtype ncurses_colour = TUIColours::colour_to_ncurses_pair(colour);

  wattron(_window.get(), ncurses_colour);
  mvwprintw(_window.get(), pos.y + 1, (pos.x + 1) * 2, "██");
  wattroff(_window.get(), ncurses_colour);
}

