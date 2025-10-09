#include "bicolor-tree.h"
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
  Tree racine = *tree;
  Tree right = racine->right;
  if (!right) {
    return;
  }
  Tree rightleft = right->left;
  *tree = right;
  right->left = racine;
  racine->right = rightleft;

  /*
  
  TODO : Implement color change
  
  */
}

void right_rotate(Tree *tree) {
  Tree racine = *tree;
  Tree left = racine->left;
  if (!left) {
    return;
  }
  Tree leftright = left->right;
  *tree = left;
  left->right = racine;
  racine->left = leftright;

    /*
  
  TODO : Implement color change
  
  */
}


// Reequilibrate our red-black tree if a node violate a condition
void rebalance(Tree *ptree) {
  if (!ptree || !*ptree) {
    return;
  }

  Tree racine = *ptree;
  int left_height = 0;
  int right_height = 0;

  if (racine->left) {
    left_height = racine->left->balance + 1;
  }

  if (racine->right) {
    right_height = racine->right->balance + 1;
  }

  racine->balance = left_height - right_height;

  if (racine->balance > 1) {
    if (racine->left && racine->left->balance >= 0)
      right_rotate(ptree); // simple rotation -> left left
    else {
      left_rotate(&racine->left); // double rotation -> left right
      right_rotate(ptree);
    }
  } else if (racine->balance < -1) {
    if (racine->right && racine->right->balance <= 0)
      left_rotate(ptree); // simple rotation -> right right
    else {
      right_rotate(&racine->right); // double rotation -> right left
      left_rotate(ptree);
    }
  }
}

Tree tree_create(const void *data, size_t size) {
  Tree tree = malloc(sizeof(struct _TreeNode) + size);
  if (tree) {
    tree->left = NULL;
    tree->right = NULL;
    tree->color = BLACK;
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
bool tree_insert_sorted(Tree *ptree, const void *data,
                        size_t size,
                        int (*compare)(const void *, const void *)) {
  if (!ptree) {
    return false;
  }

  if (*ptree == NULL) { // Empty tree
    *ptree = tree_create(data, size);
    (*ptree)->color = RED;
    if (!*ptree)
      return false;
    return true;
  }

  Tree racine = *ptree;
  int pos = compare(data, racine->data);

  if (pos < 0) { // Left insertion
    if (!tree_insert_sorted(&racine->left, data, size, compare)) {
      return false;
    }
    if (racine->left) {
      racine->left->parent = racine;
    }

  } else if (pos > 0) { // Right insertion
    if (!tree_insert_sorted(&racine->right, data, size, compare)) {
      return false;
    }
    if (racine->right) {
      racine->right->parent = racine;
    }
  } else { // duplicate, do nothing
    return false;
  }

  rebalance(ptree);

  return true;
}

// Remove the element from a the tree
void node_delete(Tree *ptree, void *data, void (*delete_func)(void *),
                 int (*compare)(const void *, const void *), size_t size) {
  if (!ptree || !*ptree) {
    return;
  }

  Tree racine = *ptree;
  int cmp = compare(data, racine->data);

  if (cmp < 0) {
    node_delete(&racine->left, data, delete_func, compare, size);
  } else if (cmp > 0) {
    node_delete(&racine->right, data, delete_func, compare, size);
  } else {
    if (racine->left && racine->right) {
      // Two childs
      Tree succ = racine->right;
      while (succ->left) {
        succ = succ->left;
      }
      memcpy(racine->data, succ->data, size);
      node_delete(&racine->right, racine->data, delete_func, compare, size);
    } else {
      // 0 or 1 child
      Tree enfant;
      if (racine->left) {
        enfant = racine->left;
      } else {
        enfant = racine->right;
      }

      if (delete_func) {
        delete_func(racine->data);
      }
      free(racine);

      *ptree = enfant;

      if (enfant) {
        enfant->parent = NULL;
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