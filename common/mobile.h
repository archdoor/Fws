#ifndef __MOBILE_H__
#define __MOBILE_H__

// 手持端类
class Mobile 
{
public:
    Mobile();

    unsigned char m_IMEI[20];
    int m_MSISDN;
    unsigned char m_IMSI[20];
    unsigned char m_type[64];
};



#endif


