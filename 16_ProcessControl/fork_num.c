#include<stdio.h>
#include<unistd.h>

int cnt = 0;

int main()
{
    int ret = fork();
    while(1)
    {
        if(ret<0){
            printf("fork error! ret = %d",cnt);
            break;
        }
        else if(ret == 0){
            while(1)
            {
        //        sleep(1);
            }
        }
        cnt++;
    }
    return 0;
}
