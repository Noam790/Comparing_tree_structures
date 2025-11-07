#include "bicolor-tree.h"
#include <stdbool.h>
#include <string.h>

// Add method to get grand parent / uncle / min tree

static Tree get_grandparent(Tree n) {
  return (n && n->parent) ? n->parent->parent : NULL;
}

static Tree get_uncle(Tree n) {
  Tree g = get_grandparent(n);
  if (!g)
    return NULL;
  return (n->parent == g->left) ? g->right : g->left;
}

static void replace_node(Tree *root, Tree oldn, Tree newn) {
  if (!oldn->parent)
    *root = newn;
  else if (oldn == oldn->parent->left)
    oldn->parent->left = newn;
  else
    oldn->parent->right = newn;
  if (newn)
    newn->parent = oldn->parent;
}

// Helps the delete method respecting the red black trees conditions
static void delete_fixup(Tree *root, Tree x, Tree parent) {
  while ((x != *root) && (!x || x->color == BLACK)) {
    bool left_side = (parent && x == parent->left);
    Tree s = left_side ? parent->right : parent->left;

    if (s && s->color == RED) { // Case 1
      s->color = BLACK;
      parent->color = RED;
      if (left_side)
        left_rotate(root, parent);
      else
        right_rotate(root, parent);
      s = left_side ? parent->right : parent->left;
    }

    if ((!s->left || s->left->color == BLACK) &&
        (!s->right || s->right->color == BLACK)) { // Case 2
      if (s)
        s->color = RED;
      x = parent;
      parent = x ? x->parent : NULL;
    } else {
      if (left_side && (!s->right || s->right->color == BLACK)) { // Case 3
        if (s->left)
          s->left->color = BLACK;
        s->color = RED;
        right_rotate(root, s);
        s = parent->right;
      } else if (!left_side && (!s->left || s->left->color == BLACK)) {
        if (s->right)
          s->right->color = BLACK;
        s->color = RED;
        left_rotate(root, s);
        s = parent->left;
      }

      // Case 4
      s->color = parent->color;
      parent->color = BLACK;
      if (left_side && s->right)
        s->right->color = BLACK;
      else if (!left_side && s->left)
        s->left->color = BLACK;

      if (left_side)
        left_rotate(root, parent);
      else
        right_rotate(root, parent);
      x = *root;
      break;
    }
  }
  if (x)
    x->color = BLACK;
}

static Tree tree_minimum(Tree n) {
  while (n && n->left)
    n = n->left;
  return n;
}

Tree tree_new() { return NULL; }

void tree_delete(Tree tree, void (*delete)(void *)) {
  if (tree) {
    tree_delete(tree->left, delete);
    tree_delete(tree->right, delete);
    if (delete)
      delete (tree->data);
    free(tree);
  }
}

void left_rotate(Tree *root, Tree x) {
  Tree y = x->right;
  if (!y)
    return;

  x->right = y->left;
  if (y->left)
    y->left->parent = x;

  y->parent = x->parent;
  if (!x->parent)
    *root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}

void right_rotate(Tree *root, Tree x) {
  Tree y = x->left;
  if (!y)
    return;

  x->left = y->right;
  if (y->right)
    y->right->parent = x;

  y->parent = x->parent;
  if (!x->parent)
    *root = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;

  y->right = x;
  x->parent = y;
}

Tree tree_create(const void *data, size_t size) {
  Tree tree = malloc(sizeof(struct _TreeNode) + size);
  if (tree) {
    tree->left = NULL;
    tree->right = NULL;
    tree->color = RED;
    tree->parent = NULL;
    memcpy(tree->data, data, size);
  }

  return tree;
}

Tree tree_get_left(Tree tree) {
  if (tree)
    return tree->left;
  else
    return NULL;
}

Tree tree_get_right(Tree tree) {
  if (tree)
    return tree->right;
  else
    return NULL;
}

void *tree_get_data(Tree tree) {
  if (tree)
    return tree->data;
  else
    return NULL;
}

bool tree_set_left(Tree tree, Tree left) {
  if (tree) {
    tree->left = left;
    if (left) {
      left->parent = tree;
    }
    return true;
  } else
    return false;
}

