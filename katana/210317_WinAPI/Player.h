#pragma once
#include "GameNode.h"
enum class PlayerState
{
	idle,
	idle_to_run,
	run,
	run_to_idle,
	attack,
	jump,
	flip,
	grab,
	roll,
	hit_fly,
	hit_ground,
	fall,
	end
};
class Image;
class Player : public GameNode
{
private:
	Image* image;
	FPOINT pos;
	FPOINT mousPos;
	FPOINT currPos;
	int tick;

	float angle;

	float moveSpeed;	// 초당 이동거리
	float currFrame;
	float attackCurrFrame;
	float maxFrame;

	bool isMove;
	bool isDying;
	bool isAlive;
	bool framRun;
	bool isAttack;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void Animation(PlayerState ani,bool loop);
	void Jumping();
	void Move();
	void Attack();
	void OnDead();
};

