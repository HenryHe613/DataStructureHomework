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

// string

void read_dynamic_string(char **str) {
    // if(*str != NULL) free(*str);
    int capacity = 10;
    int length = 0;
    char temp;
    *str = malloc(capacity * sizeof(char));
    if(*str == NULL) return;
    while((temp = getchar()) != '\n'){
        if(length + 1 >= capacity){
            capacity += 10;
            char *new_str = realloc(*str, capacity * sizeof(char));
            if(new_str == NULL){
                free(*str);
                *str = NULL;
                return;
            }
            *str = new_str;
        }
        (*str)[length++] = temp;
    }
    (*str)[length] = '\0';
}

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
    char *name;
    char *school;
    char *major;
    char *class;
    Student* next;
};
Student* stu_head;

void addStudent(){
    clearScreen();
    moveCursor(1, 1);
    Student* stu = (Student*)malloc(sizeof(Student));
    printf("请输入学生姓名: ");
    read_dynamic_string(&stu->name);
    printf("请输入学生学校: ");
    read_dynamic_string(&stu->school);
    printf("请输入学生专业: ");
    read_dynamic_string(&stu->major);
    printf("请输入学生班级: ");
    read_dynamic_string(&stu->class);
    Student* p = stu_head;
    while(p->next!=NULL){
        p = p->next;
    }
    p->next = stu;
    stu->next = NULL;
}

void deleteStudent(){
    clearScreen();
    moveCursor(1, 1);
    printf("请输入学生姓名: ");
    char *name;
    read_dynamic_string(&name);
    Student* p = stu_head->next;
    while(p!=NULL){
        if(strcmp(p->name, name)==0){
            Student* q = p->next;
            p->next = q->next;
            free(q);
            printf("删除成功\n");
            slp(1000);
            return;
        }
        p = p->next;
    }
    printf("未找到该学生\n");
    slp(1000);
}

void modifyStudent(){
    clearScreen();
    moveCursor(1, 1);
    printf("请输入学生姓名: ");
    char *name;
    read_dynamic_string(&name);
    Student* p = stu_head->next;
    while(p!=NULL){
        if(strcmp(p->name, name)==0){
            printf("请输入学生学校: ");
            read_dynamic_string(&p->school);
            printf("请输入学生专业: ");
            read_dynamic_string(&p->major);
            printf("请输入学生班级: ");
            read_dynamic_string(&p->class);
            printf("修改成功\n");
            slp(1000);
            return;
        }
        p = p->next;
    }
    printf("未找到该学生\n");
    slp(1000);
}

void queryStudent(){
    clearScreen();
    moveCursor(1, 1);
    printf("请输入学生姓名: ");
    char *name;
    read_dynamic_string(&name);
    Student* p = stu_head->next;
    while(p!=NULL){
        if(strcmp(p->name, name)==0){
            printf("学生姓名: %s\n", p->name);
            printf("学生学校: %s\n", p->school);
            printf("学生专业: %s\n", p->major);
            printf("学生班级: %s\n", p->class);
            slp(1000);
            return;
        }
        p = p->next;
    }
    printf("未找到该学生\n");
    slp(1000);
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
            default:
                break;
        }
        slp(1000);
    }
    return 0;
}