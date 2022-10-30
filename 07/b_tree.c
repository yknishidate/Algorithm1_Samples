#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 5

// 4 bytes
typedef enum {
    INTERNAL,
    EXTERNAL,
} node_type;

typedef struct node_ node;

typedef struct {
    node* ptr;
    int bound;
} pair;

struct node_ {
    node_type tag;

    // 各インスタンスは internal か external の一方の
    // データのみを必要とするため、無名共用体を使います。
    union {
        struct {
            int count;
            pair children[M];
        } internal;

        struct {
            int key;
            char value[32];
        } external;
    };
};

// TODO: use
node* init_external_node(int key, const char* value) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->tag = EXTERNAL;
    new_node->external.key = key;
    strcpy(new_node->external.value, value);
    return new_node;
}

int locate(node* n, int target) {
    int low = 1;
    int high = n->internal.count - 1;
    while (low <= high) {
        int middle = (low + high) / 2;
        if (target < n->internal.children[middle].bound) {
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }
    return high;
}

node* search(node* root, int target) {
    if (root == NULL) {
        return false;
    }

    node* current = root;
    while (current->tag == INTERNAL) {
        int index = locate(current, target);
        current = current->internal.children[index].ptr;
    }
    if (current->external.key == target) {
        return current;
    }
    return NULL;
}

bool insert_(node** p_current, int key, const char* value, pair** p_secondary) {
    node* current = *p_current;
    pair* secondary = *p_secondary;
    if (current->tag == EXTERNAL) {
        assert(current->external.key != key);

        node* new_node = (node*)malloc(sizeof(node));
        new_node->tag = EXTERNAL;
        new_node->external.key = key;
        strcpy(new_node->external.value, value);
        if (key < current->external.key) {
            // current と new_node を swap
            node* tmp = current;
            *p_current = new_node;
            new_node = tmp;
        }

        secondary->ptr = new_node;
        secondary->bound = new_node->external.key;
        return true;
    }

    // current is INTERNAL
    int index = locate(current, key);
    node* child = current->internal.children[index].ptr;
    if (insert_(&child, key, value, p_secondary)) {
        if (current->internal.count < M) {  // まだ子ノードを入れられる場合
            // before: [1][3][5][ ][ ][ ]
            // insert: 4
            // after:  [1][3][4][5][ ][ ]
            for (int j = current->internal.count - 1; j >= index; j--) {
                current->internal.children[j + 1] = current->internal.children[j];
            }
            current->internal.children[index + 1] = *secondary;
            current->internal.count++;
        } else {  // もう子ノードを入れられない場合
            printf("count is M. It's full");
            // assert(false);

            // node* new_node = (node*)malloc(sizeof(node));
            // new_node->tag = INTERNAL;
            // // new_node->external.key = key;
            // // strcpy(new_node->external.value, value);
            // int n = (M + 1) / 2;
            // if (index >= n) {
            //     for (int j = n + 1; j < index; j++) {
            //         current->internal.children[j - n] = current->internal.children[j];
            //     }
            //     current->internal.children[index + 1 - n] = *secondary;
            //     for (int j = index + 1; j < M; j++) {
            //         current->internal.children[j - n] = current->internal.children[j];
            //     }
            // }
        }
    } else {
    }
    assert(false);
    return false;
}

void insert(node** p_root, int key, const char* value) {
    if (*p_root == NULL) {
        node* root = (node*)malloc(sizeof(node));
        root->tag = EXTERNAL;
        root->external.key = key;
        strcpy(root->external.value, value);
        *p_root = root;
        return;
    }

    pair* secondary = (pair*)malloc(sizeof(pair));
    if (insert_(p_root, key, value, &secondary)) {
        node* new_root = (node*)malloc(sizeof(node));
        new_root->tag = INTERNAL;
        new_root->internal.children[0].ptr = *p_root;
        new_root->internal.children[1] = *secondary;
        *p_root = new_root;
    }
}

void print(node* current, int depth) {
    if (current == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    if (current->tag == INTERNAL) {
        printf("[");
        for (int i = 0; i < current->internal.count; i++) {
            printf("Bound=%d ", current->internal.children[i].bound);
        }
        printf("]\n");

        for (int i = 0; i < current->internal.count; i++) {
            // printf("Bound=%d ", current->internal.children[i].bound);
            print(current->internal.children[i].ptr, depth + 1);
        }
    } else {
        printf("{%d, %s}\n", current->external.key, current->external.value);
    }
}

int main() {
    node* root = NULL;
    for (int i = 0; i < 15; i++) {
        printf("insert %d\n", i);
        insert(&root, i, "A");
    }
    print(root, 0);
    // n.internal.count = 23;
    // printf("%d\n", n.internal.count);
}

// 実行結果