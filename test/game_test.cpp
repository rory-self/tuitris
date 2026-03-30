#include "game/game_session.hpp"

#include <gtest/gtest.h>

namespace {
class GameSessionTest : public ::testing::Test {
private:
  static constexpr int test_seed = 1;
  
  void prepare_single() {
    // Place O Shape
    right(3);
    drop();

    // Place I Shape
    drop();

    // Place L Shape
    left(3);
    drop();
  }

  void prepare_double() {
    prepare_single();
   
    // Place L Shape
    rotate_clockwise();
    left(4);
    drop();

    // Place J Shape
    drop();

    // Place J Shape
    flip();
    right();
    drop();
  }

  void prepare_triple() {
    prepare_double();

    // Place S
    left(2);
    drop();

    // Place O
    right(3);
    drop();
  }

  void apply_n_transformations(Transformation transformation, std::size_t n) {
    for (std::size_t i = 0; i < n; i++) {
      game.try_transformation(transformation);
    }
  }

  void left(const std::size_t n = 1) {
    apply_n_transformations(Transformation::Left, n);
  }

  void right(const std::size_t n = 1) {
    apply_n_transformations(Transformation::Right, n);
  }

  void rotate_clockwise(const std::size_t n = 1) {
    apply_n_transformations(Transformation::RotateClockwise, n);
  }

  void rotate_anticlockwise(const std::size_t n = 1) {
    apply_n_transformations(Transformation::RotateAntiClockwise, n);
  }

  void flip() {
    apply_n_transformations(Transformation::RotateClockwise, 2);
  }

  void complete_with_left_i() {
    rotate_anticlockwise();
    left(5);
    drop();
  }

protected:
  GameSession game;

  GameSessionTest(): game(test_seed) {}

  void drop(const std::size_t n = 1) {
    apply_n_transformations(Transformation::Drop, n);
  }

  void complete_single() {
    prepare_single();
    
    // Place L Shape
    left(4);
    flip();
    drop();
  }

  void complete_double() {
    prepare_double();
   
    drop(4);

    complete_with_left_i();
  }

  void complete_triple() {
    prepare_triple();

    drop(2);

    complete_with_left_i();
  }

  void complete_tetris() {
    prepare_triple();

    // Place T
    right();
    drop();

    // Place T
    flip();
    left(3);
    drop();

    complete_with_left_i();
  } 
};
} // namespace

// Tests //
TEST_F(GameSessionTest, GameOver) {
  constexpr std::size_t drops_required_for_game_over = 12;
  for (std::size_t i = 0; i < drops_required_for_game_over; i++) {
    ASSERT_FALSE(game.game_over());
    drop();
  }

  EXPECT_TRUE(game.game_over());
}

TEST_F(GameSessionTest, ScoreSingle) {
  complete_single();

  constexpr unsigned int single_score = 40;
  EXPECT_EQ(game.get_score(), single_score);
}

TEST_F(GameSessionTest, ScoreDouble) {
  complete_double();

  constexpr unsigned int double_score = 100;
  EXPECT_EQ(game.get_score(), double_score);
}

TEST_F(GameSessionTest, ScoreTriple) {
  complete_triple();
  constexpr unsigned int triple_score = 300;
  EXPECT_EQ(game.get_score(), triple_score);
}

TEST_F(GameSessionTest, ScoreTetris) {
  complete_tetris();
  
  constexpr unsigned int tetris_score = 1200;
  EXPECT_EQ(game.get_score(), tetris_score);
}
