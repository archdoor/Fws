#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <arpa/inet.h>
#include "crc.h" 
#include "log.h" 

using namespace std;

// const char *g_data_dir = "./data/tmp/";
const char *g_data_dir = "./data/";

const char *g_monitor_ip = "127.0.0.1";
unsigned short g_monitor_port = 8888;

int intervel_time = 100*1000;

int g_frame_len = 10*1024;

struct Param
{
    int fd;
    struct sockaddr_in addr; 
};

int reverse_frame(unsigned char *frame, unsigned short frame_len)
{
    unsigned char buff[g_frame_len] = {0};
    unsigned char *tmp = buff;
    int length = frame_len;

    memcpy(tmp++, &frame[0], 1);
    for( int i = 1; i < frame_len - 1; ++i )
    {
        if( frame[i] == 0x01 || frame[i] == 0x03 || frame[i] == 0x10 )
        {
            unsigned short reverse = 0;
            switch(frame[i])
            {
                case 0x01:
                    reverse = 0x8110; break;
                case 0x03:
                    reverse = 0x8310; break;
                case 0x10:
                    reverse = 0x9010; break;
            }
            memcpy(tmp, &reverse, 2);
            tmp += 2;
            length += 1;
        }
        else
        {
            memcpy(tmp, &frame[i], 1);
            tmp += 1;
        }
    }
    memcpy(tmp, &frame[frame_len - 1], 1);
    memcpy(frame, buff, length);

    return length;
}

int pack_frame(unsigned char type, unsigned char *buff, int count = 0, bool reasonable = true)
{
    static int switch_count = 0;
    switch_count++;
    switch_count %= 4050;

    static unsigned short switch_change_count = 3050;
    switch_change_count++;
    switch_change_count %= 4060;

    // if ( count > 10 )
    //     return -1;
    // if ( reasonable )
    // {
    //     // 合法最长数据
    //     switch_change_count = 0x0fbf;
    // }
    // else
    // {
    //     //不合法最短数据
    //     switch_change_count = 0x0fc0;
    // }
    unsigned char frame_head = 0x01;
    unsigned short datalen = 0;
    unsigned short crc = 0;
    unsigned char frame_tail = 0x03;

    unsigned char *tmp = buff;

    if(type == 0x11)
    {
        //帧头
        memcpy(tmp, &frame_head, sizeof(frame_head));
        tmp += 1;

        //帧类型
        memcpy(tmp, &type, sizeof(type));
        tmp += 1;

        //数据段长度
        datalen = switch_count % 8 > 0 ? switch_count/8 + 1 : switch_count/8;
        memcpy(tmp, &datalen, sizeof(datalen));
        tmp += 2;

        //数据段
        unsigned char data_buff[datalen] = {0};
        unsigned short value = 0x0101;
        for( int i = 0; i < datalen; ++i )
        {
            data_buff[i] = value++;
        }
        memcpy(tmp, &data_buff, sizeof(data_buff));
        tmp += datalen;

        //CRC校验
        crc = calc_crc(buff+1, datalen+3);
        unsigned short r_crc = htons(crc);
        memcpy(tmp, &r_crc, sizeof(r_crc));
        tmp += 2;

        //帧尾
        memcpy(tmp, &frame_tail, sizeof(frame_tail));
    }
    else if(type == 0x12)
    {
        //帧头
        memcpy(tmp, &frame_head, sizeof(frame_head));
        tmp += 1;

        //帧类型
        memcpy(tmp, &type, sizeof(type));
        tmp += 1;

        //数据段长度
        datalen = switch_change_count * 2 + 2;
        memcpy(tmp, &datalen, sizeof(datalen));
        tmp += 2;

        //数据段
        memcpy(tmp, &switch_change_count, sizeof(switch_change_count));
        tmp += 2;
        unsigned short change_velue = 0xcaff;
        for( int i = 0; i < switch_change_count; ++i )
        {
            change_velue += 0x0f;
            memcpy(tmp, &change_velue, sizeof(change_velue));
            tmp += 2;
        }

        //CRC校验
        crc = calc_crc(buff+1, datalen+3);
        unsigned short r_crc = htons(crc);
        memcpy(tmp, &r_crc, sizeof(r_crc));
        tmp += 2;

        //帧尾
        memcpy(tmp, &frame_tail, sizeof(frame_tail));
    }
    else
        LogWarning("the other frame type\n");

    LogDebug("转义前：%d\n", datalen + 7);

    int frame_len = reverse_frame(buff, datalen + 7);

    LogDebug("转义后：%d\n", frame_len);

    return frame_len;
}

