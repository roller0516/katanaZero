#include "BattleScene.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "CommonFunction.h"
#include "Missile.h"
#include "Image.h"
#include "Player.h"
#include "Camera.h"
#include "CollisionManager.h"
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

	player->SetitemManager(itemManager);

	astarManager = new AstarManager;
	astarManager->Init();
	astarManager->SetTarget(player);

	enemyManager = new EnemyManager;
	enemyManager->RegisterClone("PompEnemy", new Enemy_pomp);
	enemyManager->RegisterClone("BoldEnemy", new Enemy_Bold);
	enemyManager->RegisterClone("GruntEnemy", new Enemy_Grunt);
	enemyManager->RegisterClone("CopEnemy", new Enemy_Cop);

	collisionManager = new CollisionManager;
	enemyManager->AddEnemy("BoldEnemy",1);
	enemyManager->Init(player,500, 500, 0);

	MapLoad(1);

	for (int i = 0; i < enemyManager->GetMonsterList().size(); i++)
	{
		enemyManager->GetMonsterList()[i]->GetData()->astar = astarManager;
		enemyManager->GetMonsterList()[i]->GetData()->astar->SetOnwer(enemyManager->GetMonsterList()[i]);
		astarManager->AddAtsar(i);
	}

	for (int j = 0; j < TILE_Y; j++)
	{
		for (int k = 0; k < TILE_X; k++)
		{
			if (tileInfo[j * TILE_X + k].type == TileType::Wall)
				astarManager->SetWall(j, k);
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
	SAFE_RELEASE(astarManager);
}

void BattleScene::Update()
{
	if (astarManager)
		astarManager->Update();
	if (enemyManager)
		enemyManager->Update();

	if (itemManager)
		itemManager->Update();

	if (player)
	{
		player->Update();
	}
	
	for (int i = 0; i < enemyManager->GetMonsterList().size(); i++) 
	{
		collisionManager->MissilePlayerEnemy(enemyManager->GetMonsterList()[i]->GetMissileManager(),player, enemyManager,i);
		collisionManager->EnemyPlayer(enemyManager, player, i);
		collisionManager->EnemyItem(player, enemyManager, itemManager, i);
	}
}


void BattleScene::Render(HDC hdc)
{
	Camera::GetSingleton()->Render(hdc);
	Camera::GetSingleton()->View();

	if (itemManager)
		itemManager->Render(hdc);

	if (astarManager)
		astarManager->Render(hdc);

	if (enemyManager)
		enemyManager->Render(hdc);

	if (player)
		player->Render(hdc);

	//for (int i = 0; i < TILE_X * TILE_Y; i++)
	//{
	//	if (tileInfo[i].rcTile.left - Camera::GetSingleton()->GetCameraPos().x > WINSIZE_X)
	//		continue;
	//	if (tileInfo[i].rcTile.top - Camera::GetSingleton()->GetCameraPos().y > WINSIZE_Y)
	//		continue;
	//	tileInfo[i].hOldBrush = (HBRUSH)SelectObject(hdc, tileInfo[i].hBrush);
	//	Rectangle(hdc,
	//		tileInfo[i].rcTile.left - Camera::GetSingleton()->GetCameraPos().x,
	//		tileInfo[i].rcTile.top - Camera::GetSingleton()->GetCameraPos().y,
	//		tileInfo[i].rcTile.right - Camera::GetSingleton()->GetCameraPos().x,
	//		tileInfo[i].rcTile.bottom - Camera::GetSingleton()->GetCameraPos().y);
	//	SelectObject(hdc, tileInfo[i].hOldBrush);
	//}

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
	//string fileName = "Save/saveMapData";  // 1.map";
	//fileName += to_string(stageNum) + ".map";
	string fileName1 = "Save/saveMapDataTile";  // 1.map";
	fileName1 += to_string(stageNum) + ".map";
	DWORD readBytes[2];
	//HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0,
	//	0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	HANDLE hFile2 = CreateFile(fileName1.c_str(), GENERIC_READ, 0,
		0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	/*if (ReadFile(hFile, enemySize, sizeof(ENMY_INFO) * 100,
		&readBytes[0], NULL))
	{
		for (int i = 0; i < 100; i++)
		{
			string str(enemySize[i].Name);
			if (enemySize[i].Name == "")
				break;
			enemyManager->AddEnemy(str, 1);
			enemyManager->Init(nullptr, enemySize[i].x, enemySize[i].y, enemySize[i].index);
		}
	}
	else
	{
		MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
	}*/

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
	//CloseHandle(hFile);
}

