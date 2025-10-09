#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define USER "USER"
// ./mycmd -a -b  ----->这是一个长字符串
// 0号一般就是指向命令  后面还有跟着的参数（命令行参数）也会解析进去
// 接下来我们再来看一看环境变量参数
int main(int argc,char* argv[], char* env[])
{
    for(int a=0; env[a]; a++)
    {
        printf("env[%d]->%s\n",a,env[a]);
    }
    for(int i=0;i<argc;i++)
    {
        printf("argc[%d]-> %s\n",i,argv[i]);
    }
    char *who = getenv("USER");
    if(strcmp(who,"root") == 0)
    {
        printf("user:%s\n", who);
    }
    else
    {
        printf("权限不足\n");
    }
    //printf("user:%s\n", who);
    return 0;
}
