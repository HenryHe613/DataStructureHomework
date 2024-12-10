#include "elevator.h"
#include "console.h"
#include <string.h>

// 全局日志数组
char elevator_log_global[1000][100];
int elevator_log_count = 0;

// 全局 gone 数组
People elevator_gone_global[1000];
int elevator_gone_count = 0;

void Elevator_init(Elevator* e) {
    e->elevatorDirection = 'd';
    e->elevatorPositionX = 1;
    e->elevatorPositionY = 1;
    e->currentFloor = 5;
    e->peopleCount = 0;
    Elevator_drawElevator(e);
}

void Elevator_drawElevatorWithCoordinates(Elevator* e, int x, int y) {
    int i;
    for (i = 0; i < ELEVATOR_WIDTH; i++) {
        drawBlock(x + i, y);
        drawBlock(x + i, y + ELEVATOR_HEIGHT - 1);
    }
    for (i = 0; i < ELEVATOR_HEIGHT; i++) {
        drawBlock(x, y + i);
        drawBlock(x + ELEVATOR_WIDTH - 1, y + i);
    }
}

void Elevator_deleteElevatorWithCoordinates(Elevator* e, int x, int y) {
    int i;
    for (i = 0; i < ELEVATOR_WIDTH; i++) {
        deleteGrid(x + i, y);
        deleteGrid(x + i, y + ELEVATOR_HEIGHT - 1);
    }
    for (i = 0; i < ELEVATOR_HEIGHT; i++) {
        deleteGrid(x, y + i);
        deleteGrid(x + ELEVATOR_WIDTH - 1, y + i);
    }
}

void Elevator_drawElevator(Elevator* e) {
    int i;
    for (i = 0; i < ELEVATOR_WIDTH; i++) {
        drawBlock(e->elevatorPositionX + i, e->elevatorPositionY);
        drawBlock(e->elevatorPositionX + i, e->elevatorPositionY + ELEVATOR_HEIGHT - 1);
    }

    for (i = 0; i < ELEVATOR_HEIGHT; i++) {
        drawBlock(e->elevatorPositionX, e->elevatorPositionY + i);
        drawBlock(e->elevatorPositionX + ELEVATOR_WIDTH - 1, e->elevatorPositionY + i);
    }
}

void Elevator_deleteElevator(Elevator* e) {
    int i;
    for (i = 0; i < ELEVATOR_WIDTH; i++) {
        deleteGrid(e->elevatorPositionX + i, e->elevatorPositionY);
        deleteGrid(e->elevatorPositionX + i, e->elevatorPositionY + ELEVATOR_HEIGHT - 1);
    }

    for (i = 0; i < ELEVATOR_HEIGHT; i++) {
        deleteGrid(e->elevatorPositionX, e->elevatorPositionY + i);
        deleteGrid(e->elevatorPositionX + ELEVATOR_WIDTH - 1, e->elevatorPositionY + i);
    }
}

void Elevator_goDownElevator(Elevator* e) {
    // 更新方向
    e->elevatorDirection = 'd';
    Elevator_deleteElevator(e);
    e->elevatorPositionY += 1;
    Elevator_drawElevator(e);
}

void Elevator_goUpElevator(Elevator* e) {
    e->elevatorDirection = 'u';
    Elevator_deleteElevator(e);
    e->elevatorPositionY -= 1;
    Elevator_drawElevator(e);
}

void Elevator_goDownFloor(Elevator* e) {
    elevator_log_count = 0; 
    snprintf(elevator_log_global[elevator_log_count++], 100, "%s", "电梯下行");
    printLogWithMargin(61, 1, elevator_log_global, elevator_log_count);

    e->currentFloor -= 1;
    int i;
    for (i = 0; i < ELEVATOR_HEIGHT; i++) {
        Elevator_goDownElevator(e);
        elevator_sleep_ms(20);
    }
}

void Elevator_goUpFloor(Elevator* e) {
    elevator_log_count = 0; 
    snprintf(elevator_log_global[elevator_log_count++], 100, "%s", "电梯上行");
    printLogWithMargin(61, 1, elevator_log_global, elevator_log_count);

    e->currentFloor += 1;
    int i;
    for (i = 0; i < ELEVATOR_HEIGHT; i++) {
        Elevator_goUpElevator(e);
        elevator_sleep_ms(20);
    }
}

