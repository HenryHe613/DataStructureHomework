#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif

// 红黑树的节点颜色定义
typedef enum { RED, BLACK } NodeColor;

// 红黑树节点结构
typedef struct RBTreeNode {
    int key; // 关键字
    NodeColor color; // 节点颜色
    struct RBTreeNode *left, *right, *parent; // 左孩子、右孩子、父节点
} RBTreeNode;

// 红黑树结构
typedef struct RBTree {
    RBTreeNode *root; // 树根
    RBTreeNode *nil; // 哨兵节点
} RBTree;

// 创建一个新的红黑树节点
RBTreeNode* createNode(int key, NodeColor color, RBTreeNode *nil) {
    RBTreeNode *node = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    node->key = key;
    node->color = color;
    node->left = nil;
    node->right = nil;
    node->parent = nil;
    return node;
}

// 初始化红黑树
RBTree* createRBTree() {
    RBTree *tree = (RBTree*)malloc(sizeof(RBTree));
    tree->nil = createNode(0, BLACK, NULL);
    tree->root = tree->nil;
    return tree;
}

// 左旋操作
void leftRotate(RBTree *tree, RBTreeNode *x) {
    RBTreeNode *y = x->right; // 取得x的右孩子
    x->right = y->left; // 将y的左孩子作为x的右孩子
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent; // 更新y的父节点
    if (x->parent == tree->nil) {
        tree->root = y; // 如果x是根节点，则更新根节点
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x; // 调整x和y的关系
    x->parent = y;
}

// 右旋操作
void rightRotate(RBTree *tree, RBTreeNode *y) {
    RBTreeNode *x = y->left; // 取得y的左孩子
    y->left = x->right; // 将x的右孩子作为y的左孩子
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent; // 更新x的父节点
    if (y->parent == tree->nil) {
        tree->root = x; // 如果y是根节点，则更新根节点
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

// 插入修复函数
void insertFixup(RBTree *tree, RBTreeNode *z) {
    // 先检查是否为根节点
    if (z == tree->root) {
        z->color = BLACK;
        return;
    }
    while (z != tree->root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBTreeNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            RBTreeNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// 插入节点
void insertRBTree(RBTree *tree, int key) {
    RBTreeNode *z = createNode(key, RED, tree->nil); // 新节点，初始为红色
    RBTreeNode *y = tree->nil;
    RBTreeNode *x = tree->root;
    while (x != tree->nil) { // 查找插入位置
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == tree->nil) {
        tree->root = z; // 新节点成为根节点
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED; // 新插入的节点为红色
    insertFixup(tree, z); // 修复红黑树性质
}

// 中序遍历红黑树
void inorderRBTree(RBTree *tree, RBTreeNode *node) {
    if (node != tree->nil) {
        inorderRBTree(tree, node->left);
        printf("%d ", node->key);
        inorderRBTree(tree, node->right);
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    RBTree *tree = createRBTree();

    insertRBTree(tree, 10);
    insertRBTree(tree, 20);
    insertRBTree(tree, 30);
    insertRBTree(tree, 40);
    insertRBTree(tree, 50);
    insertRBTree(tree, 25);

    printf("中序遍历：");
    inorderRBTree(tree, tree->root);
    printf("\n");

    return 0;
}
