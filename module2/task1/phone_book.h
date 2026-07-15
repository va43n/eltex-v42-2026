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

int phone_book_add(phone_book* head, char full_name[], char job_place[],
                         char job_position[], char** phone_numbers,
                         size_t phone_numbers_n, socials_t* socials,
                         size_t socials_n, char other[]);
int remove_phone_book(phone_book* head, size_t index);

void print_phone_book(phone_book* head);

int set_full_name(phone_book* pb, char full_name[]);
int set_job_place(phone_book* pb, char job_place[]);
int set_job_position(phone_book* pb, char job_position[]);
int set_phone_numbers(phone_book* pb, char** phone_numbers,
                      size_t phone_numbers_n);
int set_socials(phone_book* pb, socials_t* socials, size_t socials_n);
int set_other(phone_book* pb, char other[]);

void free_one_page(phone_book* pb);

size_t get_unique_index(phone_book* head);