#include <unistd.h>
#include "worker.h"
#include "interact.h"
#include "frames.h"
#include "common.h"
#include "crc.h"
#include "log.h"

int id_define(unsigned short btn_id)
{

    if ( 1 > btn_id || btn_id > 3266 )
    {
        LogDebug("非法开关量：%d\n", btn_id);
    }
    else if ( 617 <= btn_id && btn_id <= 778 )
    {
        LogDebug("轨道区段：%d\n", btn_id);
    }
    else if ( 1071 <= btn_id && btn_id <= 1095 )
    {
        LogDebug("运行方向：%d\n", btn_id);
    }
    else if ( 1450 <= btn_id && btn_id <= 1548 )
    {
        LogDebug("按钮开关：%d\n", btn_id);
    }
    else
    {
        LogDebug("码位表：%d\n", btn_id);
    }

    return 0;
}

Worker::Worker()
{

}

int Worker::handle(unsigned char *frame, int len)
{
    unsigned char type = frame[1];
    unsigned char *data = frame + 4;
    unsigned short datalen = *(unsigned short *)(frame + 2);

    switch(type)
    {
        case E_ALL_BTN_FRAME:
            LogDebug("all botton status frame(%d/%d)\n", datalen, len - 7);
            // handle_all_btn_frame(data, datalen);
            break;

        case E_CNG_BTN_FRAME:
            LogDebug("changed botton status frame(%d/%d)\n", datalen, len - 7);
            handle_cng_btn_frame(data, datalen);
            break;

        case E_RESERVE_FRAME_1:
            LogDebug("reserved frame type 1\n");
            break;

        case E_RESERVE_FRAME_2:
            LogDebug("reserved frame type 2\n");
            break;

        case E_RESERVE_FRAME_3:
            LogDebug("reserved frame type 3\n");
            break;

        default:
            LogWarning("Unknown frame type: %.2x\n", type);
            break;
    }

    return 0;
}

// 全体开关量信息帧处理
int Worker::handle_all_btn_frame(unsigned char *data, unsigned short datalen)
{
    int btn_id = 0;
    unsigned char *byte = data;
    unsigned char check = 0x01;

    for( int i = 0; i < datalen; ++i )
    {
        for ( int j = 0; j < 8; ++j )
        {
            btn_id++;
            if ( ((*byte) & (check << j)) > 0)
            {
                LogDebug("开关量：%d (开)\n", btn_id);
            }
            else
            {
                LogDebug("开关量：%d (关)\n", btn_id);
            }
        }
    }

    return 0;
}

// 变化开关量信息帧处理
int Worker::handle_cng_btn_frame(unsigned char *data, unsigned short datalen)
{
    unsigned short btn_num = *(unsigned short *)(data);
    unsigned short btn_id = 0;
    int status = 0;

    LogDebug("btn_num: %d\n", btn_num);

    unsigned short *btn = (unsigned short *)(data + 2);
    for( int i = 0; i < btn_num; ++i )
    {
        btn_id = (*btn) & 0x7fff;
        status = (*btn) & 0x8000;

        if ( status == 0 )
        {
            LogDebug("变化开关量：%d（关）\n", btn_id);
        }
        else
        {
            LogDebug("变化开关量：%d（开）\n", btn_id);
        }
         
        id_define(btn_id);
    }

    // 获取区段状态信息表
    unsigned short framelen = 11 + 3*btn_num;
    unsigned char buff[framelen + 1] = {0};
    int ret = get_seg_status_frame();
    if ( ret < 0 )
    {
        LogWarning("get_seg_status_frame error\n");
        return -1;
    }

    Interactor *actor = Interactor::get_instance();

    // 转发至客户端连接模块
    if ( actor->send_to_module_clinker(buff, framelen) < 0 )
    {
        LogWarning("send_to_module_clinker fail\n");
    }

    // 转发至安全管理模块
    if ( actor->send_to_module_safety(buff, framelen) < 0 )
    {
        LogWarning("send_to_module_safety fail\n");
    }

    return 0;
}



