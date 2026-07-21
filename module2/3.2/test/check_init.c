#include "test.h"

START_TEST(check_init_1) {
  gateway gw;
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "255.255.0.255"), SUCCESS);
  ck_assert_int_eq(gw.ip == 0b01111111000000000000000000000001, SUCCESS);
  ck_assert_int_eq(gw.mask == 0b11111111111111110000000011111111, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "217.71.138.1", "105.123.1.5"), SUCCESS);
  ck_assert_int_eq(gw.ip == 0b11011001010001111000101000000001, SUCCESS);
  ck_assert_int_eq(gw.mask == 0b01101001011110110000000100000101, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "0.0.0.0", "255.255.255.255"), SUCCESS);
  ck_assert_int_eq(gw.ip == 0b00000000000000000000000000000000, SUCCESS);
  ck_assert_int_eq(gw.mask == 0b11111111111111111111111111111111, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "1.2.3", "105.123.1.5"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "1.2.3"), ERROR);

  ck_assert_int_eq(gateway_init(&gw, "1.2.3.4.5", "105.123.1.5"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "1.2.3.4.5"), ERROR);

  ck_assert_int_eq(gateway_init(&gw, "1..2.3.4", "105.123.1.5"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "1..2.3.4"), ERROR);

  ck_assert_int_eq(gateway_init(&gw, ".1.2.3.4", "105.123.1.5"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", ".1.2.3.4"), ERROR);

  ck_assert_int_eq(gateway_init(&gw, "1.2.3.4.", "105.123.1.5"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "1.2.3.4."), ERROR);

  ck_assert_int_eq(gateway_init(&gw, "1.2.q.4", "105.123.1.5"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "1.2.q.4"), ERROR);

  ck_assert_int_eq(gateway_init(&gw, "1.256.3.4", "105.123.1.5"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "1.400.3.4"), ERROR);

  ck_assert_int_eq(gateway_init(&gw, "1.-2.3.4", "105.123.1.5"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "1.2.3.-4"), ERROR);
}

START_TEST(check_init_2) {
  gateway gw;
  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "/8"), SUCCESS);
  ck_assert_int_eq(gw.mask == 0b11111111000000000000000000000000, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "217.71.138.1", "/1"), SUCCESS);
  ck_assert_int_eq(gw.mask == 0b10000000000000000000000000000000, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "0.0.0.0", "/32"), SUCCESS);
  ck_assert_int_eq(gw.mask == 0b11111111111111111111111111111111, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "127.0.0.1", "/0"), SUCCESS);
  ck_assert_int_eq(gw.mask == 0b00000000000000000000000000000000, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "217.71.138.1", "/31"), SUCCESS);
  ck_assert_int_eq(gw.mask == 0b11111111111111111111111111111110, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "0.0.0.0", "/17"), SUCCESS);
  ck_assert_int_eq(gw.mask == 0b11111111111111111000000000000000, SUCCESS);

  ck_assert_int_eq(gateway_init(&gw, "1.2.3.4", "/33"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "1.2.3.4", "1"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "1.2.3.4", "/-2"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "1.2.3.4", "/1.2"), ERROR);
  ck_assert_int_eq(gateway_init(&gw, "1.2.3.4", "/1q"), ERROR);
}

Suite *check_init() {
  Suite *s = suite_create("check_init");
  TCase *tc = tcase_create("init test cases for gateway.a");

  tcase_add_test(tc, check_init_1);
  tcase_add_test(tc, check_init_2);
  suite_add_tcase(s, tc);

  return s;
}