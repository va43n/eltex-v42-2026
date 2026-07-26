#include "test.h"

START_TEST(check_find_1) {
  int result;

  phone_book *head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME2, JOB_PLACE1, JOB_POSITION1, NUMBER3, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER2, &result);
  USER_3(NAME4, JOB_PLACE1, JOB_POSITION1, OTHER3, &result);
  USER_2(NAME4, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER4, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER5, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER5, &result);
  USER_2(NAME3, JOB_PLACE2, JOB_POSITION2, NUMBER4, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER6, &result);

  ck_assert_int_eq(
      strcmp(phone_book_get_page(head,
                                 phone_book_find_page_by_full_name(head, NAME1))
                 ->other,
             OTHER1),
      0);
  ck_assert_int_eq(
      strcmp(phone_book_get_page(
                 head, phone_book_find_page_by_full_name(head, "Yashkov"))
                 ->other,
             OTHER1),
      0);
  ck_assert_int_eq(
      strcmp(phone_book_get_page(
                 head, phone_book_find_page_by_full_name(head, "shkov"))
                 ->other,
             OTHER1),
      0);
  ck_assert_int_eq(
      strcmp(phone_book_get_page(head, phone_book_find_page_by_full_name(
                                           head, "yashkov ivan vitalevich"))
                 ->other,
             OTHER1),
      0);
  ck_assert_int_eq(
      strcmp(phone_book_get_page(
                 head, phone_book_find_page_by_full_name(head, "yashkov"))
                 ->other,
             OTHER1),
      0);
  ck_assert_int_eq(
      strcmp(phone_book_get_page(
                 head, phone_book_find_page_by_full_name(head, "ich$"))
                 ->other,
             OTHER1),
      0);
  ck_assert_int_eq(
      strcmp(phone_book_get_page(head,
                                 phone_book_find_page_by_full_name(head, "ich"))
                 ->other,
             OTHER4),
      0);
  ck_assert_int_eq(strcmp(phone_book_get_page(
                              head, phone_book_find_page_by_full_name(head, ""))
                              ->other,
                          OTHER6),
                   0);

  ck_assert_int_eq(
      strcmp(phone_book_get_page(
                 head, phone_book_find_page_by_number(head, "\\+73332221100"))
                 ->other,
             OTHER1),
      0);
  ck_assert_int_eq(
      strcmp(phone_book_get_page(head,
                                 phone_book_find_page_by_number(head, NUMBER2))
                 ->other,
             OTHER2),
      0);
  ck_assert_int_eq(strcmp(phone_book_get_page(
                              head, phone_book_find_page_by_number(head, "-45"))
                              ->other,
                          OTHER6),
                   0);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

START_TEST(check_find_2) {
  int result;

  phone_book *head = phone_book_create();

  USER_1(NAME1, JOB_PLACE1, JOB_POSITION1, NUMBER1, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER1, &result);
  USER_1(NAME2, JOB_PLACE1, JOB_POSITION1, NUMBER3, NUMBER2, SOCIALS_NAME1,
         SOCIALS_NAME2, SOCIALS_NAME3, SOCIALS_NAME4, SOCIALS_URL1,
         SOCIALS_URL2, SOCIALS_URL3, SOCIALS_URL4, OTHER2, &result);
  USER_3(NAME4, JOB_PLACE1, JOB_POSITION1, OTHER3, &result);
  USER_2(NAME4, JOB_PLACE2, JOB_POSITION2, NUMBER3, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER4, &result);
  USER_2(NAME2, JOB_PLACE2, JOB_POSITION2, NUMBER5, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER5, &result);
  USER_2(NAME3, JOB_PLACE2, JOB_POSITION2, NUMBER4, SOCIALS_NAME1, SOCIALS_URL1,
         OTHER6, &result);

  ck_assert_int_eq(phone_book_find_page_by_full_name(head, "Denis"),
                   INDEX_NOT_FOUND);

  ck_assert_int_eq(phone_book_find_page_by_number(head, "123-456-789-0"),
                   INDEX_NOT_FOUND);

  ck_assert_int_eq(phone_book_free(head), SUCCESS);
}

Suite *check_find() {
  Suite *s = suite_create("check_find");
  TCase *tc = tcase_create("find test cases for phone_book.a");

  tcase_add_test(tc, check_find_1);
  tcase_add_test(tc, check_find_2);
  suite_add_tcase(s, tc);

  return s;
}