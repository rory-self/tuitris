#include "tetromino.hpp"

#include <random>

class TetrominoBag {
private:
  std::vector<Tetromino> _bag;
  std::mt19937 _rng;

  void init_bag();
  void refill();

public:
  explicit TetrominoBag(std::optional<int> seed = std::nullopt);

  [[nodiscard]] auto get_next_tetromino_offsets() const -> const TileOffsets&;
  [[nodiscard]] auto take() -> Tetromino;
};
