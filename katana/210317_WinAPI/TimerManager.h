#pragma once
#include "config.h"
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;
	char szText[128];
	bool stop;
	bool slow;
	float cooltime;
public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetTimeStop(bool stop) { this->stop = stop; }
	void SetTimeSlow(bool slow) { this->slow = slow; }
	float GetElapsedTime();
	float GetCurrTime();
};

