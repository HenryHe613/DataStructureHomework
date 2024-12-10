#include "init.h"

void setConsoleUTF8() {
#ifdef _WIN32
    // 在Windows下设置控制台编码为UTF-8
    SetConsoleOutputCP(CP_UTF8); 
    SetConsoleCP(CP_UTF8);      
#else
    // Linux下一般终端已支持UTF-8，无需特殊设置
    // 如需，可尝试使用setlocale等函数
    // setlocale(LC_ALL, "");
#endif
}

void enableANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        // 无法获取控制台句柄
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        // 无法获取控制台模式
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#else
    // Linux下一般终端已支持ANSI转义序列，无需额外操作
#endif
}

void setConsoleSize(int columns, int rows) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: Unable to get console handle\n");
        return;
    }

    // 获取控制台支持的最大窗口大小
    COORD maxSize = GetLargestConsoleWindowSize(hOut);
    if (columns > maxSize.X || rows > maxSize.Y) {
        fprintf(stderr, "Error: Requested size exceeds maximum console window size.\n");
        return;
    }

    // Step 1: 缓冲区大小设置
    COORD bufferSize;
    bufferSize.X = (SHORT)columns;
    bufferSize.Y = (SHORT)rows;

    if (!SetConsoleScreenBufferSize(hOut, bufferSize)) {
        fprintf(stderr, "Error: Unable to set console screen buffer size\n");
        return;
    }

    // Step 2: 设置窗口大小
    SMALL_RECT windowSize;
    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = (SHORT)(columns - 1);
    windowSize.Bottom = (SHORT)(rows - 1);

    if (!SetConsoleWindowInfo(hOut, TRUE, &windowSize)) {
        fprintf(stderr, "Error: Unable to set console window size\n");
        return;
    }
#else
    // Linux下无法直接用Windows API设置终端大小
    // 可通过ANSI转义序列尝试控制(仅某些终端兼容)
    // 一般可忽略或使用 stty 命令行工具，但不是跨平台的可靠方案。
    // 这里忽略
    (void)columns;
    (void)rows;
#endif
}

void Init() {
    enableANSI();
    // 假设 HEIGHT 和 WIDTH 在 global.h 中定义
    setConsoleSize(HEIGHT * 2, WIDTH);
}




















// #include "init.h"
// #include <Windows.h>
// #include <consoleapi.h>

// void setConsoleUTF8() {
//     SetConsoleOutputCP(CP_UTF8); // 设置输出编码为 UTF-8
//     SetConsoleCP(CP_UTF8);       // 设置输入编码为 UTF-8
// }

// void enableANSI() {
//     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//     if (hOut == INVALID_HANDLE_VALUE)
//         return;

//     DWORD dwMode = 0;
//     if (!GetConsoleMode(hOut, &dwMode))
//         return;

//     dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
//     SetConsoleMode(hOut, dwMode);
// }

// void setConsoleSize(int columns, int rows) {
//     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//     if (hOut == INVALID_HANDLE_VALUE) {
//         std::cerr << "Error: Unable to get console handle" << std::endl;
//         return;
//     }

//     // 获取控制台支持的最大窗口大小
//     COORD maxSize = GetLargestConsoleWindowSize(hOut);
//     if (columns > maxSize.X || rows > maxSize.Y) {
//         std::cerr
//             << "Error: Requested size exceeds maximum console window size."
//             << std::endl;
//         return;
//     }

//     // Step 1: 缓冲区大小设置
//     COORD bufferSize;
//     bufferSize.X = columns;
//     bufferSize.Y = rows;

//     if (!SetConsoleScreenBufferSize(hOut, bufferSize)) {
//         std::cerr << "Error: Unable to set console screen buffer size"
//                   << std::endl;
//         return;
//     }

//     // Step 2: 设置窗口大小
//     SMALL_RECT windowSize;
//     windowSize.Left = 0;
//     windowSize.Top = 0;
//     windowSize.Right = columns - 1;
//     windowSize.Bottom = rows - 1;

//     if (!SetConsoleWindowInfo(hOut, TRUE, &windowSize)) {
//         std::cerr << "Error: Unable to set console window size" << std::endl;
//         return;
//     }
// }

// void Init() {
//     enableANSI();
//     setConsoleSize(HEIGHT * 2, WIDTH);
// }