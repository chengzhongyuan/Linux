#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main()
{
    // C接口
    printf("c语言提供\n");
    fprintf(stdout,"c语言提供\n");
    const char* cString = "c语言提供\n";
    fputs(cString,stdout);

    // 系统接口
    const char* wString = "系统接口\n";
    write(1,wString,strlen(wString));
    // 代码结束之前创建子进程
    /*
    1、首先在我们没有进行重定向的时候，输出的是四条消息，这个时候的缓冲机制是行缓冲，FILE中包含缓冲区
    所以在创建子进程之前，FILE内部就没有顺序了，在这时候分流就什么也不会显示（再进行一点解释，子进程
    是完全继承了父亲的的整个内存空间的，多以对于他来说没有任何可以输出的东西）

    2、如果我们进行重定向，那么整个缓冲策略就从"行缓冲"->"全缓冲"，也就是说等到缓冲区满才进行刷新。然    后紧接着就是进程退出，stdout属于父进程，这个时候发生写时拷贝！！所以最终数据显示了两倍

    3、那么为什么write没有显示呢？
    因为这是系统所提供的接口直接使用的是fd，与FILE无关，所以数据也无法存储到FILE中的缓冲区之中。因此     只会显示一次
    */
    fork();
    return 0;
}
