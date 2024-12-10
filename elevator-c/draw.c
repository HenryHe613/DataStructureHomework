#include "draw.h"
#include <stdio.h>

// 在 Windows 下使用控制台 API，在 Linux 下使用 ANSI 转义序列
#ifdef _WIN32
void setCursorPositionInternal(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(hConsole, coord);
}
#else
void setCursorPositionInternal(int x, int y) {
    printf("\033[%d;%dH", y+1, x+1);
}
#endif

// 带网格换算的光标位置设置(x*2,y)
void setCursorPositionWithGrid(int x, int y) {
    setCursorPositionInternal(x * 2, y);
}

// 设置文本颜色
void setTextColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
    printf("\033[32m");
#endif
}

// 重置文本颜色
void resetTextColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    printf("\033[0m");
#endif
}

// 绘制垂直线
void drawVerticalLine(int x, int y) {
    setCursorPositionWithGrid(x, y);
    printf("|");
}

// 绘制水平线(此处假设用 "==" 表示水平线)
void drawHorizontalLine(int x, int y) {
    setCursorPositionWithGrid(x, y);
    printf("==");
}

// 绘制一个方格(*)
void drawBlock(int x, int y) {
    setCursorPositionWithGrid(x, y);
    printf("*");
}

// 画一个墙(#)
void drawWallBlock(int x, int y) {
    setCursorPositionWithGrid(x, y);
    printf("#");
}

// 删除一个方块(空格覆盖)
void deleteGrid(int x, int y) {
    setCursorPositionWithGrid(x, y);
    printf("  ");
}

// 绘制数字
void drawNumber(int num, int x, int y) {
    setCursorPositionWithGrid(x, y);
    printf("%d", num);
}

// 在指定位置输出字符串
void DrawString(const char s[], int x, int y) {
    setCursorPositionWithGrid(x, y);
    printf("%s", s);
}

// 画一个正三角(▲)
void DrawUpTriangle(int x, int y) {
    DrawString("▲", x, y);
}

// 画一个倒三角(▼)
void DrawDownTriangle(int x, int y) {
    DrawString("▼", x, y);
}