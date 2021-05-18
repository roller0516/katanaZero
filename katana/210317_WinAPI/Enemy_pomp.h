#pragma once
#include "Enemy.h"

// TODO : 업캐스팅, 다운캐스팅 설명
class Image;
class Player;
class Enemy_pomp : public Enemy
{
private:
	FPOINT pos;
	Image* image;
	Player* target;
	string name;
	RECT shape;
	int currFrameX;
	int maxFrame;
	int updateCount;
	int size;
	float moveSpeed;
	float angle;
	bool isAlive;

public:
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

