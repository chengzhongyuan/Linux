// 封装一个日志接口
#pragma once

#include <time.h>
#include <iostream>
#include <string>
#include <cstdarg> // 提供可变参数
#include <unistd.h> // 获取 pid

#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERROR 3
#define FATAL 4

// 辅助函数将这些错误代号转换成字符串
const char * LevelToString(int level)
{
    switch (level)
    {
        case DEBUG: return "DEBUG";
        case NORMAL: return "NORMAL";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        case FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

void LogMessage(int level,const char* format, ...)
{
    // [日志等级] [时间戳] [pid] [message]
    // 1、获取日志等级字符串
    const char* level_string = LevelToString(level);

    // 2、获取格式化的时间
    time_t t = time(nullptr);
    struct tm* ctime = localtime(&t);
    char time_str[64];
    snprintf(time_str, sizeof(time_str), "%d-%02d-%02d %02d:%02d:%02d",
             ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
             ctime->tm_hour, ctime->tm_min, ctime->tm_sec);
    
    // 3、获取当前进程的PID
    pid_t pid = getpid();

    // 4、处理可变参数这部分是最重要的，作用就是：把类似 "hello %d" + 参数，拼成最终字符串，写进 msg_buf
    char msg_buf[1024];
    va_list args;
    va_start(args, format); // 初始化可变参数列表
    vsnprintf(msg_buf, sizeof(msg_buf), format, args); // 像 printf 一样把内容写进 msg_buf
    va_end(args); // 清理可变参数列表

    printf("[%s] [%s] [PID:%d] %s\n", level_string, time_str, pid, msg_buf);
    // 这一步也可以换成写入文件的操作
}