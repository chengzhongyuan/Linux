#include <iostream> 
#include <vector> 
#include <stdlib.h> 
#include <semaphore.h> 
#include <pthread.h> 
#include <unistd.h>
#define NUM 16   // 队列容量默认值

// 环形队列类，使用信号量实现生产者-消费者模型
class RingQueue{ 
private: 
    std::vector<int> q;     // 用 vector 模拟环形队列
    int cap;                // 队列容量
    sem_t data_sem;         // 数据信号量：表示当前队列中已有数据的数量
    sem_t space_sem;        // 空间信号量：表示当前队列中剩余空间的数量
    int consume_step;       // 写入位置（生产者下标）
    int product_step;       // 读取位置（消费者下标）

public: 
    // 构造函数：初始化队列和信号量
    RingQueue(int _cap = NUM):q(_cap),cap(_cap) 
    { 
        sem_init(&data_sem, 0, 0);     // 初始时队列中没有数据
        sem_init(&space_sem, 0, cap);  // 初始时队列空间为容量大小
        consume_step = 0;              // 写指针初始化
        product_step = 0;              // 读指针初始化
    } 

    // 生产者放入数据
    void PutData(const int &data) 
    { 
        sem_wait(&space_sem);          // 等待有空位（P操作）
        q[consume_step] = data;        // 写入数据到队列当前位置
        consume_step++;                // 写指针后移
        consume_step %= cap;           // 环绕（模运算）
        sem_post(&data_sem);           // 通知有新数据（V操作）
    } 

    // 消费者取出数据
    void GetData(int &data) 
    { 
        sem_wait(&data_sem);           // 等待有数据（P操作）
        data = q[product_step];        // 取出数据
        product_step++;                // 读指针后移
        product_step %= cap;           // 环绕（模运算）
        sem_post(&space_sem);          // 通知有新空间（V操作）
    } 

    // 析构函数：销毁信号量
    ~RingQueue() 
    { 
        sem_destroy(&data_sem); 
        sem_destroy(&space_sem); 
    } 
}; 

// 消费者线程函数
void *consumer(void *arg) 
{ 
    RingQueue *rqp = (RingQueue*)arg; 
    int data; 
    for( ; ; ){ 
        rqp->GetData(data); 
        std::cout << "Consume data done : " << data << std::endl; 
        sleep(1); // 模拟消费速度较慢
    } 
} 

// 生产者线程函数
void *producter(void *arg) 
{ 
    RingQueue *rqp = (RingQueue*)arg; 
    srand((unsigned long)time(NULL)); 
    for( ; ; ){ 
        int data = rand() % 1024;      // 随机生成数据
        rqp->PutData(data); 
        std::cout << "Prodoct data done: " << data << std::endl; 
        // sleep(1); // 如果打开，生产速度会变慢
    } 
} 

// 主函数：创建环形队列，启动生产者和消费者线程
int main() 
{ 
    RingQueue rq; 
    pthread_t c,p; 
    pthread_create(&c, NULL, consumer, (void*)&rq);   // 创建消费者线程
    pthread_create(&p, NULL, producter, (void*)&rq);  // 创建生产者线程
    pthread_join(c, NULL);   // 等待消费者线程结束（实际上不会结束）
    pthread_join(p, NULL);   // 等待生产者线程结束（实际上不会结束）
} 
