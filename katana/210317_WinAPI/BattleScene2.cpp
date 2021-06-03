#include "BattleScene2.h"
#include "BattleScene.h"
#include "CommonFunction.h"
#include "Missile.h"
#include "MissileManager.h"
#include "Image.h"
#include "Player.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Enemy_Bold.h"
#include "Enemy_pomp.h"
#include "Enemy_Grunt.h"
#include "Enemy_Cop.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "AstarManager.h"
#include "InstallObject.h"
#include "Boss.h"
#include "UI.h"
#include "CommonEffect.h"

HRESULT BattleScene2::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	curSor = ImageManager::GetSingleton()->AddImage("cursor", "Image/Katana/effect/cursor.bmp", 50, 50, true, RGB(255, 0, 255));
	player = new Player();
	player->Init();
	player->SetPos(1344 / 2, 1424 / 2);
	ui = new UI();
	ui->Init();
	player->SetUI(ui);

	boss = new Boss;
	boss->Init(1100,900);
	boss->GetData()->target = player;
	boss->GetData()->missileManager = missileManager;
	boss->GetData()->stage = 4;

	enemyManager = new EnemyManager;
	enemyManager->RegisterClone("PompEnemy", new Enemy_pomp);
	enemyManager->RegisterClone("BoldEnemy", new Enemy_Bold);
	enemyManager->RegisterClone("GruntEnemy", new Enemy_Grunt);
	enemyManager->RegisterClone("CopEnemy", new Enemy_Cop);

	enemyManager->AddEnemy("PompEnemy",1);
	enemyManager->Init(player,510,900,0);

	enemyManager->AddEnemy("BoldEnemy", 1);
	enemyManager->Init(player,220,900,1);

	enemyManager->AddEnemy("CopEnemy", 1);
	enemyManager->Init(player, 865, 900, 2);

	enemyManager->AddEnemy("GruntEnemy", 1);
	enemyManager->Init(player, 1120, 900, 3);

	enemyManager->AddEnemy("GruntEnemy", 1);
	enemyManager->Init(player, 865, 900, 4);

	enemyManager->AddEnemy("BoldEnemy", 1);
	enemyManager->Init(player, 220, 900, 5);

	flameEffect = new CommonEffect[50];

	for (int i = 0; i < 50; i++) 
	{
		flameEffect[i].Init(0,600);
	}

	collisionManager = new CollisionManager;

	//문 1572 2886
	Camera::GetSingleton()->SetWorld(1344, 1424);

	missileManager = new MissileManager;
	missileManager->Init();

	boss->GetData()->missileManager = nullptr;

	for (int i = 0; i < enemyManager->GetMonsterList().size(); i++) 
	{
		enemyManager->GetMonsterList()[i]->GetData()->missileManager = nullptr;
		enemyManager->GetMonsterList()[i]->GetData()->isRender = false;
	}

	bgPos.x = 0;
	bgPos.y = 0;

	ShowCursor(false);

	return S_OK;
}

void BattleScene2::Release()
{

}

void BattleScene2::Update()
{

	for (int i = 0; i < 50; i++)
	{
		flameEffect[i].Update();
	}

	if (boss)
		boss->Update();
	if (boss->GetData()->isHit && chagnePage == false)
	{
		chagnePage = true;
		page++;
		monsterCount = 3;
	}
	switch (page)
	{
	case 1:
		enemyManager->GetMonsterList()[0]->GetData()->isRender = true;
		enemyManager->GetMonsterList()[1]->GetData()->isRender = true;
		enemyManager->GetMonsterList()[4]->GetData()->isRender = true;
		enemyManager->GetMonsterList()[0]->GetData()->isalhpa = true;
		enemyManager->GetMonsterList()[1]->GetData()->isalhpa = true;
		enemyManager->GetMonsterList()[4]->GetData()->isalhpa = true;
		break;
	case 2:
		enemyManager->GetMonsterList()[2]->GetData()->isRender = true;
		enemyManager->GetMonsterList()[3]->GetData()->isRender = true;
		enemyManager->GetMonsterList()[5]->GetData()->isRender = true;
		enemyManager->GetMonsterList()[2]->GetData()->isalhpa = true;
		enemyManager->GetMonsterList()[3]->GetData()->isalhpa = true;
		enemyManager->GetMonsterList()[5]->GetData()->isalhpa = true;
		break;
	case 3:
		flameCooltime +=10 *TimerManager::GetSingleton()->GetElapsedTime();
		if (flameCooltime > 1) 
		{
			flameCooltime = 0;
			flameEffect[flameIndex].SetAlive(true);
			flameEffect[flameIndex].SetPos(flameIndex * 100, 900);
			flameIndex++;
		}
		break;
	}

	Camera::GetSingleton()->View();
	if (missileManager)
		missileManager->Update();
	if (enemyManager)
		enemyManager->Update();

	if (player)
		player->Update();
	if (ui)
		ui->Update();
	
	if (page>0 && monsterCount == 0)
	{
		boss->GetData()->isAlive = true;
		chagnePage = false;
	}

	for (int i = 0; i < enemyManager->GetMonsterList().size(); i++)
	{
		collisionManager->MissilePlayerEnemy(missileManager, player, enemyManager, this, i);
		collisionManager->EnemyPlayer(enemyManager, player, this, i);
	}

	if (collisionManager)
		collisionManager->BossPlayer(player, boss);

	if (player->Getpos().y >1400)
	{
		SceneManager::GetSingleton()->ChangeScene("보스전투");
	}
}

void BattleScene2::Render(HDC hdc)
{
	Camera::GetSingleton()->Render(hdc, "stage4_bg_render");


	for (int i = 0; i < 50; i++)
	{
		flameEffect[i].Render(hdc);
	}
	

	if (missileManager)
		missileManager->Render(hdc);

	if (enemyManager)
		enemyManager->Render(hdc);
	if (boss)
		boss->Render(hdc,false);
	if (curSor)
		curSor->Render(hdc, g_ptMouse.x, g_ptMouse.y, true);
	if (player)
		player->Render(hdc);
	if (ui)
		ui->Render(hdc);
}
