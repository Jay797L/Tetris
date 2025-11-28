#include "tetris.h"

FullGameInfo_t* getInfo() {
  static FullGameInfo_t state = {0};
  return &state;
}

void initInfo() {
  FullGameInfo_t* state = getInfo();
  state->screen.field = (int**)calloc(sizeof(int*), LENGTH);
  for (int i = 0; i < LENGTH; i++) {
    state->screen.field[i] = (int*)calloc(sizeof(int), WIDTH);
  }
  state->screen.next = (int**)calloc(sizeof(int*), 4);
  for (int i = 0; i < 4; i++) {
    state->screen.next[i] = (int*)calloc(sizeof(int), 4);
  }
  state->screen.score = 0;
  state->screen.high_score = 0;
  state->screen.level = 1;
  state->screen.speed = 1200;
  state->screen.pause = 0;

  state->rows_to_delete[0] = 0;
  for (int i = 1; i < 5; i++) {
    state->rows_to_delete[i] = -1;
  }

  state->status = START;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state->matrix_without_block[i][j] = 0;
    }
  }

  state->block_now.matrix = NULL;
  state->block_next.matrix = NULL;
}

void start_game() {
  FullGameInfo_t* state = getInfo();
  if (state->status == GAMEOVER) {
    remove_block(&state->block_now);
    remove_block(&state->block_next);
    state->screen.score = 0;
    state->screen.level = 1;
    state->screen.speed = 1200;
    state->screen.pause = 0;
    for (int i = 0; i < LENGTH; i++) {
      for (int j = 0; j < WIDTH; j++) state->matrix_without_block[i][j] = 0;
    }
  }
  if (state->status == START || state->status == GAMEOVER) {
    state->status = GAME;
    create_block(&state->block_now);
    create_block(&state->block_next);
  }
}

void pause_game() {
  FullGameInfo_t* state = getInfo();
  if (state->status == PAUSE)
    state->status = GAME;
  else if (state->status == GAME)
    state->status = PAUSE;
  state->screen.pause = state->status == PAUSE;
}

void terminate_game() {
  FullGameInfo_t* state = getInfo();
  if (state->status != START) {
    remove_block(&state->block_now);
    remove_block(&state->block_next);
  }
  state->status = QUIT;
  clean_screen(&state->screen);
}

void rotate() {
  FullGameInfo_t* state = getInfo();
  if (state->status != GAME || state->block_now.type == DELTA) return;
  Block buffer;
  buffer.y = state->block_now.y;
  transpose_block(&state->block_now, &buffer);
  if (state->block_now.type == ALPHA)
    state->block_now.y += (state->block_now.rows == 4 ? 1 : -1);
  if (state->block_now.y < 0 ||
      state->block_now.y + state->block_now.columns >= WIDTH)
    state->block_now.y =
        (state->block_now.y < 0 ? 0 : WIDTH - state->block_now.columns);
  if (full_field() == 0)
    remove_block(&buffer);
  else {
    remove_block(&state->block_now);
    state->block_now.matrix = buffer.matrix;
    state->block_now.rows = buffer.rows;
    state->block_now.columns = buffer.columns;
    state->block_now.y = buffer.y;
  }
}

int move_block(int move) {
  FullGameInfo_t* state = getInfo();
  int res = 0;
  if (state->status != GAME) return res;
  int old_x = state->block_now.x;
  int old_y = state->block_now.y;
  switch (move) {
    case RIGHT:
      state->block_now.y += 1;
      if (full_field() != 0) state->block_now.y = old_y;
      break;
    case LEFT:
      state->block_now.y -= 1;
      if (full_field() != 0) state->block_now.y = old_y;
      break;

    case DOWN:
      state->block_now.x += 1;
      if (full_field() != 0) {
        state->block_now.x = old_x;
        attachment();
        res = 1;
      }
      break;

    default:
      break;
  }
  return res;
}

void lower_block() {
  FullGameInfo_t* state = getInfo();
  if (state->status != GAME) return;
  while (move_block(DOWN) == 0);
}

void clean_screen(TetrisInfo* screen) {
  for (int i = 0; i < LENGTH; i++) {
    free(screen->field[i]);
  }
  free(screen->field);

  for (int i = 0; i < 4; i++) {
    free(screen->next[i]);
  }
  free(screen->next);
}

