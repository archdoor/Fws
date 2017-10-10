#include <string.h>
#include "frames.h"
#include "common.h"
#include "crc.h"

// 帧基类
BaseFrame::BaseFrame()
{
    m_buff = NULL;
    m_start_1 = 0xff;
    m_start_2 = 0x7e;
    m_framelen = 0;
    m_type = 0;
}

// 帧格式检查
int BaseFrame::frame_format_check()
{
    if ( m_buff == NULL )
        return -1;

    if ( (m_buff[0] != m_start_1) || (m_buff[1] != m_start_2) )
        return -1;

    return 0;
}

// 用户位置信息帧
UserFrame::UserFrame()
{
    m_buff = NULL;
    m_type = E_USER_FRAME;
    m_framelen = E_USER_FRAME_LEN;
}

// 手持端状态信息帧
EquipStatusFrame::EquipStatusFrame()
{
    m_buff = NULL;
    m_type = E_EQUIP_STATUS_FRAME;
    m_framelen = E_EQUIP_STATUS_FRAME_LEN;
}

// 车站信息帧
StationFrame::StationFrame()
{
    m_buff = NULL;
    m_type = E_STATION_FRAME;
    m_framelen = E_STATION_FRAME_LEN;
}

// 告警信息帧
WarnFrame::WarnFrame()
{
    m_buff = NULL;
    m_type = E_WARN_FRAME;
    m_framelen = E_WARN_FRAME_LEN;
}

// 信息接收确认帧
RecvAckFrame::RecvAckFrame()
{
    m_buff = NULL;
    m_type = E_RECV_ACK_FRAME;
    m_framelen = E_RECV_ACK_FRAME_LEN;
}

// 用户注册帧
RegFrame::RegFrame()
{
    m_buff = NULL;
    m_type = E_REG_FRAME;
    m_framelen = E_REG_FRAME_LEN;
}

// 帧格式检查
int RegFrame::frame_format_check()
{

    return 0;
}

// 用户注册应答帧
RegAnswerFrame::RegAnswerFrame(unsigned char *buff)
{
    m_buff = buff;
    m_type = E_REG_ANSWER_FRAME;
    m_framelen = E_REG_ANSWER_FRAME_LEN;
}

// 用户注销帧
LogoutFrame::LogoutFrame()
{
    m_buff = NULL;
    m_type = E_LOGOUT_FRAME;
    m_framelen = E_LOGOUT_FRAME_LEN;
}

// 用户注销应答帧
LogoutAnswerFrame::LogoutAnswerFrame()
{
    m_buff = NULL;
    m_type = E_LOGOUT_ANSWER_FRAME;
    m_framelen = E_LOGOUT_ANSWER_FRAME_LEN;
}

// 区段状态信息帧
SectionStatusFrame::SectionStatusFrame()
{
    m_buff = NULL;
    m_type = E_SECTION_STATUS_FRAME;
    m_framelen = E_SECTION_STATUS_FRAME_LEN;
}

// 配置更新帧
ConfigUpdateFrame::ConfigUpdateFrame()
{
    m_buff = NULL;
    m_type = E_CONFIG_UPDATE_FRAME;
    m_framelen = E_CONFIG_UPDATE_FRAME_LEN;
}

// 当前全部用户信息帧
CurAllUserFrame::CurAllUserFrame()
{
    m_buff = NULL;
    m_type = E_CUR_ALL_USER_FRAME;
    m_framelen = E_CUR_ALL_USER_FRAME_LEN;
}

// 当前全部区段信息帧
CurAllSectionFrame::CurAllSectionFrame()
{
    m_buff = NULL;
    m_type = E_CUR_ALL_SECTION_FRAME;
    m_framelen = E_CUR_ALL_SECTION_FRAME_LEN;
}

// 用户编组帧
UserGroupFrame::UserGroupFrame()
{
    m_buff = NULL;
    m_type = E_USER_GROUP_FRAME;
    m_framelen = E_USER_GROUP_FRAME_LEN;
}

// 获取信息接收确认帧
int get_recv_ack_frame(unsigned char *buff, int len, unsigned char *module, unsigned int *station, unsigned char *IMEI)
{
    if ( len < E_RECV_ACK_FRAME_LEN )
    {
        return -1;
    }

    unsigned char start_1 = 0xff;
    unsigned char start_2 = 0x7e;
    unsigned short framelen = E_RECV_ACK_FRAME_LEN;
    unsigned char frame_type = E_RECV_ACK_FRAME;
    unsigned short crc = 0;

    unsigned char *tmp = buff;

    memcpy(tmp++, &start_1, sizeof(start_1));
    memcpy(tmp++, &start_2, sizeof(start_2));
    memcpy(tmp, &framelen, sizeof(framelen));
    tmp += sizeof(framelen);
    memcpy(tmp++, &frame_type, sizeof(frame_type));
    memcpy(tmp++, module, 1);
    memcpy(tmp, station, 4);
    tmp += 4;
    memcpy(tmp, IMEI, 15);
    tmp += 15;
    crc = calc_crc(buff, tmp - buff);
    memcpy(tmp, &crc, sizeof(crc));
    tmp += sizeof(crc);

    if ( (tmp - buff) != E_RECV_ACK_FRAME_LEN )
    {
        return -1;
    }

    return E_RECV_ACK_FRAME_LEN;
}

// 获取区段状态信息帧
int get_seg_status_frame()
{
    return 0;
}
