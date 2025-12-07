#include "cli.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define INFO_WIDTH 12
#define DISPLAY_HEIGHT 22

// Инициализация цветов
void initColors() {
  start_color();

  // Основной цвет блоков (например, белый на чёрном)
  init_pair(COLOR_BLOCK, COLOR_WHITE, COLOR_BLACK);

  // Тёмно-серый цвет для "призрачных" блоков
  init_pair(COLOR_GHOST, COLOR_BLACK, COLOR_BLACK);

  // Цвет границ
  init_pair(COLOR_BORDER, COLOR_CYAN, COLOR_BLACK);

  // Цвет текста
  init_pair(COLOR_TEXT, COLOR_GREEN, COLOR_BLACK);
}

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, FALSE);
  curs_set(0);

  // Инициализация цветов
  if (has_colors()) initColors();
}

// Функция отрисовки рамки меню
void drawMenuFrame(int start_y, int start_x, int height, int width) {
  // Используем цвет для границ
  if (has_colors()) {
    attron(COLOR_PAIR(COLOR_BORDER));
  }

  // Верхняя граница
  for (int i = 0; i < width; i++) {
    mvprintw(start_y, start_x + i, "*");
  }

  // Боковые границы и внутреннее пространство
  for (int y = 1; y < height - 1; y++) {
    mvprintw(start_y + y, start_x, "*");
    mvprintw(start_y + y, start_x + width - 1, "*");
    for (int x = 1; x < width - 1; x++) {
      mvprintw(start_y + y, start_x + x, " ");
    }
  }

  // Нижняя граница
  for (int i = 0; i < width; i++) {
    mvprintw(start_y + height - 1, start_x + i, "*");
  }

  if (has_colors()) {
    attroff(COLOR_PAIR(COLOR_BORDER));
  }
}

// Функция отрисовки текста меню
void drawMenuText(int start_y, int start_x, int width) {
  if (has_colors()) {
    attron(COLOR_PAIR(COLOR_TEXT));
  }

  char *title = "CHOOSE A GAME";
  int title_x = start_x + (width - strlen(title)) / 2;
  mvprintw(start_y + 5, title_x, "%s", title);

  char *menu_items[] = {"1. TETRIS", "2. ------", "3. ------", "4. ------"};

  int menu_start_y = start_y + 10;
  for (int i = 0; i < 4; i++) {
    int item_x = start_x + (width - strlen(menu_items[i])) / 2;
    mvprintw(menu_start_y + i * 2, item_x, "%s", menu_items[i]);
  }

  if (has_colors()) {
    attroff(COLOR_PAIR(COLOR_TEXT));
  }
}

// Основная функция отрисовки меню
void printMenu() {
  clear();

  int height = 24;
  int width = 58;

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  int start_y = (max_y - height) / 2;
  int start_x = (max_x - width) / 2;

  // Отрисовка рамки меню
  drawMenuFrame(start_y, start_x, height, width);

  // Отрисовка текста меню
  drawMenuText(start_y, start_x, width);

  refresh();
}

// Функция отрисовки отдельной ячейки поля
void drawFieldCell(int value, int draw_y, int draw_x) {
  if (value == 0) return;

  if (value > 0) {
    // Основные блоки
    if (has_colors()) attron(COLOR_PAIR(COLOR_BLOCK));

    mvprintw(draw_y, draw_x, "[]");
    if (has_colors()) attroff(COLOR_PAIR(COLOR_BLOCK));

  } else if (value < 0) {
    // Тёмно-серые блоки (призрачные)
    if (has_colors()) attron(COLOR_PAIR(COLOR_GHOST));

    mvprintw(draw_y, draw_x, "[]");
    if (has_colors()) attroff(COLOR_PAIR(COLOR_GHOST));
  }
}

