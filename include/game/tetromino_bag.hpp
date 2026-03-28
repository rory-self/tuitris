#include "tetromino.hpp"

#include <random>

class TetrominoBag {
private:
  std::vector<Tetromino> _bag;
  std::mt19937 _rng{std::random_device{}()};

  void refill();

public:
  TetrominoBag();

  [[nodiscard]] auto get_next_tetromino_offsets() const -> const TileOffsets&;
  [[nodiscard]] auto take() -> Tetromino;
};
