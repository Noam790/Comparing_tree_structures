#include "bicolor-tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

clock_t start, end;
double time_ct;

// Comparators
int compare_int(const void *a, const void *b) {
  int va = *(int *)a, vb = *(int *)b;
  return (va < vb) ? -1 : (va > vb);
}

typedef struct {
  char word[50];
  char definition[200];
} Hashmap;

int compare_dico(const void *a, const void *b) {
  const Hashmap *da = a, *db = b;
  return strcmp(da->word, db->word);
}


double test_insert_complexity(Tree *root, int *values, size_t n){
  start = clock();
  for (size_t i = 0; i < n; i++) {
    tree_insert_sorted(root, &values[i], sizeof(int), compare_int);
  }

  end = clock();
  time_ct = (double)(end - start) / CLOCKS_PER_SEC;
  return time_ct;
}

double test_delete_complexity(Tree *root, int *values, size_t n){
  start = clock();
  for (size_t i = 0; i < n; i++) {
    node_delete(root, &values[i], NULL, compare_int, sizeof(int));
  }

  end = clock();
  time_ct = (double)(end - start) / CLOCKS_PER_SEC;
  return time_ct;
}

// Print functions
void print_int(void *data) { printf("%d", *(int *)data); }
void print_hashmap(void *data) {
  Hashmap *d = data;
  printf("%s -> %s", d->word, d->definition);
}

// Display entire tree
void print_tree(Tree tree, void (*print)(void *), int depth) {
  if (!tree)
    return;
  for (int i = 0; i < depth; i++)
    printf("  ");
  printf("[%s] ", tree->color == BLACK ? "BLACK" : "RED");
  print(tree->data);
  printf("\n");
  print_tree(tree->left, print, depth + 1);
  print_tree(tree->right, print, depth + 1);
}

// Test values + int complexity
void test_int() {
  Tree root = NULL;
  int values[] = {10, 20, 5, 15, 2, 25, 7, 12};
  size_t n = sizeof(values) / sizeof(int);
  
  double time_insert = test_insert_complexity(&root, values, n); // time between 8 insertions
  printf("\n Insertion time : %fsec for %ld values", time_insert, sizeof(values) / sizeof(int));

  printf("\n Integer tree after inserting:\n");
  print_tree(root, print_int, 0);
  printf("\n");

  int delete_vals[] = {20, 5, 10};
  size_t n_del = sizeof(delete_vals) / sizeof(int);
  double time_delete = test_delete_complexity(&root, delete_vals, n_del);

  printf("\n Deletion time : %fsec for %ld values", time_delete, sizeof(delete_vals) / sizeof(int)); // time between 3 deletions

  printf("\n Integer tree after deleting:\n");
  print_tree(root, print_int, 0);
  tree_delete(root, NULL);
  printf("\n");
}


void test_hashmap() {
  Tree root = NULL;
  Hashmap entries[] = {{"cat", "domestic animal"},
                       {"dog", "man's best friend"},
                       {"fish", "live in water"},
                       {"mouse", "little rodent"},
                       {"bird", "can fly"}};
  size_t n = sizeof(entries) / sizeof(entries[0]);

  for (size_t i = 0; i < n; i++) {
    printf("Inserting value: %s\n", entries[i].word);
    tree_insert_sorted(&root, &entries[i], sizeof(Hashmap), compare_dico);
  }

  printf("\n Hashmap tree after inserting:\n");
  print_tree(root, print_hashmap, 0);
  printf("\n");

  Hashmap delete_entries[] = {{"dog", ""}, {"mouse", ""}};
  size_t m = sizeof(delete_entries) / sizeof(delete_entries[0]);
  for (size_t i = 0; i < m; i++) {
    printf("Deleting value: %s\n", delete_entries[i].word);
    node_delete(&root, &delete_entries[i], NULL, compare_dico, sizeof(Hashmap));
  }

  printf("\n Integer tree after deleting:\n");
  print_tree(root, print_hashmap, 0);
  tree_delete(root, NULL);
  printf("\n");
}

int main() {
  test_int();
  // test_hashmap();
  return 0;
}