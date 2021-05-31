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
	door_break,
	end
};
class PlayerEffect;
class ItemManager;
class Image;
class Player : public GameNode
{
private:
	ItemManager* itemManager;
	PlayerEffect* playerEffect;
	Image* image;
	RECT shape;
	RECT hitShape;
	RECT attackShape;
	FPOINT Worldpos;
	FPOINT Clientpos; //
	FPOINT currPos;
	POINT mousPos;

	int count;
	int runCount;
	int attCount;
	int itemCount;
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

	bool isReflect;
	bool black;
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
	bool isDoor;
	int itemIndex;
	int keycount;
public:
	Direction dir;
	PlayerState playerstate;

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

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
	void Getitem();
	void DoorBreak();
	void PixelCollisionBottom();
	void PixelCollisionLeft();
	void PixelCollisionRight();
	void PixelCollisionTop();
	void ReflectEffect();
	void HitEffect(int x ,int y);

	inline void SetReflect(bool reflect) { this->isReflect = reflect; }
	inline void SetDoor(bool door) { this->isDoor = door; }
	inline bool GetisGround() { return isGround; }
	inline Direction GetDir() { return this->dir; }
	inline int GetItemIndex() { return this->itemIndex; }
	inline void SetitemManager(ItemManager* itemManager) { this->itemManager = itemManager; }
	inline RECT GetAttackShape() { return this->attackShape; }
	inline bool GetFalling(){ return isFall; }
	inline void SetGround(bool isGround) { this->isGround = isGround; }
	inline void SetPos(int x, int y) { this->Worldpos.x = x; this->Worldpos.y = y; }
	inline FPOINT GetWorldpos() { return this->Worldpos; }
	inline FPOINT Getpos() { return this->Clientpos; }
	inline FPOINT GetCurrPos() { return this->currPos; }
	inline RECT GetRect() { return this->hitShape; }
	inline float GetPlayerAngle() { return this->angle; }
	inline Image* GetImageInfo() { return this->image; }
	inline void SetPosY(int y) { this->Worldpos.y = y; }
};

