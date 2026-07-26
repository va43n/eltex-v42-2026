#include "test.h"

START_TEST(check_check_1) {
  gateway gw;
  ck_assert_int_eq(gateway_init(&gw, "255.255.255.255", "255.255.255.255"),
                   SUCCESS);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111111111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111111111110), 0);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111101111111), 0);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111011111111), 0);
  ck_assert_int_eq(gateway_check_ip(gw, 0b01111111111111111111111111111111), 0);

  ck_assert_int_eq(gateway_init(&gw, "255.255.255.255", "255.255.255.0"),
                   SUCCESS);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111111111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111111111110), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111101111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111100000000), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111011111111), 0);
  ck_assert_int_eq(gateway_check_ip(gw, 0b01111111111111111111111111111111), 0);

  ck_assert_int_eq(gateway_init(&gw, "255.255.255.255", "255.254.127.255"),
                   SUCCESS);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111111111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111101111111111111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111110111111111111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111100111111111111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111111111110), 0);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111101111111), 0);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111100000000), 0);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111011111111), 0);
  ck_assert_int_eq(gateway_check_ip(gw, 0b01111111111111111111111111111111), 0);

  ck_assert_int_eq(gateway_init(&gw, "255.255.255.255", "/1"), SUCCESS);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111111111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111111111110), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111101111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b11111111111111111111111011111111), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b10000000000000000000000000000000), 1);
  ck_assert_int_eq(gateway_check_ip(gw, 0b01111111111111111111111111111111), 0);
}

Suite *check_check() {
  Suite *s = suite_create("check_check");
  TCase *tc = tcase_create("check test cases for gateway.a");

  tcase_add_test(tc, check_check_1);
  suite_add_tcase(s, tc);

  return s;
}