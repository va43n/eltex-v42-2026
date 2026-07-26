#include "phone_book.h"

// static int _balancing_in_progress = 0;

phone_book* phone_book_create() {
  phone_book* head = (phone_book*)calloc(1, sizeof(phone_book));
  head->index = 0;
  head->left = head->right = NULL;
  return head;
}

int phone_book_free(phone_book* head) {
  if (head == NULL) return ERROR;

  _free_page_from_tree(head->left);
  free(head);

  return SUCCESS;
}

int phone_book_add_page(phone_book* head, char full_name[], char job_place[],
                        char job_position[], char** numbers, size_t numbers_n,
                        socials_t* socials, size_t socials_n, char other[]) {
  if (head == NULL) return ERROR;

  size_t index = _get_unique_index(head);
  if (index == INDEX_NOT_FOUND) return ERROR;

  phone_book* pb = (phone_book*)calloc(1, sizeof(phone_book));
  pb->index = index;

  if (_set_full_name(pb, full_name) &&
      phone_book_set_job_place(pb, job_place) &&
      phone_book_set_job_position(pb, job_position) &&
      phone_book_set_numbers(pb, numbers, numbers_n) &&
      phone_book_set_socials(pb, socials, socials_n) &&
      phone_book_set_other(pb, other) == SUCCESS) {
    if (!_add_page_to_tree(&(head->left), pb)) {
      _free_one_page(pb);
      free(pb);
      return ERROR;
    }

    // if (!_balancing_in_progress) {
    //   static size_t function_used = 0;
    //   function_used++;
    //   if (function_used == BALANCING_NUMBER) {
    //     function_used = 0;
    //     _balancing_in_progress = 1;
    //     phone_book_balance(&head);
    //     _balancing_in_progress = 0;
    //   }
    // }
    return SUCCESS;
  }

  _free_one_page(pb);
  free(pb);

  return ERROR;
}

int phone_book_remove_page(phone_book* head, size_t index) {
  // static size_t function_used = 0;
  if (head == NULL || head->left == NULL) return ERROR;

  phone_book* ptr = head->left;
  head->left = _delete_page_from_tree(ptr, index);

  if (_find_page_in_tree(head->left, index) != NULL) return ERROR;
  // function_used++;
  // if (function_used == BALANCING_NUMBER) {
  //   function_used = 0;
  //   phone_book_balance(&head);
  // }
  return SUCCESS;
}

size_t phone_book_find_page_by_full_name(phone_book* head, char full_name[]) {
  if (head == NULL || head->left == NULL) return INDEX_NOT_FOUND;

  regex_t regex;
  if (regcomp(&regex, full_name, REG_EXTENDED | REG_ICASE) != 0)
    return INDEX_NOT_FOUND;

  size_t result = INDEX_NOT_FOUND;

  _find_page_by_full_name_in_tree(head->left, regex, &result);
  regfree(&regex);

  return result;
}

size_t phone_book_find_page_by_number(phone_book* head, char number[]) {
  if (head == NULL || head->left == NULL) return INDEX_NOT_FOUND;

  regex_t regex;
  if (regcomp(&regex, number, REG_EXTENDED) != 0) return INDEX_NOT_FOUND;

  size_t result = INDEX_NOT_FOUND;

  _find_page_by_number_in_tree(head->left, regex, &result);
  regfree(&regex);
  return result;
}

phone_book* phone_book_get_page(phone_book* head, size_t index) {
  if (head == NULL) return NULL;
  return _find_page_in_tree(head->left, index);
}

int phone_book_set_full_name(phone_book* head, size_t index, char full_name[]) {
  if (strlen(full_name) == 0) return ERROR;

  phone_book* pb = phone_book_get_page(head, index);
  if (pb == NULL) return ERROR;

  if (!phone_book_add_page(head, full_name, pb->job_place, pb->job_position,
                           pb->numbers, pb->numbers_n, pb->socials,
                           pb->socials_n, pb->other))
    return ERROR;

  phone_book* new_pb = phone_book_get_page(
      head, phone_book_find_page_by_full_name(head, full_name));

  if (!phone_book_remove_page(head, index)) return ERROR;

  new_pb->index = index;

  return SUCCESS;
}

int phone_book_set_job_place(phone_book* pb, char job_place[]) {
  strcpy(pb->job_place, job_place);
  return SUCCESS;
}

int phone_book_set_job_position(phone_book* pb, char job_position[]) {
  strcpy(pb->job_position, job_position);
  return SUCCESS;
}

