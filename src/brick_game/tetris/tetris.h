#ifndef SRC_S21_TETRIS_H_
#define SRC_S21_TETRIS_H_

#include "../../s21_brick_game.h"

typedef enum{
    ALPHA,      //Линия
    BETA,       //Г
    GAMMA,      //Перевернутая Г
    DELTA,      //Квадрат
    OMEGA,      //Перевернутая Z
    PSI,        //Т
    ZETA        //Z
} BlockType;

typedef struct block_struct {
    int** matrix;
    int rows;
    int columns;
    BlockType type;
    int x;
    int y;
    
} Block;

typedef struct {
    GameInfo_t screen;
    Block block_now;
    Block block_next;

    int matrix_without_block[LENGTH][WIDTH];
} FullGameInfo_t;

void create_block(int type, Block *block);
void gen_block(Block *block);
void rotate(Block *block);
void remove_block(Block *block);

#endif