#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#include <iostream>

using std::string;

typedef struct _global_configs
{
    // Monitor
    string monitor_log_level;
    string monitor_log_mode;
    string monitor_ip;
    unsigned short monitor_tcp_port_for_interface;
    int monitor_tcp_recv_timeout;
    int monitor_tcp_reconnect_time;
    int monitor_tcp_reconnect_interval;
     
    // Users
    string users_log_level;
    string users_log_mode;
    string users_ip;
    unsigned short users_tcp_port_for_equip;
    unsigned short users_udp_port_for_safety;
    unsigned short users_udp_port_for_clinker;

    // Interface
    string interface_log_level;
    string interface_log_mode;
    string interface_ip;

    // Safety
    string safety_log_level;
    string safety_log_mode;
    string safety_ip;
    unsigned short safety_udp_port_for_users;
    unsigned short safety_udp_port_for_interface;
    unsigned short safety_udp_port_for_clinker;

    // CLinker
    string clinker_log_level;
    string clinker_log_mode;
    string clinker_ip;
    unsigned short clinker_udp_port_for_users;
    unsigned short clinker_udp_port_for_interface;
    unsigned short clinker_udp_port_for_safety;

    // Proxy
    string proxy_log_level;
    string proxy_log_mode;
    string proxy_ip;
    unsigned short proxy_tcp_port_for_users;

}GlobalConfigs;


// 全局配置信息
extern GlobalConfigs g_configs;

int load_configs();

#endif


