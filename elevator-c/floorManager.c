#include "floorManager.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 全局楼层数组
Floor global_floorList[1000];

void FloorManager_init(FloorManager* fm) {
    fm->floorCount = 5;
    for (int i = 1; i <= fm->floorCount; i++) {
        Floor_init(&global_floorList[i-1], i); 
    }
}

void FloorManager_generateRandomPeople(FloorManager* fm) {
    // uniform_int(5)：产生1~5之间的随机数
    int num = uniform_int(5);
    // 创建一个 People，initPeople(&p, initFloor, valid=1)
    People p;
    initPeople(&p, num, 1);

    // 将该人加入对应的楼层
    Floor* floor = &global_floorList[num - 1];

    // 在之前 floor.c 的逻辑中，people保存在 global_peopleList中
    // 新增一人则直接放在 global_peopleList[floor->peopleNum]中
    // 并且 peopleNum++
    if (floor->peopleNum < 1000) {
        global_peopleList[floor->peopleNum] = p;
        floor->peopleNum++;
    }
}

bool FloorManager_checkUp(FloorManager* fm, int x) {
    // x是当前楼层号，若 x == 5，无上层
    if (x == 5) {
        return false;
    }
    // 检查从x开始往上的楼层
    for (int i = x; i < fm->floorCount; i++) {
        // floorCount=5，楼层从0到4存储，对应1~5楼
        Floor now = global_floorList[i];
        if (now.peopleNum > 0) {
            return true;
        }
    }
    return false;
}

bool FloorManager_checkDown(FloorManager* fm, int x) {
    // x是当前楼层号，若 x == 1，无下层
    if (x == 1) {
        return false;
    }
    // 检查从x-2到0的楼层(因x-1为当前楼层的数组下标)
    for (int i = x - 2; i >= 0; i--) {
        Floor now = global_floorList[i];
        if (now.peopleNum > 0) {
            return true;
        }
    }
    return false;
}

People FloorManager_popPeople(FloorManager* fm, int floorNum) {
    // floorNum是楼层号，从1开始，对应数组下标 floorNum-1
    return Floor_pop(&global_floorList[floorNum - 1]);
}

bool FloorManager_isVoid(FloorManager* fm) {
    bool isnull = false;
    for (int i = 0; i < fm->floorCount; i++) {
        Floor* floor = &global_floorList[i];
        // 使用 Floor_check 判断该楼层是否有人
        // Floor_check 返回 false 表示没有人
        if (!Floor_check(floor)) {
            // 任意一层没人则isnull = true
            isnull = true;
        }
    }
    return isnull;
}




















// #include "floorManager.h"
// #include "people.h"


// FloorManager::FloorManager() {
//     for (int i = 1; i <= 5; i++) {
//         Floor floor = Floor(i);
//         FloorList.push_back(floor);
//     };
// };

// // used to generateRandomPeople in runtime
// void FloorManager::generateRandomPeople() { int num = uniform_int(5);
//     People people = People(num);
//     FloorList[num - 1].peopleList.push_back(people);
//     FloorList[num - 1].peopleNum++;
// }

// // x is current floor num
// bool FloorManager::checkUp(int x) {
//     if (x == 5) {
//         return false;
//     }
//     for (int i = x-1; i < 5; i++) {
//         Floor now = this->FloorList[i];
//         if (now.peopleNum > 0) {
//             return true;
//         }
//     }

//     return false;
// }
// bool FloorManager::checkDown(int x) {
//     if ( x == 1 ) {
//         return false;
//     }

//     for (int i = x - 1; i >= 0; i--) {
//         Floor now = this->FloorList[i];
//         if (now.peopleNum > 0) {
//             return true;
//         }
//     }

//     return false;
// }

// People FloorManager::popPeople(int floorNum) {
//     // it must not empty because FloorManager init to have them
//     People people = this->FloorList[floorNum - 1].pop();
//     return people;
// }

// bool FloorManager::isVoid() {
//     bool isnull = false;
//     for(auto floor : FloorList) {
//         if (!floor.check()) {
//             isnull = true;
//         }
//     }
//     return isnull;
// }









