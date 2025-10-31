#include "comm.hpp"
int main()
{
    /* 1、创建唯一key值 */
    key_t k = getkey();

    /* 2、创建共享内存 */
    int shmid = creatshm(k, 4096, IPC_CREAT | 0666);

    /* 3、获取创建好的共享内存区地址,也就是把当前进程和共享空间进行关联了 */
    char* shmarr = (char*)getshm(shmid,NULL,0);

    /* 4、向共享区之中写入数据 1.数据信息 2.进程PID 3.计数器 */
    strcpy(shmarr,"hello world!");
    const char* msg = "hello I am server";
    pid_t id = getpid();
    int counter = 0;
    while(true)
    {
        snprintf(shmarr, 4096, "server[%d]: %s [%d]\n", id, msg, counter++);
        sleep(2);
    }



    /* 5、去除关联,但是这个共享内存依旧是存在的,去除关联之后只要共享内存不被删除还是可以读取到信息的 */
    shmdt(shmarr);

    sleep(5);
    
    /* 5、删除共享内存 */
    shmdel(shmid, IPC_RMID, NULL);
    return 0;
}

/*
共享内存的声明周期是随着操作系统的，不是随着进程的！！
------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
0x66010072 0          zhongyuan  666        1024       0       
*/

// 共享内存不会给我们进行同步和互斥的操作，没有对数据做任何的保护
// 系统分配共享内存是以4KB为单位的