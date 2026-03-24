#ifndef GAME_HPP
#define GAME_HPP

#include <cstddef>
#include <array>
#include <optional>

namespace {
constexpr std::size_t game_width = 10;
constexpr std::size_t game_height = 20;

enum class TileState { Empty, Taken, Hovering };
using TileData = std::array<std::array<TileState, game_width>, game_height>;
} // namespace

class Game {
private:
  std::optional<TileData> _tile_data;

public:
  [[nodiscard]] bool has_started() const noexcept;
  void start();
  void tick();
};

#endif // GAME_HPP
