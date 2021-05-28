#pragma once
#include "GameNode.h"

// ���� MainGame Ŭ������ ������ �̵�
class AstarManager;
class ItemManager;
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
	CollisionManager* collisionManager;
	ItemManager* itemManager;
	AstarManager* astarManager;
	Enemy* enemy;
	Player* player;
	Image* BackGround;
	Image* curSor;
	FPOINT bgPos;
	char szText[128];
	TILE_INFO tileInfo[TILE_X * TILE_Y];
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void MapLoad(int stageNum);

	virtual ~BattleScene() {};
};

