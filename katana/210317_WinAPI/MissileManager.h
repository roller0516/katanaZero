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
	//Enemy* owner;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	void Fire(float angle,int x, int y, float delay);
	void Lazer(float angle, int x, int y,float delay);
	void TopLazer(float angle, int x, int y, float delay);
	void Mine(float angle, int x, int y,float delay);

	vector<Missile*> GetMissile() { return this->vMissiles; }
};

