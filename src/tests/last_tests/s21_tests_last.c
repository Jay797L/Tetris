#include "s21_tests_last.h"

int* get_i() {
  static int i = -1;
  return &i;
}

int get_char() {
  int* i = get_i();
  int chars[1000] = {
      '1',       's',       'p',       KEY_UP,    KEY_LEFT,  's',
      ' ',       'p',       KEY_UP,    KEY_LEFT,  KEY_RIGHT, KEY_RIGHT,
      KEY_RIGHT, KEY_RIGHT, KEY_RIGHT, KEY_UP,    KEY_DOWN,  ' ',
      KEY_LEFT,  ' ',       KEY_RIGHT, KEY_RIGHT, ' ',       KEY_LEFT,
      KEY_LEFT,  KEY_LEFT,  ' ',       KEY_UP,    ' ',       KEY_UP,
      KEY_RIGHT, KEY_RIGHT, KEY_RIGHT, KEY_RIGHT, ' ',       KEY_UP,
      KEY_UP,    KEY_UP,    KEY_LEFT,  KEY_LEFT,  KEY_LEFT,  ' ',
      KEY_UP,    KEY_LEFT,  KEY_LEFT,  ' ',       KEY_RIGHT, KEY_RIGHT,
      KEY_RIGHT, ' ',       KEY_RIGHT, KEY_RIGHT, KEY_UP,    KEY_UP,
      ' ',       KEY_UP,    KEY_UP,    KEY_UP,    ' ',       KEY_LEFT,
      KEY_LEFT,  KEY_LEFT,  KEY_LEFT,  KEY_LEFT,  KEY_LEFT,  KEY_UP,
      KEY_LEFT,  ' ',       'p',       'p',       ' ',       ' ',
      ' ',       ' ',       ' ',       ' ',       ' ',       ' ',
      ' ',       ' ',       ' ',       ' ',       ' ',       ' ',
      's',       ' ',       'q',       '1',       's',       ' ',
      ' ',       ' ',       'q',       'q'};
  *i = *i + 1;
  return chars[*i];
}

START_TEST(tetris_test) {
  int ch;
  bool hold = 0;
  FullGameInfo_t* state = getInfo();
  initInfo();
  struct timeval last_time, current_time;
  gettimeofday(&last_time, NULL);
  while (state->status != QUIT) {
    // ck_assert(test(*get_i()));

    auto_fall(&last_time, &current_time);
    render(updateCurrentState());
    ch = get_char();
    userInput(functionKeys(ch), hold);
    nodelay(stdscr, state->status == GAME);
    usleep(500000);
  }
  ck_assert(state->status == QUIT);
}
END_TEST

Suite* tetris_suite(void) {
  Suite* s = suite_create("Tetris");

  TCase* tc_core = tcase_create("Core");
  tcase_add_test(tc_core, tetris_test);
  suite_add_tcase(s, tc_core);
  return s;
}

int main() {
  srand(0);
  initNcurses();
  int ch;
  int number_failed = 0;
  Suite* s = tetris_suite();
  SRunner* sr = srunner_create(s);

  while (1) {
    printMenu();
    ch = get_char();
    switch (ch) {
      case '1':
        clear();
        srunner_run_all(sr, CK_NORMAL);
        number_failed += srunner_ntests_failed(sr);
        srunner_free(sr);
        break;
      case 'q':
      case 'Q':
        endwin();
        return 0;
      default:
        break;
    }
  }

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// int test(int i){
//   int res = 0;
//   if(i >= 0 && i < 25) res = tests_0_24(i);
//   if(i >= 25 && i < 50) res = tests_25_49(i);
//   if(i >= 50 && i < 75) res = tests_50_74(i);
//   return res;
// }

// int tests_0_24(int i){
//   int res = 0;
//   if(i == 0) res = test_0();
//   if(i == 1) res = test_1();
//   if(i == 2) res = test_2();
//   if(i == 3) res = test_3();
//   if(i == 4) res = test_4();
//   if(i == 5) res = test_5();
//   if(i == 6) res = test_6();
//   if(i == 7) res = test_7();
//   if(i == 8) res = test_8();
//   if(i == 9) res = test_9();
//   if(i == 10) res = test_10();
//   if(i == 11) res = test_11();
//   if(i == 12) res = test_12();
//   if(i == 13) res = test_13();
//   if(i == 14) res = test_14();
//   if(i == 15) res = test_15();
//   if(i == 16) res = test_16();
//   if(i == 17) res = test_17();
//   if(i == 18) res = test_18();
//   if(i == 19) res = test_19();
//   if(i == 20) res = test_20();
//   if(i == 21) res = test_21();
//   if(i == 22) res = test_22();
//   if(i == 23) res = test_23();
//   if(i == 24) res = test_24();
// }

// int tests_25_49(int i){
//   int res = 0;
//   if(i == 25) res = test_25();
//   if(i == 26) res = test_26();
//   if(i == 27) res = test_27();
//   if(i == 28) res = test_28();
//   if(i == 29) res = test_29();
//   if(i == 30) res = test_30();
//   if(i == 31) res = test_31();
//   if(i == 32) res = test_32();
//   if(i == 33) res = test_33();
//   if(i == 34) res = test_34();
//   if(i == 35) res = test_35();
//   if(i == 36) res = test_36();
//   if(i == 37) res = test_37();
//   if(i == 38) res = test_38();
//   if(i == 39) res = test_39();
//   if(i == 40) res = test_40();
//   if(i == 41) res = test_41();
//   if(i == 42) res = test_42();
//   if(i == 43) res = test_43();
//   if(i == 44) res = test_44();
//   if(i == 45) res = test_45();
//   if(i == 46) res = test_46();
//   if(i == 47) res = test_47();
//   if(i == 48) res = test_48();
//   if(i == 49) res = test_49();
// }

// int tests_50_74(int i){
//   int res = 0;
//   if(i == 50) res = test_50();
//   if(i == 51) res = test_51();
//   if(i == 52) res = test_52();
//   if(i == 53) res = test_53();
//   if(i == 54) res = test_54();
//   if(i == 55) res = test_55();
//   if(i == 56) res = test_56();
//   if(i == 57) res = test_57();
//   if(i == 58) res = test_58();
//   if(i == 59) res = test_59();
//   if(i == 60) res = test_60();
//   if(i == 61) res = test_61();
//   if(i == 62) res = test_62();
//   if(i == 63) res = test_63();
//   if(i == 64) res = test_64();
//   if(i == 65) res = test_65();
//   if(i == 66) res = test_66();
//   if(i == 67) res = test_67();
//   if(i == 68) res = test_68();
//   if(i == 69) res = test_69();
//   if(i == 70) res = test_70();
//   if(i == 71) res = test_71();
//   if(i == 72) res = test_72();
//   if(i == 73) res = test_73();
//   if(i == 74) res = test_74();
// }