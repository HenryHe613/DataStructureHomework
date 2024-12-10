#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef _WIN32
#include<windows.h>
#endif

#define WIDTH 50
#define HEIGHT 30
#define ELEVATOR_HEIGHT 4
#define ELEVATOR_WIDTH 10
#define FLOOR_HEIGHT 4
#define FLOOR_COUNT 7
#define FLOOR_WIDTH 10

void screenPrint(int x, int y, char* s){
    printf("\033[%d;%dH%s", x+1, y*2+1, s);
    fflush(stdout);
}

void initBlock(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    for(int i=1;i<=WIDTH;i++){
        screenPrint(0,i,"──");
        screenPrint(HEIGHT+1, i, "──");
    }
    for(int i=1;i<=HEIGHT;i++){
        screenPrint(i,0,"│");
        screenPrint(i,WIDTH+1," │");
    }
    screenPrint(0,0,"┌─");
    screenPrint(0,WIDTH+1,"─┐");
    screenPrint(HEIGHT+1,0,"└─");
    screenPrint(HEIGHT+1,WIDTH+1,"─┘");
    screenPrint(HEIGHT+1, 10, "\n");
}

typedef struct{
    int x; //position
    int direction; //1:up -1:down
} Elevator;

void refreshElevator(Elevator elevator){
    for(int i=1;i<HEIGHT;i++){
        for(int j=1;j<ELEVATOR_WIDTH;j++){
            screenPrint(i,j,"  ");
        }
    }
    for(int i=2;i<ELEVATOR_WIDTH;i++){
        screenPrint(elevator.x,i,"──");
        screenPrint(elevator.x+ELEVATOR_HEIGHT,i,"──");
    }
    for(int i=1;i<ELEVATOR_HEIGHT;i++){
        screenPrint(i+elevator.x,1,"│ ");
    }
    screenPrint(elevator.x,1,"┌─");
    screenPrint(elevator.x+ELEVATOR_HEIGHT,1,"└─");
    if(elevator.direction==1){
        screenPrint(elevator.x+1,2,"▲");
    }else{
        screenPrint(elevator.x+1,2,"▼");
    }
    screenPrint(elevator.x+ELEVATOR_HEIGHT-1,2,"LOOPY");
}

void printFloor(){
    for(int i=0;i<=FLOOR_COUNT;i++){
        for(int j=0;j<=FLOOR_WIDTH;j++){
            screenPrint(i*FLOOR_HEIGHT+1,ELEVATOR_WIDTH+j,"██");
        }
    }
    for(int i=1;i<=FLOOR_COUNT*FLOOR_HEIGHT+1;i++){
        screenPrint(i,ELEVATOR_WIDTH+FLOOR_WIDTH,"██");
    }
}

int main(){
    printf("\033[?25l"); //hide cursor
    fflush(stdout);
    Elevator elevator = {.x=1, .direction=1};
    initBlock();
    refreshElevator(elevator);
    printFloor();
    for(int i=1;i<HEIGHT-ELEVATOR_HEIGHT;i++){
        usleep(50000);
        elevator.x = i;
        refreshElevator(elevator);
    }
    screenPrint(HEIGHT+1, 10, "\n");
}

