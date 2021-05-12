#pragma once
#include "GameNode.h"
enum class Direction 
{
	LEFT,
	RIHGT,
	END
};
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
class Camera;
class Image;
class Player : public GameNode
{
private:
	Camera* camera;
	Image* image;
	RECT shape;

	FPOINT Worldpos;
	FPOINT Clientpos; //
	POINT mousPos;
	FPOINT currPos;
	int tick;

	float angle;
	float count[2];
	float moveSpeed;	// 초당 이동거리
	float currFrame;
	float attackCurrFrame;
	float maxFrame;

	float velocity;
	float jumpHeight;

	int frameSet;


	bool gravityOn;
	bool isGround;
	bool isMove;
	bool isDying;
	bool isAlive;
	bool frameRun;
	bool isAttack;

public:
	Direction dir;
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void Animation(PlayerState ani, bool loop);
	void Jumping();
	void Move();
	void Attack();
	void OnDead();

	inline void SetGround(bool isGround) { this->isGround = isGround; }
	inline FPOINT GetWorldpos() { return this->Worldpos; }
	inline FPOINT Getpos() { return this->Clientpos; }
	inline FPOINT GetCurrPos() { return this->currPos; }
	inline RECT GetRect() { return this->shape; }
	inline float GetPlayerAngle() { return this->angle; }
	inline Image* GetImageInfo() { return this->image; }
	inline void SetPosY(int y) { this->Worldpos.y = y; }
	inline Camera* GetCamera() { return this->camera; }
};

