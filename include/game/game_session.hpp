#ifndef GAME_SESSION_HPP
#define GAME_SESSION_HPP

#include "coordinates.hpp"
#include "transformation.hpp"
#include "tetromino.hpp"
#include "tile_grid.hpp"
#include "tetromino_bag.hpp"

#include <optional>

namespace {
constexpr std::size_t drop_height = vanishing_area_height - 6;
constexpr std::size_t drop_x = game_width / 2;
} // namespace

constexpr Coordinates drop_pos = { .x = drop_x, .y = drop_height };

class GameSession {
private:
  struct FallingTetromino {
    Tetromino tetromino;
    Coordinates pivot_pos = drop_pos;
  };    

  bool _game_over = false;
  TileGrid _tile_data;
  TetrominoBag _bag;
  std::optional<FallingTetromino> _falling_tetromino;

  unsigned int _score = 0;

  [[nodiscard]] auto can_not_place(const TilePositions& tile_positions) -> bool;

  void place_tiles(const TilePositions& falling_tile_positions);
  void score_removed_rows(std::size_t rows_removed);
  auto try_rotate_tetromino(bool clockwise) -> bool;
  auto try_move_tetromino(bool move_right) -> bool;
  void drop_tetromino();

public:
  explicit GameSession(std::optional<int> seed = std::nullopt);

  [[nodiscard]] auto game_over() const noexcept -> bool;
  [[nodiscard]] auto get_score() const noexcept -> unsigned int;
  [[nodiscard]] auto get_tile_data() const noexcept -> const TileGrid&;
  [[nodiscard]] auto get_bag() const noexcept -> const TetrominoBag&;

  auto try_transformation(Transformation transformation) -> bool;
  void tick();
};

#endif // GAME_HPP
