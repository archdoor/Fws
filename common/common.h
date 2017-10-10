#ifndef __COMMON_H__
#define __COMMON_H__

// 模块编号
enum _E_ModuleId
{
    // 用户信息处理模块
    E_USERS_HANDLE_MODULE = 0x01,
    // 集中检测系统接口模块
    E_MONITOR_INTERFACE_MODULE = 0x02,
    // 安全处理模块
    E_SAFETY_MODULE = 0x03,
    // 客户端连接模块
    E_CLIENT_LINK_MODULE = 0x04,
    // 定位分配服务器模块
    E_ALLOC_LOCALTION_MODULE = 0x11,
    // 手持端
    E_EQUIP_MODULE = 0x12,
    // 维护终端
    E_MAINTAIN_TERMINAL_MODULE = 0x13,
    // 转发代理用户端
    E_PROXY_USER_MODULE = 0x80,
    // 转发代理平台端
    E_PROXY_PLATFORM_MODULE = 0x81,
};

// 用户位置精度类型
enum _E_PrecisionType
{
    // GPS差分修正数据
    E_GPS_DIFF_CORRECT = 0X00,
    // GPS普通数据
    E_GPS_COMMON = 0X01,
};

// 帧类型
enum _E_FrameType
{
    // 用户信息帧
    E_USER_FRAME = 0X01,
    // 手持端状态信息帧
    E_EQUIP_STATUS_FRAME = 0X02,
    // 车站信息帧
    E_STATION_FRAME = 0X03,
    // 告警信息帧
    E_WARN_FRAME = 0X04,
    // 信息接收确认帧
    E_RECV_ACK_FRAME = 0X0F,
    // 用户注册帧
    E_REG_FRAME = 0X10,
    // 用户注册应答帧
    E_REG_ANSWER_FRAME = 0X11,
    // 用户注销帧
    E_LOGOUT_FRAME = 0X12,
    // 用户注销应答帧
    E_LOGOUT_ANSWER_FRAME = 0X13,
    // 区段状态信息帧
    E_SECTION_STATUS_FRAME = 0X20,
    // 配置更新帧
    E_CONFIG_UPDATE_FRAME = 0X30,
    // 当前全部用户信息帧
    E_CUR_ALL_USER_FRAME = 0X31,
    // 当前全部区段信息帧
    E_CUR_ALL_SECTION_FRAME = 0X32,
    // 用户编组帧
    E_USER_GROUP_FRAME = 0X33,

    // 全体开关量信息帧
    E_ALL_BTN_FRAME = 0X11,
    // 变化开关量信息帧
    E_CNG_BTN_FRAME = 0X12,
    // 预留
    E_RESERVE_FRAME_1 = 0X13,
    // 预留
    E_RESERVE_FRAME_2 = 0X14,
    // 预留
    E_RESERVE_FRAME_3 = 0X15,
};

// 帧长度(-1为变长帧)
enum _E_FrameLen
{
    E_USER_FRAME_LEN = 55,
    E_EQUIP_STATUS_FRAME_LEN = 19,
    E_STATION_FRAME_LEN = 52,
    E_WARN_FRAME_LEN = -1,
    E_RECV_ACK_FRAME_LEN = 27,
    E_REG_FRAME_LEN = 49,
    E_REG_ANSWER_FRAME_LEN = 17,
    E_LOGOUT_FRAME_LEN = 16,
    E_LOGOUT_ANSWER_FRAME_LEN = 17,
    E_SECTION_STATUS_FRAME_LEN = 14,
    E_CONFIG_UPDATE_FRAME_LEN = -1,
    E_CUR_ALL_USER_FRAME_LEN = 6,
    E_CUR_ALL_SECTION_FRAME_LEN = 6,
    E_USER_GROUP_FRAME_LEN = -1,
};

// 注册结果
enum _E_RegResult
{
    E_REG_SUCCESS = 0X00,
    E_REG_FAIL_AUTH = 0X01,
    E_REG_EQUIP_FORBIT = 0X02,
};

#endif


