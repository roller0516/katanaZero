#pragma once
#include "GameNode.h"

// TODO : 업캐스팅, 다운캐스팅 설명
class Image;
class MissileManager;
class Player;
class Enemy : public GameNode
{
private:
	MissileManager* missileMgr;

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
	HRESULT Init(int posX = 0, int posY = 0);		
	void Release();		
	void Update();		
	void Render(HDC hdc);

	void Move();
	void HorizonMove();

	// get, set
	inline void SetPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPos() { return this->pos; }
	inline int GetSize() { return this->size; }
	inline void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	inline bool GetIsAlive() { return this->isAlive; }
};

