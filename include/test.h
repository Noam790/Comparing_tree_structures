#ifndef TEST_H
#define TEST_H

#include <stddef.h>

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

#endif // TEST_H
