#include "ui/window.hpp"
#include "ui/tui_colours.hpp"

Window::Window(const int height, const int width, const int y_pos, const int x_pos)
    : _window(newwin(height, width, y_pos, x_pos), delwin) {}

void Window::init_titled(const std::string_view title) const {
  box(_window.get(), 0, 0);
  move_print({ .x = 1, .y = 0 }, title);
}

void Window::move_print(const Coordinates& pos, const std::string_view text) const {
  mvwprintw(_window.get(), pos.y, pos.x, "%s", text.data());
}

void Window::draw_border(const std::size_t box_bottom_y,
    const std::size_t box_top_y, const std::size_t box_left_x, const std::size_t box_right_x) const {
  mvwvline(_window.get(), box_top_y, box_left_x, ACS_VLINE, play_area_height);
  mvwvline(_window.get(), box_top_y, box_right_x, ACS_VLINE, play_area_height);
  mvwhline(_window.get(), box_bottom_y, 2, ACS_HLINE, box_right_x - 2);
  mvwhline(_window.get(), box_top_y - 1, box_left_x + 1, '-', box_right_x - 2);

  mvwaddch(_window.get(), box_top_y - 1, box_left_x, ACS_ULCORNER);
  mvwaddch(_window.get(), box_top_y - 1, box_right_x, ACS_URCORNER);
  mvwaddch(_window.get(), box_bottom_y, box_left_x, ACS_LLCORNER);
  mvwaddch(_window.get(), box_bottom_y, box_right_x, ACS_LRCORNER);
}

void Window::erase() const {
  werase(_window.get());
}

void Window::refresh() const {
  wrefresh(_window.get());
}

void Window::print_block(const Coordinates& pos, const Colour colour) const {
  const chtype ncurses_colour = TUIColours::colour_to_ncurses_pair(colour);

  wattron(_window.get(), ncurses_colour);
  move_print({ .x = (pos.x + 1) * 2, .y = pos.y + 1 }, "██");
  wattroff(_window.get(), ncurses_colour);
}

