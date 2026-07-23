CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11
COVERAGE_FLAGS = --coverage

MATH_LIB = -lm
CHECK_LIBS = -lcheck -lpthread
ifeq ($(shell uname),Linux)
    CHECK_LIBS += -lsubunit
endif

SRC = $(wildcard calc_*.c)
LIBS = $(SRC:.c=.so)

MANUAL_TEST_SRC = main.c
MANUAL_TEST_TARGET = man_test_exe

TEST_SRC = $(wildcard test/*.c)
TEST_TARGET = tests_exe

all: $(LIBS)

%.so: %.c
	@$(CC) $(CFLAGS) -shared -o $@ $< $(MATH_LIB)

manual_testing: $(LIBS)
	@$(CC) $(CFLAGS) -rdynamic -o $(MANUAL_TEST_TARGET) $(MANUAL_TEST_SRC) -ldl
	./$(MANUAL_TEST_TARGET)

test: CFLAGS += $(COVERAGE_FLAGS)
test: $(LIBS)
	@rm -rf */*.gcda */*.info *.gcda *.info 
	@$(CC) $(CFLAGS) -rdynamic -o $(TEST_TARGET) $(TEST_SRC) $(CHECK_LIBS) $(MATH_LIB) -ldl
	./$(TEST_TARGET)

gcov_report:
	lcov --directory . --capture --output-file coverage.info --rc branch_coverage=1 --exclude="*/test/*"
	genhtml coverage.info --output-directory report --branch-coverage

format:
	$(CLANG_FORMAT) -i *.h $(SRC) $(TEST_SRC) $(MANUAL_TEST_SRC)

check_styles:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind: test
	valgrind --tool=memcheck --leak-check=yes --quiet ./$(TEST_TARGET)

clean:
	@rm -rf $(OBJ) \
	        $(LIBS) \
			$(TEST_TARGET) \
			$(MANUAL_TEST_TARGET) \
			*.gcno \
			*.gcda \
			*.info \
			report \
			*.o

.PHONY: all \
		manual_testing \
		clean \
		test \
		gcov_report \
		format \
		check_styles \
		valgrind
