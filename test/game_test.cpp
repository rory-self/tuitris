#include "game/game_session.hpp"

#include <gtest/gtest.h>

namespace {
constexpr int test_seed = 1;

void apply_n_transformations(GameSession &game, const Transformation transformation, const std::size_t n) {
  for (std::size_t i = 0; i < n; i++) {
    EXPECT_TRUE(game.try_transformation(transformation));
  }
}

void drop_shape(GameSession& game) {
  EXPECT_TRUE(game.try_transformation(Transformation::Drop));
}
}

TEST(GameTest, ScoreSingle) {
  GameSession game(test_seed);
  
  // Place O Shape
  game.tick();
  apply_n_transformations(game, Transformation::Right, 3);
  drop_shape(game);

  // Place I Shape
  game.tick();
  drop_shape(game);

  // Place L Shape
  game.tick();
  apply_n_transformations(game, Transformation::Left, 3);
  drop_shape(game);

  // Place L Shape
  game.tick();
  apply_n_transformations(game, Transformation::Left, 4);
  apply_n_transformations(game, Transformation::RotateClockwise, 2);
  drop_shape(game);

  constexpr unsigned int single_score = 40;
  ASSERT_EQ(game.get_score(), single_score);
}
