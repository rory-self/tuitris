#include "game/tetromino.hpp"

#include "gtest/gtest.h"
#include <unordered_set>

namespace {
struct CoordinatesHasher {
  [[nodiscard]] auto operator()(const Coordinates& coords) const -> std::size_t {
    return coords.x ^ (coords.y << 1);
  }
};
using OffsetSet = std::unordered_set<Coordinates, CoordinatesHasher>;

void test_offsets_eq(const Tetromino& tetromino, const OffsetSet& expected_offsets) {
  for (const Coordinates& offset : tetromino.get_offsets()) {
    EXPECT_TRUE(expected_offsets.contains(offset));
  }
}
}

TEST(TetrominoTest, LSpawnOffsets) {
  const Tetromino l_tetromino(TetrominoShape::L);

  const OffsetSet expected_offsets = {{
    { .x = -1, .y =  0 },
    { .x =  1, .y =  0 },
    { .x =  1, .y = -1 },
  }};
  test_offsets_eq(l_tetromino, expected_offsets);
}

TEST(TetrominoTest, OSpawnOffsets) {
  const Tetromino o_tetromino(TetrominoShape::O);

  const OffsetSet expected_offsets = {{
    { .x = 0, .y = -1 },
    { .x = 1, .y =  0 },
    { .x = 1, .y = -1 },
  }};
  test_offsets_eq(o_tetromino, expected_offsets);
}

TEST(TetrominoTest, ISpawnOffsets) {
  const Tetromino i_tetromino(TetrominoShape::I);

  const OffsetSet expected_offsets = {{
    { .x = -1, .y = 0 },
    { .x =  1, .y =  0 },
    { .x =  2, .y = 0 },
  }};
  test_offsets_eq(i_tetromino, expected_offsets);
}

TEST(TetrominoTest, SSpawnOffsets) {
  const Tetromino s_tetromino(TetrominoShape::S);

  const OffsetSet expected_offsets = {{
    { .x = -1, .y =  0 },
    { .x =  0, .y = -1 },
    { .x =  1, .y = -1 },
  }};
  test_offsets_eq(s_tetromino, expected_offsets);
}

TEST(TetrominoTest, ZSpawnOffsets) {
  const Tetromino z_tetromino(TetrominoShape::Z);

  const OffsetSet expected_offsets = {{
    { .x = -1, .y = -1 },
    { .x =  0, .y = -1 },
    { .x =  1, .y =  0 },
  }};
  test_offsets_eq(z_tetromino, expected_offsets);
}

TEST(TetrominoTest, JSpawnOffsets) {
  const Tetromino j_tetromino(TetrominoShape::J);

  const OffsetSet expected_offsets = {{
    { .x = -1, .y =  0 },
    { .x = -1, .y = -1 },
    { .x =  1, .y =  0 },
  }};
  test_offsets_eq(j_tetromino, expected_offsets);
}

TEST(TetrominoTest, TSpawnOffsets) {
  const Tetromino t_tetromino(TetrominoShape::T);

  const OffsetSet expected_offsets = {{
    { .x = -1, .y =  0 },
    { .x =  0, .y = -1 },
    { .x =  1, .y =  0 },
  }};
}

