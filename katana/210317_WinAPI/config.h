// config.h

#pragma once
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "msimg32.lib")

#include <Windows.h>
#include <string>
#include <ctime>
#include <bitset>
#include <map>
#include <vector>
#include <list>

using namespace std;


#include "KeyManager.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "SceneManager.h"


#define WINSIZE_X	1280
#define WINSIZE_Y	720
#define TILEMAPTOOLSIZE_X	1620
#define TILEMAPTOOLSIZE_Y	900
#define ASTARSIZE_X	1620
#define ASTARSIZE_Y	900
// 메인 그리기 공간 정보
#define	TILE_X			150
#define	TILE_Y			150
#define TILESIZE		20
#define TILE_COUNT		TILE_X*TILE_Y

#define Gravity			9.8f
#define PI			3.14159274f
#define DegToRad(x)	((x) * PI / 180.0f)
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { p->Release(); delete p; p = nullptr; } }

enum class TileType { Start, End, Wall, None };

typedef struct tagFPoint
{
	float x;
	float y;

} FPOINT, *PFPOINT;

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
	TileType type;
	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hOldBrush;
} TILE_INFO;

typedef struct ENMY_INFO
{
	char Name[256] = "";
	int x;
	int y;
	int index;
	ENMY_INFO()
	{
		x = 0;
		y = 0;
		index = 0;
	}
};
//typedef tagFPoint FPOINT2;
//typedef tagFPoint* PFPOINT2;

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;