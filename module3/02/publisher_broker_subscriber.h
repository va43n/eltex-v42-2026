#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define PUBLISHER 'p'
#define BROKER 'b'
#define SUBSCRIBER 's'

#define SUCCESS 111
#define FAILURE -111

#define PATHNAME "broker"
#define PROJ_ID 123

#define TOPIC_SIZE 64
#define PAYLOAD_SIZE 256

typedef struct {
  long mtype;
  int message_type;
  int pid;
  char topic[TOPIC_SIZE];
  char payload[PAYLOAD_SIZE];
} message;

#define MT_SEND 10
#define MT_SUBSCRIBE 11
#define SUBSCRIBE_TEXT "subscribe"
#define MT_UNSUBSCRIBE 12
#define UNSUBSCRIBE_TEXT "unsubscribe"

static volatile int is_signal = FALSE;

// parser.c
int parse_input(int argc, char *argv[], char *mode);
int parse_flag(char *flag, char *mode);
int start_corresponding_mode(char mode);

// general_ipc_functions.c
int check_if_queue_is_created(key_t key);
int set_some_message_parameters(message *my_msg, int message_type, int pid,
                                long priority);
int get_message_and_send(int write, message *my_msg,
                         unsigned int number_of_inputs_needed);
int send_message(int write, message my_msg);
int recv_message(int write, message* my_msg);

// participant_array_logic.c
typedef struct {
  char** topics;
  char** payloads;
  size_t size;
  int pid;
} participant;
int participant_push_back(participant** ps, size_t* size, int pid, char* topic, char* payload);
int participant_add_message_by_index(participant** ps, size_t size, size_t index, char* topic, char* payload);
int participant_find_by_pid(participant* ps, size_t size, int pid, size_t* index);
int participant_remove_topic_by_index(participant** ps, size_t* size, size_t index, char* topic);
int participant_check_if_topic_exists(participant* ps, size_t size, size_t index, char* topic);
int participant_free(participant* ps, size_t size);

// publisher.c
int do_publisher_activity();

// broker.c
int do_broker_activity();
int broker_send_message_to_all_subscribers(int write, message my_msg, participant* subscribers, size_t subs_t);

// subscriber.c
int do_subscriber_activity();
