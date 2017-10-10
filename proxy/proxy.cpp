#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "configs.h" 
#include "frames.h" 
#include "common.h" 
#include "net.h" 
#include "crc.h" 
#include "log.h" 

using namespace std;

int g_cfd = 0;

// 注册过程模拟
int regist()
{

    return 0;
}

// 位置上报过程模拟
int report_position()
{
    LogDebug("开始位置上报过程模拟\n");

    unsigned char start_1 = 0xff;
    unsigned char start_2 = 0x7e;
    unsigned short framelen = E_USER_FRAME_LEN;
    unsigned char frame_type = E_USER_FRAME;
    unsigned char module = E_PROXY_PLATFORM_MODULE;
    unsigned int station = 1;
    unsigned char IMEI[16] = "869515020173842";
    unsigned char precision_type= E_GPS_COMMON;
    double longitude = 39.095119476;
    double latitude = 117.273933411;
    double altitude = 0;
    unsigned char commqual = 0;
    unsigned char gpsqual = 0;
    unsigned char powerratio = 80;
    unsigned short crc = 0;

    unsigned char buff[1024] = {0};
    unsigned char *tmp = buff;

    memcpy(tmp, &start_1, sizeof(start_1));
    tmp++;

    memcpy(tmp, &start_2, sizeof(start_2));
    tmp++;

    memcpy(tmp, &framelen, sizeof(framelen));
    tmp += sizeof(framelen);

    memcpy(tmp, &frame_type, sizeof(frame_type));
    tmp++;

    memcpy(tmp, &module, sizeof(module));
    tmp++;

    memcpy(tmp, &station, sizeof(station));
    tmp += sizeof(station);

    memcpy(tmp, IMEI, sizeof(IMEI)-1);
    tmp += sizeof(IMEI)-1;

    memcpy(tmp, &precision_type, sizeof(precision_type));
    tmp += sizeof(precision_type);

    memcpy(tmp, &longitude, sizeof(longitude));
    tmp += sizeof(longitude);

    memcpy(tmp, &latitude, sizeof(latitude));
    tmp += sizeof(latitude);

    memcpy(tmp, &altitude, sizeof(altitude));
    tmp += sizeof(altitude);

    memcpy(tmp, &commqual, sizeof(commqual));
    tmp += sizeof(commqual);

    memcpy(tmp, &gpsqual, sizeof(gpsqual));
    tmp += sizeof(gpsqual);

    memcpy(tmp, &powerratio, sizeof(powerratio));
    tmp += sizeof(powerratio);

    crc = calc_crc(buff, tmp - buff);
    memcpy(tmp, &crc, sizeof(crc));
    tmp += sizeof(crc);

    if ( (tmp - buff) != E_USER_FRAME_LEN )
    {
        LogError("用户信息帧组包错误！(%d/%d)\n", tmp - buff, E_USER_FRAME_LEN);
        return -1;
    }
    LogDebug("用户信息帧组包成功\n");

    if ( Send(g_cfd, buff, E_USER_FRAME_LEN) < 0 )
    {
        LogError("用户信息帧发送失败\n");
        return -1;
    }
    LogDebug("用户信息帧发送成功\n");

    int ret = RecvAll(g_cfd, buff, E_RECV_ACK_FRAME_LEN);
    if ( ret < 0 )
    {
        LogError("信息接收确认帧接收失败: %d\n", ret);
        return -1;
    }
    LogDebug("信息接收确认帧接收成功\n");
    LogDebug("位置上报过程模拟完成\n");

    return 0;
}

int main()
{
	signal(SIGPIPE,SIG_IGN);

    // 获取全局系统配置
    if ( load_configs() < 0 )
    {
        LogError("load_configs fail!\n");
        return -1;
    }

    // 日志设置
    if ( log_init( g_configs.proxy_log_level, g_configs.proxy_log_mode, "./log/" ) < 0 )
    {
        LogError("log_config error!\n");
        return -1;
    }

	int sfd = GetTcpSocket();
    if ( sfd < 0 )
    {
        LogError("GetTcpSocket error\n");
        return -1;
    }

    if( Bind(sfd, g_configs.proxy_ip.c_str(), g_configs.proxy_tcp_port_for_users) < 0 )
    {
        LogError("Bind fail\n");
        return -1;
    }

	Listen(sfd,10);
	
    struct sockaddr_in caddr;
    g_cfd = Accept(sfd, &caddr);
    if ( g_cfd < 0 )
    {
        LogWarning("Accept error\n");
        return -1;
    }
    LogDebug("accept a connection from: %s\n", inet_ntoa(caddr.sin_addr));


    // 注册模拟
    // regist();
     
    // 位置上报模拟
    report_position();

    CloseSock(sfd);

	return 0;
}
