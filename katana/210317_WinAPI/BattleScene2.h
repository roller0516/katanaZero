#pragma once
#include "GameNode.h"

//class InstallObject;
class CommonEffect;
class MissileManager;
class ItemManager;
class EnemyManager;
class EnemyManager;
class CollisionManager;
class Player;
class Tank;
class Image;
class Enemy;
class UI;

class BattleScene2 : public GameNode
{
private:
	CommonEffect* flameEffect;
	CollisionManager* collisionManager;
	MissileManager* missileManager;
	EnemyManager* enemyManager;
	ItemManager* itemManager;
	UI* ui;
	Enemy* enemy;
	Enemy* boss;
	Player* player;
	Image* BackGround;
	Image* curSor;
	FPOINT bgPos;

	float flameCooltime;
	int flameIndex;
	int page;
	int monsterCount;
	bool bossSpwan;
	bool chagnePage;
	char szText[128];
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void SetMonsterCount(int count) { this->monsterCount = count; }
	virtual int GetMonsterCount() { return this->monsterCount; }
	virtual ~BattleScene2() {};
};