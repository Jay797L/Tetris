#include "tetris.h"

void userInput(UserAction_t action, bool hold){
    switch (action)
    {
    case Start:
        start_game();
        break;
    case Pause:
        pause_game();
        break;
    case Terminate:
        terminate_game();
        break;
    case Up:
        rotate_block();
        break;
    case Left:
        move_block(Left);
        break;
    case Right:
        move_block(Right);
        break;
    case Down:
        move_block(Down);
        break;
    case Action:
        lower_block();
        break;
    }
}

// void start_game(){
//     if(tetris == NULL){
//         srand(time(NULL));
//         tetris = calloc(sizeof(FullGameInfo_t), 1);
//         create_block(rand()%7, &tetris->block_now);
//         gen_block(&tetris->block_now);
//         create_block(rand()%7, &tetris->block_next);
//         gen_block(&tetris->block_next);
//         tetris->screen.field = calloc(sizeof(int*), LENGTH);
//         for(int i = 0; i < LENGTH; i ++) tetris->screen.field[i] = calloc(sizeof(int), WIDTH);
//         tetris->screen.next = calloc(sizeof(int*), 4);
//         for(int i = 0; i < 4; i ++) tetris->screen.next[i] = calloc(sizeof(int), WIDTH);
//     }
// }

void input_block(){

}

GameInfo_t updateCurrentState(){
    
}

void move_block(UserAction_t move){
    
}