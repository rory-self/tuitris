#include "ui/game_window.hpp"
#include "coordinates.hpp"

#include <ncurses.h>
#include <ranges>

namespace {
constexpr std::size_t score_win_width = 20;
constexpr std::size_t score_win_height = 3;
constexpr std::size_t piece_win_width = 12;
constexpr std::size_t piece_win_height = 5;
}

GameWindows::GameWindows(const GameSession& game)
  : _game_window(game_win_height, game_win_width, (LINES - game_win_height) / 4, (COLS - game_win_width) / 2)
  , _piece_window(piece_win_height, piece_win_width, ((LINES - game_win_height) / 2) + 8, ((COLS - game_win_width) / 2) + game_win_width)
  , _score_window(score_win_height, score_win_width, (LINES - game_win_height) / 2, ((COLS - game_win_width) / 2) + game_win_width)
  , _level_window(score_win_height, score_win_width, ((LINES - game_win_height) / 2) + 4, ((COLS - game_win_width) / 2) + game_win_width)
  , _game(game)
  , _bag(game.get_bag()) {
  draw_border();

  _score_window.init_titled("Score");
  update_score();

  _level_window.init_titled("Level");
  update_level();

  refresh();
}

void GameWindows::init_score_window() const {
  _score_window.init_titled("Score");
  update_score();
}

void GameWindows::init_piece_window() const {
  _piece_window.init_titled("Next");
  update_next_piece();
}

void GameWindows::init_level_window() const {
  _level_window.init_titled("Level");
  update_level();
}

void GameWindows::clear() const {
  _game_window.erase();
  _score_window.erase();
  _piece_window.erase();

  draw_border();
  init_score_window();
  init_piece_window();
}

void GameWindows::draw_border() const {
  constexpr std::size_t box_top_y = vanishing_area_height + 1;
  constexpr std::size_t box_left_x = 1;
  constexpr std::size_t box_right_x = game_win_width - 2;
  constexpr std::size_t box_bottom_y = game_win_height - 1;

  _game_window.draw_border(box_bottom_y, box_top_y, box_left_x, box_right_x);
}

void GameWindows::update_score() const {
  const std::string score_str = std::to_string(_game.get_score());
  _score_window.move_print({ .x = 1, .y = 1 }, score_str);
}

void GameWindows::update_level() const {
  const std::string level_str = std::to_string(_game.get_level());
  _level_window.move_print({ .x = 1, .y = 1}, level_str);
}

void GameWindows::update_next_piece() const {
  const TileOffsets offsets = _bag.get_next_tetromino_offsets();
  constexpr Coordinates center_pos = { .x = 1, .y = 1};
  
  _piece_window.print_block(center_pos, Colour::Cyan);
  for (const Coordinates& offset : offsets) {
    _piece_window.print_block(center_pos + offset, Colour::Cyan);
  }
}

void GameWindows::update() const {
  clear();
  const TileGrid& new_data = _game.get_tile_data();

  for (const auto& [y_coord, tile_row] : std::views::enumerate(new_data)) {
    for (const auto& [x_coord, tile] : std::views::enumerate(tile_row)) {
      std::visit([&](const auto& visited_tile) -> void {
        if constexpr (not std::is_same_v<std::decay_t<decltype(visited_tile)>, Empty>) {
            const Colour tile_colour = visited_tile.get_colour();
            const Coordinates tile_coords = {
              .x = static_cast<Coordinate>(x_coord),
              .y = static_cast<Coordinate>(y_coord)
            };

            _game_window.print_block(tile_coords, tile_colour);
        }
      }, tile);
    }
  }

  update_next_piece();
  update_score();
  update_level();
  refresh();
}

void GameWindows::refresh() const {
  _game_window.refresh();
  _piece_window.refresh();
  _score_window.refresh();
  _level_window.refresh();
}

