#pragma once
#include "GameNode.h"
#include <vector>

class Enemy;
class Missile;
class MissileManager : public GameNode
{
private:
	vector<Missile*> vMissiles;
	vector<Missile*>::iterator itMissiles;

	Enemy* owner;

public:
	HRESULT Init(Enemy* owner);
	void Release();
	void Update();
	void Render(HDC hdc);

	void Fire();
};

