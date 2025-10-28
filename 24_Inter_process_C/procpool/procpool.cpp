#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MakeSeed() srand((unsigned long)time(nullptr) ^ getpid() ^ 0x171237 ^ rand() % 1234)

#define PROCSS_NUM 10

///////////////////////////////////////�ӽ���Ҫ��ɵ�ĳ������ -- ģ��һ��/////////////////////////
// ����ָ�� ����
typedef void (*func_t)();

void downLoadTask()
{
    std::cout << getpid() << ": ��������\n"
              << std::endl;
    sleep(1);
}

void ioTask()
{
    std::cout << getpid() << ": IO����\n"
              << std::endl;
    sleep(1);
}

void flushTask()
{
    std::cout << getpid() << ": ˢ������\n"
              << std::endl;
    sleep(1);
}

void loadTaskFunc(std::vector<func_t> *out)
{
    assert(out);
    out->push_back(downLoadTask);
    out->push_back(ioTask);
    out->push_back(flushTask);
}

/////////////////////////////////////////////////����Ĵ�����һ������̳���//////////////////////
class subEp // Endpoint
{
public:
    subEp(pid_t subId, int writeFd)
        : subId_(subId), writeFd_(writeFd)
    {
        char nameBuffer[1024];
        snprintf(nameBuffer, sizeof nameBuffer, "process-%d[pid(%d)-fd(%d)]", num++, subId_, writeFd_);
        name_ = nameBuffer;
    }

public:
    static int num;
    std::string name_;
    pid_t subId_;
    int writeFd_;
};

int subEp::num = 0;

int recvTask(int readFd)
{
    int code = 0;
    ssize_t s = read(readFd, &code, sizeof code);
    if(s == 4) return code;
    else if(s <= 0) return -1;
    else return 0;
}

void sendTask(const subEp &process, int taskNum)
{
    std::cout << "send task num: " << taskNum << " send to -> " << process.name_ << std::endl;
    int n = write(process.writeFd_, &taskNum, sizeof(taskNum));
    assert(n == sizeof(int));
    (void)n;
}

void createSubProcess(std::vector<subEp> *subs, std::vector<func_t> &funcMap)
{
    std::vector<int> deleteFd;
    for (int i = 0; i < PROCSS_NUM; i++)
    {
        int fds[2];
        int n = pipe(fds);
        assert(n == 0);
        (void)n;
        // �����̴򿪵��ļ����ǻᱻ�ӽ��̹����
        // �����Ŷ��뼸��
        pid_t id = fork();
        if (id == 0)
        {
            for(int i = 0; i < deleteFd.size(); i++) close(deleteFd[i]);
            // �ӽ���, ���д�������
            close(fds[1]);
            while (true)
            {
                // 1. ��ȡ�����룬���û�з��ͣ������ӽ���Ӧ������
                int commandCode = recvTask(fds[0]);
                // 2. �������
                if (commandCode >= 0 && commandCode < funcMap.size())
                    funcMap[commandCode]();
                else if(commandCode == -1) break;
            }
            exit(0);
        }
        close(fds[0]);
        subEp sub(id, fds[1]);
        subs->push_back(sub);
        deleteFd.push_back(fds[1]);
    }
}

void loadBlanceContrl(const std::vector<subEp> &subs, const std::vector<func_t> &funcMap, int count)
{
    int processnum = subs.size();
    int tasknum = funcMap.size();
    bool forever = (count == 0 ? true : false);

    while (true)
    {
        // 1. ѡ��һ���ӽ��� --> std::vector<subEp> -> index - �����
        int subIdx = rand() % processnum;
        // 2. ѡ��һ������ --> std::vector<func_t> -> index
        int taskIdx = rand() % tasknum;
        // 3. �����͸�ѡ��Ľ���
        sendTask(subs[subIdx], taskIdx);
        sleep(1);
        if(!forever)
        {
            count--;
            if(count == 0) break;   
        }
    }
    // write quit -> read 0
    for(int i = 0; i < processnum; i++) close(subs[i].writeFd_); // waitpid();
}

    
void waitProcess(std::vector<subEp> processes)
{
    int processnum = processes.size();
    for(int i = 0; i < processnum; i++)
    {
        waitpid(processes[i].subId_, nullptr, 0);
        std::cout << "wait sub process success ...: " << processes[i].subId_ << std::endl;
    }
}

int main()
{
    MakeSeed();
    // 1. �����ӽ��̲��������ӽ���ͨ�ŵ��ŵ�, ��bug�ģ����ǲ�Ӱ�����Ǻ����д
    // 1.1 ���ط�����
    std::vector<func_t> funcMap;
    loadTaskFunc(&funcMap);
    // 1.2 �����ӽ��̣�����ά���ø���ͨ���ŵ�
    std::vector<subEp> subs;
    createSubProcess(&subs, funcMap);

    // 2. �ߵ�������Ǹ�����, �����ӽ��̣����ؾ�������ӽ��̷���������
    int taskCnt = 3; // 0: ��Զ����
    loadBlanceContrl(subs, funcMap, taskCnt);

    // 3. �����ӽ�����Ϣ
    waitProcess(subs);

    return 0;
}
