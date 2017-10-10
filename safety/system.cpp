#include <iostream>
#include "system.h"
#include "configs.h"
#include "interact.h"
#include "log.h"

void *service_thread_fn(void *args)
{
    Interactor *actor = Interactor::get_instance();
    if ( actor->init() < 0 )
    {
        LogError("Interactor init error!\n");
        return NULL;
    }

    actor->start();

    return NULL;
}

int SystemInit(int argc, char** argv)
{
    LogDebug("start system init...\n");

    // 获取全局系统配置
    if ( load_configs() < 0 )
    {
        LogError("load_configs fail!\n");
        return -1;
    }

    // 日志设置
    if ( log_init( g_configs.safety_log_level, g_configs.safety_log_mode, "./log/" ) < 0 )
    {
        LogError("log_config error!\n");
        return -1;
    }

    // 开启业务处理线程
    pthread_t pid = 0;
    if( pthread_create(&pid, NULL, service_thread_fn, NULL) != 0 )
    {
        LogError("create thread failed!\n");
        return -1;
    }
    pthread_detach(pid);

    return 0;
}

void SystemDeinit()
{
}


