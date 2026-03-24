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

  void place_tiles(GameData& data);

public:
  [[nodiscard]] bool has_started() const noexcept;
  void start();
  TileData tick();
};

#endif // GAME_HPP
