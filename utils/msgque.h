#ifndef __MSGQUE_H__
#define __MSGQUE_H__

#include <sys/time.h>

//消息队列中数据最大长度
const int g_msg_max_len = 8*1024 - sizeof(long) - sizeof(struct timeval);

//消息类型
const long g_msgtype = 100;

//消息队列中的消息结构
struct Mess
{
    long type;
    struct timeval time;
    unsigned char text[g_msg_max_len];
}__attribute__ ((packed));


class MsgQue
{
public:
    MsgQue();
    int init();
    int create_msgqueue();
    int send(int len);
    int recv();

    static int m_msgid;
    Mess m_mess;
};




#endif
