#include <check.h>

#include "../phone_book.h"

Suite *check_add();
Suite *check_remove();
Suite *check_find();
Suite *check_set();
Suite *check_edit();

#define NAME1 "Yashkov Ivan Vitalevich"
#define NAME2 "Ivanov Ivan"
#define NAME3 "123456"
#define NAME4 "Vitalevich Alexey"
#define WRONG_NAME1 ""

#define JOB_PLACE1 "Eltex"
#define JOB_PLACE2 "Some place"
#define JOB_PLACE3 ""

#define JOB_POSITION1 "postition"
#define JOB_POSITION2 "Any"
#define JOB_POSITION3 ""

#define NUMBER1 "+73332221100"
#define NUMBER2 "89876543210"
#define NUMBER3 "1234567"
#define NUMBER4 "+7-123-456-78-90"
#define NUMBER5 "123-45-67"
#define WRONG_NUMBER1 "abc"
#define WRONG_NUMBER2 "+73332 21100"
#define WRONG_NUMBER3 ""

#define SOCIALS_NAME1 "Telegram"
#define SOCIALS_NAME2 "VK"
#define SOCIALS_NAME3 "MAX"
#define SOCIALS_NAME4 "OK"

#define SOCIALS_URL1 "t.me/qwerty"
#define SOCIALS_URL2 "vk.ru/user_1"
#define SOCIALS_URL3 "max.ru/cool_guy"
#define SOCIALS_URL4 "ok.ru/123456"

#define OTHER1 "Hello, this is some information about me."
#define OTHER2 "2"
#define OTHER3 ""
#define OTHER4 "4"
#define OTHER5 "5"
#define OTHER6 "6"

#define USER_1(n, jp, jpos, N1, N2, SN1, SN2, SN3, SN4, SU1, SU2, SU3, SU4, o, result) \
do { \
  char full_name[] = n; \
  char job_place[] = jp; \
  char job_position[] = jpos; \
  char** numbers; \
  char n1[] = N1, n2[] = N2; \
  size_t numbers_n = 2; \
  socials_t* socials; \
  char sn1[] = SN1, sn2[] = SN2, sn3[] = SN3, sn4[] = SN4; \
  char su1[] = SU1, su2[] = SU2, \
       su3[] = SU3, su4[] = SU4; \
  size_t socials_n = 4; \
  char other[] = o; \
  \
  numbers = (char**)malloc(sizeof(char*) * numbers_n); \
  numbers[0] = (char*)malloc(sizeof(char) * (strlen(n1) + 1)); \
  strcpy(numbers[0], n1); \
  numbers[1] = (char*)malloc(sizeof(char) * (strlen(n2) + 1)); \
  strcpy(numbers[1], n2); \
  \
  socials = (socials_t*)malloc(sizeof(socials_t) * socials_n); \
  strcpy(socials[0].social_name, sn1); \
  strcpy(socials[0].social_url, su1); \
  strcpy(socials[1].social_name, sn2); \
  strcpy(socials[1].social_url, su2); \
  strcpy(socials[2].social_name, sn3); \
  strcpy(socials[2].social_url, su3); \
  strcpy(socials[3].social_name, sn4); \
  strcpy(socials[3].social_url, su4); \
  \
  *result = phone_book_add_page(head, full_name, job_place, job_position, numbers, \
                      numbers_n, socials, socials_n, other); \
  \
  for (size_t i = 0; i < numbers_n; i++) free(numbers[i]); \
  free(numbers); \
  free(socials); \
} while(0)

#define USER_2(n, jp, jpos, N1, SN1, SU1, o, result) \
do { \
  char full_name[] = n; \
  char job_place[] = jp; \
  char job_position[] = jpos; \
  char** numbers; \
  char n1[] = N1; \
  size_t numbers_n = 1; \
  socials_t* socials; \
  char sn1[] = SN1; \
  char su1[] = SU1; \
  size_t socials_n = 1; \
  char other[] = o; \
  \
  numbers = (char**)malloc(sizeof(char*) * numbers_n); \
  numbers[0] = (char*)malloc(sizeof(char) * (strlen(n1) + 1)); \
  strcpy(numbers[0], n1); \
  \
  socials = (socials_t*)malloc(sizeof(socials_t) * socials_n); \
  strcpy(socials[0].social_name, sn1); \
  strcpy(socials[0].social_url, su1); \
  \
  *result = phone_book_add_page(head, full_name, job_place, job_position, numbers, \
                      numbers_n, socials, socials_n, other); \
  \
  for (size_t i = 0; i < numbers_n; i++) free(numbers[i]); \
  free(numbers); \
  free(socials); \
} while(0)

#define USER_3(n, jp, jpos, o, result) \
do { \
  char full_name[] = n; \
  char job_place[] = jp; \
  char job_position[] = jpos; \
  char** numbers; \
  size_t numbers_n = 0; \
  size_t socials_n = 0; \
  socials_t *socials; \
  char other[] = o; \
  \
  numbers = (char**)malloc(sizeof(char*) * numbers_n); \
  \
  socials = (socials_t*)malloc(sizeof(socials_t) * socials_n); \
  \
  *result = phone_book_add_page(head, full_name, job_place, job_position, numbers, \
                      numbers_n, socials, socials_n, other); \
  \
  for (size_t i = 0; i < numbers_n; i++) free(numbers[i]); \
  free(numbers); \
  free(socials); \
} while(0)
