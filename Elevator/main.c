#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef _WIN32
#include<windows.h>
#endif

#define WIDTH 50
#define HEIGHT 30
#define ELEVATOR_HEIGHT 4
#define ELEVATOR_WIDTH 12
#define FLOOR_HEIGHT 4
#define FLOOR_COUNT 7
#define FLOOR_WIDTH 8
#define TIME 50000

void screenPrintString(int x, int y, char* s){
    printf("\033[%d;%dH%s", x+1, y*2+1, s);
    fflush(stdout);
}

void screenPrintNumber(int x, int y, int d){
    printf("\033[%d;%dH%d", x+1, y*2+1, d);
    fflush(stdout);
}

// Block

void initBlock(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    for(int i=1;i<=WIDTH;i++){
        screenPrintString(0,i,"──");
        screenPrintString(HEIGHT+1, i, "──");
    }
    for(int i=1;i<=HEIGHT;i++){
        screenPrintString(i,0,"│");
        screenPrintString(i,WIDTH+1," │");
    }
    screenPrintString(0,0,"┌─");
    screenPrintString(0,WIDTH+1,"─┐");
    screenPrintString(HEIGHT+1,0,"└─");
    screenPrintString(HEIGHT+1,WIDTH+1,"─┘");
    screenPrintString(HEIGHT+1, 10, "\n");
}

// Elevator

typedef struct Elevator Elevator;
struct Elevator{
    int x; //position
    int direction; //1:up -1:down
    int floor;
}elevator = {.x=1, .direction=1, .floor=FLOOR_COUNT};

void refreshElevator(){
    for(int i=1;i<HEIGHT;i++){
        for(int j=1;j<ELEVATOR_WIDTH;j++){
            screenPrintString(i,j,"  ");
        }
    }
    for(int i=2;i<ELEVATOR_WIDTH;i++){
        screenPrintString(elevator.x,i,"──");
        screenPrintString(elevator.x+ELEVATOR_HEIGHT,i,"──");
    }
    for(int i=1;i<ELEVATOR_HEIGHT;i++){
        screenPrintString(i+elevator.x,1,"│ ");
    }
    screenPrintString(elevator.x,1,"┌─");
    screenPrintString(elevator.x+ELEVATOR_HEIGHT,1,"└─");
    if(elevator.direction==1){
        screenPrintString(elevator.x+1,2,"▲");
    }else{
        screenPrintString(elevator.x+1,2,"▼");
    }
    screenPrintString(elevator.x+ELEVATOR_HEIGHT-1,2,"LOOPY");
}

void elevatorUp(){
    if(elevator.x<=1) return;
    elevator.direction = 1;
    elevator.x--;
    refreshElevator();
}

void elevatorDown(){
    if(elevator.x>=HEIGHT-ELEVATOR_HEIGHT-1) return;
    elevator.direction = -1;
    elevator.x++;
    refreshElevator();
}

void elevatorGo(int floor){
    if(floor>elevator.floor){
        for(int i=0;i<(floor-elevator.floor)*FLOOR_HEIGHT;i++){
            elevatorUp();
            usleep(TIME);
        }
    }else if(floor<elevator.floor){
        for(int i=0;i<(elevator.floor-floor)*FLOOR_HEIGHT;i++){
            elevatorDown();
            usleep(TIME);
        }
    }
    elevator.floor = floor;
}

// Floor

void printFloor(){
    for(int i=0;i<=FLOOR_COUNT;i++){
        for(int j=0;j<=FLOOR_WIDTH;j++){
            screenPrintString(i*FLOOR_HEIGHT+1,ELEVATOR_WIDTH+j,"██");
        }
    }
    for(int i=0;i<FLOOR_COUNT;i++){
        screenPrintNumber(i*FLOOR_HEIGHT+2, ELEVATOR_WIDTH+FLOOR_WIDTH-1, FLOOR_COUNT-i);
    }
    for(int i=1;i<=FLOOR_COUNT*FLOOR_HEIGHT+1;i++){
        screenPrintString(i,ELEVATOR_WIDTH+FLOOR_WIDTH,"██");
    }
}

// Main

int main(){
    printf("\033[?25l"); //hide cursor
    fflush(stdout);
    initBlock();
    refreshElevator();
    printFloor();
    elevatorGo(1);
    elevatorGo(5);
    screenPrintString(HEIGHT+1, 10, "\n");
    return 0;
}

