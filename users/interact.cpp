#include <unistd.h>
#include <string.h>
#include "interact.h"
#include "configs.h"
#include "frames.h"
#include "worker.h"
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
    // 与转发代理平台端TCP接口
    m_proxy_fd = GetTcpSocket();
    if ( m_proxy_fd < 0 )
    {
        LogError("GetTcpSocket error\n");
        return -1;
    }
    if ( Connect(m_proxy_fd, g_configs.proxy_ip.c_str(), g_configs.proxy_tcp_port_for_users) < 0 )
    {
        CloseSock(m_proxy_fd);
        LogError("Connect %s:%d fail\n", g_configs.proxy_ip.c_str(), g_configs.proxy_tcp_port_for_users);
        return -1;
    }

    // 与安全处理模块UDP接收接口
    m_safety_rfd = GetUdpBindSocket(g_configs.safety_ip.c_str(), g_configs.users_udp_port_for_safety);
    if ( m_safety_rfd < 0 )
    {
        CloseSock(m_safety_rfd);
        LogError("GetUdpBindSocket error\n");
        return -1;
    }
     
    // 与客户端连接模块UDP接收接口
    m_clinker_rfd = GetUdpBindSocket(g_configs.users_ip.c_str(), g_configs.users_udp_port_for_clinker);
    if ( m_clinker_rfd < 0 )
    {
        CloseSock(m_clinker_rfd);
        LogError("GetUdpBindSocket error\n");
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

// 开始模块间交互信息处理 
int Interactor::start()
{
    unsigned char buff[g_frames_max_size] = {0};
    Worker worker;

    while(1)
    {   
        memset(buff, 0, sizeof(buff));
        int framelen = recv_module_proxy(buff, sizeof(buff));
        if ( framelen < 0 )
        {
            LogWarning("recv_module_proxy fail\n");
            sleep(2);
            continue;
        }

        worker.handle(buff, framelen);
    }   

    return 0;
}

// 向转发代理平台发送数据
int Interactor::send_module_proxy(unsigned char *buff, int len)
{
    return Send(m_proxy_fd, buff, len);
}

// 向转发代理平台接收数据
int Interactor::recv_module_proxy(unsigned char *buff, int len)
{
    int ret = FackRecv(m_proxy_fd, buff, 5);
    if ( ret <= 0 )
    {
        LogWarning("Proxy outline or Recv error\n");
        return -1;
    }

    unsigned short framelen = *(unsigned short *)(buff + 2);
    if ( framelen > len )
    {
        LogWarning("frame too long: %d/%d\n", framelen, len);
        ClearRecv(m_proxy_fd);
        return -1;
    }

    ret = PersistRecv(m_proxy_fd, buff, framelen);
    if ( ret <= 0 )
    {
        LogWarning("Proxy outline or Recv error\n");
        ClearRecv(m_proxy_fd);
        return -1;
    }

    return ret;
}

// 向安全处理模块发送数据
int Interactor::send_to_module_safety(unsigned char *buff, int len)
{
    return SendTo(m_udp_wfd, g_configs.safety_ip.c_str(), g_configs.safety_udp_port_for_users, buff, len);
}

// 向安全处理模块接收数据
int Interactor::recv_from_module_safety(unsigned char *buff, int len)
{
    return RecvFrom(m_safety_rfd, buff, len);
}

// 向客户端连接模块发送数据
int Interactor::send_to_module_clinker(unsigned char *buff, int len)
{
    return SendTo(m_udp_wfd, g_configs.clinker_ip.c_str(), g_configs.clinker_udp_port_for_users, buff, len);
}

// 向客户端连接模块接收数据
int Interactor::recv_from_module_clinker(unsigned char *buff, int len)
{
    return RecvFrom(m_clinker_rfd, buff, len);
}

