#include "test.h"

START_TEST(check_remove_1) {
  int result;

  phone_book* head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);
  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);

  phone_book_remove_page(head, 1);
  phone_book_remove_page(head, 7);
  phone_book_remove_page(head, 3);
  phone_book_remove_page(head, 4);

  phone_book* page;

  page = phone_book_get_page(head, 1);
  printf("one single page\n");
  phone_book_print_page(page);
  ck_assert_int_eq(page == NULL, 1);

  page = phone_book_get_page(head, 5);
  ck_assert_int_eq(page == NULL, 0);
  ck_assert_int_eq(strcmp(page->full_name, NAME2), 0);
  ck_assert_int_eq(page->numbers_n, 1);
  ck_assert_int_eq(strcmp(page->numbers[0], NUMBER3), 0);
  ck_assert_int_eq(page->socials_n, 1);
  ck_assert_int_eq(strcmp(page->socials[0].social_name, SOCIALS_NAME1), 0);
  ck_assert_int_eq(strcmp(page->socials[0].social_url, SOCIALS_URL1), 0);
  ck_assert_int_eq(strcmp(page->other, OTHER2), 0);

  page = phone_book_get_page(head, 6);
  ck_assert_int_eq(page == NULL, 0);
  ck_assert_int_eq(strcmp(page->full_name, NAME3), 0);
  ck_assert_int_eq(page->numbers_n, 0);
  ck_assert_int_eq(page->socials_n, 0);
  ck_assert_int_eq(strcmp(page->other, OTHER2), 0);

  ck_assert_int_eq(phone_book_get_page(head, 1) == NULL, 1);
  ck_assert_int_eq(phone_book_get_page(head, 3) == NULL, 1);
  ck_assert_int_eq(phone_book_get_page(head, 4) == NULL, 1);
  ck_assert_int_eq(phone_book_get_page(head, 7) == NULL, 1);

  ck_assert_int_eq(phone_book_find_page_by_full_name(head, NAME1), 2);
  ck_assert_int_eq(phone_book_find_page_by_full_name(head, NAME2), 5);
  ck_assert_int_eq(phone_book_find_page_by_full_name(head, NAME3), 6);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

START_TEST(check_remove_2) {
  int result;

  phone_book* head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);

  phone_book_print_tree(head);

  phone_book_remove_page(head, 2);
  phone_book_remove_page(head, 3);

  USER_3("0123", JOB_PLACE2, JOB_POSITION1, OTHER4, &result);
  ck_assert_int_eq(result, SUCCESS);
  USER_3("Ivanych", JOB_PLACE2, JOB_POSITION1, OTHER5, &result);
  ck_assert_int_eq(result, SUCCESS);
  USER_3("Alex", JOB_PLACE2, JOB_POSITION1, OTHER6, &result);
  ck_assert_int_eq(result, SUCCESS);

  size_t idx;
  idx = phone_book_find_page_by_full_name(head, "0123");
  ck_assert_int_ne(idx, INDEX_NOT_FOUND);
  phone_book* page = phone_book_get_page(head, idx);
  ck_assert_int_eq(page == NULL, 0);
  ck_assert_int_eq(strcmp(page->other, OTHER4), 0);

  idx = phone_book_find_page_by_full_name(head, "Ivanych");
  ck_assert_int_ne(idx, INDEX_NOT_FOUND);
  page = phone_book_get_page(head, idx);
  ck_assert_int_eq(page == NULL, 0);
  ck_assert_int_eq(strcmp(page->other, OTHER5), 0);

  idx = phone_book_find_page_by_full_name(head, "Alex");
  ck_assert_int_ne(idx, INDEX_NOT_FOUND);
  page = phone_book_get_page(head, idx);
  ck_assert_int_eq(page == NULL, 0);
  ck_assert_int_eq(strcmp(page->other, OTHER6), 0);

  ck_assert_int_eq(phone_book_get_page(head, 2) == NULL, 0);
  ck_assert_int_eq(phone_book_get_page(head, 3) == NULL, 0);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

START_TEST(check_remove_3) {
  int result;

  phone_book* head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER2, &result);
  USER_3(NAME3, JOB_PLACE2, JOB_POSITION1, OTHER2, &result);

  phone_book_remove_page(head, 1);
  phone_book_remove_page(head, 2);
  phone_book_remove_page(head, 3);
  phone_book_remove_page(head, 4);
  phone_book_remove_page(head, 5);

  printf("remove_3\n");
  phone_book_print_tree(head);

  ck_assert_int_eq(head->left == NULL, 1);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

Suite* check_remove() {
  Suite* s = suite_create("check_remove");
  TCase* tc = tcase_create("remove test cases for phone_book.a");

  tcase_add_test(tc, check_remove_1);
  tcase_add_test(tc, check_remove_2);
  tcase_add_test(tc, check_remove_3);
  suite_add_tcase(s, tc);

  return s;
}