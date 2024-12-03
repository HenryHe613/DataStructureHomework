#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifdef _WIN32
#include<windows.h>
#endif

int adjacencyList[101][101];
int inDegree[101];
int n = 0;

int main(){
    // Windows下设置UTF-8编码
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    DWORD mode;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
#endif
    memset(adjacencyList, 0, sizeof(adjacencyList));
    memset(inDegree, 0, sizeof(inDegree));
    printf("请输入无向图顶点个数(1~100): ");
    scanf("%d", &n);
    printf("请输入无向图的每条边,以\"0 0\"结尾:\n");
    int a, b;
    scanf("%d %d", &a, &b);
    while(a != 0 && b != 0){
        adjacencyList[a][0]++;
        adjacencyList[a][adjacencyList[a][0]] = b;
        inDegree[b]++;
        scanf("%d %d", &a, &b);
    }
    for(int i = 0;i < n;i++){
        int now = 0;
        for(int j = 1;j <= n;j++){
            if(inDegree[j] == 0){
                now = j;
                break;
            }
        }
        if(now == 0){
            printf("图中有环");
            return 0;
        } else{
            printf("%d ", now);
            inDegree[now] = -1;
            for(int j = 1;j <= adjacencyList[now][0];j++){
                inDegree[adjacencyList[now][j]]--;
            }
        }
    }
    printf("\n");
    printf("按回车键退出\n");
    getchar();
    getchar();
    return 0;
}