// Функция отрисовки границ окна
void drawWindowBorders(int start_y, int start_x) {
  int field_window_width = FIELD_WIDTH * 2 + 3;
  int info_window_width = INFO_WIDTH * 2 + 3;

  int field_start_x = start_x + 1;
  int info_start_x = start_x + field_window_width + 1;

  // Включаем цвет для границ
  if (has_colors()) attron(COLOR_PAIR(COLOR_BORDER));

  // Верхние границы
  for (int i = 0; i < field_window_width - 2; i++)
    mvprintw(start_y, field_start_x + i, "*");

  for (int i = 0; i < info_window_width - 2; i++)
    mvprintw(start_y, info_start_x + i, "*");

  // Боковые границы и содержимое
  for (int y = 1; y <= DISPLAY_HEIGHT - 2; y++) {
    // Левое окно - поле
    mvprintw(start_y + y, field_start_x - 1, "*");
    mvprintw(start_y + y, field_start_x + FIELD_WIDTH * 2, "*");

    // Правое окно - информация
    mvprintw(start_y + y, info_start_x - 1, "*");
    mvprintw(start_y + y, info_start_x + INFO_WIDTH * 2 + 1, "*");

    // Заполняем пробелами
    for (int x = 0; x < FIELD_WIDTH * 2; x++)
      mvprintw(start_y + y, field_start_x + x, " ");
    for (int x = 0; x < INFO_WIDTH * 2; x++)
      mvprintw(start_y + y, info_start_x + x, " ");
  }

  // Нижние границы
  for (int i = 0; i < field_window_width - 2; i++)
    mvprintw(start_y + DISPLAY_HEIGHT - 1, field_start_x + i, "*");
  for (int i = 0; i < info_window_width - 2; i++)
    mvprintw(start_y + DISPLAY_HEIGHT - 1, info_start_x + i, "*");

  if (has_colors()) attroff(COLOR_PAIR(COLOR_BORDER));
}

// Функция отрисовки рамки для следующей фигуры
void drawNextFigureBox(GameInfo_t screen, int next_box_y, int next_box_x) {
  if (has_colors()) attron(COLOR_PAIR(COLOR_BORDER));

  // Верхняя граница рамки
  for (int i = 0; i < 12; i++) mvprintw(next_box_y, next_box_x + i, "*");

  // Боковые границы
  for (int i = 1; i <= 4; i++) {
    mvprintw(next_box_y + i, next_box_x, "*");
    mvprintw(next_box_y + i, next_box_x + 11, "*");
    // Заполняем пробелами внутри рамки
    for (int j = 1; j < 11; j++) mvprintw(next_box_y + i, next_box_x + j, " ");
  }
  // Нижняя граница
  for (int i = 0; i < 12; i++) mvprintw(next_box_y + 5, next_box_x + i, "*");

  if (has_colors()) attroff(COLOR_PAIR(COLOR_BORDER));

  // Отрисовка следующей фигуры по центру рамки
  if (screen.next != NULL)
    for (int y = 0; y < 4; y++)
      for (int x = 0; x < 4; x++)
        if (screen.next[y][x]) {
          // Центрируем фигуру в рамке 12x6
          int draw_y = next_box_y + y + 1;
          int draw_x = next_box_x + (12 - 8) / 2 + x * 2;
          if (has_colors()) attron(COLOR_PAIR(COLOR_BLOCK));
          mvprintw(draw_y, draw_x, "[]");
          if (has_colors()) attroff(COLOR_PAIR(COLOR_BLOCK));
        }
}

// Функция отрисовки игрового поля
void renderField(GameInfo_t screen, int start_y, int start_x) {
  int field_start_x = start_x + 1;

  // Отрисовка игрового поля (только 20 строк)
  if (screen.field != NULL)
    for (int y = 0; y < 20; y++)
      for (int x = 0; x < FIELD_WIDTH; x++)
        if (screen.field[y][x] != 0) {
          int cell_value = screen.field[y][x];
          int draw_y = start_y + y + 1;
          int draw_x = field_start_x + x * 2;
          drawFieldCell(cell_value, draw_y, draw_x);
        }
}

