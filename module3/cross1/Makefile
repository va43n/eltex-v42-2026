CC = gcc
CLANG_FORMAT = clang-format
CFLAGS = -Wall -Wextra -Werror -std=gnu11

SOCK_LIB_SRC = lib/create.c \
               lib/send.c \
							 lib/receive.c \
							 lib/close.c
SOCK_LIB_OBJ = $(SOCK_LIB_SRC:.c=.o)
SOCK_LIB_NAME = raw_socket_operations.a

SRC = signal_handler.c \
      parser.c \
			client.c \
			server.c \
			clients_buffer.c
OBJ = $(SRC:.c=.o)

MANUAL_TEST_SRC = main.c
MANUAL_TEST_TARGET = echo_reply.out

all: manual_testing

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

manual_testing: $(OBJ) $(SOCK_LIB_NAME)
	$(CC) $(CFLAGS) \
	$(MANUAL_TEST_SRC) \
	$(OBJ) $(SOCK_LIB_NAME) \
	-o $(MANUAL_TEST_TARGET)

$(SOCK_LIB_NAME): $(SOCK_LIB_OBJ)
	ar rcs $@ $^

format:
	$(CLANG_FORMAT) -i *.h $(SRC) $(MANUAL_TEST_SRC)

check_styles:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind: manual_testing
	valgrind --tool=memcheck --leak-check=yes --quiet ./$(MANUAL_TEST_TARGET)

clean:
	@rm -rf $(OBJ) \
					$(SOCK_LIB_OBJ) \
					$(MANUAL_TEST_TARGET) \
					$(SOCK_LIB_NAME) \
					*.o \
					*.out

.PHONY: all \
		manual_testing \
		clean \
		format \
		check_styles \
		valgrind
