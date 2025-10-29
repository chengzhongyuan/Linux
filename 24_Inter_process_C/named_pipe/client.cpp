#include "comm.h"

// 你可不可以把刚刚写的改成命名管道呢！
int main()
{
    std::cout << "client begin" << std::endl;
    // 其实就是以只写的方式去打开了管道文件
    int FIFOfd = open(NAMED_PIPE, O_WRONLY); //同时都打开才会向后运行
    std::cout << "client end" << std::endl;
    if(FIFOfd < 0) exit(1); 

    //write
    char buffer[1024];
    while(true)
    {
        std::cout << "Please Say# ";
        fgets(buffer, sizeof(buffer), stdin); // abcd\n
        if(strlen(buffer) > 0) buffer[strlen(buffer)-1] = 0;
        // 向管道文件之中写入
        ssize_t n = write(FIFOfd, buffer, strlen(buffer));
        assert(n == strlen(buffer));
        (void)n;
    }

    close(FIFOfd);
    return 0;
}