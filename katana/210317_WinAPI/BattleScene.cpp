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
#include "AstarManager.h"
#include "CollisionManager.h"
#include "ItemManager.h"
#include "AstarManager.h"
#include "InstallObject.h"

HRESULT BattleScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	curSor = ImageManager::GetSingleton()->AddImage("cursor", "Image/Katana/effect/cursor.bmp", 50, 50, true,RGB(255,0,255));
	player = new Player();
	player->Init();

	itemManager = new ItemManager;
	itemManager->AddItem("Knife",new Item);
	itemManager->Init(WINSIZE_X/2-100, WINSIZE_Y / 2,player);
	itemManager->AddItem("Oil", new Item);
	itemManager->Init(WINSIZE_X / 2 - 80, WINSIZE_Y / 2, player);
	itemManager->AddItem("bottle", new Item);
	itemManager->Init(WINSIZE_X / 2 - 60, WINSIZE_Y / 2, player);
	itemManager->AddItem("Bust", new Item);
	itemManager->Init(WINSIZE_X / 2 - 40, WINSIZE_Y / 2, player);

	installObj = new InstallObject;
	installObj->Init(1600, 2880, InstallType::Door);

	player->SetitemManager(itemManager);

	enemyManager = new EnemyManager;
	enemyManager->RegisterClone("PompEnemy", new Enemy_pomp);
	enemyManager->RegisterClone("BoldEnemy", new Enemy_Bold);
	enemyManager->RegisterClone("GruntEnemy", new Enemy_Grunt);
	enemyManager->RegisterClone("CopEnemy", new Enemy_Cop);

	collisionManager = new CollisionManager;

	//문 1572 2886

	missileManager = new MissileManager;
	missileManager->Init();

	MapLoad(1);

	for (int i = 0; i < enemyManager->GetMonsterList().size(); i++)
	{
		enemyManager->GetMonsterList()[i]->GetData()->target = player;
		enemyManager->GetMonsterList()[i]->GetData()->astar->SetOnwer(enemyManager->GetMonsterList()[i]);
		enemyManager->GetMonsterList()[i]->GetData()->astar->SetTarget(player);
		enemyManager->GetMonsterList()[i]->GetData()->missileManager = missileManager;
		for (int j = 0; j < TILE_Y; j++)
		{
			for (int k = 0; k < TILE_X; k++)
			{
				if (tileInfo[j * TILE_X + k].type == TileType::Wall)
					enemyManager->GetMonsterList()[i]->GetData()->astar->SetWall(j, k);
			}
		}
	}

	bgPos.x = 0;
	bgPos.y = 0;

	ShowCursor(false);

	return S_OK;
}

void BattleScene::Release()
{
	SAFE_RELEASE(player);
	SAFE_RELEASE(enemyManager);
	SAFE_RELEASE(itemManager);
}

void BattleScene::Update()
{
	if (installObj)
		installObj->Update();
	if(missileManager)
		missileManager->Update();
	if (enemyManager)
		enemyManager->Update();
	if (itemManager)
		itemManager->Update();
	if (player)
		player->Update();

	for (int i = 0; i < enemyManager->GetMonsterList().size(); i++) 
	{
		collisionManager->MissilePlayerEnemy(missileManager,player, enemyManager,i);
		collisionManager->EnemyPlayer(enemyManager, player, i);
		collisionManager->EnemyItem(player, enemyManager, itemManager, i);
		collisionManager->PlayerDoorEnemy(player, installObj, enemyManager, i);
	}
}


void BattleScene::Render(HDC hdc)
{
	Camera::GetSingleton()->Render(hdc,"stage1_bg_render");
	Camera::GetSingleton()->View();

	if (itemManager)
		itemManager->Render(hdc);

	if (missileManager)
		missileManager->Render(hdc);

	if (enemyManager)
		enemyManager->Render(hdc);

	if (installObj)
		installObj->Render(hdc);

	if (player)
		player->Render(hdc);
	
	FPOINT pos;
	pos.x = WINSIZE_X - Camera::GetSingleton()->GetCameraPos().x;
	pos.y = WINSIZE_Y - Camera::GetSingleton()->GetCameraPos().y;
	if (curSor)
		curSor->Render(hdc, g_ptMouse.x, g_ptMouse.y, true);
	sprintf_s(szText, "playerX : %f , playerY : %f", player->GetWorldpos().x, player->GetWorldpos().y);
	TextOut(hdc, WINSIZE_X - 800, 20, szText, strlen(szText));
	sprintf_s(szText, "angle : %f", player->GetPlayerAngle());
	TextOut(hdc, WINSIZE_X - 400, 20, szText, strlen(szText));
	sprintf_s(szText, "X : %d, Y : %d", g_ptMouse.x, g_ptMouse.y);
	TextOut(hdc, 200, 20, szText, strlen(szText));
}


void BattleScene::MapLoad(int stageNum)
{
	string fileName = "Save/saveMapData";  // 1.map";
	fileName += to_string(stageNum) + ".map";
	string fileName1 = "Save/saveMapDataTile";  // 1.map";
	fileName1 += to_string(stageNum) + ".map";
	DWORD readBytes[2];
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0,
		0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	HANDLE hFile2 = CreateFile(fileName1.c_str(), GENERIC_READ, 0,
		0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, enemySize, sizeof(ENMY_INFO) * 100,
		&readBytes[0], NULL))
	{
		for (int i = 0; i < 100; i++)
		{
			string str(enemySize[i].Name);
			if (str == "")
				break;
			enemyManager->AddEnemy(str, 1);
			enemyManager->Init(nullptr, enemySize[i].x, enemySize[i].y, enemySize[i].index);
		}
	}
	else
	{
		MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
	}

	if (ReadFile(hFile2, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y,
		&readBytes[1], NULL))
	{
		for (int i = 0; i < TILE_Y; i++)
		{
			for (int j = 0; j < TILE_X; j++)
			{
				if (tileInfo[i * TILE_X + j].color)
					tileInfo[i * TILE_X + j].hBrush = CreateSolidBrush(tileInfo[i * TILE_X + j].color);
			}
		}
	}
	else
	{
		MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
	}

	CloseHandle(hFile2);
	CloseHandle(hFile);
}

