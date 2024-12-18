#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <winnt.h>
#endif

void setTextColor();
void resetTextColor();
void setCursorPositionWithGrid(int x, int y);
void drawVerticalLine(int x, int y);
void drawHorizontalLine(int x, int y);
void drawBlock(int x, int y);
void drawWallBlock(int x, int y);
void deleteGrid(int x, int y);
void drawNumber(int num, int x, int y);
void DrawString(const char *c, int x, int y);
void DrawUpTriangle(int x, int y);
void DrawDownTriangle(int x, int y);

#endif
