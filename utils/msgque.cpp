#include <iostream>
#include <sys/msg.h>
#include "msgque.h"

int MsgQue::m_msgid = -1;

MsgQue::MsgQue()
{
    m_mess.type = g_msgtype;

    init();
}

int MsgQue::init()
{
    if ( m_msgid < 0 )
    {
        create_msgqueue();
    }

    return 0;
}

int MsgQue::create_msgqueue()
{
    key_t msgkey = ftok(__FILE__, 200);
    m_msgid = msgget(msgkey, IPC_CREAT | 0644);
    if( m_msgid < 0 )
    {
        return -1;
    }

    return 0;
}

int MsgQue::send(int len)
{
    gettimeofday(&m_mess.time, NULL);
    if( msgsnd(m_msgid, &m_mess, len + sizeof(long) + sizeof(struct timeval), 0) != 0 ) 
    {
        return -1;
    }
    return 0;
}

int MsgQue::recv()
{
    int msglen = msgrcv(m_msgid, &m_mess, g_msg_max_len, g_msgtype, 0);
    if ( msglen < 0 )
    {
        return -1;
    }
    int datalen = msglen - sizeof(long) - sizeof(struct timeval);

    return datalen;
}

