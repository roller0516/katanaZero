#pragma once
#include "Enemy.h"


enum class BossState 
{
	Idle,
	Dash,
	Jump,
	DashEnd,
	PreDash,
	Dead,
	DieFly,
	DieGround,
	Land,
	Aim,
	DrawGun,
	Recover,
	RecoverHide,
	In_Pattern,
	In_Sweep,
	Out_Pattern,
	Out_Sweep,
	PreJump,
	Shoot,
	TelePort_Fly,
	TelePort_Ground,
	WallGrab,
	WallJump,
	Head_Ground,
	Head_Fly,
	Head_no,
};

class EnemyEffect;
class MissileManager;
class Image;
class Player;
class Boss :
	public Enemy
{
private:
	MissileManager* missileManager;
	EnemyEffect* enemyEffect;
	Image* head;
	FPOINT currPos;
	EnemyData* data;
	EnemyDir dir;
	BossState state;
	int count;
	bool isBlack;
	bool patterChange;
	//bool End;
	bool start;
	bool stop;
	int index;
	int LastIndex;
	int patternIndex;
	float attackCooltime;
public:
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc, bool World);
	virtual Enemy* Clone();
	virtual void SetSample(bool isSample) { this->data->isSamPle = isSample; }
	virtual void Pattern();
	virtual EnemyData* GetData() { return this->data; }
	virtual MissileManager* GetMissileManager() { return this->missileManager; }

	virtual ~Boss() {};

	void SkyShoot();
	void Dash();
	void Hurt(int index);
	void Idle();
	void Die(bool isGround);
	void Grab();
	void DrawGun();
	void TelePort();

	void Animation(BossState ani);
	void PixelCollisionBottom();
	void PixelCollisionLeft();
	void PixelCollisionRight();
	void SetIndex(int index) { this->data->Index; }

	void MaptoolAstar();
	// get, set

	inline int GetSize() { return this->data->size; }
	inline bool GetIsAlive() { return this->data->isAlive; }
	inline void SetPos(FPOINT pos) { this->data->worldPos = pos; }
	inline void SetIsAlive(bool isAlive) { this->data->isAlive = isAlive; }
};

