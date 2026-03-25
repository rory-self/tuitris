#ifndef GAME_HPP
#define GAME_HPP

#include "coordinates.hpp"
#include "tetromino.hpp"

#include <array>
#include <optional>
#include <vector>

enum class TileState { Empty, Taken, Falling };
enum class TileColour { Cyan, Blue, Orange, Yellow, Green, Purple, Red };
struct Tile {
  TileState state;

  [[nodiscard]] auto operator==(const TileState& other_state) -> bool {
    return state == other_state;
  }
};

using TileData = std::array<std::array<Tile, game_width>, game_height>;

enum Input {
  Quit = 'q',
  Start = 's',
  Left = 'a',
  Right = 'd',
  RotateClockwise = 'e',
  None,
};

class GameSession {
private:
  struct FallingTetromino {
    Tetromino tetromino;
    Coordinates pivot_pos;
  };

  TileData _tile_data;
  std::optional<FallingTetromino> _falling_tetromino;
  std::vector<Tetromino> _shape_bag;

  void place_tiles(const std::array<Coordinates, 4>& falling_tile_positions);
  void refill_bag();

public:
  GameSession();

  [[nodiscard]] auto get_tile_data() const noexcept -> const TileData&;
  void update_falling_tiles(const FallingTetromino& _falling_tetromino, const std::array<Coordinates, 4>& old_tile_positions, const std::array<Coordinates, 4>& new_tile_positions);
  auto try_move(const Input input) -> bool;
  void tick();
  void drop_tetromino();
};

class Game {
private:
  std::optional<GameSession> _session = std::nullopt;

public:
  [[nodiscard]] auto get_session() -> GameSession&;
  [[nodiscard]] auto has_started() const noexcept -> bool;
  void start();
};

#endif // GAME_HPP
