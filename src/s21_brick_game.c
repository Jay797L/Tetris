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

void startTetris(){
    int ch;
    bool hold = 0;
    UserAction_t input;
    FullGameInfo_t* state = getInfo();
    initInfo();
    while(1) {
      render(updateCurrentState());
      ch = getch();
      input = functionKeys(ch);
      userInput(ch, hold);
        
    }
}

UserAction_t functionKeys(int ch){
  UserAction_t res = 0;
  switch (ch)
  {
  case ('s'):
    res = Start;
    break;
  case (''):
    res = 1;
  break;
  case (''):
    res = 2;
  break;
  case (''):
    res = 3;
  break;
  case (''):
    res = 4;
  break;
  case (''):
    res = 5;
  break;
  case (''):
    res = 6;
  break;
  case (''):
    res = 7;
  break;
  default:
    break;
  }
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
}

GameInfo_t updateCurrentState(){

    //тут должно быть чтота

    FullGameInfo_t* state = getInfo();
    s21_brick_game screen;
    screen.tetris = state->screen;
    return screen.screen;
}
