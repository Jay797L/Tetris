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
    for (int i = 0; i < 35; i++) bag.bag[i] = i / 5;
    bag.order = NULL;
    bag.history = NULL;
    init = 0;
  }
  return &bag;
}

BlockType getBlock() {
  Bag *bag = getBag();
  List *story = NULL;
  List *buf = NULL;
  short index = 0;
  short i = 0;
  do {
    index = rand() % 35;
    i++;
  } while (nebolshaya_istoricheskaya_spravka(bag->bag[index]) && i < 6);
  create_list(&story, bag->bag[index]);
  story->next = bag->history;
  bag->history = story;
  updateOrder(index);  // тута
  buf = bag->history;
  for (int i = 0; i < 3 && buf->next != NULL; i++) {
    if (i == 2 && buf->next != NULL) {
      free(buf->next);
      buf->next = NULL;
    } else
      buf = buf->next;
  }
  return (BlockType)bag->history->block;
}

short nebolshaya_istoricheskaya_spravka(BlockType type) {
  Bag *bag = getBag();
  short res = 0;
  List *buf = bag->history;
  for (int i = 0; i < 3 && buf != NULL; i++) {
    res += buf->block == type;
    buf = buf->next;
  }
  return res;
}

void updateOrder(short index) {
  Bag *bag = getBag();
  BlockType type = bag->bag[index];
  List *prev = NULL;
  List *current = bag->order;
  List *found = NULL;
  while (current != NULL) {
    if (current->block == type) {
      found = current;
      break;
    }
    prev = current;
    current = current->next;
  }
  if (found != NULL) {
    if (prev != NULL) {
      prev->next = found->next;
    } else {
      bag->order = found->next;
    }
    List *last = bag->order;
    while (last != NULL && last->next != NULL) {
      last = last->next;
    }
    found->next = NULL;
    if (last == NULL) {
      bag->order = found;
    } else {
      last->next = found;
    }
  } else {
    List *new_node = (List *)calloc(sizeof(List), 1);
    new_node->block = type;
    new_node->next = NULL;
    List *last = bag->order;
    if (last == NULL) {
      bag->order = new_node;
    } else {
      while (last->next != NULL) {
        last = last->next;
      }
      last->next = new_node;
    }
  }
  bag->bag[index] = bag->order->block;
}

void create_list(List **list, BlockType value) {
  *list = (List *)calloc(sizeof(List), 1);
  (*list)->next = NULL;
  (*list)->block = value;
}

void remove_bag() {
  Bag *bag = getBag();
  List *buf;
  if (bag->history != NULL) {
    while (bag->history->next != NULL) {
      buf = bag->history->next;
      bag->history->next = bag->history->next->next;
      free(buf);
    }
    free(bag->history);
  }
  if (bag->order != NULL) {
    while (bag->order->next != NULL) {
      buf = bag->order->next;
      bag->order->next = bag->order->next->next;
      free(buf);
    }
    free(bag->order);
  }
}
