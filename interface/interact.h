#ifndef __INTERACT_H__
#define __INTERACT_H__

// 模块间信息交互类
class Interactor
{
public:
    Interactor();
    static Interactor *get_instance();
    int release_instance();
    int init();
    int start();

    // 连接集中监测系统模块
    int connect_monitor();
    // 重新连接集中监测系统模块
    int reconnect_monitor();
    // 集中监测系统模块信息交互
    int recv_module_monitor(unsigned char *buff, int len);

    // 安全处理模块信息交互
    int send_to_module_safety(unsigned char *buff, int len);

    // 客户端连接模块信息交互
    int send_to_module_clinker(unsigned char *buff, int len);

    
    // 与集中监测系统TCP接口 
    int m_tcp_monitor_fd;

    // 与其它模块UDP数据发送接口
    int m_udp_wfd;

private:
    static Interactor *m_interactor;
};




#endif


