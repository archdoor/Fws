#ifndef __DATAFILE_H__
#define __DATAFILE_H__

#include "btnframe.h"

// 数据文件最大长度
const int g_data_file_size = 20*1024*1024;

// 数据帧文件
class DataFile
{
public:
    DataFile();

    int create_file_by_time(struct timeval msg_time);
    int save_to_file(BtnFrame &btnframe);


    int m_file;
};


#endif

