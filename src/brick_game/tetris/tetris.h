#ifndef SRC_S21_TETRIS_H_
#define SRC_S21_TETRIS_H_

#define LENGTH 20
#define WIDTH 10

#define LEFT 3
#define RIGHT 4
#define DOWN 6

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



typedef enum {
  ALPHA,  // Линия
  BETA,   // Г
  GAMMA,  // Перевернутая Г
  DELTA,  // Квадрат
  OMEGA,  // Перевернутая Z
  PSI,    // Т
  ZETA    // Z
} BlockType;

typedef struct{
  int **matrix;
  int rows;
  int columns;
  BlockType type;
  int x;
  int y;

} Block;

typedef enum{
  START,
  PAUSE,
  GAMEOVER,
  GAME
} GameStatus;

typedef struct {
  TetrisInfo screen;
  Block block_now;
  Block block_next;
  GameStatus status;
  int matrix_without_block[LENGTH][WIDTH];
} FullGameInfo_t;

FullGameInfo_t* getInfo() {
  static FullGameInfo_t state = {0};
  return &state;
}

void initInfo();

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} TetrisInfo;

void create_block(int type, Block *block);
void gen_block(Block *block);
void rotate(Block *block);
void remove_block(Block *block);

void start_game();
void pause_game();
void terminate_game();
void rotate_block();
void move_block(int move);
void lower_block();

#endif