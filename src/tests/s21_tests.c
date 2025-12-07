#include "s21_tests.h"

// =================== Тесты для tetris.c ===================

START_TEST(test_getInfo) {
    FullGameInfo_t* state1 = getInfo();
    ck_assert_ptr_nonnull(state1);
    
    FullGameInfo_t* state2 = getInfo();
    ck_assert_ptr_eq(state1, state2); // Должен возвращать один и тот же указатель
    
    // Проверяем инициализацию
    ck_assert_int_eq(state1->screen.high_score, 0);
    // status может быть разным, не проверяем
}
END_TEST

START_TEST(test_initInfo) {
    FullGameInfo_t* state = getInfo();
    initInfo();
    
    // Проверяем выделение памяти
    ck_assert_ptr_nonnull(state->screen.field);
    ck_assert_ptr_nonnull(state->screen.next);
    
    // Проверяем инициализацию значений
    ck_assert_int_eq(state->screen.score, 0);
    ck_assert_int_eq(state->screen.level, 1);
    ck_assert_int_eq(state->screen.speed, 1200);
    ck_assert_int_eq(state->screen.pause, 0);
    ck_assert_int_eq(state->status, START);
    
    // Проверяем matrix_without_block
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            ck_assert_int_eq(state->matrix_without_block[i][j], 0);
        }
    }
    
    // Проверяем rows_to_delete
    ck_assert_int_eq(state->rows_to_delete[0], 0);
    for (int i = 1; i < 5; i++) {
        ck_assert_int_eq(state->rows_to_delete[i], -1);
    }
}
END_TEST

START_TEST(test_start_game) {
    FullGameInfo_t* state = getInfo();
    
    // Тестируем переход из START в GAME
    state->status = START;
    state->screen.score = 0;
    start_game();
    ck_assert_int_eq(state->status, GAME);
    ck_assert_ptr_nonnull(state->block_now.matrix);
    ck_assert_ptr_nonnull(state->block_next.matrix);
    
    // Тестируем переход из GAMEOVER в GAME
    state->status = GAMEOVER;
    state->screen.score = 100;
    state->screen.level = 5;
    state->screen.speed = 800;
    start_game();
    ck_assert_int_eq(state->status, GAME);
    ck_assert_int_eq(state->screen.score, 0);
    ck_assert_int_eq(state->screen.level, 1);
    ck_assert_int_eq(state->screen.speed, 1200);
    ck_assert_ptr_nonnull(state->block_now.matrix);
    ck_assert_ptr_nonnull(state->block_next.matrix);
    
    // Очищаем
    remove_block(&state->block_now);
    remove_block(&state->block_next);
}
END_TEST

START_TEST(test_pause_game) {
    FullGameInfo_t* state = getInfo();
    
    // GAME -> PAUSE
    state->status = GAME;
    state->screen.pause = 0;
    pause_game();
    ck_assert_int_eq(state->status, PAUSE);
    ck_assert_int_eq(state->screen.pause, 1);
    
    // PAUSE -> GAME
    pause_game();
    ck_assert_int_eq(state->status, GAME);
    ck_assert_int_eq(state->screen.pause, 0);
    
    // START не должен меняться
    state->status = START;
    state->screen.pause = 0;
    pause_game();
    ck_assert_int_eq(state->status, START);
    ck_assert_int_eq(state->screen.pause, 0);
}
END_TEST

START_TEST(test_rotate) {
    FullGameInfo_t* state = getInfo();
    
    // Настраиваем состояние
    state->status = GAME;
    initInfo(); // Инициализируем поле
    create_block(&state->block_now);
    
    // Запоминаем исходное состояние
    int old_rows = state->block_now.rows;
    int old_cols = state->block_now.columns;
    
    // Тестируем вращение
    rotate();
    
    // Проверяем изменения (кроме квадрата)
    if (state->block_now.type != DELTA) {
        ck_assert(state->block_now.rows == old_cols);
        ck_assert(state->block_now.columns == old_rows);
    }
    
    // Проверяем граничные случаи
    state->block_now.y = WIDTH - 2;
    rotate();
    ck_assert(state->block_now.y >= 0);
    ck_assert(state->block_now.y + state->block_now.columns <= WIDTH);
    
    remove_block(&state->block_now);
}
END_TEST

