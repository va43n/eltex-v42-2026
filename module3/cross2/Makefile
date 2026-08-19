CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11

# LIB_SRC = 
# LIB_OBJ = $(LIB_SRC:.c=.o)
# LIB_NAME = 

SRC = command_handler.c user_input_operations.c pids_array_operations.c queue_operations.c child_process.c
OBJ = $(SRC:.c=.o)

MANUAL_TEST_SRC = main.c
MANUAL_TEST_TARGET = taxi.out

all: manual_testing

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

manual_testing: $(OBJ)
	$(CC) $(CFLAGS) $(MANUAL_TEST_SRC) $(OBJ) -o $(MANUAL_TEST_TARGET)

format:
	$(CLANG_FORMAT) -i *.h \
										 $(SRC) \
										 $(MANUAL_TEST_SRC)

check:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./$(MANUAL_TEST_TARGET) $(INPUT)

clean:
	@rm -rf $(OBJ) \
					$(MANUAL_TEST_TARGET) \
					*.o \
					*.out

.PHONY: all \
				manual_testing \
				clean \
				format \
				check_styles \
				valgrind
