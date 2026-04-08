#include "game/tetromino.hpp"

#include "gtest/gtest.h"
#include <unordered_set>

namespace {
constexpr Coordinates dummy_tetromino_pos = { .x = 2, .y = 2 };

struct CoordinatesHasher {
  [[nodiscard]] auto operator()(const Coordinates& coords) const -> std::size_t {
    return coords.x ^ (coords.y << 1);
  }
};
using CoordinateSet = std::unordered_set<Coordinates, CoordinatesHasher>;

void test_offsets_eq(const Tetromino& tetromino, const CoordinateSet& expected_offsets) {
  for (const Coordinates& offset : tetromino.get_offsets()) {
    EXPECT_TRUE(expected_offsets.contains(offset));
  }
}

[[nodiscard]] auto dummy_placement_method(const TilePositions& old_pos, const TilePositions& new_pos) -> bool {
  return true;
}
}

TEST(TetrominoTest, LSpawnOffsets) {
  const Tetromino l_tetromino(TetrominoShape::L);

  const CoordinateSet expected_offsets = {{
    { .x = -1, .y =  0 },
    { .x =  1, .y =  0 },
    { .x =  1, .y = -1 },
  }};
  test_offsets_eq(l_tetromino, expected_offsets);
}

TEST(TetrominoTest, OSpawnOffsets) {
  const Tetromino o_tetromino(TetrominoShape::O);

  const CoordinateSet expected_offsets = {{
    { .x = 0, .y = -1 },
    { .x = 1, .y =  0 },
    { .x = 1, .y = -1 },
  }};
  test_offsets_eq(o_tetromino, expected_offsets);
}

TEST(TetrominoTest, ISpawnOffsets) {
  const Tetromino i_tetromino(TetrominoShape::I);

  const CoordinateSet expected_offsets = {{
    { .x = -1, .y = 0 },
    { .x =  1, .y =  0 },
    { .x =  2, .y = 0 },
  }};
  test_offsets_eq(i_tetromino, expected_offsets);
}

TEST(TetrominoTest, SSpawnOffsets) {
  const Tetromino s_tetromino(TetrominoShape::S);

  const CoordinateSet expected_offsets = {{
    { .x = -1, .y =  0 },
    { .x =  0, .y = -1 },
    { .x =  1, .y = -1 },
  }};
  test_offsets_eq(s_tetromino, expected_offsets);
}

TEST(TetrominoTest, ZSpawnOffsets) {
  const Tetromino z_tetromino(TetrominoShape::Z);

  const CoordinateSet expected_offsets = {{
    { .x = -1, .y = -1 },
    { .x =  0, .y = -1 },
    { .x =  1, .y =  0 },
  }};
  test_offsets_eq(z_tetromino, expected_offsets);
}

TEST(TetrominoTest, JSpawnOffsets) {
  const Tetromino j_tetromino(TetrominoShape::J);

  const CoordinateSet expected_offsets = {{
    { .x = -1, .y =  0 },
    { .x = -1, .y = -1 },
    { .x =  1, .y =  0 },
  }};
  test_offsets_eq(j_tetromino, expected_offsets);
}

TEST(TetrominoTest, TSpawnOffsets) {
  const Tetromino t_tetromino(TetrominoShape::T);

  const CoordinateSet expected_offsets = {{
    { .x = -1, .y =  0 },
    { .x =  0, .y = -1 },
    { .x =  1, .y =  0 },
  }};
  test_offsets_eq(t_tetromino, expected_offsets);
}

TEST(TetrominoTest, GetTilePositions) {
  const Tetromino t_tetromino(TetrominoShape::T);

  const auto& [dummy_x, dummy_y] = dummy_tetromino_pos;
  const CoordinateSet expected_positions = {{
    { .x = dummy_x - 1, .y = dummy_y },
    { .x = dummy_x + 1, .y = dummy_y },
    { .x = dummy_x, .y = dummy_y - 1 },
    dummy_tetromino_pos,
  }};

  for (const Coordinates& pos : t_tetromino.calc_tile_positions(dummy_tetromino_pos)) {
    EXPECT_TRUE(expected_positions.contains(pos));
  }
}

TEST(TetrominoTest, BasicRightRotation) {
  Tetromino t_tetromino(TetrominoShape::T);
  EXPECT_TRUE(t_tetromino.try_rotate(dummy_tetromino_pos, dummy_placement_method, true));

  const CoordinateSet expected_offsets = {{
    { .x = 0, .y = -1 },
    { .x = 1, .y =  0 },
    { .x = 0, .y =  1 },
  }};
  test_offsets_eq(t_tetromino, expected_offsets);
}

TEST(TetrominoTest, BasicLeftRotation) {
  Tetromino t_tetromino(TetrominoShape::T);
  EXPECT_TRUE(t_tetromino.try_rotate(dummy_tetromino_pos, dummy_placement_method, false));

  const CoordinateSet expected_offsets = {{
    { .x =  0, .y = -1 },
    { .x = -1, .y =  0 },
    { .x =  0, .y =  1 },
  }};
  test_offsets_eq(t_tetromino, expected_offsets);
}
