#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define ERROR 0

#define FIRST_UNIQUE_INDEX 1
#define INDEX_NOT_FOUND 0

typedef struct socials_t {
  char social_name[32];
  char social_url[128];
} socials_t;

typedef struct phone_book {
  size_t index;
  char full_name[128];
  char job_place[128];
  char job_position[128];
  char** numbers;
  size_t numbers_n;
  socials_t* socials;
  size_t socials_n;
  char other[512];
  struct phone_book* next;
} phone_book;

phone_book* phone_book_create();
int phone_book_free(phone_book* head);

int phone_book_add_page(phone_book* head, char full_name[], char job_place[],
                        char job_position[], char** numbers, size_t numbers_n,
                        socials_t* socials, size_t socials_n, char other[]);
int phone_book_remove_page(phone_book* head, size_t index);

size_t phone_book_find_page_by_full_name(phone_book* head, char full_name[]);
size_t phone_book_find_page_by_number(phone_book* head, char number[]);

phone_book* phone_book_get_page(phone_book* head, size_t index);

int phone_book_print(phone_book* head);
int phone_book_print_page(phone_book* page);

int phone_book_set_full_name(phone_book* pb, char full_name[]);
int phone_book_set_job_place(phone_book* pb, char job_place[]);
int phone_book_set_job_position(phone_book* pb, char job_position[]);
int phone_book_set_numbers(phone_book* pb, char** numbers, size_t numbers_n);
int phone_book_set_socials(phone_book* pb, socials_t* socials,
                           size_t socials_n);
int phone_book_set_other(phone_book* pb, char other[]);

int phone_book_compare_pages(phone_book* pb1, phone_book* pb2);
int phone_book_compare(phone_book* head1, phone_book* head2);

int phone_book_save(phone_book* head, char file_name[]);
phone_book* phone_book_load(char file_name[]);

#define EDIT_FULL_NAME 0x10000000
#define EDIT_JOB_PLACE 0x01000000
#define EDIT_JOB_POSITION 0x00100000
#define EDIT_NUMBERS 0x00010000
#define EDIT_NUMBERS_ADD 0x00001000
//  EDIT_NUMBERS_DELETE  0x____0___
#define EDIT_SOCIALS 0x00000100
#define EDIT_SOCIALS_ADD 0x00000010
//  EDIT_SOCIALS_DELETE  0x______0_
#define EDIT_OTHER 0x00000001

int phone_book_edit(phone_book* head, unsigned int format, ...);

void _free_one_page(phone_book* pb);
size_t _get_unique_index(phone_book* head);
int _check_number(char* pn);