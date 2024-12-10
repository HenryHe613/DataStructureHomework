#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "draw.h"
#include "global.h"
#include "people.h"
#include <stdio.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// 定义电梯结构体
typedef struct {
    char elevatorDirection; 
    int elevatorPositionX;
    int elevatorPositionY;
    int currentFloor;

    // 替换 vector<People> peopleList 为一个定长数组和计数器
    People peopleList[1000];
    int peopleCount;
} Elevator;

// 全局日志数组（替代 vector<std::string>），1000行，每行100字符
extern char elevator_log_global[1000][100];
extern int elevator_log_count;

// 全局 gone 数组（替代 vector<People> gone）
extern People elevator_gone_global[1000];
extern int elevator_gone_count;

// 电梯相关函数
void Elevator_init(Elevator* e);
void Elevator_drawElevatorWithCoordinates(Elevator* e, int x, int y);
void Elevator_deleteElevatorWithCoordinates(Elevator* e, int x, int y);
void Elevator_drawElevator(Elevator* e);
void Elevator_deleteElevator(Elevator* e);
void Elevator_goDownElevator(Elevator* e);
void Elevator_goUpElevator(Elevator* e);
void Elevator_goDownFloor(Elevator* e);
void Elevator_goUpFloor(Elevator* e);
void Elevator_closeDoor(Elevator* e);
void Elevator_drawElevatorDirection(Elevator* e);
void Elevator_go(Elevator* e, char direction);
void Elevator_changeDirection(Elevator* e);
void Elevator_pushPeople(Elevator* e, People people);
bool Elevator_isVoid(Elevator* e);
int Elevator_clean(Elevator* e, int floorNum); // 返回gone数量，并且 gone 信息存储在全局数组中

// 辅助睡眠函数
static inline void elevator_sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

#endif



















// #pragma once
// #include "draw.h"
// #include "global.h"
// #include <chrono>
// #include <thread>
// #include <vector>
// #include "draw.h"
// #include "global.h"
// #include "people.h"

// class Elevator {
//     public:
//     char elevatorDirection = 'd'; // direcion of elevator
//     int elevatorPositionX = 1;
//     int elevatorPositionY = 1;
//     int currentFloor = 5;

//     std::vector<People> peopleList; // people on that elevator
//     // init function for elevator. It needs to draw elevator.
//     Elevator() ;
    
//     void drawElevator();
//     void drawElevatorWithCoordinates(int x, int y);
//     void deleteElevator();
//     void deleteElevatorWithCoordinates(int x, int y);
//     // 电梯下移5格（调用5次goDownElevator）
//     void goDownFloor();

//     // 电梯上移5格（调用5次goUpElevator）
//     void goUpFloor();

//     // 电梯关门
//     void closeDoor();

//     // 绘制电梯方向
//     void drawElevatorDirection();

//     // 根据方向自动运行, the direction won't change, but will move to the first place have people
//     void go(char direction);

//     void changeDirection();

//     void pushPeople(People people);

//     bool isVoid();

//  std::vector<People> clean(int floorNum);   
// private:
//     // go only one floor; must be private.
//     void goDownElevator();
//     void goUpElevator();
// };


