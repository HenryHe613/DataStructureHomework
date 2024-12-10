#include "utility.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <time.h>
#endif

// Initialize random number generator once
static int initialized = 0;

int uniform_int(int x) {
    if (!initialized) {
#ifdef _WIN32
        srand((unsigned int)GetTickCount());
#else
        srand((unsigned int)time(NULL));
#endif
        initialized = 1;
    }
    return (rand() % x) + 1;
}















// #include "utility.h"
// #include <random>

// int uniform_int(int x) {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dist(1, x);

//     int random_number = dist(gen);

//     return random_number;
//   }
