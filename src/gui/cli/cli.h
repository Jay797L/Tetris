#ifndef SRC_S21_CLI_H_
#define SRC_S21_CLI_H_

#include <ncurses.h>
#include <string.h>

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void initNcurses();

void printMenu();

void render();

#endif