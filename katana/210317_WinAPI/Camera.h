#pragma once
#include "config.h"
#include "Singleton.h"
class Image;
class TilemapTool;
class Player;
class Camera : public Singleton<Camera>
{
private:
	FPOINT worldPos;
	FPOINT pos; // 카메라 포즈
	FPOINT worldrc;
	TilemapTool* TilemapTooltarget;
	Player* target;
	Image* bg;
	Image* bg_Collision;
	FPOINT pivot;
	float curtime;
	bool debug;
public:
	HRESULT Init(Player* target);
	HRESULT Init(TilemapTool* target);
	void Release();
	void Update();
	void Render(HDC hdc);
	void View();
	inline FPOINT GetCamerPivot() { return this->pivot; }
	inline FPOINT GetWorld() { return this->worldrc; }
	inline FPOINT GetCameraPos() { return this->pos; }
	inline Image* GetCollisionBG() { return this->bg_Collision; }
	inline void SetPos(FPOINT pos) { this->pos = pos; }
	FPOINT Distance(FPOINT pos, POINT pos2);
	FPOINT Distance(FPOINT pos, FPOINT pos2);
	float GetAngle(FPOINT pos, POINT targetPos);
	float GetAngle(FPOINT pos, FPOINT targetPos);
	FPOINT GetWorldMousePos();
};


