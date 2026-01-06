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
    // 创建了两个条件变量一个是链接生产者线程
    // 另一个是链接消费者线程
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
        // 这个其中也可以增加一个水位线，避免频繁的唤醒休眠
        // 这个while可以确保再次进入循环进行判定保证代码健壮性
        while(IsFull()) {
            std::cout << "Queue full, producer waits..." << std::endl;
            // 这个条件的第二个参数必须是锁，它会自动的在内部讲锁释放然后把自己挂起
            pthread_cond_wait(&empty, &lock); // 生产者线程进行等待，等待变空
        }
        q.push(data);
        std::cout << "Produce data done: " << data << std::endl;
        // 唤醒的时候会自动的给自己加上锁
        pthread_cond_signal(&full); // 唤醒消费者线程
        UnLockQueue();
    }

    void PopData(int &data) {
        LockQueue();
        while(IsEmpty()) {
            std::cout << "Queue empty, consumer waits..." << std::endl;
            pthread_cond_wait(&full, &lock); // 消费者线程进行等待，等待有
        }
        data = q.front();
        q.pop();
        std::cout << "Consume data done: " << data << std::endl;
        pthread_cond_signal(&empty); // 唤醒生产者线程
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
    // 全部都是对这个类进行操作对吗

    BlockQueue bq;
    pthread_t c, p;
    pthread_create(&c, NULL, consumer, (void*)&bq);
    pthread_create(&p, NULL, producer, (void*)&bq);

    pthread_join(c, NULL);
    pthread_join(p, NULL);
    return 0;
}
