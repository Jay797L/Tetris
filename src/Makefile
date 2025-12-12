CC = gcc
FLAGS = -Wall -Wextra -g -Werror
PROJECT_NAME = brick_game
BACKEND = brick_game/tetris/tetris.c brick_game/tetris/s21_block.c
FRONTEND = gui/cli/cli.c
SOURCES = s21_$(PROJECT_NAME).c s21_$(PROJECT_NAME)_functions.c $(BACKEND) $(FRONTEND) 
HEADER = s21_$(PROJECT_NAME).h 
RESULT = s21_$(PROJECT_NAME).o
LIB = s21_backend.a
TEST = tests/s21_tests.c
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CHECK_LIBS = -L/usr/local/Cellar/check/0.15.2/lib -lcheck

# Установочные пути
PREFIX = /usr/local
DESTDIR = 

all: $(RESULT)

$(LIB): $(BACKEND) clean
	$(CC) $(FLAGS) -c $(BACKEND)
	ar rcs $(LIB) *.o
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

install: $(RESULT)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp tetris $(DESTDIR)$(PREFIX)/bin/
	chmod 755 $(DESTDIR)$(PREFIX)/bin/tetris
	@echo "Tetris installed to $(DESTDIR)$(PREFIX)/bin/tetris"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/tetris
	@echo "Tetris uninstalled from $(DESTDIR)$(PREFIX)/bin"

# dvi - отображает документацию
dvi:
	cat dvi/README.md

# dist - создает архив с исходниками
dist: clean
	mkdir -p $(PROJECT_NAME)-1.0
	cp -r *.c *.h */*.c */*.h */*/*.c */*/*.h Makefile README* dvi/ materials/ $(PROJECT_NAME)-1.0/ 2>/dev/null || true
	tar -czf $(PROJECT_NAME)-1.0.tar.gz $(PROJECT_NAME)-1.0/
	rm -rf $(PROJECT_NAME)-1.0
	@echo "Distribution package created: $(PROJECT_NAME)-1.0.tar.gz"

clean:
	rm -rf *.dSYM *.a test *.gcda *.gcno coverage.info coverage_report s21_matrix.info gcov.out report
	rm -rf *.o tetris test record.txt
	rm -rf $(PROJECT_NAME)-1.0 $(PROJECT_NAME)-1.0.tar.gz

clang: clean
	echo "---\nBasedOnStyle: Google\n" > .clang-format
	clang-format -n *.c *.h */*.c */*.h */*/*.c */*/*.h
	clang-format -i *.c *.h */*.c */*.h */*/*.c */*/*.h
	rm -rf .clang-format

leaks: test
	CK_FORK=no leaks -atExit -- ./test

run: $(RESULT)
	./tetris

.PHONY: all test gcov_report install uninstall dvi dist clean clang leaks run