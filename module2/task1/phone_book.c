#include "phone_book.h"

phone_book* phone_book_create() {
  phone_book* head = (phone_book*)malloc(sizeof(phone_book));
  head->next = NULL;

  return head;
}

int phone_book_free(phone_book* head) {
  if (head == NULL) return ERROR;

  phone_book* ptr = head;

  while (ptr != NULL) {
    free_one_page(ptr);

    phone_book* temp = ptr;
    ptr = ptr->next;
    free(temp);
  }

  return SUCCESS;
}

int phone_book_add_page(phone_book* head, char full_name[], char job_place[],
                        char job_position[], char** phone_numbers,
                        size_t phone_numbers_n, socials_t* socials,
                        size_t socials_n, char other[]) {
  if (head == NULL) return ERROR;

  phone_book* pb = (phone_book*)malloc(sizeof(phone_book));
  pb->index = get_unique_index(head);

  if (set_full_name(pb, full_name) && set_job_place(pb, job_place) &&
      set_job_position(pb, job_position) &&
      set_phone_numbers(pb, phone_numbers, phone_numbers_n) &&
      set_socials(pb, socials, socials_n) && set_other(pb, other)) {
    pb->next = head->next;
    head->next = pb;
    return SUCCESS;
  }

  free_one_page(pb);

  return ERROR;
}

int phone_book_remove_page(phone_book* head, size_t index) {
  if (head == NULL) return ERROR;

  phone_book *ptr = head->next, *prev = head;

  while (ptr != NULL) {
    if (ptr->index == index) {
      prev->next = ptr->next;
      ptr->next = NULL;
      free_one_page(ptr);
      free(ptr);

      return SUCCESS;
    }
    ptr = ptr->next;
    prev = prev->next;
  }

  return ERROR;
}

size_t phone_book_find_page_by_full_name(phone_book* head, char full_name[]) {
  if (head == NULL) return INDEX_NOT_FOUND;

  regex_t regex;
  int reti;

  reti = regcomp(&regex, full_name, REG_EXTENDED | REG_ICASE);
  if (reti) {
    return INDEX_NOT_FOUND;
  }

  phone_book* ptr = head->next;
  while (ptr != NULL) {
    reti = regexec(&regex, ptr->full_name, 0, NULL, 0);
    if (!reti) {
      regfree(&regex);
      return ptr->index;
    }

    ptr = ptr->next;
  }

  regfree(&regex);

  return INDEX_NOT_FOUND;
}

size_t phone_book_find_page_by_phone_number(phone_book* head,
                                            char phone_number[]) {
  if (head == NULL) return INDEX_NOT_FOUND;

  regex_t regex;
  int reti;

  reti = regcomp(&regex, phone_number, REG_EXTENDED);
  if (reti) {
    return INDEX_NOT_FOUND;
  }

  phone_book* ptr = head->next;
  while (ptr != NULL) {
    for (size_t i = 0; i < ptr->phone_numbers_n; i++) {
      reti = regexec(&regex, ptr->phone_numbers[i], 0, NULL, 0);
      if (!reti) {
        regfree(&regex);
        return ptr->index;
      }
    }
    ptr = ptr->next;
  }

  regfree(&regex);

  return INDEX_NOT_FOUND;
}

phone_book* phone_book_get_page(phone_book* head, size_t index) {
  if (head == NULL) return NULL;

  phone_book* ptr = head->next;

  while (ptr != NULL) {
    if (ptr->index == index) return ptr;
    ptr = ptr->next;
  }

  return NULL;
}

int phone_book_print(phone_book* head) {
  if (head == NULL) return ERROR;

  phone_book* ptr = head->next;

  printf("===============================\n");
  printf("My phone book! \n");

  while (ptr != NULL) {
    phone_book_print_page(ptr);
    ptr = ptr->next;
  }
  printf("===============================\n\n");

  return SUCCESS;
}

int phone_book_print_page(phone_book* page) {
  if (page == NULL) return ERROR;

  printf("\n--------------------------------\n");
  printf("Unique index: %ld\n", page->index);
  printf("Full name: %s\n", page->full_name);
  printf("Job place: %s\n", page->job_place);
  printf("Job position: %s\n", page->job_position);

  printf("Phone numbers:\n");
  for (size_t j = 0; j < page->phone_numbers_n; j++) {
    printf("\t%s\n", page->phone_numbers[j]);
  }

  printf("Socials:\n");
  for (size_t j = 0; j < page->socials_n; j++) {
    printf("\t%s: %s\n", page->socials[j].social_network_name,
           page->socials[j].social_network_url);
  }

  printf("Other information:%s%s\n", strlen(page->other) == 0 ? "" : "\n",
         page->other);

  printf("--------------------------------\n");

  return SUCCESS;
}