#ifndef SRC_S21_MANUAL_TESTS_H_
#define SRC_S21_MANUAL_TESTS_H_

#include "../s21_brick_game.h"

#endif

char get_char(){
  static int i = -1;
  int chars[50] = {'1', 's', 'p', KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_DOWN, ' ', 'p', KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_DOWN, ' ', 'q', 'q'};
  i++;
  return chars[i];
}

int main() {
  srand(0);
  initNcurses();
  int ch;
  while (1) {
    printMenu();
    ch = get_char();
    switch (ch) {
      case '1':
        clear();
        tetris();
        break;
      case 'q':
      case 'Q':
        endwin();
        return 0;
      default:
        break;
    }
  }
}

void tetris() {
  int ch;
  bool hold = 0;
  FullGameInfo_t* state = getInfo();
  initInfo();
  struct timeval last_time, current_time;
  gettimeofday(&last_time, NULL);
  while (state->status != QUIT) {
    auto_fall(&last_time, &current_time);
    render(updateCurrentState());
    ch = get_char();
    userInput(functionKeys(ch), hold);
    nodelay(stdscr, state->status == GAME);
  }
}

