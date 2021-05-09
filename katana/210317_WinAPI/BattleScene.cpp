#include "BattleScene.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "CommonFunction.h"
#include "Missile.h"
#include "Image.h"
#include "Player.h"


HRESULT BattleScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	player = new Player();
	player->Init();

	BackGround = new Image;
	BackGround = ImageManager::GetSingleton()->AddImage("stage1_bg_render", "Image/Katana/stage1_bg_render.bmp", 2176, 3500);

	return S_OK;
}

void BattleScene::Release()
{
	SAFE_RELEASE(player);
}

void BattleScene::Update()
{
	if (player)
	{
		player->Update();
	}
}

void BattleScene::Render(HDC hdc)
{
	if (BackGround)
		BackGround->Render(hdc, 0, 0);
	if (player)
	{
		player->Render(hdc);
	}
}

