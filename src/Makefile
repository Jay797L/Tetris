CC = gcc
FLAGS = -Wall -Wextra -g -Werror
PROJECT_NAME = brick_game
BACKEND = brick_game/tetris/tetris.c brick_game/tetris/s21_block.c
FRONTEND = gui/cli/cli.c
SOURCES = s21_$(PROJECT_NAME).c s21_$(PROJECT_NAME)_functions.c $(BACKEND) $(FRONTEND) 
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
	$(CC) $(FLAGS) $(SOURCES) -o tetris -lncurses

test: clean
	$(CC) $(FLAGS) $(TEST) $(BACKEND) $(CHECK_LIBS) -o test
	./test

gcov_report: clean
	$(CC) $(FLAGS) $(TEST) $(BACKEND) -o test $(CHECK_LIBS) $(GCOV_FLAGS) -o gcov.out -lncurses
	./gcov.out
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	open ./coverage_report/index.html

clean:
	rm -rf *.dSYM *.a test *.gcda *.gcno coverage.info coverage_report s21_matrix.info gcov.out report
	rm -rf *.o tetris test record.txt

clang: clean
	echo "---\nBasedOnStyle: Google\n" > .clang-format
	clang-format -n *.c *.h */*.c */*.h */*/*.c */*/*.h
	clang-format -i *.c *.h */*.c */*.h */*/*.c */*/*.h
	rm -rf .clang-format

leaks: test
	CK_FORK=no leaks -atExit -- ./test

run: $(RESULT)
	./tetris