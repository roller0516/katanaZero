#pragma once
#include "GameNode.h"

class Image;
class Enemy;
class Missile : public GameNode
{
private:
	// ÀÌ¹ÌÁö
	Image* img;
	FPOINT worldPos;
	FPOINT localPos;
	RECT shape;
	int size;
	float moveSpeed;
	float moveTime;
	float angle;
	bool isFired;
	int fireIndex;
	int fireStep;
	float destAngle;
	Enemy* owner;

public:
	HRESULT Init(Enemy* owner);
	void Release();		
	void Update();		
	void Render(HDC hdc);

	void MovingNormal();
	void SetIsFired(bool isFired);

	inline void SetPos(int x, int y) { this->worldPos.x = x; this->worldPos.y = y; }
	inline void SetPos(FPOINT pos) { this->worldPos = pos; }
	inline FPOINT GetWorldPos() { return this->worldPos; }
	inline FPOINT GetLocalPos() { return this->localPos; }
	inline bool GetIsFired() { return this->isFired; }
	inline void SetAngle(float angle) { this->angle = angle; }
	inline void SetFireIndex(int fireIndex) { this->fireIndex = fireIndex; }
	inline int GetSize() { return this->size; }
};

