#ifndef __FRAMES_H__
#define __FRAMES_H__

// 信息帧允许的最大长度
const unsigned short g_frames_max_size = 3*1024;

// 帧基类
class BaseFrame
{
public:
    BaseFrame();

    int frame_format_check();

    unsigned char *m_buff;

    // 帧起始
    unsigned char m_start_1;
    unsigned char m_start_2;
    // 帧长度
    unsigned short m_framelen;
    // 帧类型
    unsigned char m_type;
};

// 用户信息帧
class UserFrame : BaseFrame
{
public:
    UserFrame();

};

// 手持端状态信息帧
class EquipStatusFrame : BaseFrame
{
public:
    EquipStatusFrame();

};

// 车站信息帧
class StationFrame : BaseFrame
{
public:
    StationFrame();

};

// 告警信息帧
class WarnFrame : BaseFrame
{
public:
    WarnFrame();

};

// 信息接收确认帧
class RecvAckFrame : BaseFrame
{
public:
    RecvAckFrame();

};

// 用户注册帧
class RegFrame : BaseFrame
{
public:
    RegFrame();
    int frame_format_check();

};

// 用户注册应答帧
class RegAnswerFrame : BaseFrame
{
public:
    RegAnswerFrame(unsigned char *buff);

};

// 用户注销帧
class LogoutFrame : BaseFrame
{
public:
    LogoutFrame();

};

// 用户注销应答帧
class LogoutAnswerFrame : BaseFrame
{
public:
    LogoutAnswerFrame();

};

// 区段状态信息帧
class SectionStatusFrame : BaseFrame
{
public:
    SectionStatusFrame();

};

// 配置更新帧
class ConfigUpdateFrame : BaseFrame
{
public:
    ConfigUpdateFrame();

};

// 当前全部用户信息帧
class CurAllUserFrame : BaseFrame
{
public:
    CurAllUserFrame();

};

// 当前全部区段信息帧
class CurAllSectionFrame : BaseFrame
{
public:
    CurAllSectionFrame();

};

// 用户编组帧
class UserGroupFrame : BaseFrame
{
public:
    UserGroupFrame();

};

// 获取信息接收确认帧
int get_recv_ack_frame(unsigned char *buff, int len, unsigned char *module, unsigned int *station, unsigned char *IMEI);
// 获取区段状态信息帧
int get_seg_status_frame();

#endif

