#pragma once
#include "GameNode.h"

//class InstallObject;
class MissileManager;
//class AstarManager;
class ItemManager;
class EnemyManager;
class EnemyManager;
class CollisionManager;
class Player;
class Tank;
class Image;
class Enemy;
class UI;

class BattleScene : public GameNode
{
private:
	//AstarManager* astarManager;
	//InstallObject* installObj;
	CollisionManager* collisionManager;
	MissileManager* missileManager;
	EnemyManager* enemyManager;
	ItemManager* itemManager;
	UI* ui;
	ENMY_INFO enemySize[100];

	int monsterCount;
	Enemy* enemy;
	Player* player;
	Image* BackGround;
	Image* curSor;
	FPOINT bgPos;
	char szText[128];
	//TILE_INFO tileInfo[TILE_X * TILE_Y];
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetMonsterCount(int count) { this->monsterCount = count; }
	int GetMonsterCount() { return this->monsterCount; }
	void MapLoad(int stageNum);

	virtual ~BattleScene() {};
};