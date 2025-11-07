#ifndef TEST_H
#define TEST_H
#define NB_TESTS 15

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    char word[50];
    char definition[200];
} Hashmap;

typedef struct {
    size_t n;
    double insert_time;
    double search_time;
    double delete_time;
} Result;

int *unique_list(size_t size);
int compare_int(const void *a, const void *b);
int compare_dico(const void *a, const void *b);

void print_int(void *data);
void print_hashmap(void *data);

typedef bool (*InsertFunc)(void *root, const void *data, size_t size,
                           int (*compare)(const void *, const void *));
typedef void (*DeleteFunc)(void *root, void *data, void (*delete)(void *),
                           int (*compare)(const void *, const void *), size_t size);
typedef void *(*SearchFunc)(void *root, const void *data,
                            int (*compare)(const void *, const void *));

double test_insert_complexity(void *root, int *values, size_t n, InsertFunc insert);
double test_delete_complexity(void *root, int *values, size_t n, DeleteFunc del);
double test_search_complexity(void *root, int *values, size_t n, SearchFunc search);

#endif // TEST_H
