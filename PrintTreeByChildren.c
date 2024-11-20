#include <stdio.h>
#include <stdlib.h>
typedef struct Tree Tree;
typedef struct List List;

typedef struct List{
    Tree* value;
    struct List* next;
}List;

typedef struct Tree{
    int value;
    List* children;
}Tree;

Tree* createNode(int value){
    Tree* now = (Tree*)malloc(sizeof(Tree));
    now->value = value;
    now->children = NULL;
    return now;
}

void printPreorder(Tree* root){
    if (root == NULL) return;
    printf("%d ", root->value);
    List* cur = root->children;
    while (cur != NULL){
        printPreorder(cur->value);
        cur = cur->next;
    }
}

void printByChildren(Tree* root){
    if (root == NULL) return;
    List* cur = root->children;
    while (cur != NULL){
        printf("%d ", cur->value->value);
        cur = cur->next;
    }
    printf("\n");
    cur = root->children;
    while (cur != NULL){
        printByChildren(cur->value);
        cur = cur->next;
    }
}


Tree* input(){
    char c;
    Tree* root = NULL;
    Tree* arr[100];
    int depth = 0;
    c = getchar();
    while (c != '\n'){
        if (c == ' '){
            c = getchar();
            continue;
        }
        if (c == '('){
            Tree* now = arr[depth - 1];
            List* cur = now->children;
            while (cur->next != NULL){
                cur = cur->next;
            }
            arr[depth] = cur->value;
            c = getchar();
            continue;   
        }
        if (c == ')'){
            depth--;
            c = getchar();
            continue;
        }
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
            if (root == NULL){
                root = createNode(c);
                arr[depth] = root;
                depth++;
            }
            else{
                Tree* now = createNode(c);
                Tree* parent = arr[depth - 1];
                List* cur = parent->children;
                if (cur == NULL){
                    parent->children = (List*)malloc(sizeof(List));
                    parent->children->value = now;
                    parent->children->next = NULL;
                }
                else{
                    while (cur->next != NULL){
                        cur = cur->next;
                    }
                    cur->next = (List*)malloc(sizeof(List));
                    cur->next->value = now;
                    cur->next->next = NULL;
                }
            }
        }
    }
    return root;
}


int main(){
    Tree* root = input();
    printByChildren(root);
    return 0;
}