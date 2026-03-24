#ifndef GAME_HPP
#define GAME_HPP

#include <cstddef>
#include <array>
#include <optional>
#include <vector>

constexpr std::size_t game_width = 10;
constexpr std::size_t game_height = 20;

enum class TileState { Empty, Taken, Hovering };
using TileData = std::array<std::array<TileState, game_width>, game_height>;

enum Input {
  Quit = 'q',
  Start = 's',
  Left = 'a',
  Right = 'd',
  None,
};

struct Coordinates {
  int x;
  int y;
};

class Game {
private:
  struct GameData {
    TileData tile_data;
    std::vector<Coordinates> floating_tiles;
  };

  std::optional<GameData> _data = std::nullopt;

  void place_tiles(TileData& data, std::vector<Coordinates>& floating_tiles);

public:
  [[nodiscard]] auto get_tile_data() const -> const TileData&;
  [[nodiscard]] auto has_started() const noexcept -> bool;
  void start();
  auto try_move(const Input input) -> bool;
  void tick();
};

#endif // GAME_HPP
