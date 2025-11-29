#include "../../gui/cli/cli.h"
#include "tetris.h"
void create_block(Block *block) {
  int type = getBlock();
  block->rows = (type == ALPHA ? 1 : 2);
  block->columns = (type == ALPHA ? 4 : (type == DELTA) ? 2 : 3);
  block->matrix = (int **)calloc(block->rows, sizeof(int *));
  for (int i = 0; i < block->rows; i++)
    block->matrix[i] = (int *)calloc(block->columns, sizeof(int));

  block->type = type;

  gen_block(block);
}

void gen_block(Block *block) {
  block->x = 0;
  block->y = (10 - block->rows) / 2;
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

void remove_block(Block *block) {
  for (int i = 0; i < block->rows; i++) free(block->matrix[i]);
  free(block->matrix);
  block->matrix = NULL;
}

void transfer_block(Block *now, Block *next) {
  remove_block(now);
  now->rows = next->rows;
  now->columns = next->columns;
  now->matrix = next->matrix;
  now->type = next->type;
  now->y = next->y;
  now->x = next->x;

  create_block(next);
}

void transpose_block(Block *now, Block *bufer) {
  bufer->rows = now->rows;
  bufer->columns = now->columns;
  bufer->matrix = now->matrix;
  now->rows = bufer->columns;
  now->columns = bufer->rows;
  now->matrix = (int **)calloc(now->rows, sizeof(int *));
  for (int i = 0; i < now->rows; i++)
    now->matrix[i] = (int *)calloc(now->columns, sizeof(int));

  for (int i = 0; i < bufer->rows; i++)
    for (int j = 0; j < bufer->columns; j++)
      now->matrix[bufer->columns - j - 1][i] = bufer->matrix[i][j];
}

Bag *getBag() {
  static Bag bag = {0};
  static short init = 1;
  if (init) {
    for (int i = 0; i < 35; i++) bag.bag[i] = i / 7;
    for (int i = 0; i < 7; i++) bag.types[i] = 5;
    bag.story = NULL;
    bag.minimum = rand() % 7;
  }
  return &bag;
}

BlockType getBlock() {
  Bag *bag = getBag();
  struct History *story = (struct History *)calloc(sizeof(struct History), 1);
  short index = 0;
  do index = rand() % 35;
  while (nebolshaya_istoricheskaya_spravka(bag->bag[index]));
  story->block = bag->bag[index];
  story->next = bag->story;
  bag->story = story;
  new_minimum(index);
  return (BlockType)bag->story->block;
}

int nebolshaya_istoricheskaya_spravka(BlockType type) {
  Bag *bag = getBag();
  int res = 0;
  if (bag->story != NULL && bag->story->next != NULL &&
      bag->story->next->next != NULL) {
    res = (bag->story->block == type && bag->story->next->block == type &&
           bag->story->next->next->block == type);
    if (!res) free(bag->story->next->next);
  }
  return res;
}

void new_minimum(int index) {
  Bag *bag = getBag();
  short min = -1;
  bag->bag[index] = bag->minimum;
  bag->types[bag->minimum] += 1;
  mvprintw(1, 50, "OKOKOKOKOKOKOKOKOK %hd", bag->story->block);
  refresh();
  bag->types[bag->story->block] -= 1;
  mvprintw(2, 50, "OKOKOKOKOKOKOKOKOK");
  refresh();
  for (short i = 0; i < 7; i++)
    if (min > bag->types[i] && i != (short)bag->story->block &&
        i != (short)bag->story->next->block &&
        i != (short)bag->story->next->next->block) {
      min = bag->types[i];
      bag->minimum = i;
    }
}