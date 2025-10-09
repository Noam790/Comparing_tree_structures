#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl-tree.h"


// Tests sur int
int compare_int(const void *a, const void *b) {
  int val_a = *(int *)a;
  int val_b = *(int *)b;
  if (val_a < val_b)
    return -1;
  if (val_a > val_b)
    return 1;
  return 0;
}

// Tests sur char[1]
int compare_str(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

// Tests sur une structure
typedef struct {
    char mot[50];
    char definition[200];
} Dictionnaire;

int compare_dico(const void *a, const void *b) {
    const Dictionnaire *da = (const Dictionnaire *)a;
    const Dictionnaire *db = (const Dictionnaire *)b;
    return strcmp(da->mot, db->mot);
}

// Tests affichages par type
void print_int(void *data) {
    printf("Valeur : %d", *(int *)data);
}

void print_str(void *data) {
    printf("Valeur : %s", *(char **)data);
}

void print_dico(void *data) {
    Dictionnaire *d = (Dictionnaire *)data;
    printf("Mot : %s -> %s", d->mot, d->definition);
}


// Vérifier les balances et le parcours en post order de l'arbre
void verify_tree(Tree tree, void (*func)(void *)) {
  if (!tree)
    return;

  int left_height = 0;
  int right_height = 0;

  if (tree->left) {
    verify_tree(tree->left, func);
    left_height = tree->left->balance + 1;
  }
  if (tree->right) {
    verify_tree(tree->right, func);
    right_height = tree->right->balance + 1;
  }

  int expected_balance = left_height - right_height;
  assert(tree->balance == expected_balance);

  func(tree->data);
  printf(", Balance : %d\n", tree->balance);
}

void test_int(void) {
  Tree root = tree_new();
  assert(root == NULL);

  int values[] = {10, 5, 20, 15, 25, 2, 7};
  size_t n = sizeof(values) / sizeof(values[0]);
  size_t sizeInt = sizeof(int);

  // Insertion
  for (size_t i = 0; i < n; i++) {
    assert(tree_insert_sorted(&root, &values[i], sizeInt, compare_int) == true);
    int *found = tree_search(root, &values[i], compare_int);
    assert(found != NULL && *found == values[i]);
    printf("Après insertion de %d :\n", values[i]);
    verify_tree(root, print_int);
  }

  // Recherche
  for (size_t i = 0; i < n; i++) {
    int *found = tree_search(root, &values[i], compare_int);
    assert(found != NULL && *found == values[i]);
  }

  // Suppression
  printf("affichage arbre après supressions\n");
  int delete_vals[] = {20, 5, 10};
  size_t m = sizeof(delete_vals) / sizeof(delete_vals[0]);

  for (size_t i = 0; i < m; i++) {
    printf("Suppression de %d :\n", delete_vals[i]);
    node_delete(&root, &delete_vals[i], NULL, compare_int, sizeof(int));
    assert(tree_search(root, &delete_vals[i], compare_int) == NULL);
    verify_tree(root, print_int);
  }

  tree_delete(root, NULL);
}

void test_strings() {
    printf("Test Strings\n");
    Tree root = tree_new();
    const char *words[] = {"pomme", "banane", "orange", "kiwi", "fraise"};
    size_t n = sizeof(words)/sizeof(words[0]);
    for (size_t i = 0; i < n; i++) {
        const char *w = words[i];
        assert(tree_insert_sorted(&root, &w, sizeof(char *), compare_str));
        printf("Après insertion de %s :\n", w);
        verify_tree(root, print_str);
        printf("----------------\n");
    }

    const char *delete_words[] = {"banane", "kiwi"};
    size_t m = sizeof(delete_words)/sizeof(delete_words[0]);
    for (size_t i = 0; i < m; i++) {
        printf("Suppression de %s :\n", delete_words[i]);
        node_delete(&root, &delete_words[i], NULL, compare_str, sizeof(char *));
        verify_tree(root, print_str);
        printf("----------------\n");
    }

    tree_delete(root, NULL);
}

void test_struct() {
    printf("Test Dictionnaire\n");
    Tree root = tree_new();
    Dictionnaire entries[] = {
        {"chat", "animal domestique"},
        {"chien", "meilleur ami de l'homme"},
        {"poisson", "vit dans l'eau"},
        {"souris", "petit rongeur"}
    };
    size_t n = sizeof(entries)/sizeof(entries[0]);

    for (size_t i = 0; i < n; i++) {
        assert(tree_insert_sorted(&root, &entries[i], sizeof(Dictionnaire), compare_dico));
        printf("\nAprès insertion de %s :\n", entries[i].mot);
        verify_tree(root, print_dico);
    }

    Dictionnaire del = {"chien", ""};
    printf("\nSuppression de %s :\n", del.mot);
    node_delete(&root, &del, NULL, compare_dico, sizeof(Dictionnaire));
    verify_tree(root, print_dico), print_dico;
    tree_delete(root, NULL);
}

int main() {
    test_int();
    printf("\nTests int réussis\n\n");
    test_strings();
    printf("\nTests char réussis\n\n");
    test_struct();
    printf("\nTests dictionnaire réussis.\n");
    return EXIT_SUCCESS;
}