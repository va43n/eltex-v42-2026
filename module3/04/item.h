#define TRUE 1
#define FALSE 0

#define ARRAY_SIZE 16

typedef struct item {
  int array[ARRAY_SIZE];
  unsigned int size;
  struct item* next;
} item;

int check_if_item_is_processed(item* my_item);
item generate_item();
