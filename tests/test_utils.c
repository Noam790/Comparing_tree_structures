#include "test.h"

int *unique_list(const size_t size) {
    int *list = malloc(sizeof(int) * size);
    if (!list) return NULL;

    for (size_t i = 0; i < size; i++) {
        list[i] = (int)i;
    }

    return list;
}

int compare_int(const void *a, const void *b) {
    const int va = *(int *)a;
    const int vb = *(int *)b;
    return (va < vb) ? -1 : (va > vb);
}

int compare_dico(const void *a, const void *b) {
    const Hashmap *da = (const Hashmap *)a;
    const Hashmap *db = (const Hashmap *)b;
    return strcmp(da->word, db->word);
}

void print_int(void *data) {
    printf("%d", *(int *)data);
}

void print_hashmap(void *data) {
    Hashmap *d = data;
    printf("%s -> %s", d->word, d->definition);
}

double test_insert_complexity(void **root, int *values, size_t n, InsertFunc insert) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < n; i++) {
        insert(root, &values[i], sizeof(int), compare_int);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) * 1e-9;
}

double test_delete_complexity(void **root, int *values, size_t n, DeleteFunc del) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < n; i++) {
        del(root, &values[i], NULL, compare_int, sizeof(int));
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) * 1e-9;
}

double test_search_complexity(void **root, int *values, size_t n, SearchFunc search) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < n; i++) {
        /* search prend un Tree, donc cast du pointeur contenu dans root */
        search(*root, &values[i], compare_int);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) * 1e-9;
}


