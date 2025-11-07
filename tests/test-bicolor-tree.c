#include "test.h"
#include "bicolor-tree.h"

void print_bicolor_tree(Tree tree, void (*print)(void *), int depth) {
  if (!tree)
    return;
  for (int i = 0; i < depth; i++)
    printf("  ");
  printf("[%s] ", tree->color == BLACK ? "BLACK" : "RED");
  print(tree->data);
  printf("\n");
  print_bicolor_tree(tree->left, print, depth + 1);
  print_bicolor_tree(tree->right, print, depth + 1);
}

// Test values + int complexity
void test_int()
{
  // add n values and delete n values
  size_t sizes[] = {10, 50, 100, 500, 1000, 5000, 10000,
                  50000, 100000, 500000, 1000000, 5000000, 10000000,
                  20000000, 50000000};
  Result results[NB_TESTS];

  for (int i = 0; i < NB_TESTS; i++) {
    size_t n = sizes[i];
    int *values = unique_list(n);
    Tree root = NULL;

    results[i].n = n;
    results[i].insert_time = test_insert_complexity(&root, values, n, (InsertFunc)tree_insert_sorted);
    results[i].search_time = test_search_complexity(&root, values, n, (SearchFunc)tree_search);
    results[i].delete_time = test_delete_complexity(&root, values, n, (DeleteFunc)node_delete);

    tree_delete(root, NULL);
    free(values);
  }

  // Write our results into a csv file
#ifdef _WIN32
  system("mkdir ..\\..\\result 2>nul");
  const char* python_cmd = "python ../../src/plot_results.py ../../result/results_bicolor.csv bicolor";
#else
  system("mkdir -p ../../result");
  const char* python_cmd = "python3 ../../src/plot_results.py ../../result/results_bicolor.csv bicolor";
#endif

  FILE *f = fopen("../../result/results_bicolor.csv", "w");

  fprintf(f, "n,insert_time,search_time,delete_time\n");
  for (int i = 0; i < NB_TESTS; i++) {
    fprintf(f, "%zu,%.10f,%.10f,%.10f\n", results[i].n, results[i].insert_time, results[i].search_time, results[i].delete_time);
  }
  fclose(f);

  system(python_cmd); // Generate plot
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
  print_bicolor_tree(root, print_hashmap, 0);
  printf("\n");

  Hashmap delete_entries[] = {{"dog", ""}, {"mouse", ""}};
  size_t m = sizeof(delete_entries) / sizeof(delete_entries[0]);
  for (size_t i = 0; i < m; i++) {
    printf("Deleting value: %s\n", delete_entries[i].word);
    node_delete(&root, &delete_entries[i], NULL, compare_dico, sizeof(Hashmap));
  }

  printf("\n Integer tree after deleting:\n");
  print_bicolor_tree(root, print_hashmap, 0);
  tree_delete(root, NULL);
  printf("\n");
}

int main() {
  test_int();
  // test_hashmap();
  return 0;
}