START_TEST(test_move_block) {
    FullGameInfo_t* state = getInfo();
    
    // Настраиваем состояние
    state->status = GAME;
    initInfo();
    create_block(&state->block_now);
    
    int initial_x = state->block_now.x;
    int initial_y = state->block_now.y;
    
    // Тестируем движение вправо
    int res = move_block(RIGHT);
    ck_assert_int_eq(res, 0);
    if (state->block_now.y + state->block_now.columns < WIDTH) {
        ck_assert_int_eq(state->block_now.y, initial_y + 1);
    }
    
    // Тестируем движение влево
    move_block(LEFT);
    ck_assert_int_eq(state->block_now.y, initial_y);
    
    // Тестируем движение вниз (без прикрепления)
    res = move_block(DOWN);
    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(state->block_now.x, initial_x + 1);
    
    remove_block(&state->block_now);
}
END_TEST

START_TEST(test_check_input) {
    
    // Создаем тестовый блок
    Block block;
    create_block(&block);
    
    // Проверяем пустое поле
    int res = check_input(block);
    ck_assert_int_eq(res, 0);
    
    // Помещаем блок в конец поля
    block.x = LENGTH - block.rows;
    res = check_input(block);
    ck_assert_int_eq(res, 0);
    
    // Пытаемся выйти за границы
    block.x = LENGTH - block.rows + 1;
    res = check_input(block);
    ck_assert_int_eq(res, 1);
    
    // Проверяем выход за левую границу
    block.x = 0;
    block.y = -1;
    res = check_input(block);
    ck_assert_int_eq(res, 1);
    
    // Проверяем выход за правую границу
    block.y = WIDTH;
    res = check_input(block);
    ck_assert_int_eq(res, 1);
    
    remove_block(&block);
}
END_TEST

START_TEST(test_full_field) {
    FullGameInfo_t* state = getInfo();
    initInfo();
    state->status = GAME;
    
    // Создаем блок
    create_block(&state->block_now);
    
    // Проверяем на пустом поле
    int res = full_field();
    ck_assert_int_eq(res, 0);
    
    // Проверяем, что поле обновилось
    ck_assert_ptr_nonnull(state->screen.field);
    
    remove_block(&state->block_now);
}
END_TEST

START_TEST(test_attachment) {
    FullGameInfo_t* state = getInfo();
    initInfo();
    state->status = GAME;
    
    // Инициализируем блоки
    create_block(&state->block_now);
    create_block(&state->block_next);
    
    // Устанавливаем блок в низ поля
    state->block_now.x = LENGTH - state->block_now.rows;
    
    // Вызываем прикрепление
    attachment();
    
    // Проверяем, что появился следующий блок
    ck_assert_ptr_nonnull(state->block_now.matrix);
    ck_assert_ptr_nonnull(state->block_next.matrix);
    
    // Очищаем
    remove_block(&state->block_now);
    remove_block(&state->block_next);
}
END_TEST

