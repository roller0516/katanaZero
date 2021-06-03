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
	Image* lazerAim;
	Image* boss_warning;
	FPOINT currPos;
	FPOINT headPos;
	FPOINT localHeadpos;
	EnemyData* data;
	EnemyDir dir;
	BossState state;

	bool isGround;
	bool isBlack;
	bool patterChange;
	bool start;
	bool stop;
	bool isDie;
	bool noHead;
	bool shot;
	bool hide;
	bool aim;
	int count;
	int maxHp;
	int index;
	int LastIndex;
	int patternIndex;
	int missileCount;
	float testAngle;
	float headFallforce;
	float headFrame;
	float headMaxFrame;
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
	void AttackBox(bool on);
	void SkyShoot();
	void Dash();
	void Hurt();
	void Idle();
	void Die(bool isGround);
	void Grab();
	void DrawGun();
	void DrawGunEasy();
	void TelePort();
	void Shoot();
	void RecoverHide();
	void Recover();
	void TelePort_Ground();
	void Aim();
	void NoHead();

	void changePattern(int index);

	void SetHide(bool hide) { this->hide = hide; }

	void Animation(BossState ani);
	void PixelCollisionBottom();
	void PixelCollisionLeft();
	void PixelCollisionRight();
	void SetIndex(int index) { this->data->Index; }
	// get, set

	inline int GetSize() { return this->data->size; }
	inline bool GetIsAlive() { return this->data->isAlive; }
	inline void SetPos(FPOINT pos) { this->data->worldPos = pos; }
	inline void SetIsAlive(bool isAlive) { this->data->isAlive = isAlive; }
};

