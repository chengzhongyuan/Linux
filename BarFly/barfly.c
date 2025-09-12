#include "barfly.h"
void Barfly()// 函数的定义
{
    int cnt = 0;
    char bar[NUM];
    memset(bar, '\0', sizeof(bar));
    // 写一个逆时针旋转
    const char *label = "|\\-/";
    // 至少循环101次
    while(cnt <= 100)
    {
        printf("\033[32m[%-100s][%d%%][%c]\033[0m\r",bar,cnt,label[cnt%4]);// 这个%100s是预留空间的意思%是一个特殊符号，两个%就可以正常显示
        //printf("[%-101s][%d%%][%c]\r",bar,cnt,label[cnt%4]);// 这个%100s是预留空间的意思%是一个特殊符号，两个%就可以正常显示
        fflush(stdout);
        bar[cnt++] = STYLE;
        //sleep(1);
        usleep(50000);
    }
    printf("\n");
}
