#include "phone_book.h"

int main() {
  phone_book* head = phone_book_create();

  char full_name[] = "Yashkov Ivan Vitalevich";
  char job_place[] = "Eltex";
  char job_position[] = "Any";
  char** phone_numbers;
  char n1[] = "+77775557711", n2[] = "8-123-456-78-90";
  size_t phone_numbers_n = 2;
  socials_t* socials;
  char sn1[] = "Telegram", sn2[] = "VK", sn3[] = "MAX", sn4[] = "OK";
  char su1[] = "t.me/tg_user", su2[] = "vk.com/vk_profile",
       su3[] = "max.ru/max_lover", su4[] = "odnoklassnik.ru/1";
  size_t socials_n = 4;
  char other[] =
      "My name is Ivan and this is some information about me. I live in "
      "Novosibirsk.";

  phone_numbers = (char**)malloc(sizeof(char*) * phone_numbers_n);
  phone_numbers[0] = (char*)malloc(sizeof(char) * (strlen(n1) + 1));
  strcpy(phone_numbers[0], n1);
  phone_numbers[1] = (char*)malloc(sizeof(char) * (strlen(n2) + 1));
  strcpy(phone_numbers[1], n2);

  socials = (socials_t*)malloc(sizeof(socials_t) * socials_n);
  strcpy(socials[0].social_network_name, sn1);
  strcpy(socials[0].social_network_url, su1);
  strcpy(socials[1].social_network_name, sn2);
  strcpy(socials[1].social_network_url, su2);
  strcpy(socials[2].social_network_name, sn3);
  strcpy(socials[2].social_network_url, su3);
  strcpy(socials[3].social_network_name, sn4);
  strcpy(socials[3].social_network_url, su4);

  phone_book_print(head);

  phone_book_add_page(head, full_name, job_place, job_position, phone_numbers,
                      phone_numbers_n, socials, socials_n, other);
  for (size_t i = 0; i < phone_numbers_n; i++) free(phone_numbers[i]);
  free(phone_numbers);
  free(socials);

  phone_book_print(head);

  char full_name2[] = "Ivanov Ivan Ivanovich";
  char job_place2[] = "";
  char job_position2[] = "";
  char** phone_numbers2;
  char n21[] = "+77775557711";
  size_t phone_numbers_n2 = 1;
  socials_t* socials2;
  size_t socials_n2 = 0;
  char other2[] = "";

  phone_numbers2 = (char**)malloc(sizeof(char*) * phone_numbers_n2);
  phone_numbers2[0] = (char*)malloc(sizeof(char) * (strlen(n21) + 1));
  strcpy(phone_numbers2[0], n21);

  socials2 = (socials_t*)malloc(sizeof(socials_t) * socials_n2);

  phone_book_add_page(head, full_name2, job_place2, job_position2,
                      phone_numbers2, phone_numbers_n2, socials2, socials_n2,
                      other2);
  for (size_t i = 0; i < phone_numbers_n2; i++) free(phone_numbers2[i]);
  free(phone_numbers2);
  free(socials2);

  phone_book_print(head);

  char full_name3[] = "???";
  char job_place3[] = "";
  char job_position3[] = "";
  char** phone_numbers3;
  size_t phone_numbers_n3 = 0;
  socials_t* socials3;
  size_t socials_n3 = 0;
  char other3[] = "";

  phone_numbers3 = (char**)malloc(sizeof(char*) * phone_numbers_n3);

  socials3 = (socials_t*)malloc(sizeof(socials_t) * socials_n3);

  phone_book_add_page(head, full_name3, job_place3, job_position3,
                      phone_numbers3, phone_numbers_n3, socials3, socials_n3,
                      other3);
  for (size_t i = 0; i < phone_numbers_n3; i++) free(phone_numbers3[i]);
  free(phone_numbers3);
  free(socials3);

  phone_book_print(head);

  printf("%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\n",
         phone_book_find_page_by_full_name(head, "Ivanov Ivan Ivanovich"),
         phone_book_find_page_by_full_name(head, "Ivanovich"),
         phone_book_find_page_by_full_name(head, "ivanov ivan ivanovich"),
         phone_book_find_page_by_full_name(head, "ich"),
         phone_book_find_page_by_full_name(head, "ich$"),
         phone_book_find_page_by_full_name(head, "Denis"),
         phone_book_find_page_by_full_name(head, ""),
         phone_book_find_page_by_full_name(head, "123"));

  printf("%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\n",
         phone_book_find_page_by_phone_number(head, "\\+77775557711"),
         phone_book_find_page_by_phone_number(head, "77775557711"),
         phone_book_find_page_by_phone_number(head, "555"),
         phone_book_find_page_by_phone_number(head, "5"),
         phone_book_find_page_by_phone_number(head, "1$"),
         phone_book_find_page_by_phone_number(head, "Denis"),
         phone_book_find_page_by_phone_number(head, ""),
         phone_book_find_page_by_phone_number(head, "123"));

  phone_book_print_page(phone_book_get_page(head, phone_book_find_page_by_full_name(head, "Denis")));
  phone_book_print_page(phone_book_get_page(head, phone_book_find_page_by_full_name(head, "ich")));

  phone_book_remove_page(head, 2);
  phone_book_print(head);

  phone_book_remove_page(head, 0);
  phone_book_print(head);

  phone_book_remove_page(head, 1);
  phone_book_print(head);

  phone_book_free(head);

  return 0;
}