void Elevator_closeDoor(Elevator* e) {
    int doorX = e->elevatorPositionX + ELEVATOR_WIDTH - 1;
    int doorY = e->elevatorPositionY;

    int i;
    for (i = 0; i < ELEVATOR_HEIGHT; i++) {
        deleteGrid(doorX, doorY + i);
    }
}

void Elevator_drawElevatorDirection(Elevator* e) {
    int x = e->elevatorPositionX + ELEVATOR_WIDTH - 2;
    int y = e->elevatorPositionY + ELEVATOR_HEIGHT / 2;

    if (e->elevatorDirection == 'u') {
        DrawUpTriangle(x, y);
    } else {
        DrawDownTriangle(x, y);
    }
}

void Elevator_go(Elevator* e, char direction) {
    if (direction == 'd') {
        Elevator_goDownFloor(e);
    } else if (direction == 'u') {
        Elevator_goUpFloor(e);
    }
}

void Elevator_changeDirection(Elevator* e) {
    if (e->elevatorDirection == 'u') {
        e->elevatorDirection = 'd';
    } else {
        e->elevatorDirection = 'u';
    }
}

void Elevator_pushPeople(Elevator* e, People people) {
    if (e->peopleCount < 1000) {
        e->peopleList[e->peopleCount++] = people;
    }
}

bool Elevator_isVoid(Elevator* e) {
    if (e->peopleCount == 0) {
        return true;
    } 
    return false;
}

int Elevator_clean(Elevator* e, int floorNum) {
    elevator_gone_count = 0;
    int i;
    // 遍历peopleList，将目标楼层为floorNum的人加入gone数组，并从列表中移除
    // 简单做法：新建一个临时数组，把不离开的人复制回去
    People temp[1000];
    int tempCount = 0;

    for (i = 0; i < e->peopleCount; i++) {
        if (e->peopleList[i].gotoFloor == floorNum) {
            // 该人要离开
            peopleLeaveLog(&e->peopleList[i], floorNum);
            if (elevator_gone_count < 1000) {
                elevator_gone_global[elevator_gone_count++] = e->peopleList[i];
            }
        } else {
            // 不走的人
            temp[tempCount++] = e->peopleList[i];
        }
    }

    // 更新电梯中的乘客列表
    for (i = 0; i < tempCount; i++) {
        e->peopleList[i] = temp[i];
    }
    e->peopleCount = tempCount;

    return elevator_gone_count;
}





















// #include "elevator.h"
// #include "people.h"
// #include <algorithm>
// #include "console.h"



// Elevator::Elevator() {
//     this->elevatorDirection = 'd';
//     this->elevatorPositionX = 1;
//     this->elevatorPositionY = 1;
//     drawElevator(); 
// }




// // 画一个电梯。输入坐标
// void Elevator::drawElevatorWithCoordinates(int x, int y) {
//     for (int i = 0; i < ELEVATOR_WIDTH; i++) {
//         drawBlock(x + i, y);
//         drawBlock(x + i, y + ELEVATOR_HEIGHT - 1);
//     }

//     for (int i = 0; i < ELEVATOR_HEIGHT; i++) {
//         drawBlock(x, y + i);
//         drawBlock(x + ELEVATOR_WIDTH - 1, y + i);
//     }
// }

// // 删除指定坐标的电梯
// void Elevator::deleteElevatorWithCoordinates(int x, int y) {
//     for (int i = 0; i < ELEVATOR_WIDTH; i++) {
//         deleteGrid(x + i, y);
//         deleteGrid(x + i, y + ELEVATOR_HEIGHT - 1);
//     }

//     for (int i = 0; i < ELEVATOR_HEIGHT; i++) {
//         deleteGrid(x, y + i);
//         deleteGrid(x + ELEVATOR_WIDTH - 1, y + i);
//     }
//     //  drawElevatorDirection();
// }

// // 画一个电梯，位置由程序操控
// void Elevator::drawElevator() {
//     for (int i = 0; i < ELEVATOR_WIDTH; i++) {
//         drawBlock(elevatorPositionX + i, elevatorPositionY);
//         drawBlock(elevatorPositionX + i,
//                   elevatorPositionY + ELEVATOR_HEIGHT - 1);
//     }

