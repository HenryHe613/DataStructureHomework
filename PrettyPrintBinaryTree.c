#include<stdio.h>
#include<string.h>
#include <stdlib.h>

const int LEN = strlen("└──");

typedef struct TreeNode{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode;

TreeNode* createNode(int data){
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void pretty_print(TreeNode* root, char* pre, int depth){
    for(int i=0;i<depth;i++){
        printf("%c", pre[i]);
    }
    printf("%d\n", root->data);
    for (int i = 0; i < depth * 3; i += 3) { // 每次检查三个字符
        if (strncmp(pre + i, "├──", 9) == 0) {
            strncpy(pre + i, "│  ", 9); // 替换为 "│  "
        } else if (strncmp(pre + i, "└──", 9) == 0) {
            strncpy(pre + i, "   ", 9); // 替换为 "   "
        }
    }
    if(root->left != NULL){
        strcpy(pre + depth, "├──");
        depth += LEN;
        pretty_print(root->left, pre, depth);
        depth -= LEN;
    }
    if(root->right != NULL){
        strcpy(pre + depth, "└──");
        depth += LEN;
        pretty_print(root->right, pre, depth);
        depth -= LEN;
    }
}

int main(){
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->left->right->left = createNode(10);
    root->left->right->right = createNode(11);
    root->right->left = createNode(6);
    root->right->left->left = createNode(8);
    root->right->left->right = createNode(9);
    root->right->right = createNode(7);
    char pre[100];
    pretty_print(root, pre, 0);
    return 0;
}