CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11
COVERAGE_FLAGS = --coverage

CHECK_LIBS = -lcheck -lm -lpthread
ifeq ($(shell uname),Linux)
    CHECK_LIBS += -lsubunit
endif

SRC = prepare.c
OBJ = $(SRC:.c=.o)

MANUAL_TEST_SRC = main.c
MANUAL_TEST_TARGET = man_test_exe

all: manual_testing

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

manual_testing: $(OBJ)
	@$(CC) $(CFLAGS) $(MANUAL_TEST_SRC) $(OBJ) -o $(MANUAL_TEST_TARGET)
	./$(MANUAL_TEST_TARGET)

format:
	$(CLANG_FORMAT) -i *.h $(SRC) $(TEST_SRC) $(MANUAL_TEST_SRC)

check_styles:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind: test
	valgrind --tool=memcheck --leak-check=yes --quiet ./$(TEST_TARGET)

clean:
	@rm -rf $(OBJ) \
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
		format \
		check_styles \
		valgrind
