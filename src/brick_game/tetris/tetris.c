#include "tetris.h"

FullGameInfo_t* getInfo() {
  static FullGameInfo_t state = {0};
  return &state;
}

void initInfo(){
    FullGameInfo_t* state = getInfo();
    state->screen.field = (int**)calloc(sizeof(int*), LENGTH);
    for (int i = 0; i < LENGTH; i++){
        state->screen.field[i] = (int*)calloc(sizeof(int), WIDTH);
    }
    state->screen.next = (int**)calloc(sizeof(int*), 4);
    for (int i = 0; i < 4; i++){
        state->screen.next[i] = (int*)calloc(sizeof(int), 4);
    }
    state->screen.score = 0;
    state->screen.high_score = 0;
    state->screen.level = 1;
    state->screen.speed = 1;
    state->screen.pause = 0;



    state->status = 0;

    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            state->matrix_without_block[i][j] = 0;
        }
    }


}

void start_game(){
    FullGameInfo_t* state = getInfo();
    if(state->status == START){
      state->status = GAME;

    }
}

void pause_game(){
    //FullGameInfo_t* state = getInfo();
}

void terminate_game(){
    FullGameInfo_t* state = getInfo();
    remove_block(&state->block_now);
    remove_block(&state->block_next);
    state->status = QUIT;
    clean_screen(&state->screen);
}

void rotate_block(){
    //FullGameInfo_t* state = getInfo();
}

void move_block(int move){
  FullGameInfo_t* state = getInfo();
  switch (move)
  {
  case RIGHT:
    state->block_now->x += 1;
    break;
  case LEFT:
    state->block_now->x -= 1;
    break;
  case DOWN:
    state->block_now->y -= 1;
    break;
  
  default:
    break;
  } 
}

void lower_block(){
    //FullGameInfo_t* state = getInfo();
}

void clean_screen(TetrisInfo* screen){
  for(int i = 0; i < LENGTH; i++){
    free(screen->field[i]);
  }
  free(screen->field);

  for(int i = 0; i < 4; i++){
    free(screen->next[i]);
  }
  free(screen->next);
}