START_TEST(test_kill_score_input) {
    FullGameInfo_t* state = getInfo();
    
    // Тестируем разные комбинации заполненных строк
    state->rows_to_delete[0] = 0;
    
    // 1 строка
    state->rows_to_delete[1] = 5;
    state->rows_to_delete[2] = -1;
    state->rows_to_delete[3] = -1;
    state->rows_to_delete[4] = -1;
    kill_score_input();
    ck_assert_int_eq(state->rows_to_delete[0], 100);
    
    // 2 строки
    state->rows_to_delete[0] = 0;
    state->rows_to_delete[1] = 5;
    state->rows_to_delete[2] = 6;
    state->rows_to_delete[3] = -1;
    state->rows_to_delete[4] = -1;
    kill_score_input();
    ck_assert_int_eq(state->rows_to_delete[0], 300);
    
    // 3 строки
    state->rows_to_delete[0] = 0;
    state->rows_to_delete[1] = 5;
    state->rows_to_delete[2] = 6;
    state->rows_to_delete[3] = 7;
    state->rows_to_delete[4] = -1;
    kill_score_input();
    ck_assert_int_eq(state->rows_to_delete[0], 700);
    
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

START_TEST(test_scan_save_score) {
    // Сохраняем оригинальный счет
    FILE* file = fopen("record.txt", "r");
    int original_score = 0;
    if (file) {
        fscanf(file, "%d", &original_score);
        fclose(file);
    }
    
    // Сохраняем тестовый счет
    int test_score = 1234;
    save_score(test_score);
    
    // Считываем и проверяем
    int loaded_score = scan_score();
    ck_assert_int_eq(loaded_score, test_score);
    
    // Тестируем с нулевым счетом
    save_score(0);
    loaded_score = scan_score();
    ck_assert_int_eq(loaded_score, 0);
    
    // Восстанавливаем оригинальный счет
    save_score(original_score);
}
END_TEST

// =================== Тесты для s21_block.c ===================

START_TEST(test_create_block) {
    Block block;
    create_block(&block);
    
    ck_assert_ptr_nonnull(block.matrix);
    ck_assert_int_ge(block.type, ALPHA);
    ck_assert_int_le(block.type, ZETA);
    
    // Проверяем размеры в зависимости от типа
    switch (block.type) {
        case ALPHA:
            ck_assert_int_eq(block.rows, 1);
            ck_assert_int_eq(block.columns, 4);
            break;
        case DELTA:
            ck_assert_int_eq(block.rows, 2);
            ck_assert_int_eq(block.columns, 2);
            break;
        default:
            ck_assert_int_eq(block.rows, 2);
            ck_assert_int_eq(block.columns, 3);
            break;
    }
    
    remove_block(&block);
}
END_TEST

START_TEST(test_gen_block) {
    Block block;
    block.type = ALPHA;
    block.rows = 1;
    block.columns = 4;
    block.matrix = (int**)calloc(block.rows, sizeof(int*));
    for (int i = 0; i < block.rows; i++)
        block.matrix[i] = (int*)calloc(block.columns, sizeof(int));
    
    gen_block(&block);
    
    // Проверяем позицию
    ck_assert_int_eq(block.x, 0);
    ck_assert_int_eq(block.y, (10 - block.rows) / 2);
    
    // Проверяем заполнение матрицы
    for (int j = 0; j < block.columns; j++) {
        ck_assert_int_eq(block.matrix[0][j], 1);
    }
    
    for (int i = 0; i < block.rows; i++) free(block.matrix[i]);
    free(block.matrix);
}
END_TEST

START_TEST(test_remove_block) {
    Block block;
    create_block(&block);
    
    ck_assert_ptr_nonnull(block.matrix);
    remove_block(&block);
    ck_assert_ptr_null(block.matrix);
}
END_TEST

START_TEST(test_transfer_block) {
    Block now, next;
    create_block(&now);
    create_block(&next);
    
    // Запоминаем указатель на матрицу next
    int** next_matrix = next.matrix;
    BlockType next_type = next.type;
    
    transfer_block(&now, &next);
    
    // Проверяем, что now получил матрицу next
    ck_assert_int_eq(now.type, next_type);
    ck_assert_ptr_nonnull(now.matrix);
    
    // Проверяем, что next создал новую матрицу
    ck_assert_ptr_nonnull(next.matrix);
    ck_assert_ptr_ne(next.matrix, next_matrix);
    
    remove_block(&now);
    remove_block(&next);
}
END_TEST

START_TEST(test_transpose_block) {
    Block now, buffer;
    
    // Создаем L-образный блок (2x3)
    now.type = BETA;
    now.rows = 2;
    now.columns = 3;
    now.matrix = (int**)calloc(now.rows, sizeof(int*));
    for (int i = 0; i < now.rows; i++)
        now.matrix[i] = (int*)calloc(now.columns, sizeof(int));
    
    // Заполняем стандартную форму
    now.matrix[0][0] = 1;
    now.matrix[1][0] = 1;
    now.matrix[1][1] = 1;
    now.matrix[1][2] = 1;
    
    transpose_block(&now, &buffer);
    
    // Проверяем размеры после транспонирования
    ck_assert_int_eq(now.rows, buffer.columns); // 3
    ck_assert_int_eq(now.columns, buffer.rows); // 2
    
    // Проверяем, что матрица создана
    ck_assert_ptr_nonnull(now.matrix);
    
    // Освобождаем память
    for (int i = 0; i < buffer.rows; i++) 
        if (buffer.matrix[i]) free(buffer.matrix[i]);
    if (buffer.matrix) free(buffer.matrix);
    remove_block(&now);
}
END_TEST

START_TEST(test_getBag) {
    Bag* bag1 = getBag();
    Bag* bag2 = getBag();
    
    ck_assert_ptr_nonnull(bag1);
    ck_assert_ptr_eq(bag1, bag2); // Singleton pattern
    
    // Проверяем инициализацию bag
    for (int i = 0; i < 35; i++) {
        ck_assert_int_ge(bag1->bag[i], ALPHA);
        ck_assert_int_le(bag1->bag[i], ZETA);
        // Для первых 35 элементов должно быть 0-6 (по 5 каждого типа)
        ck_assert_int_eq(bag1->bag[i], i / 5);
    }
}
END_TEST

START_TEST(test_getBlock) {
    // Тестируем несколько раз, чтобы проверить генерацию
    int counts[7] = {0};
    for (int i = 0; i < 50; i++) {
        BlockType type = getBlock();
        ck_assert_int_ge(type, ALPHA);
        ck_assert_int_le(type, ZETA);
        counts[type]++;
    }
    
    // Проверяем, что все типы встречаются
    int total_types = 0;
    for (int i = 0; i < 7; i++) {
        if (counts[i] > 0) total_types++;
    }
    ck_assert_int_ge(total_types, 5); // Должны быть все или почти все типы
}
END_TEST

START_TEST(test_updateOrder) {
    Bag* bag = getBag();
    
    // Сохраняем исходное состояние
    BlockType original_bag[35];
    memcpy(original_bag, bag->bag, sizeof(original_bag));
    
    // Обновляем порядок для первого элемента
    updateOrder(0);
    
    // Проверяем, что элемент переместился в конец
    ck_assert_int_eq(bag->bag[34], original_bag[0]);
    
    // Проверяем, что другие элементы сдвинулись
    for (int i = 0; i < 34; i++) {
        ck_assert_int_eq(bag->bag[i], original_bag[i+1]);
    }
}
END_TEST

// =================== Suite создания ===================

Suite* tetris_suite(void) {
    Suite* s = suite_create("Tetris");
    
    // Добавляем тесты для tetris.c
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_getInfo);
    tcase_add_test(tc_core, test_initInfo);
    tcase_add_test(tc_core, test_start_game);
    tcase_add_test(tc_core, test_pause_game);
    suite_add_tcase(s, tc_core);
    
    TCase* tc_gameplay = tcase_create("Gameplay");
    tcase_add_test(tc_gameplay, test_rotate);
    tcase_add_test(tc_gameplay, test_move_block);
    tcase_add_test(tc_gameplay, test_check_input);
    tcase_add_test(tc_gameplay, test_full_field);
    tcase_add_test(tc_gameplay, test_attachment);
    tcase_add_test(tc_gameplay, test_kill_score_input);
    suite_add_tcase(s, tc_gameplay);
    
    TCase* tc_score = tcase_create("Score");
    tcase_add_test(tc_score, test_scan_save_score);
    suite_add_tcase(s, tc_score);
    
    // Добавляем тесты для s21_block.c
    TCase* tc_blocks = tcase_create("Blocks");
    tcase_add_test(tc_blocks, test_create_block);
    tcase_add_test(tc_blocks, test_gen_block);
    tcase_add_test(tc_blocks, test_remove_block);
    tcase_add_test(tc_blocks, test_transfer_block);
    tcase_add_test(tc_blocks, test_transpose_block);
    tcase_add_test(tc_blocks, test_getBag);
    tcase_add_test(tc_blocks, test_getBlock);
    tcase_add_test(tc_blocks, test_updateOrder);
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