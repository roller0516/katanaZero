#pragma once
#include "GameNode.h"

// 기존 MainGame 클래스의 내용을 이동
class InstallObject;
class MissileManager;
class AstarManager;
class ItemManager;
class EnemyManager;
class EnemyManager;
class CollisionManager;
class Player;
class Tank;
class Image;
class Enemy;
class BattleScene : public GameNode
{
private:
	CollisionManager* collisionManager;
	MissileManager* missileManager;
	AstarManager* astarManager;
	EnemyManager* enemyManager;
	ItemManager* itemManager;
	InstallObject* installObj;
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

