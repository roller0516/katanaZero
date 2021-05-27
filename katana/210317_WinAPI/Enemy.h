#pragma once
#include "config.h"

enum EnemyState
{
	idle,
	walk,
	run,
	attack,
	hurt
};
enum class EnemyDir
{
	Right,
	Left
};
class MissileManager;
class Image;
class AstarManager;
class Player;
class Enemy
{
public:
	struct EnemyData
	{
		Image* image = nullptr;
		Player* target = nullptr;
		AstarManager* astar = nullptr;
		RECT shape = { 0,0,0,0 };
		string Name;
		FPOINT worldPos = { 0, 0 };
		FPOINT localPos = { 0, 0 };
		float currFrameX = 0;
		float velocity = 60;
		float angle = 0;
		float moveSpeed = 0;
		float delay = 0;
		float fallForce = 0;
		float findRange = 0;
		float attackRange = 0;
		float attackSpeed = 0;
		float attackAngle = 0.0f;
		float knockBackPower = 0.0f;
		int maxFrame = 0;
		int updateCount = 0;
		int size = 0;
		bool isKnockBack = false;
		bool isSamPle = false;
		bool isFind = false;
		bool isAttack = false;
		bool isAlive = false;
		bool isPhysic = true;
		bool leftWall = false;
		bool rightWall = false;
	};
public:
	virtual HRESULT Init(int posX = 0, int posY = 0) = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc, bool world) = 0;
	virtual EnemyData* GetData() = 0;
	virtual Enemy* Clone() =0;
	virtual MissileManager* GetMissileManager() = 0;
	/*virtual void SetMoveSpeed(float moveSpeed) = 0;
	virtual void SetSample(bool isSample) = 0;
	virtual void Pattern() = 0;
	virtual void SetTarget(Player* player) = 0;
	virtual FPOINT GetPos() = 0;
	virtual RECT GetRect() = 0;
	virtual AstarManager* GetastarManager() = 0;*/
	virtual ~Enemy() {};
};

