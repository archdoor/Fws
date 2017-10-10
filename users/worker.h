#ifndef __WORK_H__
#define __WORK_H__

class Worker
{
public:
    Worker();
    int handle(unsigned char *frame, int len);
    int userframe_handle(unsigned char *frame, int len);

};

#endif