bool tree_set_right(Tree tree, Tree right) {
  if (tree && right) {
    tree->right = right;
    if (right) {
      right->parent = tree;
    }
    return true;
  } else
    return false;
}

bool tree_set_data(Tree tree, const void *data, size_t size) {
  if (tree) {
    memcpy(tree->data, data, size);
    return true;
  } else
    return false;
}

// Insert at the right place if it violates the conditions of a red-black tree
bool tree_insert_sorted(Tree *root, const void *data, size_t size,
                        int (*compare)(const void *, const void *)) {
  Tree parent = NULL, cur = *root;

  while (cur) {
    parent = cur;
    int cmp = compare(data, cur->data);
    if (cmp == 0)
      return false;
    cur = (cmp < 0) ? cur->left : cur->right;
  }

  Tree node = tree_create(data, size);
  node->parent = parent;

  if (!parent)
    *root = node;
  else if (compare(data, parent->data) < 0)
    parent->left = node;
  else
    parent->right = node;

  while (node != *root && node->parent->color == RED) {
    Tree g = get_grandparent(node);
    if (!g)
      break;

    bool left_side = (node->parent == g->left);
    Tree uncle = get_uncle(node);

    if (uncle && uncle->color == RED) {
      node->parent->color = BLACK;
      uncle->color = BLACK;
      g->color = RED;
      node = g;
    } else {
      if (left_side && node == node->parent->right) {
        node = node->parent;
        left_rotate(root, node);
      } else if (!left_side && node == node->parent->left) {
        node = node->parent;
        right_rotate(root, node);
      }

      node->parent->color = BLACK;
      g->color = RED;
      if (left_side)
        right_rotate(root, g);
      else
        left_rotate(root, g);
    }
  }

  (*root)->color = BLACK;
  return true;
}

// Remove the element from a the tree
void node_delete(Tree *root, void *data, void (*del)(void *),
                 int (*compare)(const void *, const void *), size_t size) {
  Tree z = *root;
  while (z) {
    int cmp = compare(data, z->data);
    if (cmp == 0)
      break;
    z = (cmp < 0) ? z->left : z->right;
  }
  if (!z)
    return;

  Tree y = z;
  Color orig = y->color;
  Tree x = NULL, parent = NULL;

  if (!z->left) {
    x = z->right;
    parent = z->parent;
    replace_node(root, z, z->right);
  } else if (!z->right) {
    x = z->left;
    parent = z->parent;
    replace_node(root, z, z->left);
  } else {
    y = tree_minimum(z->right);
    orig = y->color;
    x = y->right;
    parent = (y->parent == z) ? y : y->parent;
    if (y->parent != z) {
      replace_node(root, y, y->right);
      y->right = z->right;
      if (y->right)
        y->right->parent = y;
    }
    replace_node(root, z, y);
    y->left = z->left;
    if (y->left)
      y->left->parent = y;
    y->color = z->color;
  }

  if (del)
    del(z->data);
  free(z);

  if (orig == BLACK)
    delete_fixup(root, x, parent);
}

void tree_pre_order(Tree tree, void (*func)(void *, void *), void *extra_data) {
  if (tree) {
    func(tree, extra_data);
    tree_pre_order(tree->left, func, extra_data);
    tree_pre_order(tree->right, func, extra_data);
  }
}

void tree_in_order(Tree tree, void (*func)(void *, void *), void *extra_data) {
  if (tree) {
    tree_in_order(tree->left, func, extra_data);
    func(tree, extra_data);
    tree_in_order(tree->right, func, extra_data);
  }
}

void tree_post_order(Tree tree, void (*func)(void *, void *),
                     void *extra_data) {
  if (tree) {
    tree_post_order(tree->left, func, extra_data);
    tree_post_order(tree->right, func, extra_data);
    func(tree, extra_data);
  }
}

void *tree_search(Tree tree, const void *data,
                  int (*compare)(const void *, const void *)) {
  if (tree) {
    switch (compare(data, tree->data)) {
    case -1:
      return tree_search(tree->left, data, compare);
    case 0:
      return tree->data;
    case 1:
      return tree_search(tree->right, data, compare);
    default:
      return NULL;
    }
  } else
    return NULL;
}