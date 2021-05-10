#pragma once
#include "GameNode.h"

// 기존 MainGame 클래스의 내용을 이동
class Player;
class Tank;
class EnemyManager;
class Image;
class Camera;
class BattleScene : public GameNode
{
private:
	Player* player;
	Camera* camera;
	Image* BackGround;
	FPOINT bgPos;

	Image* curSor;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~BattleScene() {};
};