void *send_data_thread_fn(void *args)
{
    struct Param *arguments = (struct Param *)args;
    int cfd = arguments->fd;
    unsigned char buff[g_frame_len] = {0};
    int count = 1000;

    while(count--)
    {
        LogDebug("count: %d\n", count);
        memset(buff, 0, sizeof(buff));

        //发送开关量信息数据帧
        int frame_len = pack_frame(0x11, buff, count);
        if ( frame_len > 0 )
        {
            int ret = send(cfd, buff, frame_len, 0);
            if( ret < 0)
            {
                LogWarning("connection interrupt: %s\n", inet_ntoa(arguments->addr.sin_addr));
                free(arguments);
                close(cfd);
                pthread_exit(NULL);
            }

            usleep(intervel_time);
        }

        //发送开关量变化信息数据帧（合法长度）
        int frame_len_2 = pack_frame(0x12, buff, count);
        if ( frame_len_2 > 0 )
        {
            int ret_2 = send(cfd, buff, frame_len_2, 0);
            if(ret_2 < 0)
            {
                LogWarning("connection interrupt: %s\n", inet_ntoa(arguments->addr.sin_addr));
                free(arguments);
                close(cfd);
                pthread_exit(NULL);
            }

            usleep(intervel_time);
        }
    }
    close(cfd);

    return NULL;
}

int string_to_hex(unsigned char *buff)
{
    int datalen = 0;
    int i = 0;
    while(buff[i] != '\0' && buff[i] != '\n')
    {
        unsigned char tmp = '\0';
        for( int j = i; j < i + 2; ++j )
        {
            if ( buff[j] <= '9' )
            {
                tmp |= buff[j] - '0';
            }
            if ( buff[j] >= 'a' )
            {
                tmp |= buff[j] - 'a' + 10;
            }
            if ( j % 2 == 0 )
                tmp <<= 4;
        }
        memcpy(&buff[datalen], &tmp, 1);
        datalen++;
        i += 2;
    }
    return datalen;
}

void *send_data_from_file_thread_fn(void *args)
{
    struct Param *arguments = (struct Param *)args;
    int cfd = arguments->fd;
    unsigned char buff[g_frame_len*2+1] = {0};

    DIR *dir = opendir(g_data_dir);
    struct dirent *file = readdir(dir);
    while( file != NULL )
    {
        if ( strcmp( file->d_name, "." ) != 0 && strcmp( file->d_name, ".." ) != 0 )
        {
            char filename[125] = {0};
            strcat(filename, g_data_dir);
            strcat(filename, file->d_name);
            ifstream datafile(filename);

            int count = 1;
            while(datafile.getline((char *)buff, sizeof(buff)))
            {
                LogDebug("count：%d\n", count++);
                memset(buff, 0, sizeof(buff));
                datafile.getline((char *)buff, sizeof(buff));

                int datalen = string_to_hex(buff);
                LogDebug("转义前：%d\n", datalen);

                int frame_len = reverse_frame(buff, datalen);
                LogDebug("转义后：%d 0x%.2x\n", frame_len, buff[1]);
                if ( frame_len > 0 )
                {
                    int ret = send(cfd, buff, frame_len, 0);
                    if( ret < 0)
                    {
                        LogWarning("connection interrupt: %s\n", inet_ntoa(arguments->addr.sin_addr));
                        free(arguments);
                        close(cfd);
                        pthread_exit(NULL);
                    }

                    usleep(intervel_time);
                }
            }
            datafile.close();
            LogDebug("send [%s] complete\n", file->d_name);
        }
        file = readdir(dir);
    }

    closedir(dir);
    free(arguments);
    close(cfd);

    return NULL;
}

int main()
{
	signal(SIGPIPE,SIG_IGN);

    log_init("DEBUG", "DUAL", "./log/");

	int sfd = socket(AF_INET, SOCK_STREAM, 0);

	int on = 1;	
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	struct sockaddr_in seraddr={0};
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(g_monitor_port);
	seraddr.sin_addr.s_addr = inet_addr(g_monitor_ip);

	socklen_t size = sizeof(struct sockaddr_in);

    if( bind(sfd, (struct sockaddr *)&seraddr, size) < 0 )
    {
        LogError("bind fail\n");
        return -1;
    }

    LogDebug("listening at %s:%d\n", g_monitor_ip, g_monitor_port);
	listen(sfd,10);
	
    struct Param *arguments = (struct Param *)malloc(sizeof(struct Param));
    memset(arguments, 0, sizeof(struct Param));

    arguments->fd = accept(sfd, (struct sockaddr *)&arguments->addr, &size);
    LogDebug("accept a connection from: %s\n", inet_ntoa(arguments->addr.sin_addr));

    sleep(2);

    // send_data_thread_fn((void *)arguments);

    send_data_from_file_thread_fn((void *)arguments);

    close(sfd);

	return 0;
}

