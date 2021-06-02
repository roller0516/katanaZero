#pragma once
#include "GameNode.h"

enum class MissileOwnerType {enemy,player};
enum class MissileType {Toplazer,lazer,Normal,Mine};
class Image;
class Enemy;
class Missile : public GameNode
{
private:
	// ÀÌ¹ÌÁö
	Image* img;
	Image* minerange;
	Image* flameEffect[6];
	FPOINT worldPos;
	FPOINT localPos;
	FPOINT flamePos;
	RECT shape;
	int size;
	int circleSize;
	float moveSpeed;
	float moveTime;
	float angle;
	bool isFired;
	bool flameOn;

	int fireIndex;
	int fireStep;
	float fallForce;
	float velocity;
	float destAngle;
	float currFrame;
	float maxFrame;
	float coolTime;
	float delay;
	//Enemy* owner;
	MissileOwnerType missile;
	MissileType missileType;
public:
	HRESULT Init();//Enemy* owner);
	void Release();		
	void Update();		
	void Render(HDC hdc);

	void MovingNormal();
	void lazer();
	void Toplazer();
	void Mine();

	inline void SetIsFired(bool isFired) {this->isFired = isFired;};
	inline RECT GetRect() { return this->shape; }
	inline MissileOwnerType GetOwnerType() {return this->missile; }
	inline void SetOwnerType(MissileOwnerType type) { this->missile = type; }
	inline MissileType GetType() { return this->missileType; }
	inline void SetDelay(float delay) { this->coolTime = delay; }
	inline void SetType(MissileType type) { this->missileType = type; }
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

