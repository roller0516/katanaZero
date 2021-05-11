#pragma once
#include "GameNode.h"

// ���� MainGame Ŭ������ ������ �̵�
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

