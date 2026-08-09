CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11

SRC = chat.c socket_functions.c signal_handler.c
OBJ = $(SRC:.c=.o)

MANUAL_TEST_SRC = main.c
MANUAL_TEST_TARGET = man_test_exe

all: manual_testing

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

manual_testing: $(OBJ)
	@$(CC) $(CFLAGS) $(MANUAL_TEST_SRC) $(OBJ) -o $(MANUAL_TEST_TARGET) -lrt

format:
	$(CLANG_FORMAT) -i *.h $(SRC) $(MANUAL_TEST_SRC)

check_styles:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind: manual_testing
	valgrind --tool=memcheck --leak-check=yes --quiet ./$(MANUAL_TEST_TARGET)

clean:
	@rm -rf $(OBJ) \
			$(MANUAL_TEST_TARGET) \
			*.o 

.PHONY: all \
		manual_testing \
		clean \
		format \
		check_styles \
		valgrind
