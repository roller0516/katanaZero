#pragma once
#include "GameNode.h"

// 기존 MainGame 클래스의 내용을 이동
class Player;
class Tank;
class EnemyManager;
class Image;
class BattleScene : public GameNode
{
private:
	Player* player;
	Image* BackGround;
	FPOINT bgPos;
	char szText[128];
	Image* curSor;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~BattleScene() {};
};

