#include <unistd.h>
#include "worker.h"
#include "interact.h"
#include "frames.h"
#include "common.h"
#include "crc.h"
#include "log.h"

Worker::Worker()
{

}

int Worker::handle(unsigned char *frame, int len)
{
    LogDebug("开始业务处理\n");
    unsigned char type = frame[4];

    switch(type)
    {
        case E_REG_FRAME:
            LogWarning("用户注册信息，暂未处理\n");
            break;

        case E_LOGOUT_FRAME:
            LogWarning("用户注销信息，暂未处理\n");
            break;

        case E_USER_FRAME:
            LogDebug("用户信息帧处理\n");
            userframe_handle(frame, len);
            break;

        default:
            LogWarning("Unknown frame type: %.2x\n", type);
            break;

    }


    return 0;
}

// 用户信息帧处理
int Worker::userframe_handle(unsigned char *frame, int len)
{
    if ( frame == NULL || len != E_USER_FRAME_LEN )
    {
        LogWarning("user frame wrong\n");
        return -1;
    }

    // 检查crc校验
    unsigned short *crc = (unsigned short *)(frame + E_USER_FRAME_LEN - 2);
    if ( crc_check_little(frame, E_USER_FRAME_LEN - 2, crc) < 0 )
    {
        LogWarning("crc_check_little error\n");
        return -1;
    }

    // 发送信息接收确认帧
    unsigned char buff[E_RECV_ACK_FRAME_LEN + 1] = {0};
    unsigned char *module = (unsigned char *)(frame + 5);
    unsigned int *station = (unsigned int *)(frame + 6);
    unsigned char *IMEI = (unsigned char *)(frame + 10);
    int ret = get_recv_ack_frame(buff, sizeof(buff), module, station, IMEI);
    if ( ret < 0 )
    {
        LogWarning("get_recv_ack_frame error\n");
        return -1;
    }
    Interactor *actor = Interactor::get_instance();
    if ( actor->send_module_proxy(buff, ret) < 0 )
    {
        LogWarning("send_module_proxy fail\n");
    }

    // 转发至安全管理模块
    if ( actor->send_to_module_safety(frame, len) < 0 )
    {
        LogWarning("send_to_module_safety fail\n");
    }

    // 转发至客户端连接模块
    if ( actor->send_to_module_clinker(frame, len) < 0 )
    {
        LogWarning("send_to_module_clinker fail\n");
    }

    return 0;
}


