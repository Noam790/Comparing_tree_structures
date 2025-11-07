

#include "avl-tree.h"
#include "min-max.h"
#include <stdbool.h>
#include <string.h>

/*--------------------------------------------------------------------*/
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

void left_rotate(Tree *tree) {
  Tree root = *tree;
  Tree right = root->right;
  if (!right) {
    return;
  }
  Tree rightleft = right->left;
  *tree = right;
  right->left = root;
  root->right = rightleft;

  int oldrootBal = root->balance;
  int oldRightBal = right->balance;

  root->balance = oldrootBal - 1 - MAX(oldRightBal, 0);
  right->balance = oldRightBal - 1 + MIN(root->balance, 0);
}

void right_rotate(Tree *tree) {
  Tree root = *tree;
  Tree left = root->left;
  if (!left) {
    return;
  }
  Tree leftright = left->right;
  *tree = left;
  left->right = root;
  root->left = leftright;

  int oldrootBal = root->balance;
  int oldLeftBal = left->balance;
  root->balance = oldrootBal + 1 - MIN(oldLeftBal, 0);
  left->balance = oldLeftBal + 1 + MAX(root->balance, 0);
}


// Equilibrate the tree using its new balances
void rebalance(Tree *ptree) {
  if (!ptree || !*ptree) {
    return;
  }

  Tree root = *ptree;
  int left_height = 0;
  int right_height = 0;

  if (root->left) {
    left_height = root->left->balance + 1;
  }

  if (root->right) {
    right_height = root->right->balance + 1;
  }

  root->balance = left_height - right_height;

  if (root->balance > 1) {
    if (root->left && root->left->balance >= 0)
      right_rotate(ptree); // simple rotation -> left left
    else {
      left_rotate(&root->left); // double rotation -> left right
      right_rotate(ptree);
    }
  } else if (root->balance < -1) {
    if (root->right && root->right->balance <= 0)
      left_rotate(ptree); // simple rotation -> right right
    else {
      right_rotate(&root->right); // double rotation -> right left
      left_rotate(ptree);
    }
  }
}

Tree tree_create(const void *data, size_t size) {
  Tree tree = malloc(sizeof(struct _TreeNode) + size);
  if (tree) {
    tree->left = NULL;
    tree->right = NULL;
    tree->balance = 0;
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

// Insert a data at the right position and rebalance it
bool tree_insert_sorted(Tree *ptree, const void *data,
                        size_t size,
                        int (*compare)(const void *, const void *)) {
  if (!ptree) {
    return false;
  }

  if (*ptree == NULL) {
    *ptree = tree_create(data, size);
    if (!*ptree)
      return false;
    return true;
  }

  Tree root = *ptree;
  int pos = compare(data, root->data);

  if (pos < 0) { // left insertion
    if (!tree_insert_sorted(&root->left, data, size, compare)) {
      return false;
    }
    if (root->left) {
      root->left->parent = root;
    }

  } else if (pos > 0) { // right insertion
    if (!tree_insert_sorted(&root->right, data, size, compare)) {
      return false;
    }
    if (root->right) {
      root->right->parent = root;
    }
  } else { // don't add duplicates
    return false;
  }

  rebalance(ptree);

  return true;
}

void node_delete(Tree *ptree, void *data, void (*delete_func)(void *),
                 int (*compare)(const void *, const void *), size_t size) {
  if (!ptree || !*ptree) {
    return;
  }

  Tree root = *ptree;
  int cmp = compare(data, root->data);

  if (cmp < 0) {
    node_delete(&root->left, data, delete_func, compare, size);
  } else if (cmp > 0) {
    node_delete(&root->right, data, delete_func, compare, size);
  } else {
    if (root->left && root->right) {
      // Two childrens
      Tree succ = root->right;
      while (succ->left) {
        succ = succ->left;
      }
      memcpy(root->data, succ->data, size);
      node_delete(&root->right, root->data, delete_func, compare, size);
    } else {
      // 0 or 1 child
      Tree child;
      if (root->left) {
        child = root->left;
      } else {
        child = root->right;
      }

      if (delete_func) {
        delete_func(root->data);
      }
      free(root);

      *ptree = child;

      if (child) {
        child->parent = NULL;
      }
      return;
    }
  }
  
  rebalance(ptree);
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

size_t tree_height(Tree tree) {
  if (tree)
    return 1 + MAX(tree_height(tree->left), tree_height(tree->right));
  else
    return 0;
}

size_t tree_size(Tree tree) {
  if (tree)
    return 1 + tree_size(tree->left) + tree_size(tree->right);
  else
    return 0;
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

static void set(void *data, void *array) {
  static size_t size;
  static size_t offset;

  if (data) {
    memcpy(array + offset, data, size);
    offset += size;
  } else {
    offset = 0;
    size = *(size_t *)array;
  }
}

int tree_sort(void *array, size_t length, size_t size,
              int (*compare)(const void *, const void *)) {
  size_t i;
  Tree tree = tree_new();
  void *pointer;

  pointer = array;
  for (i = 0; i < length; i++) {
    if (tree_insert_sorted(&tree, pointer, size, compare))
      pointer += size;
    else {
      tree_delete(tree, NULL);
      return false;
    }
  }
  set(NULL, &size);
  tree_in_order(tree, set, array);
  tree_delete(tree, NULL);
  return true;
}