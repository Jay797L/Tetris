#include "s21_tests.h"

// Тестовые фикстуры
static FullGameInfo_t* state;

void setup(void) {
  state = getInfo();
  initInfo();
  state->status = GAME;
}

void teardown(void) { terminate_game(); }

// --------------------------------------------------
// Тесты для функций из tetris.c
// --------------------------------------------------

START_TEST(test_getInfo_returns_same_instance) {
  FullGameInfo_t* state1 = getInfo();
  FullGameInfo_t* state2 = getInfo();
  ck_assert_ptr_eq(state1, state2);
}
END_TEST

START_TEST(test_initInfo_initializes_fields) {
  initInfo();

  ck_assert_int_eq(state->screen.score, 0);
  ck_assert_int_eq(state->screen.level, 1);
  ck_assert_int_eq(state->screen.speed, 1200);
  ck_assert_int_eq(state->screen.pause, 0);
  ck_assert_int_eq(state->status, START);

  // Проверка выделения памяти для поля
  ck_assert_ptr_nonnull(state->screen.field);
  ck_assert_ptr_nonnull(state->screen.field[0]);
  ck_assert_int_eq(state->screen.field[0][0], 0);

  // Проверка матрицы без блока
  ck_assert_int_eq(state->matrix_without_block[0][0], 0);

  // Проверка массива строк для удаления
  ck_assert_int_eq(state->rows_to_delete[0], 0);
  for (int i = 1; i < 5; i++) {
    ck_assert_int_eq(state->rows_to_delete[i], -1);
  }
}
END_TEST

START_TEST(test_start_game_from_start_state) {
  state->status = START;
  start_game();

  ck_assert_int_eq(state->status, GAME);
  ck_assert_ptr_nonnull(state->block_now.matrix);
  ck_assert_ptr_nonnull(state->block_next.matrix);
}
END_TEST

START_TEST(test_start_game_from_gameover_state) {
  // Подготовка состояния GAMEOVER
  state->status = GAMEOVER;
  state->screen.score = 100;
  state->screen.level = 2;
  state->screen.speed = 1100;

  // Заполняем матрицу данными
  for (int i = 0; i < LENGTH; i++) {
    for (int j = 0; j < WIDTH; j++) {
      state->matrix_without_block[i][j] = 1;
    }
  }

  start_game();

  ck_assert_int_eq(state->status, GAME);
  ck_assert_int_eq(state->screen.score, 0);
  ck_assert_int_eq(state->screen.level, 1);
  ck_assert_int_eq(state->screen.speed, 1200);

  // Проверка очистки матрицы
  ck_assert_int_eq(state->matrix_without_block[0][0], 0);
}
END_TEST

START_TEST(test_pause_game_toggles) {
  state->status = GAME;
  pause_game();
  ck_assert_int_eq(state->status, PAUSE);
  ck_assert_int_eq(state->screen.pause, 1);

  pause_game();
  ck_assert_int_eq(state->status, GAME);
  ck_assert_int_eq(state->screen.pause, 0);
}
END_TEST

// START_TEST(test_terminate_game_cleans_resources) {
//     // Создаем блоки для теста
//     state->status = GAME;
//     create_block(&state->block_now);
//     create_block(&state->block_next);

//     terminate_game();

//     ck_assert_int_eq(state->status, QUIT);
//     ck_assert_ptr_null(state->block_now.matrix);
//     ck_assert_ptr_null(state->block_next.matrix);
// }
// END_TEST

START_TEST(test_move_block_right) {
  state->status = GAME;
  create_block(&state->block_now);
  int initial_y = state->block_now.y;

  move_block(RIGHT);

  ck_assert_int_eq(state->block_now.y, initial_y + 1);
}
END_TEST

START_TEST(test_move_block_left) {
  state->status = GAME;
  create_block(&state->block_now);
  state->block_now.y = 5;
  int initial_y = state->block_now.y;

  move_block(LEFT);

  ck_assert_int_eq(state->block_now.y, initial_y - 1);
}
END_TEST

START_TEST(test_move_block_down_returns_attachment_status) {
  state->status = GAME;
  create_block(&state->block_now);

  // Блок в верхней позиции - не должно быть присоединения
  state->block_now.x = 0;
  int result = move_block(DOWN);
  ck_assert_int_eq(result, 0);

  // Перемещаем блок вниз до конца
  state->block_now.x = LENGTH - state->block_now.rows;
  result = move_block(DOWN);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_full_field_without_collision) {
  state->status = GAME;
  create_block(&state->block_now);

  int result = full_field();
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_kill_string_input) {
  kill_string_input(5);
  ck_assert_int_eq(state->rows_to_delete[1], 5);

  kill_string_input(10);
  ck_assert_int_eq(state->rows_to_delete[2], 10);
}
END_TEST

