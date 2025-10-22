#pragma once 

#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>

// 自己实现一个行缓冲策略

#define SIZE 1024

#define SYNC_NOW  1
#define SYNC_LINE 2
#define SYNC_FULL 4

typedef struct _FILE{
    int flags;  // 这里就是告诉别人你要哪一种缓冲模式
    int fileno; // 这里包含的就是文件描述符 
    int pos;    // pos代表的是我的写入位置
    char buffers[SIZE];
}FILE_;

// 调用系统接口选择用什么样子的方式对文件进行打开
FILE_ *fopen_(const char*path_name,const char* mode);
// 对文件中进行写入 ptr是需要写入的数据块的指针 fp就是我们通过fopen打开的有效文件流
int fwrite_(void *ptr,int num, FILE_*fp);
// 关闭文件
void fclose(FILE_* fp);
