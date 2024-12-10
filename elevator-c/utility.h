#ifndef UTILITY_H
#define UTILITY_H

#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <time.h>
#endif

// from 1 to x uniform_int distribution
int uniform_int(int x);

#endif // UTILITY_H








// #pragma once

// // from 1 to x uniform_int distribution
// int uniform_int(int x);
