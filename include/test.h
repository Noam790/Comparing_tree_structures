#ifndef TEST_H
#define TEST_H
#define NB_TESTS 16 // 16 values in the test array

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    char word[50];
    char definition[200];
} Hashmap;

/* Structure to store performance results */
typedef struct {
    size_t n;
    double insert_time;  /* Time to insert n elements (seconds) */
    double search_time;  /* Time to search n elements (seconds) */
    double delete_time;  /* Time to delete n elements (seconds) */
} Result;

/**
 * Create an array of unique integers from 0 to size-1.
 * Returns pointer to allocated array (must be freed by caller), or NULL on failure.
 */
int *unique_list(size_t size);

/**
 * Compare two integers.
 * Returns -1 if a < b, 1 if a > b, 0 if equal.
 */
int compare_int(const void *a, const void *b);

/**
 * Compare two Hashmap entries by their 'word' field.
 * Returns strcmp result between a and b.
 */
int compare_dico(const void *a, const void *b);

/**
 * Print an integer to stdout.
 */
void print_int(void *data);

/**
 * Print a Hashmap entry as "word -> definition".
 */
void print_hashmap(void *data);

/* Function type for insertion into a tree */
typedef bool (*InsertFunc)(void *root, const void *data, size_t size,
                           int (*compare)(const void *, const void *));

/* Function type for deletion from a tree */
typedef void (*DeleteFunc)(void *root, void *data, void (*delete)(void *),
                           int (*compare)(const void *, const void *), size_t size);

/* Function type for searching in a tree */
typedef void *(*SearchFunc)(void *root, const void *data,
                            int (*compare)(const void *, const void *));

/**
 * Measure the time to insert 'n' integer values into the tree.
 * 'root' is a pointer to the tree root.
 * 'insert' is the insertion function to test.
 * Returns elapsed time in seconds.
 */
double test_insert_complexity(void **root, int *values, size_t n, InsertFunc insert);

/**
 * Measure the time to delete 'n' integer values from the tree.
 * 'del' is the deletion function to test.
 * Returns elapsed time in seconds.
 */
double test_delete_complexity(void **root, int *values, size_t n, DeleteFunc del);

/**
 * Measure the time to search for 'n' integer values in the tree.
 * 'search' is the search function to test.
 * Returns elapsed time in seconds.
 */
double test_search_complexity(void **root, int *values, size_t n, SearchFunc search);

#endif // TEST_H
