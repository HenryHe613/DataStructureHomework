#include "floor.h"

// 全局数组替代 vector
People global_peopleList[1000];

// 初始化Floor
void Floor_init(Floor* f, int floorNum) {
    f->peopleMax = 5;
    // uniform_int由utility.h中定义的函数，保留用法不变
    int peopleNum = uniform_int(f->peopleMax);
    f->peopleNum = peopleNum;

    // 初始化对应数量的人，并放入 global_peopleList 前 peopleNum 个位置
    for (int i = 0; i < peopleNum; i++) {
        People p;
        // 原代码： People people = People(floorNum);
        // 转为C函数调用，如： initPeople(&p, floorNum, 1);
        initPeople(&p, floorNum, 1);
        global_peopleList[i] = p;
    }
}

// 对应原 pop 方法
People Floor_pop(Floor* f) {
    // 如果没有人，则返回一个无效People
    if (f->peopleNum <= 0) {
        People p;
        // 原代码：People people = People(123, false);
        // 转C: initPeople(&p, 123, 0)表示无效
        initPeople(&p, 123, 0);
        return p;
    }

    // 返回最前面的人
    People p = global_peopleList[0];

    // 删除第一个人(相当于从数组开头移除)
    // 将后面的人往前移
    for (int i = 1; i < f->peopleNum; i++) {
        global_peopleList[i-1] = global_peopleList[i];
    }

    f->peopleNum--;
    return p;
}

// 对应原 drawFloor 方法
void Floor_drawFloor(Floor* f) {
    // 如果有需要在这里实现绘制楼层功能
    // 暂时空实现或根据需求实现
    (void)f; // 防止未使用警告
}

// 对应原 check 方法
bool Floor_check(Floor* f) {
    // 原逻辑: if (this->peopleList.empty()) return false; else return true;
    // 现在用 peopleNum 判断
    if (f->peopleNum <= 0) {
        return false;
    } else {
        return true;
    }
}


















// #include "floor.h"

// Floor::Floor(int floorNum) {
//     int peopleNum = uniform_int(this->peopleMax);

//     this->peopleNum = peopleNum;

//     for (int i = 0;i < peopleNum; i++) {
//         People people = People(floorNum);
//         this->peopleList.push_back(people);
//     }
// }

// People Floor::pop() {
//     // check if empty, but we don't want to go wrong!
//     //if (this->peopleList.empty()) {
//       //  std::cerr << "peopleList empty";
//      //   system("exit");
//     //}

//     if (this->peopleList.empty()) {
//         People people = People(123, false);
//         return people;
//     }
//     People people = peopleList[0];
    
//     // erase the first
//     peopleList.erase(peopleList.begin());
//     this->peopleNum--;
//     return people;
// }


// bool Floor::check() {
//     if (this->peopleList.empty()) {
//         return false;
//     } else {
//         return true;
//     }
// }    
