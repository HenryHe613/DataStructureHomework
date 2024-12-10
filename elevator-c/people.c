#include "people.h"
#include "console.h"

// 全局 peopleID
int peopleID = 1;

// 用于替代vector的全局数组
char logs_global[1000][100];

static double box_muller(double m, double s) {
    // Box-Muller变换产生标准正态分布
    double u = 0.0, v = 0.0, r = 0.0;
    while (r == 0.0 || r >= 1.0) {
        u = (double)rand() / RAND_MAX * 2.0 - 1.0;
        v = (double)rand() / RAND_MAX * 2.0 - 1.0;
        r = u*u + v*v;
    }
    double c = sqrt(-2.0 * log(r) / r);
    return m + u * c * s;
}

void initPeople(People* p, int initFloor, int valid) {
    // 初始化随机数种子(可根据需要只在程序入口初始化一次)
    srand((unsigned int)time(NULL));

    p->id = peopleID;
    peopleID++;

    p->weight = people_generateWeight(p);
    p->gotoFloor = people_generateFloor(p);
    p->initFloor = initFloor;
    p->valid = valid;
    snprintf(p->status, sizeof(p->status), "%s", "waiting");

    peopleLog(p);
}

double people_generateWeight(People* p) {
    // 正态分布，均值70，标准差10
    double mean = 70.0;
    double stddev = 10.0;

    double w = box_muller(mean, stddev);
    // 四舍五入
    double rounded_weight = floor(w + 0.5);
    return rounded_weight;
}

int people_generateFloor(People* p) {
    int floors[5] = {1, 2, 3, 4, 5};
    int index = rand() % 5;
    return floors[index];
}

void peopleLog(People* p) {
    char logEntry[100];
    snprintf(logEntry, sizeof(logEntry), "[info] 已生成 id%d 体重%.0lf 初始楼层 %d 目标楼层 %d 的人",
             p->id, p->weight, p->initFloor, p->gotoFloor);

    // 将logEntry拷贝到全局logs数组的第0个位置(只存单条日志)
    snprintf(logs_global[0], sizeof(logs_global[0]), "%s", logEntry);

    // 调用未知函数printLogWithMargin
    printLogWithMargin(61, 1, logs_global, 0);
}

void peopleLeaveLog(People* p, int floorNum) {
    char logEntry[100];
    snprintf(logEntry, sizeof(logEntry), "[info] %d 号乘客已从 %d层 离开", p->id, floorNum);

    // 将logEntry拷贝到全局logs数组的第0个位置(只存单条日志)
    snprintf(logs_global[0], sizeof(logs_global[0]), "%s", logEntry);

    // 调用未知函数printLogWithMargin
    printLogWithMargin(61, 1, logs_global, 0);
}

// 未知函数，保持名称与用法
// 假设其定义在其他文件中
// void printLogWithMargin(int x, int y, char logs[][100]) {
//     // 如果需要实现，可在此实现，这里只给出空实现以满足要求
//     (void)x;
//     (void)y;
//     (void)logs;
//     // 保留原有用法
// }




















// #include "people.h"
// #include "console.h"
// #include <array>
// #include <cmath>
// #include <random>

// int People::peopleID = 1;

// // 人的初始化函数
// People::People(int initFloor, bool valid) {
//     this->id = peopleID;
//     peopleID++;

//     this->weight = generateWeight();
//     this->gotoFloor = generateFloor();
//     this->initFloor = initFloor;
//     this->valid = valid;


//     peopleLog();
// }

// // 随机生成体重（正态分布）
// double People::generateWeight() {
//     std::random_device rd;
//     std::mt19937 gen(rd());

//     double mean = 70.0;   // 均值
//     double stddev = 10.0; // 标准差

//     std::normal_distribution<> d(mean, stddev);

//     double weight = std::round(d(gen));

//     return weight;
// }
// // 随机生成想去的楼层
// int People::generateFloor() {
//     // 创建一个随机数生成器
//     std::random_device rd;
//     std::mt19937 gen(rd());

//     // 定义楼层数组
//     std::array<int, 5> floors = {1, 2, 3, 4, 5};

//     // 创建一个均匀分布
//     std::uniform_int_distribution<> dist(0, floors.size() - 1);

//     // 生成一个随机索引
//     int index = dist(gen);

//     // 返回对应的楼层
//     return floors[index];
// }

// void People::peopleLog() {
//     char buffer[100];
//     sprintf_s(buffer, "[info] 已生成 id%d 体重%.0lf 初始楼层 %d 目标楼层 %d 的人", this->id, this->weight, this->initFloor,
//               this->gotoFloor);

//     // 将 char 数组转换为 std::string
//     std::string logEntry(buffer);

//     // 创建一个包含单个字符串的向量
//     std::vector<std::string> logs = {logEntry};

//     // 调用 printLogWithMargin 函数
//     printLogWithMargin(61, 1, logs);
// }

// void People::peopleLeaveLog(int floorNum) {
//     char buffer[100];
//     sprintf_s(buffer, "[info] %d 号乘客已从 %d层 离开", this->id, floorNum);

//     // 将 char 数组转换为 std::string
//     std::string logEntry(buffer);

//     // 创建一个包含单个字符串的向量
//     std::vector<std::string> logs = {logEntry};

//     // 调用 printLogWithMargin 函数
//     printLogWithMargin(61, 1, logs);
// }