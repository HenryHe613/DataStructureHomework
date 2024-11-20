#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include<windows.h>
#endif

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
    if(root == NULL) return;
    printf("%c ", root->value);
    List* cur = root->children;
    while(cur != NULL){
        printPreorder(cur->value);
        cur = cur->next;
    }
}

void printByChildren(Tree* root){
    if(root == NULL) return;
    List* cur = root->children;
    printf("%c -> ", root->value);
    while(cur != NULL){
        printf("%c ", cur->value->value);
        printf("-> ");
        cur = cur->next;
    }
    printf("^\n");
    cur = root->children;
    while(cur != NULL){
        printByChildren(cur->value);
        cur = cur->next;
    }
}

Tree* input(){
    Tree* root = NULL;
    Tree* arr[100];
    arr[0] = root;
    int depth = 0;
    printf("举例: A(B(D,E),C(F,G))\n");
    printf("请输入二叉树的字符序列：");
    char c = getchar();
    while(c != '\n'){
        if(c == '(')
            depth++;
        if(c == ')')
            depth--;
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
            if(root == NULL){
                root = createNode(c);
                arr[depth] = root;
            } else{
                Tree* now = createNode(c);
                Tree* parent = arr[depth - 1];
                List* cur = parent->children;
                if(cur == NULL){
                    parent->children = (List*)malloc(sizeof(List));
                    parent->children->value = now;
                    parent->children->next = NULL;
                } else{
                    while(cur->next != NULL){
                        cur = cur->next;
                    }
                    cur->next = (List*)malloc(sizeof(List));
                    cur->next->value = now;
                    cur->next->next = NULL;
                }
                arr[depth] = now;
            }
        }
        c = getchar();
    }
    return root;
}


int main(){
    // Windows下设置UTF-8编码
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    DWORD mode;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
    #endif
    Tree* root = input();
    printf("先序遍历: \n");
    printPreorder(root);
    printf("\n");
    printf("孩子表示法: \n");
    printByChildren(root);
    return 0;
}