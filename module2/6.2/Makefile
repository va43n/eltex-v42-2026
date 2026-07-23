CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11
COVERAGE_FLAGS = --coverage

CHECK_LIBS = -lcheck -lm -lpthread
ifeq ($(shell uname),Linux)
    CHECK_LIBS += -lsubunit
endif

SRC = phone_book.c phone_book_additional.c
OBJ = $(SRC:.c=.o)
LIB_NAME = libphone_book.so
SHORT_LIB_NAME = -lphone_book

MANUAL_TEST_SRC = main.c
MANUAL_TEST_TARGET = man_test_exe

TEST_SRC = $(wildcard test/*.c)
TEST_TARGET = tests_exe

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	@$(CC) -shared -fPIC $^ -o $@

%.o: %.c
	@$(CC) -c -fPIC $(CFLAGS) $<

manual_testing: $(LIB_NAME)
	@$(CC) $(CFLAGS) $(MANUAL_TEST_SRC) -o $(MANUAL_TEST_TARGET) -L. $(SHORT_LIB_NAME) -Wl,-rpath,$(PWD)
	./$(MANUAL_TEST_TARGET)

test: $(LIB_NAME)
	@rm -rf */*.gcda */*.info *.gcda *.info 
	@$(CC) $(CFLAGS) $(TEST_SRC) $(CHECK_LIBS) -o $(TEST_TARGET) -L. $(SHORT_LIB_NAME) -Wl,-rpath,$(PWD)
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
			$(LIB_NAME) \
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
