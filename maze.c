#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#ifdef _WIN32
#include <windows.h>
#define usleep(x) Sleep((x)/1000)
#endif
#define NMAX 50



int matrix[NMAX][NMAX];
int n=20;
int queue_size = 0;
// 1为墙，0为通路，2为走过的路，3为死路


int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
char* direction_arrows[] = {"→", "↓", "←", "↑"};


// 终端输出
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void print(int x, int y, char* s, int threads){
    // 打印并刷新
    printf("\033[%d;%dH%s", x+1, y*2+1, s);
    fflush(stdout);
    usleep(40000/threads);
}


void init_matrix(){
    // 初始化迷宫
    if(n>NMAX)
        n = NMAX;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            matrix[i][j] = 1;
}


void print_matrix(){
    // 打印完整迷宫
    clearScreen();
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(matrix[i][j]==1)
                printf("██");
            else
                printf("  ");
            fflush(stdout);
            usleep(800);
        }
        printf("\n");
    }
}


// 栈的实现
int stack_depth = 0;
int stack[NMAX*NMAX][3];
void stack_push(int x, int y, int direction){
    stack[stack_depth][0] = x;
    stack[stack_depth][1] = y;
    stack[stack_depth][2] = direction;
    stack_depth++;
}
void stack_pop(){
    print(stack[stack_depth-1][0], stack[stack_depth-1][1], "░░", 1);
    stack_depth--;
}
void stack_print(){
    int temp_count=0;
    for(int i=0;i<stack_depth;i++){
        if(temp_count++%5==0)
            printf("\n");
        printf("(%02d,%02d) -> ", stack[i][0], stack[i][1]);
        fflush(stdout);
        usleep(40000);
    }
    printf("\n");
}


// 队列的实现
typedef struct queue{
    int x,y;
    struct queue* next;
}queue;
queue* head = NULL;
queue* tail = NULL;
void queue_push(int x, int y){
    queue* temp = (queue*)malloc(sizeof(queue));
    temp->x = x;
    temp->y = y;
    temp->next = NULL;
    if(head==NULL){
        head = temp;
        tail = temp;
    }else{
        tail->next = temp;
        tail = temp;
    }
    queue_size++;
}
void queue_pop(int* x, int* y){
    if(head==NULL){
        *x = -1;
        *y = -1;
        return;
    }
    queue* temp = head;
    head = head->next;
    *x = temp->x;
    *y = temp->y;
    free(temp);
    queue_size--;
}


void refresh(){
    if(stack_depth>1)
        print(stack[stack_depth-2][0], stack[stack_depth-2][1], direction_arrows[stack[stack_depth-1][2]], 1);
    if(stack_depth>0)
        print(stack[stack_depth-1][0], stack[stack_depth-1][1], "㊣", 1);
}


int solve_matrix(int x,int y){
    // 递归解决迷宫
    matrix[x][y] = 2;
    refresh();
    if(x==n-2 && y==n-1)
        return 1;
    int random;
    int cur = 0;
    int arr[4] = {-1,-1,-1,-1};
    while(cur!=4){
        random = rand()%4;
        int i;
        for(i=0;i<cur;i++)
            if(arr[i]==random)
                break;
        if(i==cur) arr[cur++] = random;
    }
    for(int i=0;i<4;i++){
        int nx = x + directions[arr[i]][0];
        int ny = y + directions[arr[i]][1];
        if(nx<0 || nx>=n || ny<0 || ny>=n)
            continue;
        if(matrix[nx][ny]==0){
            stack_push(nx, ny, arr[i]);
            if(solve_matrix(nx, ny)) return 1;
            stack_pop();
            matrix[nx][ny]=3;
            refresh();
        }
    }
    return 0;
}


void input_matrix(){
    // 手动输入迷宫
    clearScreen();
    printf("Remember: 1 for blocks and 0 for paths.\nPlease input the size of the matrix: ");
    scanf("%d", &n);
    init_matrix();
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            scanf("%d", &matrix[i][j]);
    usleep(100000);
}


void find_next_step(int now_x, int now_y, int* next_x_ref, int* next_y_ref, int* status){
    *status = 1;
    int go_directions[4][2] = {{0,-1},{-1,0},{0,1},{1,0}};
    int detect_directions[4][2] = {{-1,-1},{-1,1},{1,1},{1,-1}};
    int available_directions[4] = {1,1,1,1};
    for(int i=0;i<4;i++){
        int test_x = now_x + go_directions[i][0];
        int test_y = now_y + go_directions[i][1];
        if(matrix[test_x][test_y]==0)
            available_directions[i]=0;
        else if(test_x == 0 || test_x == n-1 || test_y == 0 || test_y == n-1)
            available_directions[i]=0;
    }
    for(int i=0;i<4;i++){
        int test_x = now_x + detect_directions[i][0];
        int test_y = now_y + detect_directions[i][1];
        if(matrix[test_x][test_y]==0){
            available_directions[i]=0;
            available_directions[(i+1)%4]=0;
        }
    }
    int directions_count = 0;
    for(int i=0;i<4;i++)
        directions_count += available_directions[i];
    if(directions_count == 0) return;
    int next_x, next_y;
    while(1){
        int direction = rand()%4;
        while(available_directions[direction]==0)
            direction = rand()%4;
        next_x = now_x + go_directions[direction][0];
        next_y = now_y + go_directions[direction][1];
        if(next_x<1 || next_x>=n-1 || next_y<1 || next_y>=n-1){
            if(directions_count==1) return;
            else continue;
        }
        break;
    }
    matrix[next_x][next_y] = 0;
    print(next_x, next_y, "  ", queue_size);
    *next_x_ref = next_x;
    *next_y_ref = next_y;
    *status = 0;
    return;
}


void generate_matrix(){
    // 随机生成迷宫
    if(n>NMAX)
        n = NMAX;
    int begin_x = 1, begin_y = 0;
    int end_x = n-2, end_y = n-1;
    matrix[begin_x][begin_y] = 0;
    matrix[end_x][end_y] = 0;
    print(begin_x, begin_y, "▒▒", 1);
    print(end_x, end_y, "▒▒", 1);
    int now_x, now_y;
    int next_x, next_y, status;
    queue_push(1,0);
    queue_push(n-2,n-1);
    while(queue_size>0){
        int random = rand()%3;
        queue_pop(&now_x, &now_y);
        if(random==0){
            find_next_step(now_x, now_y, &next_x, &next_y, &status);
            if(status==0)
                queue_push(next_x, next_y);
        }
        find_next_step(now_x, now_y, &next_x, &next_y, &status);
        if(status==0)
            queue_push(next_x, next_y);
    }
}


int main(){
    #ifdef _WIN32
    #endif
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    DWORD mode;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
    #endif
    srand(time(NULL));
    clearScreen();
    printf("Choose a mode.\n1. Manual input\n2. Random input\nYour choice: ");
    int type = 0;
    scanf("%d", &type);
    getchar();
    if(type==1){
        input_matrix();
        print_matrix();
        solve_matrix(1,0);
        stack_print();
    }else if(type==2){
        clearScreen();
        printf("Please input the size of the matrix: ");
        scanf("%d", &n);
        init_matrix();
        print_matrix();
        generate_matrix();
        usleep(100000);
        solve_matrix(1,0);
        print(n+1,0,"\n", 1);
        stack_print();
    }
    printf("按任意键退出...\n");
    getchar(); // 等待用户输入
    return 0;
}