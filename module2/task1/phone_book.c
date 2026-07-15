#include "phone_book.h"

phone_book* create_phone_book() {
  phone_book* head = (phone_book*)malloc(sizeof(phone_book));
  head->next = NULL;

  return head;
}

void free_phone_book(phone_book* head) {
  if (head == NULL) return;

  phone_book *ptr = head;

  while (ptr != NULL) {
    if (ptr->phone_numbers != NULL) {
        for (size_t i = 0; i < ptr->phone_numbers_n; i++)
            free(ptr->phone_numbers[i]);
        free(ptr->phone_numbers);
    }
    if (ptr->socials != NULL) free(ptr->socials);

    phone_book* temp = ptr;
    ptr = ptr->next;
    free(temp);
  }
}

void phone_book_push_back(phone_book* head, char full_name[], char job_place[],
                          char job_position[], char** phone_numbers,
                          size_t phone_numbers_n, socials_t* socials,
                          size_t socials_n, char other[]) {
  phone_book* ptr = head;

  while (ptr->next != NULL) {
    ptr = ptr->next;
  }

  phone_book* new_record = (phone_book*)malloc(sizeof(phone_book));
  new_record->index = find_unique_index(head);
  strcpy(new_record->full_name, full_name);
  strcpy(new_record->job_place, job_place);
  strcpy(new_record->job_position, job_position);

  new_record->phone_numbers_n = phone_numbers_n;
  new_record->phone_numbers = (char**)malloc(sizeof(char*) * phone_numbers_n);
  for (size_t i = 0; i < phone_numbers_n; i++) {
    new_record->phone_numbers[i] =
        (char*)malloc(sizeof(char) * (strlen(phone_numbers[i]) + 1));
    strcpy(new_record->phone_numbers[i], phone_numbers[i]);
  }

  new_record->socials_n = socials_n;
  new_record->socials = (socials_t*)malloc(sizeof(socials_t) * socials_n);
  for (size_t i = 0; i < socials_n; i++) {
    new_record->socials[i] = socials[i];
  }

  strcpy(new_record->other, other);

  new_record->next = NULL;

  ptr->next = new_record;
}

size_t find_unique_index(phone_book* head) {
    phone_book *ptr = head->next;
    size_t start_index = 0;

    while (ptr != NULL) {
        if (ptr->index > start_index + 1) {
            start_index = ptr->index;
            break;
        }

        start_index = ptr->index;
        ptr = ptr->next;
    }

    return start_index + 1;
}

// void remove_phone_book(phone_book *head, size_t index) {
//     //
//     //
// }

void print_phone_book(phone_book* head) {
  printf("My phone book! \n");
  phone_book* ptr = head->next;
  size_t counter = 0;
  while (ptr != NULL) {
    printf("---%ld--------------------------\n", ++counter);
    printf("Unique index: %ld\n", ptr->index);
    printf("Full name: %s\n", ptr->full_name);
    printf("Job place: %s\n", ptr->job_place);
    printf("Job position: %s\n", ptr->job_position);

    printf("Phone numbers:\n");
    for (size_t j = 0; j < ptr->phone_numbers_n; j++) {
      printf("\t%s\n", ptr->phone_numbers[j]);
    }

    printf("Socials:\n");
    for (size_t j = 0; j < ptr->socials_n; j++) {
      printf("\t%s: %s\n", ptr->socials[j].social_network_name,
             ptr->socials[j].social_network_url);
    }

    printf("Other information:%s%s\n", strlen(ptr->other) == 0 ? "" : "\n",
           ptr->other);

    ptr = ptr->next;
    printf("---%ld--------------------------\n\n", counter);
  }
  printf("\n");
}