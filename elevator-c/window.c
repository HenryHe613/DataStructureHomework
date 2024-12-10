#include "window.h"
#include "draw.h"
#include "global.h"

void windowInit() {
    // for (int i = 0; i < WIDTH; i++)
    //{
    //	drawHorizontalLine(i, 0);
    //	drawHorizontalLine(i, 37);
    // }

    // 绘制水平边框
    for (int i = 0; i < WIDTH; i++) {
        drawHorizontalLine(i, 0); // for debug purpose
        drawHorizontalLine(i, 37);
    }

    // 绘制垂直边框
    for (int j = 0; j < HEIGHT + 2 * BORDERWIDTH; j++) {
        drawVerticalLine(0, j); // for debug purpose
        drawVerticalLine(WIDTH, j);
    }

    // 绘制楼层
    int xInitPosition = BORDERWIDTH + ELEVATOR_WIDTH; // 楼层左侧起始点
    int yInitPosition = BORDERWIDTH;
    for (int j = 7; j < 40; j += 7) {
        for (int i = 0; i < FLOOR_WIDTH; i++) {
            drawWallBlock(xInitPosition + i, j);
        }
    }

    // 绘制垂直线，水平划分区域
    int vPosition = BORDERWIDTH + ELEVATOR_WIDTH + FLOOR_WIDTH; //  日志栏右侧
    for (int i = 0; i < HEIGHT; i++) {
        drawWallBlock(vPosition, i);

    }

    // 绘制楼层数字
    int floorLeft = xInitPosition;
    for (int i = 0; i < 5; i++) {
        drawNumber(5 - i, floorLeft + FLOOR_WIDTH - 2, BORDERWIDTH + i * FLOOR_HEIGHT);
    }
}