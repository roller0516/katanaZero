#include "MainGame.h"
#include "Image.h"
#include "TilemapTool.h"
#include "BattleScene.h"
#include "AstarScene.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);

	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();

	int maxWidth, maxHeight;
	maxWidth = max(WINSIZE_X, TILEMAPTOOLSIZE_X);
	maxHeight = max(WINSIZE_Y, TILEMAPTOOLSIZE_Y);

	backBuffer = new Image();
	backBuffer->Init(maxWidth, maxHeight);

	SceneManager::GetSingleton()->AddScene("ÀüÅõ_1", new BattleScene());
	SceneManager::GetSingleton()->AddScene("Å¸ÀÏ¸ÊÅø", new TilemapTool());
	SceneManager::GetSingleton()->AddScene("A½ºÅ¸", new AstarScene());

	SceneManager::GetSingleton()->ChangeScene("Å¸ÀÏ¸ÊÅø");

	isInited = true;

	return S_OK;
}

void MainGame::Release()
{
	KeyManager::GetSingleton()->Release();
	ImageManager::GetSingleton()->Release();
	SceneManager::GetSingleton()->Release();

	SAFE_RELEASE(backBuffer);

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F10))
		SceneManager::GetSingleton()->ChangeScene("Å¸ÀÏ¸ÊÅø");
}

void MainGame::Render()
{
	HDC hBackDC = backBuffer->GetMemDC();

	SceneManager::GetSingleton()->Render(hBackDC);

	// ÀÎ»ç
	TextOut(hBackDC, 20, 20, "MainGame ·»´õ Áß", strlen("MainGame ·»´õ Áß"));
	// ¸¶¿ì½º ÁÂÇ¥

	// FPS
	TimerManager::GetSingleton()->Render(hBackDC);

	backBuffer->Render(hdc);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		break;
	case WM_LBUTTONDOWN:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
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
