#pragma once
#include "Enemy.h"

// TODO : 업캐스팅, 다운캐스팅 설명

class Image;
class Enemy_pomp : public Enemy
{
private:
	EnemyData* data;
public:
	virtual HRESULT Init(int posX = 0, int posY = 0);		
	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc, bool world);
	virtual Enemy* Clone();
	virtual void Pattern();
	virtual EnemyData* GetData() { return this->data; }
	/*virtual RECT GetRect() { return this->data->shape; }
	virtual FPOINT GetPos() { return this->data->worldPos; }
	virtual AstarManager* GetastarManager() { return this->data->astar; }
	virtual void SetMoveSpeed(float moveSpeed) { this->data->moveSpeed = moveSpeed; }
	virtual void SetSample(bool isSample) { this->data->isSamPle = isSample; }
	virtual void SetTarget(Player* player) { this->data->target = player; }*/
	virtual MissileManager* GetMissileManager();
	virtual ~Enemy_pomp() {};
	void Move();
	void HorizonMove();
	// get, set
	inline void SetPos(FPOINT pos) { this->data->worldPos = pos; }
	inline int GetSize() { return this->data->size; }
	inline void SetIsAlive(bool isAlive) { this->data->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->data->isAlive; }
	
};

