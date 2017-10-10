#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <stdio.h>
#include <pthread.h>

using std::string;

enum e_log_level
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

enum e_log_mode{
    LOG_CONSOLE,
    LOG_LOGFILE,
    LOG_DUAL
};

//日志文件最大长度
const int LOG_MAX_FILE_SIZE = 10*1024*1024;

// #define LOG_ADD_FORM  "[%lu][%s:%d] "
#define LOG_ADD_FORM  "[%lx][%s:%d] "

//调试类信息打印
#define LogDebug(format, args...) \
    log_print(LOG_DEBUG, LOG_ADD_FORM format, pthread_self(), __FILE__, __LINE__, ##args)

//通知类信息打印
#define LogInfo(format, args...) \
    log_print(LOG_INFO, LOG_ADD_FORM format, pthread_self(), __FILE__, __LINE__, ##args)

//警告类信息打印
#define LogWarning(format, args...) \
    log_print(LOG_WARNING, LOG_ADD_FORM format, pthread_self(), __FILE__, __LINE__, ##args)

//错误类信息打印
#define LogError(format, args...) \
    log_print(LOG_ERROR, LOG_ADD_FORM format, pthread_self(), __FILE__, __LINE__, ##args)

//危险类信息打印
#define LogCritical(format, args...) \
    log_print(LOG_CRITICAL, LOG_ADD_FORM format, pthread_self(), __FILE__, __LINE__, ##args)

//普通打印
#define Print(format, args...) \
    printf(format, ##args)

//内存十六进制对齐打印
#define LogMemAlignPrint(mem, len) \
    for( int i = 0; i < len; ++i )  \
    {                               \
        printf("%.2x ", mem[i]);    \
        if ( (i+1) % 30 == 0 ) {    \
            printf("\n");           \
            continue;               \
        }                           \
        if ( (i+1) % 5 == 0 )       \
        {                           \
            printf(" ");            \
        }                           \
    }                               \
    printf("\n");

//内存十六进制打印
#define LogMemPrint(mem, len) \
    for( int i = 0; i < len; ++i )  \
    {                               \
        printf("%.2x ", mem[i]);    \
    }                               \
    printf("\n");

#define LogStrPrint(buff)           \
{                                   \
    int i = 0;                      \
    while( buff[i] != '\0' )        \
    {                               \
        printf("%c", buff[i]);     \
        i++;                        \
    }                               \
    printf("\n");                   \
}


int log_init(string level, string mode, const char *file_path);
void log_print(int level, const char *fmt, ...);

#endif
