#pragma once
#include "Enemy.h"
class Enemy_Grunt :
    public Enemy
{
private:
	
public:
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual Enemy* Clone();
};

