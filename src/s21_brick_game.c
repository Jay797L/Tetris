#include "s21_brick_game.h"


int main(){
    initNcurses();
    int ch;
    while(1) {
        printMenu();
        ch = getch();
        switch(ch) {
            case '1':
                tetris();
                break;
            case 'q':
            case 'Q':
                endwin();
                return 0;
            default:
                break;
        }
    }
}

void tetris(){
    int ch;
    bool hold = 0;
    FullGameInfo_t* state = getInfo();
    initInfo();
    while(state->status != QUIT) {
      render(updateCurrentState());
      ch = getch();
      userInput(functionKeys(ch), hold);
    }
}

UserAction_t functionKeys(int ch){
  UserAction_t res = 0;
  switch (ch)
  {
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
    res = Up;
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
    res = Action;
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
  if (hold == 1) hold = 0;
}

GameInfo_t updateCurrentState(){

    //тут должно быть чтота

    FullGameInfo_t* state = getInfo();
    s21_brick_game screen;
    screen.tetris = state->screen;
    return screen.screen;
}