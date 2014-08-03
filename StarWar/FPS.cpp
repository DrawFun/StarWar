#include "FPS.h"

CFPS::CFPS(void) : m_totalTicks(0.0f),
                             m_totalGameTime(0.0f),
                             m_elapsedGameTime(0.0f),
                             m_previousTicks(0.0f),
                             m_startTicks(0.0f),
                             m_FrameCnt(0),
                             m_TimeElapsed(0.0f),
                             m_FPS(0.0f)
{
    // 初始FPS串
    char* strFPS = "FPS:0000000\0";
    memcpy(m_strFPS,strFPS,13);
}

CFPS::~CFPS(void)
{
}

void CFPS::Start()
{
    // 计时器启动时先捕捉起始时间戳及前次时间戳
    m_startTicks = (float)timeGetTime();
    m_previousTicks = m_startTicks;
}

void CFPS::Tick()
{
    // 每次心跳时更新系统时间戳
    m_totalTicks = (float)timeGetTime();
    // 单帧时间差 = 系统时间戳 - 前次时间戳
    m_elapsedGameTime = m_totalTicks - m_previousTicks;
    // 游戏运作时间 = 系统时间戳 - 起始时间戳
    m_totalGameTime = m_totalTicks - m_startTicks;
    // 更新前次时间戳
    m_previousTicks = m_totalTicks;
}

void CFPS::Stop()
{
    // 计时器停止时全部变量归0
    m_totalTicks = 0.0f;
    m_totalGameTime = 0.0f;
    m_elapsedGameTime = 0.0f;
    m_previousTicks = 0.0f;
    m_startTicks = 0.0f;
    m_FrameCnt = 0;
    m_TimeElapsed = 0.0f;
    m_FPS = 0.0f;
}

void CFPS::CalcFPS()
{
    // 帧数递增
    m_FrameCnt++;
    // 累计时间递增
    m_TimeElapsed += m_elapsedGameTime;
    // 当累计时间超过1秒时
    if(m_TimeElapsed >= 1000.0f)
    {
        // FPS测算
        m_FPS = (float)m_FrameCnt * 1000.0f / m_TimeElapsed;
        sprintf(&m_strFPS[4], "%f", m_FPS);
        m_strFPS[12] = '\0';
        // 处理累计时间及帧数
        m_TimeElapsed -= 1000.0f;
        m_FrameCnt    = 0;
    }
}