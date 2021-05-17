#pragma once
#include "Enemy.h"

// TODO : 업캐스팅, 다운캐스팅 설명
class Image;
class Player;
class Enemy_pomp : public Enemy
{
private:
	
	Image* image;
	int currFrameX;
	int updateCount;

	FPOINT pos;
	int size;
	string name;
	RECT shape;
	float moveSpeed;
	bool isAlive;
	float angle;

	//GameNode* target;
	Player* target;
	int dir;
	int fireCount;

public:
	Enemy::EnemyType type;
	virtual HRESULT Init(int posX = 0, int posY = 0);		
	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc);
	virtual Enemy* Clone();

	void Move();
	void HorizonMove();

	// get, set
	inline void SetPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPos() { return this->pos; }
	inline int GetSize() { return this->size; }
	inline void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->isAlive; }
};

