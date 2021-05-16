#pragma once
#include "GameNode.h"

// TODO : 업캐스팅, 다운캐스팅 설명
class Image;
class MissileManager;
class Enemy : public GameNode
{
private:
	Image* image;
	int currFrameX;

	FPOINT pos;
	RECT shape;
	string name;
	float moveSpeed;
	float angle;
	bool isAlive;
	bool isAttack;
	int size;
	int dir;
	int hp;
	MissileManager* missileManager;
	//GameNode* target;
	//Tank* target;
	int elapsedTime;
	int elapsedTime2;
	int attackTime;

public:
	HRESULT Init();		
	void Release();		
	void Update();		
	void Render(HDC hdc);

	void Move();
	void HorizonMove();
	void Pattern1();
	// get, set
	inline void SetPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPos() { return this->pos; }
	//inline void SetTarget(Tank* target) { this->target = target; }
	inline int GetSize() { return this->size; }
	inline void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->isAlive; }
};

