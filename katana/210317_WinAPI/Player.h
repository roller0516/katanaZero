#pragma once
#include "GameNode.h"
enum class Direction 
{
	LEFT,
	RIHGT,
	TOP,
	BOTTOM,
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
	jumpEnd,
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
	RECT shape;

	FPOINT Worldpos;
	FPOINT Clientpos; //
	FPOINT currPos;
	POINT mousPos;


	int size;
	int tick;
	float angle;
	float moveSpeed;	// 초당 이동거리
	float currFrame;
	float maxFrame;
	float attackSpeed;
	float fallForce;
	float velocity;
	float wallJumpTime;
	
	int StartchangeWallIndex;
	int EndchangeWallIndex;

	bool isFilp;
	bool isGrab;
	bool leftWall;
	bool RightWall;
	bool isJumping;
	bool isFall;
	bool isGround;
	bool isMove;
	bool frameRun;
	bool isAttack;
	bool isPhysics;
	bool isRoll;

	int keycount;
public:
	Direction dir;
	PlayerState playerstate;

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual Player* Clone();

	void Animation(PlayerState ani);

	void Run();
	void Roll();
	void Flip();
	void Grab();
	void RuntoIdle();
	void Attack();
	void Falling();
	void Jumping();
	void JumpingEnd();
	void PlayerFSM();
	void PlayerKeyMove();
	void GrabEnd();

	void PixelCollisionBottom();
	void PixelCollisionLeft();
	void PixelCollisionRight();
	void PixelCollisionTop();
	
	inline bool GetFalling(){ return isFall; }
	inline void SetGround(bool isGround) { this->isGround = isGround; }
	inline FPOINT GetWorldpos() { return this->Worldpos; }
	inline FPOINT Getpos() { return this->Clientpos; }
	inline FPOINT GetCurrPos() { return this->currPos; }
	inline RECT GetRect() { return this->shape; }
	inline float GetPlayerAngle() { return this->angle; }
	inline Image* GetImageInfo() { return this->image; }
	inline void SetPosY(int y) { this->Worldpos.y = y; }
};

