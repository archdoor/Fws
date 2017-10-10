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


    return 0;
}


