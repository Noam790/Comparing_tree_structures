

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

  int oldRacineBal = racine->balance;
  int oldRightBal = right->balance;

  racine->balance = oldRacineBal - 1 - MAX(oldRightBal, 0);
  right->balance = oldRightBal - 1 + MIN(racine->balance, 0);
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

  int oldRacineBal = racine->balance;
  int oldLeftBal = left->balance;
  racine->balance = oldRacineBal + 1 - MIN(oldLeftBal, 0);
  left->balance = oldLeftBal + 1 + MAX(racine->balance, 0);
}


// Permet d'equilibrer notre arbre en fonction des balances obtenues
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
      right_rotate(ptree); // simple rotation -> gauche gauche
    else {
      left_rotate(&racine->left); // double rotation -> gauche droite
      right_rotate(ptree);
    }
  } else if (racine->balance < -1) {
    if (racine->right && racine->right->balance <= 0)
      left_rotate(ptree); // simple rotation -> droite droite
    else {
      right_rotate(&racine->right); // double rotation -> droite gauche
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

// Respecter les criteres de l'arbre avl, reequilibrage si mauvaise insertion
bool tree_insert_sorted(Tree *ptree, const void *data,
                        size_t size,
                        int (*compare)(const void *, const void *)) {
  if (!ptree) {
    return false;
  }

  if (*ptree == NULL) { // Arbre vide
    *ptree = tree_create(data, size);
    if (!*ptree)
      return false;
    return true;
  }

  Tree racine = *ptree;
  int pos = compare(data, racine->data);

  if (pos < 0) { // Insertion à gauche
    if (!tree_insert_sorted(&racine->left, data, size, compare)) {
      return false;
    }
    if (racine->left) { // Si non NULL
      racine->left->parent = racine;
    }

  } else if (pos > 0) { // Insertion à droite
    if (!tree_insert_sorted(&racine->right, data, size, compare)) {
      return false;
    }
    if (racine->right) {
      racine->right->parent = racine;
    }
  } else { // doublon donc on ajoute pas
    return false;
  }

  rebalance(ptree);

  return true;
}

// Supprimer l'element d'un arbre, on passe size pour les tests avec plusieurs types
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
      // Deux enfants : remplacer par successeur
      Tree succ = racine->right;
      while (succ->left) {
        succ = succ->left;
      }
      memcpy(racine->data, succ->data, size);
      node_delete(&racine->right, racine->data, delete_func, compare, size);
    } else {
      // 0 ou 1 enfant
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

  // Rééquilibrage
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