#pragma once
#include "GameNode.h"

enum class MissileType {enemy,player};

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
	//Enemy* owner;
	MissileType missile;
public:
	HRESULT Init();//Enemy* owner);
	void Release();		
	void Update();		
	void Render(HDC hdc);

	void MovingNormal();
	void SetIsFired(bool isFired);

	inline RECT GetRect() { return this->shape; }
	inline MissileType GetType() {return this->missile; }
	inline void SetType(MissileType type) { this->missile = type; }
	inline void SetPos(int x, int y) { this->worldPos.x = x; this->worldPos.y = y; }
	inline void SetLocalPos(int x, int y) { this->localPos.x = x; this->localPos.y = y; }
	inline void SetPos(FPOINT pos) { this->worldPos = pos; }
	inline FPOINT GetWorldPos() { return this->worldPos; }
	inline FPOINT GetLocalPos() { return this->localPos; }
	inline bool GetIsFired() { return this->isFired; }
	inline void SetAngle(float angle) { this->angle = angle; }
	inline void SetFireIndex(int fireIndex) { this->fireIndex = fireIndex; }
	inline int GetSize() { return this->size; }
};

