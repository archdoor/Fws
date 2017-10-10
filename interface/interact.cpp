#include <unistd.h>
#include <string.h>
#include "interact.h"
#include "configs.h"
#include "btnframe.h"
#include "worker.h"
#include "msgque.h"
#include "datafile.h"
#include "net.h"
#include "log.h"

Interactor *Interactor::m_interactor = NULL;

Interactor::Interactor()
{
    
}

// 获取单例对象
Interactor *Interactor::get_instance()
{
    if ( m_interactor == NULL )
    {
        LogDebug("get new Interactor instance\n");
        m_interactor = new Interactor();
    }
    return m_interactor;
}

// 释放单例对象
int Interactor::release_instance()
{
    if ( m_interactor != NULL )
    {
        LogDebug("release Interactor instance\n");
        delete m_interactor;
    }
    return 0;
}

// 模块间接口初始化
int Interactor::init()
{
    // 与集中监测系统TCP接口 
    if ( connect_monitor() < 0 )
    {
        LogError("connect_monitor fail\n");
        return -1;
    }

    // 与其它模块UDP数据发送接口
    m_udp_wfd = GetUdpSocket();
    if ( m_udp_wfd < 0 )
    {
        CloseSock(m_udp_wfd);
        LogError("GetUdpSocket fail\n");
        return -1;
    }
     
    return 0;
}

// 数据接收线程
void *recv_thread_fn(void *args)
{
    LogDebug("start recv data from monitor\n");

    Interactor *actor = (Interactor *)args;
    unsigned char buff[2048] = {0};

    BtnFrame btnframe;

    while(1)
    {
        memset(buff, 0, sizeof(buff));
        int recv_len = actor->recv_module_monitor(buff, sizeof(buff) - 1);
        if( recv_len <= 0 )
        {
            LogWarning("recv timeout or monitor outline!\n");
            if( actor->reconnect_monitor() < 0 )
            {
                LogError("reconnect monitor fail!\n");
                pthread_exit(NULL);
            }
            else
            {
                // 重连成功后，清空数据缓存
                btnframe.frame_clean();
                continue;
            }
        }
        LogDebug("recv data len: %d\n", recv_len);
        btnframe.push_data(buff, recv_len);
    }

    return NULL;
}

// 数据处理线程
void *handle_thread_fn(void *args)
{
    LogDebug("start data save thread\n");

    BtnFrame btnframe;
    DataFile file;
    Worker worker;

    while(1)
    {
        btnframe.frame_clean();
        if( btnframe.get_frame() < 0 )
        {
            LogError("get_frame error!\n");
            continue;
        }

        // 数据帧保存到文件
        file.save_to_file( btnframe );
        
        // 数据帧处理
        worker.handle(btnframe.m_msgque.m_mess.text, btnframe.framelen());
    }

    return NULL;
}
 
// 开始模块间交互信息处理 
int Interactor::start()
{
    // 创建数据接收线程
    pthread_t pid_recv = 0;
    if( pthread_create(&pid_recv, NULL, recv_thread_fn, (void *)this) != 0 ) {
        LogError("create data recv thread failed!\n");
        return -1;
    }
    pthread_detach(pid_recv);

    // 创建数据处理线程
    pthread_t pid_save = 0;
    if( pthread_create(&pid_save, NULL, handle_thread_fn, NULL) != 0 ) {
        LogError("create data handle thread failed!\n");
        return -1;
    }
    pthread_detach(pid_save);

    return 0;
}

// 连接集中监测系统模块
int Interactor::connect_monitor()
{
    m_tcp_monitor_fd = GetTcpSocket();
    if ( m_tcp_monitor_fd < 0 )
    {
        LogError("GetTcpSocket error\n");
        return -1;
    }

    if ( SetSockTimeout(m_tcp_monitor_fd, g_configs.monitor_tcp_recv_timeout) < 0 )
    {
        LogError("SetSockTimeout error\n");
        return -1;
    }

    if ( Connect(m_tcp_monitor_fd, g_configs.monitor_ip.c_str(), g_configs.monitor_tcp_port_for_interface) < 0 )
    {
        CloseSock(m_tcp_monitor_fd);
        LogError("Connect error\n");
        return -1;
    }

    return 0;
}

// 重新连接集中监测系统模块
int Interactor::reconnect_monitor()
{
    int reconnect_time = g_configs.monitor_tcp_reconnect_time;

    while(1)
    {
        // TODO:向安全处理模块发送告警信息
        CloseSock(m_tcp_monitor_fd);
        LogWarning("trying to reconnect... \n");

        sleep(g_configs.monitor_tcp_reconnect_interval);

        if( reconnect_monitor() == 0 )
        {
            LogWarning("reconnect success\n");
            // TODO:向安全处理模块发送解除告警信息
            return 0;
        }
        LogWarning("reconnect fail\n");

        if ( reconnect_time != 0 )
        {
            reconnect_time--;
            if ( reconnect_time <= 0 )
                break;
        }
    }
    return -1;
}

// 从集中监测系统模块接收数据
int Interactor::recv_module_monitor(unsigned char *buff, int len)
{
    return RecvFrom(m_tcp_monitor_fd, buff, len);
}

// 向安全处理模块发送数据
int Interactor::send_to_module_safety(unsigned char *buff, int len)
{
    return SendTo(m_udp_wfd, g_configs.safety_ip.c_str(), g_configs.safety_udp_port_for_interface, buff, len);
}

// 向客户端连接模块发送数据
int Interactor::send_to_module_clinker(unsigned char *buff, int len)
{
    return SendTo(m_udp_wfd, g_configs.clinker_ip.c_str(), g_configs.clinker_udp_port_for_interface, buff, len);
}

