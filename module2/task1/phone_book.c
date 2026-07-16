#include "phone_book.h"

phone_book* phone_book_create() {
  phone_book* head = (phone_book*)malloc(sizeof(phone_book));
  head->next = NULL;

  return head;
}

int phone_book_free(phone_book* head) {
  if (head == NULL) return ERROR;

  phone_book* ptr = head;

  while (ptr != NULL) {
    _free_one_page(ptr);

    phone_book* temp = ptr;
    ptr = ptr->next;
    free(temp);
  }

  return SUCCESS;
}

int phone_book_add_page(phone_book* head, char full_name[], char job_place[],
                        char job_position[], char** phone_numbers,
                        size_t phone_numbers_n, socials_t* socials,
                        size_t socials_n, char other[]) {
  if (head == NULL) return ERROR;

  phone_book* pb = (phone_book*)calloc(1, sizeof(phone_book));
  pb->index = _get_unique_index(head);

  if (phone_book_set_full_name(pb, full_name) && phone_book_set_job_place(pb, job_place) &&
      phone_book_set_job_position(pb, job_position) &&
      phone_book_set_phone_numbers(pb, phone_numbers, phone_numbers_n) &&
      phone_book_set_socials(pb, socials, socials_n) && phone_book_set_other(pb, other) == SUCCESS) {
    pb->next = head->next;
    head->next = pb;
    return SUCCESS;
  }

  _free_one_page(pb);

  return ERROR;
}

int phone_book_remove_page(phone_book* head, size_t index) {
  if (head == NULL) return ERROR;

  phone_book *ptr = head->next, *prev = head;

  while (ptr != NULL) {
    if (ptr->index == index) {
      prev->next = ptr->next;
      ptr->next = NULL;
      _free_one_page(ptr);
      free(ptr);

      return SUCCESS;
    }
    ptr = ptr->next;
    prev = prev->next;
  }

  return ERROR;
}

size_t phone_book_find_page_by_full_name(phone_book* head, char full_name[]) {
  if (head == NULL) return INDEX_NOT_FOUND;

  regex_t regex;
  int reti;

  reti = regcomp(&regex, full_name, REG_EXTENDED | REG_ICASE);
  if (reti) {
    return INDEX_NOT_FOUND;
  }

  phone_book* ptr = head->next;
  while (ptr != NULL) {
    reti = regexec(&regex, ptr->full_name, 0, NULL, 0);
    if (!reti) {
      regfree(&regex);
      return ptr->index;
    }

    ptr = ptr->next;
  }

  regfree(&regex);

  return INDEX_NOT_FOUND;
}

size_t phone_book_find_page_by_phone_number(phone_book* head,
                                            char phone_number[]) {
  if (head == NULL) return INDEX_NOT_FOUND;

  regex_t regex;
  int reti;

  reti = regcomp(&regex, phone_number, REG_EXTENDED);
  if (reti) {
    return INDEX_NOT_FOUND;
  }

  phone_book* ptr = head->next;
  while (ptr != NULL) {
    for (size_t i = 0; i < ptr->phone_numbers_n; i++) {
      reti = regexec(&regex, ptr->phone_numbers[i], 0, NULL, 0);
      if (!reti) {
        regfree(&regex);
        return ptr->index;
      }
    }
    ptr = ptr->next;
  }

  regfree(&regex);

  return INDEX_NOT_FOUND;
}

phone_book* phone_book_get_page(phone_book* head, size_t index) {
  if (head == NULL) return NULL;

  phone_book* ptr = head->next;

  while (ptr != NULL) {
    if (ptr->index == index) return ptr;
    ptr = ptr->next;
  }

  return NULL;
}

