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

// Цветовые пары
#define COLOR_BLOCK 1  // Основной цвет блоков
#define COLOR_GHOST 2  // Тёмно-серый цвет для "призрачных" блоков
#define COLOR_BORDER 3  // Цвет границ
#define COLOR_TEXT 4    // Цвет текста

void initNcurses();
void printMenu();
void render(GameInfo_t screen);
void initColors();

#endif