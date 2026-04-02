#include "game/game_session.hpp"
#include "inputs.hpp"
#include "ui/ncurses_session.hpp"
#include "ui/game_window.hpp"
#include "game/game.hpp"

#include <ncurses.h>
#include <chrono>
#include <functional>

namespace {
using Clock = std::chrono::steady_clock;
using Time = std::chrono::time_point<Clock>;

// Prototypes //
[[nodiscard]] auto read_seed_arg(std::span<const char *const> args) -> std::optional<int>;
void game_routine(GameSession& game, const GameWindows& game_win, Input input, Time& next_tick);
} // namespace

// Implementation //
auto main(const int argc, const char *const argv[]) -> int {
  const std::optional<int> seed = read_seed_arg(std::span(argv, argc)); 
  NCursesSession::init();

  Game game;
  std::optional<GameWindows> game_win;

  Input input = Input::None;
  Time next_tick = Clock::now();
  while (input != Input::Quit) {
    const bool has_started = game.has_started();
    if (const auto session = game.get_session()) {
      game_routine(*session, game_win.value(), input, next_tick);      
    } 

    input = capture_input(has_started);
    if (input == Input::Start and not has_started) {
      const GameSession& session = game.start(seed);
      game_win.emplace(session);
      input = None;
    } 
  }
}

namespace {
auto read_seed_arg(const std::span<const char *const> args) -> std::optional<int> {
  if (args.size() <= 1) {
    return std::nullopt;
  }

  const std::string seed_arg = *(args.cbegin() + 1);
  return std::stoi(seed_arg);
}

void game_routine(GameSession& game, const GameWindows& game_wins, const Input input, Time& next_tick) {
  const auto transformation_res = std::invoke([&game, input] -> TransformationRes {
    const auto transformation = input_to_transformation(input);
    return transformation ? game.try_transformation(*transformation) : TransformationRes::Fail;
  });

  const bool ticked = Clock::now() >= next_tick;
  if (ticked) {
    game.tick();
  }

  const bool reset_tick = transformation_res == TransformationRes::ResetTick;
  if (ticked or reset_tick) {
    using namespace std::chrono_literals;
    next_tick = Clock::now() + 300ms;
  }

  if (ticked or reset_tick or transformation_res == TransformationRes::Success) {
    game_wins.update();
  }
}
} // namespace
