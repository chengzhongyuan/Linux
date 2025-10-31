/*
SYNOPSIS
       #include <sys/ipc.h>
       #include <sys/shm.h>

       int shmget(key_t key, size_t size, int shmflg);

DESCRIPTION
       shmget()  returns the identifier of the System V shared memory segment associated
       with the value of the argument key.  A new shared memory segment, with size equal
       to the value of size rounded up to a multiple of PAGE_SIZE, is created if key has
       the value IPC_PRIVATE or key isn't IPC_PRIVATE, no shared memory  segment  corre‐
       sponding to key exists, and IPC_CREAT is specified in shmflg.

       If  shmflg  specifies  both  IPC_CREAT  and  IPC_EXCL and a shared memory segment
       already exists for key, then shmget() fails with errno set to EEXIST.   (This  is
       analogous to the effect of the combination O_CREAT | O_EXCL for open(2).)

       The value shmflg is composed of:

       IPC_CREAT   如果不存在就创建一个共享区

       IPC_EXCL    和IPC_CREAT一起使用，如果共享内存存在，就出错返回

       RETURN VALUE 和文件系统脱节，也是一个共享内存的标识符
       On success, a valid shared memory  identifier  is  returned.   On  errir,  -1  is
       returned, and errno is set to indicate the error.

       key_t key 是什么不重要，重要的是可以标定唯一性 ftok()形成一个key
*/
#include "comm.hpp"
int main()
{
       /* 1、获取唯一键值 确保创建的共享内存是唯一的 */
       key_t k = getkey();

       /* 2、获取共享内存ID key和shmid 都是为了解耦内核机制和用户机制*/
       int shmid = creatshm(k, 4096, 0);

       /* 3、获取已经创建好的共享内存地址 */
       char *shmaddr = (char *)getshm(shmid, NULL, 0);

       /* 4、可以重复多次进行数据读取，不是读一次数据就没有了 */
       while (true)
       {
              std::cout << "Client receive: " << shmaddr << std::endl;
              sleep(2); // 每 2 秒读取一次
       }

       return 0;
}