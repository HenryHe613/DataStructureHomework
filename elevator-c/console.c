#include "console.h"
#include "draw.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
// Windows 下通过 GetConsoleScreenBufferInfo 获取控制台大小
#else
#include <unistd.h>
// Linux 下无直接API获取控制台大小，简单设定默认值或使用 ANSI 询问
#endif

const int xPassengerConsoleStart = BORDERWIDTH + ELEVATOR_WIDTH + FLOOR_WIDTH + 1;
const int yPassengerConsoleStart = BORDERWIDTH;

// 全局变量：记录当前打印位置
static int currentY = 0;

void printPassengerLog(const char *s) {
    int x = xPassengerConsoleStart;
    int y = yPassengerConsoleStart;
    DrawString(s, x, y);
}

// 清屏函数
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 获取控制台大小的函数（若需要）
void getConsoleSize(int *maxWidth, int *maxHeight) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        *maxWidth = csbi.dwSize.X;
        *maxHeight = csbi.dwSize.Y;
    } else {
        *maxWidth = 80;
        *maxHeight = 24;
    }
#else
    // 简单假设
    *maxWidth = 80;
    *maxHeight = 24;
#endif
}

// 模拟日志输出的函数
void printLogWithMargin(int x, int y, char logs[][100], int logs_count) {
    int maxWidth, maxHeight;
    getConsoleSize(&maxWidth, &maxHeight);

    // 确保起始位置合法
    if (x < 0 || y < 0 || x >= maxWidth || y >= maxHeight) {
        fprintf(stderr, "Error: Invalid margins (x or y out of bounds).\n");
        return;
    }

    // 如果 currentY 小于 y，初始化到指定顶部间距
    if (currentY < y) {
        currentY = y;
    }

    for (int i = 0; i < logs_count; i++) {
        if (currentY >= maxHeight) {
            fprintf(stderr, "\nError: Exceeded console height limit. Clearing screen...\n");
            clearScreen();
            currentY = y;
        }

        setCursorPositionWithGrid(x, currentY);

        // 打印时防止超宽
        char buffer[200];
        int availableWidth = maxWidth - x;
        if (availableWidth < 0) availableWidth = 0;

        // 截断字符串
        strncpy(buffer, logs[i], (size_t)availableWidth);
        buffer[availableWidth] = '\0';

        printf("%s", buffer);

        // 模拟延迟
#ifdef _WIN32
        Sleep(200);
#else
        usleep(200000); // 200毫秒
#endif

        currentY++;
    }
}

















// #include "console.h"
// #include "draw.h"
// #include <chrono>
// #include <thread>

// // 在乘客信息栏打印乘客信息
// void printPassengerLog(const char *s) {
//     int x = xPassengerConsoleStart;
//     int y = yPassengerConsoleStart;
//     DrawString(s, x, y);
// }
// // 全局变量：记录当前打印位置
// int currentY = 0;

// // 模拟日志输出的函数
// void printLogWithMargin(int x, int y, const std::vector<std::string>& logs) {
//     // 获取窗口大小，防止越界
//     CONSOLE_SCREEN_BUFFER_INFO csbi;
//     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//     GetConsoleScreenBufferInfo(hConsole, &csbi);
//     int maxWidth = csbi.dwSize.X;  // 最大宽度
//     int maxHeight = csbi.dwSize.Y; // 最大高度

//     // 确保起始位置合法
//     if (x < 0 || y < 0 || x >= maxWidth || y >= maxHeight) {
//         std::cerr << "Error: Invalid margins (x or y out of bounds).\n";
//         return;
//     }

//     // 如果 currentY 小于 y，初始化到指定顶部间距
//     if (currentY < y) {
//         currentY = y;
//     }

//     // 遍历日志内容逐行输出
//     for (const auto& log : logs) {
//         if (currentY >= maxHeight) {
//             std::cerr << "\nError: Exceeded console height limit. Clearing screen...\n";
//             system("cls"); // 清屏
//             currentY = y;  // 重置打印位置
//         }

//         // 设置光标位置，并输出日志
//         setCursorPosition(x, currentY);
//         std::cout << log.substr(0, maxWidth - x); // 防止超出屏幕宽度

//         // 模拟延迟（类似 apt install 的逐行更新效果）
//         std::this_thread::sleep_for(std::chrono::milliseconds(200));

//         currentY++; // 移动到下一行
//     }
// }
