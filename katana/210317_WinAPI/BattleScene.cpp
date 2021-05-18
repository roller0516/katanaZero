#include "BattleScene.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "CommonFunction.h"
#include "Missile.h"
#include "Image.h"
#include "Player.h"
#include "Camera.h"
#include "CollisionManager.h"

HRESULT BattleScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	curSor = ImageManager::GetSingleton()->AddImage("cursor", "Image/Katana/effect/cursor.bmp", 50, 50, true,RGB(255,0,255));

	player = new Player();
	player->Init();


	bgPos.x = 0;
	bgPos.y = 0;
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
	if (player)
	{
		player->Render(hdc);
	}
	FPOINT pos;
	pos.x = WINSIZE_X - Camera::GetSingleton()->GetCameraPos().x;
	pos.y = WINSIZE_Y - Camera::GetSingleton()->GetCameraPos().y;
	RenderRectToCenter(hdc, pos.x, pos.y, 50, 50);
	if (curSor)
		curSor->Render(hdc, g_ptMouse.x, g_ptMouse.y, true);
	sprintf_s(szText, "playerX : %f , playerY : %f", player->GetWorldpos().x, player->GetWorldpos().y);
	TextOut(hdc, WINSIZE_X - 800, 20, szText, strlen(szText));
	sprintf_s(szText, "angle : %f", player->GetPlayerAngle());
	TextOut(hdc, WINSIZE_X - 400, 20, szText, strlen(szText));
	sprintf_s(szText, "X : %f, Y : %f", GetWorldMousePos(player->GetWorldpos()).x, GetWorldMousePos(player->GetWorldpos()).y);
	TextOut(hdc, 200, 20, szText, strlen(szText));
}

