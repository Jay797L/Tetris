#include "tetris.h"

void create_block(Block *block) {
  int type = rand()%7;
  block->rows = (type == ALPHA ? 1 : 2);
  block->columns = (type == ALPHA ? 4 : (type == DELTA) ? 2 : 3);
  block->matrix = (int **)calloc(block->rows, sizeof(int *));
  for (int i = 0; i < block->rows; i++) {
    block->matrix[i] = (int *)calloc(block->columns, sizeof(int));
  }
  block->type = type;

  gen_block(block);
}

void gen_block(Block *block) {
  block->x = 0;
  block->y = (10 - block->rows)/2;
  switch (block->type) {
    case ALPHA:
      block->matrix[0][0] = 1;
      block->matrix[0][1] = 1;
      block->matrix[0][2] = 1;
      block->matrix[0][3] = 1;
      break;
    case BETA:
      block->matrix[0][0] = 1;
      block->matrix[1][0] = 1;
      block->matrix[1][1] = 1;
      block->matrix[1][2] = 1;
      break;
    case GAMMA:
      block->matrix[0][2] = 1;
      block->matrix[1][0] = 1;
      block->matrix[1][1] = 1;
      block->matrix[1][2] = 1;
      break;
    case DELTA:
      block->matrix[0][0] = 1;
      block->matrix[0][1] = 1;
      block->matrix[1][0] = 1;
      block->matrix[1][1] = 1;
      break;
    case OMEGA:
      block->matrix[0][1] = 1;
      block->matrix[0][2] = 1;
      block->matrix[1][0] = 1;
      block->matrix[1][1] = 1;
      break;
    case PSI:
      block->matrix[0][1] = 1;
      block->matrix[1][0] = 1;
      block->matrix[1][1] = 1;
      block->matrix[1][2] = 1;
      break;
    case ZETA:
      block->matrix[0][0] = 1;
      block->matrix[0][1] = 1;
      block->matrix[1][1] = 1;
      block->matrix[1][2] = 1;
      break;
  }
}

void rotate_block(Block *block) {
  Block bufer = *block;
  remove_block(block);
  block->rows = bufer.columns;
  block->columns = bufer.rows;
  for (int i = 0; i < bufer.rows; i++) {
    for (int j = 0; j < bufer.columns; j++)
      block->matrix[j][i] = bufer.matrix[i][j];
  }
  block->y += bufer.columns - block->columns;
  block->x += bufer.rows - block->rows;
}

void remove_block(Block *block) {
  for (int i = 0; i < block->rows; i++) {
    free(block->matrix[i]);
  }
  free(block->matrix);
  block->matrix = NULL;
}

void transfer_block(Block *now, Block *next){
  remove_block(now);
  now->rows = next->rows;
  now->columns = next->columns;
  now->matrix = next->matrix;
  now->type = next->type;
  now->y = next->y;
  now->x = next->x;

  create_block(next);
}

void transpose_block(Block *now, Block *bufer){
  bufer->rows = now->rows;
  bufer->columns = now->columns;
  bufer->matrix = now->matrix;
  now->rows = bufer->columns;
  now->columns = bufer->rows;
  now->matrix = (int **)calloc(now->rows, sizeof(int *));
  for (int i = 0; i < now->rows; i++) {
    now->matrix[i] = (int *)calloc(now->columns, sizeof(int));
  }

  for(int i = 0; i < bufer->rows; i++){
    for(int j = 0; j < bufer->columns; j++) now->matrix[bufer->columns - j - 1][i] = bufer->matrix[i][j];
  }
}