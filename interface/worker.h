#ifndef __WORKER_H__
#define __WORKER_H__

const int g_interface_worker_buff_size = 10*1024;

class Worker
{
public:
    Worker();
    int handle(unsigned char *frame, int len);
    int handle_all_btn_frame(unsigned char *data, unsigned short datalen);
    int handle_cng_btn_frame(unsigned char *data, unsigned short datalen);

    unsigned char m_buff[g_interface_worker_buff_size];
};

#endif

