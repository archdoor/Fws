#ifndef __BTN_FRAME_H__
#define __BTN_FRAME_H__

#include "msgque.h"

// 开关量信息帧
class BtnFrame
{
public:
    BtnFrame();
    int push_data(unsigned char *data, int len);
    int frame_clean();

    int get_frame();
    int del_reverse();

    int framelen();
    int datalen();


    unsigned char *m_buff;
    unsigned char *m_data;
    int m_framelen;
    unsigned short m_datalen;
    int m_head_flag;
    unsigned char m_type;
    MsgQue m_msgque;

    static unsigned char m_head;
    static unsigned char m_tail;
};






#endif

