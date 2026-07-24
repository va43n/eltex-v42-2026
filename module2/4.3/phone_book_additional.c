#include "phone_book.h"

int _size_t_sort(const void* a, const void* b) {
  int val_a = *(size_t*)a;
  int val_b = *(size_t*)b;

  if (val_a < val_b) return -1;
  if (val_a > val_b) return 1;
  return 0;
}

int _check_all_indices(phone_book* head, size_t* n, size_t** indices) {
  if (head == NULL) return SUCCESS;

  if (!_check_all_indices(head->left, n, indices)) return ERROR;

  (*n)++;
  size_t* temp = realloc(*indices, sizeof(size_t) * (*n));
  *indices = temp;
  (*indices)[(*n) - 1] = head->index;

  if (!_check_all_indices(head->right, n, indices)) return ERROR;

  return SUCCESS;
}

size_t _get_unique_index(phone_book* head) {
  if (head == NULL) return ERROR;
  if (head->left == NULL) return FIRST_UNIQUE_INDEX;

  phone_book* ptr = head->left;
  size_t* indices = NULL;
  size_t n = 0;

  if (!_check_all_indices(ptr, &n, &indices)) return INDEX_NOT_FOUND;

  qsort(indices, n, sizeof(size_t), _size_t_sort);
  for (size_t i = 1; i < n; i++) {
    if (indices[i] - 1 > indices[i - 1]) {
      n = indices[i - 1] + 1;
      free(indices);
      return n;
    }
  }

  n = indices[n - 1] + 1;
  free(indices);

  return n;
}

int _add_page_to_tree(phone_book** head, phone_book* pb) {
  if (*head == NULL) {
    *head = pb;
    return SUCCESS;
  }

  if (strcmp(pb->full_name, (*head)->full_name) < 0)
    return _add_page_to_tree(&((*head)->left), pb);
  else
    return _add_page_to_tree(&((*head)->right), pb);
}

phone_book* _find_page_in_tree(phone_book* pb, size_t index) {
  if (pb == NULL) return NULL;
  if (pb->index == index) return pb;

  phone_book* res = _find_page_in_tree(pb->left, index);
  if (res != NULL) return res;
  return _find_page_in_tree(pb->right, index);
}

phone_book* _find_min_page(phone_book** pb) {
  if (*pb == NULL) return NULL;
  if ((*pb)->left == NULL) {
    phone_book* ptr = *pb;
    *pb = ptr->right;
    ptr->right = NULL;
    return ptr;
  }
  return _find_min_page(&((*pb)->left));
}

phone_book* _find_max_page(phone_book* pb) {
  if (pb == NULL) return NULL;
  while (pb->right != NULL) pb = pb->right;
  return pb;
}

int _free_page_from_tree(phone_book* pb) {
  if (pb == NULL) return ERROR;
  _free_page_from_tree(pb->left);
  _free_page_from_tree(pb->right);
  _free_one_page(pb);
  free(pb);

  return SUCCESS;
}

phone_book* _delete_page_from_tree(phone_book* pb, size_t index) {
  if (pb == NULL) return NULL;

  if (pb->index == index) {
    if (pb->left == NULL) {
      phone_book* right = pb->right;
      _free_one_page(pb);
      free(pb);
      return right;
    }
    if (pb->right == NULL) {
      phone_book* left = pb->left;
      _free_one_page(pb);
      free(pb);
      return left;
    }

    phone_book* new_root = _find_min_page(&(pb->right));

    phone_book* left_subtree = pb->left;
    phone_book* right_subtree = pb->right;

    _free_one_page(pb);
    free(pb);

    new_root->left = left_subtree;
    new_root->right = right_subtree;
    return new_root;
  }

  pb->left = _delete_page_from_tree(pb->left, index);
  pb->right = _delete_page_from_tree(pb->right, index);
  return pb;
}

void _free_one_page(phone_book* pb) {
  if (pb->numbers != NULL) {
    for (size_t i = 0; i < pb->numbers_n; i++) free(pb->numbers[i]);
    free(pb->numbers);
  }
  if (pb->socials != NULL) free(pb->socials);
}

