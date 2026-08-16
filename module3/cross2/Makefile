CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11

# LIB_SRC = 
# LIB_OBJ = $(LIB_SRC:.c=.o)
# LIB_NAME = 

SRC = # add later
OBJ = $(SRC:.c=.o)

MANUAL_TEST_SRC = main.c
MANUAL_TEST_TARGET = header.out

all: manual_testing

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

manual_testing: $(OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) $(MANUAL_TEST_SRC) $(OBJ) $(LIB_NAME) -o $(MANUAL_TEST_TARGET)

$(LIB_NAME): $(LIB_OBJ)
	ar rcs $@ $^

format:
	$(CLANG_FORMAT) -i *.h \
										 lib/*.h \
										 $(SRC) \
										 $(LIB_SRC) \
										 $(MANUAL_TEST_SRC)

check:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./$(MANUAL_TEST_TARGET) $(INPUT)

clean:
	@rm -rf $(OBJ) \
					$(LIB_OBJ) \
					$(MANUAL_TEST_TARGET) \
					$(LIB_NAME) \
					*.o \
					*.out

.PHONY: all \
				manual_testing \
				clean \
				format \
				check_styles \
				valgrind
