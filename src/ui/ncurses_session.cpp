#include "ui/ncurses_session.hpp"
#include "ui/tui_colours.hpp"

#include <ncurses.h>
#include <clocale>

namespace {
void print_basic_info() {
  mvprintw(LINES * 19 / 20, COLS / 8, "Quit (x)");
  mvprintw(LINES / 2, COLS / 2, "'s' to start");
  refresh();
}
} // namespace

NCursesSession::NCursesSession() {
  setlocale(LC_ALL, ""); // utf-8 support
  initscr();
  curs_set(0); // hide cursor
  cbreak();
  noecho(); 
  TUIColours::init();
  keypad(stdscr, true);

  print_basic_info();
}

void NCursesSession::init() {
  static NCursesSession session;
}

NCursesSession::~NCursesSession() {
  endwin();
}