// Функция отрисовки следующей фигуры
void renderNextFigure(GameInfo_t screen, int start_y, int info_start_x) {
  if (has_colors()) attron(COLOR_PAIR(COLOR_TEXT));

  // NEXT (следующая фигура)
  char *next_text = "NEXT";
  int next_x = info_start_x + (INFO_WIDTH * 2 - strlen(next_text)) / 2;
  mvprintw(start_y + 1, next_x, "%s", next_text);

  // Рамка для следующей фигуры
  int next_box_y = start_y + 2;
  int next_box_x = info_start_x + (INFO_WIDTH * 2 - 12) / 2;
  drawNextFigureBox(screen, next_box_y, next_box_x);

  if (has_colors()) attroff(COLOR_PAIR(COLOR_TEXT));
}

// Функция отрисовки остальной информации (очки, уровень, пауза и т.д.)
void renderGameInfo(GameInfo_t screen, int start_y, int info_start_x) {
  if (has_colors()) {
    attron(COLOR_PAIR(COLOR_TEXT));
  }

  // SCORE
  char *score_text = "SCORE";
  int score_x = info_start_x + (INFO_WIDTH * 2 - strlen(score_text)) / 2;
  mvprintw(start_y + 9, score_x, "%s", score_text);

  char score_str[8];
  snprintf(score_str, sizeof(score_str), "%07d", screen.score);
  int score_val_x = info_start_x + (INFO_WIDTH * 2 - 7) / 2;
  mvprintw(start_y + 10, score_val_x, "%s", score_str);

  // HIGH SCORE
  char *high_score_text = "HIGH SCORE";
  int high_score_x =
      info_start_x + (INFO_WIDTH * 2 - strlen(high_score_text)) / 2;
  mvprintw(start_y + 12, high_score_x, "%s", high_score_text);

  char high_score_str[8];
  snprintf(high_score_str, sizeof(high_score_str), "%07d", screen.high_score);
  int high_score_val_x = info_start_x + (INFO_WIDTH * 2 - 7) / 2;
  mvprintw(start_y + 13, high_score_val_x, "%s", high_score_str);

  // PAUSE
  char *pause_text = "PAUSE";
  int pause_x = info_start_x + (INFO_WIDTH * 2 - strlen(pause_text)) / 2;
  mvprintw(start_y + 15, pause_x, "%s", pause_text);

  // Отображаем статус паузы
  char *pause_status = screen.pause ? "ON" : "OFF";
  int pause_status_x =
      info_start_x + (INFO_WIDTH * 2 - strlen(pause_status)) / 2;
  mvprintw(start_y + 16, pause_status_x, "%s", pause_status);

  // LEVEL
  char level_str[12];
  snprintf(level_str, sizeof(level_str), "LEVEL %02d", screen.level);
  int level_x = info_start_x + (INFO_WIDTH * 2 - strlen(level_str)) / 2;
  mvprintw(start_y + 18, level_x, "%s", level_str);

  // SPEED
  char speed_str[12];
  snprintf(speed_str, sizeof(speed_str), "SPEED %02d", screen.speed);
  int speed_x = info_start_x + (INFO_WIDTH * 2 - strlen(speed_str)) / 2;
  mvprintw(start_y + 19, speed_x, "%s", speed_str);

  if (has_colors()) attroff(COLOR_PAIR(COLOR_TEXT));
}

// Основная функция отрисовки
void render(GameInfo_t screen) {
  // Определяем размеры окна отрисовки
  int field_window_width = FIELD_WIDTH * 2 + 3;
  int info_window_width = INFO_WIDTH * 2 + 3;
  int total_width = field_window_width + info_window_width + 1;

  // Получаем размеры терминала
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  // Вычисляем начальные координаты для центрирования
  int start_y = (max_y - DISPLAY_HEIGHT) / 2;
  int start_x = (max_x - total_width) / 2;

  // Проверяем, достаточно ли места
  if (start_y < 0) start_y = 0;
  if (start_x < 0) start_x = 0;

  // Отрисовка границ
  drawWindowBorders(start_y, start_x);

  // Вычисляем позиции для полей
  int info_start_x = start_x + field_window_width + 1;

  // Отрисовка игрового поля
  renderField(screen, start_y, start_x);

  // Отрисовка следующей фигуры
  renderNextFigure(screen, start_y, info_start_x);

  // Отрисовка остальной информации
  renderGameInfo(screen, start_y, info_start_x);

  // Обновление экрана
  wnoutrefresh(stdscr);
  doupdate();
}