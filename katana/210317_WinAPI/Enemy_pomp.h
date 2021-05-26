#pragma once
#include "Enemy.h"

// TODO : 업캐스팅, 다운캐스팅 설명

class AstarManager;
class Image;
class Player;
class Enemy_pomp : public Enemy
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
	virtual ~Enemy_pomp() {};
	void Move();
	void HorizonMove();
	virtual RECT GetRect() { return this->data->shape; }
	// get, set
	virtual void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	virtual void SetSample(bool isSample) { this->isSamPle = isSample; }
	virtual void SetTarget(Player* player) { this->target = player; }
	inline void SetPos(FPOINT pos) { this->worldPos = pos; }
	virtual void SetAstarManager(AstarManager* astar) { this->astarManager = astar; }
	inline virtual FPOINT GetPos() { return this->worldPos; }
	inline int GetSize() { return this->size; }
	inline void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->isAlive; }
	
};

