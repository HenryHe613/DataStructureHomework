#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

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
    printf("\033[%d;%dH%s", x+1, y+1, s);
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
    int cols, rows;
    getTerminalSize(&rows, &cols);
    if(rows < 20 || cols < 40){
        printf("终端尺寸过小，请调整终端尺寸。\n");
        fflush(stdout);
        usleep(2000);
        return 1;
    }
    for(int i=0;i<rows;i++){
        screenPrint(i,0,"░░");
    }
    for(int i=0;i<cols;i++){
        screenPrint(0,i,"░░");
    }
    int elevatorWidth = cols/4;
    int elevatorHeight = 5;
    int floorHeight = 5;
    int floorWidth = cols - elevatorWidth;
    printFloor(5,10,2,4);
    printf("%d %d",cols,rows);
    screenPrint(20,20,"░░");
    return 0;
}

int main(){
    screenClear();
    for(int i=1;i<80;i++){
        screenPrint(0,i,"─");
        screenPrint(32, i, "─");
    }
    for(int i=1;i<30;i++){
        screenPrint(i,0,"│");
        screenPrint(i,84,"│");
    }
    screenPrint(0,0,"┌");
    screenPrint(0,84,"┐");
    screenPrint(32,0,"└");
    screenPrint(32,84,"┘");
    screenPrint(34, 10, "End\n");
}