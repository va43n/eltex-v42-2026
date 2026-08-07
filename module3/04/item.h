#define TRUE 1
#define FALSE 0

#define ARRAY_SIZE 16
#define NULL_ITEM -1

typedef struct {
  int array[ARRAY_SIZE];
  unsigned int size;
  int next;
} item;

int check_if_item_is_processed(item* my_item);
item generate_item();
