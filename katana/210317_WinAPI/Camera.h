#pragma once
#include "GameNode.h"


class Player;
class Camera :
	public GameNode
{

private:
	FPOINT startPos;
	FPOINT pos; // 카메라 포즈
	FPOINT worldrc;

	Player* target;
	Image* image;
	FPOINT pivot;
	float curtime;
public:
	HRESULT Init(Player* target);
	void Release();
	void Update();
	void Render(HDC hdc);
	float lerp(float p1, float p2, float d1);
	

	FPOINT GetWorldPos() { return this->worldrc; }
	FPOINT GetCameraPos() { return this->pos; }
};

