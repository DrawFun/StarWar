#ifndef _STARWAR_FPS_H_
#define _STARWAR_FPS_H_

#include <stdio.h>
#include "Util.h"
#include "mmsystem.h"

class CFPS
{
public:
    CFPS(void);
    ~CFPS(void);
public:
    void Start();     // ��Ϸ��ʱ������
    void Tick();      // ��Ϸ��ʱ������(ÿ��Updateʱ����һ��)
    void Stop();      // ��Ϸ��ʱ��ֹͣ
public:
    float GetTotalTicks()     {return m_totalTicks;}          // ���ϵͳ����ʱ��
    float GetTotalGameTime()  {return m_totalGameTime;}       // �����Ϸ����ʱ��
    float GetElapsedGameTime(){return m_elapsedGameTime;}     // ��õ�֡ʱ���
public:
    void  CalcFPS();                                          // ����FPS(ÿ��Updateʱ����һ��)(����ʽ���ã�Ĭ�ϲ�ִ��)
    char* ShowFPS()           {return m_strFPS;}              // ��ʾFPS(�ַ���)
    float GetFPS()            {return m_FPS;}                 // ���FPS(float��ֵ)
private:
    float m_totalTicks;          // ϵͳ����ʱ��
    float m_totalGameTime;       // ��Ϸ����ʱ��
    float m_elapsedGameTime;     // ��֡����ʱ��
    float m_previousTicks;       // ǰ��ʱ���
    float m_startTicks;          // ����ʱ���
    long m_FrameCnt;            // ֡����(����FPS��������)
    float m_TimeElapsed;         // ����ʱ��(����FPS��������)
    float m_FPS;                 // FPSֵ
    char  m_strFPS[13];          // FPS��
};

#endif