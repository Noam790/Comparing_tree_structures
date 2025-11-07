#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct _BicolorTreeNode *Tree;

typedef enum {
  RED,
  BLACK,
} Color;

struct _BicolorTreeNode {
  Tree parent;
  Tree left;
  Tree right;
  Color color;
  char data[1];
};

Tree tree_new();

void tree_delete(Tree tree, void (*delete)(void *));

/* x = pivot */
void left_rotate(Tree *root, Tree x);
void right_rotate(Tree *root, Tree x);

Tree tree_create(const void *data, size_t size);

bool tree_insert_sorted(Tree *root, const void *data, size_t size,
                        int (*compare)(const void *, const void *));

void node_delete(Tree *root, void *data, void (*delete_func)(void *),
                 int (*compare)(const void *, const void *), size_t size);

Tree tree_get_left(Tree tree);
Tree tree_get_right(Tree tree);
void *tree_get_data(Tree tree);
bool tree_set_left(Tree tree, Tree left);
bool tree_set_right(Tree tree, Tree right);
bool tree_set_data(Tree tree, const void *data, size_t size);
void tree_pre_order(Tree tree, void (*func)(void *, void *), void *extra_data);
void tree_in_order(Tree tree, void (*func)(void *, void *), void *extra_data);
void tree_post_order(Tree tree, void (*func)(void *, void *), void *extra_data);
size_t tree_height(Tree tree);
size_t tree_size(Tree tree);
void *tree_search(Tree tree, const void *data,
                  int (*compare)(const void *, const void *));
int tree_sort(void *array, size_t length, size_t size,
              int (*compare)(const void *, const void *));

#endif