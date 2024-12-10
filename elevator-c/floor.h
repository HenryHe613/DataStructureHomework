#ifndef FLOOR_H
#define FLOOR_H

#include "people.h"
#include "utility.h"
#include <stdio.h>
#include <stdbool.h> // 使用bool类型

// Floor结构体，取代原class Floor
typedef struct {
    int peopleMax;
    int peopleNum;
} Floor;

// 全局数组替代原 vector<People> peopleList
// 最大容量1000
extern People global_peopleList[1000];

// 初始化Floor，相当于原构造函数 Floor(int floorNum)
void Floor_init(Floor* f, int floorNum);

// 对应原pop方法
People Floor_pop(Floor* f);

// 对应原drawFloor方法
void Floor_drawFloor(Floor* f);

// 对应原check方法
bool Floor_check(Floor* f);

#endif

















// #pragma once
// #include "people.h"
// #include "utility.h"
// #include <vector>


// // property: peopleMax, peopleList
// class Floor {
//   public:
//     int peopleMax = 5;
//     int peopleNum = 0;
//     std::vector<People> peopleList;
//     Floor(int floorNum);
    
//     People pop();

//     // frontend functions.
//     void drawFloor();

//     bool check();
// };

