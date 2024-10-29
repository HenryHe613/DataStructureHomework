#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
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
    // location从1开始, 表示完全二叉树的节点位置, 1表示根节点
    location++;
    if(location == 1){
        return NULL;
    }
    int node_depth = 0;
    int temp_location = location;
    while(temp_location){
        temp_location >>= 1;
        node_depth++;
    }
    TreeNode* parent = root;
    for(int i=node_depth-2;i>0;i--){
        if((location>>i)&1){
            parent = parent->right;
        }else{
            parent = parent->left;
        }
        if(parent == NULL){
            return NULL;
        }
    }
    return parent;
}

int get_max_width(TreeNode* root){
    // 递归获得二叉树最大度
    if(root == NULL){
        return 0;
    }
    int left_width = get_max_width(root->left);
    int right_width = get_max_width(root->right);
    int now_width = 0;
    if (root->left != NULL){
        now_width++;
    }
    if (root->right != NULL){
        now_width++;
    }
    now_width = now_width>left_width?now_width:left_width;
    now_width = now_width>right_width?now_width:right_width;
    return now_width;
}

int main(){
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    int n;
    printf("[INFO] Enter the size of nodes in the binary tree: ");
    scanf("%d", &n);
    if(n<=0){
        printf("[ERROR] The size of nodes should not be smaller than 0!\n");
        return 0;
    }

    int* arr = (int*)malloc(n*sizeof(int));

    printf("[INFO] Enter the elements of the binary tree, \n       each element should be divided by a space, \n       -1 for empty node: \n");
    for(int i=0;i<n;i++){
        scanf("%d", &arr[i]);
    }

    //创建根节点
    if(arr[0] == -1){
        printf("[ERROR] Root node cannot be empty!\n");
        return 0;
    }
    TreeNode* root = createNode(arr[0]);

    // 构建二叉树
    int deprecated_nodes_count = 0;
    for(int i=1;i<n;i++){
        if(arr[i] == -1){
            continue;
        }
        TreeNode* node = createNode(arr[i]);
        TreeNode* parent = get_parent_node(root, i);
        if (parent == NULL){
            deprecated_nodes_count++;
            continue;
        }else if((i&1) == 1){
            parent->left = node;
        }else{
            parent->right = node;
        }
    }
    printf("[ERROR] %d nodes are deprecated because no parent node found!\n", deprecated_nodes_count);
    
    char pre[200];
    printf("[INFO] The binary tree is: \n");
    pretty_print(root, pre, 0);

    printf("[INFO] The maximum width of the binary tree is: %d\n", get_max_width(root));

    printf("[INFO] Press ENTER to exit...\n");
    getchar();
    return 0;
}
