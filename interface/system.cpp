#include <iostream>
#include "system.h"
#include "xmlparser.h"
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

    //系统配置
    if ( load_configs() != 0 )
    {
        LogError("load_sys_config fail!\n");
        return -1;
    }

    //日志设置
    if ( log_init( g_configs.interface_log_level, g_configs.interface_log_mode, "./log/" ) < 0 )
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

    //服务端数据接收与保存线程
    // DataRecever *recever = DataRecever::get_instance();
    // if ( recever->init() < 0 )
    // {
    //     LogError("recever init fail!\n");
    //     return -1;
    // }
    // if( recever->start() < 0 )
    // {
    //     LogError("recever start fail!\n");
    //     return -1;
    // }

	return 0;
}

void SystemDeinit()
{
}
