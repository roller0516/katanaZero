#pragma once
#include "config.h"
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;
	char szText[128];
	bool Stop;
	float cooltime;
public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetTimeStop(bool stop) { this->Stop = stop; }

	float GetElapsedTime();
	float GetCurrTime();
};

