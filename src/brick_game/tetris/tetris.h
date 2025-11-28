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

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} TetrisInfo;

typedef enum {
  ALPHA,  // Линия
  BETA,   // Г
  GAMMA,  // Перевернутая Г
  DELTA,  // Квадрат
  OMEGA,  // Перевернутая Z
  PSI,    // Т
  ZETA    // Z
} BlockType;

typedef struct {
  int **matrix;
  int rows;
  int columns;
  int x;
  int y;
  BlockType type;
} Block;

typedef enum { START, PAUSE, GAMEOVER, GAME, QUIT } GameStatus;

typedef struct {
  TetrisInfo screen;
  Block block_now;
  Block block_next;
  int matrix_without_block[LENGTH][WIDTH];
  int rows_to_delete[5];
  GameStatus status;
} FullGameInfo_t;

// typedef struct{
//   short history[3];
//   short types[7];
//   short bag[35];
//   short minimum;
// } Bag;

FullGameInfo_t *getInfo();

void initInfo();

void create_block(Block *block);
void gen_block(Block *block);
void rotate_block(Block *block);
void remove_block(Block *block);
void transfer_block(Block *now, Block *next);
void transpose_block(Block *now, Block *bufer);

// BlockType getType();

void start_game();
void pause_game();
void terminate_game();
void rotate();
int move_block(int move);
void lower_block();

void clean_screen(TetrisInfo *screen);
int full_field();
void next_field();
void attachment();

void kill_string_input(int i);
void kill_score_input();
void killing_strings();
void remove_strings();

void game_over();
#endif