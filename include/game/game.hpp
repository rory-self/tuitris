#ifndef GAME_HPP
#define GAME_HPP

#include "coordinates.hpp"
#include "tetromino.hpp"
#include "inputs.hpp"
#include "tile.hpp"

#include <array>
#include <optional>
#include <vector>
#include <memory>
#include <random>
#include <unordered_set>

using TileRow = std::array<Tile, game_width>;
using TileGrid = std::array<TileRow, game_height>;

class GameSession {
private:
  struct FallingTetromino {
    std::unique_ptr<Tetromino> tetromino;
    Coordinates pivot_pos;
  };

  TileGrid _tile_data;
  std::optional<FallingTetromino> _falling_tetromino;
  std::vector<std::unique_ptr<Tetromino>> _shape_bag;
  std::mt19937 _bag_rng{std::random_device{}()};
  unsigned int _score = 0;

  void place_tiles(const Colour tetromino_colour, const TilePositions& falling_tile_positions);
  void refill_bag();
  void remove_filled_rows(const std::unordered_set<Coordinate>& y_coords);
  void score_removed_rows(const unsigned int rows_removed);
  auto try_rotate_tetromino(FallingTetromino& falling_tetromino, const TilePositions& tile_positions, const bool clockwise) -> bool;

public:
  GameSession();

  [[nodiscard]] auto get_tile_data() const noexcept -> const TileGrid&;
  void update_falling_tiles(const Colour tetromino_colour,
      const TilePositions& old_tile_positions,
      const TilePositions& new_tile_positions);
  auto try_transformation(const Transformation transformation) -> bool;
  void tick();
  void drop_tetromino();
};

class Game {
private:
  std::optional<GameSession> _session = std::nullopt;

public:
  [[nodiscard]] auto get_session() -> GameSession&;
  [[nodiscard]] auto has_started() const noexcept -> bool;
  auto start() -> const GameSession&;
};

#endif // GAME_HPP
