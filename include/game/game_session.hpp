#ifndef GAME_SESSION_HPP
#define GAME_SESSION_HPP

#include "coordinates.hpp"
#include "transformation.hpp"
#include "tetromino.hpp"
#include "tile_grid.hpp"
#include "tetromino_bag.hpp"

#include <optional>

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
  void score_removed_rows(std::size_t rows_removed);
  auto try_rotate_tetromino(FallingTetromino& falling_tetromino,
      const TilePositions& tile_positions, bool clockwise) -> bool;
  void drop_tetromino();
  void update_falling_tiles(Colour tetromino_colour, 
      const TilePositions& old_tile_positions,
      const TilePositions& new_tile_positions);

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
