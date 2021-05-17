#pragma once
#include "config.h"
class Enemy
{
public:
	enum class EnemyType 
	{
		Pomp,
		Bold,
		Cop,
		Grunt
	};

public:
	virtual HRESULT Init(int posX = 0, int posY = 0) = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;
	virtual Enemy* Clone() =0;
};

