#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_RECORDS 1000

typedef struct {
  int key;
  char value[32];
} record;

typedef struct {
  int length;
  record records[MAX_NUM_RECORDS];
} table;

void swap(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

// Fisher–Yates shuffle というアルゴリズムを使います
void shuffle(int* array, int length) {
  int i = length;
  while (i > 1) {
    int j = rand() % i--;
    swap(&array[i], &array[j]);
  }
}

int search(table* tab, int target) {
  tab->records[tab->length].key = target;
  int index = 0;
  while (target != tab->records[index].key) {
    index++;
  }

  // 見つからなければ -1 を返します
  return index < tab->length ? index : -1;
}

void insert_tail(table* tab, record rec) {
  if (tab->length >= MAX_NUM_RECORDS - 1) {
    printf("ERROR: No more record can be inserted into table.\n");
    return;
  }

  tab->records[tab->length] = rec;
  tab->length++;
}

void erase(table* tab, int pos) {
  for (int i = pos; i < tab->length - 1; i++) {
    tab->records[i] = tab->records[i + 1];
  }
  tab->length--;
}

void cli_insert_tail(table* tab) {
  printf("Type in a key (>= 0) and a field. (example: \"100 BBB\")\n");
  while (true) {
    record rec;
    scanf("%d %s", &rec.key, rec.value);
    if (search(tab, rec.key) != -1) {
      printf("The key is already used.\n");
    } else {
      insert_tail(tab, rec);
      return;
    }
  }
}

void print(table* tab) {
  printf("TABLE: [ ");
  for (int i = 0; i < tab->length; i++) {
    printf("{%d, %s} ", tab->records[i].key, tab->records[i].value);
  }
  printf("]\n");
  printf("LENGTH: %d\n\n", tab->length);
}

int main() {
  // shuffle keys ([0...4])
  int keys[] = {0, 1, 2, 3, 4};
  int num_keys = sizeof(keys) / sizeof(int);
  shuffle(keys, num_keys);

  // create table
  table tab = {0};
  for (int i = 0; i < num_keys; i++) {
    record rec = {keys[i], "AAA"};
    insert_tail(&tab, rec);
  }
  print(&tab);

  // insert user input
  cli_insert_tail(&tab);
  print(&tab);

  // search 3
  int index = search(&tab, 3);
  bool found = index != -1;
  printf("3 was %s\n", found ? tab.records[index].value : "NOT FOUND.");

  // erase 3
  erase(&tab, index);
  print(&tab);

  // search 3
  index = search(&tab, 3);
  found = index != -1;
  printf("3 was %s\n", found ? tab.records[index].value : "NOT FOUND.");

  return 0;
}

// 実行結果
// TABLE: [ {4, AAA} {1, AAA} {0, AAA} {2, AAA} {3, AAA} ]
// LENGTH: 5
//
// Type in a key (>= 0) and a field. (example: "100 BBB")
// 100 B
// TABLE: [ {4, AAA} {1, AAA} {0, AAA} {2, AAA} {3, AAA} {100, B} ]
// LENGTH: 6
//
// 3 was AAA
// TABLE: [ {4, AAA} {1, AAA} {0, AAA} {2, AAA} {100, B} ]
// LENGTH: 5
//
// 3 was NOT FOUND.
