#include <iostream>
#include <queue>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>   // for sleep

#define NUM 8

class BlockQueue {
private:
    std::queue<int> q;
    int cap;
    pthread_mutex_t lock;
    pthread_cond_t full;   // 队列满时，生产者等待
    pthread_cond_t empty;  // 队列空时，消费者等待

    void LockQueue() { pthread_mutex_lock(&lock); }
    void UnLockQueue() { pthread_mutex_unlock(&lock); }

    bool IsEmpty() { return q.size() == 0; }
    bool IsFull() { return q.size() == cap; }

public:
    BlockQueue(int _cap = NUM): cap(_cap) 
    {
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&full, NULL);
        pthread_cond_init(&empty, NULL);
    }

    void PushData(const int &data) {
        LockQueue();
        while(IsFull()) {
            std::cout << "Queue full, producer waits..." << std::endl;
            pthread_cond_wait(&empty, &lock); // 等待消费者消费
        }
        q.push(data);
        std::cout << "Produce data done: " << data << std::endl;
        pthread_cond_signal(&full); // 通知消费者有数据
        UnLockQueue();
    }

    void PopData(int &data) {
        LockQueue();
        while(IsEmpty()) {
            std::cout << "Queue empty, consumer waits..." << std::endl;
            pthread_cond_wait(&full, &lock); // 等待生产者生产
        }
        data = q.front();
        q.pop();
        std::cout << "Consume data done: " << data << std::endl;
        pthread_cond_signal(&empty); // 通知生产者有空位
        UnLockQueue();
    }

    ~BlockQueue() {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&full);
        pthread_cond_destroy(&empty);
    }
};

// 消费者线程函数
void* consumer(void* arg) {
    BlockQueue* bqp = (BlockQueue*)arg;
    int data;
    while(true) {
        bqp->PopData(data);
        sleep(1); // 模拟消费速度
    }
    return nullptr;
}

// 生产者线程函数
void* producer(void* arg) {
    BlockQueue* bqp = (BlockQueue*)arg;
    srand((unsigned long)time(NULL));
    while(true) {
        int data = rand() % 1024;
        bqp->PushData(data);
        sleep(1); // 模拟生产速度
    }
    return nullptr;
}

int main() {
    BlockQueue bq;
    pthread_t c, p;
    pthread_create(&c, NULL, consumer, (void*)&bq);
    pthread_create(&p, NULL, producer, (void*)&bq);

    pthread_join(c, NULL);
    pthread_join(p, NULL);
    return 0;
}