//     for (int i = 0; i < ELEVATOR_HEIGHT; i++) {
//         drawBlock(elevatorPositionX, elevatorPositionY + i);
//         drawBlock(elevatorPositionX + ELEVATOR_WIDTH - 1,
//                   elevatorPositionY + i);
//     }

//     //  drawElevatorDirection();

// }

// // 删除（隐藏）电梯
// void Elevator::deleteElevator() {
//     for (int i = 0; i < ELEVATOR_WIDTH; i++) {
//         deleteGrid(elevatorPositionX + i, elevatorPositionY);
//         deleteGrid(elevatorPositionX + i,
//                    elevatorPositionY + ELEVATOR_HEIGHT - 1);
//     }

//     for (int i = 0; i < ELEVATOR_HEIGHT; i++) {
//         deleteGrid(elevatorPositionX, elevatorPositionY + i);
//         deleteGrid(elevatorPositionX + ELEVATOR_WIDTH - 1,
//                    elevatorPositionY + i);
//     }
// }

// // 电梯下移一格，（会先删除原有的电梯，然后重新绘制电梯）
// void Elevator::goDownElevator() {
//     // 更新方向
//     elevatorDirection = 'd';
//     // 删除电梯
//     deleteElevator();
//     // 电梯坐标下移
//     elevatorPositionY += 1;
//     // 重新绘制电梯
//     drawElevator();
// }

// // 电梯上移一格，（会先删除原有的电梯，然后重新绘制电梯）
// void Elevator::goUpElevator() {
//     elevatorDirection = 'u';
//     deleteElevator();
//     elevatorPositionY -= 1;
//     drawElevator();
// }

// // 下移5格
// void Elevator::goDownFloor() {
//     std::vector<std::string> log;
//     log.push_back("电梯下行");
//     printLogWithMargin(61, 1, log);
//     this->currentFloor -= 1;
//     for (int i = 0; i < ELEVATOR_HEIGHT; i++) {
//         goDownElevator();
//         std::this_thread::sleep_for(std::chrono::milliseconds(20));
//     }
// }

// // 上移5格
// void Elevator::goUpFloor() {
//     std::vector<std::string> log;
//     log.push_back("电梯上行");
//     printLogWithMargin(61, 1, log);
//     this->currentFloor += 1;
//     for (int i = 0; i < ELEVATOR_HEIGHT; i++) {
//         goUpElevator();
//         std::this_thread::sleep_for(std::chrono::milliseconds(20));
//     }
// }


// void Elevator::closeDoor() {
//     int doorX = ELEVATOR_WIDTH;
//     int doorY = elevatorPositionY;

//     for (int i = 0; i < ELEVATOR_HEIGHT; i++) {
//         deleteGrid(doorX, doorY + i);
//     }
// }

// void Elevator::drawElevatorDirection() {
//     int x = elevatorPositionX + ELEVATOR_WIDTH - 2;
//     int y = elevatorPositionY + ELEVATOR_HEIGHT / 2;

//     if (elevatorDirection == 'u') {
//         DrawUpTriangle(x, y);
//     } else {
//         DrawDownTriangle(x, y);
//     }
// }

// void Elevator::go(char direction) {
//     if (direction == 'd') {
//         goDownFloor();
//     } else if (direction == 'u') {
//         goUpFloor();
//     }
// }


// // change direction.
// void Elevator::changeDirection() {
//     if (elevatorDirection == 'u') {
//         elevatorDirection = 'd';
//     } else {
//         elevatorDirection = 'u';
//     }
// }

// void Elevator::pushPeople(People people) {
//     this->peopleList.push_back(people);

// }


// bool Elevator::isVoid() {
//     if (peopleList.empty()) {
//         return true;
//     } 
//     return false;
// }


// std::vector<People> Elevator::clean(int floorNum) {
//     std::vector<People> gone = {};

//     for (auto people : peopleList) {
//         if (people.gotoFloor == floorNum)
//         people.peopleLeaveLog(floorNum);
//     }
    
//     auto it = std::remove_if(peopleList.begin(), peopleList.end(), [&gone, floorNum] (const People& people) {
//             if (people.gotoFloor == floorNum) {
//             gone.push_back(people);
//             return true;
//             }
//             return false;
//             }
//             );

//     peopleList.erase(it, peopleList.end());
//         return gone;
// };
