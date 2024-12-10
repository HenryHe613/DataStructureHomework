#ifndef PEOPLE_H
#define PEOPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// 假设bool为int表示(0为假，非0为真)
typedef struct {
    int valid;
    int gotoFloor; 
    int initFloor; 
    int id;
    double weight;
    char status[50];
} People;

extern int peopleID;

// 全局日志数组作为vector替代，最大1000条日志，每条日志长度100
extern char logs_global[1000][100];

// 初始化People，相当于原来的构造函数
void initPeople(People* p, int initFloor, int valid);

// 生成随机体重（正态分布）
double people_generateWeight(People* p);

// 生成随机目标楼层
int people_generateFloor(People* p);

// 日志打印函数
void peopleLog(People* p);

// 离开日志打印函数
void peopleLeaveLog(People* p, int floorNum);

// 保留未知函数和用法
// void printLogWithMargin(int x, int y, char logs[][100]);

#endif















// #pragma once

// #include <iostream>
// #include <string>

// // property: gotoFloor, initFloor, weight, status.
// class People {
//   public:
    
//     People(int initFloor, bool valid=true); // init people, which gengerates people id, weight, gotoFloor. will be called by class floor.
    
//     bool valid = true;

//     int gotoFloor; // 到哪个楼层
//     int initFloor; // 初始楼层
//     int id = 1;
//     double weight;
//     std::string status = "waiting";
//     // 生成随机体重
//     double generateWeight();

//     int generateFloor();

//     void peopleLog();

//     void peopleLeaveLog(int floorNum);

//     static int peopleID;
// };
