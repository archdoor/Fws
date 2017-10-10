#include <string.h>
#include <arpa/inet.h>
#include "btnframe.h"
#include "crc.h"
#include "log.h"

// 内存搜索
int mem_search(unsigned char *membuff, int len, unsigned char byte)
{
    for( int i = 0; i < len ; ++i )
    {
        if ( membuff[i] == byte )
            return ( i + 1 );
    }
    return 0;
}

unsigned char BtnFrame::m_head = 0x01;
unsigned char BtnFrame::m_tail = 0x03;

BtnFrame::BtnFrame()
{
    m_framelen = 0;
    m_framelen = 0;
    m_head_flag = 0;
    m_type = 0x00;

    m_buff = m_msgque.m_mess.text;
}

// 把接收数据存入缓存，若有完整帧，则发送到消息队列
int BtnFrame::push_data(unsigned char *data, int len)
{
    if ( data == NULL || len <= 0)
    {
        LogWarning("parame error\n");
        return -1;
    }

    if ( m_head_flag == 0 )
    {
        // 定位到帧头
        int head = mem_search( data, len, m_head );
        if ( head == 0 ) {
            return -1;
        }
        else
        {
            if ( head > 1 ) {
                LogDebug("skip to find frame head\n");
            }
            m_head_flag = 1;
            push_data(data + head - 1, len - head + 1);
        }
    }
    else
    {
        // 查找帧尾
        int tail = mem_search(data, len, m_tail);
        if ( tail == 0 )
        {
            if ( m_framelen + len > g_msg_max_len )
            {
                LogWarning("btnframe too long: %d\n", m_framelen + len);
                frame_clean();
                return -1;
            }
            LogDebug("push_data: %d\n", len);
            memcpy(m_buff + m_framelen, data, len);
            m_framelen += len;
        }
        else
        {
            if ( m_framelen + tail > g_msg_max_len ) {
                LogWarning("btnframe too long: %d\n", m_framelen + tail);
            }
            else
            {
                LogDebug("push_data: %d\n", tail);
                memcpy(m_buff + m_framelen, data, tail);
                m_framelen += tail;

                m_msgque.send(m_framelen);
            }
            m_framelen = 0;
            m_head_flag = 0;
            if ( tail != len )
            {
                LogDebug("left data len：%d\n", len - tail);
                push_data(data + tail, len - tail);
            }
        }
    }
    return 0;
}

int BtnFrame::get_frame()
{
    m_framelen = m_msgque.recv();
    if ( m_framelen < 0 )
    {
        return -1;
    }
    if ( m_buff[m_framelen - 1] != 0x03 )
    {
        LogError("tail is not right\n");
    }

    m_framelen = del_reverse();
    if ( m_framelen < 0 )
    {
        return -1;
    }
    LogDebug("del reverse data len: %d\n", m_framelen);
    if ( m_buff[m_framelen - 1] != 0x03 )
    {
        LogError("tail is not right\n");
    }

    // 数据校验
    if ( crc_check_big(m_buff + 1, m_framelen - 4, (unsigned short *)(m_buff + m_framelen - 3)) < 0 )
    {
        LogWarning("CRC校验失败\n");
        return -1;
    }

    m_type = m_buff[1];
    m_data = m_buff + 4;
    m_datalen = *(unsigned short *)(m_buff + 2);

    return 0;
}

// 清空数据帧
int BtnFrame::frame_clean()
{
    m_framelen = 0;
    m_datalen = 0;
    m_head_flag = 0;

    return 0;
}

int BtnFrame::del_reverse()
{
    unsigned char *pwrite = m_buff + 1;
    unsigned char raw = 0x00;

    int i = 0;
    int len = m_framelen;

    for( i = 1; i < len - 2; )
    {
        unsigned short *comp = (unsigned short *)(m_buff + i);
        if( (*comp ==  0x8110) || (*comp ==  0x8310) || (*comp ==  0x9010) )
        {
            m_framelen--;
            i += 2;
            switch( *comp )
            {
                case 0x8110:
                    raw = 0x01; break;
                case 0x8310:
                    raw = 0x03; break;
                case 0x9010:
                    raw = 0x10; break;
            }
        }
        else {
            raw = m_buff[i];
            i++;
        }

        memcpy(pwrite, &raw, 1);
        pwrite++;
    }
    memcpy(pwrite, m_buff + i, len - i);

    return m_framelen;
}

int BtnFrame::framelen()
{ 
    return m_framelen; 
}

int BtnFrame::datalen()
{ 
    return m_datalen; 
}


