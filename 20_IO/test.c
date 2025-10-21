#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // 关闭标准输出（fd 1）
    close(1);

    // 再打开一个文件
    int fd = open("file.txt", O_CREAT | O_WRONLY, 0644);

    // 打印分配的文件描述符
    printf("file.txt 的 fd: %d （是否复用了标准输出的 fd 1？）\n", fd);
    fflush(stdout);
    // 关闭文件
    close(fd);

    return 0;
}

