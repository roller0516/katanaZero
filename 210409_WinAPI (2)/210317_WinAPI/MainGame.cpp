#include "MainGame.h"
#include "Tank.h"
#include "Enemy.h"
#include "Missile.h"
#include "Image.h"
#include "Iori.h"
HRESULT MainGame::Init()
{
	KeyManager::GetSingleton()->Init();

	// 메인게임의 초기화 함수
	hTimer = (HANDLE)SetTimer(g_hWnd, 0, 10, NULL);

	// 백버퍼 이미지
	backBuffer = new Image();
	backBuffer->Init(WINSIZE_X, WINSIZE_Y);

	bin = new Image();
	bin->Init("Image/background.bmp", WINSIZE_X, WINSIZE_Y);

	// 랜덤한 위치로 셋팅
	srand(time(NULL));
	FPOINT randPos;

	enemy = new Enemy();

	enemy->Init();
	randPos.x = WINSIZE_X /2;
	randPos.y = WINSIZE_Y/2;
	enemy->SetPos(randPos);

	//randPos.x = 50;
	//randPos.y = 50;
	//enemy[1].SetPos(randPos);

	//randPos.x = 50;
	//randPos.y = WINSIZE_Y - 150;
	//enemy[2].SetPos(randPos);

	//randPos.x = WINSIZE_X - 50;
	//randPos.y = WINSIZE_Y - 150;
	//enemy[3].SetPos(randPos);


	isInited = true;

	return S_OK;
}

void MainGame::Release()
{
	KeyManager::GetSingleton()->Release();

	backBuffer->Release();
	delete backBuffer;
	backBuffer = nullptr;

	bin->Release();
	delete bin;
	bin = nullptr;


	enemy->Release();
	
	delete enemy;
	enemy = nullptr;

	KillTimer(g_hWnd, 0);
}

void MainGame::Update()
{
	enemy->Update();
	/*if (enemy)
	{
		for (int i = 0; i < enemyCount; i++)
		{
			enemy[i].Update();
		}
	}*/

	//CheckCollision();

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render(HDC hdc)
{
	HDC hBackDC = backBuffer->GetMemDC();

	if (bin)
	{
		bin->Render(hBackDC/*, -100, 100*/);
	}

	// 인사
	TextOut(hBackDC, 20, 20, "MainGame 렌더 중", strlen("MainGame 렌더 중"));
	// 마우스 좌표
	wsprintf(szText, "X : %d, Y : %d", ptMouse.x, ptMouse.y);
	TextOut(hBackDC, 200, 20, szText, strlen(szText));

	enemy->Render(hBackDC);
	/*if (enemy)
	{
		for (int i = 0; i < enemyCount; i++)
		{
			enemy[i].Render(hBackDC);
		}
	}*/

	backBuffer->Render(hdc);
}

//void MainGame::CheckCollision()
//{
//	// 적 <-> 탱크 미사일 
//	float distance;
//	FPOINT enemyPos;
//	FPOINT missilePos;
//	float x, y;
//	int r1, r2;
//
//	for (int i = 0; i < enemyCount; i++)
//	{
//		if (enemy[i].GetIsAlive() == false)	continue;
//
//		for (int j = 0; j < tank->GetMissileCount(); j++)
//		{
//			if (missileArray[j].GetIsFired() == false)	continue;
//
//			enemyPos = enemy[i].GetPos();
//			missilePos = missileArray[j].GetPos();
//
//			x = enemyPos.x - missilePos.x;
//			y = enemyPos.y - missilePos.y;
//
//			distance = sqrtf(x * x + y * y);
//
//			r1 = enemy[i].GetSize() / 2;
//			r2 = missileArray[j].GetSize() / 2;
//
//			if (distance <= r1 + r2)
//			{
//				enemy[i].SetIsAlive(false);
//				missileArray[j].SetIsFired(false);
//				break;
//			}
//		}
//	}
//
//	// 적 <-> 탱크
//
//	// 적 미사일 <-> 탱크
//
//	// 적 미사일 <-> 탱크 미사일
//}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		break;
	case WM_TIMER:
		if (isInited)
		{
			this->Update();
		}
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);

		if (isInited)
		{
			this->Render(hdc);
		}

		EndPaint(g_hWnd, &ps);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
	isInited = false;
}

MainGame::~MainGame()
{
}
