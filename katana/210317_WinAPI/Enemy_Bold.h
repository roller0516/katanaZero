#pragma once
#include "Enemy.h"

class AstarManager;
class MissileManager;
class Image;
class Player;
class Enemy_Bold :
    public Enemy
{
private:
	MissileManager* missileManager;
	AstarManager* astarManager;
	FPOINT worldPos;
	FPOINT localPos;
	Image* image;
	Image* armLImage;
	Image* armRImage;
	Image* GunImage;
	Player* target;
	string name;

	EnemyData* data;
	EnemyDir dir;
	EnemyState state;

	int targeton;

	float angle;
	float destAngle;
public:
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc, bool world);
	virtual Enemy* Clone();
	virtual void Pattern();
	virtual ~Enemy_Bold() {};

	void Attack(EnemyDir dir);
	void Run();
	void Walk();
	void Hurt();
	void Idle();

	void Animation(Enemy::EnemyState ani);
	void PixelCollisionBottom();
	void PixelCollisionLeft();
	void PixelCollisionRight();
	// get, set
	virtual void SetAstarManager(AstarManager* astar) { this->astarManager = astar; }
	inline void SetPos(FPOINT pos) { this->worldPos = pos; }
	inline virtual FPOINT GetPos() { return this->worldPos; }
	inline int GetSize() { return this->data->size; }
	inline void SetIsAlive(bool isAlive) { this->data->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->data->isAlive; }
	virtual RECT GetRect() { return this->data->shape; }
	virtual void SetTarget(Player* player) { this->target = player; }
	virtual void SetMoveSpeed(float moveSpeed) { this->data->moveSpeed = moveSpeed; }
	virtual void SetSample(bool isSample) { this->data->isSamPle = isSample; }
};

