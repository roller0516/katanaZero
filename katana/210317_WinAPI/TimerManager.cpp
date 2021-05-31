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
    if(KeyManager::GetSingleton()->IsStayKeyDown(VK_SHIFT))
        return 0.001;
    if(KeyManager::GetSingleton()->IsOnceKeyUp(VK_SHIFT))
        return timer->GetElapsedTime();
    if (Stop) 
    {
        cooltime+= 0.001;
        if (cooltime > 0.5f) 
        {
            cooltime = 0;
            Stop = false;
        }
            
        return 0.0001;
    }
    
       
    //return 0.005;
    return timer->GetElapsedTime();
}

float TimerManager::GetCurrTime()
{
    return timer->GetCurrTime();
}
