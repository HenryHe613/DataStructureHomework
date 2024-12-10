#ifndef INIT_H
#define INIT_H

#include "global.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <winnt.h>
#else
// Linux下无需导入windows相关头文件
#endif

void setConsoleUTF8();
void enableANSI();
void setConsoleSize(int columns, int rows);
void Init();

#endif









// #pragma once

// // 用于初始化
// #include "global.h"
// #include <iostream>

// void setConsoleUTF8();

// void enableANSI();

// void setConsoleSize(int columns, int rows);

// void Init();