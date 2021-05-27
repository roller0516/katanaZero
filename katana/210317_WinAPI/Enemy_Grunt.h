#pragma once
#include "Enemy.h"
class AstarManager;
class Player;
class Image;
class Enemy_Grunt :
    public Enemy
{
private:
	AstarManager* astarManager;
	Image* image;
	Player* target;
	EnemyData* data;
public:
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc, bool world);
	virtual Enemy* Clone();
	virtual EnemyData* GetData() { return this->data; }
	virtual void Pattern();
	virtual MissileManager* GetMissileManager();
	/*virtual void SetTarget(Player* player) { this->target = player; }
	virtual FPOINT GetPos() { return this->data->worldPos; }
	virtual AstarManager* GetastarManager() { return this->astarManager; }
	virtual void SetMoveSpeed(float moveSpeed) { this->data->moveSpeed = moveSpeed; }
	virtual void SetSample(bool isSample) { this->data->isSamPle = isSample; }
	virtual RECT GetRect() { return data->shape; }*/
	virtual ~Enemy_Grunt() {};

	void Move();
	void HorizonMove();

	// get, set
	inline void SetPos(FPOINT pos) { this->data->worldPos = pos; }
	inline void SetIsAlive(bool isAlive) { this->data->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->data->isAlive; }
};

