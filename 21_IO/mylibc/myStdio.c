#include"myStdio.h"

/*将一个已有的文件描述符（fd）转换为一个 FILE* 流指针，以便使用 fread、fprintf、fgets 等标准 I/O 函数进行操作*/
FILE_ *fopen_(const char*path_name,const char* mode)
{
    int flag =0;
     flag |= O_CREAT;
    if(strcmp(mode,"r") == 0)
    {
        flag |= O_RDONLY;
    }
    else if(strcmp(mode,"w") == 0) 
    {
        flag |= O_WRONLY | O_TRUNC;
    }
    else if(strcmp(mode,"rw") == 0) 
    {
        flag |= O_RDWR;
    }
    else{
        return NULL;
    }

    // 接下来我么就是打开文件
    int fd = open(path_name,flag,0666);
    if(fd == -1) return NULL;

    // 我们最后需要返回的就是一个结构体指针
    // 现在我们手里已经有了fd，并且我们还得让数据存储在我们的缓存区里面哇
    FILE_* fp = (FILE_*)malloc(sizeof(FILE_));
    if (!fp) {
        close(fd);
        return NULL;
    }
    
    fp->fileno = fd;
    fp->pos = 0;
    fp->flags = SYNC_LINE;// 默认进行行缓冲
    memset(fp->buffers,0,SIZE);// 初始化缓冲区
    return fp;
}

// 对文件中进行写入
int fwrite_(void *ptr,int num, FILE_*fp)
{
    // 要确保传入的全部都不是空的否则就返回-1
    if(ptr == NULL | num == 0 | fp == NULL) return -1;
    // 数据传入自定义缓冲区的过程
    for (int i = 0; i < num; ++i) 
    {
        char ch = ((char*)ptr)[i];
        fp->buffers[fp->pos++] = ch;    
    }
    //int written = write(fp->fileno,fp->buffers,num);// 这样就把数据全部写入到我们的缓冲区域之中去了
    
    // 应该采取什么样子的方式去读取换行符号呢
    for (int i = 0;i < fp->pos; ++i) {
        if (fp->buffers[i] == '\n') {
            write(fp->fileno,fp->buffers,i);
            fp->pos = 0;
            break; // 找到换行符就停止
        }
    }

    // 解读出第一个换行符号然后再进行打印输出
    
    return 0;
}

void fclose(FILE_* fp)
{
    // 关掉文件就比较简单了
    
    // 0、在关掉之前我应该先去将缓冲区的内容全部进行输出
    if(fp->pos != 0)
        write(1,fp->buffers,(fp->pos));
    // 1、调用系统关掉文件
    close(fp->fileno);
    // 2、整个结构体指针
    free(fp);
}
