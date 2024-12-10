#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

int terminalWidth, terminalHeight;
struct block{
    int x, y;
    int width, height;
} elevator, rt_msg, rules, passenger;

// 终端输出
void screenClear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 在指定位置打印字符串
void screenPrint(int x, int y, char* s){
    printf("\033[%d;%dH%s", x, y*2-1, s);
    fflush(stdout);
}

// 获取终端大小
void getTerminalSize(int* rows, int* cols) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int ret;
    ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    if (ret) {
        *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        *cols = 80; // 默认宽度
        *rows = 25; // 默认高度
    }
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *cols = w.ws_col;
    *rows = w.ws_row;
#endif
}

// 隐藏光标
void hideCursor() {
    printf("\033[?25l");
    fflush(stdout);
}

void printFloor(int left, int right, int floorHeight, int floorCount){
    floorHeight++;
    for(int i=0;i<=floorCount;i++){
        for(int j=left;j<=right;j++){
            screenPrint(i*floorHeight, j, "██");
        }
    }
}

int screenInit(){
    screenClear();
    printf("请输入终端大小:\n");
    scanf("%d %d", &terminalHeight, &terminalWidth);
    int nowTerminalHeight, nowTerminalWidth;
    getTerminalSize(&nowTerminalHeight, &nowTerminalWidth);
    if(nowTerminalHeight < terminalHeight || nowTerminalWidth < terminalWidth*2){
        printf("终端尺寸过小，请调整终端尺寸。\n");
        return 1;
    }
    int rows = terminalHeight, cols = terminalWidth/2;
    for(int i=1;i<=rows;i++){
        screenPrint(i,1,"░░");
        screenPrint(i,cols,"░░");
    }
    for(int i=1;i<=cols;i++){
        screenPrint(1,i,"░░");
        screenPrint(rows, i, "░░");
    }
    return 0;
}

int main(){
}