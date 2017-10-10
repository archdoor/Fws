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

    // 与用户信息处理模块信息交互
    int recv_from_module_users(unsigned char *buff, int len);
    int send_to_module_users(unsigned char *buff, int len);

    // 与集中监测系统接口模块信息交互
    int recv_from_module_interface(unsigned char *buff, int len);

    // 与安全处理模块信息交互
    int recv_from_module_safety(unsigned char *buff, int len);
    int send_to_module_safety(unsigned char *buff, int len);

    
    // 与用户信息处理模块UDP接发接口
    int m_users_rfd;
    int m_users_wfd;
    // 与集中监测系统接口模块UDP接收接口
    int m_interface_rfd;
    // 与安全处理模块UDP收发接口
    int m_safety_rfd;
    int m_safety_wfd;

private:
    static Interactor *m_interactor;
};




#endif


