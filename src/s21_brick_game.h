#ifndef SRC_S21_BRICK_GAME_H_
#define SRC_S21_BRICK_GAME_H_

#include "brick_game/tetris/tetris.h"
#include "gui/cli/cli.h"


typedef union {
    GameInfo_t screen;
    TetrisInfo tetris;
} s21_brick_game;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

void tetris();

UserAction_t functionKeys(int ch);

#endif