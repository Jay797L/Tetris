CC = gcc
FLAGS = -Wall -Wextra -g -Werror
PROJECT_NAME = brick_game
SOURCES = s21_$(PROJECT_NAME).c brick_game/tetris/tetris.c brick_game/tetris/s21_block.c gui/cli/cli.c
HEADER = s21_$(PROJECT_NAME).h 
RESULT = s21_$(PROJECT_NAME).o
RESULT_LIB = s21_$(PROJECT_NAME).a
TEST = tests/s21_tests.c
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CHECK_LIBS = -L/usr/local/Cellar/check/0.15.2/lib -lcheck

all: $(RESULT)

$(RESULT_LIB): $(SOURCES) $(HEADER) clean
	$(CC) $(FLAGS) -c $(SOURCES)
	ar rcs s21_matrix.a *.o
	@rm *.o

$(RESULT): $(SOURCES) $(HEADER) clean
	$(CC) $(FLAGS) -o tetris $(SOURCES) -lncurses

test: clean $(RESULT)
	$(CC) $(FLAGS) $(TEST) brick_game/tetris/tetris.c brick_game/tetris/s21_block.c gui/cli/cli.c -o test $(CHECK_LIBS) -lncurses
	./test

gcov_report: clean
	$(CC) $(FLAGS) $(GCOV_FLAGS) $(SOURCES) $(TEST) -o gcov.out $(CHECK_LIBS)
	./gcov.out
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	open ./coverage_report/index.html

clean:
	rm -rf *.dSYM *.a test *.gcda *.gcno coverage.info coverage_report s21_matrix.info gcov.out report
	rm -rf *.o tetris record.txt

clang: clean
	echo "---\nBasedOnStyle: Google\n" > .clang-format
	clang-format -n *.c *.h */*/*.c */*/*.h
	clang-format -i *.c *.h */*/*.c */*/*.h
	rm -rf .clang-format

leaks: test
	CK_FORK=no leaks -atExit -- ./test

run: $(RESULT)
	./tetris