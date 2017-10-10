#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "datafile.h"
#include "log.h"

// 数据文件路径
const char *g_data_file_path = "./data/";

DataFile::DataFile()
{
    // 创建数据存储文件
    struct timeval cur_time = {0};
    gettimeofday(&cur_time, NULL);
    if ( create_file_by_time(cur_time) < 0 )
    {
        LogWarning("create_data_file fail!\n");
    }
}

// 创建数据文件
int DataFile::create_file_by_time(struct timeval msg_time)
{
    char filename[32] = {0};
    strftime(filename, sizeof(filename), "%Y-%m-%d_%H-%M-%S", localtime(&msg_time.tv_sec));

    char pathname[64] = {0};
    strcat(pathname, g_data_file_path);
    strcat(pathname, filename);
    strcat(pathname, ".data");

    m_file = open(pathname, O_CREAT | O_WRONLY | O_SYNC, 0766);
    if( m_file < 0 )
    {
        LogError("open data file failed!\n");
        return -1;
    }

    return 0;
}

// 保存数据到文件
int DataFile::save_to_file(BtnFrame &btnframe)
{
    if ( m_file < 0 )
    {
        LogWarning("data file not opened!\n");
        return -1;
    }

    // 消息时间
    char sdate[25] = {0};
    strftime(sdate, sizeof(sdate), "%Y-%m-%d_%H:%M:%S", localtime(&btnframe.m_msgque.m_mess.time.tv_sec));
    strcat(sdate, "\n");

    // 数据转化为十六进制字符
    char hex_buff[btnframe.framelen()*2 + 1] = {0};
    char *tmp = hex_buff;
    for( int i = 0; i < btnframe.framelen(); ++i )
    {
        sprintf(tmp, "%.2x", btnframe.m_buff[i]);
        tmp += 2;
    }
    memcpy(tmp, "\n", strlen("\n"));

    // 时间和数据写入文件
    write(m_file, sdate, strlen(sdate));
    write(m_file, hex_buff, sizeof(hex_buff));

    // 判断文件大小，过大则新建文件
    struct stat filemsg = {0};
    fstat(m_file, &filemsg);
    if( filemsg.st_size > g_data_file_size )
    {
        close(m_file);
        LogDebug("create new data file\n");

        create_file_by_time(btnframe.m_msgque.m_mess.time);
    }
    return 0;
}
