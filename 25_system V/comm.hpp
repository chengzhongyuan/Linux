#ifndef _COMM_HPP_
#define _COMM_HPP_

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <ctime>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* key_t ftok(const char *pathname, int proj_id); */

#define PATHNAME "."
#define PROJ_ID 0x66

/* 1、创建key_t */
key_t getkey()
{
    key_t k = ftok(PATHNAME, PROJ_ID);
    if(k<0)
    {
        std::cerr<<errno<<": "<<strerror(errno)<<std::endl;
        exit(1);
    }
}
/* int shmget(key_t key, size_t size, int shmflg);返回值就是共享内存段的标识符 */
/* 2、通过key值创建共享内存空间 */
int creatshm(key_t key,size_t size, int shmflg)
{
    int id = shmget(key, size, shmflg);
    if(id ==-1) 
    {
        std::cerr<<errno<<": "<<strerror(errno)<<std::endl;
        exit(1);
    }
    return id;
    
}
/* void *shmat(int shmid, const void *shmaddr, int shmflg); NULL表示自主分配*/
/* 3、获取已经创建好的共享内存空间 */
void * getshm(int shmid,const void *shmaddr, int shmflg)
{
    void* address = shmat(shmid, shmaddr,  shmflg);
    // 关于这里强制类型转换丢失精度的问题，这是64位的机器转成整形四个字节
    // 所以出了问题，改成longlong就好
    if((long long)address == -1L) std::cerr<<errno<<": "<<strerror(errno)<<std::endl;
    return address;
}

/* int shmctl(int shmid, int cmd, struct shmid_ds *buf); */
/* 4、共享进程的删除 */

int shmdel(int shmid, int cmd, struct shmid_ds *buf)
{
    if(shmctl(shmid, cmd, buf) == -1) std::cerr<<errno<<": "<<strerror(errno)<<std::endl;
}

#endif 