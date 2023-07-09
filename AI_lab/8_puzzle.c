#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRIME 1000

struct hash_table_entry {
  int key;
  bool value;
  struct hash_table_entry *next;
};
typedef struct hash_table_entry entry;

struct hash_table {
  struct hash_table_entry *arr;
  int table_size;
};
typedef struct hash_table hash_table;

hash_table *create_hash_table(int table_size) {
  hash_table *ret = (hash_table *)malloc(sizeof(hash_table));
  ret->arr = (entry *)malloc(table_size * sizeof(entry));
  ret->table_size = table_size;
  return ret;
}

void initialize_hash_table(hash_table *table) {
  for (int i = 0; i < table->table_size; i++) {
    (table->arr + i)->value = false;
    (table->arr + i)->next = NULL;
  }
}

int hash(int key) { return key % PRIME; }

void insert(hash_table *table, int key, bool value) {
  int index = hash(key);
  if (table->arr[index].value == false) {
    table->arr[index].key = key;
    table->arr[index].value = value;
  } else {
    entry *temp = (entry *)malloc(sizeof(entry));
    temp->next = table->arr[index].next;
    table->arr[index].next = temp;
    temp->value = value;
    temp->key = key;
  }
}

bool search(hash_table *table, int key) {
  int index = hash(key);
  entry *temp = table->arr + index;
  while (temp != NULL && temp->key != key)
    temp = temp->next;
  if (temp == NULL)
    return false;
  return temp->value;
}

// void delete(hash_table *table,int key){
// 	int index=hash(key);
//         entry *temp=table->arr+index;
// 	if(temp->key==key){
// 		temp->value=-1;
// 		return;
// 	}
//         while (temp->next!=NULL && temp->next->key!=key) temp=temp->next;
// 	if(temp->next==NULL) return;
// 	entry *temp1=temp->next;
// 	temp->next=temp->next->next;
// 	free(temp1);
// }

void deallocate(hash_table *table) {
  entry *temp, *temp1;
  for (int i = 0; i < table->table_size; i++) {
    temp = (table->arr + i)->next;
    while (temp != NULL) {
      temp1 = temp;
      temp = temp->next;
      free(temp1);
    }
  }
  free(table->arr);
  free(table);
}

typedef struct node {
  struct node *parent;
  int priority;
  int cost;
  char state[10];
  char action;
} node;

void swap(node **arr, int a, int b) {
  node *temp = arr[a];
  arr[a] = arr[b];
  arr[b] = temp;
}

void min_heapify(
    node **arr, int i,
    int heap_size) // i,l and r are C array type indices throughout the code
{
  int l, r;

  while (1) {
    l = (i << 1) + 1;
    r = (i << 1) + 2;

    if (l > heap_size - 1)
      break;

    else if (l == heap_size - 1) {
      if (arr[i]->priority > arr[l]->priority)
        swap(arr, i, l);
      break;
    }

    else if (arr[i]->priority > arr[l]->priority &&
             arr[r]->priority >= arr[l]->priority) {
      swap(arr, i, l);
      i = l;
    }

    else if (arr[i]->priority > arr[r]->priority &&
             arr[l]->priority > arr[r]->priority) {
      swap(arr, i, r);
      i = r;
    }

    else
      break;
  }
}

node *heap_extract_min(node **arr, int *heap_size) {
  node *min = *arr;
  *arr = *(arr + *heap_size - 1);
  (*heap_size)--;
  min_heapify(arr, 0, *heap_size);
  return min;
}

void min_heap_insert(node **arr, node *key, int *heap_size) {
  arr[*heap_size] = key;
  (*heap_size)++;
  int i = *heap_size;
  // while (arr[i - 1] > arr[(i >> 1) - 1] && (i >> 1) >= 1)
  // {
  //     swap(arr, i - 1, (i >> 1) - 1);
  //     i = (i >> 1);
  // }
  // if (key == NULL)
  //     printf("key is null\n");
  while (i >> 1 >= 1 && key->priority <= arr[(i >> 1) - 1]->priority) {
    // printf("hello");
    arr[i - 1] = arr[(i >> 1) - 1];
    i >>= 1;
  }
  arr[i - 1] = key; // using insertion sort type data shift and reducing no. of
                    // assignments from 3 to 1
}

int manhattan(node *nod) {
  int count = 0, x;
  for (int i = 0; i < 9; i++) {
    if (x = nod->state[i] - '0') {
      count += abs((x - 1) % 3 - i % 3) + abs((x - 1) / 3 - i / 3);
    }
  }
  return count;
}

