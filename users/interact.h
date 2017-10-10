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

    // 与转发代理平台端信息交互
    int send_module_proxy(unsigned char *buff, int len);
    int recv_module_proxy(unsigned char *buff, int len);

    // 安全处理模块信息交互
    int send_to_module_safety(unsigned char *buff, int len);
    int recv_from_module_safety(unsigned char *buff, int len);

    // 客户端连接模块信息交互
    int send_to_module_clinker(unsigned char *buff, int len);
    int recv_from_module_clinker(unsigned char *buff, int len);

    
    // 与转发代理平台端TCP接口
    int m_proxy_fd;
    // 与安全处理模块UDP接收接口
    int m_safety_rfd;
    // 与客户端连接模块UDP接收接口
    int m_clinker_rfd;

    // 与其它模块UDP数据发送接口
    int m_udp_wfd;

private:
    static Interactor *m_interactor;
};




#endif


