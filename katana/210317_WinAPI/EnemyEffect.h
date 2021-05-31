#pragma once
#include "GameNode.h"

enum class EnemyEffectType {EnemySpark , EnemySlash ,blood_ani, blood1, blood2, blood3, blood4 , Find,HitEffect};
enum class EnemyEffectDir {Right,Left};

class Enemy;
class Image;
class EnemyEffect : public GameNode
{
private:
	Image* img;
	FPOINT worldPos;
	FPOINT localPos;
	float currFrame;
	float maxFrame;
	EnemyEffectType Type;
	EnemyEffectDir dir;
	Enemy* owner;

	bool isAlive;
	
public:
	 HRESULT Init(int x, int y, EnemyEffectType type);
	 void Release();
	 void Update();
	 void Render(HDC hdc);
	 bool GetAlive() { return this->isAlive; }
	 void SetAlive(bool alive) { this->isAlive = alive; }
	 void SetDir(EnemyEffectDir dir) { this->dir = dir; }
	 void SetPos(int x, int y) { this->worldPos.x = x; this->worldPos.y = y; }
	 void SetOwner(Enemy* owner) { this->owner = owner; }
};