bool solvable(node *nod) {
  int inversions = 0;
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 9; j++) {
      if (nod->state[i] > nod->state[j] && nod->state[i] != '0' &&
          nod->state[j] != '0')
        inversions++;
    }
  }
  return inversions % 2 == 0;
}

bool goal_test(node *nod) {
  for (int i = 0; i < 8; i++) {
    if (nod->state[i] - '0' != i + 1)
      return false;
  }
  return true;
}

node *a_star(node *nod) {
  unsigned int explored_num = 0;
  int heap_size = 0, i;
  node *arr[10000];
  node *x, *y;
  hash_table *hash_map = create_hash_table(1000);
  initialize_hash_table(hash_map);
  char temp;
  min_heap_insert(arr, nod, &heap_size);
  insert(hash_map, atoi(nod->state), true);
  while (true) {
    x = heap_extract_min(arr, &heap_size);
    explored_num++;
    // printf("No. of nodes explored: %u\n", explored_num);
    if (goal_test(x)) {
      printf("No. of nodes explored: %u\n", explored_num);
      return x;
    }
    for (i = 0; x->state[i] != '0'; i++)
      ;
    if (i % 3) {
      y = (node *)malloc(sizeof(node));
      y->action = 'l';
      y->cost = x->cost + 1;
      y->parent = x;
      strcpy(y->state, x->state);
      temp = y->state[i];
      y->state[i] = y->state[i - 1];
      y->state[i - 1] = temp;
      y->priority = manhattan(y) + y->cost;
      if (!search(hash_map, atoi(y->state))) {
        if (y == NULL) {
          printf("Memory overflow\n");
          printf("No. of nodes explored: %u\n", explored_num);
          return x;
        }
        min_heap_insert(arr, y, &heap_size);
        insert(hash_map, atoi(y->state), true);
      }
    }
    if ((i + 1) % 3) {
      y = (node *)malloc(sizeof(node));
      y->action = 'r';
      y->cost = x->cost + 1;
      y->parent = x;
      strcpy(y->state, x->state);
      temp = y->state[i];
      y->state[i] = y->state[i + 1];
      y->state[i + 1] = temp;
      y->priority = manhattan(y) + y->cost;
      if (!search(hash_map, atoi(y->state))) {
        if (y == NULL) {
          printf("Memory overflow\n");
          printf("No. of nodes explored: %u\n", explored_num);
          return x;
        }
        min_heap_insert(arr, y, &heap_size);
        insert(hash_map, atoi(y->state), true);
      }
    }
    if (i > 2) {
      y = (node *)malloc(sizeof(node));
      y->action = 'u';
      y->cost = x->cost + 1;
      y->parent = x;
      strcpy(y->state, x->state);
      temp = y->state[i];
      y->state[i] = y->state[i - 3];
      y->state[i - 3] = temp;
      y->priority = manhattan(y) + y->cost;
      if (!search(hash_map, atoi(y->state))) {
        if (y == NULL) {
          printf("Memory overflow\n");
          printf("No. of nodes explored: %u\n", explored_num);
          return x;
        }
        min_heap_insert(arr, y, &heap_size);
        insert(hash_map, atoi(y->state), true);
      }
    }
    if (i < 6) {
      y = (node *)malloc(sizeof(node));
      y->action = 'd';
      y->cost = x->cost + 1;
      y->parent = x;
      strcpy(y->state, x->state);
      temp = y->state[i];
      y->state[i] = y->state[i + 3];
      y->state[i + 3] = temp;
      y->priority = manhattan(y) + y->cost;
      if (!search(hash_map, atoi(y->state))) {
        if (y == NULL) {
          printf("Memory overflow\n");
          printf("No. of nodes explored: %u\n", explored_num);
          return x;
        }
        min_heap_insert(arr, y, &heap_size);
        insert(hash_map, atoi(y->state), true);
      }
    }
  }
  deallocate(hash_map);
}

void print(node *solution) {
  if (solution) {
    print(solution->parent);
    printf("%c ", solution->action);
  }
  return;
}

int main() {
  node *start = (node *)malloc(sizeof(node));
  start->action = '0'; // Null action
  start->cost = 0;
  start->parent = NULL;
  printf("enter starting position: ");
  scanf("%s", start->state);
  start->priority = manhattan(start);
  if (solvable(start)) {
    print(a_star(start));
  } else
    printf("Not solvable");
}