int phone_book_set_full_name(phone_book* pb, char full_name[]) {
  if (strlen(full_name) == 0) return ERROR;

  strcpy(pb->full_name, full_name);
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

int phone_book_set_phone_numbers(phone_book* pb, char** phone_numbers,
                      size_t phone_numbers_n) {
  if (pb->phone_numbers != NULL) {
    for (size_t i = 0; i < pb->phone_numbers_n; i++) {
      free(pb->phone_numbers[i]);
    }
    free(pb->phone_numbers);
  }
  pb->phone_numbers_n = phone_numbers_n;
  pb->phone_numbers = (char**)malloc(sizeof(char*) * phone_numbers_n);
  for (size_t i = 0; i < phone_numbers_n; i++) {
    if (!_check_phone_number(phone_numbers[i])) {
      pb->phone_numbers_n = i;
      return ERROR;
    }
    pb->phone_numbers[i] =
        (char*)malloc(sizeof(char) * (strlen(phone_numbers[i]) + 1));
    strcpy(pb->phone_numbers[i], phone_numbers[i]);
  }

  return SUCCESS;
}

int phone_book_set_socials(phone_book* pb, socials_t* socials, size_t socials_n) {
  if (pb->socials != NULL) {
    free(pb->socials);
  }
  pb->socials_n = socials_n;
  pb->socials = (socials_t*)malloc(sizeof(socials_t) * socials_n);
  for (size_t i = 0; i < socials_n; i++) {
    strcpy(pb->socials[i].social_network_name, socials[i].social_network_name);
    strcpy(pb->socials[i].social_network_url, socials[i].social_network_url);
  }

  return SUCCESS;
}

int phone_book_set_other(phone_book* pb, char other[]) {
  strcpy(pb->other, other);
  return SUCCESS;
}

int phone_book_print(phone_book* head) {
  if (head == NULL) return ERROR;

  phone_book* ptr = head->next;

  printf("===============================\n");
  printf("My phone book! \n");

  while (ptr != NULL) {
    phone_book_print_page(ptr);
    ptr = ptr->next;
  }
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
  for (size_t j = 0; j < page->phone_numbers_n; j++) {
    printf("\t%s\n", page->phone_numbers[j]);
  }

  printf("Socials:\n");
  for (size_t j = 0; j < page->socials_n; j++) {
    printf("\t%s: %s\n", page->socials[j].social_network_name,
           page->socials[j].social_network_url);
  }

  printf("Other information:%s%s\n", strlen(page->other) == 0 ? "" : "\n",
         page->other);

  printf("-------------------------------\n");

  return SUCCESS;
}

int phone_book_save(phone_book* head, char file_name[]) {
  FILE *file = fopen(file_name, "w");

  if (file == NULL) {
    return ERROR;
  }
  phone_book* ptr = head->next;
  phone_book** all_pages = NULL;
  size_t n = 0;
  while (ptr != NULL) {
    n++;
    phone_book** temp = (phone_book**)realloc(all_pages, n * sizeof(phone_book*));
    if (temp == NULL) {
      free(all_pages);
      fclose(file);
      return ERROR;
    }

    all_pages = temp;
    all_pages[n - 1] = ptr;

    ptr = ptr->next;
  }

  fprintf(file, "phone_book\n");
  for (size_t i = n; i > 0; i--) {
    fprintf(file, "---\n");

    fprintf(file, "%ld\n", all_pages[i - 1]->index);
    fprintf(file, "%s\n", all_pages[i - 1]->full_name);
    fprintf(file, "%s\n", all_pages[i - 1]->job_place);
    fprintf(file, "%s\n", all_pages[i - 1]->job_position);

    fprintf(file, "numbers:\n");
    fprintf(file, "%ld\n", all_pages[i - 1]->phone_numbers_n);
    for (size_t j = 0; j < all_pages[i - 1]->phone_numbers_n; j++)
      fprintf(file, "%s\n", all_pages[i - 1]->phone_numbers[j]);
    
    fprintf(file, "socials:\n");
    fprintf(file, "%ld\n", all_pages[i - 1]->socials_n);
    for (size_t j = 0; j < all_pages[i - 1]->socials_n; j++)
      fprintf(file, "%s\n%s\n", all_pages[i - 1]->socials[j].social_network_name, all_pages[i - 1]->socials[j].social_network_url);

    fprintf(file, "%s\n", all_pages[i - 1]->other);

    fprintf(file, "---\n");
  }

  free(all_pages);

  fclose(file);

  return SUCCESS;
}

#define LOAD_ERROR_1(file) \
do { \
  printf("error1\n"); \
  fclose(file); \
  return NULL; \
} while(0)

#define LOAD_ERROR_2(file, head) \
do { \
  fclose(file); \
  printf("error2\n"); \
  phone_book_free(head); \
  return NULL; \
} while(0)

#define LOAD_ERROR_3(file, head, phone_numbers, i) \
do { \
  fclose(file); \
  printf("error3\n"); \
  phone_book_free(head); \
  for (size_t j = 0; j < i; j++) { \
    free(phone_numbers[j]); \
  } \
  free(phone_numbers); \
  return NULL; \
} while(0)

#define LOAD_ERROR_4(file, head, phone_numbers, i, socials) \
do { \
  fclose(file); \
  printf("error4\n"); \
  phone_book_free(head); \
  for (size_t j = 0; j < i; j++) { \
    free(phone_numbers[j]); \
  } \
  free(phone_numbers); \
  free(socials); \
  return NULL; \
} while(0)

phone_book* phone_book_load(char file_name[]) {
  FILE *file = fopen(file_name, "r");

  if (file == NULL) {
    return NULL;
  }

  char header[32];
  if (fgets(header, sizeof(header), file) == NULL) LOAD_ERROR_1(file);
  header[strlen(header) - 1] = '\0';
  if (strcmp(header, "phone_book") != 0) LOAD_ERROR_1(file);
  
  phone_book* head = phone_book_create();
  if (head == NULL) {
    fclose(file);
    return NULL;
  }

  char hyphens[16];
  char line[512];
  
  while (fgets(hyphens, sizeof(hyphens), file) != NULL) {
    hyphens[strlen(hyphens) - 1] = '\0';
    
    if (strcmp(hyphens, "---") != 0) break;
    
    char full_name[128];
    char job_place[128];
    char job_position[128];
    char** phone_numbers = NULL;
    size_t phone_numbers_n = 0;
    socials_t* socials = NULL;
    size_t socials_n = 0;
    char other[512];

    if (fgets(line, sizeof(line), file) == NULL) LOAD_ERROR_2(file, head);

    if (fgets(full_name, sizeof(full_name), file) == NULL) LOAD_ERROR_2(file, head);
    full_name[strlen(full_name) - 1] = '\0';
    
    if (fgets(job_place, sizeof(job_place), file) == NULL) LOAD_ERROR_2(file, head);
    job_place[strlen(job_place) - 1] = '\0';
    
    if (fgets(job_position, sizeof(job_position), file) == NULL) LOAD_ERROR_2(file, head);
    job_position[strlen(job_position) - 1] = '\0';

    if (fgets(line, sizeof(line), file) == NULL) LOAD_ERROR_2(file, head);
    line[strlen(line) - 1] = '\0';
    if (strcmp(line, "numbers:") != 0) LOAD_ERROR_2(file, head);
    
    if (fgets(line, sizeof(line), file) == NULL) LOAD_ERROR_2(file, head);
    phone_numbers_n = (size_t)strtoull(line, NULL, 10);

    if (phone_numbers_n > 0) {
      phone_numbers = (char**)malloc(sizeof(char*) * phone_numbers_n);
      if (phone_numbers == NULL) LOAD_ERROR_2(file, head);
      
      for (size_t i = 0; i < phone_numbers_n; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
          LOAD_ERROR_3(file, head, phone_numbers, i);
        }
        line[strlen(line) - 1] = '\0';
        
        phone_numbers[i] = (char*)malloc(strlen(line) + 1);
        if (phone_numbers[i] == NULL) {
          LOAD_ERROR_3(file, head, phone_numbers, i);
        }
        strcpy(phone_numbers[i], line);
      }
    }

    if (fgets(line, sizeof(line), file) == NULL) {
      LOAD_ERROR_3(file, head, phone_numbers, phone_numbers_n);
    }
    line[strlen(line) - 1] = '\0';
    if (strcmp(line, "socials:") != 0) {
      LOAD_ERROR_3(file, head, phone_numbers, phone_numbers_n);
    }
    
    if (fgets(line, sizeof(line), file) == NULL) {
      LOAD_ERROR_3(file, head, phone_numbers, phone_numbers_n);
    }
    socials_n = (size_t)strtoull(line, NULL, 10);

    if (socials_n > 0) {
      socials = (socials_t*)malloc(sizeof(socials_t) * socials_n);
      if (socials == NULL) {
        LOAD_ERROR_3(file, head, phone_numbers, phone_numbers_n);
      }
      
      for (size_t i = 0; i < socials_n; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
          LOAD_ERROR_4(file, head, phone_numbers, phone_numbers_n, socials);
        }
        line[strlen(line) - 1] = '\0';
        strcpy(socials[i].social_network_name, line);

        if (fgets(line, sizeof(line), file) == NULL) {
          LOAD_ERROR_4(file, head, phone_numbers, phone_numbers_n, socials);
        }
        line[strlen(line) - 1] = '\0';
        strcpy(socials[i].social_network_url, line);
      }
    }

    if (fgets(other, sizeof(other), file) == NULL) {
      LOAD_ERROR_4(file, head, phone_numbers, phone_numbers_n, socials);
    }
    other[strlen(other) - 1] = '\0';

    if (phone_book_add_page(head, full_name, job_place, job_position, 
                           phone_numbers, phone_numbers_n, socials, socials_n, other) != SUCCESS) {
      if (phone_numbers != NULL) {
        for (size_t i = 0; i < phone_numbers_n; i++) free(phone_numbers[i]);
        free(phone_numbers);
      }
      if (socials != NULL) free(socials);
      LOAD_ERROR_2(file, head);
    }

    if (phone_numbers != NULL) {
      for (size_t i = 0; i < phone_numbers_n; i++) free(phone_numbers[i]);
      free(phone_numbers);
    }
    if (socials != NULL) free(socials);

    if (fgets(line, sizeof(line), file) == NULL) LOAD_ERROR_2(file, head);
    line[strlen(line) - 1] = '\0';
    if (strcmp(line, "---") != 0) LOAD_ERROR_2(file, head);
  }

  fclose(file);
  return head;
}

