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
// Linux 下使用 ANSI 转义序列
// 注意：ANSI 转义序列中的行列是从1开始计数，因此需要+1
// 光标移动序列：\033[row;colH
void setCursorPositionInternal(int x, int y) {
    printf("\033[%d;%dH", y+1, x+1);
}
#endif

// 标准光标位置设置，不带网格换算
void setCursorPosition(int x, int y) {
    setCursorPositionInternal(x, y);
}

// 带网格换算的光标位置设置(x*2,y)
void setCursorPositionWithGrid(int x, int y) {
    // x 方向乘以2，表示一个单位格子两个字符宽度
    setCursorPositionInternal(x * 2, y);
}

// 设置文本颜色
void setTextColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 设置绿色文本为例
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
    // Linux下使用ANSI颜色转义序列，设置绿色
    printf("\033[32m");
#endif
}

// 重置文本颜色
void resetTextColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 恢复默认
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    // Linux下重置颜色
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

// 在指定位置打印一个字符
void DrawChar(char c, int x, int y) {
    setCursorPositionWithGrid(x, y);
    printf("%c", c);
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

// debug用途
void DrawDebug(int x, int y) {
    DrawString("&&", x, y);
}




// #include "draw.h"
// #include <Windows.h>
// #include <stdio.h>

// // 原生设置鼠标位置
// void setCursorPosition(int x, int y) {
//     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//     COORD coord;
//     coord.X = x;
//     coord.Y = y;
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
// }

// // 用标准布局，设定鼠标位置
// void setCursorPositionWithGrid(int x, int y) {
//     COORD coord;
//     coord.X = x * 2;
//     coord.Y = y;
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
// }

// // 绘制垂直
// void drawVerticalLine(int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     printf("|");
// }
// // 绘制一个水平线(x, y)
// void drawHorizontalLine(int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     printf("==");
// }

// // 绘制一个方格（x, y）
// void drawBlock(int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     printf("*");
// }

// // 画一个墙
// void drawWallBlock(int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     printf("#");
// }

// // 删除一个方块（两个字符）
// void deleteGrid(int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     printf("  ");
// };

// // 在指定位置一个数字
// void drawNumber(int num, int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     printf("%d", num);
// }

// // 在指定位置打印一个字符
// void DrawChar(char c, int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     printf("%c", c);
// }

// // 在指定位置输出字符串
// void DrawString(const char s[], int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     printf("%s", s);
// }

// // ==========以下用于设置三角形实心倒三角：▲  △  ▼ ▽

// // 画一个正三角
// void DrawUpTriangle(int x, int y) {
//     setCursorPositionWithGrid(x, y);
//     DrawString("▲", x, y);
// }

// // 画一个倒三角
// void DrawDownTriangle(int x, int y) {
//     // setCursorPositionWithGrid(x, y);
//     DrawString("▼", x, y);
// }

// // debug用途
// void DrawDebug(int x, int y) {
//     // setCursorPositionWithGrid(x, y);
//     DrawString("&&", x, y);
// }