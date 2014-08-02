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
    void Start();     // 游戏计时器启动
    void Tick();      // 游戏计时器心跳(每次Update时调用一次)
    void Stop();      // 游戏计时器停止
public:
    float GetTotalTicks()     {return m_totalTicks;}          // 获得系统启动时间
    float GetTotalGameTime()  {return m_totalGameTime;}       // 获得游戏运作时间
    float GetElapsedGameTime(){return m_elapsedGameTime;}     // 获得单帧时间差
public:
    void  CalcFPS();                                          // 计算FPS(每次Update时调用一次)(需显式调用，默认不执行)
    char* ShowFPS()           {return m_strFPS;}              // 显示FPS(字符串)
    float GetFPS()            {return m_FPS;}                 // 获得FPS(float数值)
private:
    float m_totalTicks;          // 系统启动时间
    float m_totalGameTime;       // 游戏运作时间
    float m_elapsedGameTime;     // 单帧运作时间
    float m_previousTicks;       // 前次时间戳
    float m_startTicks;          // 启动时间戳
    long m_FrameCnt;            // 帧总数(计算FPS辅助变量)
    float m_TimeElapsed;         // 消耗时间(计算FPS辅助变量)
    float m_FPS;                 // FPS值
    char  m_strFPS[13];          // FPS串
};

#endif