#include "BossScene.h"
#include "Player.h"
#include "Boss.h"
#include "Enemy.h"
#include "Camera.h"
#include "Image.h"
#include "CollisionManager.h"
#include "MissileManager.h"
#include "UI.h"
HRESULT BossScene::Init()
{
	curSor = ImageManager::GetSingleton()->AddImage("cursor", "Image/Katana/effect/cursor.bmp", 50, 50, true, RGB(255, 0, 255));
	ui = new UI;
	ui->Init();
	player = new Player();
	player->Init();
	player->SetUI(ui);
	collisionManager = new CollisionManager;
	missileManager = new MissileManager;
	missileManager->Init();
	boss = new Boss;
	boss->Init(1000,600);
	boss->GetData()->target = player;
	boss->GetData()->missileManager = missileManager;
	boss->GetData()->stage = 5;
	Camera::GetSingleton()->SetWorld(1344, 784);

	ShowCursor(false);

	return S_OK;
}

void BossScene::Release()
{

}

void BossScene::Update()
{
	Camera::GetSingleton()->View();

	if (ui)
		ui->Update();
	if(boss)
		boss->Update();
	if(player)
		player->Update();
	if (missileManager)
		missileManager->Update();
	if(collisionManager)
		collisionManager->BossPlayer(player, boss);
}

void BossScene::Render(HDC hdc)
{
	Camera::GetSingleton()->Render(hdc, "stage5_bg_render");
	
	if (missileManager)
		missileManager->Render(hdc);
	if (ui)
		ui->Render(hdc);
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

