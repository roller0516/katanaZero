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
	Image* bg;
	Image* bg_Collision;
	FPOINT pivot;
	float curtime;

public:
	HRESULT Init(Player* target);
	void Release();
	void Update();
	void Render(HDC hdc);

	inline FPOINT GetCamerPivot() { return this->pivot; }
	inline FPOINT GetWorldPos() { return this->worldrc; }
	inline FPOINT GetCameraPos() { return this->pos; }
	inline Image* GetCollisionBG() { return this->bg_Collision; }
	inline void SetPos(FPOINT pos) { this->pos = pos; }


};


