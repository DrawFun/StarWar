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
    // ��ʼFPS��
    char* strFPS = "FPS:0000000\0";
    memcpy(m_strFPS,strFPS,13);
}

CFPS::~CFPS(void)
{
}

void CFPS::Start()
{
    // ��ʱ������ʱ�Ȳ�׽��ʼʱ�����ǰ��ʱ���
    m_startTicks = (float)timeGetTime();
    m_previousTicks = m_startTicks;
}

void CFPS::Tick()
{
    // ÿ������ʱ����ϵͳʱ���
    m_totalTicks = (float)timeGetTime();
    // ��֡ʱ��� = ϵͳʱ��� - ǰ��ʱ���
    m_elapsedGameTime = m_totalTicks - m_previousTicks;
    // ��Ϸ����ʱ�� = ϵͳʱ��� - ��ʼʱ���
    m_totalGameTime = m_totalTicks - m_startTicks;
    // ����ǰ��ʱ���
    m_previousTicks = m_totalTicks;
}

void CFPS::Stop()
{
    // ��ʱ��ֹͣʱȫ��������0
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
    // ֡������
    m_FrameCnt++;
    // �ۼ�ʱ�����
    m_TimeElapsed += m_elapsedGameTime;
    // ���ۼ�ʱ�䳬��1��ʱ
    if(m_TimeElapsed >= 1000.0f)
    {
        // FPS����
        m_FPS = (float)m_FrameCnt * 1000.0f / m_TimeElapsed;
        sprintf(&m_strFPS[4], "%f", m_FPS);
        m_strFPS[12] = '\0';
        // �����ۼ�ʱ�估֡��
        m_TimeElapsed -= 1000.0f;
        m_FrameCnt    = 0;
    }
}