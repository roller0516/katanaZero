#pragma once
#include "GameNode.h"

// ���� MainGame Ŭ������ ������ �̵�
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

