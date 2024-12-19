#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#endif

// screen

void moveCursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
}

void clearScreen(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void homeScreen(){
    clearScreen();
    moveCursor(1, 1);
    printf("学生管理系统\n");
    printf("1. 添加学生\n");
    printf("2. 删除学生\n");
    printf("3. 修改学生\n");
    printf("4. 查询学生\n");
    printf("5. 树形打印所有学生\n");
    printf("6. 保存记录\n");
    printf("7. 加载记录\n");
    printf("0. 退出程序\n");
    printf("你的操作: ");
}

void slp(int ms){ // sleep
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms*1000);
#endif
}

// student

typedef struct Student Student;
struct Student{
    char name[100];
    char school[100];
    char major[100];
    char class[100];
    Student* next;
};
Student* stu_head;

void addStudent(){
    Student* stu = (Student*)malloc(sizeof(Student));
    printf("请输入学生姓名: ");
    scanf("%s",stu->name);
    printf("请输入学生学校: ");
    scanf("%s",stu->school);
    printf("请输入学生专业: ");
    scanf("%s",stu->major);
    printf("请输入学生班级: ");
    scanf("%s",stu->class);
    Student* p = stu_head;
    while(p->next!=NULL){
        if(strcmp(p->next->name, stu->name)==0){
            printf("学生已存在\n");
            free(stu);
            return;
        }
        p = p->next;
    }
    p->next = stu;
    stu->next = NULL;
    printf("添加成功\n");
}

void deleteStudent(){
    printf("请输入学生姓名: ");
    char name[100];
    scanf("%s", name);
    Student* p = stu_head->next;
    Student* q = stu_head;
    while(p != NULL){
        if(strcmp(p->name, name) == 0){
            q->next = p->next;
            free(p);
            printf("删除成功\n");
            return;
        }
        q = p;        // 将 q 移动到当前的 p
        p = p->next;  // p 移动到下一个节点
    }
    printf("未找到该学生\n");
}

void modifyStudent(){
    printf("请输入学生姓名: ");
    char name[100];
    scanf("%s",name);
    Student* p = stu_head->next;
    while(p!=NULL){
        if(strcmp(p->name, name)==0){
            printf("请输入学生学校: ");
            scanf("%s",p->school);
            printf("请输入学生专业: ");
            scanf("%s",p->major);
            printf("请输入学生班级: ");
            scanf("%s",p->class);
            printf("修改成功\n");
            return;
        }
        p = p->next;
    }
    printf("未找到该学生\n");
}

void queryStudent(){
    printf("请输入学生姓名: ");
    char name[100];
    scanf("%s",name);
    Student* p = stu_head->next;
    while(p!=NULL){
        if(strcmp(p->name, name)==0){
            printf("学生姓名: %s\n", p->name);
            printf("学生学校: %s\n", p->school);
            printf("学生专业: %s\n", p->major);
            printf("学生班级: %s\n", p->class);
            printf("按下回车等待1秒返回主菜单\n");
            getchar();
            return;
        }
        p = p->next;
    }
    printf("未找到该学生\n");
}

void exchange(Student* a, Student* b){
    Student* temp = (Student*)malloc(sizeof(Student));
    strcpy(temp->name , a->name);
    strcpy(temp->school , a->school);
    strcpy(temp->major , a->major);
    strcpy(temp->class , a->class);
    strcpy(a->name , b->name);
    strcpy(a->school , b->school);
    strcpy(a->major , b->major);
    strcpy(a->class , b->class);
    strcpy(b->name , temp->name);
    strcpy(b->school , temp->school);
    strcpy(b->major , temp->major);
    strcpy(b->class , temp->class);
    free(temp);
}

void printStudent(){
    //sort
    Student* p = stu_head->next;
    if(p==NULL){
        printf("无学生信息\n");
        return;
    }
    while(p->next!=NULL){
        Student* cur1 = p;
        Student* cur2 = p->next;
        while(cur2!=NULL){
            if(strcmp(cur1->school, cur2->school)>0) exchange(cur1, cur2);
            else if(strcmp(cur1->school, cur2->school)==0 && strcmp(cur1->major, cur2->major)>0) exchange(cur1, cur2);
            else if(strcmp(cur1->school, cur2->school)==0 && strcmp(cur1->major, cur2->major)==0 && strcmp(cur1->class, cur2->class)>0) exchange(cur1, cur2);
            cur1 = cur1->next;
            cur2 = cur2->next;
        }
        p = p->next;
    }
    //print
    p = stu_head->next;
    printf("%s ── %s ── %s ── %s\n", p->school, p->major, p->class, p->name);
    Student* q = p;
    p = p->next;
    while(p!=NULL){
        if(strcmp(p->school, q->school)==0){
            if(strcmp(p->major, q->major)==0){
                if(strcmp(p->class, q->class)==0){
                    for(int i=0;i<strlen(p->school)+strlen(p->major)+strlen(p->class)+6;i++) printf(" ");
                    printf(" └─── %s\n", p->name);
                }else{
                    for(int i=0;i<strlen(p->school)+strlen(p->major)+2;i++) printf(" ");
                    printf(" └─── %s ── %s\n", p->class, p->name);
                }
            }else{
                for(int i=0;i<strlen(p->school)+4;i++) printf(" ");
                printf(" └─ %s ── %s ── %s\n",p->major, p->class, p->name);
            }
        }else{
            printf("%s ── %s ── %s ── %s\n", p->school, p->major, p->class, p->name);
        }
        q = p;
        p = p->next;
    }
    printf("按下回车等待1秒返回主菜单\n");
    getchar();
}

// save & load

void save(){
    FILE* fp = fopen("student.dat", "wb");
    if(fp==NULL){
        printf("文件打开失败\n");
        return;
    }
    Student* p = stu_head->next;
    while(p!=NULL){
        fwrite(p, sizeof(Student), 1, fp);
        p = p->next;
    }
    fclose(fp);
    printf("保存成功\n");
}

void load(){
    FILE* fp = fopen("student.dat", "rb");
    if(fp == NULL){
        printf("文件打开失败\n");
        return;
    }
    if(stu_head == NULL) {
        stu_head = (Student*)malloc(sizeof(Student));
        stu_head->next = NULL;
    }
    Student* p = stu_head;
    while(p->next != NULL) {
        p = p->next;
    }
    while(1){
        Student* stu = (Student*)malloc(sizeof(Student));
        if(fread(stu, sizeof(Student), 1, fp) != 1){
            free(stu);
            break;
        }
        p->next = stu;
        stu->next = NULL;
        p = stu;
    }
    fclose(fp);
    printf("加载成功\n");
}

// main

int main(){
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    DWORD mode;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
#endif
    int op;
    stu_head = (Student*)malloc(sizeof(Student)*100);
    while(1){
        homeScreen();
        scanf("%d", &op);
        getchar(); // eat '\n'
        // clearScreen();
        // moveCursor(1, 1);
        switch(op){
            case 1:
                addStudent();
                break;
            case 2:
                deleteStudent();
                break;
            case 3:
                modifyStudent();
                break;
            case 4:
                queryStudent();
                break;
            case 5:
                printStudent();
                break;
            case 6:
                save();
                break;
            case 7:
                load();
                break;
            case 0:
                return 0;
            default:
                break;
        }
        slp(1000);
    }
    return 0;
}