int _check_number(char* pn) {
  regex_t regex;
  int reti;

  reti = regcomp(&regex, "^\\+?[0-9\\-]+$", REG_EXTENDED);
  if (reti) {
    return ERROR;
  }

  reti = regexec(&regex, pn, 0, NULL, 0);

  regfree(&regex);

  return !reti;
}

int _set_full_name(phone_book* pb, char full_name[]) {
  if (strlen(full_name) == 0) return ERROR;

  strcpy(pb->full_name, full_name);
  return SUCCESS;
}

int _find_page_by_full_name_in_tree(phone_book* pb, regex_t regex,
                                    size_t* result) {
  if (pb == NULL || *result != INDEX_NOT_FOUND) return ERROR;
  _find_page_by_full_name_in_tree(pb->left, regex, result);
  if (regexec(&regex, pb->full_name, 0, NULL, 0) == 0) {
    *result = pb->index;
    return ERROR;
  }
  _find_page_by_full_name_in_tree(pb->right, regex, result);

  return SUCCESS;
}

int _find_page_by_number_in_tree(phone_book* pb, regex_t regex,
                                 size_t* result) {
  if (pb == NULL || *result != INDEX_NOT_FOUND) return ERROR;
  _find_page_by_number_in_tree(pb->left, regex, result);
  for (size_t i = 0; i < pb->numbers_n; i++) {
    if (regexec(&regex, pb->numbers[i], 0, NULL, 0) == 0) {
      *result = pb->index;
      break;
    }
  }

  if (*result == INDEX_NOT_FOUND)
    _find_page_by_number_in_tree(pb->right, regex, result);

  return SUCCESS;
}

int _print_tree_infix(phone_book* pb) {
  if (pb == NULL) return ERROR;
  _print_tree_infix(pb->left);
  phone_book_print_page(pb);
  _print_tree_infix(pb->right);

  return SUCCESS;
}

void _print_tree_like_tree(phone_book* pb, int depth) {
    if (pb == NULL) return;
    _print_tree_like_tree(pb->right, depth + 1);
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    printf("'%-8.8s' (i=%ld)\n", pb->full_name, pb->index);
    _print_tree_like_tree(pb->left, depth + 1);
}

size_t _get_depth_of_tree(phone_book* head) {
  if (head == NULL) return 0;

  size_t left = _get_depth_of_tree(head->left);
  size_t right = _get_depth_of_tree(head->left);
  
  return left > right ? left + 1 : right + 1;
}

void _get_all_pages_from_tree(phone_book* head, phone_book** pbs, size_t* pbs_n) {
  if (head != NULL) {
    _get_all_pages_from_tree(head->left, pbs, pbs_n);
    (*pbs_n)++;
    phone_book* temp = (phone_book*)realloc(*pbs, *pbs_n * sizeof(phone_book));
    *pbs = temp;
    strcpy((*pbs)[*pbs_n - 1].full_name, head->full_name);
    strcpy((*pbs)[*pbs_n - 1].job_place, head->job_place);
    strcpy((*pbs)[*pbs_n - 1].job_position, head->job_position);
    strcpy((*pbs)[*pbs_n - 1].other, head->other);
    phone_book_set_numbers(&(*pbs)[*pbs_n - 1], head->numbers, head->numbers_n);
    phone_book_set_socials(&(*pbs)[*pbs_n - 1], head->socials, head->socials_n);

    _get_all_pages_from_tree(head->right, pbs, pbs_n);
  }
}

int _add_pages_in_balanced_order(phone_book** head, phone_book* pbs, size_t start, size_t end) {
  if (end < start) return 0;
  size_t mid = (end - start) / 2 + start;
  printf("%s: %ld %ld %ld\n", pbs[mid].full_name, mid, start, end);
  phone_book_add_page(*head, pbs[mid].full_name, pbs[mid].job_place, pbs[mid].job_position, pbs[mid].numbers, pbs[mid].numbers_n, pbs[mid].socials, pbs[mid].socials_n, pbs[mid].other);
  if (end == start || end == start + 1) return 0;
  _add_pages_in_balanced_order(head, pbs, start, mid - 1);
  _add_pages_in_balanced_order(head, pbs, mid + 1, end);

  return 0;
}