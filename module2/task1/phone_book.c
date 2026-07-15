#include "phone_book.h"

phone_book* create_phone_book() {
  phone_book* head = (phone_book*)malloc(sizeof(phone_book));
  head->next = NULL;

  return head;
}

void free_phone_book(phone_book* head) {
  if (head == NULL) return;

  phone_book* ptr = head;

  while (ptr != NULL) {
    free_one_page(ptr);

    phone_book* temp = ptr;
    ptr = ptr->next;
    free(temp);
  }
}

int phone_book_add(phone_book* head, char full_name[], char job_place[],
                         char job_position[], char** phone_numbers,
                         size_t phone_numbers_n, socials_t* socials,
                         size_t socials_n, char other[]) {
  phone_book* pb = (phone_book*)malloc(sizeof(phone_book));
  pb->index = get_unique_index(head);

  if (set_full_name(pb, full_name) && set_job_place(pb, job_place) &&
      set_job_position(pb, job_position) &&
      set_phone_numbers(pb, phone_numbers, phone_numbers_n) &&
      set_socials(pb, socials, socials_n) && set_other(pb, other)) {
    pb->next = head->next;
    head->next = pb;
    return 1;
  }

  free_one_page(pb);

  return 0;
}

int remove_phone_book(phone_book* head, size_t index) {
  phone_book *ptr = head->next, *prev = head;

  while (ptr != NULL) {
    if (ptr->index == index) {
      prev->next = ptr->next;
      ptr->next = NULL;
      free_one_page(ptr);
      free(ptr);

      return 1;
    }
    ptr = ptr->next;
    prev = prev->next;
  }

  return 0;
}

void print_phone_book(phone_book* head) {
  printf("===============================\n");
  printf("My phone book! \n");
  phone_book* ptr = head->next;
  size_t counter = 0;
  while (ptr != NULL) {
    printf("\n---%ld--------------------------\n", ++counter);
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
    printf("---%ld--------------------------\n", counter);
  }
  printf("===============================\n\n");
}