int full_field() {
  FullGameInfo_t* state = getInfo();
  Block block = state->block_now;
  int res = 0;
  if (block.x < 0 || block.x + block.rows > LENGTH || block.y < 0 ||
      block.y + block.columns > WIDTH) {
    res = 1;
  }

  for (int i = 0; i < LENGTH; i++) {
    for (int j = 0; j < WIDTH; j++) {
      state->screen.field[i][j] = state->matrix_without_block[i][j];
      int is_now_x = i - block.x;
      int is_now_y = j - block.y;
      if (is_now_x < block.rows && is_now_x >= 0 && is_now_y < block.columns &&
          is_now_y >= 0)
        state->screen.field[i][j] += block.matrix[is_now_x][is_now_y];
      if (state->screen.field[i][j] > 1) res = state->screen.field[i][j];
    }
  }
  return res;
}

void next_field() {
  FullGameInfo_t* state = getInfo();
  Block block = state->block_next;
  int x = 1;
  int y = 0;
  if (block.type == DELTA) y = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state->screen.next[i][j] = 0;
      int is_now_x = i - x;
      int is_now_y = j - y;
      if (is_now_x < block.rows && is_now_x >= 0 && is_now_y < block.columns &&
          is_now_y >= 0)
        state->screen.next[i][j] = block.matrix[is_now_x][is_now_y];
    }
  }
}

void attachment() {
  FullGameInfo_t* state = getInfo();
  full_field();
  int is_kill = 0;
  for (int i = 0; i < LENGTH; i++) {
    is_kill = 1;
    for (int j = 0; j < WIDTH; j++) {
      state->matrix_without_block[i][j] = state->screen.field[i][j];
      if (state->screen.field[i][j] == 0) is_kill = 0;
    }
    if (is_kill) kill_string_input(i);
  }
  kill_score_input();
  if (state->rows_to_delete[0] != 0) {
    killing_strings();
    remove_strings();
  }
  transfer_block(&state->block_now, &state->block_next);
  if (full_field() != 0) {
    game_over();
  }
}

void kill_string_input(int i) {
  FullGameInfo_t* state = getInfo();
  for (int j = 1; j < 5; j++) {
    if (state->rows_to_delete[j] == -1) {
      state->rows_to_delete[j] = i;
      break;
    }
  }
}

void kill_score_input() {
  FullGameInfo_t* state = getInfo();
  if (state->rows_to_delete[1] != -1) state->rows_to_delete[0] = 100;
  if (state->rows_to_delete[2] != -1) state->rows_to_delete[0] = 300;
  if (state->rows_to_delete[3] != -1) state->rows_to_delete[0] = 700;
  if (state->rows_to_delete[4] != -1) state->rows_to_delete[0] = 1500;
}

void killing_strings() {
  FullGameInfo_t* state = getInfo();
  for (int i = 1; i < 5; i++) {
    if (state->rows_to_delete[i] != -1) {
      for (int j = 0; j < WIDTH; j++) {
        state->screen.field[state->rows_to_delete[i]][j] = 2;
      }
      state->rows_to_delete[i] = -1;
    }
  }
  state->screen.score += state->rows_to_delete[0];
  state->screen.level =
      (state->screen.score <= 6000) ? (state->screen.score / 600 % 10 + 1) : 10;
  state->screen.speed = 1200 - state->screen.level * 100;
  state->rows_to_delete[0] = 0;
  if (state->screen.score > state->screen.high_score)
    state->screen.high_score = state->screen.score;
}

void remove_strings() {
  FullGameInfo_t* state = getInfo();
  int shift = 0;
  for (int i = LENGTH - 1; i >= 0; i--) {
    while ((i - shift > 0) && (state->screen.field[i - shift][0] == 2))
      shift += 1;
    for (int j = 0; j < WIDTH; j++) {
      if (i - shift >= 0) {
        state->matrix_without_block[i][j] = state->screen.field[i - shift][j];
      } else
        state->matrix_without_block[i][j] = 0;
    }
  }
}

void game_over() {
  FullGameInfo_t* state = getInfo();
  state->status = GAMEOVER;
}