START_TEST(test_kill_score_input_calculates_correctly) {
  // 1 строка
  state->rows_to_delete[1] = 5;
  kill_score_input();
  ck_assert_int_eq(state->rows_to_delete[0], 100);

  // 2 строки
  state->rows_to_delete[0] = 0;
  state->rows_to_delete[1] = 5;
  state->rows_to_delete[2] = 6;
  kill_score_input();
  ck_assert_int_eq(state->rows_to_delete[0], 300);

  // 4 строки
  state->rows_to_delete[0] = 0;
  state->rows_to_delete[1] = 5;
  state->rows_to_delete[2] = 6;
  state->rows_to_delete[3] = 7;
  state->rows_to_delete[4] = 8;
  kill_score_input();
  ck_assert_int_eq(state->rows_to_delete[0], 1500);
}
END_TEST

START_TEST(test_scan_and_save_score) {
  // Тестируем сохранение и чтение
  save_score(9999);
  int score = scan_score();
  ck_assert_int_eq(score, 9999);

  // Восстанавливаем оригинальное значение
  save_score(0);
}
END_TEST

// --------------------------------------------------
// Тесты для функций из s21_block.c
// --------------------------------------------------

START_TEST(test_create_block_allocates_memory) {
  Block block;
  create_block(&block);

  ck_assert_ptr_nonnull(block.matrix);
  ck_assert_int_ge(block.type, ALPHA);
  ck_assert_int_le(block.type, ZETA);
  ck_assert_int_ge(block.rows, 1);
  ck_assert_int_le(block.rows, 2);
  ck_assert_int_ge(block.columns, 2);
  ck_assert_int_le(block.columns, 4);

  remove_block(&block);
}
END_TEST

// START_TEST(test_gen_block_initializes_correctly) {
//     Block block;
//     block.type = ALPHA;
//     block.rows = 1;
//     block.columns = 4;
//     block.matrix = (int**)calloc(block.rows, sizeof(int*));
//     for (int i = 0; i < block.rows; i++) {
//         block.matrix[i] = (int*)calloc(block.columns, sizeof(int));
//     }

//     gen_block(&block);

//     // Проверка I-образного блока (ALPHA)
//     ck_assert_int_eq(block.matrix[0][0], 1);
//     ck_assert_int_eq(block.matrix[0][1], 1);
//     ck_assert_int_eq(block.matrix[0][2], 1);
//     ck_assert_int_eq(block.matrix[0][3], 1);
//     ck_assert_int_eq(block.y, 3); // (10 - 4) / 2 = 3

//     for (int i = 0; i < block.rows; i++) {
//         free(block.matrix[i]);
//     }
//     free(block.matrix);
// }
// END_TEST

START_TEST(test_remove_block_frees_memory) {
  Block block;
  create_block(&block);

  remove_block(&block);

  ck_assert_ptr_null(block.matrix);
  // Примечание: не можем проверить что память освобождена,
  // но можем проверить что указатель обнулен
}
END_TEST

START_TEST(test_transfer_block_moves_next_to_now) {
  Block now, next;
  create_block(&now);
  create_block(&next);

  // Запоминаем указатель на матрицу next
  int** next_matrix = next.matrix;
  BlockType next_type = next.type;

  transfer_block(&now, &next);

  // Теперь now должен иметь матрицу из next
  ck_assert_ptr_eq(now.matrix, next_matrix);
  ck_assert_int_eq(now.type, next_type);

  // Next должен иметь новую матрицу
  ck_assert_ptr_nonnull(next.matrix);
  ck_assert_ptr_ne(next.matrix, next_matrix);

  remove_block(&now);
  remove_block(&next);
}
END_TEST

START_TEST(test_getBag_returns_same_instance) {
  Bag* bag1 = getBag();
  Bag* bag2 = getBag();
  ck_assert_ptr_eq(bag1, bag2);
}
END_TEST

START_TEST(test_getBlock_returns_valid_type) {
  srand(time(NULL));
  BlockType type = getBlock();

  ck_assert_int_ge(type, ALPHA);
  ck_assert_int_le(type, ZETA);
}
END_TEST

START_TEST(test_nebolshaya_istoricheskaya_spravka) {
  Bag* bag = getBag();

  // Создаем историю
  List* history = NULL;
  create_list(&history, ALPHA);
  history->next = bag->history;
  bag->history = history;

  // Проверяем что ALPHA есть в истории
  short result = nebolshaya_istoricheskaya_spravka(ALPHA);
  ck_assert_int_eq(result, 1);

  // Проверяем что BETA нет в истории
  result = nebolshaya_istoricheskaya_spravka(BETA);
  ck_assert_int_eq(result, 0);

  // Очищаем
  remove_bag();
}
END_TEST

