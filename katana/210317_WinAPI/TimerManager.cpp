#include "TimerManager.h"
#include "Timer.h"

HRESULT TimerManager::Init()
{
    timer = new Timer();
    timer->Init();

    return S_OK;
}

void TimerManager::Release()
{
    SAFE_DELETE(timer);
}

void TimerManager::Update()
{
    if (timer)
    {
        timer->Tick();
    }
}

void TimerManager::Render(HDC hdc)
{
    if (timer)
    {
        wsprintf(szText, "FPS : %d", timer->GetFPS());
        TextOut(hdc, WINSIZE_X - 150, 20, szText, strlen(szText));
    }
}

float TimerManager::GetElapsedTime()
{
    if (slow) 
        return 0.0008;

    if (stop) 
    {
        cooltime+= 0.0005;
        if (cooltime > 0.5f) 
        {
            cooltime = 0;
            stop = false;
        }
            
        return 0.0001;
    }
    
       
    //return 0.0005;
    return timer->GetElapsedTime();
}

float TimerManager::GetCurrTime()
{
    return timer->GetCurrTime();
}
