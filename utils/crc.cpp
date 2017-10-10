#include <arpa/inet.h>
#include "crc.h"

// 计算CRC校验码：多项式 0x11021 X^16+X^12+X^5+1
unsigned short calc_crc(unsigned char *data, int len)
{
    if( len <= 0 )
        return 0;

    unsigned short crc = 0;
    unsigned char byte;
    unsigned short byte_first_bit;
    unsigned short crc_first_bit;

    for( int i = 0; i < len; i++ )
    {
        byte = data[i];
        for( int j = 0; j < 8; j++ )
        {
            byte_first_bit = 0;
            crc_first_bit = 0;

            if ( byte & 0x80 ) {
                byte_first_bit = 1;
            }
            byte <<= 1;

            if ( crc & 0x8000 ) {
                crc_first_bit = 1;
            }
            crc <<= 1;

            if( byte_first_bit + crc_first_bit == 1 ) {
                crc ^= 0x1021;
            }
        }
    }
    return crc;
}

// CRC校验(校验码小端存储)
int crc_check_little(unsigned char *data, int len, unsigned short *raw_crc)
{
    unsigned short crc = calc_crc(data, len);
    if ( crc != *raw_crc )
    {
        return -1;
    }

    return 0;
}

// CRC校验(校验码大端存储)
int crc_check_big(unsigned char *data, int len, unsigned short *raw_crc)
{
    unsigned short crc = calc_crc(data, len);
    if ( htons(crc) != *raw_crc )
    {
        return -1;
    }

    return 0;
}
