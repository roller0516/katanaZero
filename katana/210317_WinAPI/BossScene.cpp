#include "BossScene.h"
#include "Player.h"
#include "Boss.h"
#include "Enemy.h"
#include "Camera.h"
#include "Image.h"
HRESULT BossScene::Init()
{
	curSor = ImageManager::GetSingleton()->AddImage("cursor", "Image/Katana/effect/cursor.bmp", 50, 50, true, RGB(255, 0, 255));

	player = new Player();
	player->Init();

	boss = new Boss;
	boss->Init(1100,400);
	boss->GetData()->target = player;
	Camera::GetSingleton()->SetWorld(1344, 784);

	ShowCursor(false);

	return S_OK;
}

void BossScene::Release()
{

}

void BossScene::Update()
{
	boss->Update();
	player->Update();
}

void BossScene::Render(HDC hdc)
{
	Camera::GetSingleton()->Render(hdc, "stage5_bg_render");
	Camera::GetSingleton()->View();
	
	//if (missileManager)
	//	missileManager->Render(hdc);
	//
	//if (enemyManager)
	//	enemyManager->Render(hdc);
	//
	//if (installObj)
	//	installObj->Render(hdc);
	boss->Render(hdc,false);

	if (player)
		player->Render(hdc);

	if (curSor)
		curSor->Render(hdc, g_ptMouse.x, g_ptMouse.y, true);
}

