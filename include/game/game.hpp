#ifndef GAME_HPP
#define GAME_HPP

#include "coordinates.hpp"
#include "tetromino.hpp"
#include "tetromino_bag.hpp"
#include "inputs.hpp"
#include "tile.hpp"

#include <array>
#include <optional>
#include <unordered_set>

using TileRow = std::array<Tile, game_width>;
using TileGrid = std::array<TileRow, game_height>;

class GameSession {
private:
  struct FallingTetromino {
    Tetromino tetromino;
    Coordinates pivot_pos;
  };

  bool _game_over = false;
  TileGrid _tile_data;
  TetrominoBag _bag;
  std::optional<FallingTetromino> _falling_tetromino;

  unsigned int _score = 0;

  void place_tiles(Colour tetromino_colour, const TilePositions& falling_tile_positions);
  [[nodiscard]] auto is_overflowing() const -> bool;
  [[nodiscard]] auto is_taken_or_out_of_bounds(const Coordinates& pos) const -> bool;
  void remove_filled_rows(const std::unordered_set<Coordinate>& y_coords);
  void score_removed_rows(unsigned int rows_removed);
  auto try_rotate_tetromino(FallingTetromino& falling_tetromino,
      const TilePositions& tile_positions, bool clockwise) -> bool;
  void drop_tetromino();

public:
  GameSession();

  [[nodiscard]] auto get_score() const noexcept -> unsigned int;
  [[nodiscard]] auto get_tile_data() const noexcept -> const TileGrid&;
  [[nodiscard]] auto get_tile(const Coordinates& pos) const -> const Tile&;
  [[nodiscard]] auto get_tile(const Coordinates& pos) -> Tile&;
  [[nodiscard]] auto get_bag() const noexcept -> const TetrominoBag&;

  void update_falling_tiles(Colour tetromino_colour,
      const TilePositions& old_tile_positions,
      const TilePositions& new_tile_positions);
  auto try_transformation(Transformation transformation) -> bool;
  void tick();
};

class Game {
private:
  std::optional<GameSession> _session = std::nullopt;

public:
  [[nodiscard]] auto get_session() -> std::optional<std::reference_wrapper<GameSession>>;
  [[nodiscard]] auto has_started() const noexcept -> bool;
  auto start() -> const GameSession&;
};

#endif // GAME_HPP
