#pragma once
#include "GameNode.h"
class Enemy;
class Image;
class MainGame : public GameNode
{
private:
	bool isInited;

	HDC hdc;
	PAINTSTRUCT ps;
	HANDLE hTimer;
	POINT ptMouse{ 0, 0 };
	char szText[128] = "";

	Image* backBuffer;
	Image* bin;

	int enemyCount;
	Enemy* enemy;


public:
	HRESULT Init();		// 오버라이딩 : 다형성
	void Release();
	void Update();
	void Render(HDC hdc);

	//void CheckCollision();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();
};

