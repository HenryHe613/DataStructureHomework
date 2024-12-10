#ifndef CONSOLE_H
#define CONSOLE_H

#include "global.h"
#include <stdio.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#endif

extern const int xPassengerConsoleStart;
extern const int yPassengerConsoleStart;

// 在乘客信息栏打印乘客信息
void printPassengerLog(const char *s);

// 使用全局数组替代vector<string>的日志打印函数
// logs: 二维char数组，每行100字符
// logs_count: 日志数量
void printLogWithMargin(int x, int y, char logs[][100], int logs_count);

#endif



















// #pragma once

// #include "draw.h"
// #include "global.h"
// #include <vector>
// #include <Windows.h>

// const int xPassengerConsoleStart =
//     BORDERWIDTH + ELEVATOR_WIDTH + FLOOR_WIDTH + 1;
// const int yPassengerConsoleStart = BORDERWIDTH;

// void printPassengerLog(const char *s);


// void testLog(const char* s);


// void printLogWithMargin(int x, int y, const std::vector<std::string> &logs);

