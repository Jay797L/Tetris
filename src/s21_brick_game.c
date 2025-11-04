#include "s21_brick_game.h"

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