#pragma once
#include "Enemy.h"
class Enemy_Grunt :
    public Enemy
{
private:
	
public:
	Enemy::EnemyType type;
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual Enemy* Clone();
};