int phone_book_set_numbers(phone_book* pb, char** numbers, size_t numbers_n) {
  for (size_t i = 0; i < numbers_n; i++) {
    if (!_check_number(numbers[i]) || strlen(numbers[i]) == 0) return ERROR;
  }

  if (pb->numbers != NULL) {
    for (size_t i = 0; i < pb->numbers_n; i++) {
      free(pb->numbers[i]);
    }
    free(pb->numbers);
  }
  pb->numbers_n = numbers_n;
  pb->numbers = (char**)malloc(sizeof(char*) * numbers_n);
  for (size_t i = 0; i < numbers_n; i++) {
    pb->numbers[i] = (char*)malloc(sizeof(char) * (strlen(numbers[i]) + 1));
    strcpy(pb->numbers[i], numbers[i]);
  }

  return SUCCESS;
}

int phone_book_set_socials(phone_book* pb, socials_t* socials,
                           size_t socials_n) {
  for (size_t i = 0; i < socials_n; i++) {
    if (strlen(socials[i].social_name) == 0 ||
        strlen(socials[i].social_url) == 0)
      return ERROR;
  }

  if (pb->socials != NULL) {
    free(pb->socials);
  }

  pb->socials_n = socials_n;
  pb->socials = (socials_t*)malloc(sizeof(socials_t) * socials_n);
  for (size_t i = 0; i < socials_n; i++) {
    strcpy(pb->socials[i].social_name, socials[i].social_name);
    strcpy(pb->socials[i].social_url, socials[i].social_url);
  }

  return SUCCESS;
}

int phone_book_set_other(phone_book* pb, char other[]) {
  strcpy(pb->other, other);
  return SUCCESS;
}

int phone_book_print(phone_book* head) {
  if (head == NULL) return ERROR;

  printf("===============================\n");
  printf("My phone book! \n");

  _print_tree_infix(head->left);
  printf("===============================\n\n");

  return SUCCESS;
}

int phone_book_print_page(phone_book* page) {
  if (page == NULL) return ERROR;

  printf("\n-------------------------------\n");
  printf("Unique index: %ld\n", page->index);
  printf("Full name: %s\n", page->full_name);
  printf("Job place: %s\n", page->job_place);
  printf("Job position: %s\n", page->job_position);

  printf("Phone numbers:\n");
  for (size_t j = 0; j < page->numbers_n; j++) {
    printf("\t%s\n", page->numbers[j]);
  }

  printf("Socials:\n");
  for (size_t j = 0; j < page->socials_n; j++) {
    printf("\t%s: %s\n", page->socials[j].social_name,
           page->socials[j].social_url);
  }

  printf("Other information:%s%s\n", strlen(page->other) == 0 ? "" : "\n",
         page->other);

  printf("-------------------------------\n");

  return SUCCESS;
}

int phone_book_print_tree(phone_book* head) {
  if (head == NULL) return ERROR;
  printf("===============================\n");
  printf("My phone book (tree style)! \n");
  _print_tree_like_tree(head->left, 0);
  printf("===============================\n\n");
  return SUCCESS;
}

int phone_book_compare_pages(phone_book* pb1, phone_book* pb2) {
  if (pb1 == NULL || pb2 == NULL) return 0;
  int result;

  result = !strcmp(pb1->full_name, pb2->full_name) &&
           !strcmp(pb1->job_place, pb2->job_place) &&
           !strcmp(pb1->job_position, pb2->job_position) &&
           !strcmp(pb1->other, pb2->other) &&
           pb1->numbers_n == pb2->numbers_n && pb1->socials_n == pb2->socials_n;

  for (size_t i = 0; i < pb1->numbers_n && result; i++) {
    result &= !strcmp(pb1->numbers[i], pb2->numbers[i]);
  }
  for (size_t i = 0; i < pb1->socials_n && result; i++) {
    result &= !strcmp(pb1->socials[i].social_name, pb2->socials[i].social_name);
    result &= !strcmp(pb1->socials[i].social_url, pb2->socials[i].social_url);
  }

  return result;
}

int phone_book_compare(phone_book* head1, phone_book* head2) {
  if (head1 == NULL || head2 == NULL) return 0;

  int result = 1;

  if (head1->left != NULL && head2->left != NULL)
    result &= phone_book_compare(head1->left, head2->left);
  result &= phone_book_compare_pages(head1, head2);
  if (head1->right != NULL && head2->right != NULL)
    result &= phone_book_compare(head1->right, head2->right);

  return result;
}

