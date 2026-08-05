#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define CREATOR 123
#define USER -123

#define SUCCESS 111
#define FAILURE -111

#define SIZE 256

static volatile int is_signal = 0;

// prepare.c
int parse_input(int argc, char *argv[], char **queue_name);

// queue_handler.c
int check_if_queue_is_created(char *queue_name, int *status);
int create_queue(char *queue_name);
int connect_to_queue(char *queue_name, int status, mqd_t *read, mqd_t *write);
int start_handler(mqd_t read, mqd_t write);
int get_message_and_send(mqd_t write);
int send_message(mqd_t write, char *buffer, unsigned int priority);
int recv_message(mqd_t read);
int delete_queue(char *queue_name);
void create_actual_queue_name(char *buffer, char *queue_name, char *addition);

// handle_sigint.c
void handle_SIGINT(int sig);
