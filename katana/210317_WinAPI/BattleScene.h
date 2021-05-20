#pragma once
#include "GameNode.h"

// 기존 MainGame 클래스의 내용을 이동
class EnemyManager;
class CollisionManager;
class Player;
class Tank;
class EnemyManager;
class Image;
class Enemy;
class BattleScene : public GameNode
{
private:
	EnemyManager* enemyManager;
	Enemy* enemy;
	CollisionManager* collisionManager;
	Player* player;
	Image* BackGround;
	Image* curSor;
	FPOINT bgPos;
	char szText[128];

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~BattleScene() {};
};

