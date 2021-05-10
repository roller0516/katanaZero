#pragma once
#include "GameNode.h"

// ���� MainGame Ŭ������ ������ �̵�
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

