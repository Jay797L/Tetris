#include "s21_brick_game.h"

UserAction_t functionKeys(int ch) {
  UserAction_t res = -1;
  switch (ch) {
    case ('s'):
      res = Start;
      break;
    case ('p'):
      res = Pause;
      break;
    case ('q'):
      res = Terminate;
      break;
    case (KEY_UP):
      res = Action;
      break;
    case (KEY_LEFT):
      res = Left;
      break;
    case (KEY_RIGHT):
      res = Right;
      break;
    case (KEY_DOWN):
      res = Down;
      break;
    case (' '):
      res = Up;
      break;
    default:
      break;
  }
  return res;
}

void userInput(UserAction_t action, bool hold) {
  switch (action) {
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
      lower_block();
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
      rotate();
      break;
    default:
      break;
  }
  if (hold == 1) hold = 0;
}

GameInfo_t updateCurrentState() {
  FullGameInfo_t* state = getInfo();
  full_field();
  next_field();
  s21_brick_game screen;
  screen.tetris = state->screen;
  return screen.screen;
}

void auto_fall(struct timeval* last_time, struct timeval* current_time) {
  FullGameInfo_t* state = getInfo();
  // Получаем текущее время
  gettimeofday(current_time, NULL);

  // Вычисляем прошедшее время в миллисекундах
  long elapsed_ms = (current_time->tv_sec - last_time->tv_sec) * 1000 +
                    (current_time->tv_usec - last_time->tv_usec) / 1000;

  // Если прошло достаточно времени для автопадения
  if (elapsed_ms >= state->screen.speed && state->status == GAME) {
    move_block(Down);
    gettimeofday(last_time, NULL);  // Сбрасываем таймер
  }
}