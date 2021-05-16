#pragma once
#include "GameNode.h"

class Image;
class Enemy;
class Missile : public GameNode
{
public:
	enum TYPE { Normal, Skill_01, FollowTarget,StarMoving ,End };

private:
	// ÀÌ¹ÌÁö
	Image* img;

	FPOINT pos;
	float moveSpeed;
	int size;
	RECT shape;
	int damage;
	float angle;
	bool isFired;
	bool isDraw;
	TYPE missileType;
	int fireIndex;
	int fireStep;
	float timer;
	//GameNode* target;
	Enemy* target;
	float destAngle;

public:
	HRESULT Init();		
	void Release();		
	void Update();		
	void Render(HDC hdc);

	void MovingNormal();
	void MovingSkill_01();
	void MovingFollowTarget();
	void MovigStar();

	inline void SetPos(FPOINT pos) { this->pos = pos; }	
	inline void SetPos(float posX, float posY) { this->pos.x = posX; this->pos.y = posY; }
	inline FPOINT GetPos() { return this->pos; }
	inline void SetIsFired(bool isFired) { this->isFired = isFired; }
	inline bool GetIsFired() { return this->isFired; }


	inline void SetTarget(Enemy* target) { this->target = target; }
	inline void SetType(TYPE type) { this->missileType = type; }
	inline void SetAngle(float angle) { this->angle = angle; }
	inline void SetFireIndex(int fireIndex) { this->fireIndex = fireIndex; }
	inline void SetDraw(bool draw) { this->isDraw = draw; }
	inline bool GetDraw() { return this->isDraw; }
	inline int GetSize() { return this->size; }
};