START_TEST(test_rotate_block) {
  state->status = GAME;
  create_block(&state->block_now);
  state->block_now.type = BETA;  // Г-образный блок
  int initial_rows = state->block_now.rows;
  int initial_cols = state->block_now.columns;

  rotate();

  // После поворота размеры должны поменяться местами
  ck_assert_int_eq(state->block_now.rows, initial_cols);
  ck_assert_int_eq(state->block_now.columns, initial_rows);
}
END_TEST

START_TEST(test_rotate_square_block_not_allowed) {
  state->status = GAME;
  create_block(&state->block_now);
  state->block_now.type = DELTA;  // Квадратный блок
  int** original_matrix = state->block_now.matrix;

  rotate();

  // Квадратный блок не должен поворачиваться
  ck_assert_ptr_eq(state->block_now.matrix, original_matrix);
}
END_TEST

START_TEST(test_rotate_block_fail) {
  state->status = GAME;
  create_block(&state->block_now);
  state->block_now.type = BETA;  // Г-образный блок
  int initial_rows = state->block_now.rows;
  int initial_cols = state->block_now.columns;

  for (int i = 0; i < LENGTH; i++)
    for (int j = 0; j < WIDTH; j++) state->matrix_without_block[i][j] = 1;

  rotate();

  // После поворота размеры должны поменяться местами
  ck_assert_int_eq(state->block_now.rows, initial_rows);
  ck_assert_int_eq(state->block_now.columns, initial_cols);
}
END_TEST

START_TEST(test_gen_blocks) {
  Block block;
  create_block_with_type(&block, ALPHA);
  ck_assert_int_eq(block.rows, 1);
  ck_assert_int_eq(block.columns, 4);
  remove_block(&block);

  create_block_with_type(&block, DELTA);
  ck_assert_int_eq(block.rows, 2);
  ck_assert_int_eq(block.columns, 2);
  remove_block(&block);

  create_block_with_type(&block, BETA);
  ck_assert_int_eq(block.rows, 2);
  ck_assert_int_eq(block.columns, 3);
  remove_block(&block);

  create_block_with_type(&block, GAMMA);
  ck_assert_int_eq(block.rows, 2);
  ck_assert_int_eq(block.columns, 3);
  remove_block(&block);

  create_block_with_type(&block, OMEGA);
  ck_assert_int_eq(block.rows, 2);
  ck_assert_int_eq(block.columns, 3);
  remove_block(&block);

  create_block_with_type(&block, PSI);
  ck_assert_int_eq(block.rows, 2);
  ck_assert_int_eq(block.columns, 3);
  remove_block(&block);

  create_block_with_type(&block, ZETA);
  ck_assert_int_eq(block.rows, 2);
  ck_assert_int_eq(block.columns, 3);
  remove_block(&block);
}
END_TEST

Suite* tetris_suite(void) {
  Suite* s = suite_create("Tetris");

  // Core tetris.c tests
  TCase* tc_core = tcase_create("Core");
  tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_getInfo_returns_same_instance);
  tcase_add_test(tc_core, test_initInfo_initializes_fields);
  tcase_add_test(tc_core, test_start_game_from_start_state);
  tcase_add_test(tc_core, test_start_game_from_gameover_state);
  tcase_add_test(tc_core, test_pause_game_toggles);
  // tcase_add_test(tc_core, test_terminate_game_cleans_resources);
  tcase_add_test(tc_core, test_move_block_right);
  tcase_add_test(tc_core, test_move_block_left);
  tcase_add_test(tc_core, test_move_block_down_returns_attachment_status);
  tcase_add_test(tc_core, test_full_field_without_collision);
  tcase_add_test(tc_core, test_kill_string_input);
  tcase_add_test(tc_core, test_kill_score_input_calculates_correctly);
  tcase_add_test(tc_core, test_scan_and_save_score);
  suite_add_tcase(s, tc_core);

  // Block manipulation tests (s21_block.c)
  TCase* tc_blocks = tcase_create("Blocks");
  tcase_add_checked_fixture(tc_blocks, setup, teardown);
  tcase_add_test(tc_blocks, test_create_block_allocates_memory);
  // tcase_add_test(tc_blocks, test_gen_block_initializes_correctly);
  tcase_add_test(tc_blocks, test_remove_block_frees_memory);
  tcase_add_test(tc_blocks, test_transfer_block_moves_next_to_now);
  tcase_add_test(tc_blocks, test_getBag_returns_same_instance);
  tcase_add_test(tc_blocks, test_getBlock_returns_valid_type);
  tcase_add_test(tc_blocks, test_nebolshaya_istoricheskaya_spravka);
  tcase_add_test(tc_blocks, test_rotate_block);
  tcase_add_test(tc_blocks, test_rotate_square_block_not_allowed);
  tcase_add_test(tc_blocks, test_rotate_block_fail);
  tcase_add_test(tc_blocks, test_gen_blocks);
  suite_add_tcase(s, tc_blocks);

  return s;
}

int main(void) {
  int number_failed;
  Suite* s = tetris_suite();
  SRunner* sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}