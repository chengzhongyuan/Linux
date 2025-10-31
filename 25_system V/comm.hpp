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

/* 1������key_t */
key_t getkey()
{
    key_t k = ftok(PATHNAME, PROJ_ID);
    if(k<0)
    {
        std::cerr<<errno<<": "<<strerror(errno)<<std::endl;
        exit(1);
    }
}
/* int shmget(key_t key, size_t size, int shmflg);����ֵ���ǹ����ڴ�εı�ʶ�� */
/* 2��ͨ��keyֵ���������ڴ�ռ� */
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
/* void *shmat(int shmid, const void *shmaddr, int shmflg); NULL��ʾ��������*/
/* 3����ȡ�Ѿ������õĹ����ڴ�ռ� */
void * getshm(int shmid,const void *shmaddr, int shmflg)
{
    void* address = shmat(shmid, shmaddr,  shmflg);
    // ��������ǿ������ת����ʧ���ȵ����⣬����64λ�Ļ���ת�������ĸ��ֽ�
    // ���Գ������⣬�ĳ�longlong�ͺ�
    if((long long)address == -1L) std::cerr<<errno<<": "<<strerror(errno)<<std::endl;
    return address;
}

/* int shmctl(int shmid, int cmd, struct shmid_ds *buf); */
/* 4��������̵�ɾ�� */

int shmdel(int shmid, int cmd, struct shmid_ds *buf)
{
    if(shmctl(shmid, cmd, buf) == -1) std::cerr<<errno<<": "<<strerror(errno)<<std::endl;
}

#endif 