#include <stdio.h>
#include <stdlib.h>

typedef struct List{
    Tree* value;
    struct List* next;
}List;

typedef struct Tree{
    int value;
    List* children;
}Tree;

Tree* createNode(int value){
    Tree* now=(Tree*)malloc(sizeof(Tree));
    now->value=value;
    now->children=NULL;
    return now;
}

void printPreorder(Tree* root){
    if (root==NULL) return;
    printf("%d ", root->value);
    List* cur=root->children;
    while (cur!=NULL){
        printPreorder(cur->value);
        cur=cur->next;
    }
}

void printByChildren(Tree* root){
    if (root==NULL) return;
    
}

