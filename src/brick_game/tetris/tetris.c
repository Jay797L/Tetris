#include "tetris.h"

// void start_game(){
//     if(tetris == NULL){
//         srand(time(NULL));
//         tetris = calloc(sizeof(FullGameInfo_t), 1);
//         create_block(rand()%7, &tetris->block_now);
//         gen_block(&tetris->block_now);
//         create_block(rand()%7, &tetris->block_next);
//         gen_block(&tetris->block_next);
//         tetris->screen.field = calloc(sizeof(int*), LENGTH);
//         for(int i = 0; i < LENGTH; i ++) tetris->screen.field[i] =
//         calloc(sizeof(int), WIDTH); tetris->screen.next =
//         calloc(sizeof(int*), 4); for(int i = 0; i < 4; i ++)
//         tetris->screen.next[i] = calloc(sizeof(int), WIDTH);
//     }
// }

void initInfo(){
    FullGameInfo_t* state = getInfo();
    state->screen.field = (int**)calloc(sizeof(int*), WIDTH);
    for (int i = 0; i < WIDTH; i++){
        state->screen.field[i] = (int*)calloc(sizeof(int), LENGTH);
    }
    state->screen.next = (int**)calloc(sizeof(int*), 4);
    for (int i = 0; i < WIDTH; i++){
        state->screen.next[i] = (int*)calloc(sizeof(int), 4);
    }
    state->screen.score = 0;
    state->screen.high_score = 0;
    state->screen.level = 0;
    state->screen.speed = 0;
    state->screen.pause = 0;

    state->status = 0;
}

void start_game(){
    FullGameInfo_t* state = getInfo();
}

void pause_game(){
    FullGameInfo_t* state = getInfo();
}

void terminate_game(){
    FullGameInfo_t* state = getInfo();
}

void rotate_block(){
    FullGameInfo_t* state = getInfo();
}

void move_block(int move){
    FullGameInfo_t* state = getInfo();
}

void lower_block(){
    FullGameInfo_t* state = getInfo();
}
