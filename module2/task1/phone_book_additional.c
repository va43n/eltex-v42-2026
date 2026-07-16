#include "phone_book.h"

int set_full_name(phone_book* pb, char full_name[]) {
  if (strlen(full_name) == 0) return 0;

  strcpy(pb->full_name, full_name);
  return 1;
}

int set_job_place(phone_book* pb, char job_place[]) {
  strcpy(pb->job_place, job_place);
  return 1;
}

int set_job_position(phone_book* pb, char job_position[]) {
  strcpy(pb->job_position, job_position);
  return 1;
}

int check_phone_number(char* pn) {
  regex_t regex;
  int reti;

  reti = regcomp(&regex, "^\\+?[0-9]+$", REG_EXTENDED);
  if (reti) {
    return 0;
  }

  reti = regexec(&regex, pn, 0, NULL, 0);

  regfree(&regex);

  return !reti;
}

int set_phone_numbers(phone_book* pb, char** phone_numbers,
                      size_t phone_numbers_n) {
  pb->phone_numbers_n = phone_numbers_n;
  pb->phone_numbers = (char**)malloc(sizeof(char*) * phone_numbers_n);
  for (size_t i = 0; i < phone_numbers_n; i++) {
    if (!check_phone_number(phone_numbers[i])) {
      pb->phone_numbers_n = i;
      return 0;
    }
    pb->phone_numbers[i] =
        (char*)malloc(sizeof(char) * (strlen(phone_numbers[i]) + 1));
    strcpy(pb->phone_numbers[i], phone_numbers[i]);
  }

  return 1;
}

int set_socials(phone_book* pb, socials_t* socials, size_t socials_n) {
  pb->socials_n = socials_n;
  pb->socials = (socials_t*)malloc(sizeof(socials_t) * socials_n);
  for (size_t i = 0; i < socials_n; i++) {
    strcpy(pb->socials[i].social_network_name, socials[i].social_network_name);
    strcpy(pb->socials[i].social_network_url, socials[i].social_network_url);
  }

  return 1;
}

int set_other(phone_book* pb, char other[]) {
  strcpy(pb->other, other);
  return 1;
}

size_t get_unique_index(phone_book* head) {
  phone_book* ptr = head->next;
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

void free_one_page(phone_book* pb) {
  if (pb->phone_numbers != NULL) {
    for (size_t i = 0; i < pb->phone_numbers_n; i++) free(pb->phone_numbers[i]);
    free(pb->phone_numbers);
  }
  if (pb->socials != NULL) free(pb->socials);
}