int phone_book_edit(phone_book* head, size_t index, unsigned int format, ...) {
  phone_book* pb = phone_book_get_page(head, index);
  if (pb == NULL) return ERROR;
  va_list va;
  int ret;

  va_start(va, format);

  if ((format & EDIT_FULL_NAME) == EDIT_FULL_NAME) {
    char* full_name = va_arg(va, char*);
    ret = phone_book_set_full_name(head, index, full_name);
    pb = phone_book_get_page(head, index);
    if (ret == ERROR) return ret;
  }
  if ((format & EDIT_JOB_PLACE) == EDIT_JOB_PLACE) {
    char* job_place = va_arg(va, char*);
    ret = phone_book_set_job_place(pb, job_place);
    if (ret == ERROR) return ret;
  }
  if ((format & EDIT_JOB_POSITION) == EDIT_JOB_POSITION) {
    char* job_position = va_arg(va, char*);
    ret = phone_book_set_job_position(pb, job_position);
    if (ret == ERROR) return ret;
  }
  if ((format & EDIT_NUMBERS) == EDIT_NUMBERS) {
    int is_adding = format & EDIT_NUMBERS_ADD;
    if (is_adding) {
      char* number = va_arg(va, char*);
      size_t new_numbers_n = pb->numbers_n + 1;
      char** new_numbers = (char**)malloc(sizeof(char*) * new_numbers_n);
      for (size_t i = 0; i < new_numbers_n - 1; i++) {
        new_numbers[i] = (char*)malloc(sizeof(char) * strlen(pb->numbers[i]));
        strcpy(new_numbers[i], pb->numbers[i]);
      }
      new_numbers[new_numbers_n - 1] =
          (char*)malloc(sizeof(char) * strlen(number));
      strcpy(new_numbers[new_numbers_n - 1], number);

      ret = phone_book_set_numbers(pb, new_numbers, new_numbers_n);
      for (size_t i = 0; i < new_numbers_n; i++) free(new_numbers[i]);
      free(new_numbers);
      if (ret == ERROR) return ret;
    } else {
      size_t pos = va_arg(va, size_t);
      size_t new_numbers_n = pb->numbers_n - 1;
      if (pos > new_numbers_n) return ERROR;

      char** new_numbers = (char**)malloc(sizeof(char*) * new_numbers_n);
      for (size_t i = 0; i < pos; i++) {
        new_numbers[i] = (char*)malloc(sizeof(char) * strlen(pb->numbers[i]));
        strcpy(new_numbers[i], pb->numbers[i]);
      }
      for (size_t i = pos; i < new_numbers_n; i++) {
        new_numbers[i] = (char*)malloc(sizeof(char) * strlen(pb->numbers[i]));
        strcpy(new_numbers[i], pb->numbers[i + 1]);
      }

      ret = phone_book_set_numbers(pb, new_numbers, new_numbers_n);
      for (size_t i = 0; i < new_numbers_n; i++) free(new_numbers[i]);
      free(new_numbers);
      if (ret == ERROR) return ret;
    }
  }
  if ((format & EDIT_SOCIALS) == EDIT_SOCIALS) {
    int is_adding = format & EDIT_SOCIALS_ADD;

    if (is_adding) {
      socials_t social = va_arg(va, socials_t);
      size_t new_socials_n = pb->socials_n + 1;

      socials_t* new_socials =
          (socials_t*)malloc(sizeof(socials_t) * new_socials_n);
      for (size_t i = 0; i < new_socials_n - 1; i++) {
        strcpy(new_socials[i].social_name, pb->socials[i].social_name);
        strcpy(new_socials[i].social_url, pb->socials[i].social_url);
      }
      strcpy(new_socials[new_socials_n - 1].social_name, social.social_name);
      strcpy(new_socials[new_socials_n - 1].social_url, social.social_url);

      ret = phone_book_set_socials(pb, new_socials, new_socials_n);
      free(new_socials);
      if (ret == ERROR) return ret;
    } else {
      size_t pos = va_arg(va, size_t);
      size_t new_socials_n = pb->socials_n - 1;
      if (pos > new_socials_n) return ERROR;

      socials_t* new_socials =
          (socials_t*)malloc(sizeof(socials_t) * new_socials_n);
      for (size_t i = 0; i < pos; i++) {
        strcpy(new_socials[i].social_name, pb->socials[i].social_name);
        strcpy(new_socials[i].social_url, pb->socials[i].social_url);
      }
      for (size_t i = pos; i < new_socials_n; i++) {
        strcpy(new_socials[i].social_name, pb->socials[i + 1].social_name);
        strcpy(new_socials[i].social_url, pb->socials[i + 1].social_url);
      }

      ret = phone_book_set_socials(pb, new_socials, new_socials_n);
      free(new_socials);
      if (ret == ERROR) return ret;
    }
  }
  if ((format & EDIT_OTHER) == EDIT_OTHER) {
    char* other = va_arg(va, char*);
    ret = phone_book_set_other(pb, other);
    if (ret == ERROR) return ret;
  }

  va_end(va);

  return SUCCESS;
}

int phone_book_balance(phone_book** head) {
  if (head == NULL || (*head) == NULL) return ERROR;
  if ((*head)->left == NULL) return SUCCESS;

  phone_book* pbs = NULL;
  size_t pbs_n = 0;

  _get_all_pages_from_tree((*head)->left, &pbs, &pbs_n);

  phone_book_free(*head);
  *head = phone_book_create();

  _add_pages_in_balanced_order(head, pbs, 0, pbs_n - 1);

  for (size_t i = 0; i < pbs_n; i++) _free_one_page(&pbs[i]);

  return SUCCESS;
}