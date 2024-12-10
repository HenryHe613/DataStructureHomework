#include "elevator.h"
#include "window.h"
#include "floorManager.h"
#include "global.h"
#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {
    getchar();
    
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // 初始化窗口
    windowInit(); // 画边框等

    // 初始化楼层管理器
    FloorManager fm;
    FloorManager_init(&fm);

    // 初始化电梯
    Elevator e;
    Elevator_init(&e);

    // 准备日志数组（替代vector<string> v）
    // 当需要输出日志时用此数组
    char logs[1000][100];
    int logs_count = 0;

    for (int round = 0; round < 300; round++) {
        windowInit();
        if (FloorManager_isVoid(&fm) && Elevator_isVoid(&e)) {
            logs_count = 0;
            snprintf(logs[logs_count++], 100, "%s", "[info] All the passengers are gone!!!");
            printLogWithMargin(61, 1, logs, logs_count);
            return 0;
        }

        if (round % RT == 0) {
            FloorManager_generateRandomPeople(&fm);
        }

        int currentFloor = e.currentFloor;
        char currentDirection = e.elevatorDirection;

        bool peopleThere = false;
        if (currentDirection == 'u') {
            peopleThere = FloorManager_checkUp(&fm, currentFloor);
        } else if (currentDirection == 'd') {
            peopleThere = FloorManager_checkDown(&fm, currentFloor);
        }

        if (peopleThere) {
            Elevator_go(&e, currentDirection);
            People pickedPeople = FloorManager_popPeople(&fm, e.currentFloor);
            Elevator_clean(&e, currentFloor);
            if (pickedPeople.valid == 0) {
                // 人无效，继续下一回合
                continue;
            }
            Elevator_pushPeople(&e, pickedPeople);
        } else {
            Elevator_changeDirection(&e);
            continue;
        }
    }

    return 0;
}




























// #include "elevator.h"
// #include "window.h"
// #include <cmath>
// #include <windows.h>
// #include <wingdi.h>
// #include "floorManager.h"
// #include "global.h"
// #include "console.h"

// // we need a class for elevator, a class for floor, and a class for all floors (floorManager).

// // we are going to make a structure
// int main() {
//     getchar();
    
//     system("cls");
//     // elevator 14 * 7
//     // floor 14 * 8
//     // console ??
//    windowInit();    // draw the window, only draws the border
    
//     FloorManager FM = FloorManager();   
//     // draw the floors, need to draw the floor, people on floor, floor number, init direction
//     Elevator E = Elevator(); // init and draw the elevator

//     // a recurser that continue, generate people based on round;
//     for (int round=0; round < 300; round ++) {
//         windowInit(); 
//         if (FM.isVoid() && E.isVoid()) {
//             std::vector<std::string> v;
//             v.push_back("[info] All the passengers are gone!!!");
//             printLogWithMargin(61, 1, v);
//             return 0;
//         }
//         if (round % RT == 0) {
//             FM.generateRandomPeople();
//         }
//         // fetch current elevator floor and direction
//         int currentFloor = E.currentFloor;
//         char currentDirection = E.elevatorDirection;

//         // is people on that direction?
//         bool peopleThere = false;
//         if (currentDirection == 'u') {
//             peopleThere = FM.checkUp(currentFloor); // it also need to check if we go out bound

//         } else if (currentDirection == 'd') {
//             peopleThere = FM.checkDown(currentFloor);
//         }

//         // if people there, pick them
//         // else change direction
//         if (peopleThere) { 
//             E.go(currentDirection); // auto change its direciton property.
//             People pickedPeople = FM.popPeople(E.currentFloor);
//             E.clean(currentFloor);
//             if (pickedPeople.valid == false) {
//                 continue;
//             }
//             E.pushPeople(pickedPeople);

//         } else {
//             E.changeDirection();
//             continue;
//         }
//     }
// }
