#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stdlib.h>

/* ============================
   Red-Black Tree Types
   ============================ */

typedef struct _BicolorTreeNode *Tree;

typedef enum {
    RED,
    BLACK,
} Color;

/* Red-Black tree node */
struct _BicolorTreeNode {
    Tree parent;
    Tree left;
    Tree right;
    Color color;
    char data[1];
};


/**
 * Create a new empty tree.
 * Returns NULL.
 */
Tree tree_new();

/**
 * Recursively delete all nodes in the tree.
 * Optionally calls 'delete' on each node's data.
 */
void tree_delete(Tree tree, void (*delete)(void *));

/**
 * Perform a left rotation around node x.
 * Updates parent and child pointers.
 */
void left_rotate(Tree *root, Tree x);

/**
 * Perform a right rotation around node x.
 * Updates parent and child pointers.
 */
void right_rotate(Tree *root, Tree x);

/**
 * Allocate a new tree node with given data.
 * The node is initialized as RED with no children.
 */
Tree tree_create(const void *data, size_t size);

/**
 * Insert data into the tree while maintaining red-black properties.
 * Returns true if insertion succeeds, false if duplicate.
 */
bool tree_insert_sorted(Tree *root, const void *data, size_t size,
                        int (*compare)(const void *, const void *));

/**
 * Delete a node containing the given data.
 * Calls delete_fixup internally if necessary.
 * 'delete_func' is called on the node's data if provided.
 */
void node_delete(Tree *root, void *data, void (*delete_func)(void *),
                 int (*compare)(const void *, const void *), size_t size);

/* Return left child, right child, or data pointer */
Tree tree_get_left(Tree tree);
Tree tree_get_right(Tree tree);
void *tree_get_data(Tree tree);

/* Set left child, right child, or data */
bool tree_set_left(Tree tree, Tree left);
bool tree_set_right(Tree tree, Tree right);
bool tree_set_data(Tree tree, const void *data, size_t size);

/**
 * Apply 'func' to each node in pre-order.
 * 'extra_data' can be used as context.
 */
void tree_pre_order(Tree tree, void (*func)(void *, void *), void *extra_data);

/**
 * Apply 'func' to each node in in-order.
 */
void tree_in_order(Tree tree, void (*func)(void *, void *), void *extra_data);

/**
 * Apply 'func' to each node in post-order.
 */
void tree_post_order(Tree tree, void (*func)(void *, void *), void *extra_data);

/* Return tree height (number of levels) */
size_t tree_height(Tree tree);

/* Return total number of nodes in the tree */
size_t tree_size(Tree tree);

/**
 * Search for data in the tree using 'compare'.
 * Returns pointer to the data if found, NULL otherwise.
 */
void *tree_search(Tree tree, const void *data,
                  int (*compare)(const void *, const void *));

/**
 * Sort an array using a red-black tree.
 * Returns true on success, false if duplicate insertion fails.
 */
int tree_sort(void *array, size_t length, size_t size,
              int (*compare)(const void *, const void *));

#endif
