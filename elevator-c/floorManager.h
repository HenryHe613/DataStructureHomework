#ifndef FLOORMANAGER_H
#define FLOORMANAGER_H

#include "floor.h"
#include "people.h"
#include <stdbool.h>

// FloorManager结构体
typedef struct {
    int floorCount; // 实际楼层数量，默认5层
} FloorManager;

// 全局数组替代 std::vector<Floor> FloorList
// 假设最多1000层
extern Floor global_floorList[1000];

// 初始化FloorManager，相当于原构造函数
void FloorManager_init(FloorManager* fm);

// 用于在运行时生成随机people
void FloorManager_generateRandomPeople(FloorManager* fm);

// 检查往上方向是否有楼层有人
bool FloorManager_checkUp(FloorManager* fm, int x);

// 检查往下方向是否有楼层有人
bool FloorManager_checkDown(FloorManager* fm, int x);

// 从指定楼层弹出一位乘客
People FloorManager_popPeople(FloorManager* fm, int floorNum);

// 检查是否所有楼层均无人员
bool FloorManager_isVoid(FloorManager* fm);

#endif


















// #pragma once

// #include "floor.h"
// #include "people.h"
// #include "vector"

// class FloorManager {
// public:
//     std::vector<Floor> FloorList;

//     // this is the function that creates the floors.
//     FloorManager();

//     void generateRandomPeople();

//     bool checkUp(int x);

//     bool checkDown(int x);

//     People popPeople(int floorNum);

//     bool isVoid();
// };
