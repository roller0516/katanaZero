#pragma once
#include "Enemy.h"
class Image;
class Player;
class Enemy_Bold :
    public Enemy
{
private:
	FPOINT worldPos;
	FPOINT localPos;
	Image* image;
	Image* armLImage;
	Image* armRImage;
	Image* GunImage;
	Player* target;
	string name;
	RECT shape;
	EnemyData* data;
	EnemyDir dir;
	EnemyState state;
public:
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc, bool world);
	virtual Enemy* Clone();
	virtual void Pattern();
	
	void Attack();
	void Run();
	void Walk();
	void Hurt();
	void Idle();

	void Animation(Enemy::EnemyState ani);
	void PixelCollisionBottom();
	void PixelCollisionLeft();
	void PixelCollisionRight();
	// get, set
	inline void SetPos(FPOINT pos) { this->worldPos = pos; }
	inline FPOINT GetPos() { return this->worldPos; }
	inline int GetSize() { return this->data->size; }
	inline void SetIsAlive(bool isAlive) { this->data->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->data->isAlive; }

	virtual void SetTarget(Player* player) { this->target = player; }
	virtual void SetMoveSpeed(float moveSpeed) { this->data->moveSpeed = moveSpeed; }
	virtual void SetSample(bool isSample) { this->data->isSamPle = isSample; }
};

