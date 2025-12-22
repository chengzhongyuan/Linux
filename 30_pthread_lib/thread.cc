#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <pthread.h> 

#include<memory>
#include"Thread.hpp"

void *thread_run( void * arg ) 
{ 
    // pthread_detach(pthread_self()); 
    printf("%s\n", (char*)arg); 
    return NULL; 
} 
int main( void ) 
{ 
    Thread T1(thread_run,(void*)"thread->1",1);
    T1.start();
    T1.join();
    // // 1、创建新的线程
    // // 2、新线程的任务就是把自己变成分离线程
    // // 3、这个时候一个线程只只能有一种状态，分离和等待不可以同时存在，所以这个时候就会等待失败
    // pthread_t tid; 
    // if ( pthread_create(&tid, NULL, thread_run, (void*)"thread1 run...") != 0 ) 
    // { 
    //     printf("create thread error\n"); 
    //     return 1; 
    // } 
    // int ret = 0; 
    // sleep(1);//很重要，要让线程先分离，再等待 
    // if ( pthread_join(tid, NULL ) == 0 ) { 
    //     printf("pthread wait success\n"); 
    //     ret = 0; 
    // } else { 
    //     printf("pthread wait failed\n"); 
    //     ret = 1; 
    // } 
    // return ret; 
} 