#ifndef __SUBSCRIBER_H__
#define __SUBSCRIBER_H__

// 用户类
class Subscriber
{
public:
    Subscriber();

    unsigned char m_loginname[64];
    unsigned char m_pwd[64];
    unsigned char m_realname[64];
};



#endif

