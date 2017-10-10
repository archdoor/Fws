#include <unistd.h>
#include <string.h>
#include <sys/select.h>
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
    // 与用户信息处理模块UDP接收接口
    m_users_rfd = GetUdpBindSocket(g_configs.safety_ip.c_str(), g_configs.safety_udp_port_for_users);
    if ( m_users_rfd < 0 )
    {
        CloseSock(m_users_rfd);
        LogError("GetUdpBindSocket error\n");
        return -1;
    }
    // 与用户信息处理模块UDP发送接口
    m_users_wfd = GetUdpSocket();
    if ( m_users_wfd < 0 )
    {
        CloseSock(m_users_wfd);
        LogError("GetUdpSocket error\n");
        return -1;
    }

    // 与客户端连接模块UDP接收接口
    m_clinker_rfd = GetUdpBindSocket(g_configs.safety_ip.c_str(), g_configs.safety_udp_port_for_clinker);
    if ( m_clinker_rfd < 0 )
    {
        CloseSock(m_clinker_rfd);
        LogError("GetUdpBindSocket error\n");
        return -1;
    }
    // 与客户端连接模块UDP发送接口
    m_clinker_wfd = GetUdpSocket();
    if ( m_clinker_wfd < 0 )
    {
        CloseSock(m_clinker_wfd);
        LogError("GetUdpSocket error\n");
        return -1;
    }

    // 与集中监测系统接口模块UDP接收接口
    m_interface_rfd = GetUdpBindSocket(g_configs.safety_ip.c_str(), g_configs.safety_udp_port_for_interface);
    if ( m_interface_rfd < 0 )
    {
        CloseSock(m_interface_rfd);
        LogError("GetUdpBindSocket error\n");
        return -1;
    }
    return 0;
}

// 开始模块间交互信息处理 
int Interactor::start()
{
    unsigned char buff[g_frames_max_size] = {0};
    Worker worker;

    fd_set fd_read_set;
    while(1)
    {   
        FD_ZERO(&fd_read_set);
        FD_SET(m_users_rfd, &fd_read_set);
        FD_SET(m_clinker_rfd, &fd_read_set);
        FD_SET(m_interface_rfd, &fd_read_set);

        if ( select(FD_SETSIZE, &fd_read_set, NULL, NULL, NULL) < 0 )
        {
            LogWarning("select error\n");
        }
        else 
        {
            if ( FD_ISSET(m_users_rfd, &fd_read_set) )
            {
                int framelen = recv_from_module_users(buff, sizeof(buff));
                if ( framelen < 0 )
                {
                    LogWarning("recv_from_module_users fail\n");
                }
                worker.handle(buff, framelen);
            }

            if ( FD_ISSET(m_clinker_rfd, &fd_read_set) )
            {
                int framelen = recv_from_module_clinker(buff, sizeof(buff));
                if ( framelen < 0 )
                {
                    LogWarning("recv_from_module_clinker fail\n");
                }
                LogDebug("收到来自clinker的信息帧\n");
                // worker.handle(buff, framelen);
            }

            if ( FD_ISSET(m_interface_rfd, &fd_read_set) )
            {
                int framelen = recv_from_module_interface(buff, sizeof(buff));
                if ( framelen < 0 )
                {
                    LogWarning("recv_from_module_interface fail\n");
                }
                LogDebug("收到来自interface的信息帧\n");
                // worker.handle(buff, framelen);
            }
        }
    }

    return 0;
}

// 向用户信息处理模块接收数据
int Interactor::recv_from_module_users(unsigned char *buff, int len)
{
    return RecvFrom(m_users_rfd, buff, len);
}

// 向用户信息处理模块发送数据
int Interactor::send_to_module_users(unsigned char *buff, int len)
{
    return SendTo(m_users_wfd, g_configs.users_ip.c_str(), g_configs.users_udp_port_for_safety, buff, len);
}

// 向客户端连接模块接收数据
int Interactor::recv_from_module_clinker(unsigned char *buff, int len)
{
    return RecvFrom(m_clinker_rfd, buff, len);
}

// 向客户端连接模块发送数据
int Interactor::send_to_module_clinker(unsigned char *buff, int len)
{
    return SendTo(m_clinker_wfd, g_configs.clinker_ip.c_str(), g_configs.clinker_udp_port_for_safety, buff, len);
}

// 向集中监测系统接口模块接收数据
int Interactor::recv_from_module_interface(unsigned char *buff, int len)
{
    return RecvFrom(m_interface_rfd, buff, len);
}

