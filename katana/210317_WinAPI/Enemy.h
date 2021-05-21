#pragma once
#include "config.h"

class Player;
class Enemy
{
public:
	enum EnemyState
	{
		idle,
		walk,
		run,
		attack,
		hurt
	};
public:
	enum class EnemyDir
	{
		Right,
		Left
	};
public:
	struct EnemyData
	{
		float currFrameX = 0;
		float velocity = 60;
		float angle = 0;
		float moveSpeed = 0;
		float delay = 0;
		float fallForce = 0;
		float findRange = 0;
		float attackRange = 0;
		int maxFrame = 0;
		int updateCount = 0;
		int size = 0;
		bool isSamPle = false;
		bool isFind = false;
		bool isAttack = false;
		bool isAlive = true;
		bool isPhysic = true;
		bool leftWall = false;
		bool rightWall = false;
	};
public:
	virtual HRESULT Init(int posX = 0, int posY = 0) = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc, bool world) = 0;
	virtual Enemy* Clone() =0;
	virtual void SetMoveSpeed(float moveSpeed) = 0;
	virtual void SetSample(bool isSample) = 0;
	virtual void Pattern() = 0;
	virtual void SetTarget(Player* player) = 0;
};

