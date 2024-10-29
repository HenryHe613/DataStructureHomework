#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

const int NMAX = 50;
int matrix[NMAX][NMAX];
int n=0;
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

void print(int x, int y, char* s){
    printf("\033[%d;%dH%s", x+1, y*2+1, s);
    fflush(stdout);
    usleep(40000);
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
    print(stack[stack_depth-1][0], stack[stack_depth-1][1], "☒ ");
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
// typedef struct queue queue;
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
}
void queue_pop(int* x, int* y){
    if(head==NULL){
        x = NULL;
        y = NULL;
        return;
    }
    queue* temp = head;
    head = head->next;
    *x = temp->x;
    *y = temp->y;
    free(temp);
}

void refresh(){
    if(stack_depth>1)
        print(stack[stack_depth-2][0], stack[stack_depth-2][1], direction_arrows[stack[stack_depth-1][2]]);
    if(stack_depth>0)
        print(stack[stack_depth-1][0], stack[stack_depth-1][1], "㊣");
}

int go(int x,int y){
    matrix[x][y] = 2;
    refresh();
    if(x==n-2 && y==n-1)
        return 1;
    for(int i=0;i<4;i++){
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        if(nx<0 || nx>=n || ny<0 || ny>=n)
            continue;
        if(matrix[nx][ny]==0){
            stack_push(nx, ny, i);
            if(go(nx, ny)) return 1;
            stack_pop();
            matrix[nx][ny]=3;
            refresh();
        }
    }
    return 0;
}

void solve_manual_matrix(){
    clearScreen();
    printf("Remember: 1 for blocks and 0 for paths.\nPlease input the size of the matrix: ");
    scanf("%d", &n);
    for(int i=0;i<NMAX;i++)
        for(int j=0;j<NMAX;j++)
            matrix[i][j]=1;
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            scanf("%d", &matrix[i][j]);
    usleep(100000);
    clearScreen();
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(matrix[i][j]==1)
                printf("██");
            else
                printf("  ");
        }
        printf("\n");
    }
    printf("Start\n");
    go(1, 0);
    print(n, 0, "\n");
    stack_print();
    printf("Program end.");
    return;
}

void solve_random_matrix(){
    int n;
    printf("Please input the size of the matrix: ");
    scanf("%d", &n);

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            matrix[i][j] = 1;
}

void init_matrix(int n){
    if(n>NMAX)
        n = NMAX;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            matrix[i][j] = 1;
}

void print_matrix(int n){
    if(n>NMAX)
        n = NMAX;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(matrix[i][j]==1)
                printf("██");
            else
                printf("  ");
            fflush(stdout);
            usleep(5000);
        }
        printf("\n");
    }
}

void generate_matrix(int n){
    if(n>NMAX)
        n = NMAX;
    int begin_x = 1, begin_y = 0;
    int end_x = n-2, end_y = n-1;
    // matrix[begin_x][begin_y] = 0;
    // matrix[end_x][end_y] = 0;
    int go_directions[4][2] = {{0,-1},{-1,0},{0,1},{1,0}};
    int test_directions[4][2] = {{-1,-1},{-1,1},{1,1},{1,-1}};
    int detect_directions[9][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, 0}};
    int now_x = begin_x, now_y = begin_y;
    while(now_x!=end_x || now_y!=end_y){
        int next_x, next_y;
        int arr_test[4] = {1,1,1,1};
        for(int i=0;i<4;i++){
            int test_x = now_x + test_directions[i][0];
            int test_y = now_y + test_directions[i][1];
            if(matrix[test_x][test_y]==0){
                arr_test[i]=0;
                arr_test[(i+1)%4]=0;
            }
        }
        int flag = 1;
        for(int i=0;i<4;i++){
            if(arr_test[i]==1){
                flag = 0;
            }
        }
        if(flag) return;
        do{
            int direction = rand()%4;
            while(arr_test[direction]==0)
                direction = rand()%4;
            next_x = now_x + go_directions[direction][0];
            next_y = now_y + go_directions[direction][1];
        }while(next_x != now_x && next_y != now_y || !(next_x>0 && next_x<n-1 && next_y>0 && next_y<n-1));
        
        // for(int i=0;i<9;i++){
        //     if(next_x+detect_directions[i][0]<1 || next_x+detect_directions[i][0]>=n-1 || next_y+detect_directions[i][1]<1 || next_y+detect_directions[i][1]>=n-1)
        //         continue;
        //     if(matrix[next_x+detect_directions[i][0]][next_y+detect_directions[i][1]]==0)
        //         continue;
        // }
        matrix[next_x][next_y] = 0;
        print(next_x, next_y, "  ");
        now_x = next_x;
        now_y = next_y;
    }
}

int main(){
    srand(time(NULL));  // 设置随机数种子
    clearScreen();
    // printf("Choose a mode.\n1. Manual input\n2. Random input\nYour choice: ");
    // int mode = 0;
    // scanf("%d", &mode);
    // if(mode==1)
    //     solve_manual_matrix();
    // else if(mode==2){
    //     solve_random_matrix();
    // }
    int n = 20;
    init_matrix(n);
    print_matrix(n);
    generate_matrix(n);
    return 0;
}