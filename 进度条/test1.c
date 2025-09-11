#include<stdio.h>
int main()
{
    int cnt=10;
    while(cnt)
    {
        printf("剩余时间：%2d\r", cnt);
        cnt--;
        fflush(stdout);
        sleep(1);
    }
    return 0;
}
