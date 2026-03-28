#include "game/tetromino_bag.hpp"

#include <algorithm>

namespace {
constexpr std::size_t num_bag_layers = 2;
constexpr std::size_t num_unique_tetrominoes = 7;
};

TetrominoBag::TetrominoBag() {
  _bag.reserve(num_bag_layers * num_unique_tetrominoes);
  refill();
}

void TetrominoBag::refill() {
  for (std::size_t i = 0; i < num_bag_layers; i++) {
    for (std::size_t j = 0; j < num_unique_tetrominoes; j++) {
      _bag.emplace_back(static_cast<TetrominoShape>(j));
    }
  }

  std::ranges::shuffle(_bag, _rng);
}

auto TetrominoBag::take() -> Tetromino {
  Tetromino tetromino = std::move(_bag.back());
  _bag.pop_back();

  if (_bag.empty()) {
    refill();
  }
  
  return tetromino;
}

auto TetrominoBag::get_next_tetromino_offsets() const -> const TileOffsets& {
  return _bag.back().get_offsets();
}
