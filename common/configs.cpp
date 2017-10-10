#include "configs.h"
#include "xmlparser.h"

GlobalConfigs g_configs;

const char *config_file = "../config/configs.xml";

int load_configs()
{
    XmlParser config(config_file);
    if ( config.is_empty() ) 
    {
        return -1;
    }

    // Monitor
    int monitor_0 = config.get_string("Module-Monitor", "LogLevel", g_configs.monitor_log_level);
    int monitor_1 = config.get_string("Module-Monitor", "LogMode", g_configs.monitor_log_mode);
    int monitor_2 = config.get_string("Module-Monitor", "Ip", g_configs.monitor_ip);
    int monitor_3 = config.get_ushort("Module-Monitor", "TcpPortForModuleInterface", g_configs.monitor_tcp_port_for_interface);
    int monitor_4 = config.get_int("Module-Monitor", "TcpRecvTimeout", g_configs.monitor_tcp_recv_timeout);
    int monitor_5 = config.get_int("Module-Monitor", "TcpReconnectTime", g_configs.monitor_tcp_reconnect_time);
    int monitor_6 = config.get_int("Module-Monitor", "TcpReconnectInterval", g_configs.monitor_tcp_reconnect_interval);

    if ( (monitor_0 | monitor_1 | monitor_2 | monitor_3 | monitor_4 | monitor_5 | monitor_6) != 0 )
    {
        return -1;
    }

    // Users
    int users_0 = config.get_string("Module-Users", "LogLevel", g_configs.users_log_level);
    int users_1 = config.get_string("Module-Users", "LogMode", g_configs.users_log_mode);
    int users_2 = config.get_string("Module-Users", "Ip", g_configs.users_ip);
    int users_3 = config.get_ushort("Module-Users", "TcpPortForModuleEquip", g_configs.users_tcp_port_for_equip);
    int users_4 = config.get_ushort("Module-Users", "UdpPortForModuleSafety", g_configs.users_udp_port_for_safety);
    int users_5 = config.get_ushort("Module-Users", "UdpPortForModuleCLinker", g_configs.users_udp_port_for_clinker);

    if ( (users_0 | users_1 | users_2 | users_3 | users_4 | users_5) != 0 )
    {
        return -1;
    }

    // Interface
    int interface_0 = config.get_string("Module-Interface", "LogLevel", g_configs.interface_log_level);
    int interface_1 = config.get_string("Module-Interface", "LogMode", g_configs.interface_log_mode);
    int interface_2 = config.get_string("Module-Interface", "Ip", g_configs.interface_ip);

    if ( (interface_0 | interface_1 | interface_2) != 0 )
    {
        return -1;
    }

    // Safety
    int safety_0 = config.get_string("Module-Safety", "LogLevel", g_configs.safety_log_level);
    int safety_1 = config.get_string("Module-Safety", "LogMode", g_configs.safety_log_mode);
    int safety_2 = config.get_string("Module-Safety", "Ip", g_configs.safety_ip);
    int safety_3 = config.get_ushort("Module-Safety", "UdpPortForModuleUsers", g_configs.safety_udp_port_for_users);
    int safety_4 = config.get_ushort("Module-Safety", "UdpPortForModuleInterface", g_configs.safety_udp_port_for_interface);
    int safety_5 = config.get_ushort("Module-Safety", "UdpPortForModuleCLinker", g_configs.safety_udp_port_for_clinker);

    if ( (safety_0 | safety_1 | safety_2 | safety_3 | safety_4 | safety_5) != 0 )
    {
        return -1;
    }

    // CLinker
    int clinker_0 = config.get_string("Module-CLinker", "LogLevel", g_configs.clinker_log_level);
    int clinker_1 = config.get_string("Module-CLinker", "LogMode", g_configs.clinker_log_mode);
    int clinker_2 = config.get_string("Module-CLinker", "Ip", g_configs.clinker_ip);
    int clinker_3 = config.get_ushort("Module-CLinker", "UdpPortForModuleUsers", g_configs.clinker_udp_port_for_users);
    int clinker_4 = config.get_ushort("Module-CLinker", "UdpPortForModuleInterface", g_configs.clinker_udp_port_for_interface);
    int clinker_5 = config.get_ushort("Module-CLinker", "UdpPortForModuleSafety", g_configs.clinker_udp_port_for_safety);

    if ( (clinker_0 | clinker_1 | clinker_2 | clinker_3 | clinker_4 | clinker_5) != 0 )
    {
        return -1;
    }

    // Proxy
    int proxy_0 = config.get_string("Module-Proxy", "LogLevel", g_configs.proxy_log_level);
    int proxy_1 = config.get_string("Module-Proxy", "LogMode", g_configs.proxy_log_mode);
    int proxy_2 = config.get_string("Module-Proxy", "Ip", g_configs.proxy_ip);
    int proxy_3 = config.get_ushort("Module-Proxy", "TcpPortForModuleUsers", g_configs.proxy_tcp_port_for_users);

    if ( (proxy_0 | proxy_1 | proxy_2 | proxy_3) != 0 )
    {
        return -1;
    }

    return 0;
}
