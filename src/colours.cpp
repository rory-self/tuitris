#include "colours.hpp"

void TUIColours::init() {
  if (not has_colors()) {
    // TODO
    return;
  }

  start_color();
  use_default_colors();

  for (std::size_t i = 0; i < NUM_COLOURS; i++) {
    const short id = static_cast<short>(i);
    const auto [colour, ncurses_colour] = _colour_mappings[i];

    init_pair(id, ncurses_colour, ncurses_colour);
    _pair_id_by_colour[colour] = id;
  }
}

auto TUIColours::colour_to_ncurses_pair(const Colour colour) -> chtype {
  const short colour_pair_id = _pair_id_by_colour[colour];
  return COLOR_PAIR(colour_pair_id);
}