int phone_book_compare_pages(phone_book* pb1, phone_book* pb2) {
  if (pb1 == NULL || pb2 == NULL) return 0;
  int result;

  result = !strcmp(pb1->full_name, pb2->full_name) && !strcmp(pb1->job_place, pb2->job_place) && !strcmp(pb1->job_position, pb2->job_position) && !strcmp(pb1->other, pb2->other) && pb1->phone_numbers_n == pb2->phone_numbers_n && pb1->socials_n == pb2->socials_n;

  for (size_t i = 0; i < pb1->phone_numbers_n && result; i++) {
    result &= !strcmp(pb1->phone_numbers[i], pb2->phone_numbers[i]);
  }
  for (size_t i = 0; i < pb1->socials_n && result; i++) {
    result &= !strcmp(pb1->socials[i].social_network_name, pb2->socials[i].social_network_name);
    result &= !strcmp(pb1->socials[i].social_network_url, pb2->socials[i].social_network_url);
  }

  return result;
}

int phone_book_compare(phone_book* head1, phone_book* head2) {
  if (head1 == NULL || head2 == NULL) return 0;
  int result = 1;
  phone_book* ptr1 = head1->next, *ptr2 = head2->next;

  while (result && (ptr1 != NULL || ptr2 != NULL)) {
    result &= phone_book_compare_pages(ptr1, ptr2);

    ptr1 = ptr1->next;
    ptr2 = ptr2->next;
  }
  result &= (ptr1 == NULL && ptr2 == NULL);

  return result;
}