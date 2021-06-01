#pragma once
#include "Enemy.h"

class EnemyEffect;
class AstarManager;
class MissileManager;
class Image;
class Player;
class Enemy_pomp :
	public Enemy
{
private:
	MissileManager* missileManager;
	EnemyEffect* enemyEffect;
	FPOINT currPos;
	EnemyData* data;
	EnemyDir dir;
	EnemyState state;
	int count;
	bool isBlack;
public:
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc, bool world);
	virtual Enemy* Clone();
	virtual void SetSample(bool isSample) { this->data->isSamPle = isSample; }
	virtual void Pattern();
	virtual EnemyData* GetData() { return this->data; }
	virtual MissileManager* GetMissileManager() { return this->missileManager; }

	virtual ~Enemy_pomp() {};

	void Attack(EnemyDir dir);
	void Run();
	void Walk();
	void Hurt();
	void Idle();
	void Die();
	void KnockBack();

	void Animation(EnemyState ani);
	void PixelCollisionBottom();
	void PixelCollisionLeft();
	void PixelCollisionRight();
	void SetIndex(int index) { this->data->Index; }

	void MaptoolAstar();
	// get, set

	inline int GetSize() { return this->data->size; }
	inline bool GetIsAlive() { return this->data->isAlive; }
	inline void SetPos(FPOINT pos) { this->data->worldPos = pos; }
	inline void SetIsAlive(bool isAlive) { this->data->isAlive = isAlive; }
};

