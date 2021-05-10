#include "BattleScene.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "CommonFunction.h"
#include "Missile.h"
#include "Image.h"
#include "Player.h"
#include "Camera.h"

HRESULT BattleScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	player = new Player();
	player->Init();

	camera = new Camera;
	camera->Init(player);

	BackGround = ImageManager::GetSingleton()->AddImage("stage1_bg_render", "Image/Katana/stage1_bg_render.bmp", 2176, 3500);
	bgPos.x = 0;
	bgPos.y = 0;
	curSor = ImageManager::GetSingleton()->AddImage("cursor", "Image/Katana/effect/cursor.bmp", 50, 50, true,RGB(255,0,255));
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
	camera->Update();
	//camera->Move(BackGround);
	
}


void BattleScene::Render(HDC hdc)
{
	float x, y;
	x = player->Getpos().x;
	y = player->Getpos().y;
	if (BackGround) 
	{
		if (0 > (x - WINSIZE_X / 2))
		{
			x = WINSIZE_X +x ;
		}
		else if (2176 > ((x + WINSIZE_X / 2)))
		{
			x = x - WINSIZE_X / 2;
		}
		BackGround->CameraRender(hdc, x, y, WINSIZE_X, WINSIZE_Y, false);
	}
		
		
	if (player)
	{
		player->Render(hdc);
	}
	if (curSor)
		curSor->Render(hdc, g_ptMouse.x, g_ptMouse.y, true);
	
}

