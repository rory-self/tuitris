#ifndef GAME_HPP
#define GAME_HPP

#include "coordinates.hpp"
#include "tetromino.hpp"

#include <array>
#include <optional>
#include <vector>
#include <memory>
#include <random>
#include <variant>

class OccupiedTile {
private:
  Colour _colour;

protected:

  OccupiedTile(const Colour colour);

public:
  [[nodiscard]] auto get_colour() const noexcept -> Colour;
};

class Taken : public OccupiedTile {
public:
  Taken(const Colour colour);
};

class Falling : public OccupiedTile{
public:
  Falling(const Colour colour);
};

struct Empty {};
using Tile = std::variant<Empty, Taken, Falling>;

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
    std::unique_ptr<Tetromino> tetromino;
    Coordinates pivot_pos;
  };

  TileData _tile_data;
  std::optional<FallingTetromino> _falling_tetromino;
  std::vector<std::unique_ptr<Tetromino>> _shape_bag;
  std::mt19937 _bag_rng{std::random_device{}()};

  void place_tiles(const Colour tetromino_colour,
      const std::array<Coordinates, 4>& falling_tile_positions);
  void refill_bag();

public:
  GameSession();

  [[nodiscard]] auto get_tile_data() const noexcept -> const TileData&;
  void update_falling_tiles(const Colour tetromino_colour,
      const std::array<Coordinates, 4>& old_tile_positions,
      const std::array<Coordinates, 4>& new_tile_positions);
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
