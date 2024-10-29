#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#ifdef _WIN32
#include<windows.h>
#endif


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
    for(int i=0;i<depth*3;i+=3){
        if(strncmp(pre+i,"├──",9)==0){
            strncpy(pre+i,"│  ",9);
        }else if(strncmp(pre+i,"└──",9)==0){
            strncpy(pre+i,"   ",9);
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

TreeNode* get_parent_node(TreeNode* root, int location){
    if(location == 0){
        return NULL;
    }
    int depth = 0;
    int temp_location = location;
    while(temp_location){
        temp_location = (temp_location-1)/2;
        depth++;
    }
}

int main(){
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    int n;
    printf("Enter the size of nodes in the binary tree: ");
    scanf("%d", &n);

    int* arr = (int*)malloc(n*sizeof(int));
    printf("Enter the elements of the binary tree, \neach element should be divided by a space, \n-1 for empty node: \n");

    for(int i=0;i<n;i++){
        scanf("%d", &arr[i]);
    }

    TreeNode* root = createNode(arr[0]);
    
    char pre[20];
    pretty_print(root, pre, 0);
    printf("Press ENTER to exit...\n");
    getchar();
    return 0;
}
