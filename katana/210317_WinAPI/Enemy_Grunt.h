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
	FPOINT worldPos;
	FPOINT localPos;
	Image* image;
	Player* target;
	string name;
	RECT shape;
	EnemyData* data;
	float currFrameX;
	float angle;
	float moveSpeed;

	int maxFrame;
	int updateCount;
	int size;

	bool isAlive;
	bool isSamPle;
public:
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc, bool world);
	virtual Enemy* Clone();
	virtual void Pattern();
	virtual void SetTarget(Player* player) { this->target = player; }
	virtual ~Enemy_Grunt() {};
	virtual RECT GetRect() { return this->data->shape; }
	void Move();
	void HorizonMove();

	// get, set
	virtual void SetAstarManager(AstarManager* astar) { this->astarManager = astar; }
	inline void SetPos(FPOINT pos) { this->worldPos = pos; }
	inline virtual FPOINT GetPos() { return this->worldPos; }
	inline int GetSize() { return this->size; }
	inline void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->isAlive; }
	virtual void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	virtual void SetSample(bool isSample) { this->isSamPle = isSample; }

};

