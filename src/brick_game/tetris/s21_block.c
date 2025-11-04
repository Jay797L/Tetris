#include "tetris.h"

void create_block(int type, Block *block){
    block->rows = (type == ALPHA ? 1 : 2);
    block->columns = (type == ALPHA ? 4 : (type == DELTA) ? 2 : 3);
    block->matrix = (int **)calloc(block->rows, sizeof(int *));
    for(int i = 0; i < block->rows; i++){
        block->matrix[i] = (int *)calloc(block->columns, sizeof(int));
    }
    block->type = type;
    gen_block(block);
}

void gen_block(Block *block){
    switch (block->type)
    {
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

void rotate(Block *block){
    Block bufer = *block;
    remove_block(block);
    block->rows = bufer.columns;
    block->columns = bufer.rows;
    for(int i = 0; i < bufer.rows; i++){
        for(int j = 0; j < bufer.columns; j++) block->matrix[j][i] = bufer.matrix[i][j];
    }
    block->y += bufer.columns - block->columns;
    block->x += bufer.rows - block->rows;
}

void remove_block(Block *block){
    for(int i = 0; i < block->rows; i++){
        free(block->matrix[i]);
    }
    free(block->matrix);
}

