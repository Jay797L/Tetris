CC = gcc
FLAGS = -Wall -Wextra -g -Werror
SOURCES = s21_$(PROJECT_NAME).c
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
	$(CC) $(FLAGS) -c $(SOURCES)

test: clean s21_matrix.a
	$(CC) $(FLAGS) $(TEST) s21_matrix.a -o test $(CHECK_LIBS)
	./test

gcov_report: clean
	$(CC) $(FLAGS) $(GCOV_FLAGS) $(SOURCES) $(TEST) -o gcov.out $(CHECK_LIBS)
	./gcov.out
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	open ./coverage_report/index.html

clean:
	rm -rf *.dSYM *.a test *.gcda *.gcno coverage.info coverage_report s21_matrix.info gcov.out report
	rm -rf *.o

clang: clean
	echo "---\nBasedOnStyle: Google\n" > .clang-format
	clang-format -n *.c *.h */*/*.c */*/*.h
	clang-format -i *.c *.h */*/*.c */*/*.h
	rm -rf .clang-format

leaks: test
	CK_FORK=no leaks -atExit -- ./test