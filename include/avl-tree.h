#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stdlib.h>

/* ============================
   AVL Tree Types
   ============================ */

typedef struct _AvlTreeNode *Tree;

/* AVL tree node */
struct _AvlTreeNode {
    Tree parent;
    Tree left;
    Tree right;
    int balance;   /* Balance factor: left height - right height */
    char data[1];
};

/**
 * Create a new empty AVL tree.
 * Returns NULL.
 */
Tree tree_new();

/**
 * Recursively delete all nodes in the tree.
 * Optionally calls 'delete' on each node's data.
 */
void tree_delete(Tree tree, void (*delete)(void *));

/**
 * Perform a left rotation around the given node.
 * Updates child and parent pointers.
 */
void left_rotate(Tree *tree);

/**
 * Perform a right rotation around the given node.
 * Updates child and parent pointers.
 */
void right_rotate(Tree *tree);

/**
 * Insert data into the AVL tree while maintaining balance.
 * Returns true if insertion succeeds, false if duplicate.
 */
bool tree_insert_sorted(Tree *ptree, const void *data, size_t size,
                        int (*compare)(const void *, const void *));

/**
 * Delete a node containing the given data.
 * Calls rebalance internally to maintain AVL properties.
 * 'delete' function is called on node data if provided.
 */
void node_delete(Tree *ptree, void *data, void (*delete)(void *),
                 int (*compare)(const void *, const void *), size_t size);

/**
 * Rebalance the tree at the given node, adjusting balance factors
 * and performing rotations if necessary.
 */
void rebalance(Tree *ptree);

/**
 * Allocate a new node with the given data.
 * Node balance is initialized to 0 and has no children.
 */
Tree tree_create(const void *data, size_t size);

/* Return left child, right child, or pointer to data */
Tree tree_get_left(Tree tree);
Tree tree_get_right(Tree tree);
void *tree_get_data(Tree tree);

/* Set left child, right child, or node data */
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

/* Return the height of the tree (number of levels) */
size_t tree_height(Tree tree);

/* Return the total number of nodes in the tree */
size_t tree_size(Tree tree);

/**
 * Search for data in the tree using 'compare'.
 * Returns pointer to the data if found, NULL otherwise.
 */
void *tree_search(Tree tree, const void *data,
                  int (*compare)(const void *, const void *));

#endif
