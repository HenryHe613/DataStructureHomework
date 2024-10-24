#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct ListData{
    int data;
    struct ListData* next;
};
typedef struct ListData ListData;

struct ListNode{
    int length;
    ListData* data;
    struct ListNode* next;
};
typedef struct ListNode ListNode;

ListData* createDataNode(int data){
    ListData* newNode=(ListData*)malloc(sizeof(ListData));
    newNode->data=data;
    newNode->next=NULL;
    return newNode;
}

ListNode* createListNode(){
    ListNode* newNode=(ListNode*)malloc(sizeof(ListNode));
    newNode->length=0;
    newNode->data=NULL;
    newNode->next=NULL;
    return newNode;
}

void freeDataNode(ListData* node){
    ListData* next=node->next;
    while(next!=NULL){
        free(node);
        node=next;
        next=next->next;
    }
    free(node);
}

void freeListNode(ListNode* node){
    freeDataNode(node->data);
    free(node);
}

ListNode* input(){
    ListNode* newListNode=createListNode();
    ListData* newDataNode=createDataNode(0);
    ListData* cur=newDataNode;
    char input_char;
    input_char=getchar();
    while(input_char!='\n'){
        if(input_char<'0' || input_char>'9'){
            while(input_char!='\n') input_char=getchar();
            freeDataNode(newDataNode);
            free(newListNode);
            return NULL;
        }else{
            cur->next=createDataNode(input_char-'0');
            cur=cur->next;
            newListNode->length++;
        }
        input_char=getchar();
    }
    newListNode->data=newDataNode->next;
    free(newDataNode);
    return newListNode;
}

void print(ListNode* node){
    ListData* dataNode=node->data;
    while(dataNode!=NULL){
        printf("%d",dataNode->data);
        dataNode=dataNode->next;
    }
    printf("\n");
}

void printListNode(ListNode* head){
    ListNode* cur=head->next;
    while(cur!=NULL){
        print(cur);
        cur=cur->next;
    }
}

int compare(ListNode* node1,ListNode* node2){
    if(node1->length!=node2->length) return 0;
    ListData* cur1=node1->data;
    ListData* cur2=node2->data;
    while(cur1!=NULL && cur2!=NULL){
        if(cur1->data!=cur2->data) return 0;
        cur1=cur1->next;
        cur2=cur2->next;
    }
    return 1;
}

int insert(ListNode* head,ListNode* nowNode){
    ListNode* cur=head->next;
    ListNode* pre=head;
    while(cur!=NULL){
        if(compare(cur,nowNode)==1) return 0;
        cur=cur->next;
        pre=pre->next;
    }
    pre->next=nowNode;
    return 1;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printInstruction(){
    printf("C Language Linked List Deduplication\n\n1. Print Linked List\n2. Store New Data\n3. Exit Program\n\nYour Operation: ");
}

void waitToReturn(){
    char temp;
    temp=getchar();
    while(temp!='\n') temp=getchar();
}

int main(){
    ListNode* head=createListNode();
    while(1){
        clearScreen();
        printInstruction();
        int type=0;
        scanf("%d",&type);
        getchar();
        clearScreen();
        if(type==1){
            printListNode(head);
            printf("Output complete, press Enter to continue...");
            waitToReturn();
        }else if(type==2){
            printf("Input the data to insert, press Enter to finish.\n");
            ListNode* newListNode=input();
            if(newListNode==NULL)
                printf("Please enter pure numbers, press Enter to continue...");
            else if(insert(head,newListNode)==1)
                printf("Successfully inserted, press Enter to continue...");
            else
                printf("Insertion failed due to a duplicate node, press Enter to continue...");
            waitToReturn();
        }else if(type==3){
            return 0;
        }
    }
    return 0;
}