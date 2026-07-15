#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct socials_t {
  char social_network_name[32];
  char social_network_url[128];
} socials_t;

typedef struct phone_book {
    size_t index;
  char full_name[128];
  char job_place[128];
  char job_position[128];
  char** phone_numbers;
  size_t phone_numbers_n;
  socials_t* socials;
  size_t socials_n;
  char other[512];
  struct phone_book* next;
} phone_book;

phone_book* create_phone_book();
void free_phone_book(phone_book* head);

void phone_book_push_back(phone_book* head, char full_name[], char job_place[],
                          char job_position[], char** phone_numbers,
                          size_t phone_numbers_n, socials_t* socials,
                          size_t socials_n, char other[]);

size_t find_unique_index(phone_book* head);

void print_phone_book(phone_book* head);